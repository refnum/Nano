/*	NAME:
		TSystemUtilities.cpp

	DESCRIPTION:
		NSystemUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSYSTEMUTILITIES(...)									TEST_NANO(TSystemUtilities, ##__VA_ARGS__)

FIXTURE_NANO(TSystemUtilities)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSYSTEMUTILITIES("OS")
{	OSVersion	theVers;



	// Perform the test
	theVers = NSystemUtilities::GetOSVersion();
	REQUIRE(theVers != kOSUnknown);
}


