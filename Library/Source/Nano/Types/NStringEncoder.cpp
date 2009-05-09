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
NStatus NStringEncoder::Convert(const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (srcEncoding) {
		case kNStringEncodingUTF8:
			theErr = ConvertFromUTF8(srcData, dstData, dstEncoding);
			break;

		case kNStringEncodingUTF16:
			theErr = ConvertFromUTF16(srcData, dstData, dstEncoding);
			break;

		case kNStringEncodingUTF32:
			theErr = ConvertFromUTF32(srcData, dstData, dstEncoding);
			break;

		default:
			NN_LOG("Unknown encoding: %d", srcEncoding);
			theErr = kNErrParam;
			break;
		}



	// Remove the terminator
	if (theErr == noErr)
		RemoveTerminator(dstData, dstEncoding);

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
	sizeTerm = GetTerminatorSize(theEncoding);

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
	sizeTerm = GetTerminatorSize(theEncoding);

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
//		NStringEncoder::ConvertFromUTF8 : Convert a UTF8 string.
//----------------------------------------------------------------------------
#pragma mark -
NStatus NStringEncoder::ConvertFromUTF8(const NData &srcData, NData &dstData, NStringEncoding dstEncoding)
{	NStatus		theErr;



	// Convert the string
	switch (dstEncoding) {
		case kNStringEncodingUTF8:
			dstData = srcData;
			theErr  = noErr;
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
			theErr  = noErr;
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
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
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
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
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
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
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
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
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
		theErr    = ProcessUnicode(dstData, dstStart, theResult);
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
			theErr = kNErrParam;
			break;
		}

	return(theErr);
}





//============================================================================
//		NStringEncoder::GetTerminatorSize : Get the size of a terminator.
//----------------------------------------------------------------------------
NIndex NStringEncoder::GetTerminatorSize(NStringEncoding theEncoding)
{	NIndex		theSize;



	// Get the state we need
	switch (theEncoding) {
		case kNStringEncodingUTF8:
			theSize = sizeof(UTF8Char);
			break;

		case kNStringEncodingUTF16:
			theSize = sizeof(UTF16Char);
			break;

		case kNStringEncodingUTF32:
			theSize = sizeof(UTF32Char);
			break;

		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			theSize = 0;
			break;
		}

	return(theSize);
}



