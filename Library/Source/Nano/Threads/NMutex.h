/*	NAME:
		NMutex.h

	DESCRIPTION:
		Mutex lock.
	
	COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMUTEX_HDR
#define NMUTEX_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSemaphore.h"
#include "NThread.h"
#include "NLock.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMutex : public NLock {
public:
										NMutex(void);
	virtual							   ~NMutex(void);


	// Acquire/release the lock
	//
	// A mutex can be locked multiple times by the same thread.
	bool								Lock(NTime waitFor=kNTimeForever);
	void								Unlock(void);


private:
	NThreadID							mOwnerID;
	NIndex								mRecursion;
	NSemaphoreRef						mSemaphore;
};






#endif // NMUTEX_HDR


