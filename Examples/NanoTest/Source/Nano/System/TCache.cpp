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
#define TEST_NCACHE(_name, _desc)									NANO_TEST(TCache, _name, _desc)

NANO_FIXTURE(TCache)
{
	NCache	theCache;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHE("Default", "Default state")
{


	// Perform the test
	REQUIRE(theCache.GetSize() == 0);
}
