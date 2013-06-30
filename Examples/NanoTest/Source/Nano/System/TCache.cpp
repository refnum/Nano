/*	NAME:
		TCache.cpp

	DESCRIPTION:
		NCache tests.

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
#include "NCache.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NCACHE(...)											TEST_NANO(TCache, ##__VA_ARGS__)

FIXTURE_NANO(TCache)
{
	NCache	theCache;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHE("Default")
{


	// Perform the test
	REQUIRE(theCache.GetSize() == 0);
}
