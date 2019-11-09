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
#define TEST_NTRANSFORM(...)										TEST_NANO(TTransform, ##__VA_ARGS__)

FIXTURE_NANO(TTransform)
{
	NTransform	theTransform;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTRANSFORM("Default")
{


	// Perform the test
	REQUIRE(theTransform.IsIdentity());
}

