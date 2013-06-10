/*	NAME:
		TString.cpp

	DESCRIPTION:
		NString tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"
#include "NTestFixture.h"
#include "NString.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const char   *kTestCharPtr									= "test string";
static const NString kTestString									= "another test string";

static const UInt8 kTestText1_UTF8[]								= { 0xEF, 0xBB, 0xBF, 0x74, 0x65, 0x73, 0x74 };
static const UInt8 kTestText1_UTF16BE[]								= { 0xFE, 0xFF, 0x00, 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74 };

static const char *kTestText2_UTF8									= "/";
static const UInt8 kTestText2_UTF16BE[]								= { 0x00, 0x2F };

static const NString kTestDiacritics_String							= "Testing Text Diacritic";
static const UInt8   kTestDiacritics_UTF8[]							= { 0x54, 0xC3, 0xA9, 0x73, 0x74, 0xC3, 0xAF, 0x6E, 0x67,
																		0x20, 0x54, 0xC3, 0xA8, 0x78, 0x74, 0x20, 0x44, 0x69,
																		0x61, 0xC3, 0xA7, 0x72, 0x69, 0x74, 0x69, 0x63, 0x00 }; 





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSTRING(_name, _desc)									NANO_TEST(TString, _name, _desc)

NANO_FIXTURE(TString)
{
	NString		theString, theString2;

	NString	Encode(const NString &theValue, NStringEncoding theEncoding);
	void	TestTrim(NStringEncoding theEncoding);
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Default", "Default state")
{


	// Perform the test
	REQUIRE(theString.IsEmpty());
	REQUIRE(theString.GetSize() == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Construction", "Construction")
{


	// Perform the test
	theString = kTestCharPtr;
	REQUIRE(theString.GetSize() == (NIndex) strlen(kTestCharPtr));
	REQUIRE(strcmp(theString.GetUTF8(), kTestCharPtr) == 0);

	theString = kTestString;
	REQUIRE(theString.GetSize() == (NIndex) strlen(kTestString.GetUTF8()));
	REQUIRE(strcmp(theString.GetUTF8(), kTestString.GetUTF8()) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Find", "Find")
{	NRangeList		theRanges;
	NRange			theRange;



	// Perform the test
	theString = "FindString";

	theRange = theString.Find("Find");
	REQUIRE(theRange == NRange(0, 4));

	theRange = theString.Find("ndStri");
	REQUIRE(theRange == NRange(2, 6));

	theRange = theString.Find("ring");
	REQUIRE(theRange == NRange(6, 4));

	theRange = theString.Find("ringer");
	REQUIRE(theRange == kNRangeNone);

	theRange = theString.Find("FindStringer");
	REQUIRE(theRange == kNRangeNone);
	
	theRange = theString.Find("String", kNStringNoCase);
	REQUIRE(theRange == NRange(4, 6));

	theRange = theString.Find("ndstr", kNStringNoCase, NRange(1, 7));
	REQUIRE(theRange == NRange(2, 5));

	theRange = theString.Find("ndstr", kNStringNoCase, NRange(2, 5));
	REQUIRE(theRange == NRange(2, 5));

	theRange = theString.Find("ndstr", kNStringNoCase, NRange(3, 4));
	REQUIRE(theRange == kNRangeNone);

	theRange = theString.Find("in");
	REQUIRE(theRange == NRange(1, 2));

	theRange = theString.Find("in", kNStringBackwards);
	REQUIRE(theRange == NRange(7, 2));

	theRanges = theString.FindAll("in");
	REQUIRE(theRanges.size() == 2);
	REQUIRE(theRanges[0] == NRange(1, 2));
	REQUIRE(theRanges[1] == NRange(7, 2));

	theRange = theString.Find("n", kNStringNone, NRange(5, kNStringLength));
	REQUIRE(theRange == NRange(8, 1));
	
	theString = "1123";
	theRange  = theString.Find("123");
	REQUIRE(theRange != kNRangeNone);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Replace", "Replace")
{	NIndex		theIndex;
	bool		theFlag;



	// Perform the test
	theString = "FindReplaceString";
	theString.Replace(NRange(0, 4), "");
	REQUIRE(theString == "ReplaceString");

	theString = "FindReplaceString";
	theString.Replace(NRange(4, 7), "");
	REQUIRE(theString == "FindString");

	theString = "FindReplaceString";
	theString.Replace(NRange(11, 6), "");
	REQUIRE(theString == "FindReplace");

	theString = "FindReplaceString";
	theString.Replace(NRange(0, 4), "A");
	REQUIRE(theString == "AReplaceString");

	theString = "FindReplaceString";
	theString.Replace(NRange(0, 4), "Longer");
	REQUIRE(theString == "LongerReplaceString");

	theString = "FindReplaceString";
	theFlag   = theString.Replace("Replace", "");
	REQUIRE(theFlag);
	REQUIRE(theString == "FindString");

	theString = "FindReplaceString";
	theFlag   = theString.Replace("Replace", "A");
	REQUIRE(theFlag);
	REQUIRE(theString == "FindAString");

	theString = "FindReplaceString";
	theFlag   = theString.Replace("Replace", "andreplace", kNStringNoCase);
	REQUIRE(theFlag);
	REQUIRE(theString == "FindandreplaceString");

	theString = "FindReplaceString";
	theFlag   = theString.Replace("Fail", "Failed");
	REQUIRE(!theFlag);
	REQUIRE(theString == "FindReplaceString");

	theString = "FindReplaceString";
	theIndex  = theString.ReplaceAll("notpresent", "notfound");
	REQUIRE(theIndex  == 0);
	REQUIRE(theString == "FindReplaceString");

	theString = "FindReplaceString";
	theIndex  = theString.ReplaceAll("in", "out");
	REQUIRE(theIndex  == 2);
	REQUIRE(theString == "FoutdReplaceStroutg");

	theString = "FindReplaceString";
	theIndex  = theString.ReplaceAll("Find", "Found");
	REQUIRE(theIndex  == 1);
	REQUIRE(theString == "FoundReplaceString");

	theString = "FindReplaceString";
	theIndex  = theString.ReplaceAll("String", "Strung");
	REQUIRE(theIndex  == 1);
	REQUIRE(theString == "FindReplaceStrung");

	theString = "FindReplaceString";
	theIndex  = theString.ReplaceAll("Find", "Found");
	REQUIRE(theIndex  == 1);
	REQUIRE(theString == "FoundReplaceString");

	theString = "OneTwoOneThreeOne";
	theIndex  = theString.ReplaceAll("One", "Zero");
	REQUIRE(theIndex  == 3);
	REQUIRE(theString == "ZeroTwoZeroThreeZero");

	theString = "FindReplaceString";
	theIndex  = theString.ReplaceAll("IN", "out", kNStringNoCase, NRange(0, 5));
	REQUIRE(theIndex  == 1);
	REQUIRE(theString == "FoutdReplaceString");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Comparisons", "Comparisons")
{


	// Perform the test
	theString  = "test";
	theString2 = "test";
	REQUIRE(theString == theString2);

	theString  = "test";
	theString2 = "texx";
	REQUIRE(theString <  theString2);
	REQUIRE(theString != theString2);
	REQUIRE(theString2 >  theString);

	theString  = "test";
	theString2 = "TeSt";
	REQUIRE(theString.Compare(theString2, kNStringNone)   == kNCompareGreaterThan);
	REQUIRE(theString.Compare(theString2, kNStringNoCase) == kNCompareEqualTo);

	REQUIRE(NString("2"			).Compare("2",			kNStringNumeric) == kNCompareEqualTo);
	REQUIRE(NString("2.0"		).Compare("2.0",		kNStringNumeric) == kNCompareEqualTo);
	REQUIRE(NString("foo2.txt"	).Compare("foo7.txt",	kNStringNumeric) == kNCompareLessThan);
	REQUIRE(NString("foo7.txt"	).Compare("foo2.txt",	kNStringNumeric) == kNCompareGreaterThan);
	REQUIRE(NString("foo002.txt").Compare("foo007.txt",	kNStringNumeric) == kNCompareLessThan);
	REQUIRE(NString("123"		).Compare("0123",		kNStringNumeric) == kNCompareLessThan);
	REQUIRE(NString("99"		).Compare("0123",		kNStringNumeric) == kNCompareLessThan);
	REQUIRE(NString("010"		).Compare("0002",		kNStringNumeric) == kNCompareGreaterThan);
	REQUIRE(NString("0100"		).Compare("20",			kNStringNumeric) == kNCompareGreaterThan);
	REQUIRE(NString("1"			).Compare("10",			kNStringNumeric) == kNCompareLessThan);
	REQUIRE(NString("2"			).Compare("11",			kNStringNumeric) == kNCompareLessThan);
	REQUIRE(NString("3"			).Compare("12",			kNStringNumeric) == kNCompareLessThan);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Hashing", "Hashing")
{


	// Perform the test
	theString  = NString(kTestText1_UTF8,    NN_ARRAY_SIZE(kTestText1_UTF8),    kNStringEncodingUTF8);
	theString2 = NString(kTestText1_UTF16BE, NN_ARRAY_SIZE(kTestText1_UTF16BE), kNStringEncodingUTF16BE);
	
	REQUIRE(theString           == theString2);
	REQUIRE(theString.GetHash() == theString2.GetHash());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Encoding", "Encoding")
{


	// Perform the test
	theString = NString(kTestText2_UTF16BE, NN_ARRAY_SIZE(kTestText2_UTF16BE), kNStringEncodingUTF16BE);
	REQUIRE(strcmp(theString.GetUTF8(), kTestText2_UTF8) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Transforms", "Transforms")
{


	// Perform the test
	REQUIRE(NString("getupper").GetUpper() == "GETUPPER");
	REQUIRE(NString("GetLower").GetLower() == "getlower");
	
	theString = "get words in caps";
	theString.Transform(kNStringCapitalizeWords);
	REQUIRE(theString == "Get Words In Caps");

	theString = "get, sentences! in.caps?";
	theString.Transform(kNStringCapitalizeSentences);
	REQUIRE(theString == "Get, sentences! In.Caps?");

	theString  = kTestDiacritics_String;
	theString2 = NString(kTestDiacritics_UTF8, kNStringLength, kNStringEncodingUTF8);
	theString2.Transform(kNStringStripDiacritics);
	REQUIRE(theString2 == theString);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Extraction", "Extraction")
{


	// Perform the test
	theString = "GetLeft";
	REQUIRE(theString.GetLeft(0)  == "");
	REQUIRE(theString.GetLeft(3)  == "Get");
	REQUIRE(theString.GetLeft(30) == "GetLeft");

	theString = "GetRight";
	REQUIRE(theString.GetRight(0)  == "");
	REQUIRE(theString.GetRight(3)  == "ght");
	REQUIRE(theString.GetRight(30) == "GetRight");

	theString = "GetString";
	REQUIRE(theString.GetString(0)  == "GetString");
	REQUIRE(theString.GetString(3)  == "String");
	REQUIRE(theString.GetString(30) == "");

	theString = "GetString";
	REQUIRE(theString.GetString(kNRangeNone)             == "");
	REQUIRE(theString.GetString(kNRangeAll)              == "GetString");
	REQUIRE(theString.GetString(NRange(5,  3))           == "rin");
	REQUIRE(theString.GetString(NRange(5,  kNIndexNone)) == "ring");
	REQUIRE(theString.GetString(NRange(50, kNIndexNone)) == "");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Split", "Split")
{	NStringList		theStrings;



	// Perform the test
	theString  = "aa,bb,cc,dd";
	theStrings = theString.Split(kNTokenComma);
	REQUIRE(theStrings.size() == 4);
	REQUIRE(theStrings[0] == "aa");
	REQUIRE(theStrings[1] == "bb");
	REQUIRE(theStrings[2] == "cc");
	REQUIRE(theStrings[3] == "dd");

	theString  = "aa,,,dd";
	theStrings = theString.Split(kNTokenComma);
	REQUIRE(theStrings.size() == 4);
	REQUIRE(theStrings[0] == "aa");
	REQUIRE(theStrings[1] == "");
	REQUIRE(theStrings[2] == "");
	REQUIRE(theStrings[3] == "dd");

	theString  = ",aa,bb,";
	theStrings = theString.Split(kNTokenComma);
	REQUIRE(theStrings.size() == 2);
	REQUIRE(theStrings[0] == "aa");
	REQUIRE(theStrings[1] == "bb");

	theString  = ",,";
	theStrings = theString.Split(kNTokenComma);
	REQUIRE(theStrings.size() == 1);
	REQUIRE(theStrings[0] == "");

	theString  = ",";
	theStrings = theString.Split(kNTokenComma);
	REQUIRE(theStrings.empty());

	theString  = "a,b,c,d";
	theStrings = theString.Split("b,");
	REQUIRE(theStrings.size() == 2);
	REQUIRE(theStrings[0] == "a,");
	REQUIRE(theStrings[1] == "c,d");

	theString  = " aa bbb\n cccc\t\tddd\t";
	theStrings = theString.Split(kNStringWhitespace);
	REQUIRE(theStrings.size() == 4);
	REQUIRE(theStrings[0] == "aa");
	REQUIRE(theStrings[1] == "bbb");
	REQUIRE(theStrings[2] == "cccc");
	REQUIRE(theStrings[3] == "ddd");

	theString  = "a,b,c,d";
	theStrings = theString.Split("[b,]", kNStringPattern);
	REQUIRE(theStrings.size() == 5);
	REQUIRE(theStrings[0] == "a");
	REQUIRE(theStrings[1] == "");
	REQUIRE(theStrings[2] == "");
	REQUIRE(theStrings[3] == "c");
	REQUIRE(theStrings[4] == "d");

	theString  = "aa";
	theStrings = theString.Split(kNTokenComma);
	REQUIRE(theStrings.size() == 1);
	REQUIRE(theStrings[0] == "aa");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Trim", "Trim")
{


	// Perform the test
	TestTrim(kNStringEncodingUTF8);
	TestTrim(kNStringEncodingUTF16);
	TestTrim(kNStringEncodingUTF32);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRING("Operators", "Operators")
{


	// Perform the test
	theString  = "hello ";
	theString2 = "world";
	REQUIRE((theString + theString2) == "hello world");

	theString += theString2;
	REQUIRE(theString == "hello world");
}





#pragma mark private
//============================================================================
//		TString::Encode : Encode a string in a specific encoding.
//----------------------------------------------------------------------------
NString TString::Encode(const NString &theValue, NStringEncoding theEncoding)
{	NString		newString;



	// Encode the string
	newString = NString(theValue.GetData(theEncoding), theEncoding);

	return(newString);
}





//============================================================================
//		TString::TestTrim : Trim tests.
//----------------------------------------------------------------------------
void TString::TestTrim(NStringEncoding theEncoding)
{


	// Execute the tests
	theString = Encode("\n \tTrim", theEncoding);
	theString.TrimLeft();
	REQUIRE(theString == "Trim");

	theString = Encode("Trim\n \t", theEncoding);
	theString.TrimRight();
	REQUIRE(theString == "Trim");

	theString = Encode("\n \tTrim\t \n", theEncoding);
	theString.Trim();
	REQUIRE(theString == "Trim");


	theString = Encode("Trim\n \t", theEncoding);
	theString.TrimLeft();
	REQUIRE(theString == "Trim\n \t");

	theString = Encode("\n \tTrim", theEncoding);
	theString.TrimRight();
	REQUIRE(theString == "\n \tTrim");

	theString = Encode("Trim", theEncoding);
	theString.Trim();
	REQUIRE(theString == "Trim");

	theString = Encode("\n \t\t \n", theEncoding);
	theString.Trim();
	REQUIRE(theString == "");


	theString = Encode("TrimLeft", theEncoding);
	theString.TrimLeft(4);
	REQUIRE(theString == "Left");

	theString = Encode("TrimRight", theEncoding);
	theString.TrimRight(5);
	REQUIRE(theString == "Trim");

	theString = Encode("TrimTrim", theEncoding);
	theString.Trim(NRange(2, 4));
	REQUIRE(theString == "Trim");
}


