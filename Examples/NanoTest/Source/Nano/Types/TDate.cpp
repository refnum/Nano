/*	NAME:
		TDate.cpp

	DESCRIPTION:
		NDate tests.

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
#include "NDate.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NGregorianDate kTestDate1								= { 2010, 6, 1, 18, 21, 57, kNTimeZoneUTC  };
static const NGregorianDate kTestDate2								= { 2010, 6, 1, 11, 21, 57, kNTimeZonePDT  };
static const NGregorianDate kTestDate3								= { 2010, 6, 1, 20, 21, 57, kNTimeZoneCEST };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDATE(_name, _desc)									NANO_TEST(TDate, _name, _desc)

NANO_FIXTURE(TDate)
{
	NDate		theDate;

	SETUP
	{
		theDate = NDate(kTestDate1);
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATE("Conversion", "Date conversion")
{	NGregorianDate		gregDate;



	// Perform the test
	gregDate = theDate.GetDate(kNTimeZoneUTC);
	REQUIRE(NDate(gregDate) == NDate(kTestDate1));

	gregDate = theDate.GetDate(kNTimeZonePDT);
	REQUIRE(NDate(gregDate) == NDate(kTestDate2));

	gregDate = theDate.GetDate(kNTimeZoneCEST);
	REQUIRE(NDate(gregDate) == NDate(kTestDate3));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATE("Indices", "Date indices")
{	NIndex		n;



	// Indices
	n = theDate.GetDayOfWeek();
	REQUIRE(n == 2);

	n = theDate.GetDayOfYear();
	REQUIRE(n == 152);

	n = theDate.GetWeekOfYear();
	REQUIRE(n == 22);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATE("Weeks", "Weeks of year")
{	NGregorianDate		gregDate;
	NIndex				n, m;



	// Perform the test
	gregDate       = kTestDate1;
	gregDate.month = 1;

	for (m = 1; m <= 10; m++)
		{
		gregDate.day = (SInt8) m;
		n            = NDate(gregDate).GetWeekOfYear();
		REQUIRE(n == 1);
		}

	gregDate.day = 11;
	n            = NDate(gregDate).GetWeekOfYear();
	REQUIRE(n == 2);
}


