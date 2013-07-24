/*	NAME:
		TDictionary.cpp

	DESCRIPTION:
		NDictionary tests.

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
#include "NDictionary.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NPoint  kTestPoint										= NPoint(10.0f, 10.0f);
static const int32_t kTestInt32										=  2000;
static const int64_t kTestInt64										= -4000;





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDICTIONARY(...)										TEST_NANO(TDictionary, ##__VA_ARGS__)

FIXTURE_NANO(TDictionary)
{
	NDictionary		theDict, theDict2;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDICTIONARY("Set")
{


	// Perform the test
	theDict.SetValue( "point", kTestPoint);
	theDict.SetValue( "int32", kTestInt32);
	theDict2.SetValue("int64", kTestInt64);

	REQUIRE(theDict.GetSize()  == 2);
	REQUIRE(theDict2.GetSize() == 1);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDICTIONARY("Join")
{


	// Perform the test
	theDict.SetValue( "point", kTestPoint);
	theDict.SetValue( "int32", kTestInt32);
	theDict2.SetValue("int64", kTestInt64);

	theDict.Join(theDict2);
	REQUIRE(theDict.GetSize()        == 3);
	REQUIRE(theDict.GetKeys().size() == 3);

	REQUIRE(!theDict.HasKey("fail"));
	REQUIRE(theDict.GetValuePoint("point") == kTestPoint);
	REQUIRE(theDict.GetValueInt32("int32") == kTestInt32);
	REQUIRE(theDict.GetValueInt64("int64") == kTestInt64);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDICTIONARY("Remove")
{


	// Perform the test
	theDict.SetValue("remove", 3.1415);
	REQUIRE(theDict.HasKey("remove"));

	theDict.RemoveKey("remove");
	REQUIRE(!theDict.HasKey("remove"));
}

