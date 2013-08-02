/*	NAME:
		NUnicodeParser.cpp

	DESCRIPTION:
		Unicode parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringEncoder.h"
#include "NUnicodeParser.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// BOM
static const uint8_t kUTF8BOM[]										= { 0xEF, 0xBB, 0xBF };
static const uint8_t kUTF16BOMBE[]									= { 0xFE, 0xFF };
static const uint8_t kUTF16BOMLE[]									= { 0xFF, 0xFE };
static const uint8_t kUTF32BOMBE[]									= { 0x00, 0x00, 0xFE, 0xFF };
static const uint8_t kUTF32BOMLE[]									= { 0xFF, 0xFE, 0x00, 0x00 };


// UTF16
static const utf16_t kUTF16SurrogateHighStart						= 0xD800;
static const utf16_t kUTF16SurrogateHighEnd							= 0xDBFF;
static const utf16_t kUTF16SurrogateLowStart						= 0xDC00;
static const utf16_t kUTF16SurrogateLowEnd							= 0xDFFF;


// UTF8
static const NIndex kUTF8TrailingBytes[256] =					{	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
																	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
																	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
																	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
																	3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5 };





//============================================================================
//		NUnicodeParser::NUnicodeParser : Constructor.
//----------------------------------------------------------------------------
NUnicodeParser::NUnicodeParser(void)
{


	// Initialise ourselves
	mEncoding = kNStringEncodingInvalid;
}





//============================================================================
//		NUnicodeParser::~NUnicodeParser : Destructor.
//----------------------------------------------------------------------------
NUnicodeParser::~NUnicodeParser(void)
{
}





//============================================================================
//		NUnicodeParser::Parse : Parse some data.
//----------------------------------------------------------------------------
void NUnicodeParser::Parse(const NData &theData, NStringEncoding theEncoding)
{	NRange		theRange;



	// Set the value
	mData     = theData;
	mEncoding = theEncoding;

	(void) GetBOM(mData, theRange);



	// Identify the code points
	switch (mEncoding) {
		case kNStringEncodingUTF8:
			mCodePoints = GetCodePointsUTF8(theRange);
			break;

		case kNStringEncodingUTF16:
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF16LE:
			mCodePoints = GetCodePointsUTF16(theRange);
			break;

		case kNStringEncodingUTF32:
		case kNStringEncodingUTF32BE:
		case kNStringEncodingUTF32LE:
			mCodePoints = GetCodePointsUTF32(theRange);
			break;
		
		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			break;
		}
}





//============================================================================
//		NUnicodeParser::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NUnicodeParser::GetSize(void) const
{


	// Get the size
	return((NIndex) mCodePoints.size());
}





//============================================================================
//		NUnicodeParser::GetRanges ; Get the character ranges.
//----------------------------------------------------------------------------
const NRangeList *NUnicodeParser::GetRanges(void) const
{


	// Get the ranges
	return(&mCodePoints);
}





//============================================================================
//		NUnicodeParser::GetRange : Get the range of a character.
//----------------------------------------------------------------------------
NRange NUnicodeParser::GetRange(NIndex theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < GetSize());
	
	
	
	// Get the range
	return(mCodePoints[theIndex]);
}





//============================================================================
//		NUnicodeParser::GetRange : Get the range of a character range.
//----------------------------------------------------------------------------
NRange NUnicodeParser::GetRange(const NRange &theRange) const
{	NIndex		offsetFirst, offsetLast;
	NRange		theResult;



	// Get the range
	offsetFirst = GetRange(theRange.GetFirst()).GetFirst();
	offsetLast  = GetRange(theRange.GetLast() ).GetLast();

	theResult.SetLocation(offsetFirst);
	theResult.SetSize(    offsetLast - offsetFirst + 1);

	return(theResult);
}





//============================================================================
//		NUnicodeParser::GetChar : Get a character.
//----------------------------------------------------------------------------
utf32_t NUnicodeParser::GetChar(NIndex theIndex, bool toLower) const
{	NStringEncoder		theEncoder;
	NRange				theRange;
	const uint8_t		*srcPtr;
	utf32_t				theChar;



	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < GetSize());



	// Get the state we need
	theRange = GetRange(theIndex);
	srcPtr   = mData.GetData(theRange.GetLocation());



	// Convert the character
	theChar = theEncoder.ConvertToUTF32(mEncoding, theRange.GetSize(), srcPtr);
	NN_ASSERT(theChar != 0);

	if (toLower)
		theChar = GetLower(theChar);

	return(theChar);
}





//============================================================================
//		NUnicodeParser::IsSpace : Is this a whitespace character?
//----------------------------------------------------------------------------
bool NUnicodeParser::IsSpace(utf32_t theChar) const
{	bool	theResult;



	// Check the character
	//
	// This test needs to be more sophisticated.
	theResult = (theChar <= kASCIILimit && isspace(theChar));
	
	return(theResult);
}





//============================================================================
//		NUnicodeParser::IsPunct : Is this a punctuation character?
//----------------------------------------------------------------------------
bool NUnicodeParser::IsPunct(utf32_t theChar) const
{	bool	theResult;



	// Check the character
	//
	// This test needs to be more sophisticated.
	theResult = (theChar <= kASCIILimit && ispunct(theChar));
	
	return(theResult);
}





//============================================================================
//		NUnicodeParser::IsAlpha : Is this an alphabetic character?
//----------------------------------------------------------------------------
bool NUnicodeParser::IsAlpha(utf32_t theChar) const
{	bool	theResult;



	// Check the character
	//
	// This test needs to be more sophisticated.
	theResult = (theChar <= kASCIILimit && isalpha(theChar));
	
	return(theResult);
}





//============================================================================
//		NUnicodeParser::IsDigit : Is this a numeric character?
//----------------------------------------------------------------------------
bool NUnicodeParser::IsDigit(utf32_t theChar) const
{	bool	theResult;



	// Check the character
	//
	// This test needs to be more sophisticated.
	theResult = (theChar <= kASCIILimit && isdigit(theChar));
	
	return(theResult);
}





//============================================================================
//		NUnicodeParser::GetLower : Get the lower-case version of a character.
//----------------------------------------------------------------------------
utf32_t NUnicodeParser::GetLower(utf32_t theChar) const
{


	// Convert the character
	//
	// This test needs to be more sophisticated.
	if (theChar <= kASCIILimit)
		theChar = tolower(theChar);
	
	return(theChar);
}





//============================================================================
//		NUnicodeParser::GetUpper : Get the upper-case version of a character.
//----------------------------------------------------------------------------
utf32_t NUnicodeParser::GetUpper(utf32_t theChar) const
{


	// Convert the character
	//
	// This test needs to be more sophisticated.
	if (theChar <= kASCIILimit)
		theChar = toupper(theChar);
	
	return(theChar);
}





//============================================================================
//		NUnicodeParser::GetBOM : Get the BOM.
//----------------------------------------------------------------------------
NStringEncoding NUnicodeParser::GetBOM(const NData &theData, NRange &theRange) const
{	NStringEncoding		theEncoding;
	const uint8_t		*dataPtr;
	NIndex				dataSize;



	// Get the state we need
	theEncoding = kNStringEncodingInvalid;
	dataSize    = theData.GetSize();
	dataPtr     = theData.GetData();
	theRange    = kNRangeNone;



	// Identify the BOM
	#define MATCH_BOM(_bom, _encoding)										\
		do																	\
			{																\
			if (theRange.IsEmpty() && dataSize >= NN_ARRAY_SIZE(_bom))		\
				{															\
				if (memcmp(dataPtr, _bom, NN_ARRAY_SIZE(_bom)) == 0)		\
					{														\
					theEncoding = _encoding;								\
					theRange    = NRange(0, NN_ARRAY_SIZE(_bom));			\
					}														\
				}															\
			}																\
		while (0)

	MATCH_BOM(kUTF8BOM,    kNStringEncodingUTF8);
	MATCH_BOM(kUTF16BOMBE, kNStringEncodingUTF16BE);
	MATCH_BOM(kUTF16BOMLE, kNStringEncodingUTF16LE);
	MATCH_BOM(kUTF32BOMBE, kNStringEncodingUTF32BE);
	MATCH_BOM(kUTF32BOMLE, kNStringEncodingUTF32LE);
	
	#undef MATCH_BOM
	
	return(theEncoding);
}





//============================================================================
//		NUnicodeParser::AddBOM : Add a BOM prefix.
//----------------------------------------------------------------------------
void NUnicodeParser::AddBOM(NData &theData, NStringEncoding theEncoding) const
{	NRange		theRange;



	// Validate our parameters
	NN_ASSERT(GetBOM(theData, theRange) == kNStringEncodingInvalid);
	
	(void) theRange;



	// Add the BOM
	switch (theEncoding) {
		case kNStringEncodingUTF8:
			AddBOMToUTF8(theData);
			break;

		case kNStringEncodingUTF16:
			AddBOMToUTF16(theData, kNEndianNative);
			break;

		case kNStringEncodingUTF16BE:
			AddBOMToUTF16(theData, kNEndianBig);
			break;

		case kNStringEncodingUTF16LE:
			AddBOMToUTF16(theData, kNEndianLittle);
			break;

		case kNStringEncodingUTF32:
			AddBOMToUTF32(theData, kNEndianNative);
			break;

		case kNStringEncodingUTF32BE:
			AddBOMToUTF32(theData, kNEndianBig);
			break;

		case kNStringEncodingUTF32LE:
			AddBOMToUTF16(theData, kNEndianLittle);
			break;

		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			break;
		}
}





//============================================================================
//		NUnicodeParser::RemoveBOM : Remove a BOM prefix.
//----------------------------------------------------------------------------
void NUnicodeParser::RemoveBOM(NData &theData, NStringEncoding theEncoding) const
{	NStringEncoding		bomEncoding;
	NRange				theBOM;



	// Silence compiler warnings
	NN_UNUSED(theEncoding);
	


	// Get the state we need
	bomEncoding = GetBOM(theData, theBOM);
	if (!theBOM.IsEmpty())
		theData.RemoveData(theBOM);



	// Validate the encoding
	//
	// Endian-specific BOMs should match the format we expected.
	switch (bomEncoding) {
		case kNStringEncodingInvalid:
			// No BOM
			break;

		case kNStringEncodingUTF8:
			NN_ASSERT(theEncoding == kNStringEncodingUTF8);
			break;

		case kNStringEncodingUTF16BE:
			NN_ASSERT(theEncoding == kNStringEncodingUTF16 || theEncoding == kNStringEncodingUTF16BE);
			break;

		case kNStringEncodingUTF16LE:
			NN_ASSERT(theEncoding == kNStringEncodingUTF16 || theEncoding == kNStringEncodingUTF16LE);
			break;

		case kNStringEncodingUTF32BE:
			NN_ASSERT(theEncoding == kNStringEncodingUTF32 || theEncoding == kNStringEncodingUTF32BE);
			break;

		case kNStringEncodingUTF32LE:
			NN_ASSERT(theEncoding == kNStringEncodingUTF32 || theEncoding == kNStringEncodingUTF32LE);
			break;

		default:
			NN_LOG("Unexpected encoding: %d", theEncoding);
			break;
		}
}





//============================================================================
//		NUnicodeParser::GetGenericEncoding : Get the generic form of a UTF encoding.
//----------------------------------------------------------------------------
NStringEncoding NUnicodeParser::GetGenericEncoding(NStringEncoding theEncoding) const
{


	// Convert the encoding
	switch (theEncoding) {
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF16LE:
			theEncoding = kNStringEncodingUTF16;
			break;

		case kNStringEncodingUTF32BE:
		case kNStringEncodingUTF32LE:
			theEncoding = kNStringEncodingUTF32;
			break;
		
		default:
			break;
		}
	
	return(theEncoding);
}





//============================================================================
//		NUnicodeParser::GetEndianFormat : Get the EndianFormat of a UTF encoding.
//----------------------------------------------------------------------------
NEndianFormat NUnicodeParser::GetEndianFormat(NStringEncoding theEncoding) const
{	NEndianFormat		theFormat;



	// Get the format
	switch (theEncoding) {
		case kNStringEncodingUTF8:
		case kNStringEncodingUTF16:
		case kNStringEncodingUTF32:
			theFormat = kNEndianNative;
			break;

		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF32BE:
			theFormat = kNEndianBig;
			break;

		case kNStringEncodingUTF16LE:
		case kNStringEncodingUTF32LE:
			theFormat = kNEndianLittle;
			break;

		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			theFormat = kNEndianNative;
			break;
		}
	
	return(theFormat);
}





//============================================================================
//		NUnicodeParser::GetNativeUTF8 : Get a native utf8_t.
//----------------------------------------------------------------------------
utf8_t NUnicodeParser::GetNativeUTF8(utf8_t theChar, NStringEncoding /*theEncoding*/) const
{


	// Get the character
	return(theChar);
}





