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
#define TEST_NPROPERTYSTORE(_name, _desc)							NANO_TEST(TPropertyStore, _name, _desc)

NANO_FIXTURE(TPropertyStore)
{
	NPropertyStore		propertyStore;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NPROPERTYSTORE("Default", "Default state")
{


	// Perform the test
	REQUIRE(propertyStore.GetProperties().IsEmpty());
	REQUIRE(propertyStore.GetDefaults().IsEmpty());
}

