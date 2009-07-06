/*	NAME:
		TUnicodeParser.cpp

	DESCRIPTION:
		NUnicodeParser tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NSTLUtilities.h"
#include "NUnicodeParser.h"

#include "TUnicodeParser.h"





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
//		TUnicodeParser::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TUnicodeParser::Execute(void)
{	NRangeList			rangesUTF8, rangesUTF16, rangesUTF32;
	NUnicodeParser		parserUTF8, parserUTF16, parserUTF32;
	NData				  dataUTF8,   dataUTF16,   dataUTF32;



	// Initialise ourselves
	dataUTF8  = NData(GET_ARRAY_SIZE(kTextUTF8),    kTextUTF8);
	dataUTF16 = NData(GET_ARRAY_SIZE(kTextUTF16BE), kTextUTF16BE);
	dataUTF32 = NData(GET_ARRAY_SIZE(kTextUTF32BE), kTextUTF32BE);

	parserUTF8.Parse( dataUTF8,  kNStringEncodingUTF8);
	parserUTF16.Parse(dataUTF16, kNStringEncodingUTF16BE);
	parserUTF32.Parse(dataUTF32, kNStringEncodingUTF32BE);



	// Size
	NN_ASSERT(parserUTF8.GetSize()  == 3);
	NN_ASSERT(parserUTF16.GetSize() == 3);
	NN_ASSERT(parserUTF32.GetSize() == 3);



	// Ranges
	rangesUTF8  =  parserUTF8.GetRanges();
	rangesUTF16 = parserUTF16.GetRanges();
	rangesUTF32 = parserUTF32.GetRanges();

	NN_ASSERT(rangesUTF8  == vector(NRange(0,3), NRange(3,3), NRange(6,3)));
	NN_ASSERT(rangesUTF16 == vector(NRange(0,2), NRange(2,2), NRange(4,2)));
	NN_ASSERT(rangesUTF32 == vector(NRange(0,4), NRange(4,4), NRange(8,4)));

	NN_ASSERT(parserUTF8.GetRange(0) == NRange(0,3));
	NN_ASSERT(parserUTF8.GetRange(1) == NRange(3,3));
	NN_ASSERT(parserUTF8.GetRange(2) == NRange(6,3));

	NN_ASSERT(parserUTF16.GetRange(0) == NRange(0,2));
	NN_ASSERT(parserUTF16.GetRange(1) == NRange(2,2));
	NN_ASSERT(parserUTF16.GetRange(2) == NRange(4,2));

	NN_ASSERT(parserUTF32.GetRange(0) == NRange(0,4));
	NN_ASSERT(parserUTF32.GetRange(1) == NRange(4,4));
	NN_ASSERT(parserUTF32.GetRange(2) == NRange(8,4));



	// Character access
	NN_ASSERT(parserUTF8.GetChar(0) == kChar0);
	NN_ASSERT(parserUTF8.GetChar(1) == kChar1);
	NN_ASSERT(parserUTF8.GetChar(2) == kChar2);

	NN_ASSERT(parserUTF16.GetChar(0) == kChar0);
	NN_ASSERT(parserUTF16.GetChar(1) == kChar1);
	NN_ASSERT(parserUTF16.GetChar(2) == kChar2);

	NN_ASSERT(parserUTF32.GetChar(0) == kChar0);
	NN_ASSERT(parserUTF32.GetChar(1) == kChar1);
	NN_ASSERT(parserUTF32.GetChar(2) == kChar2);



	// Character tests
	NN_ASSERT(!parserUTF8.IsSpace(kChar0));
	NN_ASSERT(!parserUTF8.IsPunct(kChar0));
	NN_ASSERT(!parserUTF8.IsAlpha(kChar0));
	NN_ASSERT(!parserUTF8.IsDigit(kChar0));

	NN_ASSERT(parserUTF8.IsSpace(' '));
	NN_ASSERT(parserUTF8.IsPunct('!'));
	NN_ASSERT(parserUTF8.IsAlpha('a'));
	NN_ASSERT(parserUTF8.IsDigit('0'));



	// Case translation
	NN_ASSERT(parserUTF8.GetLower('A') == 'a');
	NN_ASSERT(parserUTF8.GetLower('0') == '0');
	NN_ASSERT(parserUTF8.GetLower('?') == '?');

	NN_ASSERT(parserUTF8.GetUpper('b') == 'B');
	NN_ASSERT(parserUTF8.GetUpper('1') == '1');
	NN_ASSERT(parserUTF8.GetUpper('!') == '!');
}








