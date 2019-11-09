/*	NAME:
		TSemaphore.cpp

	DESCRIPTION:
		NSemaphore tests.

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
#include "NSemaphore.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSEMPAHORE(...)										TEST_NANO(TSemaphore, ##__VA_ARGS__)

FIXTURE_NANO(TSemaphore)
{
	NSemaphore	theSemaphore;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSEMPAHORE("Default")
{


	// Perform the test
	theSemaphore.Signal();

	REQUIRE(theSemaphore.Wait());
	REQUIRE(TimeUnder(kNTimeMillisecond));
}