//============================================================================
//		NUnicodeParser::GetNativeUTF16 : Get a native utf16_t.
//----------------------------------------------------------------------------
utf16_t NUnicodeParser::GetNativeUTF16(utf16_t theChar, NStringEncoding theEncoding) const
{


	// Get the character
	if (GetEndianFormat(theEncoding) != kNEndianNative)
		NSwapUInt16((uint16_t *) &theChar);
	
	return(theChar);
}





//============================================================================
//		NUnicodeParser::GetNativeUTF32 : Get a native utf32_t.
//----------------------------------------------------------------------------
utf32_t NUnicodeParser::GetNativeUTF32(utf32_t theChar, NStringEncoding theEncoding) const
{


	// Get the character
	if (GetEndianFormat(theEncoding) != kNEndianNative)
		NSwapUInt32((uint32_t *) &theChar);
	
	return(theChar);
}





#pragma mark private
//============================================================================
//		NUnicodeParser::GetCodePointsUTF8 : Get the code points from a UTF8 string.
//----------------------------------------------------------------------------
NRangeList NUnicodeParser::GetCodePointsUTF8(const NRange &theBOM) const
{	NIndex			n, theSize, charSize;
	NRangeList		theResult;
	NRange			theRange;
	const uint8_t	*theData;
	utf8_t			theChar;



	// Get the state we need
	theSize = mData.GetSize();
	theData = mData.GetData();



	// Get the code points
	theResult.reserve(theSize / sizeof(utf8_t));
	n = (theBOM.IsEmpty() ? 0 : theBOM.GetNext());

	while (n < theSize)
		{
		// Get the character
		theChar  = GetNativeUTF8(*((const utf8_t *) &theData[n]), mEncoding);
		charSize = kUTF8TrailingBytes[(uint8_t) theChar] + 1;
		NN_ASSERT(charSize >= 1 && charSize <= 6);


		// Break on NULL
		if (theChar == 0x00)
			break;


		// Add the code point
		theRange = NRange(n, charSize);
		theResult.push_back(theRange);
		
		n += charSize;
		}

	return(theResult);
}





