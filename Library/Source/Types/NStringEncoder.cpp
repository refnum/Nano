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
#include "NUnicodeUtilities.h"
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

	return(theErr);
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
			theErr = NUnicodeUtilities::ConvertUTF8ToUTF16(srcData, dstData);
			break;

		case kNStringEncodingUTF32:
			theErr = NUnicodeUtilities::ConvertUTF8ToUTF32(srcData, dstData);
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
			theErr = NUnicodeUtilities::ConvertUTF16ToUTF8(srcData, dstData);
			break;

		case kNStringEncodingUTF16:
			dstData = srcData;
			theErr  = noErr;
			break;

		case kNStringEncodingUTF32:
			theErr = NUnicodeUtilities::ConvertUTF16ToUTF32(srcData, dstData);
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
			theErr = NUnicodeUtilities::ConvertUTF32ToUTF8(srcData, dstData);
			break;

		case kNStringEncodingUTF16:
			theErr = NUnicodeUtilities::ConvertUTF32ToUTF16(srcData, dstData);
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



