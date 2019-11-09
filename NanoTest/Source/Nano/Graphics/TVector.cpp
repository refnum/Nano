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
#define TEST_NVECTOR(...)											TEST_NANO(TVector, ##__VA_ARGS__)

FIXTURE_NANO(TVector)
{
	NVector	theVector;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NVECTOR("Default")
{


	// Perform the test
	REQUIRE(theVector.IsZero());
}