//============================================================================
//		NUnicodeParser::GetCodePointsUTF16 : Get the code points from a UTF16 string.
//----------------------------------------------------------------------------
NRangeList NUnicodeParser::GetCodePointsUTF16(const NRange &theBOM) const
{	NIndex			n, theSize, charSize;
	NRangeList		theResult;
	NRange			theRange;
	const uint8_t	*theData;
	utf16_t			theChar;



	// Get the state we need
	theSize = mData.GetSize();
	theData = mData.GetData();

	NN_ASSERT((theSize % sizeof(utf16_t)) == 0);



	// Get the code points
	theResult.reserve(theSize / sizeof(utf16_t));
	n = (theBOM.IsEmpty() ? 0 : theBOM.GetNext());

	while (n < theSize)
		{
		// Get the character
		theChar  = GetNativeUTF16(*((const utf16_t *) &theData[n]), mEncoding);
		charSize = 2;
		
		if (theChar >= kUTF16SurrogateHighStart && theChar <= kUTF16SurrogateHighEnd)
			{
			NN_ASSERT(n <= (theSize-2));
			NN_ASSERT(GetNativeUTF16(*((const utf16_t *) &theData[n+2]), mEncoding) >= kUTF16SurrogateLowStart &&
					  GetNativeUTF16(*((const utf16_t *) &theData[n+2]), mEncoding) <= kUTF16SurrogateLowEnd);
			
			charSize += 2;
			}


		// Break on NULL
		if (theChar == 0x0000)
			break;
		
		
		// Add the code point
		theRange = NRange(n, charSize);
		theResult.push_back(theRange);
		
		n += charSize;
		}

	return(theResult);
}





