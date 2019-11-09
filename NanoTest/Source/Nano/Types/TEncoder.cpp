/*	NAME:
		TEncoder.cpp

	DESCRIPTION:
		NEncoder tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDataDigest.h"
#include "NTestFixture.h"
#include "NEncodable.h"
#include "NEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Keys
static const NString kKeyBoolean1									= "Test Boolean 1";
static const NString kKeyBoolean2									= "Test Boolean 2";
static const NString kKeyNumber1									= "Test Number 1";
static const NString kKeyNumber2									= "Test Number 2";
static const NString kKeyNumber3									= "Test Number 3";
static const NString kKeyNumber4									= "Test Number 4";
static const NString kKeyString										= "Test String";
static const NString kKeyData										= "Test Data";
static const NString kKeyArray										= "Test Array";
static const NString kKeyDictionary									= "Test Dictionary";
static const NString kKeyPoint										= "Test Point";
static const NString kKeySize										= "Test Size";
static const NString kKeyRectangle									= "Test Rectangle";


// Values
static const bool       kValueBoolean1								= true;
static const bool       kValueBoolean2								= false;
static const NNumber    kValueNumber1								=  1234LL;
static const NNumber    kValueNumber2								= -5678LL;
static const NNumber    kValueNumber3								=  1234.12f;
static const NNumber    kValueNumber4								= -5678.5678765000102;
static const NString    kValueString								= "This \" is & a ' test < text > string";
static const uint8_t    kValueData[]								= { 0xAA, 0xBB, 0xCC, 0xDD };
static const NPoint		kValuePoint									= NPoint(10, 20);
static const NSize		kValueSize									= NSize(30, 40);
static const NRectangle kValueRectangle								= NRectangle(10, 20, 30, 40);


// Results
static const uint32_t kResultBinary									= 0x0F48C7C8;
static const NString  kResultXML									=	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
																		"<encoder version=\"1.0\">\n"
																		"	<object class=\"TEncodable\" key=\"root\">\n"
																		"		<bool key=\"Test Boolean 1\">true</bool>\n"
																		"		<bool key=\"Test Boolean 2\">false</bool>\n"
																		"		<number key=\"Test Number 1\">1234</number>\n"
																		"		<number key=\"Test Number 2\">-5678</number>\n"
																		"		<number key=\"Test Number 3\">1234.12</number>\n"
																		"		<number key=\"Test Number 4\">-5678.5678765000102</number>\n"
																		"		<string key=\"Test String\">This &quot; is &amp; a &apos; test &lt; text &gt; string</string>\n"
																		"		<data key=\"Test Data\">qrvM3Q==</data>\n"
																		"		<object class=\"NArray\" key=\"Test Array\">\n"
																		"			<bool key=\"0\">true</bool>\n"
																		"			<number key=\"1\">1234</number>\n"
																		"			<string key=\"2\">This &quot; is &amp; a &apos; test &lt; text &gt; string</string>\n"
																		"		</object>\n"
																		"		<object class=\"NDictionary\" key=\"Test Dictionary\">\n"
																		"			<object class=\"NSize\" key=\"Test Size\">\n"
																		"				<number key=\"width\">30</number>\n"
																		"				<number key=\"height\">40</number>\n"
																		"			</object>\n"
																		"			<object class=\"NPoint\" key=\"Test Point\">\n"
																		"				<number key=\"x\">10</number>\n"
																		"				<number key=\"y\">20</number>\n"
																		"			</object>\n"
																		"			<string key=\"Test String\">This &quot; is &amp; a &apos; test &lt; text &gt; string</string>\n"
																		"			<number key=\"Test Number 1\">1234</number>\n"
																		"			<bool key=\"Test Boolean 1\">true</bool>\n"
																		"			<object class=\"NRectangle\" key=\"Test Rectangle\">\n"
																		"				<number key=\"x\">10</number>\n"
																		"				<number key=\"y\">20</number>\n"
																		"				<number key=\"width\">30</number>\n"
																		"				<number key=\"height\">40</number>\n"
																		"			</object>\n"
																		"		</object>\n"
																		"	</object>\n"
																		"</encoder>\n";





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
class TEncodable :	public NEncodable,
					public NComparable<TEncodable> {
public:
	NENCODABLE_DECLARE(TEncodable);


	// Encode the object
	void EncodeSelf(NEncoder &theEncoder) const
	{	NArray			theArray;
		NDictionary		theDict;
		NData			theData;
		
		theData = NData(NN_ARRAY_SIZE(kValueData), kValueData);

		theArray.AppendValue(kValueBoolean1);
		theArray.AppendValue(kValueNumber1);
		theArray.AppendValue(kValueString);

		theDict.SetValue(kKeyBoolean1,  kValueBoolean1);
		theDict.SetValue(kKeyNumber1,   kValueNumber1);
		theDict.SetValue(kKeyString,    kValueString);
		theDict.SetValue(kKeyPoint,     kValuePoint);
		theDict.SetValue(kKeySize,      kValueSize);
		theDict.SetValue(kKeyRectangle, kValueRectangle);
	
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
		NN_ASSERT(theData.GetSize() == NN_ARRAY_SIZE(kValueData));
		NN_ASSERT(memcmp(theData.GetData(), kValueData, (size_t) theData.GetSize()) == 0);

		NN_ASSERT(theEncoder.DecodeObject(kKeyArray).GetValue(theArray));
		NN_ASSERT(theArray.GetSize() == 3);
		NN_ASSERT(theArray.GetValueBoolean(0) == kValueBoolean1);
		NN_ASSERT(theArray.GetValue       (1) == kValueNumber1);
		NN_ASSERT(theArray.GetValueString (2) == kValueString);

		NN_ASSERT(theEncoder.DecodeObject(kKeyDictionary).GetValue(theDict));
		NN_ASSERT(theDict.GetSize() == 6);
		NN_ASSERT(theDict.GetValueBoolean  (kKeyBoolean1)  == kValueBoolean1);
		NN_ASSERT(theDict.GetValue         (kKeyNumber1)   == kValueNumber1);
		NN_ASSERT(theDict.GetValueString   (kKeyString)    == kValueString);
		NN_ASSERT(theDict.GetValuePoint    (kKeyPoint)     == kValuePoint);
		NN_ASSERT(theDict.GetValueSize     (kKeySize)      == kValueSize);
		NN_ASSERT(theDict.GetValueRectangle(kKeyRectangle) == kValueRectangle);
	}
	
	
	// Compare the value
	NComparison Compare(const TEncodable &theValue) const
	{
		return(GetComparison(this, &theValue));
	}
};

NENCODABLE_DEFINE(TEncodable);





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NENCODER(...)											TEST_NANO(TEncoder, ##__VA_ARGS__)

FIXTURE_NANO(TEncoder)
{
	NEncoder		theEncoder;
	TEncodable		theObject;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NENCODER("Encoding")
{	NData			dataXML, dataBinary;
	uint32_t		adlerData;
	NDataDigest		theDigest;
	NVariant		theValue;
	NString			textXML;



	// Perform the test
	dataXML    = theEncoder.Encode(theObject, kNEncoderXML);
	dataBinary = theEncoder.Encode(theObject, kNEncoderBinary);

	textXML   = NString(dataXML);
	adlerData = theDigest.GetAdler32(dataBinary);

	REQUIRE(textXML   == kResultXML);
	REQUIRE(adlerData == kResultBinary);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NENCODER("Decoding")
{	NData		dataXML, dataBinary;
	NVariant	theValue;



	// Decoding
	dataXML    = theEncoder.Encode(theObject, kNEncoderXML);
	dataBinary = theEncoder.Encode(theObject, kNEncoderBinary);

	theValue = theEncoder.Decode(dataXML);
	REQUIRE(theValue.GetValue(theObject));
	
	theValue = theEncoder.Decode(dataBinary);
	REQUIRE(theValue.GetValue(theObject));
}


