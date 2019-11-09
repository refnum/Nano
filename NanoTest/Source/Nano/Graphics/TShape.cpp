/*	NAME:
		TShape.cpp

	DESCRIPTION:
		NShape tests.

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
#include "NShape.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSHAPE(...)											TEST_NANO(TShape, ##__VA_ARGS__)

FIXTURE_NANO(TShape)
{
	NShape	theShape;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSHAPE("Default")
{


	// Perform the test
	REQUIRE(theShape.IsEmpty());
}


