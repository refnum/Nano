/*	NAME:
		TTransform.cpp

	DESCRIPTION:
		NTransform tests.

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
#include "NTransform.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NTRANSFORM(_name, _desc)								NANO_TEST(TTransform, _name, _desc)

NANO_FIXTURE(TTransform)
{
	NTransform	theTransform;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTRANSFORM("Default", "Default state")
{


	// Perform the test
	REQUIRE(theTransform.IsIdentity());
}

