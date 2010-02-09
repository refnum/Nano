/*	NAME:
		TLock.cpp

	DESCRIPTION:
		NLock tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLock.h"

#include "TLock.h"





//============================================================================
//		TLock::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TLock::Execute(void)
{	NMutexLock		theMutex;



	// Execute the tests



	// Mutex
	NN_ASSERT(!theMutex.IsLocked());
	theMutex.Lock();

		NN_ASSERT(theMutex.IsLocked());
		theMutex.Lock();
		theMutex.Unlock();
		NN_ASSERT(theMutex.IsLocked());

	theMutex.Unlock();
	NN_ASSERT(!theMutex.IsLocked());
}