//============================================================================
//		NUnicodeParser::GetCodePointsUTF32 : Get the code points from a UTF32 string.
//----------------------------------------------------------------------------
NRangeList NUnicodeParser::GetCodePointsUTF32(const NRange &theBOM) const
{	NIndex			n, theSize, charSize;
	NRangeList		theResult;
	NRange			theRange;
	const uint8_t	*theData;
	utf32_t			theChar;



	// Get the state we need
	theSize = mData.GetSize();
	theData = mData.GetData();

	NN_ASSERT((theSize % sizeof(utf32_t)) == 0);



	// Get the code points
	theResult.reserve(theSize / sizeof(utf32_t));
	n = (theBOM.IsEmpty() ? 0 : theBOM.GetNext());
	
	while (n < theSize)
		{
		// Get the character
		theChar  = GetNativeUTF32(*((const utf32_t *) &theData[n]), mEncoding);
		charSize = 4;


		// Break on NULL
		if (theChar == 0x00000000)
			break;


		// Add the code point
		theRange = NRange(n, charSize);
		theResult.push_back(theRange);
		
		n += charSize;
		}

	return(theResult);
}





//============================================================================
//		NUnicodeParser::AddBOMToUTF8 : Add a UTF8 BOM.
//----------------------------------------------------------------------------
void NUnicodeParser::AddBOMToUTF8(NData &theData) const
{	const uint8_t	*theBOM;



	// Insert the BOM
	theBOM = theData.InsertData(0, NN_ARRAY_SIZE(kUTF8BOM), kUTF8BOM);
	NN_ASSERT(theBOM == theData.GetData());
}





