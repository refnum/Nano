/*	NAME:
		TRange.cpp

	DESCRIPTION:
		NRange tests.

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
#include "NRange.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NRange kTestRange1										= NRange(0, 5);
static const NRange kTestRange2										= NRange(3, 7);





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NRANGE(_name, _desc)									NANO_TEST(TRange, _name, _desc)

NANO_FIXTURE(TRange)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Size", "Size")
{


	// Perform the test
	REQUIRE(kNRangeNone.GetLocation() == kNIndexNone);
	REQUIRE(kNRangeNone.GetSize()     == 0);

	REQUIRE(kNRangeAll.GetLocation() == 0);
	REQUIRE(kNRangeAll.GetSize()     == kNIndexNone);

	REQUIRE(kNRangeNone.IsEmpty());
	REQUIRE(!kNRangeAll.IsEmpty());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Positions", "Positions")
{


	// Perform the test
	REQUIRE(kTestRange1.GetLocation() == 0);
	REQUIRE(kTestRange1.GetSize()     == 5);
	REQUIRE(kTestRange1.GetFirst()    == 0);
	REQUIRE(kTestRange1.GetLast()     == 4);
	REQUIRE(kTestRange1.GetNext()     == 5);

	REQUIRE(kTestRange2.GetLocation() == 3);
	REQUIRE(kTestRange2.GetSize()     == 7);
	REQUIRE(kTestRange2.GetFirst()    == 3);
	REQUIRE(kTestRange2.GetLast()     == 9);
	REQUIRE(kTestRange2.GetNext()     == 10);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Manipulation", "Manipulation")
{


	// Perform the test
	REQUIRE(kTestRange1.GetUnion(       kTestRange2) == NRange(0, 10));
	REQUIRE(kTestRange1.GetIntersection(kTestRange2) == NRange(3, 2));
	REQUIRE(kTestRange1.Overlaps(kTestRange2));

	REQUIRE( kTestRange1.Contains(0));
	REQUIRE( kTestRange1.Contains(4));
	REQUIRE(!kTestRange1.Contains(5));

	REQUIRE( kTestRange2.Contains(3));
	REQUIRE( kTestRange2.Contains(9));
	REQUIRE(!kTestRange2.Contains(2));
	REQUIRE(!kTestRange2.Contains(10));

	REQUIRE(kTestRange1.GetIntersection(NRange(8, 3)) == kNRangeNone);
	REQUIRE(kTestRange1.GetUnion(       NRange(0, 3)) != kNRangeNone);
}


