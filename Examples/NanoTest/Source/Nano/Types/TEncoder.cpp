/*	NAME:
		TEncoder.cpp

	DESCRIPTION:
		NEncoder tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NEncodable.h"
#include "NEncoder.h"

#include "TEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8   kTestData[]						= { 0xAA, 0xBB, 0xCC, 0xDD };
static const NString kTestString1						= "This is the first string";
static const NString kTestString2						= "This is the second string";





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
class TEncodable : public NEncodable {
	// Get the encoder class name
	NString GetEncoderClass(void) const
	{
		return("TEncodable");
	}





	// Encode the object
	void EncodeSelf(NEncoder &theEncoder) const
	{	
		theEncoder.EncodeBoolean("TestBool1", false);
		theEncoder.EncodeBoolean("TestBool2", true);
		
		theEncoder.EncodeSInt32("TestSInt32",  1234);
		theEncoder.EncodeSInt32("TestSInt64", -5678);

		theEncoder.EncodeFloat32("TestFloat32",  1234.1234);
		theEncoder.EncodeFloat32("TestFloat64", -5678.5678);
		
		theEncoder.EncodeBytes( "TestData",    GET_ARRAY_SIZE(kTestData), kTestData);
		theEncoder.EncodeUTF8(  "TestString1", strlen(kTestString1.GetUTF8()), kTestString1.GetUTF8());
		theEncoder.EncodeObject("TestString2", kTestString2);
	}





	// Decode the object
	void DecodeSelf(const NEncoder &theEncoder)
	{
	
		   
	}
};





//============================================================================
//		TEncoder::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TEncoder::Execute(void)
{	NEncoder		theEncoder;
	TEncodable		theObject;
	NString			theText;
	NData			theData;



	// Encoding
	theData = theEncoder.Encode(theObject);
	theText.SetData(theData);
	
	NN_LOG("[%@]", theText);



	// Decoding



}


