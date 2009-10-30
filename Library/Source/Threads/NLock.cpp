/*	NAME:
		NLock.cpp

	DESCRIPTION:
		Thread locks.
	
	COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NThreadUtilities.h"
#include "NMathUtilities.h"
#include "NLock.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const SInt32 kSpinLoopTime										= 300;
static const UInt32 kSpinLockMask										= (1 << 0);





//============================================================================
//		NLock::NLock : Constructor.
//----------------------------------------------------------------------------
NLock::NLock(void)
{


	// Initialize ourselves
	mCount = 0;
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
	// We do not need to protect against a write happening while we read,
	// since we do not offer any synchronisation.
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
		NThreadUtilities::AtomicAdd32(mCount,  1);
	else
		NThreadUtilities::AtomicAdd32(mCount, -1);
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





//============================================================================
//		NMutexLock::NMutexLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NMutexLock::NMutexLock(void)
{	OSStatus	theErr;



	// Initialize ourselves
	theErr = MPCreateCriticalRegion(&mLock);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMutexLock::~NMutexLock : Destructor.
//----------------------------------------------------------------------------
NMutexLock::~NMutexLock(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mLock != NULL);



	// Clean up
	theErr = MPDeleteCriticalRegion(mLock);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMutexLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NMutexLock::Lock(EventTime theTime)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mLock != NULL);



	// Acquire the lock
	theErr = MPEnterCriticalRegion(mLock, NSystemUtilities::EventTimeToDuration(theTime));
	NN_ASSERT(theErr == noErr || theErr == kMPTimeoutErr);



	// Update our state
	if (theErr == noErr)
		AdjustLock(true);
	
	return(theErr == noErr);
}





//============================================================================
//		NMutexLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NMutexLock::Unlock(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsLocked());
	NN_ASSERT(mLock != NULL);
	


	// Release the lock
	AdjustLock(false);

	theErr = MPExitCriticalRegion(mLock);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NReadWriteLock::NReadWriteLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NReadWriteLock::NReadWriteLock(void)
{	OSStatus	theErr;



	// Initialize ourselves
	theErr = pthread_rwlock_init(&mLock, NULL);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NReadWriteLock::~NReadWriteLock : Destructor.
//----------------------------------------------------------------------------
NReadWriteLock::~NReadWriteLock(void)
{	OSStatus	theErr;



	// Clean up
	theErr = pthread_rwlock_destroy(&mLock);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NReadWriteLock::Lock : Acquire the lock for reading.
//----------------------------------------------------------------------------
bool NReadWriteLock::Lock(EventTime theTime)
{	EventTime	stopTime;
	OSStatus	theErr;



	// Acquire the lock
	//
	// A spin loop is less efficient than blocking, but sufficient for our needs.
	if (NMathUtilities::AreEqual(theTime, kEventDurationForever))
		theErr = pthread_rwlock_rdlock(&mLock);
	else
		{
		stopTime = GetCurrentEventTime() + theTime;
		do
			{
			theErr = pthread_rwlock_tryrdlock(&mLock);
			if (theErr != noErr)
				usleep(kSpinLoopTime);
			}
		while (theErr != noErr && GetCurrentEventTime() < stopTime);
		}



	// Update our state
	if (theErr == noErr)
		AdjustLock(true);
	
	return(theErr == noErr);
}





//============================================================================
//		NReadWriteLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NReadWriteLock::Unlock(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsLocked());



	// Release the lock
	AdjustLock(false);

	theErr = pthread_rwlock_unlock(&mLock);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NReadWriteLock::LockForWrite : Acquire the lock for writing.
//----------------------------------------------------------------------------
bool NReadWriteLock::LockForWrite(EventTime theTime)
{	EventTime	stopTime;
	OSStatus	theErr;



	// Acquire the lock
	//
	// A spin loop is less efficient than blocking, but sufficient for our needs.
	if (NMathUtilities::AreEqual(theTime, kEventDurationForever))
		theErr = pthread_rwlock_wrlock(&mLock);
	else
		{
		stopTime = GetCurrentEventTime() + theTime;
		do
			{
			theErr = pthread_rwlock_trywrlock(&mLock);
			if (theErr != noErr)
				usleep(kSpinLoopTime);
			}
		while (theErr != noErr && GetCurrentEventTime() < stopTime);
		}


	// Update our state
	if (theErr == noErr)
		AdjustLock(true);

	return(theErr == noErr);
}





//============================================================================
//		NSpinLock::NSpinLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NSpinLock::NSpinLock(void)
{


	// Initialize ourselves
	mLock = 0;
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
bool NSpinLock::Lock(EventTime theTime)
{	EventTime	stopTime;
	bool		gotLock;



	// Acquire the lock
	//
	// A spin loop is less efficient than blocking, but sufficient for our needs.
	if (NMathUtilities::AreEqual(theTime, kEventDurationForever))
		{
		while (!SpinLockTry(mLock))
			usleep(kSpinLoopTime);

		gotLock = true;
		}
	else
		{
		stopTime = GetCurrentEventTime() + theTime;
		gotLock  = false;
		do
			{
			gotLock = SpinLockTry(mLock);
			if (!gotLock)
				usleep(kSpinLoopTime);
			}
		while (!gotLock && GetCurrentEventTime() < stopTime);
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

	SpinLockUnlock(mLock);
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



