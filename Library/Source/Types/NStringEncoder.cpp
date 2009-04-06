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

#include "NStringEncoder.h"





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
NStatus NStringEncoder::Convert(const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding)
{


	// Convert the string
	return(Convert(srcData.GetSize(), srcData.GetData(), dstData, srcEncoding, dstEncoding));
}





//============================================================================
//		NStringEncoder::Convert : Convert a string.
//----------------------------------------------------------------------------
NStatus NStringEncoder::Convert(NIndex srcSize, const void *srcPtr, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (srcEncoding) {
		case kNStringEncodingUTF8:
			theErr = ConvertFromUTF8(srcSize, (const UInt8 *) srcPtr, dstData, dstEncoding);
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertFromUTF16(srcSize, (const UInt16 *) srcPtr, dstData, dstEncoding);
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertFromUTF32(srcSize, (const UInt32 *) srcPtr, dstData, dstEncoding);
			break;

		default:
			NN_LOG("Unknown encoding: %d", srcEncoding);
			theErr = kNErrParam;
			break;
		}

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertFromUTF8 : Convert a UTF8 string.
//----------------------------------------------------------------------------
#pragma mark -
NStatus NStringEncoder::ConvertFromUTF8(NIndex srcSize, const UInt8 *srcPtr, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			dstData = NData(srcSize, srcPtr);
			theErr  = noErr;
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertUTF8ToUTF16(srcSize, srcPtr, dstData);
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertUTF8ToUTF32(srcSize, srcPtr, dstData);
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
NStatus NStringEncoder::ConvertFromUTF16(NIndex srcSize, const UInt16 *srcPtr, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			theErr = ConvertUTF16ToUTF8(srcSize, srcPtr, dstData);
			break;

		case kNStringEncodingUTF16:
			dstData = NData(srcSize, srcPtr);
			theErr  = noErr;
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertUTF16ToUTF32(srcSize, srcPtr, dstData);
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
NStatus NStringEncoder::ConvertFromUTF32(NIndex srcSize, const UInt32 *srcPtr, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			theErr = ConvertUTF32ToUTF8(srcSize, srcPtr, dstData);
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertUTF32ToUTF16(srcSize, srcPtr, dstData);
			break;

		case kNStringEncodingUTF32:
			dstData = NData(srcSize, srcPtr);
			theErr  = noErr;
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
NStatus NStringEncoder::ConvertUTF8ToUTF16(NIndex srcSize, const UInt8 *srcPtr, NData &dstData)
{	const UTF8				*srcStart, *srcEnd;
	UTF16					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcSize * 2);

	do
		{
		srcStart = srcPtr;
		srcEnd   = srcStart + srcSize;

		dstStart = (UTF16 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF8toUTF16(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF8ToUTF32 : Convert UTF8 to UTF32.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF8ToUTF32(NIndex srcSize, const UInt8 *srcPtr, NData &dstData)
{	const UTF8				*srcStart, *srcEnd;
	UTF32					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcSize * 4);

	do
		{
		srcStart = srcPtr;
		srcEnd   = srcStart + srcSize;

		dstStart = (UTF32 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF8toUTF32(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF16ToUTF8 : Convert UTF16 to UTF8.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF16ToUTF8(NIndex srcSize, const UInt16 *srcPtr, NData &dstData)
{	const UTF16				*srcStart, *srcEnd;
	UTF8					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcSize);

	do
		{
		srcStart = srcPtr;
		srcEnd   = srcStart + srcSize;

		dstStart = (UTF8 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF16toUTF8(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF16ToUTF32 : Convert UTF16 to UTF32.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF16ToUTF32(NIndex srcSize, const UInt16 *srcPtr, NData &dstData)
{	const UTF16				*srcStart, *srcEnd;
	UTF32					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcSize * 2);

	do
		{
		srcStart = srcPtr;
		srcEnd   = srcStart + srcSize;

		dstStart = (UTF32 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF16toUTF32(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF32ToUTF8 : Convert UTF32 to UTF8.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF32ToUTF8(NIndex srcSize, const UInt32 *srcPtr, NData &dstData)
{	const UTF32				*srcStart, *srcEnd;
	UTF8					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcSize);

	do
		{
		srcStart = srcPtr;
		srcEnd   = srcStart + srcSize;

		dstStart = (UTF8 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF32toUTF8(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ConvertUTF32ToUTF16 : Convert UTF32 to UTF16.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ConvertUTF32ToUTF16(NIndex srcSize, const UInt32 *srcPtr, NData &dstData)
{	const UTF32				*srcStart, *srcEnd;
	UTF16					*dstStart, *dstEnd;
	ConversionResult		theResult;
	NStatus					theErr;



	// Convert the data
	dstData.SetSize(srcSize);

	do
		{
		srcStart = srcPtr;
		srcEnd   = srcStart + srcSize;

		dstStart = (UTF16 *)  dstData.GetData();
		dstEnd   = dstStart + dstData.GetSize();
		
		theResult = ConvertUTF32toUTF16(&srcStart, srcEnd, &dstStart, dstEnd, lenientConversion);
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
		}
	while (theErr == kNErrExhaustedDst);

	return(theErr);
}





//============================================================================
//		NStringEncoder::ProcessUnicode : Process a Unicode conversion.
//----------------------------------------------------------------------------
NStatus NStringEncoder::ProcessUnicode(NData &theData, const void *dataEnd, UInt32 theResult)
{	OSStatus	theErr;



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
			theErr = kNErrInternal;
			break;
		}

	return(theErr);
}


