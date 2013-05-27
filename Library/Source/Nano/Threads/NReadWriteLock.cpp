/*	NAME:
		NReadWriteLock.cpp

	DESCRIPTION:
		Read/write lock.
		
		Read/write locks are only available on Vista onwards on Windows, and
		timed pthread read/write locks are only available on Linux.
		
		As such we provide our own implementation, although it is subject to
		writer starvation.

	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetThread.h"
#include "NReadWriteLock.h"





//============================================================================
//		NReadWriteLock::NReadWriteLock : Constructor.
//----------------------------------------------------------------------------
NReadWriteLock::NReadWriteLock(void)
{


	// Initialize ourselves
	mNumReaders = 0;
}





//============================================================================
//		NReadWriteLock::~NReadWriteLock : Destructor.
//----------------------------------------------------------------------------
NReadWriteLock::~NReadWriteLock(void)
{


	// Validate our state
	NN_ASSERT(mNumReaders == 0);
}





//============================================================================
//		NReadWriteLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NReadWriteLock::Lock(NTime waitFor)
{	bool	gotLock;



	// Acquire the lock
	//
	// Only one thread can acquire the writer lock at a time.
	gotLock = mLockWriters.Lock(waitFor);
	if (gotLock)
		mLockCount++;

	return(gotLock);
}





//============================================================================
//		NReadWriteLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NReadWriteLock::Unlock(void)
{


	// Release the lock
	//
	// Once this writer is done, the writer lock is free for another reader/writer.
	mLockCount--;
	mLockWriters.Unlock();
}





//============================================================================
//		NReadWriteLock::LockForRead : Acquire the lock for reading.
//----------------------------------------------------------------------------
bool NReadWriteLock::LockForRead(NTime waitFor)
{	bool		gotLock;



	// Lock for reading
	//
	// Only one reader can update the reader state at a time.
	gotLock = mLockReaders.Lock(waitFor);
	if (gotLock)
		{
		// Start the reader
		//
		// The first reader acquires the writer lock, preventing further writers.
		mNumReaders++;
		mLockCount++;

		if (mNumReaders == 1)
			mLockWriters.Lock();



		// Clean up
		//
		// The "read lock" is essentially mNumReaders/mLockWriters - once those
		// have been set we can allow other readers to proceed.
		mLockReaders.Unlock();
		}

	return(gotLock);
}





//============================================================================
//		NReadWriteLock::UnlockForRead : Release the lock for reading.
//----------------------------------------------------------------------------
void NReadWriteLock::UnlockForRead(void)
{


	// Release the lock
	//
	// Once a reader is done we drop the reader count; the last reader releases
	// the writer lock, allowing blocked writers to proceed.
	mLockReaders.Lock();
	
		NN_ASSERT(mNumReaders >= 1);
		mNumReaders--;
		mLockCount--;

		if (mNumReaders == 0)
			mLockWriters.Unlock();
		
	mLockReaders.Unlock();
}

