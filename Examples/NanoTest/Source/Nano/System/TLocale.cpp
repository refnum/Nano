/*	NAME:
		TLocale.cpp

	DESCRIPTION:
		NLocale tests.

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
#include "NLocale.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NLOCALE(_name, _desc)								NANO_TEST(TLocale, _name, _desc)

NANO_FIXTURE(TLocale)
{
	NLocale		theLocale;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NLOCALE("Default", "Default state")
{	bool	valueBool;



	// Perform the test
	valueBool = theLocale.IsMetric();
	REQUIRE((valueBool == true || valueBool == false));
}


