/*	NAME:
		NTargetLock.h

	DESCRIPTION:
		Lock support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_LOCK_HDR
#define NTARGET_LOCK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLock.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetLock {
public:
	// Mutex
	static NLockRef						MutexCreate(void);
	static void							MutexDestroy(NLockRef &theLock);
	static NStatus						MutexLock(   NLockRef &theLock, NTime waitFor);
	static void							MutexUnlock( NLockRef &theLock);


	// Read/write
	static NLockRef						ReadWriteCreate(void);
	static void							ReadWriteDestroy(NLockRef &theLock);
	static NStatus						ReadWriteLock(   NLockRef &theLock, NTime waitFor, bool forWrite);
	static void							ReadWriteUnlock( NLockRef &theLock);
};





#endif // NTARGET_LOCK_HDR





