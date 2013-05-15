/*	NAME:
		NLinuxSystem.cpp

	DESCRIPTION:
		Linux system support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/select.h>
#include <sys/fcntl.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <errno.h>

#include "NFileUtilities.h"
#include "NTargetSystem.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tasks
static const NIndex kPipeRead											= 0;
static const NIndex kPipeWrite											= 1;

// Misc
static const size_t kBufferSize											= 2 * kNKilobyte;





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
		close((int) thePipe);
		thePipe = kNTaskPipeRefNone;
		}
}





//============================================================================
//		ReadPipe : Read from a pipe.
//----------------------------------------------------------------------------
static NString ReadPipe(NTaskPipeRef &thePipe, bool lastRead)
{	char		theBuffer[kBufferSize];
	NString		theString;
	NIndex		numRead;



	// Check the pipe
	if (thePipe == kNTaskPipeRefNone)
		return(theString);



	// Read from the pipe
	do
		{
		numRead = (NIndex) read((int) thePipe, theBuffer, kBufferSize);
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
{	NData	theData;



	// Check the pipe
	if (thePipe == kNTaskPipeRefNone)
		return;
	
	
	
	// Write to the pipe
	theData = theText.GetData();
	write((int) thePipe, theData.GetData(), theData.GetSize());
}





//============================================================================
//      GetProcInfoFile : Get the contents of a /proc/xxxxinfo file.
//----------------------------------------------------------------------------
static NString GetProcInfoFile(const NString &thePath)
{	char		theBuffer[kBufferSize];
	FILE		*theFile;
	size_t		numRead;
	NString		theText;



	// Read the file
	theFile = fopen(thePath.GetUTF8(), "r");
	if (theFile != NULL)
		{
		numRead = fread(theBuffer, 1, sizeof(theBuffer), theFile);
		if (numRead > 0 && numRead < sizeof(theBuffer))
			theText = NString(theBuffer, numRead);

		fclose(theFile);
		}
	
	return(theText);
}





//============================================================================
//      Public functions
//----------------------------------------------------------------------------
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
{	char		theBuffer[PATH_MAX];
	NFile		theFile, theParent;
	int			theLen;



	// Locate the executable
	if (bundleID.IsEmpty())
		{
		theLen = readlink("/proc/self/exe", theBuffer, sizeof(theBuffer)-1);
		if (theLen > 0)
			theFile = NFile(NString(theBuffer, theLen));
		}
	else
		NN_LOG("NTargetSystem::FindBundle not implemented (%@)", bundleID);



	// Locate the bundle
	//
	// If the executable is within a bundle then we return the root bundle
	// folder, otherwise we return the directory containing the executable.
	theFile = theFile.GetParent();
	if (theFile.GetName() == "Linux")
		{
		theParent = theFile.GetParent();
		if (theParent.GetName() == "Contents")
			theFile = theParent.GetParent();
		}

	return(theFile);
}





//============================================================================
//      NTargetSystem::GetOSVersion : Get the OS version.
//----------------------------------------------------------------------------
OSVersion NTargetSystem::GetOSVersion(void)
{	int					sysErr, versMajor, versMinor;
	struct utsname		theInfo;
	OSVersion			theVers;



	// Get the state we need
	sysErr = uname(&theInfo);
	NN_ASSERT_NOERR(sysErr);

	if (sysErr == 0)
		{
		if (!sscanf(theInfo.release, "%d.%d", &versMajor, &versMinor))
			sysErr = -1;
		}



	// Get the version
	theVers = kOSLinux;
	
	if (sysErr == 0)
		{
		if (versMajor == 3 && versMinor == 4)
			theVers = kOSLinuxThreeFour;

		else if (versMajor == 3 && versMinor == 2)
			theVers = kOSLinuxThreeTwo;

		else if (versMajor == 3 && versMinor == 0)
			theVers = kOSLinuxThree;

		else if (versMajor == 2 && versMinor == 6)
			theVers = kOSLinuxTwoSix;

		else if (versMajor == 2 && versMinor == 4)
			theVers = kOSLinuxTwoFour;

		else if (versMajor == 2 && versMinor == 2)
			theVers = kOSLinuxTwoTwo;

		else if (versMajor == 2 && versMinor == 0)
			theVers = kOSLinuxTwo;
		}
	
	return(theVers);
}





//============================================================================
//		NTargetSystem::GetOSName : Get the OS name.
//----------------------------------------------------------------------------
NString NTargetSystem::GetOSName(void)
{	NString				theResult;
	struct utsname		theInfo;
	int					sysErr;



	// Get the state we need
	sysErr = uname(&theInfo);
	NN_ASSERT_NOERR(sysErr);



	// Get the OS name
	if (sysErr == 0)
		theResult.Format("%s %s/%s", theInfo.sysname, theInfo.release, theInfo.version);

	return(theResult);
}





//============================================================================
//		NTargetSystem::GetSystemCPU : Get the clock speed.
//----------------------------------------------------------------------------
UInt64 NTargetSystem::GetSystemCPU(void)
{	NRangeList	theRanges;
	UInt64		theResult;
	NString		theInfo;



	// Get the state we need
	theInfo   = GetProcInfoFile("/proc/cpuinfo");
	theResult = 0;



	// Get the clock speed
	theRanges = theInfo.FindAll("cpu MHz\\s*:\\s*(\\d+)", kNStringPattern);
	if (theRanges.size() >= 2)
		theResult = NNumber(theInfo.GetString(theRanges[1])).GetUInt64();

	NN_ASSERT(theResult != 0);
	return(theResult);
}





//============================================================================
//		NTargetSystem::GetSystemRAM : Get the physical memory.
//----------------------------------------------------------------------------
UInt64 NTargetSystem::GetSystemRAM(void)
{	NRangeList	theRanges;
	UInt64		theResult;
	NString		theInfo;



	// Get the state we need
	theInfo   = GetProcInfoFile("/proc/meminfo");
	theResult = 0;



	// Get the physical memory
	theRanges = theInfo.FindAll("MemTotal:\\s*(\\d+)", kNStringPattern);
	if (theRanges.size() >= 2)
		theResult = NNumber(theInfo.GetString(theRanges[1])).GetUInt64() * kNKilobyte;

	NN_ASSERT(theResult != 0);
	return(theResult);
}





//============================================================================
//		NTargetSystem::GetSystemArch : Get the system architecture.
//----------------------------------------------------------------------------
NString NTargetSystem::GetSystemArch(void)
{	NString		theArch;



	// Get the architecture
#if defined(__i386__) || defined(__x86_64__)
	theArch = "x86";
#else
	UNKNOWN ARCH
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



	// Validate our state
	NN_ASSERT(sizeof(NTaskID)      >= sizeof(pid_t));
	NN_ASSERT(sizeof(NTaskPipeRef) >= sizeof(int));



	// Get the state we need
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
			//
			// We read from the stdin pipe and write to the stdout/stderr pipes.
			close(pipeStdIn [kPipeWrite]);
			close(pipeStdOut[kPipeRead]);
			close(pipeStdErr[kPipeRead]);

			dup2(pipeStdIn [kPipeRead],  STDIN_FILENO);
			dup2(pipeStdOut[kPipeWrite], STDOUT_FILENO);
			dup2(pipeStdErr[kPipeWrite], STDERR_FILENO);

			ExecuteTask(theCmd, cmdName, theArgs);
			_exit(1);
			break;

		default:
			// We're the parent
			//
			// We write to the stdin pipe and read from the stdout/stderr pipes.
			close(pipeStdIn [kPipeRead]);
			close(pipeStdOut[kPipeWrite]);
			close(pipeStdErr[kPipeWrite]);
			
			theTask.stdIn  = (NTaskPipeRef) pipeStdIn [kPipeWrite];
			theTask.stdOut = (NTaskPipeRef) pipeStdOut[kPipeRead];
			theTask.stdErr = (NTaskPipeRef) pipeStdErr[kPipeRead];
			
			fcntl((int) theTask.stdOut, F_SETFL, O_NONBLOCK);
			fcntl((int) theTask.stdErr, F_SETFL, O_NONBLOCK);
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
	sysErr = waitpid((pid_t) theTask.taskID, &theStatus, WNOHANG);
	if (sysErr == 0)
		return;

	else if (sysErr == (int) theTask.taskID && WIFEXITED(theStatus))
		theStatus = WEXITSTATUS(theStatus);

	else
		theStatus = ECHILD;



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
// dair, CFRunLoop not supported
NN_LOG("NTargetSystem::TaskWait skipping runloop!");
//	CFRunLoopRunInMode(kCFRunLoopDefaultMode, waitFor, false);

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
			sysErr = kill((pid_t) theTask.taskID, SIGTERM);
			NN_ASSERT_NOERR(sysErr);
			break;

		case kTaskInterrupt:
			sysErr = kill((pid_t) theTask.taskID, SIGINT);
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
{


	// dair, to do
	NN_LOG("NTargetSystem::GetLocaleValue not implemented!");
	return(0);
}





//============================================================================
//      NTargetSystem::GetLocaleBroadcaster : Get the locale broadcaster.
//----------------------------------------------------------------------------
NBroadcaster *NTargetSystem::GetLocaleBroadcaster(const NString &/*theID*/)
{


	// dair, to do
	NN_LOG("NTargetSystem::GetLocaleBroadcaster not implemented!");
	return(NULL);
}





//============================================================================
//      NTargetSystem::TransformString : Transform a string.
//----------------------------------------------------------------------------
NString NTargetSystem::TransformString(const NString &theString, NStringTransform theTransform)
{


	// dair, to do
	NN_LOG("NTargetSystem::TransformString not implemented!");
	return(theString);
}



