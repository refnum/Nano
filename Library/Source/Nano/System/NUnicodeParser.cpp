/*	NAME:
		NUnicodeParser.cpp

	DESCRIPTION:
		Unicode parser.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
static const UInt8 kUTF8BOM[]										= { 0xEF, 0xBB, 0xBF };
static const UInt8 kUTF16BOMBE[]									= { 0xFE, 0xFF };
static const UInt8 kUTF16BOMLE[]									= { 0xFF, 0xFE };
static const UInt8 kUTF32BOMBE[]									= { 0x00, 0x00, 0xFE, 0xFF };
static const UInt8 kUTF32BOMLE[]									= { 0xFF, 0xFE, 0x00, 0x00 };


// UTF16
static const UTF16Char kUTF16SurrogateHighStart						= 0xD800;
static const UTF16Char kUTF16SurrogateHighEnd						= 0xDBFF;
static const UTF16Char kUTF16SurrogateLowStart						= 0xDC00;
static const UTF16Char kUTF16SurrogateLowEnd						= 0xDFFF;


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
UTF32Char NUnicodeParser::GetChar(NIndex theIndex, bool toLower) const
{	NStringEncoder		theEncoder;
	NRange				theRange;
	const UInt8			*srcPtr;
	UTF32Char			theChar;



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
bool NUnicodeParser::IsSpace(UTF32Char theChar) const
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
bool NUnicodeParser::IsPunct(UTF32Char theChar) const
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
bool NUnicodeParser::IsAlpha(UTF32Char theChar) const
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
bool NUnicodeParser::IsDigit(UTF32Char theChar) const
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
UTF32Char NUnicodeParser::GetLower(UTF32Char theChar) const
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
UTF32Char NUnicodeParser::GetUpper(UTF32Char theChar) const
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
	const UInt8			*dataPtr;
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
//		NUnicodeParser::GetNativeUTF8 : Get a native UTF8Char.
//----------------------------------------------------------------------------
UTF8Char NUnicodeParser::GetNativeUTF8(UTF8Char theChar, NStringEncoding /*theEncoding*/) const
{


	// Get the character
	return(theChar);
}





//============================================================================
//		NUnicodeParser::GetNativeUTF16 : Get a native UTF16Char.
//----------------------------------------------------------------------------
UTF16Char NUnicodeParser::GetNativeUTF16(UTF16Char theChar, NStringEncoding theEncoding) const
{


	// Get the character
	if (GetEndianFormat(theEncoding) != kNEndianNative)
		NSwapUInt16((UInt16 *) &theChar);
	
	return(theChar);
}





//============================================================================
//		NUnicodeParser::GetNativeUTF32 : Get a native UTF32Char.
//----------------------------------------------------------------------------
UTF32Char NUnicodeParser::GetNativeUTF32(UTF32Char theChar, NStringEncoding theEncoding) const
{


	// Get the character
	if (GetEndianFormat(theEncoding) != kNEndianNative)
		NSwapUInt32((UInt32 *) &theChar);
	
	return(theChar);
}





//============================================================================
//		NUnicodeParser::GetCodePointsUTF8 : Get the code points from a UTF8 string.
//----------------------------------------------------------------------------
#pragma mark -
NRangeList NUnicodeParser::GetCodePointsUTF8(const NRange &theBOM) const
{	NIndex			n, theSize, charSize;
	NRangeList		theResult;
	NRange			theRange;
	const UInt8		*theData;
	UTF8Char		theChar;



	// Get the state we need
	theSize = mData.GetSize();
	theData = mData.GetData();



	// Get the code points
	theResult.reserve(theSize / sizeof(UTF8Char));
	n = (theBOM.IsEmpty() ? 0 : theBOM.GetNext());

	while (n < theSize)
		{
		// Get the character
		theChar  = GetNativeUTF8(*((const UTF8Char *) &theData[n]), mEncoding);
		charSize = kUTF8TrailingBytes[theChar] + 1;
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
	const UInt8		*theData;
	UTF16Char		theChar;



	// Get the state we need
	theSize = mData.GetSize();
	theData = mData.GetData();

	NN_ASSERT((theSize % sizeof(UTF16Char)) == 0);



	// Get the code points
	theResult.reserve(theSize / sizeof(UTF16Char));
	n = (theBOM.IsEmpty() ? 0 : theBOM.GetNext());

	while (n < theSize)
		{
		// Get the character
		theChar  = GetNativeUTF16(*((const UTF16Char *) &theData[n]), mEncoding);
		charSize = 2;
		
		if (theChar >= kUTF16SurrogateHighStart && theChar <= kUTF16SurrogateHighEnd)
			{
			NN_ASSERT(n <= (theSize-2));
			NN_ASSERT(GetNativeUTF16(*((const UTF16Char *) &theData[n+2]), mEncoding) >= kUTF16SurrogateLowStart &&
					  GetNativeUTF16(*((const UTF16Char *) &theData[n+2]), mEncoding) <= kUTF16SurrogateLowEnd);
			
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
	const UInt8		*theData;
	UTF32Char		theChar;



	// Get the state we need
	theSize = mData.GetSize();
	theData = mData.GetData();

	NN_ASSERT((theSize % sizeof(UTF32Char)) == 0);



	// Get the code points
	theResult.reserve(theSize / sizeof(UTF32Char));
	n = (theBOM.IsEmpty() ? 0 : theBOM.GetNext());
	
	while (n < theSize)
		{
		// Get the character
		theChar  = GetNativeUTF32(*((const UTF32Char *) &theData[n]), mEncoding);
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
{	const UInt8		*theBOM;



	// Insert the BOM
	theBOM = theData.ReplaceData(kNRangeNone, NN_ARRAY_SIZE(kUTF8BOM), kUTF8BOM);
	NN_ASSERT(theBOM == theData.GetData());
}





//============================================================================
//		NUnicodeParser::AddBOMToUTF16 : Add a UTF16 BOM.
//----------------------------------------------------------------------------
void NUnicodeParser::AddBOMToUTF16(NData &theData, NEndianFormat theFormat) const
{	const UInt8		*theBOM;



	// Insert the BOM
	if (theFormat == kNEndianBig)
		theBOM = theData.ReplaceData(kNRangeNone, NN_ARRAY_SIZE(kUTF16BOMBE), kUTF16BOMBE);
	else
		theBOM = theData.ReplaceData(kNRangeNone, NN_ARRAY_SIZE(kUTF16BOMLE), kUTF16BOMLE);
	
	NN_ASSERT(theBOM == theData.GetData());
}





//============================================================================
//		NUnicodeParser::AddBOMToUTF32 : Add a UTF32 BOM.
//----------------------------------------------------------------------------
void NUnicodeParser::AddBOMToUTF32(NData &theData, NEndianFormat theFormat) const
{	const UInt8		*theBOM;



	// Insert the BOM
	if (theFormat == kNEndianBig)
		theBOM = theData.ReplaceData(kNRangeNone, NN_ARRAY_SIZE(kUTF32BOMBE), kUTF32BOMBE);
	else
		theBOM = theData.ReplaceData(kNRangeNone, NN_ARRAY_SIZE(kUTF32BOMLE), kUTF32BOMLE);
	
	NN_ASSERT(theBOM == theData.GetData());
}





