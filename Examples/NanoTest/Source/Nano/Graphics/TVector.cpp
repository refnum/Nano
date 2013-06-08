/*	NAME:
		TVector.cpp

	DESCRIPTION:
		NVector tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NVector.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NVECTOR(_name, _desc)									NANO_TEST(TVector, _name, _desc)

NANO_FIXTURE(TVector)
{
	NVector	theVector;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NVECTOR("Default", "Default state")
{


	// Perform the test
	REQUIRE(theVector.IsZero());
}

