/*	NAME:
		NSpinLock.h

	DESCRIPTION:
		Spin lock.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSPINLOCK_HDR
#define NSPINLOCK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLock.h"





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


private:


};




#endif // NSPINLOCK_HDR


