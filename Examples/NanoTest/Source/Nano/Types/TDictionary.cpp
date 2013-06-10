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
static const NPoint kTestPoint										= NPoint(10.0f, 10.0f);
static const SInt32 kTestSInt32										=  2000;
static const SInt64 kTestSInt64										= -4000;





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDICTIONARY(_name, _desc)								NANO_TEST(TDictionary, _name, _desc)

NANO_FIXTURE(TDictionary)
{
	NDictionary		theDict, theDict2;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDICTIONARY("Set", "Set value")
{


	// Perform the test
	theDict.SetValue("point",   kTestPoint);
	theDict.SetValue("sint32",  kTestSInt32);
	theDict2.SetValue("sint64", kTestSInt64);

	NN_ASSERT(theDict.GetSize()  == 2);
	NN_ASSERT(theDict2.GetSize() == 1);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDICTIONARY("Join", "Join dictionaries")
{


	// Perform the test
	theDict.SetValue("point",   kTestPoint);
	theDict.SetValue("sint32",  kTestSInt32);
	theDict2.SetValue("sint64", kTestSInt64);

	theDict.Join(theDict2);
	NN_ASSERT(theDict.GetSize()        == 3);
	NN_ASSERT(theDict.GetKeys().size() == 3);

	NN_ASSERT(!theDict.HasKey("fail"));
	NN_ASSERT(theDict.GetValuePoint("point")   == kTestPoint);
	NN_ASSERT(theDict.GetValueSInt32("sint32") == kTestSInt32);
	NN_ASSERT(theDict.GetValueSInt64("sint64") == kTestSInt64);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDICTIONARY("Remove", "Remove value")
{


	// Perform the test
	theDict.SetValue("remove", 3.1415);
	NN_ASSERT(theDict.HasKey("remove"));

	theDict.RemoveKey("remove");
	NN_ASSERT(!theDict.HasKey("remove"));
}

