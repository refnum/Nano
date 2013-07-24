/*	NAME:
		TGeometryUtilities.cpp

	DESCRIPTION:
		NGeometryUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NSTLUtilities.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NGEOMETRYUTILITIES(...)								TEST_NANO(TGeometryUtilities, ##__VA_ARGS__)

FIXTURE_NANO(TGeometryUtilities)
{
	NPoint64List		thePoints64;
	NPoint32List		thePoints32;
	NPointList			thePoints;

	SETUP
	{
		thePoints64 = nvector(NPoint64(1.0f, 1.1f), NPoint64(2.0f, 2.2f), NPoint64(-3.0f, -3.3f));
		thePoints32 = nvector(NPoint32(1.0f, 1.1f), NPoint32(2.0f, 2.2f), NPoint32(-3.0f, -3.3f));
		thePoints   = nvector(NPoint  (1.0f, 1.1f), NPoint  (2.0f, 2.2f), NPoint  (-3.0f, -3.3f));
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NGEOMETRYUTILITIES("Bounds")
{	NRectangle64		theBounds64;
	NRectangle32		theBounds32;
	NRectangle			theBounds;



	// Perform the test
	theBounds64 = NGeometryUtilities::GetBounds(thePoints64);
	theBounds32 = NGeometryUtilities::GetBounds(thePoints32);
	theBounds   = NGeometryUtilities::GetBounds(thePoints);

	REQUIRE(NMathUtilities::AreEqual(theBounds64.origin.x,    (float64_t) theBounds32.origin.x));
	REQUIRE(NMathUtilities::AreEqual(theBounds64.origin.y,    (float64_t) theBounds32.origin.y));
	REQUIRE(NMathUtilities::AreEqual(theBounds64.size.width,  (float64_t) theBounds32.size.width));
	REQUIRE(NMathUtilities::AreEqual(theBounds64.size.height, (float64_t) theBounds32.size.height));
	REQUIRE(theBounds == theBounds32);
}



