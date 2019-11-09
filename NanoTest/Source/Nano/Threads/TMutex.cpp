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
#include "NTestFixture.h"
#include "NMutex.h"

#include "CTestUtilities.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NMUTEX(...)											TEST_NANO(TMutex, ##__VA_ARGS__)

FIXTURE_NANO(TMutex)
{
	NMutex	theLock;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NMUTEX("Basic")
{


	// Perform the test
	REQUIRE(CTestUtilities::TestLock(&theLock));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NMUTEX("Recursion")
{


	// Perform the test
	REQUIRE(!theLock.IsLocked());
	theLock.Lock();

		REQUIRE(theLock.IsLocked());
		theLock.Lock();
		theLock.Unlock();
		REQUIRE(theLock.IsLocked());

	theLock.Unlock();
	REQUIRE(!theLock.IsLocked());
}



