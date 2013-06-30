/*	NAME:
		TAtomicList.cpp

	DESCRIPTION:
		NAtomicList tests.

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
#include "NAtomicList.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NATOMICLIST(...)										TEST_NANO(TAtomicList, ##__VA_ARGS__)

FIXTURE_NANO(TAtomicList)
{
	NAtomicList<UInt32>		theList;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NATOMICLIST("Default")
{


	// Perform the test
	REQUIRE(theList.IsEmpty());
}




