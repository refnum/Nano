/*	NAME:
		NUnicodeParser.h

	DESCRIPTION:
		Unicode parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
//		Class declaration
//----------------------------------------------------------------------------
class NUnicodeParser {
public:
										NUnicodeParser(void);
	virtual							   ~NUnicodeParser(void);


	// Parse some data
	void								Parse(const NData &theData, NStringEncoding theEncoding);


	// Get the size
	//
	// Returns the number of code points in the parsed data.
	NIndex								GetSize(void) const;


	// Get the character ranges
	//
	// Returns the range of each code point in the parsed data.
	const NRangeList				   *GetRanges(void) const;


	// Get the range of a character
	//
	// Returns the range of a code point, or range of code points, within the parsed data.
	NRange								GetRange(      NIndex  theIndex) const;
	NRange								GetRange(const NRange &theRange) const;


	// Get a character
	//
	// Returns a character within the parsed data.
	utf32_t								GetChar(NIndex theIndex, bool toLower=false) const;


	// Test a character
	bool								IsSpace(utf32_t theChar) const;
	bool								IsPunct(utf32_t theChar) const;
	bool								IsAlpha(utf32_t theChar) const;
	bool								IsDigit(utf32_t theChar) const;


	// Convert case
	utf32_t								GetLower(utf32_t theChar) const;
	utf32_t								GetUpper(utf32_t theChar) const;


	// Get the BOM
	//
	// Returns kNStringEncodingInvalid if the BOM is not present.
	NStringEncoding						GetBOM(const NData &theData, NRange &theRange) const;


	// Add/remove a BOM
	void								AddBOM(   NData &theData, NStringEncoding theEncoding) const;
	void								RemoveBOM(NData &theData, NStringEncoding theEncoding) const;


	// Get the endian-native form of a UTF character
	utf16_t								GetNativeUTF16(utf16_t theChar, NStringEncoding theEncoding) const;
	utf32_t								GetNativeUTF32(utf32_t theChar, NStringEncoding theEncoding) const;


private:
	NRangeList							GetCodePointsUTF8( const NRange &theBOM) const;
	NRangeList							GetCodePointsUTF16(const NRange &theBOM) const;
	NRangeList							GetCodePointsUTF32(const NRange &theBOM) const;

	void								AddBOMToUTF8( NData &theData)                          const;
	void								AddBOMToUTF16(NData &theData, NEndianFormat theFormat) const;
	void								AddBOMToUTF32(NData &theData, NEndianFormat theFormat) const;


private:
	NData								mData;
	NStringEncoding						mEncoding;
	NRangeList							mCodePoints;
};





#endif // NUNICODEPARSER_HDR


