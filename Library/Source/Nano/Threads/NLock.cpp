/*	NAME:
		NLock.cpp

	DESCRIPTION:
		Lock objects.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
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
//		NSpinLock::NMutexLock : Constructor.
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
//		NReadWriteLock::Lock : Acquire the lock for writing.
//----------------------------------------------------------------------------
bool NReadWriteLock::Lock(NTime theTime)
{


	// Acquire the lock
	return(Lock(false, theTime));
}





//============================================================================
//		NReadWriteLock::Unlock : Release the lock for writing.
//----------------------------------------------------------------------------
void NReadWriteLock::Unlock(void)
{


	// Release the lock
	Unlock(false);
}





//============================================================================
//		NReadWriteLock::LockForRead : Acquire the lock for reading.
//----------------------------------------------------------------------------
bool NReadWriteLock::LockForRead(NTime theTime)
{


	// Acquire the lock
	return(Lock(true, theTime));
}





//============================================================================
//		NReadWriteLock::UnlockForRead : Release the lock for reading.
//----------------------------------------------------------------------------
void NReadWriteLock::UnlockForRead(void)
{


	// Release the lock
	Unlock(true);
}





//============================================================================
//		NReadWriteLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NReadWriteLock::Lock(bool forRead, NTime theTime)
{	NStatus		theErr;



	// Acquire the lock
	theErr = NTargetThread::ReadWriteLock(mLock, forRead, theTime);
	NN_ASSERT(theErr == kNoErr || theErr == kNErrTimeout);



	// Update our state
	if (theErr == kNoErr)
		AdjustLock(true);
	
	return(theErr == kNoErr);
}





//============================================================================
//		NReadWriteLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NReadWriteLock::Unlock(bool forRead)
{


	// Validate our state
	NN_ASSERT(IsLocked());



	// Release the lock
	AdjustLock(false);
	
	NTargetThread::ReadWriteUnlock(mLock, forRead);
}





//============================================================================
//		NSpinLock::NSpinLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NSpinLock::NSpinLock(void)
{


	// Initialize ourselves
	mLock = NTargetThread::SpinCreate();
}





//============================================================================
//		NSpinLock::~NSpinLock : Destructor.
//----------------------------------------------------------------------------
NSpinLock::~NSpinLock(void)
{


	// Validate our state
	NN_ASSERT(mLock != kNLockRefNone);



	// Clean up
	NTargetThread::SpinDestroy(mLock);
}





//============================================================================
//		NSpinLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NSpinLock::Lock(NTime theTime)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(mLock != kNLockRefNone);



	// Acquire the lock
	theErr = NTargetThread::SpinLock(mLock, theTime);
	NN_ASSERT(theErr == kNoErr || theErr == kNErrTimeout);



	// Update our state
	if (theErr == kNoErr)
		AdjustLock(true);
	
	return(theErr == kNoErr);
}





//============================================================================
//		NSpinLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NSpinLock::Unlock(void)
{


	// Validate our state
	NN_ASSERT(IsLocked());
	NN_ASSERT(mLock != kNLockRefNone);
	


	// Release the lock
	AdjustLock(false);
	
	NTargetThread::SpinUnlock(mLock);
}





//============================================================================
//		StLock::StLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
StLock::StLock(NLock &theLock)
{


	// Acquire the lock
	mLock = &theLock;
	mLock->Lock();
	
	NN_ASSERT(mLock->IsLocked());
}





//============================================================================
//		StLock::~StLock : Destructor.
//----------------------------------------------------------------------------
StLock::~StLock(void)
{


	// Release the lock
	mLock->Unlock();
}

