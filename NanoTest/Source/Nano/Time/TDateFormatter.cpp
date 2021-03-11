/*	NAME:
		TDateFormatter.cpp

	DESCRIPTION:
		NDateFormatter tests.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDateFormatter.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr NDate     kNTestDate1                      = NDate(2010, 6, 1);
static constexpr NInterval kNTestSeconds1                   = 66117.0;

static constexpr NDate     kNTestDate2                      = NDate(-123, 6, 1);
static constexpr NInterval kNTestSeconds2                   = 66117.0;

static constexpr NDate     kNTestDate3                      = NDate(2010, 6, 1);
static constexpr NInterval kNTestSeconds3                   = 12117.0;

static constexpr NDate     kNTestDate4                      = NDate(2010, 6, 1);
static constexpr NInterval kNTestSeconds4                   = 425.123468;





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TDateFormatter)
{
	NDateFormatter theFormatter;
	NString        theResult;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Default")
{


	// Perform the test
	theResult = theFormatter.Format();
	REQUIRE(theResult == "00:00 00/00/00");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Date")
{


	// Perform the test
	theFormatter.SetDate(NDate(2010, 7, 13), 12600);
	theResult = theFormatter.Format();
	REQUIRE(theResult == "03:30 10/07/13");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Time")
{


	// Perform the test
	theFormatter.SetDate(300684600);
	theResult = theFormatter.Format();
	REQUIRE(theResult == "03:30 10/07/13");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Quotes")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);

	theResult = theFormatter.Format("G''G");
	REQUIRE(theResult == "AD'AD");

	theResult = theFormatter.Format("G'test'G");
	REQUIRE(theResult == "ADtestAD");

	theResult = theFormatter.Format("G'testG");
	REQUIRE(theResult == "ADtestG");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Era")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);

	theResult = theFormatter.Format("G");
	REQUIRE(theResult == "AD");

	theResult = theFormatter.Format("GG");
	REQUIRE(theResult == "AD");

	theResult = theFormatter.Format("GGG");
	REQUIRE(theResult == "AD");

	theResult = theFormatter.Format("GGGG");
	REQUIRE(theResult == "Anno Domini");

	theResult = theFormatter.Format("GGGGG");
	REQUIRE(theResult == "A");


	theFormatter.SetDate(kNTestDate2, kNTestSeconds2);

	theResult = theFormatter.Format("G");
	REQUIRE(theResult == "BC");

	theResult = theFormatter.Format("GG");
	REQUIRE(theResult == "BC");

	theResult = theFormatter.Format("GGG");
	REQUIRE(theResult == "BC");

	theResult = theFormatter.Format("GGGG");
	REQUIRE(theResult == "Before Christ");

	theResult = theFormatter.Format("GGGGG");
	REQUIRE(theResult == "B");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Year")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);

	theResult = theFormatter.Format("y");
	REQUIRE(theResult == "2010");

	theResult = theFormatter.Format("yy");
	REQUIRE(theResult == "10");

	theResult = theFormatter.Format("yyy");
	REQUIRE(theResult == "2010");

	theResult = theFormatter.Format("yyyy");
	REQUIRE(theResult == "2010");

	theResult = theFormatter.Format("yyyyy");
	REQUIRE(theResult == "02010");


	theResult = theFormatter.Format("YYYYY");
	REQUIRE(theResult == "02010");

	theResult = theFormatter.Format("uuuuu");
	REQUIRE(theResult == "02010");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Quarter")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);

	theResult = theFormatter.Format("Q");
	REQUIRE(theResult == "2");

	theResult = theFormatter.Format("QQ");
	REQUIRE(theResult == "02");

	theResult = theFormatter.Format("QQQ");
	REQUIRE(theResult == "Q2");

	theResult = theFormatter.Format("QQQQ");
	REQUIRE(theResult == "2nd Quarter");


	theResult = theFormatter.Format("q");
	REQUIRE(theResult == "2");

	theResult = theFormatter.Format("qq");
	REQUIRE(theResult == "02");

	theResult = theFormatter.Format("qqq");
	REQUIRE(theResult == "Q2");

	theResult = theFormatter.Format("qqqq");
	REQUIRE(theResult == "2nd Quarter");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Month")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);

	theResult = theFormatter.Format("M");
	REQUIRE(theResult == "6");

	theResult = theFormatter.Format("MM");
	REQUIRE(theResult == "06");

	theResult = theFormatter.Format("MMM");
	REQUIRE(theResult == "Jun");

	theResult = theFormatter.Format("MMMM");
	REQUIRE(theResult == "June");

	theResult = theFormatter.Format("MMMMM");
	REQUIRE(theResult == "J");


	theResult = theFormatter.Format("L");
	REQUIRE(theResult == "6");

	theResult = theFormatter.Format("LL");
	REQUIRE(theResult == "06");

	theResult = theFormatter.Format("LLL");
	REQUIRE(theResult == "Jun");

	theResult = theFormatter.Format("LLLL");
	REQUIRE(theResult == "June");

	theResult = theFormatter.Format("LLLLL");
	REQUIRE(theResult == "J");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Week")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);

	theResult = theFormatter.Format("w");
	REQUIRE(theResult == "22");

	theResult = theFormatter.Format("W");
	REQUIRE(theResult == "1");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Day")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);

	theResult = theFormatter.Format("d");
	REQUIRE(theResult == "1");

	theResult = theFormatter.Format("D");
	REQUIRE(theResult == "152");

	//	theResult = theFormatter.Format("F");
	//	REQUIRE(theResult == "TODO");
	//
	//	theResult = theFormatter.Format("g");
	//	REQUIRE(theResult == "TODO");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "WeekDay")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);

	theResult = theFormatter.Format("E");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format("EE");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format("EEE");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format("EEEE");
	REQUIRE(theResult == "Tuesday");

	theResult = theFormatter.Format("EEEEE");
	REQUIRE(theResult == "T");


	theResult = theFormatter.Format("e");
	REQUIRE(theResult == "2");

	theResult = theFormatter.Format("ee");
	REQUIRE(theResult == "02");

	theResult = theFormatter.Format("eee");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format("eeee");
	REQUIRE(theResult == "Tuesday");

	theResult = theFormatter.Format("eeeee");
	REQUIRE(theResult == "T");


	theResult = theFormatter.Format("c");
	REQUIRE(theResult == "2");

	theResult = theFormatter.Format("cc");
	REQUIRE(theResult == "02");

	theResult = theFormatter.Format("ccc");
	REQUIRE(theResult == "Tue");

	theResult = theFormatter.Format("cccc");
	REQUIRE(theResult == "Tuesday");

	theResult = theFormatter.Format("ccccc");
	REQUIRE(theResult == "T");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Period")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);

	theResult = theFormatter.Format("a");
	REQUIRE(theResult == "PM");


	theFormatter.SetDate(kNTestDate3, kNTestSeconds3);
	theResult = theFormatter.Format("a");
	REQUIRE(theResult == "AM");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Hour")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("h");
	REQUIRE(theResult == "6");

	theFormatter.SetDate(kNTestDate3, kNTestSeconds3);
	theResult = theFormatter.Format("h");
	REQUIRE(theResult == "3");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("h");
	REQUIRE(theResult == "12");

	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("hh");
	REQUIRE(theResult == "06");

	theFormatter.SetDate(kNTestDate3, kNTestSeconds3);
	theResult = theFormatter.Format("hh");
	REQUIRE(theResult == "03");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("hh");
	REQUIRE(theResult == "12");


	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("H");
	REQUIRE(theResult == "18");

	theFormatter.SetDate(kNTestDate3, kNTestSeconds3);
	theResult = theFormatter.Format("H");
	REQUIRE(theResult == "3");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("H");
	REQUIRE(theResult == "0");

	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("HH");
	REQUIRE(theResult == "18");

	theFormatter.SetDate(kNTestDate3, kNTestSeconds3);
	theResult = theFormatter.Format("HH");
	REQUIRE(theResult == "03");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("HH");
	REQUIRE(theResult == "00");


	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("K");
	REQUIRE(theResult == "6");

	theFormatter.SetDate(kNTestDate3, kNTestSeconds3);
	theResult = theFormatter.Format("K");
	REQUIRE(theResult == "3");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("K");
	REQUIRE(theResult == "0");

	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("KK");
	REQUIRE(theResult == "06");

	theFormatter.SetDate(kNTestDate3, kNTestSeconds3);
	theResult = theFormatter.Format("KK");
	REQUIRE(theResult == "03");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("KK");
	REQUIRE(theResult == "00");


	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("k");
	REQUIRE(theResult == "18");

	theFormatter.SetDate(kNTestDate3, kNTestSeconds3);
	theResult = theFormatter.Format("k");
	REQUIRE(theResult == "3");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("k");
	REQUIRE(theResult == "24");

	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("kk");
	REQUIRE(theResult == "18");

	theFormatter.SetDate(kNTestDate3, kNTestSeconds3);
	theResult = theFormatter.Format("kk");
	REQUIRE(theResult == "03");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("kk");
	REQUIRE(theResult == "24");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Minute")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("m");
	REQUIRE(theResult == "21");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("m");
	REQUIRE(theResult == "7");

	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("mm");
	REQUIRE(theResult == "21");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("mm");
	REQUIRE(theResult == "07");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDateFormatter, "Second")
{


	// Perform the test
	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("s");
	REQUIRE(theResult == "57");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("s");
	REQUIRE(theResult == "5");

	theFormatter.SetDate(kNTestDate1, kNTestSeconds1);
	theResult = theFormatter.Format("s");
	REQUIRE(theResult == "57");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("ss");
	REQUIRE(theResult == "05");


	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("S");
	REQUIRE(theResult == "1");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("SS");
	REQUIRE(theResult == "12");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("SSS");
	REQUIRE(theResult == "123");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("SSSS");
	REQUIRE(theResult == "1235");

	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("SSSSS");
	REQUIRE(theResult == "12347");


	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("A");
	REQUIRE(theResult == "425123");


	theFormatter.SetDate(kNTestDate4, kNTestSeconds4);
	theResult = theFormatter.Format("ss.SSS");
	REQUIRE(theResult == "05.123");
}
