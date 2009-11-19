/*	NAME:
		NMacSystem.cpp

	DESCRIPTION:
		Mac system support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
#include "NCFString.h"

#include "NTargetSystem.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tasks
static const UInt32 kBufferSize											= 1024;

static const UInt32 FD_STDIN											= 0;
static const UInt32 FD_STDOUT											= 1;
static const UInt32 FD_STDERR											= 2;

static const SInt32 PIPE_INVALID										= -1;
static const SInt32 PIPE_PARENT											= 0;
static const SInt32 PIPE_CHILD											= 1;





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
	sysErr = execv(theCmd.GetUTF8(), (char * const *) &argList[0]);
	NN_ASSERT(sysErr == 0);

	_exit(EXIT_FAILURE);
}





//============================================================================
//		ClosePipe : Close a pipe.
//----------------------------------------------------------------------------
static void ClosePipe(SInt32 &thePipe)
{


	// Close the pipe
	if (thePipe != PIPE_INVALID)
		{
		close(thePipe);
		thePipe = PIPE_INVALID;
		}
}





//============================================================================
//		ReadPipe : Read from a pipe.
//----------------------------------------------------------------------------
static NString ReadPipe(SInt32 &thePipe, bool lastRead)
{	char		theBuffer[kBufferSize];
	NString		theString;
	int			numRead;



	// Check the pipe
	if (thePipe == PIPE_INVALID)
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
static void WritePipe(SInt32 thePipe, const NString &theText)
{


	// Check the pipe
	if (thePipe == PIPE_INVALID)
		return;
	
	
	
	// Write to the pipe
	write(thePipe, theText.GetUTF8(), theText.GetSize());
}





//============================================================================
//      NTargetSystem::FindBundle : Find a bundle.
//----------------------------------------------------------------------------
#pragma mark -
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
	
	theTask.stdIn  = PIPE_INVALID;
	theTask.stdOut = PIPE_INVALID;
	theTask.stdErr = PIPE_INVALID;

	cmdName = NFile(theCmd).GetName();



	// Create the pipes
	if (pipe(pipeStdIn)  == -1 ||
		pipe(pipeStdOut) == -1 ||
		pipe(pipeStdErr) == -1)
		return(theTask);



	// Fork the process
	theTask.taskID = fork();
	
	switch (theTask.taskID) {
		case -1:
			// Handle failure
			return(theTask);
			break;

		case 0:
			// We're the child
			close(FD_STDIN);
			close(FD_STDOUT);
			close(FD_STDERR);

			dup2(pipeStdIn [PIPE_CHILD], FD_STDIN);
			dup2(pipeStdOut[PIPE_CHILD], FD_STDOUT);
			dup2(pipeStdErr[PIPE_CHILD], FD_STDERR);
			
			ExecuteTask(theCmd, cmdName, theArgs);
			_exit(1);
			break;

		default:
			// We're the parent:
			close(pipeStdIn [PIPE_CHILD]);
			close(pipeStdOut[PIPE_CHILD]);
			close(pipeStdErr[PIPE_CHILD]);
			
			theTask.stdIn  = pipeStdIn [PIPE_PARENT];
			theTask.stdOut = pipeStdOut[PIPE_PARENT];
			theTask.stdErr = pipeStdErr[PIPE_PARENT];
			
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

	else if (sysErr == theTask.taskID && WIFEXITED(theStatus))
		theStatus = WEXITSTATUS(theStatus);

	else
		theStatus = EX_SOFTWARE;



	// Update the task
	theTask.taskID     = kNTaskIDNone;
	theTask.taskResult = theStatus;



	// Free our resources
	//
	// The stdout/err pipes are left open since they may contain buffered
	// data that can still be read. Since the child has exited, the stdin
	// pipe is now widowed and can be closed.
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
void NTargetSystem::TaskWait(const TaskInfo &/*theTask*/, NTime theDelay)
{


	// Wait for the task
	//
	// We must allow timers to run while waiting, since they poll the status.
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, theDelay, false);
}





//============================================================================
//      NTargetSystem::TaskKill : Kill a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskKill(const TaskInfo &theTask)
{	int		sysErr;



	// Kill the task
	sysErr = kill(theTask.taskID, SIGTERM);
	NN_ASSERT(sysErr == 0);
}








