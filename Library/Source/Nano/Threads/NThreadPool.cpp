/*	NAME:
		NThreadPool.cpp

	DESCRIPTION:
		Thread pool.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NThreadPool.h"

// Nano
#include "NFormat.h"
#include "NMachine.h"
#include "NScopedLock.h"
#include "NThreadTask.h"
#include "NTimeUtils.h"
#include "NanoConstants.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
inline constexpr NInterval kNCullInterval                   = 2 * kNTimeMinute;





//=============================================================================
//		NThreadPool::NThreadPool : Constructor.
//-----------------------------------------------------------------------------
NThreadPool::NThreadPool(const NString& theName,
						 size_t         minThreads,
						 size_t         maxThreads,
						 size_t         stackSize)
	: mName(theName)
	, mMinThreads(minThreads)
	, mMaxThreads(maxThreads)
	, mStackSize(stackSize)
	, mLock{}
	, mCullTask{}
	, mIsPaused(false)
	, mLastWork(0)
	, mWorkers{}
	, mTasks{}
{


	// Validate our parameters
	NN_REQUIRE(minThreads >= 1);
	NN_REQUIRE(minThreads <= maxThreads);



	// Initialize ourselves
	StartCulling();

	if (mMaxThreads == kNSizeMax)
	{
		mMaxThreads = NMachine::GetCores();
	}
}





//=============================================================================
//		NThreadPool::~NThreadPool : Destructor.
//-----------------------------------------------------------------------------
NThreadPool::~NThreadPool()
{


	// Clean up
	StopCulling();
	StopWorkers();
}





//=============================================================================
//		NThreadPool::GetThreads : Get the thread count.
//-----------------------------------------------------------------------------
size_t NThreadPool::GetThreads() const
{


	// Get the count
	NScopedLock acuireLock(mLock);

	return mWorkers.size();
}





//=============================================================================
//		NThreadPool::GetMinThreads : Get the minimum thread count.
//-----------------------------------------------------------------------------
size_t NThreadPool::GetMinThreads() const
{


	// Get the count
	return mMinThreads;
}





//=============================================================================
//		NThreadPool::GetMaxThreads : Get the maximum thread count.
//-----------------------------------------------------------------------------
size_t NThreadPool::GetMaxThreads() const
{


	// Get the count
	return mMaxThreads;
}





//=============================================================================
//		NThreadPool::IsPaused : Is the pool paused?
//-----------------------------------------------------------------------------
bool NThreadPool::IsPaused() const
{


	// Get the state
	return mIsPaused;
}





//=============================================================================
//		NThreadPool::Pause : Pause the pool.
//-----------------------------------------------------------------------------
void NThreadPool::Pause()
{


	// Pause the pool
	mIsPaused = true;
}





//=============================================================================
//		NThreadPool::Resume : Resume the pool.
//-----------------------------------------------------------------------------
void NThreadPool::Resume()
{


	// Resume the pool
	mIsPaused = false;

	StartWorkers();
}





//=============================================================================
//		NThreadPool::Add : Add a task.
//-----------------------------------------------------------------------------
void NThreadPool::Add(const NFunction& theFunction)
{


	// Add the task
	Add(std::make_shared<NThreadTask>(theFunction));
}





//=============================================================================
//		NThreadPool::Add : Add a task.
//-----------------------------------------------------------------------------
void NThreadPool::Add(const NSharedThreadTask& theTask)
{


	// Add the task
	mTasks.EmplaceFront(theTask);

	StartWorkers();
}





//=============================================================================
//		NThreadPool::GetMain : Get the main thread pool.
//-----------------------------------------------------------------------------
NThreadPool* NThreadPool::GetMain()
{


	// Get the main thread pool
	static NThreadPool sPool("NThreadPool");

	return &sPool;
}





#pragma mark private
//=============================================================================
//		NThreadPool::StartWorkers : Start the workers.
//-----------------------------------------------------------------------------
void NThreadPool::StartWorkers()
{


	// Check our state
	size_t numTasks = mTasks.GetSize();

	if (numTasks == 0)
	{
		return;
	}



	// Wake idle workers
	//
	// We wake one idle worker for each oustanding task.
	//
	// Running workers will consume as many tasks as they can, so an idle
	// worker may be woken unnecessarily if a running worker steals the
	// new work, however this ensures maximum throughput when tasks are
	// long-running.
	NScopedLock acquireLock(mLock);

	mLastWork = NTimeUtils::GetTime();

	for (auto& theWorker : mWorkers)
	{
		if (theWorker->isIdle)
		{
			StartWorker(theWorker.get());

			numTasks--;
			if (numTasks == 0)
			{
				return;
			}
		}
	}



	// Create new threads
	//
	// We create one new worker for each outstanding task, up to the limit.
	while (mWorkers.size() < mMaxThreads)
	{
		CreateWorker();

		numTasks--;
		if (numTasks == 0)
		{
			return;
		}
	}



	// Always wake one worker
	//
	// We may race between adding work and a worker marking itself as idle:
	//
	//		Worker: finish ExecuteTasks
	//		Caller: add task, call StartWorkers
	//		Caller: no idle threads, max workers created, do nothing
	//		Worker: mark self as idle
	//		-> new task is never picked up by a worker
	//
	// To avoid this we always bump one worker to process any new task.
	//
	// This has minimal overhead as, if we did catch a worker in this window,
	// the worker would simply re-enter its runloop rather before sleeping.
	StartWorker(mWorkers.front().get());
}





//=============================================================================
//		NThreadPool::StopWorkers : Stop the workers.
//-----------------------------------------------------------------------------
void NThreadPool::StopWorkers()
{


	// Stop the workers
	NScopedLock acquireLock(mLock);

	for (auto& theWorker : mWorkers)
	{
		theWorker->theThread->RequestStop();
	}

	for (auto& theWorker : mWorkers)
	{
		theWorker->theThread->WaitForCompletion();
	}

	mWorkers.clear();
}





//=============================================================================
//		NThreadPool::ExecuteTasks : Execute tasks.
//-----------------------------------------------------------------------------
void NThreadPool::ExecuteTasks()
{


	// Execute the tasks
	NThread* thisThread = NThread::GetCurrent();
	bool     gotWork    = true;

	while (gotWork && !mIsPaused && !thisThread->ShouldStop())
	{
		auto theWork = mTasks.PopBack();
		gotWork      = theWork.has_value();

		if (gotWork)
		{
			NSharedThreadTask theTask = *theWork;

			if (!theTask->IsCancelled())
			{
				theTask->Execute();
			}
		}
	}
}





//=============================================================================
//		NThreadPool::CreateWorker : Create a new worker.
//-----------------------------------------------------------------------------
void NThreadPool::CreateWorker()
{


	// Validate our state
	NN_REQUIRE(mWorkers.size() < mMaxThreads);



	// Create the worker
	NUniqueThreadPoolWorker theWorker = std::make_unique<NThreadPoolWorker>();

	NString threadID     = NFormat("{}.{}", mName, mWorkers.size());
	theWorker->theThread = NThread::Create(threadID, mStackSize);
	theWorker->isIdle    = true;

	mWorkers.emplace_back(std::move(theWorker));
	StartWorker(mWorkers.back().get());
}





//=============================================================================
//		NThreadPool::StartWorker : Execute a worker.
//-----------------------------------------------------------------------------
void NThreadPool::StartWorker(NThreadPoolWorker* theWorker)
{


	// Start the worker
	theWorker->theThread->GetRunLoop()->Add(
		[=]()
		{
			NN_REQUIRE(theWorker->isIdle);
			theWorker->isIdle = false;

			ExecuteTasks();

			theWorker->isIdle = true;
			mLastWork         = NTimeUtils::GetTime();
		});
}





//=============================================================================
//		NThreadPool::StartCulling : Start culling workers.
//-----------------------------------------------------------------------------
void NThreadPool::StartCulling()
{


	// Install the task
	mCullTask.Add(
		[this]()
		{
			CullWorkers();
		},
		kNCullInterval,
		kNCullInterval);
}





//=============================================================================
//		NThreadPool::StopCulling : Stop culling workers.
//-----------------------------------------------------------------------------
void NThreadPool::StopCulling()
{


	// Remove the task
	mCullTask.Clear();
}





//=============================================================================
//		NThreadPool::CullWorkers : Cull the workers.
//-----------------------------------------------------------------------------
void NThreadPool::CullWorkers()
{


	// Cull threads
	//
	// Workers are culled whenever we are idle for a prolonged period.
	//
	// We cull from the back to ensure that any further worker threads
	// continue to have sequentially numbered names (0..mMaxThreads).
	NInterval idleTime = NTimeUtils::GetTime() - mLastWork;

	if (idleTime >= kNCullInterval)
	{
		NScopedLock acquireLock(mLock);

		size_t newSize = mWorkers.size() / 2;

		while (mWorkers.size() != newSize)
		{
			NN_REQUIRE(mWorkers.back()->isIdle);
			mWorkers.pop_back();
		}
	}
}
