/*	NAME:
		TSpinLock.cpp

	DESCRIPTION:
		NSpinLock tests.

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
#include "TSpinLock.h"





//============================================================================
//		TSpinLock::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TSpinLock::Execute(void)
{	NSpinLock		theLock;



	// Test the lock
	TLock::TestLock(&theLock);
}

