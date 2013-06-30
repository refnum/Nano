/*	NAME:
		TUnicodeParser.cpp

	DESCRIPTION:
		NUnicodeParser tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NUnicodeParser.h"
#include "NTestFixture.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8 kTextUTF8[]										= { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E };
static const UInt8 kTextUTF16BE[]									= { 0x65, 0xE5, 0x67, 0x2C, 0x8A, 0x9E };
static const UInt8 kTextUTF32BE[]									= { 0x00, 0x00, 0x65, 0xE5, 0x00, 0x00, 0x67, 0x2C, 0x00, 0x00, 0x8A, 0x9E };

static const UTF32Char kChar0										= 0x000065E5;
static const UTF32Char kChar1										= 0x0000672C;
static const UTF32Char kChar2										= 0x00008A9E;





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NUNICODEPARSER(...)									TEST_NANO(TUnicodeParser, ##__VA_ARGS__)

FIXTURE_NANO(TUnicodeParser)
{
	NUnicodeParser		parserUTF8, parserUTF16, parserUTF32;

	SETUP
	{	NData	  dataUTF8,   dataUTF16,   dataUTF32;

		dataUTF8  = NData(NN_ARRAY_SIZE(kTextUTF8),    kTextUTF8);
		dataUTF16 = NData(NN_ARRAY_SIZE(kTextUTF16BE), kTextUTF16BE);
		dataUTF32 = NData(NN_ARRAY_SIZE(kTextUTF32BE), kTextUTF32BE);

		parserUTF8.Parse( dataUTF8,  kNStringEncodingUTF8);
		parserUTF16.Parse(dataUTF16, kNStringEncodingUTF16BE);
		parserUTF32.Parse(dataUTF32, kNStringEncodingUTF32BE);
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNICODEPARSER("Size")
{


	// Perform the test
	REQUIRE(parserUTF8.GetSize()  == 3);
	REQUIRE(parserUTF16.GetSize() == 3);
	REQUIRE(parserUTF32.GetSize() == 3);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNICODEPARSER("Ranges")
{	NRangeList		rangesUTF8, rangesUTF16, rangesUTF32;



	// Perform the test
	rangesUTF8  = *( parserUTF8.GetRanges());
	rangesUTF16 = *(parserUTF16.GetRanges());
	rangesUTF32 = *(parserUTF32.GetRanges());

	REQUIRE((rangesUTF8  == nvector(NRange(0,3), NRange(3,3), NRange(6,3))));
	REQUIRE((rangesUTF16 == nvector(NRange(0,2), NRange(2,2), NRange(4,2))));
	REQUIRE((rangesUTF32 == nvector(NRange(0,4), NRange(4,4), NRange(8,4))));

	REQUIRE(parserUTF8.GetRange(0) == NRange(0,3));
	REQUIRE(parserUTF8.GetRange(1) == NRange(3,3));
	REQUIRE(parserUTF8.GetRange(2) == NRange(6,3));

	REQUIRE(parserUTF16.GetRange(0) == NRange(0,2));
	REQUIRE(parserUTF16.GetRange(1) == NRange(2,2));
	REQUIRE(parserUTF16.GetRange(2) == NRange(4,2));

	REQUIRE(parserUTF32.GetRange(0) == NRange(0,4));
	REQUIRE(parserUTF32.GetRange(1) == NRange(4,4));
	REQUIRE(parserUTF32.GetRange(2) == NRange(8,4));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNICODEPARSER("Access")
{


	// Perform the test
	REQUIRE(parserUTF8.GetChar(0) == kChar0);
	REQUIRE(parserUTF8.GetChar(1) == kChar1);
	REQUIRE(parserUTF8.GetChar(2) == kChar2);

	REQUIRE(parserUTF16.GetChar(0) == kChar0);
	REQUIRE(parserUTF16.GetChar(1) == kChar1);
	REQUIRE(parserUTF16.GetChar(2) == kChar2);

	REQUIRE(parserUTF32.GetChar(0) == kChar0);
	REQUIRE(parserUTF32.GetChar(1) == kChar1);
	REQUIRE(parserUTF32.GetChar(2) == kChar2);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNICODEPARSER("Types")
{


	// Perform the test
	REQUIRE(!parserUTF8.IsSpace(kChar0));
	REQUIRE(!parserUTF8.IsPunct(kChar0));
	REQUIRE(!parserUTF8.IsAlpha(kChar0));
	REQUIRE(!parserUTF8.IsDigit(kChar0));

	REQUIRE(parserUTF8.IsSpace(' '));
	REQUIRE(parserUTF8.IsPunct('!'));
	REQUIRE(parserUTF8.IsAlpha('a'));
	REQUIRE(parserUTF8.IsDigit('0'));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNICODEPARSER("Case")
{


	// Perform the test
	REQUIRE(parserUTF8.GetLower('A') == (UTF32Char) 'a');
	REQUIRE(parserUTF8.GetLower('0') == (UTF32Char) '0');
	REQUIRE(parserUTF8.GetLower('?') == (UTF32Char) '?');

	REQUIRE(parserUTF8.GetUpper('b') == (UTF32Char) 'B');
	REQUIRE(parserUTF8.GetUpper('1') == (UTF32Char) '1');
	REQUIRE(parserUTF8.GetUpper('!') == (UTF32Char) '!');
}



