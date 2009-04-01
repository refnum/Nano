/*	NAME:
		NWinLock.cpp

	DESCRIPTION:
		Windows lock objects.
	
	COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimeUtilities.h"
#include "NLock.h"





//============================================================================
//		NMutexLock::NMutexLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NMutexLock::NMutexLock(void)
{
	// dair, create the lock
}





//============================================================================
//		NMutexLock::~NMutexLock : Destructor.
//----------------------------------------------------------------------------
NMutexLock::~NMutexLock(void)
{
	// dair, destroy the lock
}





//============================================================================
//		NMutexLock::Lock : Acquire the lock.
//----------------------------------------------------------------------------
bool NMutexLock::Lock(NTime theTime)
{
	// dair, acquire the lock
	return(true);
}





//============================================================================
//		NMutexLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NMutexLock::Unlock(void)
{
	// dair, release the lock
}





//============================================================================
//		NReadWriteLock::NReadWriteLock : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NReadWriteLock::NReadWriteLock(void)
{
	// dair, acquire the lock
}





//============================================================================
//		NReadWriteLock::~NReadWriteLock : Destructor.
//----------------------------------------------------------------------------
NReadWriteLock::~NReadWriteLock(void)
{
	// dair, release the lock
}





//============================================================================
//		NReadWriteLock::Lock : Acquire the lock for reading.
//----------------------------------------------------------------------------
bool NReadWriteLock::Lock(NTime theTime)
{
	// dair, acquire the lock
	return(true);
}





//============================================================================
//		NReadWriteLock::Unlock : Release the lock.
//----------------------------------------------------------------------------
void NReadWriteLock::Unlock(void)
{
	// dair, release the lock
}





//============================================================================
//		NReadWriteLock::LockForWrite : Acquire the lock for writing.
//----------------------------------------------------------------------------
bool NReadWriteLock::LockForWrite(NTime theTime)
{
	// dair, acquire the lock
	return(true);
}




