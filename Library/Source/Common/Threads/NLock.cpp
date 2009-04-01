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
	mLock  = NULL;
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
		NThreadUtilities::AtomicAdd32(mCount,  1);
	else
		NThreadUtilities::AtomicAdd32(mCount, -1);
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
bool NSpinLock::Lock(NTime theTime)
{	NTime		stopTime;
	bool		gotLock;



	// Acquire the lock
	//
	// A spin loop is less efficient than blocking, but sufficient for our needs.
	if (NMathUtilities::AreEqual(theTime, kNTimeForever))
		{
		while (!SpinLockTry(mSpinLock))
			NThreadUtilities::Sleep(kNThreadSpinTime);

		gotLock = true;
		}
	else
		{
		stopTime = NTimeUtilities::GetTime() + theTime;
		gotLock  = false;
		do
			{
			gotLock = SpinLockTry(mSpinLock);
			if (!gotLock)
				NThreadUtilities::Sleep(kNThreadSpinTime);
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

