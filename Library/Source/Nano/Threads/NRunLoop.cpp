/*	NAME:
		NRunLoop.cpp

	DESCRIPTION:
		Thread runloop.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#include "NTimeUtils.h"





//=============================================================================
//		NRunLoop::NRunLoop : Constructor.
//-----------------------------------------------------------------------------
NRunLoop::NRunLoop()
	: mLock()
	, mOwnerID(NThreadID::Get())
	, mNewWork(false)
	, mNextID(NRunLoopWorkNone)
	, mWork{}
{
}





//=============================================================================
//		NRunLoop::Run : Run the runloop.
//-----------------------------------------------------------------------------
void NRunLoop::Run(NInterval /*runFor*/)
{
}





//=============================================================================
//		NRunLoop::Stop : Stop the runloop.
//-----------------------------------------------------------------------------
void NRunLoop::Stop()
{
}





//=============================================================================
//		NRunLoop::Add : Add a function.
//-----------------------------------------------------------------------------
NRunLoopWorkID NRunLoop::Add(const NRunLoopWorkFunction&  theFunctor,
							 NInterval                    executeAfter,
							 NInterval                    executeEvery,
							 std::shared_ptr<NSemaphore>* theSemaphore)
{


	// Validate our parameters and state
	NN_REQUIRE(theFunctor != nullptr);
	NN_REQUIRE(executeAfter >= 0.0);
	NN_REQUIRE(executeEvery >= 0.0);

	NN_REQUIRE(mOwnerID == NThreadID::Get());



	// Get the state we need
	NRunLoopWork theWork{};

	theWork.theID        = NRunLoopWorkNone;
	theWork.executeAt    = NTimeUtils::GetTime() + executeAfter;
	theWork.executeEvery = executeEvery;
	theWork.theFunction  = theFunctor;

	if (theSemaphore != nullptr)
	{
		theWork.theSemaphore = *theSemaphore;
	}



	// Add the work
	NScopedLock acquireLock(mLock);

	mNewWork = true;
	mNextID += 1;

	theWork.theID = mNextID;
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
	NScopedLock acquireLock(mLock);

	for (auto theIter = mWork.begin(); theIter != mWork.end(); theIter++)
	{
		if (theIter->theID == theID)
		{
			mNewWork           = true;
			theIter->executeAt = theTime;
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
			mNewWork              = true;
			theIter->executeEvery = theInterval;
			return;
		}
	}

	NN_LOG_ERROR("Unable to find work {}", theID);
}
