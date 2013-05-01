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
#include "NString.h"

#include "TString.h"





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
//		TString::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TString::Execute(void)
{	NString			testString1, testString2;
	NStringList		theStrings;
	NRangeList		theRanges;
	NRange			theRange;
	NIndex			theIndex;
	bool			theFlag;



	// Compiler warnings
	NN_UNUSED(kTestText2_UTF8);



	// Construction
	testString1 = kTestCharPtr;
	NN_ASSERT(testString1.GetSize() == (NIndex) strlen(kTestCharPtr));
	NN_ASSERT(strcmp(testString1.GetUTF8(), kTestCharPtr) == 0);

	testString1 = kTestString;
	NN_ASSERT(testString1.GetSize() == (NIndex) strlen(kTestString.GetUTF8()));
	NN_ASSERT(strcmp(testString1.GetUTF8(), kTestString.GetUTF8()) == 0);



	// Size
	

	
	// Getters/setters
	
	
	
	// Find
	testString1 = "FindString";

	theRange = testString1.Find("Find");
	NN_ASSERT(theRange == NRange(0, 4));

	theRange = testString1.Find("ndStri");
	NN_ASSERT(theRange == NRange(2, 6));

	theRange = testString1.Find("ring");
	NN_ASSERT(theRange == NRange(6, 4));

	theRange = testString1.Find("ringer");
	NN_ASSERT(theRange == kNRangeNone);

	theRange = testString1.Find("FindStringer");
	NN_ASSERT(theRange == kNRangeNone);
	
	theRange = testString1.Find("String", kNStringNoCase);
	NN_ASSERT(theRange == NRange(4, 6));

	theRange = testString1.Find("ndstr", kNStringNoCase, NRange(1, 7));
	NN_ASSERT(theRange == NRange(2, 5));

	theRange = testString1.Find("ndstr", kNStringNoCase, NRange(2, 5));
	NN_ASSERT(theRange == NRange(2, 5));

	theRange = testString1.Find("ndstr", kNStringNoCase, NRange(3, 4));
	NN_ASSERT(theRange == kNRangeNone);

	theRange = testString1.Find("in");
	NN_ASSERT(theRange == NRange(1, 2));

	theRange = testString1.Find("in", kNStringBackwards);
	NN_ASSERT(theRange == NRange(7, 2));

	theRanges = testString1.FindAll("in");
	NN_ASSERT(theRanges.size() == 2);
	NN_ASSERT(theRanges[0] == NRange(1, 2));
	NN_ASSERT(theRanges[1] == NRange(7, 2));

	theRange = testString1.Find("n", kNStringNone, NRange(5, kNStringLength));
	NN_ASSERT(theRange == NRange(8, 1));
	
	testString1 = "1123";
	theRange    = testString1.Find("123");
	NN_ASSERT(theRange != kNRangeNone);



	// Replace
	testString1 = "FindReplaceString";
	testString1.Replace(NRange(0, 4), "");
	NN_ASSERT(testString1 == "ReplaceString");

	testString1 = "FindReplaceString";
	testString1.Replace(NRange(4, 7), "");
	NN_ASSERT(testString1 == "FindString");

	testString1 = "FindReplaceString";
	testString1.Replace(NRange(11, 6), "");
	NN_ASSERT(testString1 == "FindReplace");

	testString1 = "FindReplaceString";
	testString1.Replace(NRange(0, 4), "A");
	NN_ASSERT(testString1 == "AReplaceString");

	testString1 = "FindReplaceString";
	testString1.Replace(NRange(0, 4), "Longer");
	NN_ASSERT(testString1 == "LongerReplaceString");

	testString1 = "FindReplaceString";
	theFlag     = testString1.Replace("Replace", "");
	NN_ASSERT(theFlag);
	NN_ASSERT(testString1 == "FindString");

	testString1 = "FindReplaceString";
	theFlag     = testString1.Replace("Replace", "A");
	NN_ASSERT(theFlag);
	NN_ASSERT(testString1 == "FindAString");

	testString1 = "FindReplaceString";
	theFlag     = testString1.Replace("Replace", "andreplace", kNStringNoCase);
	NN_ASSERT(theFlag);
	NN_ASSERT(testString1 == "FindandreplaceString");

	testString1 = "FindReplaceString";
	theFlag     = testString1.Replace("Fail", "Failed");
	NN_ASSERT(!theFlag);
	NN_ASSERT(testString1 == "FindReplaceString");

	testString1 = "FindReplaceString";
	theIndex    = testString1.ReplaceAll("notpresent", "notfound");
	NN_ASSERT(theIndex    == 0);
	NN_ASSERT(testString1 == "FindReplaceString");

	testString1 = "FindReplaceString";
	theIndex    = testString1.ReplaceAll("in", "out");
	NN_ASSERT(theIndex    == 2);
	NN_ASSERT(testString1 == "FoutdReplaceStroutg");

	testString1 = "FindReplaceString";
	theIndex    = testString1.ReplaceAll("Find", "Found");
	NN_ASSERT(theIndex    == 1);
	NN_ASSERT(testString1 == "FoundReplaceString");

	testString1 = "FindReplaceString";
	theIndex    = testString1.ReplaceAll("String", "Strung");
	NN_ASSERT(theIndex    == 1);
	NN_ASSERT(testString1 == "FindReplaceStrung");

	testString1 = "FindReplaceString";
	theIndex    = testString1.ReplaceAll("Find", "Found");
	NN_ASSERT(theIndex    == 1);
	NN_ASSERT(testString1 == "FoundReplaceString");

	testString1 = "OneTwoOneThreeOne";
	theIndex    = testString1.ReplaceAll("One", "Zero");
	NN_ASSERT(theIndex    == 3);
	NN_ASSERT(testString1 == "ZeroTwoZeroThreeZero");

	testString1 = "FindReplaceString";
	theIndex    = testString1.ReplaceAll("IN", "out", kNStringNoCase, NRange(0, 5));
	NN_ASSERT(theIndex    == 1);
	NN_ASSERT(testString1 == "FoutdReplaceString");



	// Contents
	
	
	
	// Comparisons
	testString1 = "test";
	testString2 = "test";
	NN_ASSERT(testString1 == testString2);

	testString1 = "test";
	testString2 = "texx";
	NN_ASSERT(testString1 <  testString2);
	NN_ASSERT(testString1 != testString2);
	NN_ASSERT(testString2 >  testString1);

	testString1 = "test";
	testString2 = "TeSt";
	NN_ASSERT(testString1.Compare(testString2, kNStringNone)   == kNCompareGreaterThan);
	NN_ASSERT(testString1.Compare(testString2, kNStringNoCase) == kNCompareEqualTo);

	NN_ASSERT(NString("2"			).Compare("2",			kNStringNumeric) == kNCompareEqualTo);
	NN_ASSERT(NString("2.0"			).Compare("2.0",		kNStringNumeric) == kNCompareEqualTo);
	NN_ASSERT(NString("foo2.txt"	).Compare("foo7.txt",	kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("foo7.txt"	).Compare("foo2.txt",	kNStringNumeric) == kNCompareGreaterThan);
	NN_ASSERT(NString("foo002.txt"	).Compare("foo007.txt",	kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("123"			).Compare("0123",		kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("99"			).Compare("0123",		kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("010"			).Compare("0002",		kNStringNumeric) == kNCompareGreaterThan);
	NN_ASSERT(NString("0100"		).Compare("20",			kNStringNumeric) == kNCompareGreaterThan);
	NN_ASSERT(NString("1"			).Compare("10",			kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("2"			).Compare("11",			kNStringNumeric) == kNCompareLessThan);
	NN_ASSERT(NString("3"			).Compare("12",			kNStringNumeric) == kNCompareLessThan);



	// Hashing
	testString1 = NString(kTestText1_UTF8,    NN_ARRAY_SIZE(kTestText1_UTF8),    kNStringEncodingUTF8);
	testString2 = NString(kTestText1_UTF16BE, NN_ARRAY_SIZE(kTestText1_UTF16BE), kNStringEncodingUTF16BE);
	
	NN_ASSERT(testString1           == testString2);
	NN_ASSERT(testString1.GetHash() == testString2.GetHash());



	// Encoding
	testString1 = NString(kTestText2_UTF16BE, NN_ARRAY_SIZE(kTestText2_UTF16BE), kNStringEncodingUTF16BE);
	NN_ASSERT(strcmp(testString1.GetUTF8(), kTestText2_UTF8) == 0);



	// Transforms
	NN_ASSERT(NString("getupper").GetUpper() == "GETUPPER");
	NN_ASSERT(NString("GetLower").GetLower() == "getlower");
	
	testString1 = "get words in caps";
	testString1.Transform(kNStringCapitalizeWords);
	NN_ASSERT(testString1 == "Get Words In Caps");

	testString1 = "get, sentences! in.caps?";
	testString1.Transform(kNStringCapitalizeSentences);
	NN_ASSERT(testString1 == "Get, sentences! In.Caps?");

	testString1 = kTestDiacritics_String;
	testString2 = NString(kTestDiacritics_UTF8, kNStringLength, kNStringEncodingUTF8);
	testString2.Transform(kNStringStripDiacritics);
	NN_ASSERT(testString2 == testString1);



	// Extraction
	testString1 = "GetLeft";
	NN_ASSERT(testString1.GetLeft(0)  == "");
	NN_ASSERT(testString1.GetLeft(3)  == "Get");
	NN_ASSERT(testString1.GetLeft(30) == "GetLeft");

	testString1 = "GetRight";
	NN_ASSERT(testString1.GetRight(0)  == "");
	NN_ASSERT(testString1.GetRight(3)  == "ght");
	NN_ASSERT(testString1.GetRight(30) == "GetRight");

	testString1 = "GetString";
	NN_ASSERT(testString1.GetString(0)  == "GetString");
	NN_ASSERT(testString1.GetString(3)  == "String");
	NN_ASSERT(testString1.GetString(30) == "");

	testString1 = "GetString";
	NN_ASSERT(testString1.GetString(kNRangeNone)             == "");
	NN_ASSERT(testString1.GetString(kNRangeAll)              == "GetString");
	NN_ASSERT(testString1.GetString(NRange(5,  3))           == "rin");
	NN_ASSERT(testString1.GetString(NRange(5,  kNIndexNone)) == "ring");
	NN_ASSERT(testString1.GetString(NRange(50, kNIndexNone)) == "");



	// Split
	testString1 = "aa,bb,cc,dd";
	theStrings  = testString1.Split(kNTokenComma);
	NN_ASSERT(theStrings.size() == 4);
	NN_ASSERT(theStrings[0] == "aa");
	NN_ASSERT(theStrings[1] == "bb");
	NN_ASSERT(theStrings[2] == "cc");
	NN_ASSERT(theStrings[3] == "dd");

	testString1 = "aa,,,dd";
	theStrings  = testString1.Split(kNTokenComma);
	NN_ASSERT(theStrings.size() == 4);
	NN_ASSERT(theStrings[0] == "aa");
	NN_ASSERT(theStrings[1] == "");
	NN_ASSERT(theStrings[2] == "");
	NN_ASSERT(theStrings[3] == "dd");

	testString1 = ",aa,bb,";
	theStrings  = testString1.Split(kNTokenComma);
	NN_ASSERT(theStrings.size() == 2);
	NN_ASSERT(theStrings[0] == "aa");
	NN_ASSERT(theStrings[1] == "bb");

	testString1 = ",,";
	theStrings  = testString1.Split(kNTokenComma);
	NN_ASSERT(theStrings.size() == 1);
	NN_ASSERT(theStrings[0] == "");

	testString1 = ",";
	theStrings  = testString1.Split(kNTokenComma);
	NN_ASSERT(theStrings.empty());

	testString1 = "a,b,c,d";
	theStrings  = testString1.Split("b,");
	NN_ASSERT(theStrings.size() == 2);
	NN_ASSERT(theStrings[0] == "a,");
	NN_ASSERT(theStrings[1] == "c,d");

	testString1 = " aa bbb\n cccc\t\tddd\t";
	theStrings  = testString1.Split(kNStringWhitespace);
	NN_ASSERT(theStrings.size() == 4);
	NN_ASSERT(theStrings[0] == "aa");
	NN_ASSERT(theStrings[1] == "bbb");
	NN_ASSERT(theStrings[2] == "cccc");
	NN_ASSERT(theStrings[3] == "ddd");

	testString1 = "a,b,c,d";
	theStrings  = testString1.Split("[b,]", kNStringPattern);
	NN_ASSERT(theStrings.size() == 5);
	NN_ASSERT(theStrings[0] == "a");
	NN_ASSERT(theStrings[1] == "");
	NN_ASSERT(theStrings[2] == "");
	NN_ASSERT(theStrings[3] == "c");
	NN_ASSERT(theStrings[4] == "d");

	testString1 = "aa";
	theStrings  = testString1.Split(kNTokenComma);
	NN_ASSERT(theStrings.size() == 1);
	NN_ASSERT(theStrings[0] == "aa");



	// Trim
	//
	// Trimming has encoding-specific optimisation paths.
	TestTrim(kNStringEncodingUTF8);
	TestTrim(kNStringEncodingUTF16);
	TestTrim(kNStringEncodingUTF32);



	// Formatting
	
	
	
	// Operators
	testString1 = "hello ";
	testString2 = "world";
	NN_ASSERT(testString1 + testString2 == "hello world");

	testString1 += testString2;
	NN_ASSERT(testString1 == "hello world");
}





//============================================================================
//		TString::Encode : Encode a string in a specific encoding.
//----------------------------------------------------------------------------
#pragma mark -
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
{	NString		testString1;



	// Execute the tests
	testString1 = Encode("\n \tTrim", theEncoding);
	testString1.TrimLeft();
	NN_ASSERT(testString1 == "Trim");

	testString1 = Encode("Trim\n \t", theEncoding);
	testString1.TrimRight();
	NN_ASSERT(testString1 == "Trim");

	testString1 = Encode("\n \tTrim\t \n", theEncoding);
	testString1.Trim();
	NN_ASSERT(testString1 == "Trim");


	testString1 = Encode("Trim\n \t", theEncoding);
	testString1.TrimLeft();
	NN_ASSERT(testString1 == "Trim\n \t");

	testString1 = Encode("\n \tTrim", theEncoding);
	testString1.TrimRight();
	NN_ASSERT(testString1 == "\n \tTrim");

	testString1 = Encode("Trim", theEncoding);
	testString1.Trim();
	NN_ASSERT(testString1 == "Trim");

	testString1 = Encode("\n \t\t \n", theEncoding);
	testString1.Trim();
	NN_ASSERT(testString1 == "");


	testString1 = Encode("TrimLeft", theEncoding);
	testString1.TrimLeft(4);
	NN_ASSERT(testString1 == "Left");

	testString1 = Encode("TrimRight", theEncoding);
	testString1.TrimRight(5);
	NN_ASSERT(testString1 == "Trim");

	testString1 = Encode("TrimTrim", theEncoding);
	testString1.Trim(NRange(2, 4));
	NN_ASSERT(testString1 == "Trim");
}


