/*	NAME:
		NReadWriteLock.h

	DESCRIPTION:
		Read/write lock.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NREADWRITELOCK_HDR
#define NREADWRITELOCK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLock.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NReadWriteLock : public NLock {
public:
										NReadWriteLock(void);
	virtual							   ~NReadWriteLock(void);


	// Acquire/release the lock for writing
	//
	// A single thread may acquire the lock for writing.
	bool								Lock(NTime waitFor=kNTimeForever);
	void								Unlock(void);


	// Acquire/release the lock for reading
	//
	// Multiple threads may acquire the lock for reading.
	//
	// Readers take priority, and excessive readers may starve the writers.
	bool								LockForRead(NTime waitFor=kNTimeForever);
	void								UnlockForRead(void);


private:
	NIndex								mNumReaders;
	NMutex								mLockReaders;
	NMutex								mLockWriters;
};






#endif // NREADWRITELOCK_HDR


