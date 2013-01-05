/*	NAME:
		NMacSystem.cpp

	DESCRIPTION:
		Mac system support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/select.h>
#include <sys/fcntl.h>
#include <sysexits.h>

#include "NCoreFoundation.h"
#include "NTimeUtilities.h"
#include "NThread.h"
#include "NLock.h"
#include "NCFString.h"
#include "NMacTarget.h"
#include "NTargetSystem.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tasks
static const UInt32 kBufferSize											= 2048;

static const NIndex kPipeParent											= 0;
static const NIndex kPipeChild											= 1;

static const int FD_STDIN												= 0;
static const int FD_STDOUT												= 1;
static const int FD_STDERR												= 2;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//      ExecuteTask : Execute a task.
//----------------------------------------------------------------------------
static void ExecuteTask(const NString &theCmd, const NString &cmdName, const NStringList &theArgs)
{	std::vector<const char *>		argList;
	NStringListConstIterator		theIter;
	int								sysErr;



	// Build the argument list
	argList.push_back(cmdName.GetUTF8());
	
	for (theIter = theArgs.begin(); theIter != theArgs.end(); theIter++)
		argList.push_back(theIter->GetUTF8());
	
	argList.push_back(NULL);



	// Execute the task
	sysErr = execv(theCmd.GetUTF8(), const_cast<char* const*>(&argList[0]));
	NN_ASSERT(sysErr == 0);

	_exit(EXIT_FAILURE);
}





//============================================================================
//		ClosePipe : Close a pipe.
//----------------------------------------------------------------------------
static void ClosePipe(NTaskPipeRef &thePipe)
{


	// Close the pipe
	if (thePipe != kNTaskPipeRefNone)
		{
		close(thePipe);
		thePipe = kNTaskPipeRefNone;
		}
}





//============================================================================
//		ReadPipe : Read from a pipe.
//----------------------------------------------------------------------------
static NString ReadPipe(NTaskPipeRef &thePipe, bool lastRead)
{	char		theBuffer[kBufferSize];
	NString		theString;
	int			numRead;



	// Check the pipe
	if (thePipe == kNTaskPipeRefNone)
		return(theString);



	// Read from the pipe
	do
		{
		numRead = read(thePipe, theBuffer, kBufferSize);
		if (numRead >= 1)
			theString += NString(theBuffer, numRead);
		}
	while (numRead >= 1);



	// Clean up
	//
	// If this was the last read from the pipe, close it when done.
	if (lastRead && numRead == 0)
		ClosePipe(thePipe);

	return(theString);
}





//============================================================================
//		WritePipe : Write to a pipe.
//----------------------------------------------------------------------------
static void WritePipe(NTaskPipeRef thePipe, const NString &theText)
{	NData		theData;



	// Check the pipe
	if (thePipe == kNTaskPipeRefNone)
		return;
	
	
	
	// Write to the pipe
	theData = theText.GetData();
	write(thePipe, theData.GetData(), theData.GetSize());
}





//============================================================================
//		DoNotification : Handle a notification.
//----------------------------------------------------------------------------
static void DoNotification(CFNotificationCenterRef		/*cfCenter*/,
							void						*theObserver,
							CFStringRef					cfName,
							const void					*/*theObject*/,
							CFDictionaryRef				/*cfInfo*/)
{	NBroadcaster		*theBroadcaster = (NBroadcaster *) theObserver;



	// Handle the notification
	if (CFStringCompare(kCFLocaleCurrentLocaleDidChangeNotification, cfName, 0) == kCFCompareEqualTo)
		theBroadcaster->BroadcastMessage(kMsgNLocaleModified, &kNLocaleChangedAllKey);
}





//============================================================================
//		GestaltSInt32 : Get a gestalt value.
//----------------------------------------------------------------------------
#if NN_TARGET_MAC
static SInt32 GestaltSInt32(OSType theSelector)
{	SInt32		theValue;
	OSStatus	theErr;



	// Get the value
	theErr = Gestalt(theSelector, &theValue);
	if (theErr != noErr)
		theValue = 0;
	
	return(theValue);
}
#endif // NN_TARGET_MAC





//============================================================================
//      NTargetSystem::DebugLog : Emit a debug message.
//----------------------------------------------------------------------------
#pragma mark -
void NTargetSystem::DebugLog(const char *theMsg)
{


	// Log the message
	fprintf(stderr, "%s", theMsg);
}





