/*	NAME:
		TStringScanner.cpp

	DESCRIPTION:
		NStringScanner tests.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TStringScanner){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "Find/Literal")
{


	// Perform the test
	NRange theRange;


	theRange = NStringScanner::Find(kTestString, "Vivamus", kNStringNone, kNRangeAll);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kNStringNoCase, kNRangeAll);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kNStringNone, kNRangeAll);
	REQUIRE(theRange.IsEmpty());


	theRange = NStringScanner::Find(kTestString, "Vivamus", kNStringNone, kRangeMissed);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kNStringNoCase, kRangeMissed);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kNStringNone, kRangeMissed);
	REQUIRE(theRange.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "Find/Pattern")
{


	// Perform the test
	NRange theRange;


	theRange = NStringScanner::Find(kTestString, "Viv\\w+\\b", kNStringPattern, kNRangeAll);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", kNStringPatternNoCase, kNRangeAll);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", kNStringPattern, kNRangeAll);
	REQUIRE(theRange.IsEmpty());


	theRange = NStringScanner::Find(kTestString, "Viv\\w+\\b", kNStringPattern, kRangeMissed);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", kNStringPatternNoCase, kRangeMissed);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", kNStringPattern, kRangeMissed);
	REQUIRE(theRange.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindAll/Literal")
{


	// Perform the test
	NVectorRange theRanges;



	theRanges = NStringScanner::FindAll(kTestString, "Vivamus", kNStringNone, kNRangeAll);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kNStringNoCase, kNRangeAll);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kNStringNone, kNRangeAll);
	REQUIRE(theRanges.empty());


	theRanges = NStringScanner::FindAll(kTestString, "Vivamus", kNStringNone, kRangeMissed);
	REQUIRE(theRanges.empty());

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kNStringNoCase, kRangeMissed);
	REQUIRE(theRanges.empty());

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kNStringNone, kRangeMissed);
	REQUIRE(theRanges.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindAll/Pattern")
{


	// Perform the test
	NVectorRange theRanges;



	theRanges = NStringScanner::FindAll(kTestString, "Viv\\w+\\b", kNStringPattern, kNRangeAll);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges =
		NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kNStringPatternNoCase, kNRangeAll);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges = NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kNStringPattern, kNRangeAll);
	REQUIRE(theRanges.empty());


	theRanges = NStringScanner::FindAll(kTestString, "Viv\\w+\\b", kNStringPattern, kRangeMissed);
	REQUIRE(theRanges.empty());

	theRanges =
		NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kNStringPatternNoCase, kRangeMissed);
	REQUIRE(theRanges.empty());

	theRanges = NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kNStringPattern, kRangeMissed);
	REQUIRE(theRanges.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindGroup/Literal")
{


	// Perform the test
	NPatternGroup patternGroup;

	patternGroup = NStringScanner::FindGroup(kTestString, "Vivamus", kNStringNone, kNRangeAll);
	REQUIRE(patternGroup.thePattern == kRangeVivamus1);
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup = NStringScanner::FindGroup(kTestString, "VIVAMUS", kNStringNoCase, kNRangeAll);
	REQUIRE(patternGroup.thePattern == kRangeVivamus1);
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup = NStringScanner::FindGroup(kTestString, "VIVAMUS", kNStringNone, kNRangeAll);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());


	patternGroup = NStringScanner::FindGroup(kTestString, "Vivamus", kNStringNone, kRangeMissed);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup = NStringScanner::FindGroup(kTestString, "VIVAMUS", kNStringNoCase, kRangeMissed);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup = NStringScanner::FindGroup(kTestString, "VIVAMUS", kNStringNone, kRangeMissed);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindGroup/Pattern")
{


	// Perform the test
	NPatternGroup patternGroup;

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(v.m)us", kNStringPattern, kNRangeAll);
	REQUIRE(patternGroup.thePattern == kRangeVivamus1);
	REQUIRE(patternGroup.theGroups == kGroupsVivamus1);

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(V.M)US", kNStringPatternNoCase, kNRangeAll);
	REQUIRE(patternGroup.thePattern == kRangeVivamus1);
	REQUIRE(patternGroup.theGroups == kGroupsVivamus1);

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(V.M)US", kNStringPattern, kNRangeAll);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());


	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(v.m)us", kNStringPattern, kRangeMissed);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(V.M)US", kNStringPatternNoCase, kRangeMissed);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(V.M)US", kNStringPattern, kRangeMissed);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindAllGroups/Literal")
{


	// Perform the test
	NVectorPatternGroup patternGroups;

	patternGroups = NStringScanner::FindAllGroups(kTestString, "Vivamus", kNStringNone, kNRangeAll);
	REQUIRE(patternGroups.size() == 2);
	REQUIRE(patternGroups[0].thePattern == kRangeVivamus1);
	REQUIRE(patternGroups[0].theGroups.empty());
	REQUIRE(patternGroups[1].thePattern == kRangeVivamus2);
	REQUIRE(patternGroups[1].theGroups.empty());

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "VIVAMUS", kNStringNoCase, kNRangeAll);
	REQUIRE(patternGroups.size() == 2);
	REQUIRE(patternGroups[0].thePattern == kRangeVivamus1);
	REQUIRE(patternGroups[0].theGroups.empty());
	REQUIRE(patternGroups[1].thePattern == kRangeVivamus2);
	REQUIRE(patternGroups[1].theGroups.empty());

	patternGroups = NStringScanner::FindAllGroups(kTestString, "VIVAMUS", kNStringNone, kNRangeAll);
	REQUIRE(patternGroups.empty());


	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "Vivamus", kNStringNone, kRangeMissed);
	REQUIRE(patternGroups.empty());

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "VIVAMUS", kNStringNoCase, kRangeMissed);
	REQUIRE(patternGroups.empty());

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "VIVAMUS", kNStringNone, kRangeMissed);
	REQUIRE(patternGroups.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "FindAllGroups/Pattern")
{


	// Perform the test
	NVectorPatternGroup patternGroups;

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "(V.)(v.m)us", kNStringPattern, kNRangeAll);
	REQUIRE(patternGroups.size() == 2);
	REQUIRE(patternGroups[0].thePattern == kRangeVivamus1);
	REQUIRE(patternGroups[0].theGroups == kGroupsVivamus1);
	REQUIRE(patternGroups[1].thePattern == kRangeVivamus2);
	REQUIRE(patternGroups[1].theGroups == kGroupsVivamus2);

	patternGroups = NStringScanner::FindAllGroups(kTestString,
												  "(V.)(V.M)US",
												  kNStringPatternNoCase,
												  kNRangeAll);
	REQUIRE(patternGroups.size() == 2);
	REQUIRE(patternGroups[0].thePattern == kRangeVivamus1);
	REQUIRE(patternGroups[0].theGroups == kGroupsVivamus1);
	REQUIRE(patternGroups[1].thePattern == kRangeVivamus2);
	REQUIRE(patternGroups[1].theGroups == kGroupsVivamus2);

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "(V.)(V.M)US", kNStringPattern, kNRangeAll);
	REQUIRE(patternGroups.empty());


	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "(V.)(v.m)us", kNStringPattern, kRangeMissed);
	REQUIRE(patternGroups.empty());

	patternGroups = NStringScanner::FindAllGroups(kTestString,
												  "(V.)(V.M)US",
												  kNStringPatternNoCase,
												  kRangeMissed);
	REQUIRE(patternGroups.empty());

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "(V.)(V.M)US", kNStringPattern, kRangeMissed);
	REQUIRE(patternGroups.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "Replace")
{


	// Perform the test
	NString theString;

	theString = kTestString;
	REQUIRE(NStringScanner::Replace(theString, "Lorem", "xxx"));

	theString = kTestString;
	REQUIRE(NStringScanner::Replace(theString, "LOREM", "xxx", kNStringNoCase));

	theString = kTestString;
	REQUIRE(NStringScanner::Replace(theString, "Lo..m", "xxx", kNStringPattern));

	theString = kTestString;
	REQUIRE(NStringScanner::Replace(theString, "LO..M", "xxx", kNStringPatternNoCase));


	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "Lorem", "xxx", kNStringNone, kRangeMissed));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "LOREM", "xxx", kNStringNoCase, kRangeMissed));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "Lo..m", "xxx", kNStringPattern, kRangeMissed));

	theString = kTestString;
	REQUIRE(
		!NStringScanner::Replace(theString, "LO..M", "xxx", kNStringPatternNoCase, kRangeMissed));


	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "LORAX", "xxx"));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "LORAX", "xxx", kNStringNoCase));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "Lo..x", "xxx", kNStringPattern));

	theString = kTestString;
	REQUIRE(!NStringScanner::Replace(theString, "LO..X", "xxx", kNStringPatternNoCase));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TStringScanner, "ReplaceAll")
{


	// Perform the test
	NString theString;

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "dolor", "xxx") == 2);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "DOLOR", "xxx", kNStringNoCase) == 2);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "do..r", "xxx", kNStringPattern) == 2);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "DO..R", "xxx", kNStringPatternNoCase) == 2);


	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "dolor", "xxx", kNStringNone, kRangeMissed) == 0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "DOLOR", "xxx", kNStringNoCase, kRangeMissed) ==
			0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "do..r", "xxx", kNStringPattern, kRangeMissed) ==
			0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString,
									   "DO..R",
									   "xxx",
									   kNStringPatternNoCase,
									   kRangeMissed) == 0);


	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "zalor", "xxx") == 0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "ZALOR", "xxx", kNStringNoCase) == 0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "za..r", "xxx", kNStringPattern) == 0);

	theString = kTestString;
	REQUIRE(NStringScanner::ReplaceAll(theString, "ZA..R", "xxx", kNStringPatternNoCase) == 0);
}
