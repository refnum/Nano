/*	NAME:
		NThreadPool.cpp

	DESCRIPTION:
		Thread pool.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadUtilities.h"
#include "NSystemUtilities.h"
#include "NMathUtilities.h"
#include "NTimeUtilities.h"
#include "NThread.h"
#include "NThreadPool.h"





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
struct CompareFIFO
{
	bool operator()(const NThreadTask *a, const NThreadTask *b) const
	{
		return(a->GetTimeStamp() > b->GetTimeStamp());
	}
};

struct CompareLIFO
{
	bool operator()(const NThreadTask *a, const NThreadTask *b) const
	{
		return(a->GetTimeStamp() < b->GetTimeStamp());
	}
};

struct ComparePriority
{
	bool operator()(const NThreadTask *a, const NThreadTask *b) const
	{
		return(a->GetPriority() > b->GetPriority());
	}
};





//============================================================================
//		NThreadPool::NThreadPool : Constructor.
//----------------------------------------------------------------------------
NThreadPool::NThreadPool(void)
{


	// Initialize ourselves
	mIsPaused    = false;
	mStopThreads = false;
	mHavePushed  = true;
	mThreadLimit = NThreadUtilities::GetCPUCount();

	mActiveTasks   = 0;
	mActiveThreads = 0;
}





//============================================================================
//		NThreadPool::~NThreadPool : Destructor.
//----------------------------------------------------------------------------
NThreadPool::~NThreadPool(void)
{


	// Stop the tasks
	//
	// Tasks must be stopped in the correct sequence: scheduled tasks must be
	// flushed to avoid queueing more work, running tasks must be allowed to
	// complete, and finally the threads themselves are stopped.
	CancelTasks();
	WaitForTasks();
	StopThreads();
}





//============================================================================
//		NThreadPool::GetActiveTasks : Get the number of active tasks.
//----------------------------------------------------------------------------
NIndex NThreadPool::GetActiveTasks(void) const
{


	// Get the size
	return(mActiveTasks);
}





//============================================================================
//		NThreadPool::GetPendingTasks : Get the number of pending tasks.
//----------------------------------------------------------------------------
NIndex NThreadPool::GetPendingTasks(void) const
{	StLock		acquireLock(mLock);
	NIndex		numTasks;



	// Get the size
	numTasks = (NIndex) mTasks.size();

	return(numTasks);
}





//============================================================================
//		NThreadPool::GetThreadLimit : Get the thread limit.
//----------------------------------------------------------------------------
NIndex NThreadPool::GetThreadLimit(void) const
{	StLock	acquireLock(mLock);



	// Get our state
	return(mThreadLimit);
}





//============================================================================
//		NThreadPool::SetThreadLimit : Set the thread limit.
//----------------------------------------------------------------------------
void NThreadPool::SetThreadLimit(NIndex theValue)
{	StLock	acquireLock(mLock);



	// Set our state
	mThreadLimit = theValue;
}





//============================================================================
//		NThreadPool::Pause : Pause the pool.
//----------------------------------------------------------------------------
void NThreadPool::Pause(void)
{	StLock	acquireLock(mLock);



	// Validate our state
	NN_ASSERT(!mIsPaused);
	NN_ASSERT(mTasksPending.empty());



	// Update our state
	mIsPaused = true;
}





//============================================================================
//		NThreadPool::Resume : Resume the pool.
//----------------------------------------------------------------------------
void NThreadPool::Resume(void)
{	StLock						acquireLock(mLock);
	NThreadTaskListIterator		theIter;



	// Validate our state
	NN_ASSERT(mIsPaused);



	// Schedule the tasks
	//
	// The threads are blocked until we release the lock, so any sorting
	// of the task list will be deferred until the first thread can run.
	for (theIter = mTasksPending.begin(); theIter != mTasksPending.end(); theIter++)
		ScheduleTask(*theIter);



	// Update our state
	mIsPaused = false;
	
	mTasksPending.clear();
}





//============================================================================
//		NThreadPool::AddTask : Add a task to the pool.
//----------------------------------------------------------------------------
void NThreadPool::AddTask(NThreadTask *theTask)
{	StLock	acquireLock(mLock);



	// Add the task
	if (mIsPaused)
		mTasksPending.push_back(theTask);
	else
		ScheduleTask(theTask);
}





//============================================================================
//		NThreadPool::CancelTasks : Cancel the outstanding tasks.
//----------------------------------------------------------------------------
void NThreadPool::CancelTasks(void)
{	StLock							acquireLock(mLock);
	NThreadTaskListConstIterator	theIter;



	// Cancel the tasks
	for (theIter = mTasks.begin(); theIter != mTasks.end(); theIter++)
		delete *theIter;
	
	mTasks.clear();
}





//============================================================================
//		NThreadPool::WaitForTasks : Wait for the tasks to complete.
//----------------------------------------------------------------------------
bool NThreadPool::WaitForTasks(NTime waitFor)
{	bool		areDone, canTimeout;
	NTime		endTime;



	// Get the state we need
	endTime    =  NTimeUtilities::GetTime() + waitFor;
	canTimeout = !NMathUtilities::AreEqual(waitFor, kNTimeForever);



	// Wait for the tasks
	do
		{
		// Get the state we need
		mLock.Lock();
		areDone = (mTasks.empty() && mActiveTasks == 0);
		mLock.Unlock();



		// Wait for the tasks to complete
		if (!areDone)
			{
			NThread::Sleep();

			if (canTimeout && NTimeUtilities::GetTime() >= endTime)
				break;
			}
		}
	while (!areDone);

	return(areDone);
}





#pragma mark protected
//============================================================================
//		NThreadPool::PushTask : Push a task.
//----------------------------------------------------------------------------
void NThreadPool::PushTask(NThreadTaskList &theTasks, NThreadTask *theTask)
{


	// Push the task
	theTasks.push_back(theTask);
}





//============================================================================
//		NThreadPool::PopTask : Pop a task.
//----------------------------------------------------------------------------
NThreadTask *NThreadPool::PopTask(NThreadTaskList &theTasks, bool havePushed)
{


	// Pop the task
	if (havePushed)
		SchedulePriority(theTasks);

	return(extract_back(theTasks));
}





//============================================================================
//		NThreadPool::ScheduleFIFO : FIFO scheduler.
//----------------------------------------------------------------------------
void NThreadPool::ScheduleFIFO(NThreadTaskList &theTasks)
{


	// Sort the list
	std::sort(theTasks.begin(), theTasks.end(), CompareFIFO());
}





//============================================================================
//		NThreadPool::ScheduleLIFO : LIFO scheduler.
//----------------------------------------------------------------------------
void NThreadPool::ScheduleLIFO(NThreadTaskList &theTasks)
{


	// Sort the list
	std::sort(theTasks.begin(), theTasks.end(), CompareLIFO());
}





//============================================================================
//		NThreadPool::SchedulePriority : Priority scheduler.
//----------------------------------------------------------------------------
void NThreadPool::SchedulePriority(NThreadTaskList &theTasks)
{


	// Sort the list
	std::sort(theTasks.begin(), theTasks.end(), ComparePriority());
}





#pragma mark private
//============================================================================
//		NThreadPool::StopThreads : Stop the threads.
//----------------------------------------------------------------------------
void NThreadPool::StopThreads(void)
{	NIndex		n;



	// Stop the threads
	mLock.Lock();

	mStopThreads = true;
	
	for (n = 0; n < mActiveThreads; n++)
		mSemaphore.Signal();
	
	mLock.Unlock();



	// Wait for them to quit
	while (mActiveThreads != 0)
		NThread::Sleep();
	
	mStopThreads = false;
}





//============================================================================
//		NThreadPool::ScheduleTask : Schedule a task for execution.
//----------------------------------------------------------------------------
void NThreadPool::ScheduleTask(NThreadTask *theTask)
{


	// Validate our state
	NN_ASSERT(mLock.IsLocked());
	NN_ASSERT(mTasks.size() < (size_t) kMaxTasks);
	


	// Add the task
	PushTask(mTasks, theTask);
	mHavePushed = true;



	// Update the workers
	//
	// If we've reached the thread limit then the existing threads will need to
	// process this task, so we signal to let them know there's more work to do.
	//
	// Incrementing the thread count must be done by the main thread, since a large
	// number of tasks may be queued up before any worker thread gets a chance to run. 
	if (mActiveThreads < mThreadLimit)
		NThreadUtilities::DetachFunctor(BindSelf(NThreadPool::ExecuteTasks));

	mSemaphore.Signal();
}





//============================================================================
//		NThreadPool::ExecuteTasks : Execute the tasks.
//----------------------------------------------------------------------------
void NThreadPool::ExecuteTasks(void)
{	NThreadTask		*theTask;
	bool			areDone;



	// Update the pool
	NThreadUtilities::AtomicAdd32(mActiveThreads, 1);



	// Execute the tasks
	do
		{
		// Wait for the semaphore
		mSemaphore.Wait();



		// Get the next task
		mLock.Lock();
		
		areDone = mStopThreads;
		theTask = NULL;
		
		if (!areDone && !mTasks.empty())
			{
			theTask     = PopTask(mTasks, mHavePushed);
			mHavePushed = false;
			}

		mLock.Unlock();



		// Execute the task
		if (theTask != NULL)
			{	NN_MEMORY_POOL		thePool;

			NThreadUtilities::AtomicAdd32(mActiveTasks, 1);

			if (!theTask->IsStopped())
				theTask->Run();

			delete theTask;
			NThreadUtilities::AtomicAdd32(mActiveTasks, -1);
			}
		}
	while (!areDone);



	// Update the pool
	NThreadUtilities::AtomicAdd32(mActiveThreads, -1);
}


