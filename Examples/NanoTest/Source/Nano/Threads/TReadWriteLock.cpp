/*	NAME:
		TReadWriteLock.cpp

	DESCRIPTION:
		NReadWriteLock tests.

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
#include "TReadWriteLock.h"





//============================================================================
//		TReadWriteLock::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TReadWriteLock::Execute(void)
{	NReadWriteLock		theLock;



	// Test the lock
	TLock::TestLock(&theLock);



	// Readers/writers
	theLock.LockForRead();
	NN_ASSERT(theLock.IsLocked());

	theLock.LockForRead();
	NN_ASSERT(theLock.IsLocked());

		theLock.Lock();
		NN_ASSERT(theLock.IsLocked());

		theLock.Unlock();
		NN_ASSERT(theLock.IsLocked());

	theLock.UnlockForRead();
	NN_ASSERT(theLock.IsLocked());

	theLock.UnlockForRead();
	NN_ASSERT(!theLock.IsLocked());
}


