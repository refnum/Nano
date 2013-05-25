/*	NAME:
		NTask.cpp

	DESCRIPTION:
		Command line task.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"
#include "NTimeUtilities.h"
#include "NTargetSystem.h"
#include "NThread.h"
#include "NFile.h"
#include "NTask.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NTime kTaskSleep										= 0.75f;





//============================================================================
//		NTask::NTask : Constructor.
//----------------------------------------------------------------------------
NTask::NTask(void)
{


	// Initialize ourselves
	mTask.taskID     = kNTaskIDNone;
	mTask.taskResult = 0;
	
	mTask.stdIn  = kNTaskPipeRefNone;
	mTask.stdOut = kNTaskPipeRefNone;
	mTask.stdErr = kNTaskPipeRefNone;
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
	UpdateTask();
	
	return(mTask.taskID);
}





//============================================================================
//		NTask::GetResult : Get the task result.
//----------------------------------------------------------------------------
SInt32 NTask::GetResult(void) const
{


	// Get the task result
	UpdateTask();
	
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

	return(theErr);
}





//============================================================================
//		NTask::Terminate : Terminate the task.
//----------------------------------------------------------------------------
void NTask::Terminate(void)
{


	// Validate our state
	NN_ASSERT(IsRunning());



	// Terminate the task
	NTargetSystem::TaskSignal(mTask, kTaskKill);
	UpdateTask();
}





//============================================================================
//		NTask::Interrupt : Interrupt the task.
//----------------------------------------------------------------------------
void NTask::Interrupt(void)
{


	// Validate our state
	NN_ASSERT(IsRunning());



	// Interrupt the task
	NTargetSystem::TaskSignal(mTask, kTaskInterrupt);
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
void NTask::SetArguments(NN_TASK_ARGS_PARAM)
{


	// Set the arguments
	SetArguments(NTextUtilities::GetArguments(NN_TASK_ARGS_LIST, NULL));
}





//============================================================================
//		NTask::ReadOutput : Read the output stream.
//----------------------------------------------------------------------------
NString NTask::ReadOutput(void)
{	NString		theResult;



	// Read from the task
	theResult = mBufferOutput + NTargetSystem::TaskRead(mTask, false);
	mBufferOutput.Clear();
	
	return(theResult);
}





//============================================================================
//		NTask::ReadError : Read the error stream.
//----------------------------------------------------------------------------
NString NTask::ReadError(void)
{	NString		theResult;



	// Read from the task
	theResult = mBufferError + NTargetSystem::TaskRead(mTask, true);
	mBufferError.Clear();

	return(theResult);
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
void NTask::WaitForTask(NTime waitFor)
{	NTime		endTime;



	// Get the state we need
	endTime = NTimeUtilities::GetTime() + waitFor;



	// Wait for the task to complete
	while (IsRunning())
		{
		if (waitFor >= kNTimeNone && NTimeUtilities::GetTime() >= endTime)
			break;

		UpdateTask(kTaskSleep);
		}
}





//============================================================================
//		NTask::Execute : Execute the task.
//----------------------------------------------------------------------------
NString NTask::Execute(NTime waitFor)
{	NString		theResult;
	NTime		endTime;
	NStatus		theErr;



	// Get the state we need
	endTime = NTimeUtilities::GetTime() + waitFor;



	// Execute the command
	theErr = Launch();
	NN_ASSERT_NOERR(theErr);



	// Wait for the results
	while (IsRunning())
		{
		if (waitFor >= kNTimeNone && NTimeUtilities::GetTime() >= endTime)
			break;

		UpdateTask(kTaskSleep);
		}

	theResult = ReadOutput() + ReadError();

	return(theResult);
}





//============================================================================
//		NTask::Execute : Execute a task.
//----------------------------------------------------------------------------
NString NTask::Execute(const char *cmd, NN_TASK_ARGS_PARAM)
{	NString		theResult;
	NTask		theTask;



	// Validate our parameters
	NN_ASSERT(cmd != NULL);



	// Get the state we need
	theTask.SetCommand(cmd);
	theTask.SetArguments(NN_TASK_ARGS_LIST);



	// Execute the command
	theResult = theTask.Execute();

	return(theResult);
}





#pragma mark private
//============================================================================
//		NTask::UpdateTask : Update the task status.
//----------------------------------------------------------------------------
void NTask::UpdateTask(NTime waitFor) const
{


	// Update the task
	//
	// If the task is still running then we update its status from the target
	// layer, which monitors the underlying OS task status.
	//
	// If we're to sleep, we need to do so using the target layer rather than
	// NThread. This allows the target layer to do any book-keeping it needs
	// to monitor the underlying OS task (e.g., fire timers).
	if (mTask.taskID != kNTaskIDNone)
		{
		// Update the task
		if (waitFor > kNTimeNone)
			NTargetSystem::TaskWait(mTask, waitFor);

		NTargetSystem::TaskUpdate(mTask);


		// Update the buffers
		//
		// The output streams must also be polled periodically, or the OS may
		// block the task until its output is consumed.
		//
		// To allow callers to run a task to completion before running, we need
		// to drain the OS buffers into our own.
		mBufferOutput += NTargetSystem::TaskRead(mTask, false);
		mBufferError  += NTargetSystem::TaskRead(mTask, true);
		}
}




