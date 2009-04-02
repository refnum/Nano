/*	NAME:
		NWinLock.cpp

	DESCRIPTION:
		Windows lock support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetLock.h"





//============================================================================
//      NTargetLock::MutexCreate : Create a mutex lock.
//----------------------------------------------------------------------------
NLockRef NTargetLock::MutexCreate(void)
{
	// dair,to do
	return(NULL);
}





//============================================================================
//      NTargetLock::MutexDestroy : Destroy a mutex lock.
//----------------------------------------------------------------------------
void NTargetLock::MutexDestroy(NLockRef &theLock)
{
	// dair,to do
	theLock = NULL;
}





//============================================================================
//      NTargetLock::MutexLock : Lock a mutex lock.
//----------------------------------------------------------------------------
NStatus NTargetLock::MutexLock(NLockRef &theLock, NTime waitFor)
{
	// dair,to do
	return(kNoErr);
}





//============================================================================
//      NTargetLock::MutexUnlock : Unlock a mutex lock.
//----------------------------------------------------------------------------
void NTargetLock::MutexUnlock(NLockRef &theLock)
{
	// dair,to do
}





//============================================================================
//      NTargetLock::ReadWriteCreate : Create a read-write lock.
//----------------------------------------------------------------------------
NLockRef NTargetLock::ReadWriteCreate(void)
{
	// dair,to do
	return(NULL);
}





//============================================================================
//      NTargetLock::ReadWriteDestroy : Destroy a read-write lock.
//----------------------------------------------------------------------------
void NTargetLock::ReadWriteDestroy(NLockRef &theLock)
{
	// dair,to do
}





//============================================================================
//      NTargetLock::ReadWriteLock : Lock a read-write lock.
//----------------------------------------------------------------------------
NStatus NTargetLock::ReadWriteLock(NLockRef *theLock, NTime waitFor, bool forWrite)
{
	// dair,to do
	return(kNoErr);
}





//============================================================================
//      NTargetLock::ReadWriteUnlock : Unlock a read-write lock.
//----------------------------------------------------------------------------
void NTargetLock::ReadWriteUnlock(NLockRef &theLock)
{
	// dair,to do
}


