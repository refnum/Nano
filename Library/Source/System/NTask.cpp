/*	NAME:
		NTask.cpp

	DESCRIPTION:
		Command line task.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/select.h>
#include <sysexits.h>

#include "NFile.h"
#include "NTask.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Pipes
static const UInt32 FD_STDIN											= 0;
static const UInt32 FD_STDOUT											= 1;
static const UInt32 FD_STDERR											= 2;

static const SInt32 PIPE_INVALID										= -1;
static const SInt32 PIPE_PARENT											= 0;
static const SInt32 PIPE_CHILD											= 1;


// Misc
static const UInt32         kBufferSize									= 1024;
static const CFTimeInterval kTaskSleep									= 0.250f;
static const EventTime		kTaskStatus									= 0.500f;


// Timers
static const NTimerID kTimerCheckStatus									= 1000;





//============================================================================
//		NTask::NTask : Constructor.
//----------------------------------------------------------------------------
NTask::NTask(void)
{


	// Initialize ourselves
	mChildPid  = 0;
	mChildExit = 0;
	
	mStdIn  = PIPE_INVALID;
	mStdOut = PIPE_INVALID;
	mStdErr = PIPE_INVALID;
}





//============================================================================
//		NTask::~NTask : Destructor.
//----------------------------------------------------------------------------
NTask::~NTask(void)
{


	// Clean up
	ClosePipe(mStdIn);
	ClosePipe(mStdOut);
	ClosePipe(mStdErr);
}





//============================================================================
//		NTask::IsRunning : Is the task running?
//----------------------------------------------------------------------------
bool NTask::IsRunning(void) const
{


	// Is the task running?
	return(GetPid() != 0);
}





//============================================================================
//		NTask::GetPid : Get the task pid.
//----------------------------------------------------------------------------
pid_t NTask::GetPid(void) const
{


	// Get the task pid
	return(mChildPid);
}





//============================================================================
//		NTask::GetResult : Get the task result.
//----------------------------------------------------------------------------
SInt8 NTask::GetResult(void) const
{


	// Get the task result
	return(mChildExit);
}





//============================================================================
//		NTask::Launch : Launch the task.
//----------------------------------------------------------------------------
OSStatus NTask::Launch(void)
{	int		pipeStdIn[2], pipeStdOut[2], pipeStdErr[2];



	// Validate our state
	NN_ASSERT(!IsRunning());
	NN_ASSERT(mStdIn == PIPE_INVALID);



	// Reset the pipes
	//
	// If we've been used to launch a task already, stdin will be closed when
	// the child exits (it is an error to re-launch a task until a previously
	// launched task has exited).
	//
	// The stdout/stderr pipes may still be open, since they may contain some
	// buffered that that the caller has still to read.
	//
	// Launching a new task means we can discard them, closing them as necessary.
	if (mStdOut != PIPE_INVALID) ClosePipe(mStdOut);
	if (mStdErr != PIPE_INVALID) ClosePipe(mStdErr);



	// Create the pipes
	if (pipe(pipeStdIn) == -1 || pipe(pipeStdOut) == -1 || pipe(pipeStdErr) == -1)
		return(kUSBTooManyPipesErr);



	// Fork the command
	mChildPid = fork();
	
	switch (mChildPid) {
		case -1:
			// Handle failure
			return(dsBadLaunch);
			break;

		case 0:
			// We're the child
			close(FD_STDIN);
			close(FD_STDOUT);
			close(FD_STDERR);

			dup2(pipeStdIn [PIPE_CHILD], FD_STDIN);
			dup2(pipeStdOut[PIPE_CHILD], FD_STDOUT);
			dup2(pipeStdErr[PIPE_CHILD], FD_STDERR);
			
			LaunchChild();
			_exit(1);
			break;

		default:
			// We're the parent:
			close(pipeStdIn [PIPE_CHILD]);
			close(pipeStdOut[PIPE_CHILD]);
			close(pipeStdErr[PIPE_CHILD]);
			
			mStdIn  = pipeStdIn [PIPE_PARENT];
			mStdOut = pipeStdOut[PIPE_PARENT];
			mStdErr = pipeStdErr[PIPE_PARENT];
			
			fcntl(mStdOut, F_SETFL, O_NONBLOCK);
			fcntl(mStdErr, F_SETFL, O_NONBLOCK);
			
			AddTimer(kTaskStatus, kTaskStatus, kTimerCheckStatus);
			break;
		}
	
	return(noErr);
}





//============================================================================
//		NTask::Terminate : Terminate the task.
//----------------------------------------------------------------------------
void NTask::Terminate(void)
{	int		sysErr;



	// Validate our state
	NN_ASSERT(IsRunning());



	// Terminate the command
	sysErr = kill(mChildPid, SIGTERM);
	NN_ASSERT(sysErr == 0);



	// Update our state
	CheckStatus();
}





//============================================================================
//		NTask::GetCommand : Get the command.
//----------------------------------------------------------------------------
NString NTask::GetCommand(void) const
{


	// Get the command
	return(mCommand);
}





//============================================================================
//		NTask::SetCommand : Set the command.
//----------------------------------------------------------------------------
void NTask::SetCommand(const NString &theCmd)
{


	// Validate our parameters and state
	NN_ASSERT(theCmd.IsNotEmpty());
	NN_ASSERT(!IsRunning());



	// Set the command
	mCommand = theCmd;
}





//============================================================================
//		NTask::GetArguments : Get the arguments.
//----------------------------------------------------------------------------
NArray NTask::GetArguments(void) const
{


	// Get the arguments
	return(mArguments);
}





//============================================================================
//		NTask::SetArguments : Set the arguments.
//----------------------------------------------------------------------------
void NTask::SetArguments(const NArray &theArgs)
{


	// Validate our state
	NN_ASSERT(!IsRunning());



	// Set the arguments
	mArguments = theArgs;
}





//============================================================================
//		NTask::SetArguments : Set the arguments.
//----------------------------------------------------------------------------
void NTask::SetArguments(const char *arg1, ...)
{	const char	*argPtr;
	NArray		theArgs;
	va_list		argList;



	// Collect the arguments
	va_start(argList, arg1);

	argPtr = arg1;
	do
		{
		theArgs.AppendValueString(argPtr);
		argPtr = va_arg(argList, const char *);
		}
	while (argPtr != NULL);

	va_end(argList);



	// Set the arguments
	SetArguments(theArgs);
}





//============================================================================
//		NTask::ReadOutput : Read the output stream.
//----------------------------------------------------------------------------
NString NTask::ReadOutput(void)
{


	// Read from the child
	return(ReadPipe(mStdOut));
}





//============================================================================
//		NTask::ReadError : Read the error stream.
//----------------------------------------------------------------------------
NString NTask::ReadError(void)
{


	// Read from the child
	return(ReadPipe(mStdErr));
}





//============================================================================
//		NTask::WriteInput : Write to the input stream.
//----------------------------------------------------------------------------
void NTask::WriteInput(const NString &theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue.IsNotEmpty());



	// Write to the child
	if (mStdIn != PIPE_INVALID)
		write(mStdIn, theValue.GetUTF8(), theValue.GetSize());
}





//============================================================================
//		NTask::WaitForTask : Wait for the task to complete.
//----------------------------------------------------------------------------
void NTask::WaitForTask(void)
{


	// Wait for the task to complete
	while (IsRunning())
		CFRunLoopRunInMode(kCFRunLoopDefaultMode, kTaskSleep, false);
}





//============================================================================
//		NTask::DoTimer : Handle a timer.
//----------------------------------------------------------------------------
#pragma mark -
void NTask::DoTimer(NTimerID theTimer, TimerMsg theMsg)
{


	// Handle the timer
	switch (theTimer) {
		case kTimerCheckStatus:
			CheckStatus();
			break;
		
		default:
			NTimerTask::DoTimer(theTimer, theMsg);
			break;
		}
}





//============================================================================
//		NTask::LaunchChild : Launch the child.
//----------------------------------------------------------------------------
#pragma mark -
void NTask::LaunchChild(void)
{	UInt32			n, numArgs;
	const char		**argList;
	int				sysErr;



	// Build the argument list
	//
	// The argument list is prefixed with the name of the command, and
	// terminated with a NULL pointer.
	//
	// Since the execv will overlay this process with the child process,
	// we can strdup our strings since they will be released with us.
	numArgs = mArguments.GetSize();
	argList = (const char **) calloc(numArgs + 2, sizeof(const char *));

	if (argList == NULL)
		return;
	
	argList[0] = strdup(NFile(mCommand).GetFileName().GetUTF8());
				
	for (n = 0; n < numArgs; n++)
		argList[1 + n] = strdup(mArguments.GetValueString(n).GetUTF8());



	// Execute the command
	sysErr = execv(strdup(mCommand.GetUTF8()), (char * const *) argList);
	NN_ASSERT(sysErr == 0);
	
	_exit(EXIT_FAILURE);
}





//============================================================================
//		NTask::CheckStatus : Check the child status.
//----------------------------------------------------------------------------
void NTask::CheckStatus(void)
{	int		sysErr, theStatus;



	// Get the child status
	sysErr = wait4(mChildPid, &theStatus, WNOHANG, NULL);
	if (sysErr == 0)
		return;

	else if (sysErr == mChildPid && WIFEXITED(theStatus))
		theStatus = WEXITSTATUS(theStatus);

	else
		theStatus = EX_SOFTWARE;



	// Update our state
	mChildPid  = 0;
	mChildExit = theStatus;



	// Free our resources
	//
	// The stdout/err pipes are left open since they may contain buffered
	// data that can still be read. Since the child has exited, the stdin
	// pipe is now widowed and can be closed.
	ClosePipe(mStdIn);

	RemoveTimer(kTimerCheckStatus);



	// Let everyone know
	BroadcastMessage(kMsgTaskTerminated, this);
}





//============================================================================
//		NTask::ReadPipe : Read a pipe.
//----------------------------------------------------------------------------
NString NTask::ReadPipe(int &thePipe)
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



	// Update our state
	//
	// If the child is finished, and the pipe is empty, we can close our end.
	if (!IsRunning() && numRead == 0)
		ClosePipe(thePipe);

	return(theString);
}





//============================================================================
//		NTask::ClosePipe : Close a pipe.
//----------------------------------------------------------------------------
void NTask::ClosePipe(int &thePipe)
{


	// Close the pipe
	if (thePipe != PIPE_INVALID)
		{
		close(thePipe);
		thePipe = PIPE_INVALID;
		}
}


