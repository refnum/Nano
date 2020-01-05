/*	NAME:
		NMutex.inl

	DESCRIPTION:
		Mutex lock.

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
// Nano
#include "NDebug.h"





//=============================================================================
//		NMutex::Lock : Acquire the lock.
//-----------------------------------------------------------------------------
bool NMutex::Lock(NInterval waitFor)
{


	// Acquire the lock
	//
	// If we acquire the lock count (by taking it from 0 to 1) then we have
	// acquired the lock and can update the state directly.
	//
	// The fast path is kept inlined, without any local variables that might
	// require a stack adjustment, to get us down to 11 instructions (x86) to
	// acquire an uncontended lock.
	if (AcquireCount())
	{
		mOwner.store(NThread::GetID(), std::memory_order_relaxed);
		mRecursion++;
		return true;
	}
	else
	{
		return WaitForLock(waitFor);
	}
}





//=============================================================================
//		NMutex::Unlock : Release the lock.
//-----------------------------------------------------------------------------
void NMutex::Unlock()
{


	// Validate our state
	NN_REQUIRE(mOwner.load(std::memory_order_relaxed) == NThread::GetID());
	NN_REQUIRE(mRecursion >= 1);



	// Update our state
	mRecursion--;

	bool didUnlock = (mRecursion == 0);
	if (didUnlock)
	{
		mOwner.store(kNThreadIDNone, std::memory_order_relaxed);
	}



	// Release the lock
	//
	// We release the lock by decrementing the count. If the count is non-zero,
	// and our recursion count is also zero, we signal another thread to run.
	if (DecrementCount() > 0)
	{
		if (didUnlock)
		{
			NSemaphore::Signal(mSemaphore.load(std::memory_order_consume));
		}
	}
}





#pragma mark private
//=============================================================================
//		NMutex::AcquireCount : Acquire the lock count.
//-----------------------------------------------------------------------------
bool NMutex::AcquireCount()
{


	// Acquire the count
	//
	// A thread that takes the count from 0 to 1 acquires the lock.
	uint32_t oldCount = 0;
	uint32_t newCount = 1;

	return mLockCount.compare_exchange_strong(oldCount,
											  newCount,
											  std::memory_order_acquire,
											  std::memory_order_relaxed);
}





//=============================================================================
//		NMutex::ReleaseCount : Release the lock count.
//-----------------------------------------------------------------------------
bool NMutex::ReleaseCount()
{


	// Release the count
	//
	// If we time out waiting for the lock we must decrement the count
	// to indicate we are no longer waiting for it.
	//
	// A release is conditional, as we have not acquired the lock and
	// so we may not be able to decrement our count on the first go.
	uint32_t oldCount = mLockCount.load(std::memory_order_relaxed);
	uint32_t newCount = oldCount - 1;

	NN_REQUIRE(oldCount >= 1);

	return mLockCount.compare_exchange_strong(oldCount,
											  newCount,
											  std::memory_order_relaxed,
											  std::memory_order_relaxed);
}





//=============================================================================
//		NMutex::IncrementCount : Increment the lock count.
//-----------------------------------------------------------------------------
uint32_t NMutex::IncrementCount()
{


	// Increment the count
	//
	// Returns the new count.
	uint32_t oldCount = mLockCount.fetch_add(1, std::memory_order_acquire);
	NN_REQUIRE(oldCount < kNUInt32Max);

	return oldCount + 1;
}





//=============================================================================
//		NMutex::DecrementCount : Decrement the lock count.
//-----------------------------------------------------------------------------
uint32_t NMutex::DecrementCount()
{


	// Decrement the count
	//
	// Returns the new count.
	uint32_t oldCount = mLockCount.fetch_sub(1, std::memory_order_release);
	NN_REQUIRE(oldCount >= 1);

	return oldCount - 1;
}
