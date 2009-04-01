/*	NAME:
		NMacLock.cpp

	DESCRIPTION:
		Mac lock objects.
	
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
#include "NTimeUtilities.h"
#include "NMathUtilities.h"
#include "NLock.h"





//============================================================================
//		NMutexLock::NMutexLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NMutexLock::NMutexLock(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(sizeof(NLockRef) == sizeof(MPCriticalRegionID));



	// Initialize ourselves
	theErr = MPCreateCriticalRegion((MPCriticalRegionID *) &mLock);
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
	theErr = MPDeleteCriticalRegion((MPCriticalRegionID) mLock);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMutexLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NMutexLock::Lock(NTime theTime)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mLock != NULL);



	// Acquire the lock
	theErr = MPEnterCriticalRegion((MPCriticalRegionID) mLock, NSystemUtilities::EventTimeToDuration(theTime));
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

	theErr = MPExitCriticalRegion((MPCriticalRegionID) mLock);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NReadWriteLock::NReadWriteLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NReadWriteLock::NReadWriteLock(void)
{	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(sizeof(NLockRef) == sizeof(pthread_rwlock_t));
	


	// Initialize ourselves
	theErr = pthread_rwlock_init((pthread_rwlock_t *) &mLock, NULL);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NReadWriteLock::~NReadWriteLock : Destructor.
//----------------------------------------------------------------------------
NReadWriteLock::~NReadWriteLock(void)
{	OSStatus	theErr;



	// Clean up
	theErr = pthread_rwlock_destroy((pthread_rwlock_t *) &theLock);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NReadWriteLock::Lock : Acquire the lock for reading.
//----------------------------------------------------------------------------
bool NReadWriteLock::Lock(NTime theTime)
{	NTime		stopTime;
	OSStatus	theErr;



	// Acquire the lock
	//
	// A spin loop is less efficient than blocking, but sufficient for our needs.
	if (NMathUtilities::AreEqual(theTime, kEventDurationForever))
		theErr = pthread_rwlock_rdlock((pthread_rwlock_t *) &mLock);
	else
		{
		stopTime = NTimeUtilities::GetTime() + theTime;
		do
			{
			theErr = pthread_rwlock_tryrdlock((pthread_rwlock_t *) &mLock);
			if (theErr != noErr)
				NThreadUtilities::Sleep(kNThreadSpinTime);
			}
		while (theErr != noErr && NTimeUtilities::GetTime() < stopTime);
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

	theErr = pthread_rwlock_unlock((pthread_rwlock_t *) &mLock);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NReadWriteLock::LockForWrite : Acquire the lock for writing.
//----------------------------------------------------------------------------
bool NReadWriteLock::LockForWrite(NTime theTime)
{	NTime		stopTime;
	OSStatus	theErr;



	// Acquire the lock
	//
	// A spin loop is less efficient than blocking, but sufficient for our needs.
	if (NMathUtilities::AreEqual(theTime, kEventDurationForever))
		theErr = pthread_rwlock_wrlock((pthread_rwlock_t *) &mLock);
	else
		{
		stopTime = NTimeUtilities::GetTime() + theTime;
		do
			{
			theErr = pthread_rwlock_trywrlock((pthread_rwlock_t *) &mLock);
			if (theErr != noErr)
				NThreadUtilities::Sleep(kNThreadSpinTime);
			}
		while (theErr != noErr && NTimeUtilities::GetTime() < stopTime);
		}


	// Update our state
	if (theErr == noErr)
		AdjustLock(true);

	return(theErr == noErr);
}




