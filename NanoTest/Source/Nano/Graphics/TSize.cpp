/*	NAME:
		TSize.cpp

	DESCRIPTION:
		NSize tests.

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
#include "NSize.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NSize kTestSize1										= NSize(1.0f, -1.0f);
static const NSize kTestSize1_Normalized							= NSize(1.0f,  1.0f);

static const NSize kTestSize2										= NSize(1.20f, 7.80f);
static const NSize kTestSize2_Integral								= NSize(2.00f, 8.00f);
static const NSize kTestSize2_Resized								= NSize(2.40f, 4.40f);
static const NSize kTestSize2_Scaled								= NSize(12.0f, 78.0f);

static const NSize kTestSize3										= NSize(1.20f, 7.80f);

static const NSize kTestSize4										= NSize(1.0f, 1.0f);
static const NSize kTestSize4_Width									= NSize(0.0f, 1.0f);
static const NSize kTestSize4_Height								= NSize(1.0f, 0.0f);





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSIZE(...)												TEST_NANO(TSize, ##__VA_ARGS__)

FIXTURE_NANO(TSize)
{
	NSize	theSize;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSIZE("Size")
{


	// Perform the test
	REQUIRE(sizeof(NSize64) == 16);
	REQUIRE(sizeof(NSize32) == 8);
	REQUIRE(sizeof(NSize)   == 8);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSIZE("Contents")
{


	// Perform the test
	REQUIRE(theSize.IsEmpty());
	REQUIRE(!kTestSize1.IsEmpty());

	theSize = kTestSize1;
	REQUIRE(!theSize.IsEmpty());

	theSize.Clear();
	REQUIRE(theSize.IsEmpty());
	REQUIRE(theSize != kTestSize1);

	REQUIRE(!kTestSize4.IsEmpty());
	REQUIRE( kTestSize4_Width.IsEmpty());
	REQUIRE( kTestSize4_Height.IsEmpty());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSIZE("Comparisons")
{


	// Perform the test
	REQUIRE(NMathUtilities::AreEqual(kTestSize1.width,   1.0f));
	REQUIRE(NMathUtilities::AreEqual(kTestSize1.height, -1.0f));

	REQUIRE(kTestSize2 != kTestSize1);
	REQUIRE(kTestSize2 == kTestSize3);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSIZE("Manipulations")
{


	// Perform the test
	theSize = kTestSize1;
	theSize.Normalize();
	REQUIRE(theSize == kTestSize1.GetNormalized());
	REQUIRE(theSize == kTestSize1_Normalized);

	theSize = kTestSize2;
	theSize.MakeIntegral();
	REQUIRE(theSize == kTestSize2.GetIntegral());
	REQUIRE(theSize == kTestSize2_Integral);

	theSize = kTestSize2;
	theSize.Resize( 1.2f, -3.4f);
	REQUIRE(theSize == kTestSize2.GetResized(1.2f, -3.4f));
	REQUIRE(theSize == kTestSize2_Resized);

	theSize = kTestSize2;
	theSize.Scale( 10.0f);
	REQUIRE(theSize == kTestSize2.GetScaled(10.0f));
	REQUIRE(theSize == kTestSize2_Scaled);
}


