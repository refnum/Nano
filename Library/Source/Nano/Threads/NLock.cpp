/*	NAME:
		NLock.cpp

	DESCRIPTION:
		Lock objects.
	
	COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadUtilities.h"
#include "NMathUtilities.h"
#include "NTimeUtilities.h"
#include "NTargetThread.h"
#include "NThread.h"
#include "NLock.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt32 kSpinLockMask										= (1 << 0);





//============================================================================
//		NLock::NLock : Constructor.
//----------------------------------------------------------------------------
NLock::NLock(void)
{


	// Initialize ourselves
	mCount = 0;
	mLock  = kNLockRefNone;
}





//============================================================================
//		NLock::~NLock : Destructor.
//----------------------------------------------------------------------------
NLock::~NLock(void)
{
}





//============================================================================
//		NLock::IsLocked : Is the lock acquired?
//----------------------------------------------------------------------------
bool NLock::IsLocked(void) const
{


	// Check our state
	//
	// We do not need to protect against a write happening while
	// we read, since we do not offer any synchronisation.
	return(mCount != 0);
}





//============================================================================
//		NLock::AdjustLock : Adjust the lock.
//----------------------------------------------------------------------------
void NLock::AdjustLock(bool didLock)
{


	// Validate our state
	NN_ASSERT(mCount >= 0);



	// Update our state
	if (didLock)
		mCount++;
	else
		mCount--;
}





//============================================================================
//		NMutexLock::NMutexLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NMutexLock::NMutexLock(void)
{


	// Initialize ourselves
	mLock = NTargetThread::MutexCreate();
}





//============================================================================
//		NMutexLock::~NMutexLock : Destructor.
//----------------------------------------------------------------------------
NMutexLock::~NMutexLock(void)
{


	// Validate our state
	NN_ASSERT(mLock != kNLockRefNone);



	// Clean up
	NTargetThread::MutexDestroy(mLock);
}





//============================================================================
//		NMutexLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NMutexLock::Lock(NTime theTime)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(mLock != kNLockRefNone);



	// Acquire the lock
	theErr = NTargetThread::MutexLock(mLock, theTime);
	NN_ASSERT(theErr == kNoErr || theErr == kNErrTimeout);



	// Update our state
	if (theErr == kNoErr)
		AdjustLock(true);
	
	return(theErr == kNoErr);
}





//============================================================================
//		NMutexLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NMutexLock::Unlock(void)
{


	// Validate our state
	NN_ASSERT(IsLocked());
	NN_ASSERT(mLock != kNLockRefNone);
	


	// Release the lock
	AdjustLock(false);
	
	NTargetThread::MutexUnlock(mLock);
}





//============================================================================
//		NReadWriteLock::NReadWriteLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NReadWriteLock::NReadWriteLock(void)
{


	// Initialize ourselves
	mLock = NTargetThread::ReadWriteCreate();
}





//============================================================================
//		NReadWriteLock::~NReadWriteLock : Destructor.
//----------------------------------------------------------------------------
NReadWriteLock::~NReadWriteLock(void)
{


	// Validate our state
	NN_ASSERT(mLock != kNLockRefNone);



	// Clean up
	NTargetThread::ReadWriteDestroy(mLock);
}





//============================================================================
//		NReadWriteLock::Lock : Acquire the lock for reading.
//----------------------------------------------------------------------------
bool NReadWriteLock::Lock(NTime theTime)
{


	// Acquire the lock
	return(AcquireLock(theTime, false));
}





//============================================================================
//		NReadWriteLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NReadWriteLock::Unlock(void)
{


	// Validate our state
	NN_ASSERT(IsLocked());



	// Release the lock
	AdjustLock(false);
	
	NTargetThread::ReadWriteUnlock(mLock);
}





//============================================================================
//		NReadWriteLock::LockForWrite : Acquire the lock for writing.
//----------------------------------------------------------------------------
bool NReadWriteLock::LockForWrite(NTime theTime)
{


	// Acquire the lock
	return(AcquireLock(theTime, true));
}





//============================================================================
//		NReadWriteLock::AcquireLock : Acquire the lock.
//----------------------------------------------------------------------------
bool NReadWriteLock::AcquireLock(NTime theTime, bool forWrite)
{	NStatus		theErr;



	// Acquire the lock
	theErr = NTargetThread::ReadWriteLock(mLock, theTime, forWrite);
	NN_ASSERT(theErr == kNoErr || theErr == kNErrTimeout);



	// Update our state
	if (theErr == kNoErr)
		AdjustLock(true);
	
	return(theErr == kNoErr);
}





//============================================================================
//		NSpinLock::NSpinLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NSpinLock::NSpinLock(void)
{


	// Initialize ourselves
	mSpinLock = 0;
}





//============================================================================
//		NSpinLock::~NSpinLock : Destructor.
//----------------------------------------------------------------------------
NSpinLock::~NSpinLock(void)
{
}





//============================================================================
//		NSpinLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NSpinLock::Lock(NTime waitFor)
{	NTime		stopTime;
	bool		gotLock;



	// Acquire the lock
	if (NMathUtilities::AreEqual(waitFor, kNTimeForever))
		{
		while (!SpinLockTry(mSpinLock))
			NThread::Sleep(kNThreadSpinTime);

		gotLock = true;
		}
	else
		{
		stopTime = NTimeUtilities::GetTime() + waitFor;
		gotLock  = false;
		do
			{
			gotLock = SpinLockTry(mSpinLock);
			if (!gotLock)
				NThread::Sleep(kNThreadSpinTime);
			}
		while (!gotLock && NTimeUtilities::GetTime() < stopTime);
		}



	// Update our state
	if (gotLock)
		AdjustLock(true);
	
	return(gotLock);
}





//============================================================================
//		NSpinLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NSpinLock::Unlock(void)
{


	// Validate our state
	NN_ASSERT(IsLocked());
	


	// Release the lock
	AdjustLock(false);

	SpinLockUnlock(mSpinLock);
}





//============================================================================
//		NSpinLock::SpinLockTry : Try and acquire the lock.
//----------------------------------------------------------------------------
bool NSpinLock::SpinLockTry(UInt32 &theLock)
{


	// Validate our parameters
	NN_ASSERT(((UInt32) &theLock) % 4U == 0U);
	


	// Try and acquire the lock
	return(NThreadUtilities::AtomicCompareAndSwap32(theLock, 0, kSpinLockMask));
}





//============================================================================
//		NSpinLock::SpinLockUnlock : Unlock a spin lock.
//----------------------------------------------------------------------------
void NSpinLock::SpinLockUnlock(UInt32 &theLock)
{


	// Validate our parameters
	NN_ASSERT(((UInt32) &theLock) % 4U == 0U);
	NN_ASSERT(theLock == kSpinLockMask);
	


	// Release the lock
	NThreadUtilities::AtomicAnd32(theLock, ~kSpinLockMask);
}





//============================================================================
//		StLock::StLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
StLock::StLock(NLock &theLock)
		: mLock(theLock)
{


	// Acquire the lock
	mLock.Lock();
}





//============================================================================
//		StLock::~StLock : Destructor.
//----------------------------------------------------------------------------
StLock::~StLock(void)
{


	// Release the lock
	mLock.Unlock();
}

