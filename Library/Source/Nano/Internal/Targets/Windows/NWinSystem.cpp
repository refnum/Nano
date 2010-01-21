/*	NAME:
		NWindowsSystem.cpp

	DESCRIPTION:
		Windows system support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NTimeUtilities.h"
#include "NWindows.h"
#include "NTargetSystem.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Buffers
static const UInt32 kTaskBufferSize										= 2048;
static const UInt32 kLocaleBufferSize									= 10;





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// Pipe info
typedef struct {
	HANDLE		readHnd;
	HANDLE		writeHnd;
} PipeInfo;





//============================================================================
//      Internal prototypes
//----------------------------------------------------------------------------
static void ClosePipe(NTaskPipeRef &thePipe);





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//      CreatePipe : Create a pipe.
//----------------------------------------------------------------------------
static NTaskPipeRef CreatePipe(bool forChildRead)
{	PipeInfo				*theInfo;
	NTaskPipeRef			thePipe;
	SECURITY_ATTRIBUTES		secAttr; 
	BOOL					wasOK;



	// Get the state we need
	secAttr.nLength              = sizeof(secAttr); 
	secAttr.bInheritHandle       = TRUE; 
	secAttr.lpSecurityDescriptor = NULL; 



	// Create the pipe info
	theInfo = new PipeInfo;
	thePipe = (NTaskPipeRef) theInfo;

	if (theInfo == NULL)
		return(kNTaskPipeRefNone);

	theInfo->readHnd  = NULL;
	theInfo->writeHnd = NULL;



	// Create the pipes
	//
	// If the pipe is for our child to read from then the write handle of that pipe
	// must not be inheritable (we want it to stop with our child), and vice-versa.
	wasOK = CreatePipe(&theInfo->readHnd, &theInfo->writeHnd, &secAttr, 0);
	if (wasOK)
		wasOK = SetHandleInformation(forChildRead ? theInfo->writeHnd : theInfo->readHnd, HANDLE_FLAG_INHERIT, 0);



	// Handle failure
	if (!wasOK)
		ClosePipe(thePipe);
	
	return(thePipe);
}





//============================================================================
//      ClosePipe : Close a pipe.
//----------------------------------------------------------------------------
static void ClosePipe(NTaskPipeRef &thePipe)
{	PipeInfo		*theInfo = (PipeInfo *) thePipe;



	// Destroy the pipe
	WNSafeCloseHandle(theInfo->readHnd);
	WNSafeCloseHandle(theInfo->writeHnd);
	
	delete theInfo;
	thePipe = kNTaskPipeRefNone;
}





//============================================================================
//      GetPipeHandle : Get a pipe handle.
//----------------------------------------------------------------------------
static HANDLE GetPipeHandle(NTaskPipeRef thePipe, bool forRead)
{	PipeInfo		*theInfo = (PipeInfo *) thePipe;



	// Get the handle
	return(forRead ? theInfo->readHnd : theInfo->writeHnd);
}





//============================================================================
//		ReadPipe : Read from a pipe.
//----------------------------------------------------------------------------
static NString ReadPipe(NTaskPipeRef &thePipe, bool lastRead)
{	PipeInfo		*theInfo = (PipeInfo *) thePipe;
	char			theBuffer[kTaskBufferSize];
	NString			theString;
	DWORD			numRead;



	// Check the pipe
	if (thePipe == kNTaskPipeRefNone)
		return(theString);



	// Read from the pipe
	do
		{
		if (!ReadFile(theInfo->readHnd, theBuffer, kTaskBufferSize, &numRead, NULL))
			numRead = 0;
		
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
{	PipeInfo		*theInfo = (PipeInfo *) thePipe;
	DWORD			numWritten;
	NData			theData;
	BOOL			wasOK;



	// Check the pipe
	if (thePipe == kNTaskPipeRefNone)
		return;



	// Write to the pipe
	theData = theText.GetData();
	wasOK   = WriteFile(theInfo->writeHnd, theData.GetData(), theData.GetSize(), &numWritten, NULL);

	NN_ASSERT(wasOK);
	NN_ASSERT(((NIndex) numWritten) == theData.GetSize());
}





//============================================================================
//      NTargetSystem::DebugLog : Emit a debug message.
//----------------------------------------------------------------------------
#pragma mark -
void NTargetSystem::DebugLog(const char *theMsg)
{


	// Log the message
	OutputDebugStringA(theMsg);
}





//============================================================================
//      NTargetSystem::FindBundle : Find a bundle.
//----------------------------------------------------------------------------
NFile NTargetSystem::FindBundle(const NString &bundleID)
{	TCHAR		theBuffer[MAX_PATH];
	NFile		theFile;



	// Find the app
	if (bundleID.IsEmpty())
		{
		if (GetModuleFileName(NULL, theBuffer, MAX_PATH))
			theFile = NFile(ToNN(theBuffer)).GetParent();
		}
	
	
	
	// Find an external app
	else
		NN_LOG("NTargetSystem::FindBundle not implemented (%@)", bundleID);

	return(theFile);
}





//============================================================================
//      NTargetSystem::TaskCreate : Create a task.
//----------------------------------------------------------------------------
TaskInfo NTargetSystem::TaskCreate(const NString &theCmd, const NStringList &theArgs)
{	TCHAR					*cmdLinePtr;
	STARTUPINFO				startInfo;
	PROCESS_INFORMATION		procInfo;
	TaskInfo				theTask;
	NString					cmdLine;
	BOOL					wasOK;



	// Get the state we need
	theTask.taskID     = kNTaskIDNone;
	theTask.taskResult = 0;
	
	theTask.stdIn  = kNTaskPipeRefNone;
	theTask.stdOut = kNTaskPipeRefNone;
	theTask.stdErr = kNTaskPipeRefNone;

	memset(&startInfo, 0x00, sizeof(startInfo));
	memset(&procInfo,  0x00, sizeof(procInfo));

	startInfo.cb         = sizeof(STARTUPINFO); 
	startInfo.dwFlags   |= STARTF_USESTDHANDLES;

	cmdLine.Format("\"%@\" \"%@\"", theCmd, NString::Join(theArgs, "\" \""));
	cmdLinePtr = _wcsdup(ToWN(cmdLine));



	// Create the pipes
	theTask.stdIn  = CreatePipe(true);
	theTask.stdOut = CreatePipe(false);
	theTask.stdErr = CreatePipe(false);

	startInfo.hStdError  = GetPipeHandle(theTask.stdErr, false);
	startInfo.hStdOutput = GetPipeHandle(theTask.stdOut, false);
	startInfo.hStdInput  = GetPipeHandle(theTask.stdIn,  true);



	// Create the task
	//
	// The Unicode version of CreateProcess may modify the command
	// line argument, so we pass a mutable copy of the string.
	wasOK = CreateProcess(	NULL,			// App name passed in cmd line
							cmdLinePtr,		// Mutable command line
							NULL,			// Process security attributes 
							NULL,			// Primary thread security attributes 
							TRUE,			// Handles are inherited 
							0,				// Creation flags 
							NULL,			// Use parent's environment 
							NULL,			// Use parent's current directory 
							&startInfo,		// Startup info
							&procInfo);		// Receives process info



	// Get the task info
	//
	// We keep the task handle around to query its exit status, but
	// can close the handle to the task's main thread immediately.
	if (wasOK)
		{
		theTask.taskID = (NTaskID) procInfo.hProcess;
		WNSafeCloseHandle(procInfo.hThread);
		}
	
	
	
	// Clean up
	free(cmdLinePtr);
	
	if (!wasOK)
		TaskDestroy(theTask);
	
	return(theTask);
}





//============================================================================
//      NTargetSystem::TaskDestroy : Destroy a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskDestroy(TaskInfo &theTask)
{


	// Clean up
	if (theTask.taskID != kNTaskIDNone)
		{
		CloseHandle((HANDLE) theTask.taskID);
		theTask.taskID = kNTaskIDNone;
		}

	ClosePipe(theTask.stdIn);
	ClosePipe(theTask.stdOut);
	ClosePipe(theTask.stdErr);
}





//============================================================================
//      NTargetSystem::TaskUpdate : Update a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskUpdate(TaskInfo &theTask)
{	DWORD		theStatus;
	BOOL		wasOK;



	// Get the child status
	theStatus = (DWORD) -1;
	wasOK     = GetExitCodeProcess((HANDLE) theTask.taskID, &theStatus);

	if (wasOK && theStatus == STILL_ACTIVE)
		return;



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
{	NTime		endTime;
	MSG			theMsg;



	// Get the state we need
	endTime = NTimeUtilities::GetTime() + waitFor;



	// Wait for the task
	//
	// We must allow timers to run while waiting, since they poll the status.
	while (NTimeUtilities::GetTime() < endTime)
		{
		if (GetMessage(&theMsg, NULL, 0, 0))
			{
			TranslateMessage(&theMsg);
			DispatchMessage( &theMsg);
			}
		}
}





//============================================================================
//      NTargetSystem::TaskKill : Kill a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskKill(const TaskInfo &theTask)
{	BOOL	wasOK;



	// Kill the task
	wasOK = TerminateProcess((HANDLE) theTask.taskID, (UINT) -1);
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetSystem::GetLocaleValue : Get a locale value.
//----------------------------------------------------------------------------
NVariant NTargetSystem::GetLocaleValue(const NString &theID, const NString &theKey)
{	TCHAR			theValue[kLocaleBufferSize];
	NVariant		theResult;
	


	// Validate our parameters
	NN_ASSERT(theID == kNLocaleUser);
	NN_UNUSED(theID);



	// Get the value
	if (theKey == kLocaleIsMetricKey)
		{
		if (GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, theValue, kLocaleBufferSize))
			theResult = (bool) (theValue[0] == '0');
		}

	else
		NN_LOG("Unknown locale key: %@", theKey);

	return(theResult);
}





//============================================================================
//      NTargetSystem::GetLocaleBroadcaster : Get the locale broadcaster.
//----------------------------------------------------------------------------
NBroadcaster *NTargetSystem::GetLocaleBroadcaster(const NString &/*theID*/)
{	static NBroadcaster		sBroadcaster;



	// Get the broadcaster
	//
	// For now, all locales share the same broadcaster.
	return(&sBroadcaster);
}


