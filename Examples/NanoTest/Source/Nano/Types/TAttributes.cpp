/*	NAME:
		TAttributes.cpp

	DESCRIPTION:
		NAttributes tests.

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
#include "NAttributes.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NBitfield kTestAttribute1								= (NBitfield) (1 << 0);
static const NBitfield kTestAttribute2								= (NBitfield) (1 << 1);
static const NBitfield kTestAttribute3								= (NBitfield) (1 << 30);
static const NBitfield kTestAttribute4								= (NBitfield) (1 << 31);





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NATTRIBUTES(_name, _desc)								NANO_TEST(TAttributes, _name, _desc)

NANO_FIXTURE(TAttributes)
{
	NAttributes		theAttributes;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NATTRIBUTES("Default", "Default state")
{


	// Perform the test
	REQUIRE(theAttributes.GetAttributes() == kNAttributesNone);
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute1));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute2));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute3));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute4));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NATTRIBUTES("Set", "Set attributes")
{


	// Perform the test
	theAttributes.SetAttributes(kTestAttribute1 | kTestAttribute3);
	REQUIRE( theAttributes.HasAttribute(kTestAttribute1));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute2));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute3));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.SetAttributes(kTestAttribute2 | kTestAttribute4);
	REQUIRE( theAttributes.HasAttribute(kTestAttribute1));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute2));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute3));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute4));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NATTRIBUTES("Clear", "Clear attributes")
{


	// Perform the test
	theAttributes.SetAttributes(kTestAttribute1 | kTestAttribute2 | kTestAttribute3 | kTestAttribute4);
	theAttributes.SetAttributes(kNAttributesNone, kTestAttribute1 | kTestAttribute4);
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute1));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute2));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute3));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.ClearAttributes(kTestAttribute2 | kTestAttribute3);
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute1));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute2));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute3));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.SetAttributes(kTestAttribute1 | kTestAttribute2);
	theAttributes.SetAttributes(kTestAttribute3 | kTestAttribute4, kNAttributesAll);
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute1));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute2));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute3));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.SetAttributes(kNAttributesAll);
	REQUIRE(theAttributes.GetAttributes() == kNAttributesAll);
	REQUIRE( theAttributes.HasAttribute(kTestAttribute1));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute2));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute3));
	REQUIRE( theAttributes.HasAttribute(kTestAttribute4));

	theAttributes.ClearAttributes();
	REQUIRE(theAttributes.GetAttributes() == kNAttributesNone);
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute1));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute2));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute3));
	REQUIRE(!theAttributes.HasAttribute(kTestAttribute4));
}


