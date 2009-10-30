/*	NAME:
		NThreadPool.h

	DESCRIPTION:
		Thread pool.
	
	COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHREADPOOL_HDR
#define NTHREADPOOL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NLock.h"
#include "NSemaphore.h"
#include "NThreadTask.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadPool {
public:
										NThreadPool(void);
	virtual								~NThreadPool(void);


	// Get the number of tasks
	UInt32								GetActiveTasks( void) const;
	UInt32								GetPendingTasks(void) const;
	

	// Get/set the thread limit
	UInt32								GetThreadLimit(void) const;
	void								SetThreadLimit(UInt32 theValue);


	// Add a task to the pool
	//
	// The pool takes ownership of the task, and will release it when done.
	void								AddTask(NThreadTask *theTask);


	// Cancel pending tasks
	//
	// Tasks that have already been scheduled will continue to run.
	void								CancelTasks(void);


	// Wait for completion
	//
	// Waits for all of the scheduled tasks to complete.
	bool								WaitForTasks(EventTime waitFor=kEventDurationForever);


protected:
	// Schedule the tasks
	//
	// The default scheduler sorts by priority.
	virtual void						ScheduleTasks(NThreadTaskList &theTasks);


	// Standard schedulers
	//
	// Can be used to implement replacement schedulers.
	void								ScheduleFIFO(    NThreadTaskList &theTasks);
	void								ScheduleLIFO(    NThreadTaskList &theTasks);
	void								SchedulePriority(NThreadTaskList &theTasks);


private:
	void								StopThreads(void);
	void								ExecuteTasks(void);


private:
	mutable NMutexLock					mLock;

	bool								mStopThreads;
	bool								mScheduleTasks;
	UInt32								mThreadLimit;

	SInt32								mActiveTasks;
	SInt32								mActiveThreads;

	NThreadTaskList						mTasks;
	NSemaphore							mSemaphore;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadPoolFIFO : public NThreadPool {


protected:
	// Schedule tasks
	inline void							ScheduleTasks(NThreadTaskList &theTasks)
	{
		ScheduleFIFO(theTasks);
	}
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadPoolLIFO : public NThreadPool {


protected:
	// Schedule tasks
	inline void							ScheduleTasks(NThreadTaskList &theTasks)
	{
		ScheduleLIFO(theTasks);
	}
};







#endif // NTHREADPOOL_HDR


