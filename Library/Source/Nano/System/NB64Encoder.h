/*	NAME:
		NB64Encoder.h

	DESCRIPTION:
		Base64 encoder.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NB64ENCODER_HDR
#define NB64ENCODER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NData.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NB64Encoder {
public:
										NB64Encoder(void);
	virtual							   ~NB64Encoder(void);


	// Encode/decode to Base64
	NString								Encode(const NData   &theValue);
	NData								Decode(const NString &theValue);


private:


};





#endif // NB64ENCODER_HDR


