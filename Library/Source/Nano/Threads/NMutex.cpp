/*	NAME:
		NMutex.cpp

	DESCRIPTION:
		Implements a 'benaphore' recursive mutex lock.

		In theory most OS mutex implementations will avoid an expensive trip to the
		kernel if there is little or no contention.

		In practice the Mac OS X implementation of pthread_mutex is extremely slow.

		4 threads acquiring a shared lock 1,000,000 times (with a small amount of
		work within the lock) were seen, on a 2.2Ghz Core i7 running 10.8, to produce
		times of:

			Mac NMutex						0.12s
			Mac pthread_mutex			   15.31s

			Windows NMutex					0.11s
			Windows Critical Section		0.10s
			Windows Semaphore				3.74s

			Linux NMutex					0.14s
			Linux pthread_mutex				0.23s

		A Windows critical section uses the same technique as an NMutex (atomic spin
		before blocking), so has effectively the same performance.

		Benchmarking threaded code is often difficult (as it's not clear if slow times
		are indicative of a problem in the benchmark), however two similar experiences
		with Mac OS X are at:

			http://www.mr-edd.co.uk/blog/sad_state_of_osx_pthread_mutex_t

			http://old.nabble.com/Mutex-performance-td24892454.html.

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
#include "NMutex.h"

// Nano
#include "NThread.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kNMutexSpinCount                    = 5000;





//=============================================================================
//		NMutex::NMutex : Constructor.
//-----------------------------------------------------------------------------
NMutex::NMutex()
	: mSemaphore(kNSemaphoreNone)
	, mOwner()
	, mLockCount(0)
	, mRecursion(0)
{
}





//=============================================================================
//		NMutex::~NMutex : Destructor.
//-----------------------------------------------------------------------------
NMutex::~NMutex()
{


	// Clean up
	auto theSemaphore = mSemaphore.load(std::memory_order_consume);

	if (theSemaphore != kNSemaphoreNone)
	{
		NSemaphore::Destroy(theSemaphore);
	}
}





//=============================================================================
//		NMutex::IsLocked : Is the lock locked?
//-----------------------------------------------------------------------------
bool NMutex::IsLocked() const
{


	// Check our state
	return mLockCount != 0;
}





#pragma mark private
//=============================================================================
//		NMutex::WaitForLock : Wait for the lock.
//-----------------------------------------------------------------------------
bool NMutex::WaitForLock(NInterval waitFor)
{


	// Check the owner
	//
	// If we're already the owner then we have acquired the lock recursively.
	NThreadID currentOwner = mOwner.load(std::memory_order_relaxed);
	NThreadID thisThread   = NThreadID::Get();

	if (NN_EXPECT_UNLIKELY(currentOwner == thisThread))
	{
		IncrementCount();
		mRecursion++;
		return true;
	}



	// Spin for the lock
	//
	// We acquire the lock by being the first thread to take the count
	// from 0 to 1.
	//
	// We make at one further attempt to acquire the lock, then perform
	// a short spin if we're allowed to wait at all.
	size_t spinFor = (waitFor == kNTimeNone) ? 1 : kNMutexSpinCount;
	bool   gotLock = false;

	for (size_t n = 0; n < spinFor && !gotLock; n++)
	{
		gotLock = AcquireCount();
		if (!gotLock)
		{
			NThread::Pause();
		}
	}



	// Wait for the lock
	//
	// If we've failed to acquire the lock then we'll have to wait.
	if (!gotLock && waitFor != kNTimeNone)
	{
		// Create the semaphore
		//
		// The semaphore is created on demand to ensure a mutex can be
		// created without any additional overhead until it is waited on.
		//
		// We must swap the semaphore into place atomically, and discard
		// our semaphore if we lose, as multiple threads may attempt to
		// create the semaphore for the mutex.
		if (mSemaphore.load(std::memory_order_relaxed) == kNSemaphoreNone)
		{
			NSemaphoreRef oldSemaphore = kNSemaphoreNone;
			NSemaphoreRef newSemaphore = NSemaphore::Create(0);

			if (!mSemaphore.compare_exchange_strong(oldSemaphore,
													newSemaphore,
													std::memory_order_release,
													std::memory_order_relaxed))
			{
				NSemaphore::Destroy(newSemaphore);
			}
		}



		// Sleep the thread
		//
		// We raise the count to mark our interest in the lock.
		//
		// If the count is 1 then we know that, although it was locked on our
		// prior attempts, the mutex has since been unlocked and we have just
		// acquired it.
		//
		// If the count is higher than 1 then the lock is still being held and
		// so we need to wait for the owner to signal the semaphore.
		gotLock = (IncrementCount() == 1);
		if (!gotLock)
		{
			gotLock = WaitForSemaphore(waitFor);
		}
	}



	// Update our state
	if (gotLock)
	{
		mOwner.store(thisThread, std::memory_order_relaxed);
		mRecursion++;
	}

	return gotLock;
}





//=============================================================================
//		NMutex::WaitForSemaphore : Wait for the semaphore.
//-----------------------------------------------------------------------------
bool NMutex::WaitForSemaphore(NInterval waitFor)
{


	// Wait for the semaphore
	auto theSemaphore = mSemaphore.load(std::memory_order_consume);
	bool gotLock      = NSemaphore::Wait(theSemaphore, waitFor);

	if (!gotLock)
	{
		// Handle timeout
		//
		// If we timed out waiting for the semaphore then we must decrement
		// the lock count to indicate we are no longer waiting on the lock.
		//
		// This balances the previous increment of the count just before we
		// waited on the semaphore.
		//
		//
		// However, if the count is currently 1 then the lock must have been
		// released (and the semaphore signalled) at some point after we
		// returned from timing out on the semaphore.
		//
		// This leaves the semaphore set, so we must take it again to consume
		// that signal.
		//
		// If that second attempt succeeds then we have acquired the lock
		// after all.
		//
		// If the scond attempt fails then we must loop and try to decrement
		// our count again, as the same scenario could repeat between returning
		// from the Wait and checking the lock count.
		while (true)
		{
			// Decrement the count
			if (mLockCount.load(std::memory_order_relaxed) > 1 && ReleaseCount())
			{
				gotLock = false;
				break;
			}


			// Poll the semaphore
			if (NSemaphore::Wait(theSemaphore, kNTimeNone))
			{
				gotLock = true;
				break;
			}


			// Spinning
			NThread::Pause();
		}
	}

	return gotLock;
}
