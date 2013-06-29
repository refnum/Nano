/*	NAME:
		TCacheKey.cpp

	DESCRIPTION:
		NCacheKey tests.

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
#include "NCacheKey.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestString1									= "first test string";
static const NString kTestString2									= "another test string";

static const NData kTestData1										= kTestString1.GetData();
static const NData kTestData2										= kTestString2.GetData();

static void *kTestPointer1											= NN_TARGET_ARCH_64 ? ((void *) 0xDEADBEEFDEADBEEFLL) : ((void *) 0xDEADBEEF);
static void *kTestPointer2											= NN_TARGET_ARCH_64 ? ((void *) 0xFEEDFACEFEEDFACELL) : ((void *) 0xFEEDFACE);

static NHashCode kTestHash1											= 12345;
static NHashCode kTestHash2											= 67890;





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NCACHEKEY(_name, _desc)								TEST_NANO(TCacheKey, _name, _desc)

FIXTURE_NANO(TCacheKey)
{
	NCacheKey		testKey1, testKey2;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHEKEY("Default", "Default state test")
{


	// Perform the test
	REQUIRE(testKey1 == testKey2);
	REQUIRE(!testKey1.IsValid());
	REQUIRE(!testKey2.IsValid());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHEKEY("Strings", "String cache keys")
{


	// Perform the test
	testKey1 = NCacheKey(kTestString1);
	testKey2 = NCacheKey(kTestString2);

	REQUIRE(testKey1 != testKey2);
	REQUIRE(testKey1.IsValid());
	REQUIRE(testKey2.IsValid());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHEKEY("Data", "Data cache keys")
{


	// Perform the test
	testKey1 = NCacheKey(kTestData1);
	testKey2 = NCacheKey(kTestData2);

	REQUIRE(testKey1 != testKey2);
	REQUIRE(testKey1.IsValid());
	REQUIRE(testKey2.IsValid());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHEKEY("Pointers", "Pointer cache keys")
{


	// Perform the test
	testKey1 = NCacheKey(kTestPointer1);
	testKey2 = NCacheKey(kTestPointer2);

	REQUIRE(testKey1 != testKey2);
	REQUIRE(testKey1.IsValid());
	REQUIRE(testKey2.IsValid());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHEKEY("Indices", "NHashCode cache keys")
{


	// Perform the test
	testKey1 = NCacheKey(kTestHash1);
	testKey2 = NCacheKey(kTestHash2);

	REQUIRE(testKey1 != testKey2);
	REQUIRE(testKey1.IsValid());
	REQUIRE(testKey2.IsValid());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHEKEY("Set", "Set cache key")
{


	// Perform the test
	testKey1 = NCacheKey(kTestHash1);
	testKey2 = NCacheKey(kTestHash2);
	REQUIRE(testKey1 != testKey2);

	testKey1.SetValue(kTestHash2);
	REQUIRE(testKey1 == testKey2);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCACHEKEY("Clear", "Clear cache key")
{


	// Perform the test
	testKey1 = NCacheKey(kTestHash1);
	testKey2 = NCacheKey(kTestHash2);

	REQUIRE(testKey1 != testKey2);
	REQUIRE(testKey1.IsValid());
	REQUIRE(testKey2.IsValid());

	testKey1.Clear();
	REQUIRE(testKey1 != testKey2);
	REQUIRE(!testKey1.IsValid());

	testKey2.Clear();
	REQUIRE(testKey1 == testKey2);
	REQUIRE(!testKey2.IsValid());
}


