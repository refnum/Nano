/*	NAME:
		TDateFormatter.cpp

	DESCRIPTION:
		NDateFormatter tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDateFormatter.h"

#include "TDateFormatter.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NGregorianDate kValueDate1								= { 2010, 6, 1, 18, 21, 57,       kNTimeZoneUTC };
static const NGregorianDate kValueDate2								= { -123, 6, 1, 18, 21, 57,       kNTimeZoneUTC };
static const NGregorianDate kValueDate3								= { 2010, 6, 1,  3, 21, 57,       kNTimeZoneUTC };
static const NGregorianDate kValueDate4								= { 2010, 6, 1,  0,  7,  5.12343, kNTimeZoneUTC };





//============================================================================
//		TDateFormatter::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDateFormatter::Execute(void)
{	NDateFormatter		theFormatter;
	NString				theResult;



	// Quotes
	theResult = theFormatter.Format(kValueDate1, "G''G");
	NN_ASSERT(theResult == "AD'AD");

	theResult = theFormatter.Format(kValueDate1, "G'test'G");
	NN_ASSERT(theResult == "ADtestAD");
	
	theResult = theFormatter.Format(kValueDate1, "G'testG");
	NN_ASSERT(theResult == "ADtestG");



	// Era
	theResult = theFormatter.Format(kValueDate1, "G");
	NN_ASSERT(theResult == "AD");
	
	theResult = theFormatter.Format(kValueDate1, "GG");
	NN_ASSERT(theResult == "AD");
	
	theResult = theFormatter.Format(kValueDate1, "GGG");
	NN_ASSERT(theResult == "AD");
	
	theResult = theFormatter.Format(kValueDate1, "GGGG");
	NN_ASSERT(theResult == "Anno Domini");
	
	theResult = theFormatter.Format(kValueDate1, "GGGGG");
	NN_ASSERT(theResult == "A");


#if NN_TARGET_WINDOWS
	NN_LOG("Ignore next 5 asserts: NWinTime requires dates >= 1601");
#endif

	theResult = theFormatter.Format(kValueDate2, "G");
	NN_ASSERT(theResult == "BC");
	
	theResult = theFormatter.Format(kValueDate2, "GG");
	NN_ASSERT(theResult == "BC");
	
	theResult = theFormatter.Format(kValueDate2, "GGG");
	NN_ASSERT(theResult == "BC");
	
	theResult = theFormatter.Format(kValueDate2, "GGGG");
	NN_ASSERT(theResult == "Before Christ");
	
	theResult = theFormatter.Format(kValueDate2, "GGGGG");
	NN_ASSERT(theResult == "B");



	// Year
	theResult = theFormatter.Format(kValueDate1, "y");
	NN_ASSERT(theResult == "2010");

	theResult = theFormatter.Format(kValueDate1, "yy");
	NN_ASSERT(theResult == "10");

	theResult = theFormatter.Format(kValueDate1, "yyy");
	NN_ASSERT(theResult == "2010");

	theResult = theFormatter.Format(kValueDate1, "yyyy");
	NN_ASSERT(theResult == "2010");

	theResult = theFormatter.Format(kValueDate1, "yyyyy");
	NN_ASSERT(theResult == "02010");


	theResult = theFormatter.Format(kValueDate1, "YYYYY");
	NN_ASSERT(theResult == "02010");

	theResult = theFormatter.Format(kValueDate1, "uuuuu");
	NN_ASSERT(theResult == "02010");



	// Quarter
	theResult = theFormatter.Format(kValueDate1, "Q");
	NN_ASSERT(theResult == "2");

	theResult = theFormatter.Format(kValueDate1, "QQ");
	NN_ASSERT(theResult == "02");

	theResult = theFormatter.Format(kValueDate1, "QQQ");
	NN_ASSERT(theResult == "Q2");

	theResult = theFormatter.Format(kValueDate1, "QQQQ");
	NN_ASSERT(theResult == "2nd Quarter");


	theResult = theFormatter.Format(kValueDate1, "q");
	NN_ASSERT(theResult == "2");

	theResult = theFormatter.Format(kValueDate1, "qq");
	NN_ASSERT(theResult == "02");

	theResult = theFormatter.Format(kValueDate1, "qqq");
	NN_ASSERT(theResult == "Q2");

	theResult = theFormatter.Format(kValueDate1, "qqqq");
	NN_ASSERT(theResult == "2nd Quarter");



	// Month
	theResult = theFormatter.Format(kValueDate1, "M");
	NN_ASSERT(theResult == "6");

	theResult = theFormatter.Format(kValueDate1, "MM");
	NN_ASSERT(theResult == "06");

	theResult = theFormatter.Format(kValueDate1, "MMM");
	NN_ASSERT(theResult == "Jun");

	theResult = theFormatter.Format(kValueDate1, "MMMM");
	NN_ASSERT(theResult == "June");

	theResult = theFormatter.Format(kValueDate1, "MMMMM");
	NN_ASSERT(theResult == "J");


	theResult = theFormatter.Format(kValueDate1, "L");
	NN_ASSERT(theResult == "6");

	theResult = theFormatter.Format(kValueDate1, "LL");
	NN_ASSERT(theResult == "06");

	theResult = theFormatter.Format(kValueDate1, "LLL");
	NN_ASSERT(theResult == "Jun");

	theResult = theFormatter.Format(kValueDate1, "LLLL");
	NN_ASSERT(theResult == "June");

	theResult = theFormatter.Format(kValueDate1, "LLLLL");
	NN_ASSERT(theResult == "J");



	// Week
	theResult = theFormatter.Format(kValueDate1, "w");
	NN_ASSERT(theResult == "22");

	theResult = theFormatter.Format(kValueDate1, "W");
	NN_ASSERT(theResult == "1");



	// Day
	theResult = theFormatter.Format(kValueDate1, "d");
	NN_ASSERT(theResult == "1");

	theResult = theFormatter.Format(kValueDate1, "D");
	NN_ASSERT(theResult == "152");

/*
	theResult = theFormatter.Format(kValueDate1, "F");
	NN_ASSERT(theResult == "TODO");

	theResult = theFormatter.Format(kValueDate1, "g");
	NN_ASSERT(theResult == "TODO");
*/



	// Week day
	theResult = theFormatter.Format(kValueDate1, "E");
	NN_ASSERT(theResult == "Tue");

	theResult = theFormatter.Format(kValueDate1, "EE");
	NN_ASSERT(theResult == "Tue");

	theResult = theFormatter.Format(kValueDate1, "EEE");
	NN_ASSERT(theResult == "Tue");

	theResult = theFormatter.Format(kValueDate1, "EEEE");
	NN_ASSERT(theResult == "Tuesday");

	theResult = theFormatter.Format(kValueDate1, "EEEEE");
	NN_ASSERT(theResult == "T");


	theResult = theFormatter.Format(kValueDate1, "e");
	NN_ASSERT(theResult == "2");

	theResult = theFormatter.Format(kValueDate1, "ee");
	NN_ASSERT(theResult == "02");

	theResult = theFormatter.Format(kValueDate1, "eee");
	NN_ASSERT(theResult == "Tue");

	theResult = theFormatter.Format(kValueDate1, "eeee");
	NN_ASSERT(theResult == "Tuesday");

	theResult = theFormatter.Format(kValueDate1, "eeeee");
	NN_ASSERT(theResult == "T");


	theResult = theFormatter.Format(kValueDate1, "c");
	NN_ASSERT(theResult == "2");

	theResult = theFormatter.Format(kValueDate1, "cc");
	NN_ASSERT(theResult == "02");

	theResult = theFormatter.Format(kValueDate1, "ccc");
	NN_ASSERT(theResult == "Tue");

	theResult = theFormatter.Format(kValueDate1, "cccc");
	NN_ASSERT(theResult == "Tuesday");

	theResult = theFormatter.Format(kValueDate1, "ccccc");
	NN_ASSERT(theResult == "T");



	// Period
	theResult = theFormatter.Format(kValueDate1, "a");
	NN_ASSERT(theResult == "PM");

	theResult = theFormatter.Format(kValueDate3, "a");
	NN_ASSERT(theResult == "AM");



	// Hour
	theResult = theFormatter.Format(kValueDate1, "h");
	NN_ASSERT(theResult == "6");

	theResult = theFormatter.Format(kValueDate3, "h");
	NN_ASSERT(theResult == "3");

	theResult = theFormatter.Format(kValueDate4, "h");
	NN_ASSERT(theResult == "12");

	theResult = theFormatter.Format(kValueDate1, "hh");
	NN_ASSERT(theResult == "06");

	theResult = theFormatter.Format(kValueDate3, "hh");
	NN_ASSERT(theResult == "03");

	theResult = theFormatter.Format(kValueDate4, "hh");
	NN_ASSERT(theResult == "12");


	theResult = theFormatter.Format(kValueDate1, "H");
	NN_ASSERT(theResult == "18");

	theResult = theFormatter.Format(kValueDate3, "H");
	NN_ASSERT(theResult == "3");

	theResult = theFormatter.Format(kValueDate4, "H");
	NN_ASSERT(theResult == "0");

	theResult = theFormatter.Format(kValueDate1, "HH");
	NN_ASSERT(theResult == "18");

	theResult = theFormatter.Format(kValueDate3, "HH");
	NN_ASSERT(theResult == "03");

	theResult = theFormatter.Format(kValueDate4, "HH");
	NN_ASSERT(theResult == "00");


	theResult = theFormatter.Format(kValueDate1, "K");
	NN_ASSERT(theResult == "6");

	theResult = theFormatter.Format(kValueDate3, "K");
	NN_ASSERT(theResult == "3");

	theResult = theFormatter.Format(kValueDate4, "K");
	NN_ASSERT(theResult == "0");

	theResult = theFormatter.Format(kValueDate1, "KK");
	NN_ASSERT(theResult == "06");

	theResult = theFormatter.Format(kValueDate3, "KK");
	NN_ASSERT(theResult == "03");

	theResult = theFormatter.Format(kValueDate4, "KK");
	NN_ASSERT(theResult == "00");


	theResult = theFormatter.Format(kValueDate1, "k");
	NN_ASSERT(theResult == "18");

	theResult = theFormatter.Format(kValueDate3, "k");
	NN_ASSERT(theResult == "3");

	theResult = theFormatter.Format(kValueDate4, "k");
	NN_ASSERT(theResult == "24");

	theResult = theFormatter.Format(kValueDate1, "kk");
	NN_ASSERT(theResult == "18");

	theResult = theFormatter.Format(kValueDate3, "kk");
	NN_ASSERT(theResult == "03");

	theResult = theFormatter.Format(kValueDate4, "kk");
	NN_ASSERT(theResult == "24");



	// Minute
	theResult = theFormatter.Format(kValueDate1, "m");
	NN_ASSERT(theResult == "21");

	theResult = theFormatter.Format(kValueDate4, "m");
	NN_ASSERT(theResult == "7");

	theResult = theFormatter.Format(kValueDate1, "mm");
	NN_ASSERT(theResult == "21");

	theResult = theFormatter.Format(kValueDate4, "mm");
	NN_ASSERT(theResult == "07");



	// Second
	theResult = theFormatter.Format(kValueDate1, "s");
	NN_ASSERT(theResult == "57");

	theResult = theFormatter.Format(kValueDate4, "s");
	NN_ASSERT(theResult == "5");

	theResult = theFormatter.Format(kValueDate1, "s");
	NN_ASSERT(theResult == "57");

	theResult = theFormatter.Format(kValueDate4, "ss");
	NN_ASSERT(theResult == "05");


	theResult = theFormatter.Format(kValueDate4, "S");
	NN_ASSERT(theResult == "5.1");

	theResult = theFormatter.Format(kValueDate4, "SS");
	NN_ASSERT(theResult == "5.12");

	theResult = theFormatter.Format(kValueDate4, "SSS");
	NN_ASSERT(theResult == "5.123");

	theResult = theFormatter.Format(kValueDate4, "SSSS");
	NN_ASSERT(theResult == "5.1234");

	theResult = theFormatter.Format(kValueDate4, "SSSSS");
	NN_ASSERT(theResult == "5.12343");


/*
	theResult = theFormatter.Format(kValueDate1, "A");
	NN_ASSERT(theResult == "TODO");
*/

	

}


