/*	NAME:
		TAtomicInt.cpp

	DESCRIPTION:
		NAtomicInt tests.

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
#include "NAtomicInt.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NATOMICINT(...)										TEST_NANO(TAtomicInt, ##__VA_ARGS__)

FIXTURE_NANO(TAtomicInt)
{
	NAtomicInt	theValue;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NATOMICINT("Arithmetic")
{


	// Perform the test
	theValue = 1;
	REQUIRE((theValue == 1));
	REQUIRE((theValue != 0));

	theValue++;
	REQUIRE((theValue == 2));

	theValue--;
	REQUIRE((theValue == 1));

	theValue += 3;
	REQUIRE((theValue == 4));

	theValue -= 4;
	REQUIRE((theValue == 0));
}




