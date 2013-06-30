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

