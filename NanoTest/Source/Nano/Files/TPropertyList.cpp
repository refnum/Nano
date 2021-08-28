/*	NAME:
		TPropertyList.cpp

	DESCRIPTION:
		NPropertyList tests.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NArray.h"
#include "NData.h"
#include "NDataEncoder.h"
#include "NDictionary.h"
#include "NFile.h"
#include "NFileHandle.h"
#include "NFileUtils.h"
#include "NFormat.h"
#include "NMathUtils.h"
#include "NPropertyList.h"
#include "NTestFixture.h"

// System
#include <stdlib.h>





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
#define NN_DUMP_BINARY_PLIST                                0





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Keys
static const NString kKeyDictionary                         = "Test Dictionary";
static const NString kKeyArray                              = "Test Array";
static const NString kKeyString                             = "Test String";
static const NString kKeyBoolean                            = "Test Boolean";
static const NString kKeyNumber                             = "Test Number";
static const NString kKeyData                               = "Test Data";
static const NString kKeyTime                               = "Test Time";


// Values
static const NString   kValueString1                        = "First String";
static const NString   kValueString2                        = "Second String";
static const bool      kValueBoolean1                       = true;
static const bool      kValueBoolean2                       = false;
static const int64_t   kValueNumber1                        = -2342;
static const float64_t kValueNumber2                        = kNPi;
static const uint8_t   kValueData1[]                        = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A};
static const uint8_t   kValueData2[]                        = {0x3C, 0xE7, 0xC7, 0x32, 0xE3, 0xD8, 0x52};
static const NTime     kValueTime1                          = -886538221;
static const NTime     kValueTime2                          = 268617632;


// Property lists
static const NString kPropertyListXML =
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	"<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" "
	"\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
	"<plist version=\"1.0\">\n"
	"<dict>\n"
	"	<key>Test Array</key>\n"
	"	<array>\n"
	"		<string>First String</string>\n"
	"		<true/>\n"
	"		<integer>-2342</integer>\n"
	"		<date>1972-11-28T03:22:59Z</date>\n"
	"		<data>iVBORw0KGg==</data>\n"
	"		<dict>\n"
	"			<key>Test Boolean</key>\n"
	"			<false/>\n"
	"			<key>Test Number</key>\n"
	"			<real>3.141592653589793</real>\n"
	"			<key>Test String</key>\n"
	"			<string>Second String</string>\n"
	"		</dict>\n"
	"	</array>\n"
	"	<key>Test Data</key>\n"
	"	<data>POfHMuPYUg==</data>\n"
	"	<key>Test Dictionary</key>\n"
	"	<dict>\n"
	"		<key>Test Boolean</key>\n"
	"		<false/>\n"
	"		<key>Test Number</key>\n"
	"		<real>3.141592653589793</real>\n"
	"		<key>Test String</key>\n"
	"		<string>Second String</string>\n"
	"	</dict>\n"
	"	<key>Test Time</key>\n"
	"	<date>2009-07-07T00:00:32Z</date>\n"
	"</dict>\n"
	"</plist>\n";

static const NString kPropertyListBinary =
	"62706C6973743030D401020304050C15165F100F546573742044696374696F6E6172795A5465737420417272617959"
	"546573742054696D6559546573742044617461D3060708090A0B5C5465737420426F6F6C65616E5B54657374204E75"
	"6D6265725B5465737420537472696E670823400921FB54442D185D5365636F6E6420537472696E67A60D0E0F101112"
	"5C466972737420537472696E670913FFFFFFFFFFFFF6DA33C1CA6BBFF68000004789504E470D0A1AD306070809140B"
	"0823400921FB54442D183341B002C7A0000000473CE7C732E3D8520811232E38424956626E6F78868D9A9BA4ADB5BC"
	"BDC6CF00000000000001010000000000000017000000000000000000000000000000D7";



#if NN_DUMP_BINARY_PLIST
//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		DumpBinary : Dump the binary plist.
//-----------------------------------------------------------------------------
static void DumpBinary()
{


	// Create the binary .plist
	NFile theFile = NFileUtils::GetTemporary("binary.plist");
	NFileHandle::WriteText(theFile, kPropertyListXML);

	NString theCmd = NString("/usr/bin/plutil -convert binary1 ") + theFile.GetPath().GetPath();
	system(theCmd.GetUTF8());



	// Dump it
	NData   theData = NFileHandle::ReadData(theFile);
	NString theText = NDataEncoder::Encode(NDataEncoding::Hex, theData);

	NN_LOG_INFO("\nstatic const NString kPropertyListBinary					=	\"{}\";\n",
				theText);
}
#endif // NN_DUMP_BINARY_PLIST





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TPropertyList)
{
	NDictionary   propertyDict, testDict1, testDict2;
	NData         testData1, testData2;
	NPropertyList propertyList;
	NArray        testArray;
	NTime         testTime;
	NString       theXML;

	SETUP
	{
		testData1 = NData(sizeof(kValueData1), kValueData1);
		testData2 = NData(sizeof(kValueData2), kValueData2);

		testDict2[kKeyString]  = kValueString2;
		testDict2[kKeyBoolean] = kValueBoolean2;
		testDict2[kKeyNumber]  = kValueNumber2;

		testArray.push_back(kValueString1);
		testArray.push_back(kValueBoolean1);
		testArray.push_back(kValueNumber1);
		testArray.push_back(kValueTime1);
		testArray.push_back(testData1);
		testArray.push_back(testDict2);

		propertyDict[kKeyDictionary] = testDict2;
		propertyDict[kKeyArray]      = testArray;
		propertyDict[kKeyData]       = testData2;
		propertyDict[kKeyTime]       = kValueTime2;

		testDict1.Clear();
		testDict2.Clear();
		testData1.Clear();
		testData2.Clear();
		testArray.Clear();
	}
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPropertyList, "XMLEncode")
{


	// Perform the test
	theXML = propertyList.EncodeXML(propertyDict);
	REQUIRE(theXML == kPropertyListXML);



#if NN_DUMP_BINARY_PLIST
	// Rebuild kPropertyListBinary from kPropertyListXML.
	DumpBinary();
#endif
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPropertyList, "XMLDecode")
{


	// Perform the test
	testDict1 = propertyList.DecodeXML(kPropertyListXML);
	REQUIRE(testDict1.HasKey(kKeyDictionary));
	REQUIRE(testDict1.HasKey(kKeyArray));
	REQUIRE(testDict1.HasKey(kKeyData));
	REQUIRE(testDict1.HasKey(kKeyTime));


	testDict2 = testDict1.GetDictionary(kKeyDictionary);
	REQUIRE(testDict2.HasKey(kKeyString));
	REQUIRE(testDict2.HasKey(kKeyBoolean));
	REQUIRE(testDict2.HasKey(kKeyNumber));
	REQUIRE(testDict2.GetString(kKeyString) == kValueString2);
	REQUIRE(testDict2.GetBool(kKeyBoolean) == kValueBoolean2);
	REQUIRE(testDict2.GetFloat64(kKeyNumber) == kValueNumber2);


	testArray = testDict1.GetArray(kKeyArray);
	REQUIRE(testArray.GetSize() == 6);
	REQUIRE(testArray.GetString(0) == kValueString1);
	REQUIRE(testArray.GetBool(1) == kValueBoolean1);
	REQUIRE(testArray.GetInt64(2) == kValueNumber1);
	REQUIRE(testArray.GetTime(3) == kValueTime1);

	testData1 = testArray.GetData(4);
	REQUIRE(testData1.GetSize() == sizeof(kValueData1));
	REQUIRE(memcmp(testData1.GetData(), kValueData1, sizeof(kValueData1)) == 0);

	testDict2 = testArray.GetDictionary(5);
	REQUIRE(testDict2.HasKey(kKeyString));
	REQUIRE(testDict2.HasKey(kKeyBoolean));
	REQUIRE(testDict2.HasKey(kKeyNumber));
	REQUIRE(testDict2.GetString(kKeyString) == kValueString2);
	REQUIRE(testDict2.GetBool(kKeyBoolean) == kValueBoolean2);
	REQUIRE(testDict2.GetFloat64(kKeyNumber) == kValueNumber2);


	testData2 = testDict1.GetData(kKeyData);
	REQUIRE(testData2.GetSize() == sizeof(kValueData2));
	REQUIRE(memcmp(testData2.GetData(), kValueData2, sizeof(kValueData2)) == 0);


	testTime = testDict1.GetTime(kKeyTime);
	REQUIRE(testTime == kValueTime2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPropertyList, "BinaryEncode")
{


	// Perform the test
	testDict1 = propertyList.DecodeXML(kPropertyListXML);
	testData1 = propertyList.Encode(testDict1, NPropertyListFormat::Binary);

	testDict2 = propertyList.Decode(testData1);
	theXML    = propertyList.EncodeXML(testDict2);
	REQUIRE(theXML == kPropertyListXML);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TPropertyList, "BinaryDecode")
{
	NDataEncoder theEncoder;



	// Perform the test
	testData1 = theEncoder.Decode(NDataEncoding::Hex, kPropertyListBinary);
	testDict1 = propertyList.Decode(testData1);

	theXML = propertyList.EncodeXML(testDict1);
	REQUIRE(theXML == kPropertyListXML);
}
