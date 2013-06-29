/*	NAME:
		TRectangle.cpp

	DESCRIPTION:
		NRectangle tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NTestFixture.h"
#include "NRectangle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NRectangle kTestRectangle1								= NRectangle(1.0f,   2.0f, 10.0f, -20.0f);
static const NRectangle kTestRectangle1_Normalized					= NRectangle(1.0f, -18.0f, 10.0f,  20.0f);

static const NRectangle kTestRectangle2								= NRectangle(3.4f, 4.8f, 1.2f, 7.8f);
static const NRectangle kTestRectangle2_Integral					= NRectangle(3.0f, 4.0f, 2.0f, 9.0f);

static const NRectangle kTestRectangle3								= NRectangle(3.7f, 5.2f, 0.1f, 1.1f);





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NRECTANGLE(_name, _desc)								TEST_NANO(TRectangle, _name, _desc)

FIXTURE_NANO(TRectangle)
{
	NRectangle		theRect;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRECTANGLE("Size", "Sizes")
{


	// Perform the test
	REQUIRE(sizeof(NRectangle64) == 32);
	REQUIRE(sizeof(NRectangle32) == 16);
	REQUIRE(sizeof(NRectangle)   == 16);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRECTANGLE("Contents", "Contents")
{


	// Perform the test
	REQUIRE(theRect.IsEmpty());
	REQUIRE(theRect.origin.IsZero());
	REQUIRE(theRect.size.IsEmpty());

	REQUIRE(!kTestRectangle1.IsEmpty());
	REQUIRE(!kTestRectangle1.origin.IsZero());
	REQUIRE(!kTestRectangle1.size.IsEmpty());

	theRect = kTestRectangle1;
	REQUIRE(!theRect.IsEmpty());

	theRect.Clear();
	REQUIRE(theRect.IsEmpty());
	REQUIRE(theRect != kTestRectangle1);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRECTANGLE("Comparisons", "Comparisons")
{


	// Perform the test
	REQUIRE(NMathUtilities::AreEqual(kTestRectangle1.origin.x,      1.0f));
	REQUIRE(NMathUtilities::AreEqual(kTestRectangle1.origin.y,      2.0f));
	REQUIRE(NMathUtilities::AreEqual(kTestRectangle1.size.width,   10.0f));
	REQUIRE(NMathUtilities::AreEqual(kTestRectangle1.size.height, -20.0f));

	theRect = kTestRectangle3.GetScaled(1.0f);
	REQUIRE(kTestRectangle3 != kTestRectangle2);
	REQUIRE(kTestRectangle3 == theRect);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRECTANGLE("Manipulation", "Manipulations")
{


	// Perform the test
	theRect = kTestRectangle1;
	theRect.Normalize();
	REQUIRE(theRect == kTestRectangle1.GetNormalized());
	REQUIRE(theRect == kTestRectangle1_Normalized);

	theRect = kTestRectangle2;
	theRect.MakeIntegral();
	REQUIRE(theRect == kTestRectangle2.GetIntegral());
	REQUIRE(theRect == kTestRectangle2_Integral);

	REQUIRE(kTestRectangle2.GetScaled(1.0f) == kTestRectangle2);
	REQUIRE(kTestRectangle2.GetScaled(0.0f).IsEmpty());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRECTANGLE("Geometry", "Geometry")
{


	// Perform the test
	REQUIRE(kTestRectangle2.Contains(kTestRectangle3.origin));
	REQUIRE(kTestRectangle2.Contains(kTestRectangle3));

	REQUIRE(!kTestRectangle2.Contains(kTestRectangle1.origin));
	REQUIRE(!kTestRectangle2.Contains(kTestRectangle1));

	REQUIRE(kTestRectangle2.Intersects(kTestRectangle3));

	REQUIRE(kTestRectangle2.GetUnion(kTestRectangle3) == kTestRectangle2);
}



