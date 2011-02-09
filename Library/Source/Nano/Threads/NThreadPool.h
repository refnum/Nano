/*	NAME:
		NThreadPool.h

	DESCRIPTION:
		Thread pool.
	
	COPYRIGHT:
        Copyright (c) 2006-2010, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHREADPOOL_HDR
#define NTHREADPOOL_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NSemaphore.h"
#include "NLock.h"
#include "NThreadTask.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadPool {
public:
										NThreadPool(void);
	virtual							   ~NThreadPool(void);


	// Get the number of tasks
	UInt32								GetActiveTasks( void) const;
	UInt32								GetPendingTasks(void) const;
	

	// Get/set the thread limit
	UInt32								GetThreadLimit(void) const;
	void								SetThreadLimit(UInt32 theValue);


	// Pause/resume the pool
	//
	// Tasks may be added to the pool while it is paused, however neither
	// existing or new tasks will be scheduled until the pool is resumed.
	//
	// This allows tasks to be added in batches that can take into account
	// the pool's scheduling algorithm.
	//
	//
	// E.g., a LIFO pool may contain {A,B,C}. Pausing that pool while adding
	// {1,2,3} will ensure that pool contains {A,B,C,1,2,3} when the next
	// task is executed.
	//
	// If the tasks in this pool are driven by user actions, this ensures that
	// the last user action (3) is always executed first.
	//
	// If the pool was not paused, one or more of {A,B,C} may be dispatched
	// while {1,2,3} are being added - which can cause (3) to have to wait.
	void								Pause( void);
	void								Resume(void);


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
	bool								WaitForTasks(NTime waitFor=kNTimeForever);


protected:
	// Push/pop a task
	//
	// Tasks are saved to a list, and extracted in a scheduler-specific order.
	//
	// PopTask is passed a flag indicating if any tasks have been pushed since
	// the last pop, allowing sorting to be deferred until needed.
	//
	// The default implementation sorts tasks by priority when popping, but these
	// methods can be overridden to implement replacement schedulers.
	virtual void						PushTask(NThreadTaskList &theTasks, NThreadTask *theTask);
	virtual NThreadTask				   *PopTask( NThreadTaskList &theTasks, bool havePushed);


	// Sort a task list
	//
	// Sorts are performed in reverse order, leaving the next task at the end (so
	// that tasks can be popped off the back of the list, until a sort is required).
	void								ScheduleFIFO(    NThreadTaskList &theTasks);
	void								ScheduleLIFO(    NThreadTaskList &theTasks);
	void								SchedulePriority(NThreadTaskList &theTasks);


private:
	void								StopThreads(void);

	void								ScheduleTask(NThreadTask *theTask);
	void								ExecuteTasks(void);


private:
	mutable NMutexLock					mLock;

	bool								mIsPaused;
	bool								mStopThreads;
	bool								mHavePushed;
	UInt32								mThreadLimit;

	SInt32								mActiveTasks;
	SInt32								mActiveThreads;

	NThreadTaskList						mTasks;
	NThreadTaskList						mTasksPending;
	NSemaphore							mSemaphore;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadPoolFIFO : public NThreadPool {


protected:
	// Push/pop a task
	inline void PushTask(NThreadTaskList &theTasks, NThreadTask *theTask)
	{
		theTasks.push_back(theTask);
	}

	inline NThreadTask *PopTask(NThreadTaskList &theTasks, bool /*havePushed*/)
	{
		return(extract_front(theTasks));
	}
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadPoolLIFO : public NThreadPool {


protected:
	// Push/pop a task
	inline void PushTask(NThreadTaskList &theTasks, NThreadTask *theTask)
	{
		theTasks.push_back(theTask);
	}

	inline NThreadTask *PopTask(NThreadTaskList &theTasks, bool /*havePushed*/)
	{
		return(extract_back(theTasks));
	}
};







#endif // NTHREADPOOL_HDR


