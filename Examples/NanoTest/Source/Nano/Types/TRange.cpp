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
#define TEST_NRANGE(_name, _desc)									TEST_NANO(TRange, _name, _desc)

FIXTURE_NANO(TRange)
{
	NRange		theRange;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Default", "Default state")
{


	// Perform the test
	REQUIRE(theRange.GetLocation() == 0);
	REQUIRE(theRange.GetSize()     == 0);

	REQUIRE(kNRangeNone.GetLocation() == kNIndexNone);
	REQUIRE(kNRangeNone.GetSize()     == 0);

	REQUIRE(kNRangeAll.GetLocation() == 0);
	REQUIRE(kNRangeAll.GetSize()     == kNIndexNone);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Comparison", "Comparisons")
{


	// Perform the test
	REQUIRE(kNRangeNone == kNRangeNone);
	REQUIRE(kNRangeNone != kNRangeAll);
	
	REQUIRE(kTestRange1 == kTestRange1);
	REQUIRE(kTestRange1 != kTestRange2);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Location", "Location")
{


	// Perform the test
	theRange = kTestRange1;
	REQUIRE(theRange.GetLocation() == kTestRange1.GetLocation());
	
	theRange.SetLocation(theRange.GetLocation() + 1);
	REQUIRE(theRange.GetLocation() != kTestRange1.GetLocation());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Size", "Size")
{


	// Perform the test
	theRange = kTestRange1;
	REQUIRE(theRange.GetSize() == kTestRange1.GetSize());
	
	theRange.SetSize(theRange.GetSize() + 1);
	REQUIRE(theRange.GetSize() != kTestRange1.GetSize());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Limit", "Limits")
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
TEST_NRANGE("Offset", "Offsets")
{


	// Perform the test
	REQUIRE(kTestRange1.GetOffset(0) == kTestRange1.GetFirst());
	REQUIRE(kTestRange1.GetOffset(1) == kTestRange1.GetFirst() + 1);
	REQUIRE(kTestRange1.GetOffset(2) == kTestRange1.GetFirst() + 2);

	REQUIRE(kTestRange2.GetOffset(0) == kTestRange2.GetFirst());
	REQUIRE(kTestRange2.GetOffset(1) == kTestRange2.GetFirst() + 1);
	REQUIRE(kTestRange2.GetOffset(2) == kTestRange2.GetFirst() + 2);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Normalized", "Normalized")
{	NRange	theRange;



	// Perform the test
	theRange = kNRangeNone.GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 0 && theRange.GetSize() == 0));		// Meta (none)

	theRange = kNRangeAll.GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 0 && theRange.GetSize() == 10));		// Meta (all)


	theRange = NRange(3, kNIndexNone).GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 3 && theRange.GetSize() == 7));		// Within (meta length)

	theRange = NRange(3, 7).GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 3 && theRange.GetSize() == 7));		// Within (non-zero length)

	theRange = NRange(3, 9).GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 3 && theRange.GetSize() == 7));		// Within (clamped length)

	theRange = NRange(3, 0).GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 3 && theRange.GetSize() == 0));		// Within (zero length)


	theRange = NRange(30, kNIndexNone).GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 30 && theRange.GetSize() == 0));		// Outisde (meta length)
	
	theRange = NRange(30, 7).GetNormalized(10);
	REQUIRE((theRange.GetLocation() == 30 && theRange.GetSize() == 0));		// Outside (non-zero length)

	theRange = NRange(30, 0).GetNormalized(0);
	REQUIRE((theRange.GetLocation() == 30 && theRange.GetSize() == 0));		// Outside (zero length)
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Union", "Union")
{


	// Perform the test
	REQUIRE(NRange(4, 3).GetUnion(NRange(6, 2)) == NRange(4, 4));		// 456 + 67 = 4567    (after)
	REQUIRE(NRange(4, 3).GetUnion(NRange(8, 2)) == NRange(4, 6));		// 456 + 89 = 456789  (after gap)
	REQUIRE(NRange(4, 3).GetUnion(NRange(3, 2)) == NRange(3, 4));		// 456 + 34 = 3456    (before)
	REQUIRE(NRange(4, 3).GetUnion(NRange(1, 2)) == NRange(1, 6));		// 456 + 12 = 123456  (before gap)
	REQUIRE(NRange(4, 3).GetUnion(NRange(0, 0)) == NRange(4, 3));		// 456 + .  = 456     (empty)
	REQUIRE(NRange(4, 3).GetUnion(NRange(3, 0)) == NRange(4, 3));		// 456 + .  = 456     (empty)
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Intersection", "Intersection")
{


	// Perform the test
	REQUIRE(NRange(4, 3).GetIntersection(NRange(6, 2)) == NRange(6, 1));		// 456 ~ 67     = 6   (end)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(8, 2)) == NRange(0, 0));		// 456 ~ 89     = .   (after)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(3, 2)) == NRange(4, 1));		// 456 ~ 34     = 4   (start)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(1, 2)) == NRange(0, 0));		// 456 ~ 12     = .   (before)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(5, 1)) == NRange(5, 1));		// 456 ~ 5      = 5   (inside)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(3, 5)) == NRange(4, 3));		// 456 ~ 34567  = 456 (within)
	REQUIRE(NRange(4, 3).GetIntersection(NRange(1, 0)) == NRange(0, 0));		// 456 ~ .      = .   (empty)
	REQUIRE(NRange(0, 0).GetIntersection(NRange(1, 2)) == NRange(0, 0));		//   . ~ 12     = .   (empty)
	REQUIRE(NRange(4, 0).GetIntersection(NRange(1, 0)) == NRange(0, 0));		//   . ~ .      = .   (empty)
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Empty", "Empty ranges")
{


	// Perform the test
	REQUIRE(kNRangeNone.IsEmpty());

	REQUIRE(!NRange(4, 3).IsEmpty());
	REQUIRE( NRange(4, 0).IsEmpty());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Meta", "Meta ranges")
{


	// Perform the test
	REQUIRE(kNRangeNone.IsMeta());
	REQUIRE(kNRangeAll.IsMeta());

	REQUIRE( NRange(4, kNIndexNone).IsMeta());
	REQUIRE(!NRange(4, 1).IsMeta());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Overlaps", "Overlaps")
{


	// Perform the test
	REQUIRE( !kNRangeNone.Overlaps(NRange(6, 2)));		//   . ~ 67 = false  (never)
	REQUIRE( NRange(4, 3).Overlaps(NRange(6, 2)));		// 456 ~ 67 = true   (after)
	REQUIRE(!NRange(4, 3).Overlaps(NRange(8, 2)));		// 456 ~ 89 = false  (after gap)
	REQUIRE( NRange(4, 3).Overlaps(NRange(3, 2)));		// 456 ~ 34 = true   (before)
	REQUIRE(!NRange(4, 3).Overlaps(NRange(1, 2)));		// 456 ~ 12 = false  (before gap)
	REQUIRE(!NRange(4, 3).Overlaps(NRange(0, 0)));		// 456 ~ .  = false  (empty)
	REQUIRE(!NRange(4, 3).Overlaps(NRange(3, 0)));		// 456 ~ .  = false  (empty)
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NRANGE("Contains", "Contains")
{


	// Perform the test
	REQUIRE( !kNRangeNone.Contains(5));		//   . ~ 5 = false  (never)
	REQUIRE(!NRange(4, 3).Contains(3));		// 456 ~ 3 = false  (before)
	REQUIRE( NRange(4, 3).Contains(4));		// 456 ~ 4 = true   (first)
	REQUIRE( NRange(4, 3).Contains(5));		// 456 ~ 5 = true   (inside)
	REQUIRE( NRange(4, 3).Contains(6));		// 456 ~ 6 = true   (last)
	REQUIRE(!NRange(4, 3).Contains(7));		// 456 ~ 7 = false  (after)
	REQUIRE(!NRange(4, 0).Contains(3));		//   . ~ 3 = false  (never)
	REQUIRE(!NRange(4, 0).Contains(4));		//   . ~ 4 = false  (never)
	REQUIRE(!NRange(4, 0).Contains(5));		//   . ~ 5 = false  (never)
}



