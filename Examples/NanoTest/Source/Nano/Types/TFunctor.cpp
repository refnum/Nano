/*	NAME:
		TFunctor.cpp

	DESCRIPTION:
		NFunctor tests.

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
#include "NFunctor.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NFUNCTOR(...)											TEST_NANO(TFunctor, ##__VA_ARGS__)

FIXTURE_NANO(TFunctor)
{
	NFunctor	theFunctor;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NFUNCTOR("Default")
{


	// Perform the test
	REQUIRE((theFunctor == NULL));
}

