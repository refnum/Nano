/*	NAME:
		NUnicodeParser.h

	DESCRIPTION:
		Unicode parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUNICODEPARSER_HDR
#define NUNICODEPARSER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"
#include "NRange.h"
#include "NStringEncoder.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt32 kASCIILimit											= 0x7F;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUnicodeParser {
public:
										 NUnicodeParser(void);
	virtual								~NUnicodeParser(void);


	// Set the value
	void								SetValue(const NData &theData, NStringEncoding theEncoding);


	// Get the size
	//
	// Returns the number of characters in the string.
	NIndex								GetSize(void) const;


	// Get the range of a character
	//
	// Returns the bytes used by the character at the specified index.
	NRange								GetRange(NIndex theIndex) const;


	// Get a character
	//
	// Returns the character at the specified index.
	UTF32Char							GetChar(NIndex theIndex) const;


	// Test a character
	bool								IsSpace(UTF32Char theChar) const;
	bool								IsPunct(UTF32Char theChar) const;
	bool								IsAlpha(UTF32Char theChar) const;
	bool								IsDigit(UTF32Char theChar) const;


	// Convert case
	UTF32Char							GetLower(UTF32Char theChar) const;
	UTF32Char							GetUpper(UTF32Char theChar) const;


private:
	NRangeList							GetCodePointsUTF8( void) const;
	NRangeList							GetCodePointsUTF16(void) const;
	NRangeList							GetCodePointsUTF32(void) const;


private:
	NData								mData;
	NStringEncoding						mEncoding;
	NRangeList							mCodePoints;
};





#endif // NUNICODEPARSER_HDR


