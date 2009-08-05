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
#include "NChecksum.h"
#include "NEncoder.h"

#include "TEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Keys
static const NString kKeyBoolean1							= "Test Boolean 1";
static const NString kKeyBoolean2							= "Test Boolean 2";
static const NString kKeyNumber1							= "Test Number 1";
static const NString kKeyNumber2							= "Test Number 2";
static const NString kKeyNumber3							= "Test Number 3";
static const NString kKeyNumber4							= "Test Number 4";
static const NString kKeyString								= "Test String";
static const NString kKeyData								= "Test Data";
static const NString kKeyArray								= "Test Array";
static const NString kKeyDictionary							= "Test Dictionary";



// Values
static const bool    kValueBoolean1						= true;
static const bool    kValueBoolean2						= false;
static const NNumber kValueNumber1						= NVariant( 1234);
static const NNumber kValueNumber2						= NVariant(-5678);
static const NNumber kValueNumber3						= NVariant( 1234.123);
static const NNumber kValueNumber4						= NVariant(-5678.567);
static const NString kValueString						= "This is a string";
static const UInt8   kValueData[]						= { 0xAA, 0xBB, 0xCC, 0xDD };


// Results
static const UInt32  kResultBinary						= 0xA24D90C3;
static const NString kResultXML							=	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
															"<encoder version=\"1.0\">\n"
															"	<object class=\"TEncodable\" key=\"root\">\n"
															"		<bool key=\"Test Boolean 1\">true</bool>\n"
															"		<bool key=\"Test Boolean 2\">false</bool>\n"
															"		<number key=\"Test Number 1\">1234</number>\n"
															"		<number key=\"Test Number 2\">-5678</number>\n"
															"		<number key=\"Test Number 3\">1234.123</number>\n"
															"		<number key=\"Test Number 4\">-5678.567</number>\n"
															"		<string key=\"Test String\">This is a string</string>\n"
															"		<data key=\"Test Data\">qrvM3Q==</data>\n"
															"		<object class=\"NArray\" key=\"Test Array\">\n"
															"			<bool key=\"0\">true</bool>\n"
															"			<number key=\"1\">1234</number>\n"
															"			<string key=\"2\">This is a string</string>\n"
															"		</object>\n"
															"		<object class=\"NDictionary\" key=\"Test Dictionary\">\n"
															"			<number key=\"Test Number 1\">1234</number>\n"
															"			<bool key=\"Test Boolean 1\">true</bool>\n"
															"			<string key=\"Test String\">This is a string</string>\n"
															"		</object>\n"
															"	</object>\n"
															"</encoder>\n";





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
class TEncodable :	public NEncodable,
					public NComparable<TEncodable> {
public:
	DECLARE_NENCODABLE(TEncodable);


	// Encode the object
	void EncodeSelf(NEncoder &theEncoder) const
	{	NArray			theArray;
		NDictionary		theDict;
		NData			theData;
		
		theData = NData(GET_ARRAY_SIZE(kValueData), kValueData);

		theArray.AppendValue(kValueBoolean1);
		theArray.AppendValue(kValueNumber1);
		theArray.AppendValue(kValueString);

		theDict.SetValue(kKeyBoolean1, kValueBoolean1);
		theDict.SetValue(kKeyNumber1,  kValueNumber1);
		theDict.SetValue(kKeyString,   kValueString);
	
		theEncoder.EncodeBoolean(kKeyBoolean1,   kValueBoolean1);
		theEncoder.EncodeBoolean(kKeyBoolean2,   kValueBoolean2);
		theEncoder.EncodeNumber( kKeyNumber1,    kValueNumber1);
		theEncoder.EncodeNumber( kKeyNumber2,    kValueNumber2);
		theEncoder.EncodeNumber( kKeyNumber3,    kValueNumber3);
		theEncoder.EncodeNumber( kKeyNumber4,    kValueNumber4);
		theEncoder.EncodeString( kKeyString,     kValueString);
		theEncoder.EncodeData(   kKeyData,       theData);
		theEncoder.EncodeObject( kKeyArray,      theArray);
		theEncoder.EncodeObject( kKeyDictionary, theDict);
	}


	// Decode the object
	void DecodeSelf(const NEncoder &theEncoder)
	{	NArray			theArray;
		NDictionary		theDict;
		NData			theData;

		NN_ASSERT(theEncoder.DecodeBoolean(kKeyBoolean1) == kValueBoolean1);
		NN_ASSERT(theEncoder.DecodeBoolean(kKeyBoolean2) == kValueBoolean2);
		NN_ASSERT(theEncoder.DecodeNumber( kKeyNumber1)  == kValueNumber1);
		NN_ASSERT(theEncoder.DecodeNumber( kKeyNumber2)  == kValueNumber2);
		NN_ASSERT(theEncoder.DecodeNumber( kKeyNumber3)  == kValueNumber3);
		NN_ASSERT(theEncoder.DecodeNumber( kKeyNumber4)  == kValueNumber4);
		NN_ASSERT(theEncoder.DecodeString( kKeyString)   == kValueString);

		theData = theEncoder.DecodeData(kKeyData);
		NN_ASSERT(theData.GetSize() == GET_ARRAY_SIZE(kValueData));
		NN_ASSERT(memcmp(theData.GetData(), kValueData, theData.GetSize()) == 0);

		NN_ASSERT(theEncoder.DecodeObject(kKeyArray).GetValue(theArray));
		NN_ASSERT(theArray.GetSize() == 3);
		NN_ASSERT(theArray.GetValueBoolean(0) == kValueBoolean1);
		NN_ASSERT(theArray.GetValue       (1) == kValueNumber1);
		NN_ASSERT(theArray.GetValueString (2) == kValueString);

		NN_ASSERT(theEncoder.DecodeObject(kKeyDictionary).GetValue(theDict));
		NN_ASSERT(theDict.GetSize() == 3);
		NN_ASSERT(theDict.GetValueBoolean(kKeyBoolean1) == kValueBoolean1);
		NN_ASSERT(theDict.GetValue       (kKeyNumber1)  == kValueNumber1);
		NN_ASSERT(theDict.GetValueString (kKeyString)   == kValueString);
	}
	
	
	// Compare the value
	NComparison Compare(const TEncodable &theValue) const
	{
		return(GetComparison(this, &theValue));
	}

};

DEFINE_NENCODABLE(TEncodable);





//============================================================================
//		TEncoder::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TEncoder::Execute(void)
{	NData			dataXML, dataBinary;
	NEncoder		theEncoder;
	TEncodable		theObject;
	UInt32			adlerData;
	NChecksum		checkSum;
	NVariant		theValue;
	NString			textXML;
	NStatus			theErr;



	// Encoding
	dataXML    = theEncoder.Encode(theObject, kNEncoderXML);
	dataBinary = theEncoder.Encode(theObject, kNEncoderBinary);

	textXML   = NString(dataXML);
	adlerData = checkSum.GetAdler32(dataBinary.GetSize(), dataBinary.GetData());

	NN_ASSERT(textXML   == kResultXML);
	NN_ASSERT(adlerData == kResultBinary);



	// Decoding
	theErr = theEncoder.Decode(theObject, dataXML);
	NN_ASSERT(theErr == kNoErr);

	theErr = theEncoder.Decode(theObject, dataBinary);
	NN_ASSERT(theErr == kNoErr);
}



