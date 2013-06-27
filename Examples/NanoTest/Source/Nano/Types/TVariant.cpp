/*	NAME:
		TVariant.cpp

	DESCRIPTION:
		NVariant tests.

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
#include "NVariant.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NVARIANT(_name, _desc)									NANO_TEST(TVariant, _name, _desc)

NANO_FIXTURE(TVariant)
{
	NVariant	theValue;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NVARIANT("NData", "NData")
{	NData			dataValue;
	const NData		*dataPtr;
	bool			gotValue;



	// Perform the test
	dataValue.AppendData(4, "test");
	theValue = dataValue;
	
	dataPtr = theValue.GetValue<NData>();
	REQUIRE(dataPtr != (const NData *) NULL);
	
	gotValue = theValue.GetValue(dataValue);
	REQUIRE(gotValue);
}



