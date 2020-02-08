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
static const NString kTestStringLarge                       = "Hello World Testing Testing One Two Three";





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
NANO_TEST(TString, "Hashable")
{


	// Perform the test
	for (auto theString : stringObjects)
	{
		REQUIRE(theString.GetHash() != 0);
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
NANO_TEST(TString, "Compare")
{


	// Perform the test
	NString stringA = "Abcdef";
	NString stringB = "Abcdg";

	NComparison theResult = stringA.Compare(stringB);
	REQUIRE(theResult == NComparison::LessThan);
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
NANO_TEST(TString, "Concatenate")
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
