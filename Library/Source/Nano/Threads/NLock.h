/*	NAME:
		NLock.h

	DESCRIPTION:
		Lock object.
	
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
    int32_t								mLockCount;
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


