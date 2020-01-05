/*	NAME:
		TStringEncoder.cpp

	DESCRIPTION:
		NStringEncoder tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringEncoder.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSTRINGENCODER(...)									TEST_NANO(TStringEncoder, ##__VA_ARGS__)

FIXTURE_NANO(TStringEncoder)
{
	void	TestTerminator(const NString &theString, NStringEncoding theEncoding);
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGENCODER("Terminators")
{


	// Perform the test
	TestTerminator("Hello World", kNStringEncodingUTF8);
	TestTerminator("Hello World", kNStringEncodingUTF16);
	TestTerminator("Hello World", kNStringEncodingUTF32);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSTRINGENCODER("Encodings")
{	NData				data1, data2;
	NStringEncoder		theEncoder;
	NStatus				theErr;



	// Perform the test
	data1 = NString("Hello World").GetData(kNStringEncodingUTF8);
	
	theErr = theEncoder.Convert(data1, data2, kNStringEncodingUTF8,				kNStringEncodingUTF16);
	REQUIRE_NOERR(theErr);

	theErr = theEncoder.Convert(data2, data2, kNStringEncodingUTF16,			kNStringEncodingUTF32);
	REQUIRE_NOERR(theErr);

	theErr = theEncoder.Convert(data2, data2, kNStringEncodingUTF32,			kNStringEncodingMacRoman);
	REQUIRE_NOERR(theErr);

	theErr = theEncoder.Convert(data2, data2, kNStringEncodingMacRoman,			kNStringEncodingASCII);
	REQUIRE_NOERR(theErr);
	
	theErr = theEncoder.Convert(data2, data2, kNStringEncodingASCII,			kNStringEncodingISOLatin1);
	REQUIRE_NOERR(theErr);

	theErr = theEncoder.Convert(data2, data2, kNStringEncodingISOLatin1,		kNStringEncodingWindowsLatin1);
	REQUIRE_NOERR(theErr);

	theErr = theEncoder.Convert(data2, data2, kNStringEncodingWindowsLatin1,	kNStringEncodingUTF8);
	REQUIRE_NOERR(theErr);
	
	REQUIRE(data1 == data2);
}





#pragma mark private
//============================================================================
//		TStringEncoder::TestTerminator : Test string terminators.
//----------------------------------------------------------------------------
void TStringEncoder::TestTerminator(const NString &theString, NStringEncoding theEncoding)
{	NData				data1, data2;
	NStringEncoder		testEncoder;



	// Add terminator
	data1 = theString.GetData(theEncoding, kNStringRenderNone);
	data2 = data1;

	testEncoder.AddTerminator(data2, theEncoding);
	REQUIRE(data1 != data2);

	testEncoder.RemoveTerminator(data2, theEncoding);
	REQUIRE(data1 == data2);



	// Remove terminator
	data1 = theString.GetData(theEncoding, kNStringNullTerminate);
	data2 = data1;

	testEncoder.RemoveTerminator(data2, theEncoding);
	REQUIRE(data1 != data2);

	testEncoder.AddTerminator(data2, theEncoding);
}

