/*	NAME:
		NB64Encoder.cpp

	DESCRIPTION:
		Base64 encoder.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "cencode.h"
#include "cdecode.h"

#include "NB64Encoder.h"





//============================================================================
//		NB64Encoder::NB64Encoder : Constructor.
//----------------------------------------------------------------------------
NB64Encoder::NB64Encoder(void)
{
}





//============================================================================
//		NB64Encoder::~NB64Encoder : Destructor.
//----------------------------------------------------------------------------
NB64Encoder::~NB64Encoder(void)
{
}





//============================================================================
//		NB64Encoder::Encode : Encode to Base64.
//----------------------------------------------------------------------------
NString NB64Encoder::Encode(const NData &theValue)
{	NData					theBuffer;
	NString					theString;
	base64_encodestate		theState;
	char					*dataPtr;
	NIndex					dataSize;



	// Get the state we need
	base64_init_encodestate(&theState);

	if (!theBuffer.SetSize(theValue.GetSize() * 2))
		return(theString);



	// Encode the value
	dataPtr   = (char *) theBuffer.GetData();
	dataSize  = base64_encode_block((const char *) theValue.GetData(), theValue.GetSize(), dataPtr, &theState);
	theString = NString(dataPtr, dataSize);

	dataSize = base64_encode_blockend(dataPtr, &theState);
	if (dataSize != 0)
		theString += NString(dataPtr, dataSize);



	// Remove the trailing newline
	NN_ASSERT(theString.GetRight(1) == "\n");
	theString.TrimRight(1);

	return(theString);
}





//============================================================================
//		NB64Encoder::Decode : Decode from Base64.
//----------------------------------------------------------------------------
NData NB64Encoder::Decode(const NString &theValue)
{	base64_decodestate		theState;
	char					*dataPtr;
	NIndex					dataSize;
	NData					theData;



	// Get the state we need
	base64_init_decodestate(&theState);

	if (!theData.SetSize(theValue.GetSize()))
		return(theData);



	// Decode the value
	dataPtr  = (char *) theData.GetData();
	dataSize = base64_decode_block(theValue.GetUTF8(), theValue.GetSize(), dataPtr, &theState);

	theData.SetSize(dataSize);

	return(theData);
}



