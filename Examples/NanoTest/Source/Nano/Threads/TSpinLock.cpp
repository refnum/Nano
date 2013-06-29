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
#include "NTestFixture.h"
#include "NSpinLock.h"

#include "CTestUtilities.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSPINLOCK(_name, _desc)								TEST_NANO(TSpinLock, _name, _desc)

FIXTURE_NANO(TSpinLock)
{
	NSpinLock	theLock;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSPINLOCK("Basic", "Basic locking")
{


	// Perform the test
	REQUIRE(CTestUtilities::TestLock(&theLock));
}


