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
	
			Mac NMutex						0.17s
			Mac pthread_mutex			   15.31s
	
			Windows NMutex					0.12s
			Windows Critical Section		0.10s
			Windows Semaphore				3.74s
	
			Linux NMutex					0.12s
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
	mSemaphore = NTargetThread::SemaphoreCreate(0);
}





//============================================================================
//		NMutex::~NMutex : Destructor.
//----------------------------------------------------------------------------
NMutex::~NMutex(void)
{


	// Clean up
	NTargetThread::SemaphoreDestroy(mSemaphore);
}





//============================================================================
//		NMutex::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NMutex::Lock(NTime waitFor)
{	NThreadID		thisThread;
	bool			gotLock;
	NStatus			theErr;
	NIndex			n;



	// Get the state we need
	thisThread = NThread::GetID();
	gotLock    = false;



	// Attempt to acquire the lock
	//
	// If the thread is unlocked then we will be the first to swap the counter from
	// 0 to 1 - and so obtain ownership.
	//
	// If the thread is locked then we will obtain ownership if another thread drops
	// the count to 0 while we're waiting (allowing us to raise it to 1).
	for (n = 0; n < kMutexSpinUntilBlock; n++)
		{
		gotLock = NTargetThread::AtomicCompareAndSwap32(mLockCount, 0, 1);
		if (gotLock)
			break;

		NThread::Sleep(0.0);
		}



	// Acquire the lock
	if (!gotLock)
		{
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
				theErr  = NTargetThread::SemaphoreWait(mSemaphore, waitFor);
				gotLock = (theErr == kNoErr);
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



