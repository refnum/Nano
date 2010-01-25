/*	NAME:
		NDataEncoder.h

	DESCRIPTION:
		Data encoder.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATAENCODER_HDR
#define NDATAENCODER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Data encodings
typedef enum {
	kNDataEncodingHex,
	kNDataEncodingB64,
} NDataEncoding;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDataEncoder {
public:
										NDataEncoder(void);
	virtual							   ~NDataEncoder(void);


	// Encode/decode data
	NString								Encode(const NData   &theValue, NDataEncoding theEncoding);
	NData								Decode(const NString &theValue, NDataEncoding theEncoding);


private:
	NString								Hex_Encode(const NData   &theValue);
	NData								Hex_Decode(const NString &theValue);

	NString								B64_Encode(const NData   &theValue);
	NData								B64_Decode(const NString &theValue);


};





#endif // NDATAENCODER_HDR


