/*	NAME:
		TNumber.cpp

	DESCRIPTION:
		NNumber tests.

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
#include "NNumber.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NNUMBER(...)											TEST_NANO(TNumber, ##__VA_ARGS__)

FIXTURE_NANO(TNumber)
{
	NNumber		theNumber;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NNUMBER("Default")
{


	// Perform the test
	REQUIRE(theNumber.IsInteger());
	REQUIRE(theNumber.GetUInt8() == 0);
}

