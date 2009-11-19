/*	NAME:
		NStringEncoder.cpp

	DESCRIPTION:
		String encoder.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "ConvertUTF.h"

#include "NUnicodeParser.h"
#include "NStringEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt64 kStringTerminator									= 0;





//============================================================================
//		NStringEncoder::NStringEncoder : Constructor.
//----------------------------------------------------------------------------
NStringEncoder::NStringEncoder(void)
{
}





//============================================================================
//		NStringEncoder::~NStringEncoder : Destructor.
//----------------------------------------------------------------------------
NStringEncoder::~NStringEncoder(void)
{
}





//============================================================================
//		NStringEncoder::Convert : Convert a string.
//----------------------------------------------------------------------------
//		Note :	Currently only UTF string encodings are defined, which makes
//				the structure of the string converter straightforward.
//
//				This will need to be generalised to push all conversions
//				through a common format when non-UTF encodings are supported.
//----------------------------------------------------------------------------
NStatus NStringEncoder::Convert(const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding)
{	NStringEncoding		genericSrc, genericDst;
	NUnicodeParser		theParser;
	NData				tmpSrc;
	NStatus				theErr;



	// Get the state we need
	genericSrc = theParser.GetGenericEncoding(srcEncoding);
	genericDst = theParser.GetGenericEncoding(dstEncoding);
	tmpSrc     = srcData;



	// Prepare the input
	theParser.RemoveBOM(tmpSrc, srcEncoding);
	RemoveTerminator(   tmpSrc, srcEncoding);
	
	SwapUTF(tmpSrc, srcEncoding, genericSrc);



	// Convert the string
	switch (genericSrc) {
		case kNStringEncodingUTF8:
			theErr = ConvertFromUTF8( tmpSrc, dstData, genericDst);
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertFromUTF16(tmpSrc, dstData, genericDst);
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertFromUTF32(tmpSrc, dstData, genericDst);
			break;

		default:
			NN_LOG("Unknown encoding: %d", genericSrc);
			theErr = kNErrParam;
			break;
		}



	// Prepare the result
	if (theErr == kNoErr)
		SwapUTF(dstData, genericDst, dstEncoding);

	return(theErr);
}





//============================================================================
//		NStringEncoder::AddTerminator : Add a terminating null character.
//----------------------------------------------------------------------------
void NStringEncoder::AddTerminator(NData &theData, NStringEncoding theEncoding)
{	NIndex			sizeData, sizeTerm;
	bool			modifyData;
	const UInt8		*thePtr;



	// Get the state we need
	sizeData = theData.GetSize();
	sizeTerm = GetMaxCharSize(theEncoding);

	NN_ASSERT(sizeTerm > 0 && sizeTerm <= (NIndex) sizeof(kStringTerminator));



	// Add the terminator
	modifyData = (sizeData < sizeTerm);

	if (!modifyData)
		{
		thePtr     = theData.GetData(sizeData - sizeTerm);
		modifyData = (memcmp(thePtr, &kStringTerminator, sizeTerm) != 0);
		}

	if (modifyData)
		theData.SetSize(sizeData + sizeTerm);
}





//============================================================================
//		NStringEncoder::RemoveTerminator : Remove a terminating null character.
//----------------------------------------------------------------------------
void NStringEncoder::RemoveTerminator(NData &theData, NStringEncoding theEncoding)
{	NIndex			sizeData, sizeTerm;
	bool			modifyData;
	const UInt8		*thePtr;



	// Get the state we need
	sizeData = theData.GetSize();
	sizeTerm = GetMaxCharSize(theEncoding);

	NN_ASSERT(sizeTerm > 0 && sizeTerm <= (NIndex) sizeof(kStringTerminator));



	// Remove the terminator
	modifyData = (sizeData >= sizeTerm);
	
	if (modifyData)
		{
		thePtr     = theData.GetData(sizeData - sizeTerm);
		modifyData = (memcmp(thePtr, &kStringTerminator, sizeTerm) == 0);
		}

	if (modifyData)
		theData.SetSize(sizeData - sizeTerm);
}





//============================================================================
//		NStringEncoder::GetEncoding : Get a string encoding.
//----------------------------------------------------------------------------
NStringEncoding NStringEncoder::GetEncoding(const NData &theData)
{	NStringEncoding		theEncoding;
	NUnicodeParser		theParser;
	NRange				theRange;



	// Determine the encoding
	//
	// For now we assume a BOM, or default to UTF8. This should be improved with
	// additional heuristics (check for UTF16/32 encodings of common vowels in
	// both BE and LE, etc).
	theEncoding = theParser.GetBOM(theData, theRange);
	if (theEncoding == kNStringEncodingInvalid)
		theEncoding = kNStringEncodingUTF8;

	return(theEncoding);
}





//============================================================================
//		NStringEncoder::GetMaxCharSize : Get the maximum size of a character.
//----------------------------------------------------------------------------
#pragma mark -
NIndex NStringEncoder::GetMaxCharSize(NStringEncoding theEncoding)
{	NIndex		theSize;



	// Get the state we need
	switch (theEncoding) {
		case kNStringEncodingUTF8:
			theSize = (NIndex) sizeof(UTF8Char);
			break;

		case kNStringEncodingUTF16:
		case kNStringEncodingUTF16BE:
		case kNStringEncodingUTF16LE:
			theSize = (NIndex) sizeof(UTF16Char);
			break;

		case kNStringEncodingUTF32:
		case kNStringEncodingUTF32BE:
		case kNStringEncodingUTF32LE:
			theSize = (NIndex) sizeof(UTF32Char);
			break;

		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			theSize = 0;
			break;
		}

	return(theSize);
}





//============================================================================
//		NStringEncoder::ConvertFromUTF8 : Convert a UTF8 string.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertFromUTF8(const NData &srcData, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			dstData = srcData;
			theErr  = kNoErr;
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertUTF8ToUTF16(srcData, dstData);
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertUTF8ToUTF32(srcData, dstData);
			break;

		default:
			NN_LOG("Unknown encoding: %d", dstEncoding);
			theErr = kNErrParam;
			break;
		}

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertFromUTF16 : Convert a UTF16 string.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertFromUTF16(const NData &srcData, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			theErr = ConvertUTF16ToUTF8(srcData, dstData);
			break;

		case kNStringEncodingUTF16:
			dstData = srcData;
			theErr  = kNoErr;
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertUTF16ToUTF32(srcData, dstData);
			break;

		default:
			NN_LOG("Unknown encoding: %d", dstEncoding);
			theErr = kNErrParam;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertFromUTF32 : Convert a UTF32 string.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertFromUTF32(const NData &srcData, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			theErr = ConvertUTF32ToUTF8(srcData, dstData);
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertUTF32ToUTF16(srcData, dstData);
			break;

		case kNStringEncodingUTF32:
			dstData = srcData;
			theErr  = kNoErr;
			break;

		default:
			NN_LOG("Unknown encoding: %d", dstEncoding);
			theErr = kNErrParam;
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF8ToUTF16 : Convert UTF8 to UTF16.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF8ToUTF16(const NData &srcData, NData &dstData)
{	const UInt8				*srcBase, *dstBase;
	const UTF8				*srcStart, *srcEnd;
	UTF16					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize() * 2);

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF8 *) (srcBase + 0);
		srcEnd   = (const UTF8 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF16 *) (dstBase + 0);
		dstEnd   = (UTF16 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF8toUTF16(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF8ToUTF32 : Convert UTF8 to UTF32.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF8ToUTF32(const NData &srcData, NData &dstData)
{	const UInt8				*srcBase, *dstBase;
	const UTF8				*srcStart, *srcEnd;
	UTF32					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize() * 4);

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF8 *) (srcBase + 0);
		srcEnd   = (const UTF8 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF32 *) (dstBase + 0);
		dstEnd   = (UTF32 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF8toUTF32(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF16ToUTF8 : Convert UTF16 to UTF8.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF16ToUTF8(const NData &srcData, NData &dstData)
{	const UInt8				*srcBase, *dstBase;
	const UTF16				*srcStart, *srcEnd;
	UTF8					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize());

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF16 *) (srcBase + 0);
		srcEnd   = (const UTF16 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF8 *) (dstBase + 0);
		dstEnd   = (UTF8 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF16toUTF8(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF16ToUTF32 : Convert UTF16 to UTF32.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF16ToUTF32(const NData &srcData, NData &dstData)
{	const UInt8				*srcBase, *dstBase;
	const UTF16				*srcStart, *srcEnd;
	UTF32					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize() * 2);

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF16 *) (srcBase + 0);
		srcEnd   = (const UTF16 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF32 *) (dstBase + 0);
		dstEnd   = (UTF32 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF16toUTF32(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF32ToUTF8 : Convert UTF32 to UTF8.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF32ToUTF8(const NData &srcData, NData &dstData)
{	const UInt8				*srcBase, *dstBase;
	const UTF32				*srcStart, *srcEnd;
	UTF8					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize());

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF32 *) (srcBase + 0);
		srcEnd   = (const UTF32 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF8 *) (dstBase + 0);
		dstEnd   = (UTF8 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF32toUTF8(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF32ToUTF16 : Convert UTF32 to UTF16.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF32ToUTF16(const NData &srcData, NData &dstData)
{	const UInt8				*srcBase, *dstBase;
	const UTF32				*srcStart, *srcEnd;
	UTF16					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcData.GetSize());

	do
		{
		srcBase  = srcData.GetData();
		srcStart = (const UTF32 *) (srcBase + 0);
		srcEnd   = (const UTF32 *) (srcBase + srcData.GetSize());

		dstBase  = dstData.GetData();
		dstStart = (UTF16 *) (dstBase + 0);
		dstEnd   = (UTF16 *) (dstBase + dstData.GetSize());
		
		theResult = ConvertUTF32toUTF16(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ConvertUTF(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF : Process a Unicode conversion.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF(NData &theData, const void *dataEnd, UInt32 theResult)
{	NStatus		theErr;



	// Process the result
	switch ((ConversionResult) theResult) {
		case conversionOK:
			theData.SetSize(((const UInt8 *) dataEnd) - theData.GetData());
			theErr = kNoErr;
			break;
			
		case targetExhausted:
			theData.SetSize(theData.GetSize() * 2);
			theErr = kNErrExhaustedDst;
			break;
			
		case sourceExhausted:
			theErr = kNErrExhaustedSrc;
			break;

		case sourceIllegal:
			theErr = kNErrMalformed;
			break;

		default:
			NN_LOG("Unknown result: %d", theResult);
			theErr = kNErrParam;
			break;
		}

	return(theErr);
}





//============================================================================
//		NStringEncoder::SwapUTF : Endian-swap UTF data.
//----------------------------------------------------------------------------
void NStringEncoder::SwapUTF(NData &theData, NStringEncoding srcEncoding, NStringEncoding dstEncoding)
{	NIndex				n, charSize, dataSize;
	NEndianFormat		srcFormat, dstFormat;
	NUnicodeParser		theParser;
	UInt8				*dataPtr;



	// Validate our parameters
	NN_ASSERT(GetMaxCharSize(srcEncoding) == GetMaxCharSize(dstEncoding));



	// Get the state we need
	srcFormat = theParser.GetEndianFormat(srcEncoding);
	dstFormat = theParser.GetEndianFormat(dstEncoding);
	charSize  = GetMaxCharSize(srcEncoding);
	
	if (charSize == 1 || srcFormat == dstFormat)
		return;



	// Get the data
	dataPtr  = theData.GetData();
	dataSize = theData.GetSize();
	
	NN_ASSERT((dataSize % charSize) == 0);



	// Swap the data
	for (n = 0; n < dataSize; n += charSize)
		{
		switch (charSize) {
			case 2:
				NSwapUInt16((UInt16 *) dataPtr);
				break;
			
			case 4:
				NSwapUInt32((UInt32 *) dataPtr);
				break;

			default:
				NN_LOG("Unknown char size: %d", charSize);
				return;
				break;
			}
		
		dataPtr += charSize;
		}
}


