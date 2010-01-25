/*	NAME:
		TPropertyList.cpp

	DESCRIPTION:
		NPropertyList tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NPropertyList.h"

#include "TPropertyList.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Keys
static const NString kKeyDictionary							= "Test Dictionary";
static const NString kKeyArray								= "Test Array";
static const NString kKeyString								= "Test String";
static const NString kKeyBoolean							= "Test Boolean";
static const NString kKeyNumber								= "Test Number";
static const NString kKeyData								= "Test Data";
static const NString kKeyDate								= "Test Date";
static const NString kKeyColor								= "Test Color";


// Values
static const NString kValueString1							= "First String";
static const NString kValueString2							= "Second String";
static const bool    kValueBoolean1							= true;
static const bool    kValueBoolean2							= false;
static const SInt64  kValueNumber1							= -2342;
static const Float64 kValueNumber2							= kNPi;
static const UInt8   kValueData1[]							= { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A };
static const UInt8   kValueData2[]							= { 0x3C, 0xE7, 0xC7, 0x32, 0xE3, 0xD8, 0x52 };
static const NDate   kValueDate1							= NDate(-886538221);
static const NDate   kValueDate2							= NDate( 268617632);
static const NColor  kValueColor1							= NColor(1.0f, 0.8f, 0.4f, 0.2f);
static const NColor  kValueColor2							= NColor(0.1f, 0.2f, 0.3f, 0.4f);


// XML
static const NString kTestXML								=	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
																"<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
																"<plist version=\"1.0\">\n"
																"<dict>\n"
																"	<key>Test Array</key>\n"
																"	<array>\n"
																"		<string>First String</string>\n"
																"		<true/>\n"
																"		<integer>-2342</integer>\n"
																"		<date>1972-11-28T03:22:59Z</date>\n"
																"		<data>TmVOYzAwMDEAAADzeNqzsa/IzVEoSy0qzszPs1Uy1DNQUkjNS85PycxLt1UKDXHTtVCyt+Oy\n"
																"AYulFqGqtOPitMlPykpNLlFIzkksLrZV8nPOz8kvUlLITq20VSrKzy8BqeG0ySvNTQJqhogq\n"
																"2Rna6ENEMCTTlewM9CxwSieBpE1wSieCpI2QpG30Ic4DekAf6gM7LgDQ+UpN</data>\n"
																"		<data>iVBORw0KGg==</data>\n"
																"		<dict>\n"
																"			<key>Test Boolean</key>\n"
																"			<false/>\n"
																"			<key>Test Color</key>\n"
																"			<data>TmVOYzAwMDEAAAD1eNqzsa/IzVEoSy0qzszPs1Uy1DNQUkjNS85PycxLt1UKDXHTtVCyt+Oy\n"
																"AYulFqGqtOPitMlPykpNLlFIzkksLrZV8nPOz8kvUlLITq20VSrKzy8BqeG0ySvNTQJqhogq\n"
																"2RnoGdroQ8QwpNNB0kY4pZNA0sY4pRNB0iZI0jb6EAcCvaAP9YMdFwBGsEqm</data>\n"
																"			<key>Test Number</key>\n"
																"			<real>3.1415926535897931</real>\n"
																"			<key>Test String</key>\n"
																"			<string>Second String</string>\n"
																"		</dict>\n"
																"	</array>\n"
																"	<key>Test Data</key>\n"
																"	<data>POfHMuPYUg==</data>\n"
																"	<key>Test Date</key>\n"
																"	<date>2009-07-07T00:00:32Z</date>\n"
																"	<key>Test Dictionary</key>\n"
																"	<dict>\n"
																"		<key>Test Boolean</key>\n"
																"		<false/>\n"
																"		<key>Test Color</key>\n"
																"		<data>TmVOYzAwMDEAAAD1eNqzsa/IzVEoSy0qzszPs1Uy1DNQUkjNS85PycxLt1UKDXHTtVCyt+Oy\n"
																"AYulFqGqtOPitMlPykpNLlFIzkksLrZV8nPOz8kvUlLITq20VSrKzy8BqeG0ySvNTQJqhogq\n"
																"2RnoGdroQ8QwpNNB0kY4pZNA0sY4pRNB0iZI0jb6EAcCvaAP9YMdFwBGsEqm</data>\n"
																"		<key>Test Number</key>\n"
																"		<real>3.1415926535897931</real>\n"
																"		<key>Test String</key>\n"
																"		<string>Second String</string>\n"
																"	</dict>\n"
																"</dict>\n"
																"</plist>\n";





//============================================================================
//		TPropertyList::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TPropertyList::Execute(void)
{	NData				testData1, testData2;
	NDictionary			testDict1, testDict2;
	NString				theXML, testString;
	NArray				testArray;
	NDate				testDate;
	NPropertyList		pList;



	// Get the state we need
	testData1 = NData(NN_ARRAY_SIZE(kValueData1), kValueData1);
	testData2 = NData(NN_ARRAY_SIZE(kValueData2), kValueData2);

	testDict2.SetValue(kKeyString,  kValueString2);
	testDict2.SetValue(kKeyBoolean, kValueBoolean2);
	testDict2.SetValue(kKeyColor,   kValueColor2);
	testDict2.SetValue(kKeyNumber,  kValueNumber2);

	testArray.AppendValue(kValueString1);
	testArray.AppendValue(kValueBoolean1);
	testArray.AppendValue(kValueNumber1);
	testArray.AppendValue(kValueDate1);
	testArray.AppendValue(kValueColor1);
	testArray.AppendValue(testData1);
	testArray.AppendValue(testDict2);

	testDict1.SetValue(kKeyDictionary, testDict2);
	testDict1.SetValue(kKeyArray,      testArray);
	testDict1.SetValue(kKeyData,       testData2);
	testDict1.SetValue(kKeyDate,       kValueDate2);



	// Encode to XML
	theXML = pList.EncodeXML(testDict1);
	NN_ASSERT(theXML == kTestXML);

	testDict1.Clear();
	testDict2.Clear();
	testData1.Clear();
	testData2.Clear();
	testArray.Clear();



	// Decode from XML
	testDict1 = pList.DecodeXML(theXML);
	NN_ASSERT(testDict1.HasKey(kKeyDictionary));
	NN_ASSERT(testDict1.HasKey(kKeyArray));
	NN_ASSERT(testDict1.HasKey(kKeyData));
	NN_ASSERT(testDict1.HasKey(kKeyDate));


	testDict2 = testDict1.GetValueDictionary(kKeyDictionary);
	NN_ASSERT(testDict2.HasKey(kKeyString));
	NN_ASSERT(testDict2.HasKey(kKeyBoolean));
	NN_ASSERT(testDict2.HasKey(kKeyNumber));
	NN_ASSERT(testDict2.GetValueString( kKeyString)  == kValueString2);
	NN_ASSERT(testDict2.GetValueBoolean(kKeyBoolean) == kValueBoolean2);
	NN_ASSERT(testDict2.GetValueColor(  kKeyColor)   == kValueColor2);
	NN_ASSERT(NMathUtilities::AreEqual(testDict2.GetValueFloat64(kKeyNumber), kValueNumber2));


	testArray = testDict1.GetValueArray(kKeyArray);
	NN_ASSERT(testArray.GetSize() == 7);
	NN_ASSERT(testArray.GetValueString (0) == kValueString1);
	NN_ASSERT(testArray.GetValueBoolean(1) == kValueBoolean1);
	NN_ASSERT(testArray.GetValueSInt64 (2) == kValueNumber1);
	NN_ASSERT(testArray.GetValueDate   (3) == kValueDate1);
	NN_ASSERT(testArray.GetValueColor  (4) == kValueColor1);

	testData1 = testArray.GetValueData(5);
	NN_ASSERT(testData1.GetSize() == NN_ARRAY_SIZE(kValueData1));
	NN_ASSERT(memcmp(testData1.GetData(), kValueData1, NN_ARRAY_SIZE(kValueData1)) == 0);

	testDict2 = testArray.GetValueDictionary(6);
	NN_ASSERT(testDict2.HasKey(kKeyString));
	NN_ASSERT(testDict2.HasKey(kKeyBoolean));
	NN_ASSERT(testDict2.HasKey(kKeyNumber));
	NN_ASSERT(testDict2.GetValueString( kKeyString)  == kValueString2);
	NN_ASSERT(testDict2.GetValueBoolean(kKeyBoolean) == kValueBoolean2);
	NN_ASSERT(NMathUtilities::AreEqual(testDict2.GetValueFloat64(kKeyNumber), kValueNumber2));


	testData2 = testDict1.GetValueData(kKeyData);
	NN_ASSERT(testData2.GetSize() == NN_ARRAY_SIZE(kValueData2));
	NN_ASSERT(memcmp(testData2.GetData(), kValueData2, NN_ARRAY_SIZE(kValueData2)) == 0);


	testDate = testDict1.GetValueDate(kKeyDate);
	NN_ASSERT(testDate == kValueDate2);
}






