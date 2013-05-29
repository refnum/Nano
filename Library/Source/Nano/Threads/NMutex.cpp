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
			
			http://old.nabble.com/Mutex-performance-td24892454.html

	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThread.h"
#include "NTargetThread.h"
#include "NMutex.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NIndex kMutexSpinUntilBlock							= 5000;





//============================================================================
//		NMutex::NMutex : Constructor.
//----------------------------------------------------------------------------
NMutex::NMutex(void)
{


	// Initialize ourselves
	mOwnerID   = kNThreadIDNone;
	mRecursion = 0;
	mSemaphore = kNSemaphoreRefNone;
}





//============================================================================
//		NMutex::~NMutex : Destructor.
//----------------------------------------------------------------------------
NMutex::~NMutex(void)
{


	// Clean up
	if (mSemaphore != kNSemaphoreRefNone)
		NTargetThread::SemaphoreDestroy(mSemaphore);
}





//============================================================================
//		NMutex::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NMutex::Lock(NTime waitFor)
{	NSemaphoreRef	theSemaphore;
	NThreadID		thisThread;
	bool			gotLock;
	NIndex			n;



	// Get the state we need
	thisThread = NThread::GetID();
	gotLock    = false;



	// Attempt to acquire the lock
	//
	// If we appear to own the lock then we don't bother spinning.
	//
	// We still need to increment the counter before we can be sure this thread really
	// has it, but if we are being called recursively spinning would just waste time.
	//
	//
	// If we don't appear to own the lock then we'll lock it if we're the first thread
	// to swap the counter from 0 to 1.
	//
	// If the mutex is locked then we'll obtain ownership if another thread drops the
	// counter to 0 while we're spinning (allowing us to raise it to 1).
	//
	// We employ a simple back-off strategy, starting with a yield (0) then using an
	// extra microsecond on subsequent loops.
	if (!NThread::AreEqual(thisThread, mOwnerID))
		{
		for (n = 0; n < kMutexSpinUntilBlock; n++)
			{
			gotLock = NTargetThread::AtomicCompareAndSwap32(mLockCount, 0, 1);
			if (gotLock)
				break;

			NThread::Sleep(n * kNTimeMicrosecond);
			}
		}



	// Acquire the lock
	if (!gotLock)
		{
		// Create the semaphore
		//
		// Once we raise the count beyond 1 then whatever thread currently has the lock will
		// need to be able to set the semaphore to wake us after it decrements the count.
		//
		// Although deferring creation like this means that potentially we might create a
		// redundant semaphore, in practice this happens very rarely.
		//
		// It is also safe - even if two threads create a semaphore, only one of them will
		// assign it (the other will just release theirs).
		if (mSemaphore == kNSemaphoreRefNone)
			{
			theSemaphore = NTargetThread::SemaphoreCreate(0);

			if (!NTargetThread::AtomicCompareAndSwapPtr(mSemaphore, kNSemaphoreRefNone, theSemaphore))
				NTargetThread::SemaphoreDestroy(theSemaphore);
			}



		// Raise the count
		//
		// If raising the count takes it to exactly 1 then we've just acquired the lock.
		//
		// We have to raise the count to indicate our interest in the lock, but if it's
		// now exactly 1 then it's been unlocked between our spinlock attempt and now.
		gotLock = (NTargetThread::AtomicAdd32(mLockCount, 1) == 1);
		if (!gotLock)
			{
			// Check the owner
			//
			// The count is now 2+, but if we're the owning thread we already have the lock.
			gotLock = NThread::AreEqual(thisThread, mOwnerID);
			if (!gotLock)
				{
				// Wait for the semaphore
				//
				// Otherwise we have to wait - the current owner will set it when they're
				// done, allowing us to wake and take ownership.
				gotLock = NTargetThread::SemaphoreWait(mSemaphore, waitFor);
				}
			}
		}



	// Update our state
	if (gotLock)
		{
		mOwnerID = thisThread;
		mRecursion++;
		}

	return(gotLock);
}





//============================================================================
//      NMutex::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NMutex::Unlock(void)
{	NIndex		newRecursion;



	// Get the state we need
	NN_ASSERT(NThread::AreEqual(mOwnerID, NThread::GetID()));
	NN_ASSERT(mRecursion >= 1);



	// Update the lock
	//
	// The lock state must be reset before decrementing the count; once the count
	// falls to 0 the lock will be open for other threads.
	newRecursion = --mRecursion;
	
	if (newRecursion == 0)
		mOwnerID = kNThreadIDNone;



	// Release the lock
	//
	// If there are 1+ threads waiting on the lock after we decrement the count then,
	// provided none of them are us, we can wake one by setting the semaphore.
	if (NTargetThread::AtomicAdd32(mLockCount, -1) > 0)
		{
		if (newRecursion == 0)
			NTargetThread::SemaphoreSignal(mSemaphore);
		}
}



