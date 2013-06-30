/*	NAME:
		TTimeUtilities.cpp

	DESCRIPTION:
		NTimeUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTimeUtilities.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NTIMEUTILITIES(...)									TEST_NANO(TTimeUtilities, ##__VA_ARGS__)

FIXTURE_NANO(TTimeUtilities)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTIMEUTILITIES("Time")
{


	// Perform the test
	REQUIRE(NTimeUtilities::GetUpTime() < NTimeUtilities::GetTime());
}


