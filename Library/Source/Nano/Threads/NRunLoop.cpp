/*	NAME:
		NRunLoop.cpp

	DESCRIPTION:
		Thread runloop.

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
#include "NRunLoop.h"

// Nano
#include "NScopedLock.h"
#include "NStdAlgorithm.h"
#include "NThread.h"
#include "NTimeUtils.h"





//=============================================================================
//		NRunLoop::NRunLoop : Constructor.
//-----------------------------------------------------------------------------
NRunLoop::NRunLoop(bool isMain)
	: mLock()
	, mRunLoop(RunLoopCreate(isMain))
	, mOwnerID(NThreadID::Get())
	, mStopWork(nullptr)
	, mSortWork(false)
	, mNextID(NRunLoopWorkNone)
	, mWork{}
{
}





//=============================================================================
//		NRunLoop::~NRunLoop : Destructor.
//-----------------------------------------------------------------------------
NRunLoop::~NRunLoop()
{


	// Clean up
	RunLoopDestroy(mRunLoop);
}





//=============================================================================
//		NRunLoop::Run : Run the runloop.
//-----------------------------------------------------------------------------
void NRunLoop::Run(NInterval runFor)
{


	// Validate our parameters and state
	NN_REQUIRE(runFor == kNTimeForever || runFor >= 0.0);
	NN_REQUIRE(mOwnerID == NThreadID::Get(), "A runloop can only be run by its thread!");



	// Prepare to run
	//
	// Runloops can be nested so we use a local flag to hold the stop
	// state for each depth.
	mLock.Lock();

	bool  stopWork     = false;
	bool* prevStopWork = mStopWork;
	mStopWork          = &stopWork;

	mLock.Unlock();



	// Execute the runloop
	//
	// The runloop alternates between executing work and sleeping,
	// until it is explicitly stopped or it reaches the end time.
	NTime endTime = NTimeUtils::GetAbsolute(runFor);
	bool  onlyOne = (runFor == kNTimeNone);

	while (true)
	{
		PerformWork(onlyOne);
		if (onlyOne || stopWork || FinishedSleep(endTime))
		{
			break;
		}
	}



	// Restore our state
	mLock.Lock();

	mStopWork = prevStopWork;

	mLock.Unlock();
}





//=============================================================================
//		NRunLoop::Stop : Stop the runloop.
//-----------------------------------------------------------------------------
void NRunLoop::Stop()
{


	// Stop the runloop
	NScopedLock acquireLock(mLock);

	if (mStopWork != nullptr)
	{
		*mStopWork = true;
		RunLoopWake(mRunLoop);
	}
}





//=============================================================================
//		NRunLoop::Add : Add a function.
//-----------------------------------------------------------------------------
NRunLoopWorkID NRunLoop::Add(const NFunction&  theFunction,
							 NInterval         executeAfter,
							 NInterval         executeEvery,
							 NSharedSemaphore* theSemaphore)
{


	// Validate our parameters and state
	NN_REQUIRE(theFunction != nullptr);
	NN_REQUIRE(executeAfter >= 0.0);
	NN_REQUIRE(executeEvery >= 0.0);



	// Get the state we need
	NRunLoopWork theWork{};

	theWork.theID        = NRunLoopWorkNone;
	theWork.executeAt    = NTimeUtils::GetTime() + executeAfter;
	theWork.executeEvery = executeEvery;
	theWork.theFunction  = theFunction;

	if (theSemaphore != nullptr)
	{
		theWork.theSemaphore = *theSemaphore;
	}



	// Add the work
	NScopedLock acquireLock(mLock);

	theWork.theID = ++mNextID;
	mWork.emplace_back(theWork);



	// Schedule it
	SetWorkTime(theWork.theID, theWork.executeAt);

	return theWork.theID;
}





//=============================================================================
//		NRunLoop::Remove : Remove a function.
//-----------------------------------------------------------------------------
void NRunLoop::Remove(NRunLoopWorkID theID)
{


	// Validate our parameters
	NN_REQUIRE(theID != NRunLoopWorkNone);



	// Remove the work
	NScopedLock acquireLock(mLock);

	for (auto theIter = mWork.begin(); theIter != mWork.end(); theIter++)
	{
		if (theIter->theID == theID)
		{
			mWork.erase(theIter);
			return;
		}
	}
}





//=============================================================================
//		NRunLoop::GetWorkTime : Get the execution time for work.
//-----------------------------------------------------------------------------
NTime NRunLoop::GetWorkTime(NRunLoopWorkID theID) const
{


	// Validate our parameters
	NN_REQUIRE(theID != NRunLoopWorkNone);



	// Get the execution time
	NScopedLock acquireLock(mLock);

	for (auto theIter = mWork.begin(); theIter != mWork.end(); theIter++)
	{
		if (theIter->theID == theID)
		{
			return theIter->executeAt;
		}
	}

	NN_LOG_ERROR("Unable to find work {}", theID);
	return kNTimeDistantFuture;
}





//=============================================================================
//		NRunLoop::SetWorkTime :Set the execution time for work.
//-----------------------------------------------------------------------------
void NRunLoop::SetWorkTime(NRunLoopWorkID theID, NTime theTime)
{


	// Validate our parameters
	NN_REQUIRE(theID != NRunLoopWorkNone);



	// Set the execution time
	//
	// Changing the execution time may imply that new work should
	// be executed, so we must wake the runloop to process it.
	NScopedLock acquireLock(mLock);

	for (auto theIter = mWork.begin(); theIter != mWork.end(); theIter++)
	{
		if (theIter->theID == theID)
		{
			theIter->executeAt = theTime;
			mSortWork          = true;

			RunLoopWake(mRunLoop);
			return;
		}
	}

	NN_LOG_ERROR("Unable to find work {}", theID);
}





//=============================================================================
//		NRunLoop::GetWorkInterval : Get the execution interval for work.
//-----------------------------------------------------------------------------
NInterval NRunLoop::GetWorkInterval(NRunLoopWorkID theID) const
{


	// Validate our parameters
	NN_REQUIRE(theID != NRunLoopWorkNone);



	// Get the execution interval
	NScopedLock acquireLock(mLock);

	for (auto theIter = mWork.begin(); theIter != mWork.end(); theIter++)
	{
		if (theIter->theID == theID)
		{
			return theIter->executeEvery;
		}
	}

	NN_LOG_ERROR("Unable to find work {}", theID);
	return kNTimeNone;
}





//=============================================================================
//		NRunLoop::SetWorkInterval : Set the execution interval for work.
//-----------------------------------------------------------------------------
void NRunLoop::SetWorkInterval(NRunLoopWorkID theID, NInterval theInterval)
{


	// Validate our parameters
	NN_REQUIRE(theID != NRunLoopWorkNone);



	// Set the execution interval
	NScopedLock acquireLock(mLock);

	for (auto theIter = mWork.begin(); theIter != mWork.end(); theIter++)
	{
		if (theIter->theID == theID)
		{
			theIter->executeEvery = theInterval;
			return;
		}
	}

	NN_LOG_ERROR("Unable to find work {}", theID);
}





//=============================================================================
//		NRunLoop::GetMain : Get the main thread's runloop.
//-----------------------------------------------------------------------------
NSharedRunLoop NRunLoop::GetMain()
{


	// Get the runloop
	static auto sRunLoop = std::make_shared<NRunLoop>(true);

	return sRunLoop;
}





//=============================================================================
//		NRunLoop::GetCurrent : Get the current thread's runloop.
//-----------------------------------------------------------------------------
NSharedRunLoop NRunLoop::GetCurrent()
{


	// Get the runloop
	if (NThread::IsMain())
	{
		return GetMain();
	}
	else
	{
		static thread_local auto sRunLoop = std::make_shared<NRunLoop>(false);

		return sRunLoop;
	}
}





#pragma mark private
//=============================================================================
//		NRunLoop::PerformWork : Perform work.
//-----------------------------------------------------------------------------
void NRunLoop::PerformWork(bool onlyOne)
{


	// Perform the work
	NRunLoopWork theWork;

	while (FetchNextWork(theWork))
	{
		// Perform the work
		theWork.theFunction();

		if (theWork.theSemaphore != nullptr)
		{
			theWork.theSemaphore->Signal();
		}



		// Stop after one if necessary
		if (onlyOne)
		{
			break;
		}
	}
}





//=============================================================================
//		NRunLoop::FinishedSleep : Sleep until the next work / timeout.
//-----------------------------------------------------------------------------
bool NRunLoop::FinishedSleep(NTime endTime)
{


	// Calculate the wake time
	//
	// We want to sleep until the end time, or the next work is due.
	mLock.Lock();

	const NRunLoopWork* nextWork = GetNextWork();
	NTime               wakeTime = endTime;

	if (nextWork != nullptr)
	{
		wakeTime = std::min(wakeTime, nextWork->executeAt);
	}

	mLock.Unlock();



	// Calculate the sleep time
	//
	// We always sleep, even if work is due, to ensure that platforms with
	// an underlying native runloop always get a chance to handle events.
	//
	// If we're waiting for the distant future then we wait forever to let
	// the native runloop sleep without an associated timer.
	NInterval sleepFor = wakeTime - NTimeUtils::GetTime();

	if (sleepFor < 0.0)
	{
		sleepFor = 0.0;
	}
	else if (wakeTime >= kNTimeDistantFuture)
	{
		sleepFor = kNTimeForever;
	}



	// Sleep the runloop
	//
	// Sleeping is never exact so we must re-check the end time when we wake.
	RunLoopSleep(mRunLoop, sleepFor);

	return NTimeUtils::GetTime() >= endTime;
}





//=============================================================================
//		NRunLoop::FetchNextWork : Fetch the next work to perform.
//-----------------------------------------------------------------------------
bool NRunLoop::FetchNextWork(NRunLoopWork& theWork)
{


	// Determine if we have work
	NScopedLock acquireLock(mLock);

	NRunLoopWork* nextWork = GetNextWork();
	bool          haveWork = (nextWork != nullptr);
	NTime         timeNow  = NTimeUtils::GetTime();

	if (haveWork)
	{
		haveWork = (nextWork->executeAt <= timeNow);
	}



	// Fetch the work
	//
	// Repeating work is re-scheduled using the existing work item,
	// and will be sorted into the correct place on the next pass.
	if (haveWork)
	{
		theWork = *nextWork;

		if (nextWork->executeEvery == kNTimeNone)
		{
			mWork.pop_front();
		}
		else
		{
			nextWork->executeAt = timeNow + nextWork->executeEvery;
			mSortWork           = true;
		}
	}

	return haveWork;
}





//=============================================================================
//		NRunLoop::GetNextWork : Get the next work in the queue.
//-----------------------------------------------------------------------------
NRunLoopWork* NRunLoop::GetNextWork()
{


	// Validate our state
	NN_REQUIRE(mLock.IsLocked());



	// Check our state
	if (mWork.empty() || *mStopWork)
	{
		return nullptr;
	}



	// Sort the work
	//
	// As work can arrive at any time the queue must be sorted if dirty.
	//
	// We use a stable sort to preserve submit order in the event of equal
	// timestamps.
	if (mSortWork)
	{
		nstd::stable_sort(mWork,
						  [](const NRunLoopWork& workA, const NRunLoopWork& workB)
						  {
							  return workA.executeAt < workB.executeAt;
						  });

		mSortWork = false;
	}



	// Get the next work
	return &mWork.front();
}
