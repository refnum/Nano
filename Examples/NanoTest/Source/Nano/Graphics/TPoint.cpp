/*	NAME:
		TPoint.cpp

	DESCRIPTION:
		NPoint tests.

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
#include "NPoint.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NPoint kTestPoint1										= NPoint(1.0f, -1.0f);

static const NPoint kTestPoint2										= NPoint(1.3f, 1.9f);
static const NPoint kTestPoint2_Integral							= NPoint(1.0f, 2.0f);
static const NPoint kTestPoint2_Offset								= NPoint(5.3f, 5.9f);





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NPOINT(...)											TEST_NANO(TPoint, ##__VA_ARGS__)

FIXTURE_NANO(TPoint)
{
	NPoint		thePoint;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NPOINT("Size")
{


	// Perform the test
	REQUIRE(sizeof(NPoint64) == 16);
	REQUIRE(sizeof(NPoint32) == 8);
	REQUIRE(sizeof(NPoint)   == 8);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NPOINT("Values")
{


	// Perform the test
	REQUIRE(    thePoint.IsZero());
	REQUIRE(!kTestPoint1.IsZero());

	thePoint = kTestPoint1;
	REQUIRE(!thePoint.IsZero());

	thePoint.Clear();
	REQUIRE(thePoint.IsZero());
	REQUIRE(thePoint != kTestPoint1);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NPOINT("Comparisons")
{


	// Perform the test
	REQUIRE(NMathUtilities::AreEqual(kTestPoint1.x,  1.0f));
	REQUIRE(NMathUtilities::AreEqual(kTestPoint1.y, -1.0f));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NPOINT("Manipulations")
{


	// Perform the test
	thePoint = kTestPoint2;
	thePoint.MakeIntegral();

	REQUIRE(thePoint == kTestPoint2.GetIntegral());
	REQUIRE(thePoint == kTestPoint2_Integral);

	thePoint = kTestPoint2;
	thePoint.Offset(4.0f, 4.0f);

	REQUIRE(thePoint == kTestPoint2.GetOffset(4.0f, 4.0f));
	REQUIRE(thePoint == kTestPoint2_Offset);
}



