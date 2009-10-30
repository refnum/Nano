/*	NAME:
		NTask.cpp

	DESCRIPTION:
		Command line task.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"
#include "NTargetSystem.h"
#include "NThread.h"
#include "NFile.h"
#include "NTask.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NTime kTaskSleep										= 0.250f;
static const NTime kTaskStatus										= 0.500f;





//============================================================================
//		NTask::NTask : Constructor.
//----------------------------------------------------------------------------
NTask::NTask(void)
{


	// Initialize ourselves
	memset(&mTask, 0x00, sizeof(mTask));
}





//============================================================================
//		NTask::~NTask : Destructor.
//----------------------------------------------------------------------------
NTask::~NTask(void)
{


	// Clean up
	NTargetSystem::TaskDestroy(mTask);
}





//============================================================================
//		NTask::IsRunning : Is the task running?
//----------------------------------------------------------------------------
bool NTask::IsRunning(void) const
{


	// Is the task running?
	return(GetID() != kNTaskIDNone);
}





//============================================================================
//		NTask::GetID : Get the task ID.
//----------------------------------------------------------------------------
NTaskID NTask::GetID(void) const
{


	// Get the task ID
	return(mTask.taskID);
}





//============================================================================
//		NTask::GetResult : Get the task result.
//----------------------------------------------------------------------------
SInt8 NTask::GetResult(void) const
{


	// Get the task result
	return(mTask.taskResult);
}





//============================================================================
//		NTask::Launch : Launch the task.
//----------------------------------------------------------------------------
NStatus NTask::Launch(void)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(!IsRunning());



	// Clean up
	//
	// If we've been used to launch a task already, the task state may contain
	// open stdio streams, since they may contain some buffered output which
	// this process has still to read.
	//
	// Launching a new task means we can discard those streams, closing them
	// as necessary.
	NTargetSystem::TaskDestroy(mTask);



	// Launch the task
	mTask  = NTargetSystem::TaskCreate(mCommand, mArguments);
	theErr = IsRunning() ? kNoErr : kNErrNotSupported;

	if (theErr == kNoErr)
		mTimer.AddTimer(BindSelf(NTask::UpdateTask), kTaskStatus, kTaskStatus);

	return(kNoErr);
}





//============================================================================
//		NTask::Terminate : Terminate the task.
//----------------------------------------------------------------------------
void NTask::Terminate(void)
{


	// Validate our state
	NN_ASSERT(IsRunning());



	// Terminate the task
	NTargetSystem::TaskKill(mTask);
	UpdateTask();
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
	NN_ASSERT(!theCmd.IsEmpty());
	NN_ASSERT(!IsRunning());



	// Set the command
	mCommand = theCmd;
}





//============================================================================
//		NTask::GetArguments : Get the arguments.
//----------------------------------------------------------------------------
NStringList NTask::GetArguments(void) const
{


	// Get the arguments
	return(mArguments);
}





//============================================================================
//		NTask::SetArguments : Set the arguments.
//----------------------------------------------------------------------------
void NTask::SetArguments(const NStringList &theArgs)
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
{	va_list		argList;



	// Set the arguments
	va_start(argList, arg1);
	SetArguments(NTextUtilities::GetArguments(argList, arg1));
	va_end(argList);
}





//============================================================================
//		NTask::ReadOutput : Read the output stream.
//----------------------------------------------------------------------------
NString NTask::ReadOutput(void)
{


	// Read from the task
	return(NTargetSystem::TaskRead(mTask, false));
}





//============================================================================
//		NTask::ReadError : Read the error stream.
//----------------------------------------------------------------------------
NString NTask::ReadError(void)
{


	// Read from the task
	return(NTargetSystem::TaskRead(mTask, true));
}





//============================================================================
//		NTask::WriteInput : Write to the input stream.
//----------------------------------------------------------------------------
void NTask::WriteInput(const NString &theValue)
{


	// Validate our parameters
	NN_ASSERT(!theValue.IsEmpty());



	// Write to the task
	NTargetSystem::TaskWrite(mTask, theValue);
}





//============================================================================
//		NTask::WaitForTask : Wait for the task to complete.
//----------------------------------------------------------------------------
void NTask::WaitForTask(void)
{


	// Wait for the task to complete
	while (IsRunning())
		NTargetSystem::TaskWait(mTask, kTaskSleep);
}





//============================================================================
//		NTask::Execute : Execute a task.
//----------------------------------------------------------------------------
NString NTask::Execute(const char *cmd)
{


	// Execute the task
	return(NTask::Execute(cmd, NULL));
}





//============================================================================
//		NTask::Execute : Execute a task.
//----------------------------------------------------------------------------
NString NTask::Execute(const char *cmd, const char *arg1, ...)
{	bool		mainThread;
	NString		theResult;
	va_list		argList;
	NTask		theTask;
	NStatus		theErr;



	// Get the state we need
	theTask.SetCommand(cmd);

	if (arg1 != NULL)
		{
		va_start(argList, arg1);
		theTask.SetArguments(NTextUtilities::GetArguments(argList, arg1));
		va_end(argList);
		}



	// Execute the command
	theErr = theTask.Launch();
	NN_ASSERT_NOERR(theErr);



	// Wait for the results
	//
	// If we're not the main thread then we won't be able to fire
	// our timer, so we need to update the task status by polling.
	mainThread = NThread::IsMain();

	while (theTask.IsRunning())
		{
		theResult += theTask.ReadOutput();

		if (mainThread)
			NTargetSystem::TaskWait(theTask.mTask, kTaskSleep);
		else
			{
			NThread::Sleep();
			theTask.UpdateTask();
			}
		}

	return(theResult);
}





//============================================================================
//		NTask::UpdateTask : Update the task status.
//----------------------------------------------------------------------------
#pragma mark -
void NTask::UpdateTask(void)
{


	// Update the status
	//
	// If the task is still running then we update it, and once it has
	// exited we perform our one-time cleanup work for the task.
	if (mTask.taskID != kNTaskIDNone)
		NTargetSystem::TaskUpdate(mTask);

	if (mTask.taskID == kNTaskIDNone)
		{
		mTimer.RemoveTimer();
		BroadcastMessage(kMsgTaskTerminated, this);
		}
}



