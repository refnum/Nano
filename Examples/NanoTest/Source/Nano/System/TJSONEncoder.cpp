/*	NAME:
		TJSONEncoder.cpp

	DESCRIPTION:
		NJSONEncoder tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NJSONEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestBooleanKey								= "BooleanValue";
static const NString kTestIntegerKey								= "IntegerValue";
static const NString kTestRealKey									= "RealValue";
static const NString kTestStringKey									= "StringValue";
static const NString kTestArrayKey									= "ArrayValue";
static const NString kTestDictionaryKey								= "DictionaryValue";

static const bool      kValueBoolean1								= true;
static const bool      kValueBoolean2								= false;
static const int32_t   kValueInteger1								= 123;
static const int32_t   kValueInteger2								= 456;
static const float32_t kValueReal1									= 100.5;
static const float32_t kValueReal2									= 200.5;
static const NString   kValueString1								= "This\t is\n a string";
static const NString   kValueString2								= "This is another string";

static const NString kTestJSON										=	"{\n"
																		"	\"RealValue\": 100.5,\n"
																		"	\"ArrayValue\": [\n"
																		"		false,\n"
																		"		456,\n"
																		"		200.5,\n"
																		"		\"This is another string\"\n"
																		"	],\n"
																		"	\"StringValue\": \"This\\t is\\n a string\",\n"
																		"	\"BooleanValue\": true,\n"
																		"	\"IntegerValue\": 123,\n"
																		"	\"DictionaryValue\": {\n"
																		"		\"RealValue\": 200.5,\n"
																		"		\"ArrayValue\": [ ],\n"
																		"		\"StringValue\": \"This is another string\",\n"
																		"		\"BooleanValue\": false,\n"
																		"		\"IntegerValue\": 456,\n"
																		"		\"DictionaryValue\": { }\n"
																		"	}\n"
																		"}";





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NJSONENCODER(...)										TEST_NANO(TJSONEncoder, ##__VA_ARGS__)

FIXTURE_NANO(TJSONEncoder)
{
	NDictionary		theDictionary1, theDictionary2;
	NJSONEncoder	theEncoder;
	NArray			theArray;
	NString			theJSON;
	bool			wasOK;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NJSONENCODER("Encoding")
{


	// Perform the test
	theArray.AppendValue(kValueBoolean2);
	theArray.AppendValue(kValueInteger2);
	theArray.AppendValue(kValueReal2);
	theArray.AppendValue(kValueString2);

	theDictionary2.SetValue(kTestBooleanKey,	kValueBoolean2);
	theDictionary2.SetValue(kTestIntegerKey,	kValueInteger2);
	theDictionary2.SetValue(kTestRealKey,		kValueReal2);
	theDictionary2.SetValue(kTestStringKey,		kValueString2);
	theDictionary2.SetValue(kTestArrayKey,		NArray());
	theDictionary2.SetValue(kTestDictionaryKey,	NDictionary());

	theDictionary1.SetValue(kTestBooleanKey,	kValueBoolean1);
	theDictionary1.SetValue(kTestIntegerKey,	kValueInteger1);
	theDictionary1.SetValue(kTestRealKey,		kValueReal1);
	theDictionary1.SetValue(kTestStringKey,		kValueString1);
	theDictionary1.SetValue(kTestArrayKey,		theArray);
	theDictionary1.SetValue(kTestDictionaryKey,	theDictionary2);

	theJSON = theEncoder.Encode(theDictionary1);
	REQUIRE(theJSON == kTestJSON);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NJSONENCODER("Decoding")
{


	// Perform the test
	wasOK = theEncoder.Decode(kTestJSON).GetValue(theDictionary1);
	REQUIRE(wasOK);

	theDictionary2 = theDictionary1.GetValueDictionary(	kTestDictionaryKey);
	theArray       = theDictionary1.GetValueArray(		kTestArrayKey);

	REQUIRE(theDictionary1.GetValueBoolean(		kTestBooleanKey)	== kValueBoolean1);
	REQUIRE(theDictionary1.GetValueInt32(		kTestIntegerKey)	== kValueInteger1);
	REQUIRE(theDictionary1.GetValueString(		kTestStringKey)		== kValueString1);
	REQUIRE(theDictionary1.GetValueArray(		kTestArrayKey)		== theArray);
	REQUIRE(theDictionary1.GetValueDictionary(	kTestDictionaryKey)	== theDictionary2);
	REQUIRE(NMathUtilities::AreEqual(theDictionary1.GetValueFloat32(kTestRealKey), kValueReal1));

	REQUIRE(theDictionary2.GetValueBoolean(		kTestBooleanKey)	== kValueBoolean2);
	REQUIRE(theDictionary2.GetValueInt32(		kTestIntegerKey)	== kValueInteger2);
	REQUIRE(theDictionary2.GetValueString(		kTestStringKey)		== kValueString2);
	REQUIRE(theDictionary2.GetValueArray(		kTestArrayKey).IsEmpty());
	REQUIRE(theDictionary2.GetValueDictionary(	kTestDictionaryKey).IsEmpty());
	REQUIRE(NMathUtilities::AreEqual(theDictionary2.GetValueFloat32(kTestRealKey), kValueReal2));

	REQUIRE(theArray.GetValueBoolean(0) == kValueBoolean2);
	REQUIRE(theArray.GetValueInt32(1)   == kValueInteger2);
	REQUIRE(theArray.GetValueString(3)  == kValueString2);
	REQUIRE(NMathUtilities::AreEqual(theArray.GetValueFloat32(2), kValueReal2));
}