//============================================================================
//		NUnicodeParser::AddBOMToUTF16 : Add a UTF16 BOM.
//----------------------------------------------------------------------------
void NUnicodeParser::AddBOMToUTF16(NData &theData, NEndianFormat theFormat) const
{	const uint8_t	*theBOM;



	// Insert the BOM
	if (theFormat == kNEndianBig)
		theBOM = theData.InsertData(0, NN_ARRAY_SIZE(kUTF16BOMBE), kUTF16BOMBE);
	else
		theBOM = theData.InsertData(0, NN_ARRAY_SIZE(kUTF16BOMLE), kUTF16BOMLE);
	
	NN_ASSERT(theBOM == theData.GetData());
}





//============================================================================
//		NUnicodeParser::AddBOMToUTF32 : Add a UTF32 BOM.
//----------------------------------------------------------------------------
void NUnicodeParser::AddBOMToUTF32(NData &theData, NEndianFormat theFormat) const
{	const uint8_t	*theBOM;



	// Insert the BOM
	if (theFormat == kNEndianBig)
		theBOM = theData.InsertData(0, NN_ARRAY_SIZE(kUTF32BOMBE), kUTF32BOMBE);
	else
		theBOM = theData.InsertData(0, NN_ARRAY_SIZE(kUTF32BOMLE), kUTF32BOMLE);
	
	NN_ASSERT(theBOM == theData.GetData());
}





