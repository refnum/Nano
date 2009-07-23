/*	NAME:
		NEncoder.h

	DESCRIPTION:
		Object encoder.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NENCODER_HDR
#define NENCODER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NEncodable.h"
#include "NXMLNode.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Keys
static const NString kNEncoderValueKey							= "value";


// Encoder state
typedef enum {
	kNEncoderIdle,
	kNEncoderEncoding,
	kNEncoderDecoding,
} NEncoderState;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NEncoder {
public:
										 NEncoder(void);
	virtual								~NEncoder(void);


	// Encode/decode an object
	NData								Encode(const NEncodable &theObject);
	void								Decode(      NEncodable &theObject, const NData &theData);


	// Encode/decode a value
	//
	// Invoked by NEncodable::EncodeSelf/DecodeSelf.
	void								EncodeBoolean(const NString &theKey, bool    theValue);
	void								EncodeSInt32( const NString &theKey, SInt32  theValue);
	void								EncodeSInt64( const NString &theKey, SInt64  theValue);
	void								EncodeFloat32(const NString &theKey, Float32 theValue);
	void								EncodeFloat64(const NString &theKey, Float64 theValue);
	void								EncodeBytes(  const NString &theKey, NIndex theSize, const void       *theValue);
	void								EncodeUTF8(   const NString &theKey, NIndex theSize, const char       *theValue);
	void								EncodeObject( const NString &theKey,                 const NEncodable &theValue);

	bool								DecodeBoolean(const NString &theKey);
	SInt32								DecodeSInt32( const NString &theKey);
	SInt64								DecodeSInt64( const NString &theKey);
	Float32								DecodeFloat32(const NString &theKey);
	Float64								DecodeFloat64(const NString &theKey);
	void								DecodeBytes(  const NString &theKey, NIndex *theSize, void **theValue);
	void								DecodeUTF8(   const NString &theKey, NIndex *theSize, char **theValue);
	void								DecodeObject( const NString &theKey, NEncodable &theValue);


private:
	NXMLNode							*GetParentNode(void);

	NXMLNode							*EncodeChild(const NString &theName, const NString &theKey, const NString &theValue);


private:
	NEncoderState						mState;
	NXMLNode							*mNodeRoot;
	NXMLNodeList						mNodeStack;
};





#endif // NENCODER_HDR
