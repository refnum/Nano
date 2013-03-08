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
#include "NFileUtilities.h"
#include "NPropertyList.h"
#include "NDataEncoder.h"
#include "NTask.h"

#include "TPropertyList.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Keys
static const NString kKeyDictionary									= "Test Dictionary";
static const NString kKeyArray										= "Test Array";
static const NString kKeyString										= "Test String";
static const NString kKeyBoolean									= "Test Boolean";
static const NString kKeyNumber										= "Test Number";
static const NString kKeyData										= "Test Data";
static const NString kKeyDate										= "Test Date";
static const NString kKeyColor										= "Test Color";


// Values
static const NString kValueString1									= "First String";
static const NString kValueString2									= "Second String";
static const bool    kValueBoolean1									= true;
static const bool    kValueBoolean2									= false;
static const SInt64  kValueNumber1									= -2342;
static const Float64 kValueNumber2									= kNPi;
static const UInt8   kValueData1[]									= { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A };
static const UInt8   kValueData2[]									= { 0x3C, 0xE7, 0xC7, 0x32, 0xE3, 0xD8, 0x52 };
static const NDate   kValueDate1									= NDate(-886538221);
static const NDate   kValueDate2									= NDate( 268617632);
static const NColor  kValueColor1									= NColor(1.0f, 0.8f, 0.4f, 0.2f);
static const NColor  kValueColor2									= NColor(0.1f, 0.2f, 0.3f, 0.4f);


// Property lists
static const NString kPropertyListXML								=	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
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

static const NString kPropertyListBinary							=	"62706C6973743030D4010203040506071059546573742044617465595465737420446174615F100F546573742044696374696F6E6172795A546573742041727261793341B002C7A0000000473CE7C732E3D852D408090A0B0C0D0E0F5A5465737420436F6C6F725C5465737420426F6F6C65616E5B5465737420537472696E675B54657374204E756D6265724F10994E654E6330303031000000F578DAB3B1AFC8CD51284B2D2ACECCCFB35532D433505248CD4BCE4FC9CC4BB7550A0D71D3B550B2B7E3B2018BA516A1AAB4E3E2B4C94FCA4A4D2E5148CE492C2EB655F273CECFC92F5252C84EADB4552ACACF2F01A9E1B4C92BCD4D026A86882AD919E819DAE843C430A4D341D24638A59340D2C638A51341D22648D236FA100702BDA00FF5831D170046B04AA6085D5365636F6E6420537472696E6723400921FB54442D18A7111213141516175C466972737420537472696E670913FFFFFFFFFFFFF6DA33C1CA6BBFF68000004F10994E654E6330303031000000F378DAB3B1AFC8CD51284B2D2ACECCCFB35532D433505248CD4BCE4FC9CC4BB7550A0D71D3B550B2B7E3B2018BA516A1AAB4E3E2B4C94FCA4A4D2E5148CE492C2EB655F273CECFC92F5252C84EADB4552ACACF2F01A9E1B4C92BCD4D026A86882AD919DAE843443024D395EC0CF42C704A2781A44D704A2782A48D90A46DF421CE037A401FEA033B2E00D0F94A4D4789504E470D0A1AD408090A0B180D0E1A4F10994E654E6330303031000000F578DAB3B1AFC8CD51284B2D2ACECCCFB35532D433505248CD4BCE4FC9CC4BB7550A0D71D3B550B2B7E3B2018BA516A1AAB4E3E2B4C94FCA4A4D2E5148CE492C2EB655F273CECFC92F5252C84EADB4552ACACF2F01A9E1B4C92BCD4D026A86882AD919E819DAE843C430A4D341D24638A59340D2C638A51341D22648D236FA100702BDA00FF5831D170046B04AA60823400921FB54442D1800080011001B002500370042004B0053005C006700740080008C0128012901370140014801550156015F01680204020C021502B102B20000000000000201000000000000001B000000000000000000000000000002BB";





//============================================================================
//		TPropertyList::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TPropertyList::Execute(void)
{	NData				theData, testData1, testData2;
	NDictionary			testDict1, testDict2;
	NString				theXML, testString;
	NPropertyList		propertyList;
	NDataEncoder		theEncoder;
	NArray				testArray;
	NDate				testDate;



	// Maintenance
	//
	// DumpBinary can be used to rebuild kPropertyListBinary from kPropertyListXML.
#if 0
	DumpBinary();
#endif



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
	theXML = propertyList.EncodeXML(testDict1);
	NN_ASSERT(theXML == kPropertyListXML);

	testDict1.Clear();
	testDict2.Clear();
	testData1.Clear();
	testData2.Clear();
	testArray.Clear();



	// Decode from XML
	testDict1 = propertyList.DecodeXML(theXML);
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



	// Encode to binary
	testDict1 = propertyList.DecodeXML(kPropertyListXML);
	theData   = propertyList.Encode(testDict1, kNPropertyListBinary);

	testDict2 = propertyList.Decode(theData);
	theXML    = propertyList.EncodeXML(testDict2);
	NN_ASSERT(theXML == kPropertyListXML);



	// Decode from binary
	theData   = theEncoder.Decode(kPropertyListBinary, kNDataEncodingHex);
	testDict1 = propertyList.Decode(theData);

	theXML = propertyList.EncodeXML(testDict1);
	NN_ASSERT(theXML == kPropertyListXML);
}





//============================================================================
//		TPropertyList::DumpBinary : Dump the binary plist.
//----------------------------------------------------------------------------
void TPropertyList::DumpBinary(void)
{	NDataEncoder	theEncoder;
	NFile			theFile;
	NString			theText;
	NData			theData;



	// Create the binary .plist
	theFile = NFileUtilities::GetTemporaryFile("binary.plist");
	NFileUtilities::SetFileText(theFile, kPropertyListXML);

	NTask::Execute("/usr/bin/plutil", "-convert", "binary1", theFile.GetPath().GetUTF8());



	// Dump it
	theData = NFileUtilities::GetFileData(theFile);
	theText = theEncoder.Encode(theData, kNDataEncodingHex);
	theFile.Delete();

	NN_LOG("\nstatic const NString kPropertyListBinary					=	\"%@\";\n", theText);
}






