/*	NAME:
		NJSONEncoder.h

	DESCRIPTION:
		JSON encoder.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NJSONENCODER_HDR
#define NJSONENCODER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Structs
typedef struct _json_value *json_value_ptr;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NJSONEncoder {
public:
										NJSONEncoder(void);
	virtual							   ~NJSONEncoder(void);


	// Encode/decode a JSON document
	//
	// If an error occurs during parsing, an empty string/NULL node is returned.
	NString								Encode(const NVariant &theValue);
	NVariant							Decode(const NString  &theJSON);


private:
	NString								EncodeValue(     const NVariant    &theValue, const NString &theIndent);
	NString								EncodeBoolean(         bool         theValuet);
	NString								EncodeNumber(    const NNumber     &theValuet);
	NString								EncodeString(    const NString     &theValuet);
	NString								EncodeArray(     const NArray      &theValue, const NString &theIndent);
	NString								EncodeDictionary(const NDictionary &theValue, const NString &theIndent);

	NVariant							DecodeValue(     const json_value_ptr jsonValue);
	bool								DecodeBoolean(   const json_value_ptr jsonValue);
	SInt64								DecodeInteger(   const json_value_ptr jsonValue);
	Float64								DecodeReal(      const json_value_ptr jsonValue);
	NString								DecodeString(    const json_value_ptr jsonValue);
	NArray								DecodeArray(     const json_value_ptr jsonValue);
	NDictionary							DecodeDictionary(const json_value_ptr jsonValue);


private:

};



#endif // NJSONENCODER_HDR
