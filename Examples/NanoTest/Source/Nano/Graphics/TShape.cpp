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
#define TEST_NSHAPE(_name, _desc)									TEST_NANO(TShape, _name, _desc)

FIXTURE_NANO(TShape)
{
	NShape	theShape;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSHAPE("Default", "Default state")
{


	// Perform the test
	REQUIRE(theShape.IsEmpty());
}


