/*	NAME:
		TString.cpp

	DESCRIPTION:
		NString tests.

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
#include "NData.h"
#include "NString.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const uint8_t kTestDataSmallUTF8[] =
	{0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64};

static const uint16_t kTestDataLargeUTF16[]                 = {
	0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0x0020, 0x0057, 0x006F, 0x0072, 0x006C, 0x0064,
	0x0020, 0x0054, 0x0065, 0x0073, 0x0074, 0x0069, 0x006E, 0x0067, 0x0020, 0x0054, 0x0065,
	0x0073, 0x0074, 0x0069, 0x006E, 0x0067, 0x0020, 0x004F, 0x006E, 0x0065, 0x0020, 0x0054,
	0x0077, 0x006F, 0x0020, 0x0054, 0x0068, 0x0072, 0x0065, 0x0065};

static const uint32_t kTestDataLargeUTF32[]                 = {
	0x000048, 0x000065, 0x00006C, 0x00006C, 0x00006F, 0x000020, 0x000057, 0x00006F, 0x000072,
	0x00006C, 0x000064, 0x000020, 0x000054, 0x000065, 0x000073, 0x000074, 0x000069, 0x00006E,
	0x000067, 0x000020, 0x000054, 0x000065, 0x000073, 0x000074, 0x000069, 0x00006E, 0x000067,
	0x000020, 0x00004F, 0x00006E, 0x000065, 0x000020, 0x000054, 0x000077, 0x00006F, 0x000020,
	0x000054, 0x000068, 0x000072, 0x000065, 0x000065};

static const NString kTestStringSmall                       = "Hello World";
static const NString kTestStringLarge                       = "Hello World One World Two World Three World";





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TString)
{
	std::vector<NString> stringObjects;

	SETUP
	{
		stringObjects.push_back(kTestStringSmall);
		stringObjects.push_back(kTestStringLarge);
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Default")
{


	// Perform the test
	NString theString;

	REQUIRE(theString.IsEmpty());
	REQUIRE(theString.GetHash() == 0);
	REQUIRE(theString.GetSize() == 0);
	REQUIRE(theString.GetUTF8() != nullptr);
	REQUIRE(strcmp(theString.GetUTF8(), "") == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Container")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(!theString.IsEmpty());
		REQUIRE(theString.GetSize() != 0);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Construction")
{


	// Perform the test
	NString theString;

	theString = NString(u8"UTF8 Small");
	REQUIRE(theString.GetSize() == 10);

	theString = NString(u8"UTF8 Large Large Large Large Large Large Large");
	REQUIRE(theString.GetSize() == 46);

	theString = NString(u"UTF16 Small");
	REQUIRE(theString.GetSize() == 11);

	theString = NString(u"UTF16 Large Large Large Large Large Large Large");
	REQUIRE(theString.GetSize() == 47);

	theString = NString(U"UTF32 Small");
	REQUIRE(theString.GetSize() == 11);

	theString = NString(U"UTF32 Large Large Large Large Large Large Large");
	REQUIRE(theString.GetSize() == 47);

	theString = NString(NStringEncoding::UTF8, sizeof(kTestDataSmallUTF8), kTestDataSmallUTF8);
	REQUIRE(theString.GetSize() == 11);

	theString = NString(NStringEncoding::UTF16, sizeof(kTestDataLargeUTF16), kTestDataLargeUTF16);
	REQUIRE(theString.GetSize() == 41);

	theString = NString(NStringEncoding::UTF32, sizeof(kTestDataLargeUTF32), kTestDataLargeUTF32);
	REQUIRE(theString.GetSize() == 41);

	theString = NString();
	REQUIRE(theString.GetSize() == 0);


	theString = NString("String Literal");
	REQUIRE(theString.GetSize() == 14);

	const char* localPtr = "Char Pointer";
	theString            = NString(localPtr);
	REQUIRE(theString.GetSize() == 12);

	char charArray[20];
	strncpy(charArray, "Char Array", std::size(charArray));
	theString = NString(charArray);
	REQUIRE(theString.GetSize() == 10);

	std::string stdString("Std String");
	theString = NString(stdString.c_str());
	REQUIRE(theString.GetSize() == 10);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Clear")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(!theString.IsEmpty());
		REQUIRE(theString.GetSize() != 0);

		theString.Clear();

		REQUIRE(theString.IsEmpty());
		REQUIRE(theString.GetSize() == 0);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetText")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.GetText(NStringEncoding::UTF8) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::UTF16) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::UTF32) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::ASCII) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::MacRoman) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::ISOLatin1) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::WindowsLatin1) != nullptr);
	}


	NString     theString     = kTestStringSmall;
	const char* textUTF8      = kTestStringSmall.GetUTF8();
	const void* textASCII     = theString.GetText(NStringEncoding::ASCII);
	const void* textMacRoman  = theString.GetText(NStringEncoding::MacRoman);
	const void* textISOLatin1 = theString.GetText(NStringEncoding::ISOLatin1);
	const void* textWinLatin1 = theString.GetText(NStringEncoding::WindowsLatin1);

	REQUIRE(strcmp(textUTF8, static_cast<const char*>(textASCII)) == 0);
	REQUIRE(strcmp(textUTF8, static_cast<const char*>(textMacRoman)) == 0);
	REQUIRE(strcmp(textUTF8, static_cast<const char*>(textISOLatin1)) == 0);
	REQUIRE(strcmp(textUTF8, static_cast<const char*>(textWinLatin1)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetUTF")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.GetUTF8() != nullptr);
		REQUIRE(theString.GetUTF16() != nullptr);
		REQUIRE(theString.GetUTF32() != nullptr);

		REQUIRE(theString.GetText(NStringEncoding::UTF8) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::UTF16) != nullptr);
		REQUIRE(theString.GetText(NStringEncoding::UTF32) != nullptr);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetData")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(!theString.GetData(NStringEncoding::UTF8).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::UTF16).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::UTF32).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::ASCII).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::MacRoman).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::ISOLatin1).IsEmpty());
		REQUIRE(!theString.GetData(NStringEncoding::WindowsLatin1).IsEmpty());
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "SetData")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		NString tmpString;
		NData   tmpData;

		tmpData = theString.GetData(NStringEncoding::UTF8);
		tmpString.SetData(NStringEncoding::UTF8, tmpData);
		REQUIRE(tmpString.GetData(NStringEncoding::UTF8) == tmpData);

		tmpData = theString.GetData(NStringEncoding::UTF16);
		tmpString.SetData(NStringEncoding::UTF16, tmpData);
		REQUIRE(tmpString.GetData(NStringEncoding::UTF16) == tmpData);

		tmpData = theString.GetData(NStringEncoding::UTF32);
		tmpString.SetData(NStringEncoding::UTF32, tmpData);
		REQUIRE(tmpString.GetData(NStringEncoding::UTF32) == tmpData);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetContent")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		NStringEncoding theEncoding = NStringEncoding::Unknown;
		size_t          theSize     = 0;
		const void*     theData     = theString.GetContent(&theEncoding, &theSize);

		REQUIRE(theEncoding != NStringEncoding::Unknown);
		REQUIRE(theSize != 0);
		REQUIRE(theData != nullptr);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Find")
{


	// Perform the test
	NRange theRange;

	for (auto theString : stringObjects)
	{
		theRange = theString.Find("World", kNStringNone);
		REQUIRE(theRange == NRange(6, 5));

		theRange = theString.Find("WORLD", kNStringNoCase);
		REQUIRE(theRange == NRange(6, 5));

		theRange = theString.Find("WORLD", kNStringNone);
		REQUIRE(theRange.IsEmpty());
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "FindAll")
{


	// Perform the test
	NVectorRange theRanges;

	for (auto theString : stringObjects)
	{
		theRanges = theString.FindAll("World", kNStringNone);
		if (theString == kTestStringSmall)
		{
			REQUIRE(theRanges.size() == 1);
			REQUIRE(theRanges[0] == NRange(6, 5));
		}
		else
		{
			REQUIRE(theRanges.size() == 4);
			REQUIRE(theRanges[0] == NRange(6, 5));
			REQUIRE(theRanges[1] == NRange(16, 5));
			REQUIRE(theRanges[2] == NRange(26, 5));
			REQUIRE(theRanges[3] == NRange(38, 5));
		}


		theRanges = theString.FindAll("WORLD", kNStringNoCase);
		if (theString == kTestStringSmall)
		{
			REQUIRE(theRanges.size() == 1);
			REQUIRE(theRanges[0] == NRange(6, 5));
		}
		else
		{
			REQUIRE(theRanges.size() == 4);
			REQUIRE(theRanges[0] == NRange(6, 5));
			REQUIRE(theRanges[1] == NRange(16, 5));
			REQUIRE(theRanges[2] == NRange(26, 5));
			REQUIRE(theRanges[3] == NRange(38, 5));
		}


		theRanges = theString.FindAll("WORLD", kNStringNone);
		REQUIRE(theRanges.empty());
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "FindGroups")
{


	// Perform the test
	NPatternGroup patternGroup;

	for (auto theString : stringObjects)
	{
		patternGroup = theString.FindGroup("\\sWo(\\w+)", kNStringPattern);
		if (theString == kTestStringSmall)
		{
			REQUIRE(patternGroup.theGroups.size() == 1);
			REQUIRE(patternGroup.thePattern == NRange(5, 6));
			REQUIRE(patternGroup.theGroups[0] == NRange(8, 3));
		}
		else
		{
			REQUIRE(patternGroup.theGroups.size() == 1);
			REQUIRE(patternGroup.thePattern == NRange(5, 6));
			REQUIRE(patternGroup.theGroups[0] == NRange(8, 3));
		}


		patternGroup = theString.FindGroup("\\sWO(\\w+)", kNStringPatternNoCase);
		if (theString == kTestStringSmall)
		{
			REQUIRE(patternGroup.theGroups.size() == 1);
			REQUIRE(patternGroup.thePattern == NRange(5, 6));
			REQUIRE(patternGroup.theGroups[0] == NRange(8, 3));
		}
		else
		{
			REQUIRE(patternGroup.theGroups.size() == 1);
			REQUIRE(patternGroup.thePattern == NRange(5, 6));
			REQUIRE(patternGroup.theGroups[0] == NRange(8, 3));
		}


		patternGroup = theString.FindGroup("\\sWO(\\w+)", kNStringPattern);
		REQUIRE(patternGroup.thePattern.IsEmpty());
		REQUIRE(patternGroup.theGroups.empty());
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "FindAllGroups")
{


	// Perform the test
	NVectorPatternGroup patternGroups;

	for (auto theString : stringObjects)
	{
		patternGroups = theString.FindAllGroups("\\sWo(\\w+)", kNStringPattern);
		if (theString == kTestStringSmall)
		{
			REQUIRE(patternGroups.size() == 1);
			REQUIRE(patternGroups[0].theGroups.size() == 1);
			REQUIRE(patternGroups[0].thePattern == NRange(5, 6));
			REQUIRE(patternGroups[0].theGroups[0] == NRange(8, 3));
		}
		else
		{
			REQUIRE(patternGroups.size() == 4);
			REQUIRE(patternGroups[0].theGroups.size() == 1);
			REQUIRE(patternGroups[0].thePattern == NRange(5, 6));
			REQUIRE(patternGroups[0].theGroups[0] == NRange(8, 3));

			REQUIRE(patternGroups[1].theGroups.size() == 1);
			REQUIRE(patternGroups[1].thePattern == NRange(15, 6));
			REQUIRE(patternGroups[1].theGroups[0] == NRange(18, 3));

			REQUIRE(patternGroups[2].theGroups.size() == 1);
			REQUIRE(patternGroups[2].thePattern == NRange(25, 6));
			REQUIRE(patternGroups[2].theGroups[0] == NRange(28, 3));

			REQUIRE(patternGroups[3].theGroups.size() == 1);
			REQUIRE(patternGroups[3].thePattern == NRange(37, 6));
			REQUIRE(patternGroups[3].theGroups[0] == NRange(40, 3));
		}


		patternGroups = theString.FindAllGroups("\\sWO(\\w+)", kNStringPatternNoCase);
		if (theString == kTestStringSmall)
		{
			REQUIRE(patternGroups.size() == 1);
			REQUIRE(patternGroups[0].theGroups.size() == 1);
			REQUIRE(patternGroups[0].thePattern == NRange(5, 6));
			REQUIRE(patternGroups[0].theGroups[0] == NRange(8, 3));
		}
		else
		{
			REQUIRE(patternGroups.size() == 4);
			REQUIRE(patternGroups[0].theGroups.size() == 1);
			REQUIRE(patternGroups[0].thePattern == NRange(5, 6));
			REQUIRE(patternGroups[0].theGroups[0] == NRange(8, 3));

			REQUIRE(patternGroups[1].theGroups.size() == 1);
			REQUIRE(patternGroups[1].thePattern == NRange(15, 6));
			REQUIRE(patternGroups[1].theGroups[0] == NRange(18, 3));

			REQUIRE(patternGroups[2].theGroups.size() == 1);
			REQUIRE(patternGroups[2].thePattern == NRange(25, 6));
			REQUIRE(patternGroups[2].theGroups[0] == NRange(28, 3));

			REQUIRE(patternGroups[3].theGroups.size() == 1);
			REQUIRE(patternGroups[3].thePattern == NRange(37, 6));
			REQUIRE(patternGroups[3].theGroups[0] == NRange(40, 3));
		}


		patternGroups = theString.FindAllGroups("\\sWO(\\w+)", kNStringPattern);
		REQUIRE(patternGroups.empty());
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Replace")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	REQUIRE(theString.Replace("World", "You"));
	REQUIRE(theString == "Hello You");

	theString = kTestStringSmall;
	REQUIRE(theString.Replace("WORLD", "YOU", kNStringNoCase));
	REQUIRE(theString == "Hello YOU");

	theString = kTestStringSmall;
	REQUIRE(theString.Replace("Wo..d", "You", kNStringPattern));
	REQUIRE(theString == "Hello You");

	theString = kTestStringSmall;
	REQUIRE(theString.Replace("WO..D", "YOU", kNStringPatternNoCase));
	REQUIRE(theString == "Hello YOU");


	theString = kTestStringLarge;
	REQUIRE(theString.Replace("Three", "Your"));
	REQUIRE(theString == "Hello World One World Two World Your World");

	theString = kTestStringLarge;
	REQUIRE(theString.Replace("THREE", "YOUR", kNStringNoCase));
	REQUIRE(theString == "Hello World One World Two World YOUR World");

	theString = kTestStringLarge;
	REQUIRE(theString.Replace("Thre+", "Your", kNStringPattern));
	REQUIRE(theString == "Hello World One World Two World Your World");

	theString = kTestStringLarge;
	REQUIRE(theString.Replace("THR..", "YOUR", kNStringPatternNoCase));
	REQUIRE(theString == "Hello World One World Two World YOUR World");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "ReplaceAll")
{


	// Perform the test
	NString theString;

	theString = kTestStringSmall;
	REQUIRE(theString.ReplaceAll("o", "a") == 2);
	REQUIRE(theString == "Hella Warld");

	theString = kTestStringSmall;
	REQUIRE(theString.ReplaceAll("O", "A", kNStringNoCase) == 2);
	REQUIRE(theString == "HellA WArld");

	theString = kTestStringSmall;
	REQUIRE(theString.ReplaceAll("ll.", "pp", kNStringPattern) == 1);
	REQUIRE(theString == "Hepp World");

	theString = kTestStringSmall;
	REQUIRE(theString.ReplaceAll("LL.", "PP", kNStringPatternNoCase) == 1);
	REQUIRE(theString == "HePP World");


	theString = kTestStringLarge;
	REQUIRE(theString.ReplaceAll("World", "You") == 4);
	REQUIRE(theString == "Hello You One You Two You Three You");

	theString = kTestStringLarge;
	REQUIRE(theString.ReplaceAll("WORLD", "YOU", kNStringNoCase) == 4);
	REQUIRE(theString == "Hello YOU One YOU Two YOU Three YOU");

	theString = kTestStringLarge;
	REQUIRE(theString.ReplaceAll("Wo..d", "You", kNStringPattern) == 4);
	REQUIRE(theString == "Hello You One You Two You Three You");

	theString = kTestStringLarge;
	REQUIRE(theString.ReplaceAll("WO..D", "YOU", kNStringPatternNoCase) == 4);
	REQUIRE(theString == "Hello YOU One YOU Two YOU Three YOU");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetTransformed")
{


	// Perform the test
	NString tmpString;

	for (auto theString : stringObjects)
	{
		tmpString = theString.GetTransformed(kNStringTransformNone);
		REQUIRE(tmpString == theString);

		tmpString = theString.GetTransformed(kNStringTransformToLower);
		REQUIRE(tmpString != theString);

		tmpString = theString.GetTransformed(kNStringTransformToUpper);
		REQUIRE(tmpString != theString);
	}

	tmpString = NString("one two three").GetTransformed(kNStringTransformCapitalizeWords);
	REQUIRE(tmpString == "One Two Three");

	tmpString = NString("one? TWO! three.").GetTransformed(kNStringTransformCapitalizeSentences);
	REQUIRE(tmpString == "One? Two! Three.");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetLower")
{


	// Perform the test
	NString       tmpString;
	const utf8_t* textUTF8;

	for (auto theString : stringObjects)
	{
		tmpString = theString.GetLower();
		textUTF8  = tmpString.GetUTF8();

		while (*textUTF8 != 0x00)
		{
			REQUIRE((!isalpha(*textUTF8) || islower(*textUTF8)));
			textUTF8++;
		}
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetUpper")
{


	// Perform the test
	NString       tmpString;
	const utf8_t* textUTF8;

	for (auto theString : stringObjects)
	{
		tmpString = theString.GetUpper();
		textUTF8  = tmpString.GetUTF8();

		while (*textUTF8 != 0x00)
		{
			REQUIRE((!isalpha(*textUTF8) || isupper(*textUTF8)));
			textUTF8++;
		}
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "StartsWith")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.StartsWith("Hello"));
		REQUIRE(theString.StartsWith("HELLO", kNStringNoCase));

		REQUIRE(theString.StartsWith("Hel+o", kNStringPattern));
		REQUIRE(theString.StartsWith("HEL+O", kNStringPatternNoCase));
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "EndsWith")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.EndsWith("World"));
		REQUIRE(theString.EndsWith("WORLD", kNStringNoCase));

		REQUIRE(theString.EndsWith("Wo\\w\\wd", kNStringPattern));
		REQUIRE(theString.EndsWith("WO\\w\\wD", kNStringPatternNoCase));
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Contains")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.Contains("llo Wor"));
		REQUIRE(theString.Contains("LLO WOR", kNStringNoCase));

		REQUIRE(theString.Contains("l+o\\sW.r", kNStringPattern));
		REQUIRE(theString.Contains("L+O\\sW.R", kNStringPatternNoCase));
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Compare")
{


	// Perform the test
	NString stringA = "test";
	NString stringB = "TeSt";

	REQUIRE(stringA.Compare(stringB, kNStringNone) == NComparison::GreaterThan);
	REQUIRE(stringA.Compare(stringB, kNStringNoCase) == NComparison::EqualTo);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "EqualTo")
{


	// Perform the test
	NString stringA = "test";
	NString stringB = "TeSt";

	REQUIRE(stringA.EqualTo(stringB));
	REQUIRE(!stringA.EqualTo(stringB, kNStringNone));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetPrefix")
{


	// Perform the test
	REQUIRE(kTestStringSmall.GetPrefix(0) == "");
	REQUIRE(kTestStringSmall.GetPrefix(3) == "Hel");
	REQUIRE(kTestStringSmall.GetPrefix(5) == "Hello");

	REQUIRE(kTestStringLarge.GetPrefix(0) == "");
	REQUIRE(kTestStringLarge.GetPrefix(3) == "Hel");
	REQUIRE(kTestStringLarge.GetPrefix(5) == "Hello");

	REQUIRE(kTestStringSmall.GetPrefix(kTestStringSmall.GetSize()) == kTestStringSmall);
	REQUIRE(kTestStringLarge.GetPrefix(kTestStringLarge.GetSize()) == kTestStringLarge);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetSuffix")
{


	// Perform the test
	REQUIRE(kTestStringSmall.GetSuffix(0) == "");
	REQUIRE(kTestStringSmall.GetSuffix(3) == "rld");
	REQUIRE(kTestStringSmall.GetSuffix(5) == "World");

	REQUIRE(kTestStringLarge.GetSuffix(0) == "");
	REQUIRE(kTestStringLarge.GetSuffix(3) == "rld");
	REQUIRE(kTestStringLarge.GetSuffix(5) == "World");

	REQUIRE(kTestStringSmall.GetSuffix(kTestStringSmall.GetSize()) == kTestStringSmall);
	REQUIRE(kTestStringLarge.GetSuffix(kTestStringLarge.GetSize()) == kTestStringLarge);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetSubstring")
{


	// Perform the test
	REQUIRE(kTestStringSmall.GetSubstring(kNRangeNone) == "");
	REQUIRE(kTestStringSmall.GetSubstring(kNRangeAll) == kTestStringSmall);

	REQUIRE(kTestStringSmall.GetSubstring(NRange(0, 5)) == "Hello");
	REQUIRE(kTestStringSmall.GetSubstring(NRange(2, 3)) == "llo");
	REQUIRE(kTestStringSmall.GetSubstring(NRange(6, 5)) == "World");

	REQUIRE(kTestStringLarge.GetSubstring(NRange(0, 5)) == "Hello");
	REQUIRE(kTestStringLarge.GetSubstring(NRange(22, 9)) == "Two World");
	REQUIRE(kTestStringLarge.GetSubstring(NRange(32, 11)) == "Three World");

	REQUIRE(kTestStringSmall.GetSubstring(NRange(0, kTestStringSmall.GetSize())) ==
			kTestStringSmall);
	REQUIRE(kTestStringLarge.GetSubstring(NRange(0, kTestStringLarge.GetSize())) ==
			kTestStringLarge);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "GetSubstrings")
{


	// Perform the test
	NVectorRange  theRanges;
	NVectorString theResult;


	theRanges = {{0, 5}, {2, 3}, {6, 5}};
	theResult = kTestStringSmall.GetSubstrings(theRanges);
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "Hello");
	REQUIRE(theResult[1] == "llo");
	REQUIRE(theResult[2] == "World");

	theRanges = {{0, 5}, {22, 9}, {32, 11}};
	theResult = kTestStringLarge.GetSubstrings(theRanges);
	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == "Hello");
	REQUIRE(theResult[1] == "Two World");
	REQUIRE(theResult[2] == "Three World");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "ResolveSlice")
{


	// Perform the test
	NString     theString = kTestStringLarge.GetSubstring(NRange(22, 9));
	const char* textUTF8  = theString.GetUTF8();

	REQUIRE(strlen(textUTF8) == 9);
	REQUIRE(strcmp(textUTF8, "Two World") == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Appendable")
{


	// Perform the test
	size_t oldSize = 0;

	for (auto theString : stringObjects)
	{
		oldSize = theString.GetSize();
		theString += kTestStringSmall;
		REQUIRE(theString.GetSize() == (oldSize + kTestStringSmall.GetSize()));

		oldSize = theString.GetSize();
		theString += kTestStringLarge;
		REQUIRE(theString.GetSize() == (oldSize + kTestStringLarge.GetSize()));

		theString = kTestStringSmall + kTestStringSmall;
		REQUIRE(theString.GetSize() == (kTestStringSmall.GetSize() * 2));

		theString = kTestStringLarge + kTestStringLarge;
		REQUIRE(theString.GetSize() == (kTestStringLarge.GetSize() * 2));
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "CompareEqual")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		if (theString == kTestStringSmall)
		{
			REQUIRE(theString != kTestStringLarge);
		}
		else if (theString == kTestStringLarge)
		{
			REQUIRE(theString != kTestStringSmall);
		}
	}

	REQUIRE(NString("1234") != NString("abcd"));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "CompareOrder")
{


	// Perform the test
	REQUIRE(kTestStringSmall <= kTestStringSmall);
	REQUIRE(kTestStringSmall <= kTestStringLarge);
	REQUIRE(kTestStringSmall < kTestStringLarge);

	REQUIRE(kTestStringLarge >= kTestStringLarge);
	REQUIRE(kTestStringLarge >= kTestStringSmall);
	REQUIRE(kTestStringLarge > kTestStringSmall);

	REQUIRE(NString("A") < NString("B"));
	REQUIRE(NString("B") > NString("A"));

	REQUIRE(NString("A") < NString("a"));
	REQUIRE(NString("a") > NString("A"));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TString, "Hashable")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.GetHash() != 0);
	}


	NString stringUTF8("test");
	NString stringUTF16(u"test");
	NString stringUTF32(U"test");

	REQUIRE(stringUTF8.GetHash() == stringUTF16.GetHash());
	REQUIRE(stringUTF8.GetHash() == stringUTF32.GetHash());
}