//============================================================================
//      NTargetSystem::FindBundle : Find a bundle.
//----------------------------------------------------------------------------
NFile NTargetSystem::FindBundle(const NString &bundleID)
{	NCFObject		cfBundle, cfURL;
	NCFString		cfString;
	NFile			theFile;



	// Get the bundle
	if (bundleID.IsEmpty())
		cfBundle.SetObject(CFBundleGetMainBundle(), false);
	else
		cfBundle.SetObject(CFBundleGetBundleWithIdentifier(ToCF(bundleID)), false);

	if (!cfBundle.IsValid())
		return(theFile);



	// Get the file
	if (cfURL.SetObject(CFBundleCopyBundleURL(cfBundle)))
		{
		cfString.SetObject(CFURLCopyFileSystemPath(cfURL, kCFURLPOSIXPathStyle));
		theFile = NFile(cfString);
		}
	
	return(theFile);
}





//============================================================================
//      NTargetSystem::GetOSVersion : Get the OS version.
//----------------------------------------------------------------------------
OSVersion NTargetSystem::GetOSVersion(void)
{	SInt32		verTarget, verMajor, verMinor, verBugFix;
	OSVersion	theVers;



	// Get the version
#if NN_TARGET_MAC
	verTarget = (SInt32) kOSMac;
	verMajor  = GestaltSInt32(gestaltSystemVersionMajor);
	verMinor  = GestaltSInt32(gestaltSystemVersionMinor);
	verBugFix = GestaltSInt32(gestaltSystemVersionBugFix);
	
	verMajor <<= 16;
	verMinor <<=  8;

	theVers = (OSVersion) (verTarget | verMajor | verMinor | verBugFix);


#elif NN_TARGET_IOS
	StAutoReleasePool		autoRelease;
	NSArray					*theArray;
	NIndex					numParts;
	
	theArray = [[[UIDevice currentDevice] systemVersion] componentsSeparatedByString:@"."];
	numParts = [theArray count];

	verTarget  = (SInt32) kOSiOS;
    verMajor   = (numParts >= 1) ? [[theArray objectAtIndex:0] intValue] : 0;
	verMinor   = (numParts >= 2) ? [[theArray objectAtIndex:1] intValue] : 0;
	verBugFix  = (numParts >= 3) ? [[theArray objectAtIndex:2] intValue] : 0;

	theVers = (OSVersion) (verTarget | verMajor | verMinor | verBugFix);


#else
	UNKNOWN TARGET

#endif

	return(theVers);
}





//============================================================================
//		NTargetSystem::GetOSName : Get the OS name.
//----------------------------------------------------------------------------
NString NTargetSystem::GetOSName(void)
{	SInt32			verMajor, verMinor, verBugFix;
	OSVersion		theVersion;
	NString			theResult;



	// Get the state we need
	theVersion = GetOSVersion();

	verMajor  = ((theVersion >> 16) & 0xFF);
	verMinor  = ((theVersion >>  8) & 0xFF);
	verBugFix = ((theVersion >>  0) & 0xFF);



	// Get the name
	if (verBugFix == 0)
		theResult.Format("%ld.%ld",     verMajor, verMinor);
	else
		theResult.Format("%ld.%ld.%ld", verMajor, verMinor, verBugFix);



	// Get the name
#if NN_TARGET_MAC
	theResult.Format("Mac OS %@", theResult);

#elif NN_TARGET_IOS
	theResult.Format("iOS %@", theResult);

#else
	UNKNOWN TARGET
#endif
	
	return(theResult);
}





//============================================================================
//		NTargetSystem::GetSystemCPU : Get the clock speed.
//----------------------------------------------------------------------------
UInt64 NTargetSystem::GetSystemCPU(void)
{	UInt32		theResult;



	// Get the clock speed
	theResult = NMacTarget::GetSysctl<UInt32>(CTL_HW, HW_CPU_FREQ);
	NN_ASSERT(theResult > 0);
	
	return(theResult);
}





//============================================================================
//		NTargetSystem::GetSystemRAM : Get the physical memory.
//----------------------------------------------------------------------------
UInt64 NTargetSystem::GetSystemRAM(void)
{	uint64_t	theResult;



	// Get the physical memory
	theResult = NMacTarget::GetSysctl<UInt64>(CTL_HW, HW_MEMSIZE);
	NN_ASSERT(theResult > 0);
	
	return(theResult);
}





