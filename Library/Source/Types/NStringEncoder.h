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
	NStatus								Convert(const NData &srcData, NData &dstData, NStringEncoding srcEncoding, NStringEncoding dstEncoding);


private:
	NStatus								ConvertFromUTF8( const NData &srcData, NData &dstData, NStringEncoding dstEncoding);
	NStatus								ConvertFromUTF16(const NData &srcData, NData &dstData, NStringEncoding dstEncoding);
	NStatus								ConvertFromUTF32(const NData &srcData, NData &dstData, NStringEncoding dstEncoding);


private:


};





#endif // NSTRINGENCODER_HDR
