/*	NAME:
		TMutex.cpp

	DESCRIPTION:
		NMutex tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "TLock.h"
#include "TMutex.h"





//============================================================================
//		TMutex::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TMutex::Execute(void)
{	NMutex		theLock;



	// Test the lock
	TLock::TestLock(&theLock);



	// Recursion
	NN_ASSERT(!theLock.IsLocked());
	theLock.Lock();

		NN_ASSERT(theLock.IsLocked());
		theLock.Lock();
		theLock.Unlock();
		NN_ASSERT(theLock.IsLocked());

	theLock.Unlock();
	NN_ASSERT(!theLock.IsLocked());
}


