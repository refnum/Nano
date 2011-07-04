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
#include "NString.h"





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Task arguments
#define NN_TASK_ARGS					const char *arg1  = NULL,	\
										const char *arg2  = NULL,	\
										const char *arg3  = NULL,	\
										const char *arg4  = NULL,	\
										const char *arg5  = NULL,	\
										const char *arg6  = NULL,	\
										const char *arg7  = NULL,	\
										const char *arg8  = NULL,	\
										const char *arg9  = NULL,	\
										const char *arg10 = NULL,	\
										const char *arg11 = NULL,	\
										const char *arg12 = NULL,	\
										const char *arg13 = NULL,	\
										const char *arg14 = NULL,	\
										const char *arg15 = NULL

#define NN_TASK_ARGS_PARAM				const char *arg1,			\
										const char *arg2, 			\
										const char *arg3, 			\
										const char *arg4, 			\
										const char *arg5, 			\
										const char *arg6, 			\
										const char *arg7, 			\
										const char *arg8, 			\
										const char *arg9, 			\
										const char *arg10,			\
										const char *arg11,			\
										const char *arg12,			\
										const char *arg13,			\
										const char *arg14,			\
										const char *arg15

#define NN_TASK_ARGS_LIST				arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15





//============================================================================
//		Constants
//----------------------------------------------------------------------------
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
class NTask {
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
	virtual NStatus						Launch(   void);
	void								Terminate(void);


	// Interrupt the task
	void								Interrupt(void);


	// Get/set the command
	NString								GetCommand(void) const;
	void								SetCommand(const NString &theCmd);


	// Get/set the arguments
	//
	// Arguments may be set as a NULL-terminated list of UTF8-encoded strings.
	NStringList							GetArguments(void) const;
	void								SetArguments(const NStringList &theArgs);

	void								SetArguments(NN_TASK_ARGS);


	// Read/write the task
	//
	// Does not block; returns an empty string if no output/error data is available.
	virtual NString						ReadOutput(void);
	virtual NString						ReadError( void);

	virtual void						WriteInput(const NString &theValue);


	// Wait for the task to complete
	//
	// Blocks the current thread until the task has completed or the timeout reached.
	void								WaitForTask(NTime waitFor=kNTimeForever);


	// Execute the task
	//
	// Blocks the current thread until the task has completed or the timeout reached.
	NString								Execute(NTime waitFor=kNTimeForever);

	static NString						Execute(const char *cmd, NN_TASK_ARGS);


private:
	void								UpdateTask(NTime waitFor=kNTimeNone) const;


private:
	mutable TaskInfo					mTask;

	NString								mCommand;
	NStringList							mArguments;
};





#endif // NTASK_HDR
