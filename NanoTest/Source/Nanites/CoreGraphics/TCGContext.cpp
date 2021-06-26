/*	NAME:
		TCGContext.cpp

	DESCRIPTION:
		NCGContext tests.

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
#include "NCGContext.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TCGContext){
NCGContext	cgContext;

};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TCGContext, "Default")
{


	// Perform the test
	REQUIRE(!cgContext.IsValid());
}

