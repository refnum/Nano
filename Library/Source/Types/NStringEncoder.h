/*	NAME:
		NStringEncoder.h

	DESCRIPTION:
		String encoder.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSTRINGENCODER_HDR
#define NSTRINGENCODER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"





//============================================================================
//      Constants
//----------------------------------------------------------------------------
typedef enum {
	kNStringEncodingUTF8,
	kNStringEncodingUTF16,
	kNStringEncodingUTF32
} NStringEncoding;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NStringEncoder {
public:
										 NStringEncoder(void);
	virtual								~NStringEncoder(void);


	// Convert a string
	NStatus								Convert(const NData  &srcData,                     NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding);
	NStatus								Convert(      NIndex  srcSize, const void *srcPtr, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding);


private:
	NStatus								ConvertFromUTF8( NIndex srcSize, const UInt8  *srcPtr, NData &dstData, NStringEncoding dstEncoding);
	NStatus								ConvertFromUTF16(NIndex srcSize, const UInt16 *srcPtr, NData &dstData, NStringEncoding dstEncoding);
	NStatus								ConvertFromUTF32(NIndex srcSize, const UInt32 *srcPtr, NData &dstData, NStringEncoding dstEncoding);

	NStatus								ConvertUTF8ToUTF16(NIndex srcSize, const UInt8 *srcPtr, NData &dstData);
	NStatus								ConvertUTF8ToUTF32(NIndex srcSize, const UInt8 *srcPtr, NData &dstData);

	NStatus								ConvertUTF16ToUTF8( NIndex srcSize, const UInt16 *srcPtr, NData &dstData);
	NStatus								ConvertUTF16ToUTF32(NIndex srcSize, const UInt16 *srcPtr, NData &dstData);

	NStatus								ConvertUTF32ToUTF8( NIndex srcSize, const UInt32 *srcPtr, NData &dstData);
	NStatus								ConvertUTF32ToUTF16(NIndex srcSize, const UInt32 *srcPtr, NData &dstData);
	
	NStatus								ProcessUnicode(NData &theData, const void *dataEnd, UInt32 theResult);
};





#endif // NSTRINGENCODER_HDR