//============================================================================
//		NTargetSystem::GetSystemArch : Get the system architecture.
//----------------------------------------------------------------------------
NString NTargetSystem::GetSystemArch(void)
{	NString		theArch;



	// Get the architecture
#if NN_TARGET_MAC
	#if defined(__ppc__) || defined(__ppc64__)
		theArch = "PPC";
	#elif defined(__i386__) || defined(__x86_64__)
		theArch = "x86";
	#else
		UNKNOWN ARCH
	#endif

#elif NN_TARGET_IOS
	#if defined(__arm__)
		theArch = "ARM";
	#elif defined(__i386__)
		theArch = "x86";
	#else
		UNKNOWN ARCH
	#endif
	
#else
	UNKNOWN TARGET
#endif

	return(theArch);
}





//============================================================================
//      NTargetSystem::TaskCreate : Create a task.
//----------------------------------------------------------------------------
TaskInfo NTargetSystem::TaskCreate(const NString &theCmd, const NStringList &theArgs)
{	int				pipeStdIn[2], pipeStdOut[2], pipeStdErr[2];
	TaskInfo		theTask;
	NString			cmdName;



	// Get the state we need
	//
	// Mac OS X 10.6 does not allow the use of the File Manager between a fork()
	// and an exec(), so we need to fetch the name of the command up front.
	//
	//		http://www.lyx.org/trac/ticket/6168
	//
	//		USING_FORK_WITHOUT_EXEC_IS_NOT_SUPPORTED_BY_FILE_MANAGER
	//
	theTask.taskID     = kNTaskIDNone;
	theTask.taskResult = 0;
	
	theTask.stdIn  = kNTaskPipeRefNone;
	theTask.stdOut = kNTaskPipeRefNone;
	theTask.stdErr = kNTaskPipeRefNone;

	cmdName = NFile(theCmd).GetName();



	// Create the pipes
	if (pipe(pipeStdIn)  == -1 ||
		pipe(pipeStdOut) == -1 ||
		pipe(pipeStdErr) == -1)
		return(theTask);



	// Fork the process
	theTask.taskID = fork();
	
	switch ((SInt32) theTask.taskID) {
		case -1:
			// Handle failure
			return(theTask);
			break;

		case 0:
			// We're the child
			close(FD_STDIN);
			close(FD_STDOUT);
			close(FD_STDERR);

			dup2(pipeStdIn [kPipeChild], FD_STDIN);
			dup2(pipeStdOut[kPipeChild], FD_STDOUT);
			dup2(pipeStdErr[kPipeChild], FD_STDERR);
			
			ExecuteTask(theCmd, cmdName, theArgs);
			_exit(1);
			break;

		default:
			// We're the parent:
			close(pipeStdIn [kPipeChild]);
			close(pipeStdOut[kPipeChild]);
			close(pipeStdErr[kPipeChild]);
			
			theTask.stdIn  = (NTaskPipeRef) pipeStdIn [kPipeParent];
			theTask.stdOut = (NTaskPipeRef) pipeStdOut[kPipeParent];
			theTask.stdErr = (NTaskPipeRef) pipeStdErr[kPipeParent];
			
			fcntl(theTask.stdOut, F_SETFL, O_NONBLOCK);
			fcntl(theTask.stdErr, F_SETFL, O_NONBLOCK);
			break;
		}
	
	return(theTask);
}





//============================================================================
//      NTargetSystem::TaskDestroy : Destroy a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskDestroy(TaskInfo &theTask)
{


	// Clean up
	theTask.taskID = kNTaskIDNone;
	
	ClosePipe(theTask.stdIn);
	ClosePipe(theTask.stdOut);
	ClosePipe(theTask.stdErr);
}





//============================================================================
//      NTargetSystem::TaskUpdate : Update a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskUpdate(TaskInfo &theTask)
{	int		sysErr, theStatus;



	// Get the child status
	sysErr = wait4(theTask.taskID, &theStatus, WNOHANG, NULL);
	if (sysErr == 0)
		return;

	else if (sysErr == (int) theTask.taskID && WIFEXITED(theStatus))
		theStatus = WEXITSTATUS(theStatus);

	else
		theStatus = EX_SOFTWARE;



	// Update the task
	theTask.taskID     = kNTaskIDNone;
	theTask.taskResult = theStatus;



	// Free our resources
	//
	// The stdout/err pipes are left open since they may contain buffered data
	// that can still be read. However since the child has exited, no more data
	// can be written to it and so the stdin pipe can be closed.
	ClosePipe(theTask.stdIn);
}





//============================================================================
//      NTargetSystem::TaskRead : Read from a task.
//----------------------------------------------------------------------------
NString NTargetSystem::TaskRead(TaskInfo &theTask, bool stdErr)
{	NString		theResult;
	bool		lastRead;
	


	// Read from the task
	lastRead = (theTask.taskID == kNTaskIDNone);
	
	if (stdErr)
		theResult = ReadPipe(theTask.stdErr, lastRead);
	else
		theResult = ReadPipe(theTask.stdOut, lastRead);
	
	return(theResult);
}





