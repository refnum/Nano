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
#include "NTestFixture.h"
#include "NVector.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NVECTOR(_name, _desc)									TEST_NANO(TVector, _name, _desc)

FIXTURE_NANO(TVector)
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

