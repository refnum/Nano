/*	NAME:
		TDateFormatter.cpp

	DESCRIPTION:
		NDateFormatter tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDateFormatter.h"
#include "NTestFixture.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NGregorianDate kTestDate1								= { 2010, 6, 1, 18, 21, 57,       kNTimeZoneUTC };
static const NGregorianDate kTestDate2								= { -123, 6, 1, 18, 21, 57,       kNTimeZoneUTC };
static const NGregorianDate kTestDate3								= { 2010, 6, 1,  3, 21, 57,       kNTimeZoneUTC };
static const NGregorianDate kTestDate4								= { 2010, 6, 1,  0,  7,  5.12345, kNTimeZoneUTC };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDATEFORMATTER(_name, _desc)							NANO_TEST(TDateFormatter, _name, _desc)

NANO_FIXTURE(TDateFormatter)
{
	NDateFormatter		theFormatter;
	NString				theResult;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Quotes", "Quotes")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "G''G");
	REQUIRE(theResult == "AD'AD");

	theResult = theFormatter.Format(kTestDate1, "G'test'G");
	REQUIRE(theResult == "ADtestAD");
	
	theResult = theFormatter.Format(kTestDate1, "G'testG");
	REQUIRE(theResult == "ADtestG");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Era", "Era")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "G");
	REQUIRE(theResult == "AD");
	
	theResult = theFormatter.Format(kTestDate1, "GG");
	REQUIRE(theResult == "AD");
	
	theResult = theFormatter.Format(kTestDate1, "GGG");
	REQUIRE(theResult == "AD");
	
	theResult = theFormatter.Format(kTestDate1, "GGGG");
	REQUIRE(theResult == "Anno Domini");
	
	theResult = theFormatter.Format(kTestDate1, "GGGGG");
	REQUIRE(theResult == "A");


#if NN_TARGET_WINDOWS
	NN_LOG("Skipping BC tests: NWinTime requires dates >= 1601");
#else
	theResult = theFormatter.Format(kTestDate2, "G");
	REQUIRE(theResult == "BC");
	
	theResult = theFormatter.Format(kTestDate2, "GG");
	REQUIRE(theResult == "BC");
	
	theResult = theFormatter.Format(kTestDate2, "GGG");
	REQUIRE(theResult == "BC");
	
	theResult = theFormatter.Format(kTestDate2, "GGGG");
	REQUIRE(theResult == "Before Christ");
	
	theResult = theFormatter.Format(kTestDate2, "GGGGG");
	REQUIRE(theResult == "B");
#endif
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Year", "Year")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "y");
	REQUIRE(theResult == "2010");

	theResult = theFormatter.Format(kTestDate1, "yy");
	REQUIRE(theResult == "10");

	theResult = theFormatter.Format(kTestDate1, "yyy");
	REQUIRE(theResult == "2010");

	theResult = theFormatter.Format(kTestDate1, "yyyy");
	REQUIRE(theResult == "2010");

	theResult = theFormatter.Format(kTestDate1, "yyyyy");
	REQUIRE(theResult == "02010");


	theResult = theFormatter.Format(kTestDate1, "YYYYY");
	REQUIRE(theResult == "02010");

	theResult = theFormatter.Format(kTestDate1, "uuuuu");
	REQUIRE(theResult == "02010");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Quarter", "Quarter")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "Q");
	REQUIRE(theResult == "2");

	theResult = theFormatter.Format(kTestDate1, "QQ");
	REQUIRE(theResult == "02");

	theResult = theFormatter.Format(kTestDate1, "QQQ");
	REQUIRE(theResult == "Q2");

	theResult = theFormatter.Format(kTestDate1, "QQQQ");
	REQUIRE(theResult == "2nd Quarter");


	theResult = theFormatter.Format(kTestDate1, "q");
	REQUIRE(theResult == "2");

	theResult = theFormatter.Format(kTestDate1, "qq");
	REQUIRE(theResult == "02");

	theResult = theFormatter.Format(kTestDate1, "qqq");
	REQUIRE(theResult == "Q2");

	theResult = theFormatter.Format(kTestDate1, "qqqq");
	REQUIRE(theResult == "2nd Quarter");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Month", "Month")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "M");
	REQUIRE(theResult == "6");

	theResult = theFormatter.Format(kTestDate1, "MM");
	REQUIRE(theResult == "06");

	theResult = theFormatter.Format(kTestDate1, "MMM");
	REQUIRE(theResult == "Jun");

	theResult = theFormatter.Format(kTestDate1, "MMMM");
	REQUIRE(theResult == "June");

	theResult = theFormatter.Format(kTestDate1, "MMMMM");
	REQUIRE(theResult == "J");


	theResult = theFormatter.Format(kTestDate1, "L");
	REQUIRE(theResult == "6");

	theResult = theFormatter.Format(kTestDate1, "LL");
	REQUIRE(theResult == "06");

	theResult = theFormatter.Format(kTestDate1, "LLL");
	REQUIRE(theResult == "Jun");

	theResult = theFormatter.Format(kTestDate1, "LLLL");
	REQUIRE(theResult == "June");

	theResult = theFormatter.Format(kTestDate1, "LLLLL");
	REQUIRE(theResult == "J");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Week", "Week")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "w");
	REQUIRE(theResult == "22");

	theResult = theFormatter.Format(kTestDate1, "W");
	REQUIRE(theResult == "1");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Day", "Day")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "d");
	REQUIRE(theResult == "1");

	theResult = theFormatter.Format(kTestDate1, "D");
	REQUIRE(theResult == "152");

/*
	theResult = theFormatter.Format(kTestDate1, "F");
	REQUIRE(theResult == "TODO");

	theResult = theFormatter.Format(kTestDate1, "g");
	REQUIRE(theResult == "TODO");
*/
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Week day", "Week day")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "E");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format(kTestDate1, "EE");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format(kTestDate1, "EEE");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format(kTestDate1, "EEEE");
	REQUIRE(theResult == "Tuesday");

	theResult = theFormatter.Format(kTestDate1, "EEEEE");
	REQUIRE(theResult == "T");


	theResult = theFormatter.Format(kTestDate1, "e");
	REQUIRE(theResult == "2");

	theResult = theFormatter.Format(kTestDate1, "ee");
	REQUIRE(theResult == "02");

	theResult = theFormatter.Format(kTestDate1, "eee");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format(kTestDate1, "eeee");
	REQUIRE(theResult == "Tuesday");

	theResult = theFormatter.Format(kTestDate1, "eeeee");
	REQUIRE(theResult == "T");


	theResult = theFormatter.Format(kTestDate1, "c");
	REQUIRE(theResult == "2");

	theResult = theFormatter.Format(kTestDate1, "cc");
	REQUIRE(theResult == "02");

	theResult = theFormatter.Format(kTestDate1, "ccc");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format(kTestDate1, "cccc");
	REQUIRE(theResult == "Tuesday");

	theResult = theFormatter.Format(kTestDate1, "ccccc");
	REQUIRE(theResult == "T");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Period", "Period")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "a");
	REQUIRE(theResult == "PM");

	theResult = theFormatter.Format(kTestDate3, "a");
	REQUIRE(theResult == "AM");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Hour", "Hour")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "h");
	REQUIRE(theResult == "6");

	theResult = theFormatter.Format(kTestDate3, "h");
	REQUIRE(theResult == "3");

	theResult = theFormatter.Format(kTestDate4, "h");
	REQUIRE(theResult == "12");

	theResult = theFormatter.Format(kTestDate1, "hh");
	REQUIRE(theResult == "06");

	theResult = theFormatter.Format(kTestDate3, "hh");
	REQUIRE(theResult == "03");

	theResult = theFormatter.Format(kTestDate4, "hh");
	REQUIRE(theResult == "12");


	theResult = theFormatter.Format(kTestDate1, "H");
	REQUIRE(theResult == "18");

	theResult = theFormatter.Format(kTestDate3, "H");
	REQUIRE(theResult == "3");

	theResult = theFormatter.Format(kTestDate4, "H");
	REQUIRE(theResult == "0");

	theResult = theFormatter.Format(kTestDate1, "HH");
	REQUIRE(theResult == "18");

	theResult = theFormatter.Format(kTestDate3, "HH");
	REQUIRE(theResult == "03");

	theResult = theFormatter.Format(kTestDate4, "HH");
	REQUIRE(theResult == "00");


	theResult = theFormatter.Format(kTestDate1, "K");
	REQUIRE(theResult == "6");

	theResult = theFormatter.Format(kTestDate3, "K");
	REQUIRE(theResult == "3");

	theResult = theFormatter.Format(kTestDate4, "K");
	REQUIRE(theResult == "0");

	theResult = theFormatter.Format(kTestDate1, "KK");
	REQUIRE(theResult == "06");

	theResult = theFormatter.Format(kTestDate3, "KK");
	REQUIRE(theResult == "03");

	theResult = theFormatter.Format(kTestDate4, "KK");
	REQUIRE(theResult == "00");


	theResult = theFormatter.Format(kTestDate1, "k");
	REQUIRE(theResult == "18");

	theResult = theFormatter.Format(kTestDate3, "k");
	REQUIRE(theResult == "3");

	theResult = theFormatter.Format(kTestDate4, "k");
	REQUIRE(theResult == "24");

	theResult = theFormatter.Format(kTestDate1, "kk");
	REQUIRE(theResult == "18");

	theResult = theFormatter.Format(kTestDate3, "kk");
	REQUIRE(theResult == "03");

	theResult = theFormatter.Format(kTestDate4, "kk");
	REQUIRE(theResult == "24");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Minute", "Minute")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "m");
	REQUIRE(theResult == "21");

	theResult = theFormatter.Format(kTestDate4, "m");
	REQUIRE(theResult == "7");

	theResult = theFormatter.Format(kTestDate1, "mm");
	REQUIRE(theResult == "21");

	theResult = theFormatter.Format(kTestDate4, "mm");
	REQUIRE(theResult == "07");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATEFORMATTER("Second", "Second")
{


	// Perform the test
	theResult = theFormatter.Format(kTestDate1, "s");
	REQUIRE(theResult == "57");

	theResult = theFormatter.Format(kTestDate4, "s");
	REQUIRE(theResult == "5");

	theResult = theFormatter.Format(kTestDate1, "s");
	REQUIRE(theResult == "57");

	theResult = theFormatter.Format(kTestDate4, "ss");
	REQUIRE(theResult == "05");


	theResult = theFormatter.Format(kTestDate4, "S");
	REQUIRE(theResult == "1");

	theResult = theFormatter.Format(kTestDate4, "SS");
	REQUIRE(theResult == "12");

	theResult = theFormatter.Format(kTestDate4, "SSS");
	REQUIRE(theResult == "123");

	theResult = theFormatter.Format(kTestDate4, "SSSS");
	REQUIRE(theResult == "1235");

	theResult = theFormatter.Format(kTestDate4, "SSSSS");
	REQUIRE(theResult == "12345");


	theResult = theFormatter.Format(kTestDate4, "ss.SSS");
	REQUIRE(theResult == "05.123");


/*
	theResult = theFormatter.Format(kTestDate1, "A");
	REQUIRE(theResult == "TODO");
*/
}



