/*	NAME:
		TDate.cpp

	DESCRIPTION:
		NDate tests.

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
// Nano
#include "NDate.h"
#include "NFormat.h"
#include "NStdAlgorithm.h"
#include "NTestFixture.h"

// System
#include <array>





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TDate)
{
	NDate theDate;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "Default")
{


	// Perform the test
	REQUIRE(!theDate.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "Constructor")
{


	// Perform the test
	theDate = NDate(2021, 1, 23);
	REQUIRE(theDate.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "IsValid")
{


	// Perform the test
	theDate = NDate(2021, 1, 2);
	REQUIRE(theDate.IsValid());

	theDate = NDate(2021, 111, 2);
	REQUIRE(!theDate.IsValid());

	theDate = NDate(2021, 1, 222);
	REQUIRE(!theDate.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "IsLeapYaer")
{


	// Perform the test
	static std::array<NYear, 49> kLeapYears{
		1904, 1908, 1912, 1916, 1920, 1924, 1928, 1932, 1936, 1940, 1944, 1948, 1952,
		1956, 1960, 1964, 1968, 1972, 1976, 1980, 1984, 1988, 1992, 1996, 2000, 2004,
		2008, 2012, 2016, 2020, 2024, 2028, 2032, 2036, 2040, 2044, 2048, 2052, 2056,
		2060, 2064, 2068, 2072, 2076, 2080, 2084, 2088, 2092, 2096};

	for (NYear theYear = 1900; theYear < 2100; theYear++)
	{
		bool isLeap = nstd::contains(kLeapYears, theYear);
		REQUIRE(NDate(theYear, 1, 1).IsLeapYear() == isLeap);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "GetYear")
{


	// Perform the test
	REQUIRE(theDate.GetYear() == 0);

	theDate.SetYear(2021);
	REQUIRE(theDate.GetYear() == 2021);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "GetMonth")
{


	// Perform the test
	REQUIRE(theDate.GetMonth() == 0);

	theDate.SetMonth(12);
	REQUIRE(theDate.GetMonth() == 12);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "GetDay")
{


	// Perform the test
	REQUIRE(theDate.GetDay() == 0);

	theDate.SetDay(28);
	REQUIRE(theDate.GetDay() == 28);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "GetDayOfWeek")
{


	// Perform the test
	NWeekday theDay = NDate(2021, 3, 3).GetDayOfWeek();
	REQUIRE(theDay == kNWednesday);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "GetDayOfYear")
{


	// Perform the test
	size_t theDay = NDate(2021, 3, 3).GetDayOfYear();
	REQUIRE(theDay == 62);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "GetWeekOfYear")
{


	// Perform the test
	size_t theWeek = NDate(2021, 3, 3).GetWeekOfYear();
	REQUIRE(theWeek == 9);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "GetDaysBetween")
{


	// Perform the test
	NDate dateA(2000, 1, 10);
	NDate dateB(2021, 2, 11);
	NDate dateC(-2000, 1, 10);
	NDate dateD(-2021, 2, 11);

	REQUIRE(dateA.GetDaysBetween(dateB) == 7703);
	REQUIRE(dateB.GetDaysBetween(dateA) == 7703);

	REQUIRE(dateC.GetDaysBetween(dateD) == 7703);
	REQUIRE(dateD.GetDaysBetween(dateC) == 7703);

	REQUIRE(dateA.GetDaysBetween(dateC) == 1460988);
	REQUIRE(dateD.GetDaysBetween(dateB) == 1476394);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "Add")
{


	// Perform the test
	theDate = NDate(2001, 11, 30);
	theDate.Add(0, 0, 1);
	REQUIRE(theDate == NDate(2001, 12, 1));

	theDate = NDate(2001, 11, 30);
	theDate.Add(1, 2, 3);
	REQUIRE(theDate == NDate(2003, 2, 2));


	theDate = NDate(2001, 12, 31);
	theDate.Add(1, 0, 0);
	REQUIRE(theDate == NDate(2002, 12, 31));

	theDate = NDate(2001, 12, 31);
	theDate.Add(0, 1, 0);
	REQUIRE(theDate == NDate(2002, 1, 31));

	theDate = NDate(2001, 12, 31);
	theDate.Add(0, 0, 1);
	REQUIRE(theDate == NDate(2002, 1, 1));


	theDate = NDate(2004, 2, 29);
	theDate.Add(1, 0, 0);
	REQUIRE(theDate == NDate(2005, 2, 28));

	theDate = NDate(2004, 1, 31);
	theDate.Add(0, 1, 0);
	REQUIRE(theDate == NDate(2004, 2, 29));

	theDate = NDate(2004, 1, 29);
	theDate.Add(0, 1, 0);
	REQUIRE(theDate == NDate(2004, 2, 29));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "Subtract")
{


	// Perform the test
	theDate = NDate(2001, 1, 1);
	theDate.Subtract(0, 0, 1);
	REQUIRE(theDate == NDate(2000, 12, 31));

	theDate = NDate(2001, 11, 30);
	theDate.Subtract(1, 2, 3);
	REQUIRE(theDate == NDate(2000, 9, 27));


	theDate = NDate(2001, 1, 1);
	theDate.Subtract(1, 0, 0);
	REQUIRE(theDate == NDate(2000, 1, 1));

	theDate = NDate(2001, 1, 1);
	theDate.Subtract(0, 1, 0);
	REQUIRE(theDate == NDate(2000, 12, 1));

	theDate = NDate(2001, 1, 1);
	theDate.Subtract(0, 0, 1);
	REQUIRE(theDate == NDate(2000, 12, 31));


	theDate = NDate(2004, 2, 29);
	theDate.Subtract(1, 0, 0);
	REQUIRE(theDate == NDate(2003, 2, 28));

	theDate = NDate(2004, 5, 31);
	theDate.Subtract(0, 1, 0);
	REQUIRE(theDate == NDate(2004, 4, 30));

	theDate = NDate(2004, 5, 1);
	theDate.Subtract(0, 0, 1);
	REQUIRE(theDate == NDate(2004, 4, 30));


	theDate = NDate(2004, 2, 29);
	theDate.Subtract(1, 0, 0);
	REQUIRE(theDate == NDate(2003, 2, 28));

	theDate = NDate(2004, 3, 31);
	theDate.Subtract(0, 1, 0);
	REQUIRE(theDate == NDate(2004, 2, 29));

	theDate = NDate(2004, 3, 1);
	theDate.Subtract(0, 0, 1);
	REQUIRE(theDate == NDate(2004, 2, 29));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "CompareEqual")
{


	// Perform the test
	NDate dateA(2000, 1, 1);
	NDate dateB(2000, 1, 1);
	NDate dateC(2021, 3, 3);

	REQUIRE(dateA == dateB);
	REQUIRE(dateA != dateC);

	REQUIRE(dateB == dateA);
	REQUIRE(dateB != dateC);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "CompareOrder")
{


	// Perform the test
	NDate dateA(2000, 1, 1);
	NDate dateB(2000, 7, 8);
	NDate dateC(2021, 2, 3);

	REQUIRE(dateA <= dateA);
	REQUIRE(dateA < dateB);

	REQUIRE(dateB <= dateB);
	REQUIRE(dateB < dateC);

	REQUIRE(dateC >= dateC);
	REQUIRE(dateC > dateB);

	REQUIRE(dateB >= dateB);
	REQUIRE(dateB > dateA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDate, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", NDate(2001, 1, 1)) == "2001-01-01");
	REQUIRE(NFormat("{}", NDate(2010, 10, 10)) == "2010-10-10");
	REQUIRE(NFormat("{}", NDate(2020, 5, 20)) == "2020-05-20");
}
