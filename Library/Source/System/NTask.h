/*	NAME:
		NTask.h

	DESCRIPTION:
		Command line task.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTASK_HDR
#define NTASK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NTimerTask.h"
#include "NString.h"
#include "NArray.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgTaskTerminated						= 'task';





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTask : public NMsgBroadcaster, public NTimerTask {
public:
										NTask(void);
	virtual								~NTask(void);


	// Is the task running?
	bool								IsRunning(void) const;


	// Get the task pid
	//
	// Returns 0 if the task is not running.
	pid_t								GetPid(void) const;


	// Get the task result
	//
	// Returns 0 if the task has not started, or is still running.
	SInt8								GetResult(void) const;


	// Launch/terminate the task
	//
	// Although a task object can be reused to launch the same command
	// multiple times, each object can only run once instance at a time.
	OSStatus							Launch(   void);
	void								Terminate(void);


	// Get/set the command
	NString								GetCommand(void) const;
	void								SetCommand(const NString &theCmd);


	// Get/set the arguments
	//
	// Arguments may be set as a NULL-terminated list of UTF8-encoded strings.
	NStringList							GetArguments(void) const;
	void								SetArguments(const NStringList &theArgs);
	void								SetArguments(const char *arg1, ...);


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


protected:
	// Handle a timer
	void								DoTimer(NTimerID theTimer, TimerMsg theMsg);


private:
	void								LaunchChild(void);
	void								CheckStatus(void);

	NString								ReadPipe( int &thePipe);
	void								ClosePipe(int &thePipe);
	
	void								ResetTask(SInt8 childExit);


private:
	NString								mCommand;
	NStringList							mArguments;

	pid_t								mChildPid;
	SInt8								mChildExit;
	
	int									mStdIn;
	int									mStdOut;
	int									mStdErr;
};





#endif // NTASK_HDR
