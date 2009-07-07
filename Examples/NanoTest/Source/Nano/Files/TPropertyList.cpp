/*	NAME:
		TPropertyList.cpp

	DESCRIPTION:
		NPropertyList tests.

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
#include "NMathUtilities.h"
#include "NPropertyList.h"

#include "TPropertyList.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestString1							= "First Text";
static const NString kTestString2							= "Second Text";

static const SInt64  kTestNumber1							= -2342;
static const Float64 kTestNumber2							= kPi;

static const UInt8 kTestData1[]								= { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A };
static const UInt8 kTestData2[]								= { 0x3C, 0xE7, 0xC7, 0x32, 0xE3, 0xD8, 0x52 };

static const NDate kTestDate1								= NDate(-886538221.7);
static const NDate kTestDate2								= NDate( 268617632.3);

static const NString kTestXML								=	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
																"<plist version=\"1.0\">\n"
																"	<dict>\n"
																"		<key>Test Array</key>\n"
																"		<array>\n"
																"			<string>First Text</string>\n"
																"			<integer>-2342</integer>\n"
																"			<date>1972-11-28T03:22:58Z</date>\n"
																"			<data>iVBORw0KGg==</data>\n"
																"			<dict>\n"
																"				<key>Test Number</key>\n"
																"				<real>3.1415926535897931</real>\n"
																"				<key>Test String</key>\n"
																"				<string>Second Text</string>\n"
																"			</dict>\n"
																"		</array>\n"
																"		<key>Test Data</key>\n"
																"		<data>POfHMuPYUg==</data>\n"
																"		<key>Test Date</key>\n"
																"		<date>2009-07-07T00:00:32Z</date>\n"
																"		<key>Test Dict</key>\n"
																"		<dict>\n"
																"			<key>Test Number</key>\n"
																"			<real>3.1415926535897931</real>\n"
																"			<key>Test String</key>\n"
																"			<string>Second Text</string>\n"
																"		</dict>\n"
																"	</dict>\n"
																"</plist>";





//============================================================================
//		TPropertyList::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TPropertyList::Execute(void)
{	NData				testData1, testData2;
	NDictionary			theDict, testDict;
	NArray				theArray;
	NString				theXML;
	NPropertyList		pList;



	// Get the state we need
	testData1 = NData(GET_ARRAY_SIZE(kTestData1), kTestData1);
	testData2 = NData(GET_ARRAY_SIZE(kTestData2), kTestData2);

	testDict.SetValue("Test Number", kTestNumber2);
	testDict.SetValue("Test String", kTestString2);

	theArray.AppendValue(kTestString1);
	theArray.AppendValue(kTestNumber1);
	theArray.AppendValue(kTestDate1);
	theArray.AppendValue(testData1);
	theArray.AppendValue(testDict);

	theDict.SetValue("Test Date",  kTestDate2);
	theDict.SetValue("Test Data",  testData2);
	theDict.SetValue("Test Dict",  testDict);
	theDict.SetValue("Test Array", theArray);



	// Encode to XML
	theXML = pList.EncodeXML(theDict);
	NN_ASSERT(theXML == kTestXML);



	// Decode from XML



}




