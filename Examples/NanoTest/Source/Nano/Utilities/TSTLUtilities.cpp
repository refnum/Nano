/*	NAME:
		TSTLUtilities.cpp

	DESCRIPTION:
		NSTLUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NTestFixture.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt32List kTestList1									= nvector<UInt32>(11, 12, 13);
static const UInt32List kTestList2									= nvector<UInt32>(24, 25, 26);





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSTLUTILITIES(...)										TEST_NANO(TSTLUtilities, ##__VA_ARGS__)

FIXTURE_NANO(TSTLUtilities)
{
	UInt32List		theList;
	
	SETUP
	{
		theList = kTestList1;
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTLUTILITIES("Append")
{


	// Perform the test
	append(theList, kTestList2);
	
	REQUIRE(theList.size() == (kTestList1.size() + kTestList2.size()));
	REQUIRE(theList[0] == kTestList1[0]);
	REQUIRE(theList[1] == kTestList1[1]);
	REQUIRE(theList[2] == kTestList1[2]);
	REQUIRE(theList[3] == kTestList2[0]);
	REQUIRE(theList[4] == kTestList2[1]);
	REQUIRE(theList[5] == kTestList2[2]);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTLUTILITIES("Prepend")
{


	// Perform the test
	theList = kTestList1;
	prepend(theList, kTestList2);
	
	REQUIRE(theList.size() == (kTestList1.size() + kTestList2.size()));
	REQUIRE(theList[0] == kTestList2[0]);
	REQUIRE(theList[1] == kTestList2[1]);
	REQUIRE(theList[2] == kTestList2[2]);
	REQUIRE(theList[3] == kTestList1[0]);
	REQUIRE(theList[4] == kTestList1[1]);
	REQUIRE(theList[5] == kTestList1[2]);
}



