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
#include "NMathUtilities.h"
#include "NTimeUtilities.h"
#include "NTargetThread.h"
#include "NThread.h"
#include "NLock.h"





//============================================================================
//		NLock::NLock : Constructor.
//----------------------------------------------------------------------------
NLock::NLock(void)
{


	// Initialize ourselves
	mLock      = kNLockRefNone;
	mLockCount = 0;
}





//============================================================================
//		NLock::~NLock : Destructor.
//----------------------------------------------------------------------------
NLock::~NLock(void)
{
}





//============================================================================
//		NLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NLock::Lock(NTime waitFor)
{


	// Acquire the lock
	return(Lock(mActionLock, waitFor));
}





//============================================================================
//		NLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NLock::Unlock(void)
{


	// Release the lock
	Unlock(mActionUnlock);
}





//============================================================================
//		NLock::IsLocked : Is the lock acquired?
//----------------------------------------------------------------------------
bool NLock::IsLocked(void) const
{


	// Check our state
	return(mLockCount != 0);
}





//============================================================================
//		NLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NLock::Lock(const NLockFunctor &actionLock, NTime waitFor)
{	NTime		stopTime;
	NStatus		theErr;



	// Validate our state
	NN_ASSERT(mLockCount  >= 0);
	NN_ASSERT(mLock       != kNLockRefNone);
	NN_ASSERT(mActionLock != NULL);



	// Acquire the lock
	if (NMathUtilities::AreEqual(waitFor, kNTimeForever))
		theErr = actionLock(mLock, true);
	else
		{
		stopTime = NTimeUtilities::GetTime() + waitFor;
		do
			{
			theErr = actionLock(mLock, false);
			if (theErr == kNErrTimeout)
				NThread::Sleep();
			}
		while (theErr != kNoErr && NTimeUtilities::GetTime() < stopTime);
		}



	// Update our state
	if (theErr == kNoErr)
		mLockCount++;
	
	return(theErr == kNoErr);
}





//============================================================================
//		NLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NLock::Unlock(const NUnlockFunctor &actionUnlock)
{


	// Validate our state
	NN_ASSERT(mLockCount    >= 1);
	NN_ASSERT(mLock         != kNLockRefNone);
	NN_ASSERT(mActionUnlock != NULL);
	


	// Release the lock
	mLockCount--;

	actionUnlock(mLock);
}





#pragma mark NMutexLock
//============================================================================
//		NSpinLock::NMutexLock : Constructor.
//----------------------------------------------------------------------------
NMutexLock::NMutexLock(void)
{


	// Initialize ourselves
	mLock = NTargetThread::MutexCreate();

	mActionLock   = BindFunction(NTargetThread::MutexLock,   _1, _2);
	mActionUnlock = BindFunction(NTargetThread::MutexUnlock, _1);
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





#pragma mark NReadWriteLock
//============================================================================
//		NReadWriteLock::NReadWriteLock : Constructor.
//----------------------------------------------------------------------------
NReadWriteLock::NReadWriteLock(void)
{


	// Initialize ourselves
	mLock = NTargetThread::ReadWriteCreate();

	mActionLock       = BindFunction(NTargetThread::ReadWriteLock,   _1, false, _2);
	mActionUnlock     = BindFunction(NTargetThread::ReadWriteUnlock, _1, false);

	mActionLockRead   = BindFunction(NTargetThread::ReadWriteLock,   _1, true, _2);
	mActionUnlockRead = BindFunction(NTargetThread::ReadWriteUnlock, _1, true);
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
//		NReadWriteLock::LockForRead : Acquire the lock for reading.
//----------------------------------------------------------------------------
bool NReadWriteLock::LockForRead(NTime waitFor)
{


	// Acquire the lock
	return(Lock(mActionLockRead, waitFor));
}





//============================================================================
//		NReadWriteLock::UnlockForRead : Release the lock for reading.
//----------------------------------------------------------------------------
void NReadWriteLock::UnlockForRead(void)
{


	// Release the lock
	Unlock(mActionUnlockRead);
}





#pragma mark NSpinLock
//============================================================================
//		NSpinLock::NSpinLock : Constructor.
//----------------------------------------------------------------------------
NSpinLock::NSpinLock(void)
{


	// Initialize ourselves
	mLock = NTargetThread::SpinCreate();

	mActionLock   = BindFunction(NTargetThread::SpinLock,   _1, _2);
	mActionUnlock = BindFunction(NTargetThread::SpinUnlock, _1);
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





#pragma mark StLock
//============================================================================
//		StLock::StLock : Constructor.
//----------------------------------------------------------------------------
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

