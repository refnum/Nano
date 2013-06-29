/*	NAME:
		TLock.cpp

	DESCRIPTION:
		NLock tests.

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





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NLOCK(_name, _desc)									TEST_NANO(TLock, _name, _desc)

FIXTURE_NANO(TLock)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NLOCK("StLock", "Stack locker")
{	NMutex	theLock;



	// Perform the test
	REQUIRE(!theLock.IsLocked());
	
	{
		StLock	acquireLock(theLock);

		REQUIRE(theLock.IsLocked());
	}

	REQUIRE(!theLock.IsLocked());
}



