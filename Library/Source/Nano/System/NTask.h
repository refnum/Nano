/*	NAME:
		NTask.h

	DESCRIPTION:
		Command line task.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTASK_HDR
#define NTASK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBroadcaster.h"
#include "NString.h"
#include "NTimer.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgTaskTerminated						= 0x7461736B;	// 'task'


// Tasks
typedef UIntPtr NTaskID;
typedef UIntPtr NTaskPipeRef;

static const NTaskID      kNTaskIDNone								= 0;
static const NTaskPipeRef kNTaskPipeRefNone							= (NTaskPipeRef) -1;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Task info
typedef struct {
	NTaskID			taskID;
	SInt32			taskResult;

	NTaskPipeRef	stdIn;
	NTaskPipeRef	stdOut;
	NTaskPipeRef	stdErr;
} TaskInfo;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTask : public NBroadcaster {
public:
										NTask(void);
	virtual							   ~NTask(void);


	// Is the task running?
	bool								IsRunning(void) const;


	// Get the task ID
	//
	// The task ID is plaform-specific.
	NTaskID								GetID(void) const;


	// Get the task result
	//
	// Returns 0 if the task has not started, or is still running.
	SInt32								GetResult(void) const;


	// Launch/terminate the task
	//
	// Although a task object can be reused to launch the same command
	// multiple times, each object can only run once instance at a time.
	NStatus								Launch(   void);
	void								Terminate(void);


	// Get/set the command
	NString								GetCommand(void) const;
	void								SetCommand(const NString &theCmd);


	// Get/set the arguments
	//
	// Arguments may be set as a NULL-terminated list of UTF8-encoded strings.
	NStringList							GetArguments(void) const;
	void								SetArguments(const NStringList &theArgs);

	void								SetArguments(	const char *arg1,
														const char *arg2  = NULL,
														const char *arg3  = NULL,
														const char *arg4  = NULL,
														const char *arg5  = NULL,
														const char *arg6  = NULL,
														const char *arg7  = NULL,
														const char *arg8  = NULL,
														const char *arg9  = NULL,
														const char *arg10 = NULL);


	// Read/write the task
	//
	// Does not block; returns an empty string if no output/error data is available.
	NString								ReadOutput(void);
	NString								ReadError( void);

	void								WriteInput(const NString &theValue);


	// Wait for the task to complete
	//
	// Will block the current thread until the task has completed.
	void								WaitForTask(void);


	// Execute a task
	//
	// Will block the current thread until the task has completed.
	static NString						Execute(const char *cmd);
	static NString						Execute(const char *cmd, const char *arg1, ...);


private:
	void								UpdateTask(void);


private:
	TaskInfo							mTask;
	NTimer								mTimer;

	NString								mCommand;
	NStringList							mArguments;
};





#endif // NTASK_HDR
