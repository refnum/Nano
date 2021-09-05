/*	NAME:
		TStringScanner.cpp

	DESCRIPTION:
		NStringScanner tests.

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
#include "NStringScanner.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NString kTestString =
	"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
	"Vivamus est erat, scelerisque quis molestie a, bibendum eget risus. "
	"Etiam laoreet velit dolor, at eleifend enim ultrices vitae. "
	"Duis aliquet ex ultricies malesuada egestas. "
	"Aliquam interdum sollicitudin ipsum in porta. "
	"Curabitur faucibus semper leo non elementum. "
	"Vivamus scelerisque sapien scelerisque ligula.";

static const NRange kRangeMissed                            = NRange(340, 20);

static const NRange kRangeVivamus1                          = NRange(57, 7);
static const NRange kRangeVivamus1Group1                    = NRange(57, 2);
static const NRange kRangeVivamus1Group2                    = NRange(59, 3);

static const NRange kRangeVivamus2                          = NRange(321, 7);
static const NRange kRangeVivamus2Group1                    = NRange(321, 2);
static const NRange kRangeVivamus2Group2                    = NRange(323, 3);

static const NVectorRange kRangesVivamus1_2                 = {kRangeVivamus1, kRangeVivamus2};
static const NVectorRange kGroupsVivamus1                   = {kRangeVivamus1Group1, kRangeVivamus1Group2};
static const NVectorRange kGroupsVivamus2                   = {kRangeVivamus2Group1, kRangeVivamus2Group2};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TStringScanner){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "Find/Literal")
{


	// Perform the test
	NRange theRange;


	theRange = NStringScanner::Find(kTestString, "Vivamus", kNStringFindExact, kNRangeAll);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", NStringFind::NoCase, kNRangeAll);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kNStringFindExact, kNRangeAll);
	REQUIRE(theRange.IsEmpty());


	theRange = NStringScanner::Find(kTestString, "Vivamus", kNStringFindExact, kRangeMissed);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", NStringFind::NoCase, kRangeMissed);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kNStringFindExact, kRangeMissed);
	REQUIRE(theRange.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "Find/Pattern")
{


	// Perform the test
	NRange theRange;


	theRange = NStringScanner::Find(kTestString, "Viv\\w+\\b", NStringFind::Pattern, kNRangeAll);
	REQUIRE(theRange == kRangeVivamus1);

	theRange =
		NStringScanner::Find(kTestString, "VIV\\w+\\b", kNStringFindPatternNoCase, kNRangeAll);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", NStringFind::Pattern, kNRangeAll);
	REQUIRE(theRange.IsEmpty());


	theRange = NStringScanner::Find(kTestString, "Viv\\w+\\b", NStringFind::Pattern, kRangeMissed);
	REQUIRE(theRange.IsEmpty());

	theRange =
		NStringScanner::Find(kTestString, "VIV\\w+\\b", kNStringFindPatternNoCase, kRangeMissed);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", NStringFind::Pattern, kRangeMissed);
	REQUIRE(theRange.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindAll/Literal")
{


	// Perform the test
	NVectorRange theRanges;



	theRanges = NStringScanner::FindAll(kTestString, "Vivamus", kNStringFindExact, kNRangeAll);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", NStringFind::NoCase, kNRangeAll);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kNStringFindExact, kNRangeAll);
	REQUIRE(theRanges.empty());


	theRanges = NStringScanner::FindAll(kTestString, "Vivamus", kNStringFindExact, kRangeMissed);
	REQUIRE(theRanges.empty());

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", NStringFind::NoCase, kRangeMissed);
	REQUIRE(theRanges.empty());

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kNStringFindExact, kRangeMissed);
	REQUIRE(theRanges.empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindAll/Pattern")
{


	// Perform the test
	NVectorRange theRanges;



	theRanges =
		NStringScanner::FindAll(kTestString, "Viv\\w+\\b", NStringFind::Pattern, kNRangeAll);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges =
		NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kNStringFindPatternNoCase, kNRangeAll);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges =
		NStringScanner::FindAll(kTestString, "VIV\\w+\\b", NStringFind::Pattern, kNRangeAll);
	REQUIRE(theRanges.empty());


	theRanges =
		NStringScanner::FindAll(kTestString, "Viv\\w+\\b", NStringFind::Pattern, kRangeMissed);
	REQUIRE(theRanges.empty());

	theRanges =
		NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kNStringFindPatternNoCase, kRangeMissed);
	REQUIRE(theRanges.empty());

	theRanges =
		NStringScanner::FindAll(kTestString, "VIV\\w+\\b", NStringFind::Pattern, kRangeMissed);
	REQUIRE(theRanges.empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindMatch/Pattern")
{


	// Perform the test
	NPatternMatch patternMatch;

	patternMatch = NStringScanner::FindMatch(kTestString, "Vivamus", kNStringFindExact, kNRangeAll);
	REQUIRE(patternMatch.thePattern == kRangeVivamus1);
	REQUIRE(patternMatch.theGroups.empty());

	patternMatch =
		NStringScanner::FindMatch(kTestString, "VIVAMUS", NStringFind::NoCase, kNRangeAll);
	REQUIRE(patternMatch.thePattern == kRangeVivamus1);
	REQUIRE(patternMatch.theGroups.empty());

	patternMatch = NStringScanner::FindMatch(kTestString, "VIVAMUS", kNStringFindExact, kNRangeAll);
	REQUIRE(patternMatch.thePattern.IsEmpty());
	REQUIRE(patternMatch.theGroups.empty());


	patternMatch =
		NStringScanner::FindMatch(kTestString, "Vivamus", kNStringFindExact, kRangeMissed);
	REQUIRE(patternMatch.thePattern.IsEmpty());
	REQUIRE(patternMatch.theGroups.empty());

	patternMatch =
		NStringScanner::FindMatch(kTestString, "VIVAMUS", NStringFind::NoCase, kRangeMissed);
	REQUIRE(patternMatch.thePattern.IsEmpty());
	REQUIRE(patternMatch.theGroups.empty());

	patternMatch =
		NStringScanner::FindMatch(kTestString, "VIVAMUS", kNStringFindExact, kRangeMissed);
	REQUIRE(patternMatch.thePattern.IsEmpty());
	REQUIRE(patternMatch.theGroups.empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindMatch/Capture")
{


	// Perform the test
	NPatternMatch patternMatch;

	patternMatch =
		NStringScanner::FindMatch(kTestString, "(V.)(v.m)us", kNStringFindExact, kNRangeAll);
	REQUIRE(patternMatch.thePattern == kRangeVivamus1);
	REQUIRE(patternMatch.theGroups == kGroupsVivamus1);

	patternMatch =
		NStringScanner::FindMatch(kTestString, "(V.)(V.M)US", NStringFind::NoCase, kNRangeAll);
	REQUIRE(patternMatch.thePattern == kRangeVivamus1);
	REQUIRE(patternMatch.theGroups == kGroupsVivamus1);

	patternMatch =
		NStringScanner::FindMatch(kTestString, "(V.)(V.M)US", kNStringFindExact, kNRangeAll);
	REQUIRE(patternMatch.thePattern.IsEmpty());
	REQUIRE(patternMatch.theGroups.empty());


	patternMatch =
		NStringScanner::FindMatch(kTestString, "(V.)(v.m)us", kNStringFindExact, kRangeMissed);
	REQUIRE(patternMatch.thePattern.IsEmpty());
	REQUIRE(patternMatch.theGroups.empty());

	patternMatch =
		NStringScanner::FindMatch(kTestString, "(V.)(V.M)US", NStringFind::NoCase, kRangeMissed);
	REQUIRE(patternMatch.thePattern.IsEmpty());
	REQUIRE(patternMatch.theGroups.empty());

	patternMatch =
		NStringScanner::FindMatch(kTestString, "(V.)(V.M)US", kNStringFindExact, kRangeMissed);
	REQUIRE(patternMatch.thePattern.IsEmpty());
	REQUIRE(patternMatch.theGroups.empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindMatches/Pattern")
{


	// Perform the test
	NVectorPatternMatch patternMatches;

	patternMatches =
		NStringScanner::FindMatches(kTestString, "Vivamus", kNStringFindExact, kNRangeAll);
	REQUIRE(patternMatches.size() == 2);
	REQUIRE(patternMatches[0].thePattern == kRangeVivamus1);
	REQUIRE(patternMatches[0].theGroups.empty());
	REQUIRE(patternMatches[1].thePattern == kRangeVivamus2);
	REQUIRE(patternMatches[1].theGroups.empty());

	patternMatches =
		NStringScanner::FindMatches(kTestString, "VIVAMUS", NStringFind::NoCase, kNRangeAll);
	REQUIRE(patternMatches.size() == 2);
	REQUIRE(patternMatches[0].thePattern == kRangeVivamus1);
	REQUIRE(patternMatches[0].theGroups.empty());
	REQUIRE(patternMatches[1].thePattern == kRangeVivamus2);
	REQUIRE(patternMatches[1].theGroups.empty());

	patternMatches =
		NStringScanner::FindMatches(kTestString, "VIVAMUS", kNStringFindExact, kNRangeAll);
	REQUIRE(patternMatches.empty());


	patternMatches =
		NStringScanner::FindMatches(kTestString, "Vivamus", kNStringFindExact, kRangeMissed);
	REQUIRE(patternMatches.empty());

	patternMatches =
		NStringScanner::FindMatches(kTestString, "VIVAMUS", NStringFind::NoCase, kRangeMissed);
	REQUIRE(patternMatches.empty());

	patternMatches =
		NStringScanner::FindMatches(kTestString, "VIVAMUS", kNStringFindExact, kRangeMissed);
	REQUIRE(patternMatches.empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindMatches/Capture")
{


	// Perform the test
	NVectorPatternMatch patternMatches;

	patternMatches =
		NStringScanner::FindMatches(kTestString, "(V.)(v.m)us", kNStringFindExact, kNRangeAll);
	REQUIRE(patternMatches.size() == 2);
	REQUIRE(patternMatches[0].thePattern == kRangeVivamus1);
	REQUIRE(patternMatches[0].theGroups == kGroupsVivamus1);
	REQUIRE(patternMatches[1].thePattern == kRangeVivamus2);
	REQUIRE(patternMatches[1].theGroups == kGroupsVivamus2);

	patternMatches =
		NStringScanner::FindMatches(kTestString, "(V.)(V.M)US", NStringFind::NoCase, kNRangeAll);
	REQUIRE(patternMatches.size() == 2);
	REQUIRE(patternMatches[0].thePattern == kRangeVivamus1);
	REQUIRE(patternMatches[0].theGroups == kGroupsVivamus1);
	REQUIRE(patternMatches[1].thePattern == kRangeVivamus2);
	REQUIRE(patternMatches[1].theGroups == kGroupsVivamus2);

	patternMatches =
		NStringScanner::FindMatches(kTestString, "(V.)(V.M)US", kNStringFindExact, kNRangeAll);
	REQUIRE(patternMatches.empty());


	patternMatches =
		NStringScanner::FindMatches(kTestString, "(V.)(v.m)us", kNStringFindExact, kRangeMissed);
	REQUIRE(patternMatches.empty());

	patternMatches =
		NStringScanner::FindMatches(kTestString, "(V.)(V.M)US", NStringFind::NoCase, kRangeMissed);
	REQUIRE(patternMatches.empty());

	patternMatches =
		NStringScanner::FindMatches(kTestString, "(V.)(V.M)US", kNStringFindExact, kRangeMissed);
	REQUIRE(patternMatches.empty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "Replace/String")
{


	// Perform the test
	NString theString;

	theString = kTestString;
	REQUIRE(NStringScanner::Replace(theString, "Lorem", "xxx"));

	theString = kTestString;
	REQUIRE(NStringScanner::Replace(theString, "LOREM", "xxx", NStringFind::NoCase));

	theString = kTestString;
	REQUIRE(NStringScanner::Replace(theString, "Lo..m", "xxx", NStringFind::Pattern));

	theString = kTestString;
	REQUIRE(NStringScanner::Replace(theString, "LO..M", "xxx", kNStringFindPatternNoCase));


	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "Lorem", "xxx", kNStringFindExact, kRangeMissed));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "LOREM", "xxx", NStringFind::NoCase, kRangeMissed));

	theString = kTestString;
	REQUIRE(
		!NStringScanner::Replace(theString, "Lo..m", "xxx", NStringFind::Pattern, kRangeMissed));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString,
									 "LO..M",
									 "xxx",
									 kNStringFindPatternNoCase,
									 kRangeMissed));


	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "LORAX", "xxx"));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "LORAX", "xxx", NStringFind::NoCase));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "Lo..x", "xxx", NStringFind::Pattern));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "LO..X", "xxx", kNStringFindPatternNoCase));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "ReplaceAll/String")
{


	// Perform the test
	NString theString;

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "dolor", "xxx") == 2);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "DOLOR", "xxx", NStringFind::NoCase) == 2);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "do..r", "xxx", NStringFind::Pattern) == 2);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "DO..R", "xxx", kNStringFindPatternNoCase) == 2);


	theString = kTestString;
	REQUIRE(
		NStringScanner::ReplaceAll(theString, "dolor", "xxx", kNStringFindExact, kRangeMissed) ==
		0);

	theString = kTestString;
	REQUIRE(
		NStringScanner::ReplaceAll(theString, "DOLOR", "xxx", NStringFind::NoCase, kRangeMissed) ==
		0);

	theString = kTestString;
	REQUIRE(
		NStringScanner::ReplaceAll(theString, "do..r", "xxx", NStringFind::Pattern, kRangeMissed) ==
		0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString,
									   "DO..R",
									   "xxx",
									   kNStringFindPatternNoCase,
									   kRangeMissed) == 0);


	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "zalor", "xxx") == 0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "ZALOR", "xxx", NStringFind::NoCase) == 0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "za..r", "xxx", NStringFind::Pattern) == 0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "ZA..R", "xxx", kNStringFindPatternNoCase) == 0);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "Replace/Range")
{


	// Perform the test
	NString theString;

	theString = "Hello World";
	NStringScanner::Replace(theString, NRange(6, 5), "AA");
	REQUIRE(theString == "Hello AA");

	theString = "Hello World";
	NStringScanner::Replace(theString, NRange(6, 5), "AABBCCDD");
	REQUIRE(theString == "Hello AABBCCDD");


	theString = "Hello World";
	NStringScanner::Replace(theString, NRange(5, 6), "");
	REQUIRE(theString == "Hello");

	theString = "Hello World";
	NStringScanner::Replace(theString, NRange(5, 60), "");
	REQUIRE(theString == "Hello");


	theString = "Hello World";
	NStringScanner::Replace(theString, NRange(0, 6), "");
	REQUIRE(theString == "World");

	theString = "Hello World";
	NStringScanner::Replace(theString, NRange(0, 60), "");
	REQUIRE(theString == "");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "ReplaceAll/Range")
{


	// Perform the test
	NString theString;

	theString = "Hello World";
	NStringScanner::ReplaceAll(theString, {NRange(1, 3), NRange(5, 2), NRange(8, 3)}, "");
	REQUIRE(theString == "Hoo");

	theString = "Hello World";
	NStringScanner::ReplaceAll(theString, {NRange(5, 2), NRange(1, 3), NRange(8, 33)}, "");
	REQUIRE(theString == "Hoo");

	theString = "Hello World";
	NStringScanner::ReplaceAll(theString, {NRange(1, 3), NRange(88, 33), NRange(5, 2)}, "");
	REQUIRE(theString == "Hoorld");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "Split")
{


	// Perform the test
	NString       kTokenComma(",");
	NVectorString theResult;

	theResult = NStringScanner::Split("a");
	REQUIRE(theResult.size() == 1);
	REQUIRE(theResult[0] == "a");

	theResult = NStringScanner::Split("a b c");
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");
	REQUIRE(theResult[2] == "c");

	theResult = NStringScanner::Split("a  b   c");
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");
	REQUIRE(theResult[2] == "c");

	theResult = NStringScanner::Split("a \tb\t \tc");
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");
	REQUIRE(theResult[2] == "c");


	theResult = NStringScanner::Split("", kTokenComma);
	REQUIRE(theResult.size() == 1);
	REQUIRE(theResult[0] == "");

	theResult = NStringScanner::Split(",", kTokenComma);
	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "");

	theResult = NStringScanner::Split(",,", kTokenComma);
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "");
	REQUIRE(theResult[2] == "");


	theResult = NStringScanner::Split("a,b,c", kTokenComma);
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "b");
	REQUIRE(theResult[2] == "c");

	theResult = NStringScanner::Split(",a,b,c", kTokenComma);
	REQUIRE(theResult.size() == 4);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "a");
	REQUIRE(theResult[2] == "b");
	REQUIRE(theResult[3] == "c");

	theResult = NStringScanner::Split(",a,b,c,", kTokenComma);
	REQUIRE(theResult.size() == 5);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "a");
	REQUIRE(theResult[2] == "b");
	REQUIRE(theResult[3] == "c");
	REQUIRE(theResult[4] == "");

	theResult = NStringScanner::Split(",a,,b,c,", kTokenComma);
	REQUIRE(theResult.size() == 6);
	REQUIRE(theResult[0] == "");
	REQUIRE(theResult[1] == "a");
	REQUIRE(theResult[2] == "");
	REQUIRE(theResult[3] == "b");
	REQUIRE(theResult[4] == "c");
	REQUIRE(theResult[5] == "");

	theResult = NStringScanner::Split("a,,,b,c", kTokenComma);
	REQUIRE(theResult.size() == 5);
	REQUIRE(theResult[0] == "a");
	REQUIRE(theResult[1] == "");
	REQUIRE(theResult[2] == "");
	REQUIRE(theResult[3] == "b");
	REQUIRE(theResult[4] == "c");
}
