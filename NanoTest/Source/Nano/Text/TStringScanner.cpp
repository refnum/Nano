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

static const NRange kRangeMissed                            = NRange(60, 100);

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
NANO_FIXTURE(NStringScanner){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringScanner, "Find/Literal")
{


	// Perform the test
	NRange theRange;


	theRange = NStringScanner::Find(kTestString, "Vivamus", kNRangeAll, kNStringNone);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kNRangeAll, kNStringNoCase);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kNRangeAll, kNStringNone);
	REQUIRE(theRange.IsEmpty());


	theRange = NStringScanner::Find(kTestString, "Vivamus", kRangeMissed, kNStringNone);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kRangeMissed, kNStringNoCase);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIVAMUS", kRangeMissed, kNStringNone);
	REQUIRE(theRange.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringScanner, "Find/Pattern")
{


	// Perform the test
	NRange theRange;


	theRange = NStringScanner::Find(kTestString, "Viv\\w+\\b", kNRangeAll, kNStringPattern);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", kNRangeAll, kNStringPatternNoCase);
	REQUIRE(theRange == kRangeVivamus1);

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", kNRangeAll, kNStringPattern);
	REQUIRE(theRange.IsEmpty());


	theRange = NStringScanner::Find(kTestString, "Viv\\w+\\b", kRangeMissed, kNStringPattern);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", kRangeMissed, kNStringPatternNoCase);
	REQUIRE(theRange.IsEmpty());

	theRange = NStringScanner::Find(kTestString, "VIV\\w+\\b", kRangeMissed, kNStringPattern);
	REQUIRE(theRange.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringScanner, "FindAll/Literal")
{


	// Perform the test
	NVectorRange theRanges;



	theRanges = NStringScanner::FindAll(kTestString, "Vivamus", kNRangeAll, kNStringNone);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kNRangeAll, kNStringNoCase);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kNRangeAll, kNStringNone);
	REQUIRE(theRanges.empty());


	theRanges = NStringScanner::FindAll(kTestString, "Vivamus", kRangeMissed, kNStringNone);
	REQUIRE(theRanges.empty());

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kRangeMissed, kNStringNoCase);
	REQUIRE(theRanges.empty());

	theRanges = NStringScanner::FindAll(kTestString, "VIVAMUS", kRangeMissed, kNStringNone);
	REQUIRE(theRanges.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringScanner, "FindAll/Pattern")
{


	// Perform the test
	NVectorRange theRanges;



	theRanges = NStringScanner::FindAll(kTestString, "Viv\\w+\\b", kNRangeAll, kNStringPattern);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges =
		NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kNRangeAll, kNStringPatternNoCase);
	REQUIRE(theRanges == kRangesVivamus1_2);

	theRanges = NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kNRangeAll, kNStringPattern);
	REQUIRE(theRanges.empty());


	theRanges = NStringScanner::FindAll(kTestString, "Viv\\w+\\b", kRangeMissed, kNStringPattern);
	REQUIRE(theRanges.empty());

	theRanges =
		NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kRangeMissed, kNStringPatternNoCase);
	REQUIRE(theRanges.empty());

	theRanges = NStringScanner::FindAll(kTestString, "VIV\\w+\\b", kRangeMissed, kNStringPattern);
	REQUIRE(theRanges.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringScanner, "FindGroup/Literal")
{


	// Perform the test
	NPatternGroup patternGroup;

	patternGroup = NStringScanner::FindGroup(kTestString, "Vivamus", kNRangeAll, kNStringNone);
	REQUIRE(patternGroup.thePattern == kRangeVivamus1);
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup = NStringScanner::FindGroup(kTestString, "VIVAMUS", kNRangeAll, kNStringNoCase);
	REQUIRE(patternGroup.thePattern == kRangeVivamus1);
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup = NStringScanner::FindGroup(kTestString, "VIVAMUS", kNRangeAll, kNStringNone);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());


	patternGroup = NStringScanner::FindGroup(kTestString, "Vivamus", kRangeMissed, kNStringNone);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup = NStringScanner::FindGroup(kTestString, "VIVAMUS", kRangeMissed, kNStringNoCase);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup = NStringScanner::FindGroup(kTestString, "VIVAMUS", kRangeMissed, kNStringNone);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringScanner, "FindGroup/Pattern")
{


	// Perform the test
	NPatternGroup patternGroup;

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(v.m)us", kNRangeAll, kNStringPattern);
	REQUIRE(patternGroup.thePattern == kRangeVivamus1);
	REQUIRE(patternGroup.theGroups == kGroupsVivamus1);

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(V.M)US", kNRangeAll, kNStringPatternNoCase);
	REQUIRE(patternGroup.thePattern == kRangeVivamus1);
	REQUIRE(patternGroup.theGroups == kGroupsVivamus1);

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(V.M)US", kNRangeAll, kNStringPattern);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());


	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(v.m)us", kRangeMissed, kNStringPattern);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(V.M)US", kRangeMissed, kNStringPatternNoCase);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());

	patternGroup =
		NStringScanner::FindGroup(kTestString, "(V.)(V.M)US", kRangeMissed, kNStringPattern);
	REQUIRE(patternGroup.thePattern.IsEmpty());
	REQUIRE(patternGroup.theGroups.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringScanner, "FindAllGroups/Literal")
{


	// Perform the test
	NVectorPatternGroup patternGroups;

	patternGroups = NStringScanner::FindAllGroups(kTestString, "Vivamus", kNRangeAll, kNStringNone);
	REQUIRE(patternGroups.size() == 2);
	REQUIRE(patternGroups[0].thePattern == kRangeVivamus1);
	REQUIRE(patternGroups[0].theGroups.empty());
	REQUIRE(patternGroups[1].thePattern == kRangeVivamus2);
	REQUIRE(patternGroups[1].theGroups.empty());

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "VIVAMUS", kNRangeAll, kNStringNoCase);
	REQUIRE(patternGroups.size() == 2);
	REQUIRE(patternGroups[0].thePattern == kRangeVivamus1);
	REQUIRE(patternGroups[0].theGroups.empty());
	REQUIRE(patternGroups[1].thePattern == kRangeVivamus2);
	REQUIRE(patternGroups[1].theGroups.empty());

	patternGroups = NStringScanner::FindAllGroups(kTestString, "VIVAMUS", kNRangeAll, kNStringNone);
	REQUIRE(patternGroups.empty());


	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "Vivamus", kRangeMissed, kNStringNone);
	REQUIRE(patternGroups.empty());

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "VIVAMUS", kRangeMissed, kNStringNoCase);
	REQUIRE(patternGroups.empty());

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "VIVAMUS", kRangeMissed, kNStringNone);
	REQUIRE(patternGroups.empty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NStringScanner, "FindAllGroups/Pattern")
{


	// Perform the test
	NVectorPatternGroup patternGroups;

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "(V.)(v.m)us", kNRangeAll, kNStringPattern);
	REQUIRE(patternGroups.size() == 2);
	REQUIRE(patternGroups[0].thePattern == kRangeVivamus1);
	REQUIRE(patternGroups[0].theGroups == kGroupsVivamus1);
	REQUIRE(patternGroups[1].thePattern == kRangeVivamus2);
	REQUIRE(patternGroups[1].theGroups == kGroupsVivamus2);

	patternGroups = NStringScanner::FindAllGroups(kTestString,
												  "(V.)(V.M)US",
												  kNRangeAll,
												  kNStringPatternNoCase);
	REQUIRE(patternGroups.size() == 2);
	REQUIRE(patternGroups[0].thePattern == kRangeVivamus1);
	REQUIRE(patternGroups[0].theGroups == kGroupsVivamus1);
	REQUIRE(patternGroups[1].thePattern == kRangeVivamus2);
	REQUIRE(patternGroups[1].theGroups == kGroupsVivamus2);

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "(V.)(V.M)US", kNRangeAll, kNStringPattern);
	REQUIRE(patternGroups.empty());


	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "(V.)(v.m)us", kRangeMissed, kNStringPattern);
	REQUIRE(patternGroups.empty());

	patternGroups = NStringScanner::FindAllGroups(kTestString,
												  "(V.)(V.M)US",
												  kRangeMissed,
												  kNStringPatternNoCase);
	REQUIRE(patternGroups.empty());

	patternGroups =
		NStringScanner::FindAllGroups(kTestString, "(V.)(V.M)US", kRangeMissed, kNStringPattern);
	REQUIRE(patternGroups.empty());
}
