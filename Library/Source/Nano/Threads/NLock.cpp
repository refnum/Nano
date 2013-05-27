/*	NAME:
		NLock.cpp

	DESCRIPTION:
		Lock object.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLock.h"





//============================================================================
//		NLock::NLock : Constructor.
//----------------------------------------------------------------------------
NLock::NLock(void)
{


	// Initialize ourselves
	//
	// Lock implementations rely on the count being a 4-byte-aligned 32-bit
	// integer for atomic updates.
	mLockCount = 0;

	NN_ASSERT_ALIGNED_4(&mLockCount);
	NN_ASSERT(sizeof(mLockCount) == 4);
}





//============================================================================
//		NLock::~NLock : Destructor.
//----------------------------------------------------------------------------
NLock::~NLock(void)
{


	// Validate our state
	NN_ASSERT(mLockCount == 0);
}





//============================================================================
//		NLock::IsLocked : Is the lock acquired?
//----------------------------------------------------------------------------
bool NLock::IsLocked(void) const
{


	// Check our state
	return(mLockCount != 0);
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


	// Validate our state
	NN_ASSERT(mLock->IsLocked());



	// Release the lock
	mLock->Unlock();
}