//============================================================================
//      NTargetSystem::TaskWrite : Write to a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskWrite(const TaskInfo &theTask, const NString &theText)
{


	// Write to the task
	WritePipe(theTask.stdIn, theText);
}





//============================================================================
//      NTargetSystem::TaskWait : Block for a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskWait(const TaskInfo &/*theTask*/, NTime waitFor)
{	NTime		startTime, endTime, timeLeft;



	// Get the state we need
	startTime = NTimeUtilities::GetTime();
	endTime   = startTime + waitFor;



	// Wait for the task
	//
	// We must allow timers to run while waiting, since they poll the status.
	//
	// Despite passing CFRunLoopRunInMode a non-zero time, on 10.6 it will
	// typically much sooner (forcing us to sleep for the remaining time).
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, waitFor, false);

	timeLeft = endTime - NTimeUtilities::GetTime();
	if (timeLeft > 0.0)
		NThread::Sleep(timeLeft);
}





//============================================================================
//      NTargetSystem::TaskSignal : Signal a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskSignal(const TaskInfo &theTask, NTaskSignal theSignal)
{	int		sysErr;



	// Signal the task
	switch (theSignal) {
		case kTaskKill:
			sysErr = kill(theTask.taskID, SIGTERM);
			NN_ASSERT_NOERR(sysErr);
			break;

		case kTaskInterrupt:
			sysErr = kill(theTask.taskID, SIGINT);
			NN_ASSERT_NOERR(sysErr);
			break;
		
		default:
			NN_LOG("Unknown signal: %d", theSignal);
			break;
		}
}





//============================================================================
//      NTargetSystem::GetLocaleValue : Get a locale value.
//----------------------------------------------------------------------------
NVariant NTargetSystem::GetLocaleValue(const NString &theID, const NString &theKey)
{	NVariant		theResult;
	NCFObject		cfLocale;



	// Validate our parameters
	NN_ASSERT(theID == kNLocaleUser);
	NN_UNUSED(theID);



	// Get the state we need
	if (!cfLocale.SetObject(CFLocaleCopyCurrent()))
		return(theResult);



	// Get the value
	if (theKey == kNLocaleIsMetricKey)
		theResult = (bool) CFBooleanGetValue((CFBooleanRef) CFLocaleGetValue(cfLocale, kCFLocaleUsesMetricSystem));

	else
		NN_LOG("Unknown locale key: %@", theKey);

	return(theResult);
}





//============================================================================
//      NTargetSystem::GetLocaleBroadcaster : Get the locale broadcaster.
//----------------------------------------------------------------------------
NBroadcaster *NTargetSystem::GetLocaleBroadcaster(const NString &/*theID*/)
{	static bool				sRegisteredNotification = false;
	static NBroadcaster		sBroadcaster;
	static NSpinLock		sLock;



	// Register for notificatins
	if (!sRegisteredNotification)
		{
		sLock.Lock();
		
		if (!sRegisteredNotification)
			{
			CFNotificationCenterAddObserver(CFNotificationCenterGetLocalCenter(),
											&sBroadcaster, DoNotification,
											kCFLocaleCurrentLocaleDidChangeNotification, NULL,
											CFNotificationSuspensionBehaviorDeliverImmediately);
					
			sRegisteredNotification = true;
			}
		
		sLock.Unlock();
		}



	// Get the broadcaster
	//
	// For now, all locales share the same broadcaster.
	return(&sBroadcaster);
}





//============================================================================
//      NTargetSystem::TransformString : Transform a string.
//----------------------------------------------------------------------------
NString NTargetSystem::TransformString(const NString &theString, NStringTransform theTransform)
{	NString			theResult;
	CFStringRef		cfString;
	NCFObject		cfResult;
	bool			wasOK;



	// Validate our parameters
	NN_ASSERT(!theString.IsEmpty());
	NN_ASSERT(theTransform & kNStringStripDiacritics);



	// Get the state we need
	theResult = theString;
	cfString  = ToCF(theString);

	if (!cfResult.SetObject(CFStringCreateMutableCopy(kCFAllocatorNano, 0, cfString)))
		return(theResult);



	// Transform the string
	if (theTransform & kNStringStripDiacritics)
		{
		wasOK = CFStringTransform(cfResult, NULL, kCFStringTransformStripDiacritics, false);
		NN_ASSERT(wasOK);
		
		if (wasOK)
			theResult = ToNN((CFStringRef) cfResult);
		}
	
	return(theResult);
}



