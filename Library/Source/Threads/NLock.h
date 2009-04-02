/*	NAME:
		NLock.h

	DESCRIPTION:
		Lock objects.
	
	COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NLOCK_HDR
#define NLOCK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef void *NLockRef;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NLock {
public:
										 NLock(void);
	virtual								~NLock(void);


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
    SInt32								mCount;
	NLockRef							mLock;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMutexLock : public NLock {
public:
										 NMutexLock(void);
	virtual								~NMutexLock(void);


	// Acquire/release the lock
	//
	// Mutexes are recursive, and may be called multiple times from the same
	// thread providing every Lock is balanced with a subsequent Unlock.
	bool								Lock(NTime waitFor=kNTimeForever);
	void								Unlock(void);
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NReadWriteLock : public NLock {
public:
										 NReadWriteLock(void);
	virtual								~NReadWriteLock(void);


	// Acquire/release the lock
	//
	// Multiple readers can acquire the lock simultaneously, but will
	// block if a thread has acquired the lock for writing.
	//
	// Read/write locks are not recursive.
	bool								Lock(NTime waitFor=kNTimeForever);
	void								Unlock(void);


	// Acquire the lock for writing
	bool								LockForWrite(NTime waitFor=kNTimeForever);


private:
	bool								AcquireLock(NTime waitFor, bool forWrite);
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSpinLock : public NLock {
public:
										 NSpinLock(void);
	virtual								~NSpinLock(void);


	// Acquire/release the lock
	//
	// Spin locks are not recursive, and should typically be avoided in favour
	// of an NMutexLock unless the situation specifically calls for a spin lock.
	bool								Lock(NTime waitFor=kNTimeForever);
	void								Unlock(void);


private:
	bool								SpinLockTry(   UInt32 &theLock);
	void								SpinLockUnlock(UInt32 &theLock);


private:
    UInt32								mSpinLock;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class StLock {
public:
										 StLock(NLock &theLock);
	virtual								~StLock(void);


private:
	NLock								&mLock;
};





#endif // NLOCK_HDR


