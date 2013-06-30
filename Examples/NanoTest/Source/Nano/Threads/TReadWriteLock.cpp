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
#include "NReadWriteLock.h"
#include "NTestFixture.h"

#include "CTestUtilities.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NREADWRITELOCK(...)									TEST_NANO(TReadWriteLock, ##__VA_ARGS__)

FIXTURE_NANO(TReadWriteLock)
{
	NReadWriteLock	theLock;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NREADWRITELOCK("Basic")
{


	// Perform the test
	REQUIRE(CTestUtilities::TestLock(&theLock));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NREADWRITELOCK("Readers")
{


	// Perform the test
	theLock.LockForRead();
	REQUIRE(theLock.IsLocked());

	theLock.LockForRead();
	REQUIRE(theLock.IsLocked());

		theLock.Lock();
		REQUIRE(theLock.IsLocked());

		theLock.Unlock();
		REQUIRE(theLock.IsLocked());

	theLock.UnlockForRead();
	REQUIRE(theLock.IsLocked());

	theLock.UnlockForRead();
	REQUIRE(!theLock.IsLocked());
}



