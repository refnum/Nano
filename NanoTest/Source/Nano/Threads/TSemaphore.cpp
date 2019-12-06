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
//		Fixture
//----------------------------------------------------------------------------
NANO_FIXTURE(TSemaphore)
{
	NSemaphore	theSemaphore;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
NANO_TEST(TSemaphore, "Default")
{


	// Perform the test
	theSemaphore.Signal();

	REQUIRE(theSemaphore.Wait());
	REQUIRE(TimeUnder(kNTimeMillisecond));
}


