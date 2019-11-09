/*	NAME:
		TPropertyStore.cpp

	DESCRIPTION:
		NPropertyStore tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPropertyStore.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NPROPERTYSTORE(...)									TEST_NANO(TPropertyStore, ##__VA_ARGS__)

FIXTURE_NANO(TPropertyStore)
{
	NPropertyStore		propertyStore;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NPROPERTYSTORE("Default")
{


	// Perform the test
	REQUIRE(propertyStore.GetProperties().IsEmpty());
	REQUIRE(propertyStore.GetDefaults().IsEmpty());
}

