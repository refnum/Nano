/*	NAME:
		NSpinLock.cpp

	DESCRIPTION:
		Spin lock.
	
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
#include "NSpinLock.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NIndex kSpinLockLoopsPerYield							= 1000;





//============================================================================
//		NSpinLock::NSpinLock : Constructor.
//----------------------------------------------------------------------------
NSpinLock::NSpinLock(void)
{
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
{	bool		canBlock, gotLock;
	NTime		stopTime;
	NIndex		n;



	// Get the state we need
	canBlock = NMathUtilities::AreEqual(waitFor, kNTimeForever);
	stopTime = NTimeUtilities::GetTime() + waitFor;
	n        = 0;



	// Acquire the lock
	//
	// In general a spinlock simply loops until it acquires the lock, performing an
	// atomic compare-and-swap to try and set it from 0 to non-zero.
	//
	// On some platforms this can lead to priority inversion - Windows attempts to
	// deal with this by randomising thread priorities.
	//
	// Mac OS X provides a blocking implementation of a spinlock which will back off
	// if the lock can't be acquired, so we provide a canBlock hint that allows an
	// infinite wait for a spinlock to use this path.
	do
		{
		gotLock =  NTargetThread::SpinLock(mLockCount, canBlock);
		if (!gotLock)
			{
			n++;
			if (n == kSpinLockLoopsPerYield)
				{
				NThread::Sleep(kNTimeNone);
				n = 0;
				}
			}
		}
	while (!gotLock && NTimeUtilities::GetTime() < stopTime);

	return(gotLock);
}





//============================================================================
//		NSpinLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NSpinLock::Unlock(void)
{


	// Validate our state
	NN_ASSERT(mLockCount != 0);



	// Release the lock
	NTargetThread::SpinUnlock(mLockCount);
}





