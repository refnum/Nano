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
//		Class declaration
//----------------------------------------------------------------------------
class NLock {
public:
										NLock(void);
	virtual							   ~NLock(void);


	// Acquire/release the lock
	virtual bool						Lock(NTime waitFor=kNTimeForever) = 0;
	virtual void						Unlock(void)                      = 0;


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
	// Adjust the lock state
	void								AdjustLock(bool didLock);


protected:
    NAtomicInt							mCount;
	NLockRef							mLock;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMutexLock : public NLock {
public:
										NMutexLock(void);
	virtual							   ~NMutexLock(void);


	// Acquire/release the lock
	//
	// Mutexes are recursive, and can be locked N times by the same thread
	// providing they are also unlocked N times.
	bool								Lock(NTime waitFor=kNTimeForever);
	void								Unlock(void);
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NReadWriteLock : public NLock {
public:
										NReadWriteLock(void);
	virtual							   ~NReadWriteLock(void);


	// Acquire/release the lock for writing
	//
	// Only a single thread can acquire the lock for writing.
	bool								Lock(NTime waitFor=kNTimeForever);
	void								Unlock(void);


	// Acquire/release the lock for reading
	//
	// Multiple threads can acquire the lock for reading.
	bool								LockForRead(NTime waitFor=kNTimeForever);
	void								UnlockForRead(void);


private:
	bool								Lock(  bool forRead, NTime waitFor);
	void								Unlock(bool forRead);
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSpinLock : public NLock {
public:
										NSpinLock(void);
	virtual							   ~NSpinLock(void);


	// Acquire/release the lock
	bool								Lock(NTime waitFor=kNTimeForever);
	void								Unlock(void);
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


