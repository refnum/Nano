/*	NAME:
		NDataEncoder.cpp

	DESCRIPTION:
		Data encoder.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "cencode.h"
#include "cdecode.h"

#include "NDataEncoder.h"





//============================================================================
//		NDataEncoder::NDataEncoder : Constructor.
//----------------------------------------------------------------------------
NDataEncoder::NDataEncoder(void)
{
}





//============================================================================
//		NDataEncoder::~NDataEncoder : Destructor.
//----------------------------------------------------------------------------
NDataEncoder::~NDataEncoder(void)
{
}





//============================================================================
//		NDataEncoder::Encode : Encode to text.
//----------------------------------------------------------------------------
NString NDataEncoder::Encode(const NData &theValue, NDataEncoding theEncoding)
{	NString		theResult;



	// Encode the data
	switch (theEncoding) {
		case kNDataEncodingHex:
			theResult = Hex_Encode(theValue);
			break;

		case kNDataEncodingB64:
			theResult = B64_Encode(theValue);
			break;

		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NDataEncoder::Decode : Decode from text.
//----------------------------------------------------------------------------
NData NDataEncoder::Decode(const NString &theValue, NDataEncoding theEncoding)
{	NData	theResult;



	// Decode the string
	switch (theEncoding) {
		case kNDataEncodingHex:
			theResult = Hex_Decode(theValue);
			break;

		case kNDataEncodingB64:
			theResult = B64_Decode(theValue);
			break;

		default:
			NN_LOG("Unknown encoding: %d", theEncoding);
			break;
		}
	
	return(theResult);
}





//============================================================================
//		NDataEncoder::Hex_Encode : Encode to hex.
//----------------------------------------------------------------------------
#pragma mark -
NString NDataEncoder::Hex_Encode(const NData &theValue)
{	NIndex			n, theSize;
	NData			tmpBuffer;
	NString			theResult;
	const UInt8		*dataPtr;
	char			*textPtr;



	// Get the state we need
	theSize = theValue.GetSize();
	dataPtr = theValue.GetData();
	textPtr = (char *) tmpBuffer.AppendData((theSize * 2) + 1);

	if (theSize == 0 || dataPtr == NULL || textPtr == NULL)
		return(theResult);



	// Convert to a string
	for (n = 0; n < theSize; n++)
		{
		sprintf(textPtr, "%.2X", dataPtr[n]);
		textPtr += 2;
		}
	
	theResult = NString(tmpBuffer);
	
	return(theResult);
}





//============================================================================
//		NDataEncoder::Hex_Decode : Decode from hex.
//----------------------------------------------------------------------------
NData NDataEncoder::Hex_Decode(const NString &theValue)
{	NIndex			n, theSize;
	NData			theResult;
	UInt8			*dataPtr;
	const char		*textPtr;
	unsigned int	byteVal;



	// Validate our parameters
	NN_ASSERT(theValue.IsEmpty() || (theValue.GetSize() % 2) == 0);



	// Get the state we need
	theSize = theValue.GetSize() / 2;
	dataPtr = theResult.AppendData(theSize);
	textPtr = theValue.GetUTF8();

	if (theSize == 0 || dataPtr == NULL || textPtr == NULL)
		return(theResult);



	// Convert the string
	//
	// Visual Studio does not support 'hh' correctly, so we need to read byte
	// values into a temporary variable:
	//
	// https://connect.microsoft.com/VisualStudio/feedback/details/416843/sscanf-cannot-not-handle-hhd-format
	for (n = 0; n < theSize; n++)
		{
		sscanf(textPtr, "%2x", &byteVal);
		NN_ASSERT((byteVal & 0xFFFFFF00) == 0);

		dataPtr[n] = (UInt8) byteVal;
		textPtr   += 2;
		}
	
	return(theResult);
}





//============================================================================
//		NDataEncoder::B64_Encode : Encode to Base64.
//----------------------------------------------------------------------------
NString NDataEncoder::B64_Encode(const NData &theValue)
{	NData					theBuffer;
	NString					theString;
	base64_encodestate		theState;
	char					*dataPtr;
	NIndex					dataSize;



	// Get the state we need
	base64_init_encodestate(&theState);

	if (theValue.IsEmpty() || !theBuffer.SetSize(theValue.GetSize() * 2))
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
//		NDataEncoder::B64_Decode : Decode from Base64.
//----------------------------------------------------------------------------
NData NDataEncoder::B64_Decode(const NString &theValue)
{	NData					theData, valueData;
	base64_decodestate		theState;
	char					*dataPtr;
	NIndex					dataSize;



	// Get the state we need
	base64_init_decodestate(&theState);

	valueData = theValue.GetData(kNStringEncodingUTF8);
	
	if (!theData.SetSize(valueData.GetSize()))
		return(theData);



	// Decode the value
	dataPtr  = (char *) theData.GetData();
	dataSize = base64_decode_block((const char *) valueData.GetData(), valueData.GetSize(), dataPtr, &theState);

	theData.SetSize(dataSize);

	return(theData);
}



