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
#define TEST_NTHREADPOOL(...)										TEST_NANO(TThreadPool, ##__VA_ARGS__)

FIXTURE_NANO(TThreadPool)
{
	NThreadPool		thePool;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTHREADPOOL("Default")
{


	// Perform the test
	REQUIRE(thePool.GetActiveTasks()  == 0);
	REQUIRE(thePool.GetPendingTasks() == 0);
	REQUIRE(thePool.GetThreadLimit()  != 0);
}



