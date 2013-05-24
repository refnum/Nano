/*	NAME:
		NLock.h

	DESCRIPTION:
		Lock objects.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NLOCK_HDR
#define NLOCK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUncopyable.h"
#include "NAtomicInt.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
typedef UIntPtr NLockRef;

static const NLockRef kNLockRefNone								= 0;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Timers
typedef nfunctor<NStatus (NLockRef theLock, bool canBlock)>			NLockFunctor;
typedef nfunctor<void    (NLockRef theLock)>						NUnlockFunctor;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NLock {
public:
										NLock(void);
	virtual							   ~NLock(void);


	// Acquire/release the lock
	bool								Lock(NTime waitFor=kNTimeForever);
	void								Unlock(void);


	// Is the lock acquired?
	//
	// Testing the lock status is not sufficient for synchronization; a lock
	// must be acquired before data protected by that lock can be used.
	//
	// This method is provided to allow code to perform a cheap test against
	// a lock to check it is in the correct state (e.g., to assert that a lock
	// acquired elsewhere was indeed acquired by the time of the assert).
	bool								IsLocked(void) const;


protected:
	// Acquire/release the lock
	bool								Lock(  const NLockFunctor   &actionLock, NTime waitFor);
	void								Unlock(const NUnlockFunctor &actionUnlock);


protected:
	NLockRef							mLock;
    NAtomicInt							mLockCount;

	NLockFunctor						mActionLock;
	NUnlockFunctor						mActionUnlock;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
//		Note : NMutexLock is a recursive mutex.
//----------------------------------------------------------------------------
class NMutexLock : public NLock {
public:
										NMutexLock(void);
	virtual							   ~NMutexLock(void);
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NReadWriteLock : public NLock {
public:
										NReadWriteLock(void);
	virtual							   ~NReadWriteLock(void);


	// Acquire/release the lock for reading
	//
	// The standard NLock interface allows a single thread to acquire the
	// lock for writing.
	//
	// Multiple threads can use LockForRead to acquire the lock for reading.
	bool								LockForRead(NTime waitFor=kNTimeForever);
	void								UnlockForRead(void);


private:
	NLockFunctor						mActionLockRead;
	NUnlockFunctor						mActionUnlockRead;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSpinLock : public NLock {
public:
										NSpinLock(void);
	virtual							   ~NSpinLock(void);
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class StLock : public NUncopyable {
public:
										StLock(NLock &theLock);
	virtual							   ~StLock(void);


private:
	NLock							   *mLock;
};





#endif // NLOCK_HDR


