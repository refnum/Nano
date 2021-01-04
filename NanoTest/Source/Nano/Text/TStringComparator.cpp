/*	NAME:
		TStringComparator.cpp

	DESCRIPTION:
		NStringComparator tests.

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
#include "NStringComparator.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NString kTestStringUTF8                        = "Hello World";
static const NString kTestStringUTF16                       = u"Hello World";
static const NString kTestStringUTF32                       = U"Hello World";





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(NStringComparator)
{
	NComparison theResult;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringComparator, "Compare")
{


	// Perform the test
	theResult = NStringComparator::Compare("A", "B");
	REQUIRE(theResult == NComparison::LessThan);

	theResult = NStringComparator::Compare("A", "a");
	REQUIRE(theResult == NComparison::LessThan);


	theResult = NStringComparator::Compare("A", "A");
	REQUIRE(theResult == NComparison::EqualTo);

	theResult = NStringComparator::Compare("a", "a");
	REQUIRE(theResult == NComparison::EqualTo);


	theResult = NStringComparator::Compare("B", "A");
	REQUIRE(theResult == NComparison::GreaterThan);

	theResult = NStringComparator::Compare("a", "A");
	REQUIRE(theResult == NComparison::GreaterThan);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringComparator, "CompareFast/MatchingEncoding")
{


	// Perform the test
	NString stringA = kTestStringUTF8;
	NString stringB = kTestStringUTF8;

	theResult = NStringComparator::Compare(stringA, stringB);
	REQUIRE(theResult == NComparison::EqualTo);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringComparator, "CompareFast/SharedEncoding")
{


	// Perform the test
	NString stringA = kTestStringUTF8;
	NString stringB = kTestStringUTF16;

	(void) stringB.GetUTF8();

	theResult = NStringComparator::Compare(stringA, stringB);
	REQUIRE(theResult == NComparison::EqualTo);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringComparator, "CompareGeneral")
{


	// Perform the test
	NString stringA = kTestStringUTF8;
	NString stringB = kTestStringUTF16;

	theResult = NStringComparator::Compare(stringA, stringB);
	REQUIRE(theResult == NComparison::EqualTo);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringComparator, "NoCase")
{


	// Perform the test
	theResult = NStringComparator::Compare("aa", "AA", kNStringNone);
	REQUIRE(theResult == NComparison::GreaterThan);

	theResult = NStringComparator::Compare("aa", "AA", kNStringNoCase);
	REQUIRE(theResult == NComparison::EqualTo);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringComparator, "Numeric")
{


	// Perform the test
	REQUIRE(NStringComparator::Compare("2", "2", kNStringNumeric) == NComparison::EqualTo);
	REQUIRE(NStringComparator::Compare("2.0", "2.0", kNStringNumeric) == NComparison::EqualTo);
	REQUIRE(NStringComparator::Compare("foo2.txt", "foo7.txt", kNStringNumeric) ==
			NComparison::LessThan);
	REQUIRE(NStringComparator::Compare("foo7.txt", "foo2.txt", kNStringNumeric) ==
			NComparison::GreaterThan);
	REQUIRE(NStringComparator::Compare("foo002.txt", "foo007.txt", kNStringNumeric) ==
			NComparison::LessThan);
	REQUIRE(NStringComparator::Compare("123", "0123", kNStringNumeric) == NComparison::LessThan);
	REQUIRE(NStringComparator::Compare("99", "0123", kNStringNumeric) == NComparison::LessThan);
	REQUIRE(NStringComparator::Compare("010", "0002", kNStringNumeric) == NComparison::GreaterThan);
	REQUIRE(NStringComparator::Compare("0100", "20", kNStringNumeric) == NComparison::GreaterThan);
	REQUIRE(NStringComparator::Compare("1", "10", kNStringNumeric) == NComparison::LessThan);
	REQUIRE(NStringComparator::Compare("2", "11", kNStringNumeric) == NComparison::LessThan);
	REQUIRE(NStringComparator::Compare("3", "12", kNStringNumeric) == NComparison::LessThan);
}
