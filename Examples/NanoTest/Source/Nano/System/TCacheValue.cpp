/*	NAME:
		TCacheValue.cpp

	DESCRIPTION:
		NCacheValue tests.

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
#include "NCacheValue.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NCACHEVALUE(_name, _desc)								TEST_NANO(TCacheValue, _name, _desc)

FIXTURE_NANO(TCacheValue)
{
	NCacheValue		*theValue;
	
	SETUP
	{
		theValue = new NCacheValue;
	}
	
	TEARDOWN
	{
		theValue->Release();
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHEVALUE("Default", "Default state")
{


	// Perform the test
	REQUIRE(theValue->GetCost() == 0);
}
