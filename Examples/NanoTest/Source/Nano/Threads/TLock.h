/*	NAME:
		TLock.h

	DESCRIPTION:
		NLock tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef TLOCK_HDR
#define TLOCK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLock.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class TLock {
public:
	// Execute the tests
	static void							Execute(void);


private:
	static void							LockUnlock(NLock *theLock, NData *theData);
};





#endif // TLOCK_HDR
