/*	NAME:
		TThreadPool.cpp

	DESCRIPTION:
		NThreadPool tests.

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
#include "NThreadPool.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NTHREADPOOL(_name, _desc)								NANO_TEST(TThreadPool, _name, _desc)

NANO_FIXTURE(TThreadPool)
{
	NThreadPool		thePool;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTHREADPOOL("Default", "Default state")
{


	// Perform the test
	REQUIRE(thePool.GetActiveTasks()  == 0);
	REQUIRE(thePool.GetPendingTasks() == 0);
	REQUIRE(thePool.GetThreadLimit()  != 0);
}



