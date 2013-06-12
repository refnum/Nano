/*	NAME:
		TDataDigest.cpp

	DESCRIPTION:
		NDataDigest tests.

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
#include "NDataDigest.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt8 kTestData1[]										= { 0xAA, 0xBB, 0xCC, 0xDD };
static const UInt8 kTestData2[]										= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDATADIGEST(_name, _desc)								NANO_TEST(TDataDigest, _name, _desc)

NANO_FIXTURE(TDataDigest)
{
	NData			data1, data2, data3;
	NDataDigest		theDigest;
	
	SETUP
	{
		data1 = NData(NN_ARRAY_SIZE(kTestData1), kTestData1);
		data2 = NData(NN_ARRAY_SIZE(kTestData2), kTestData2);
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATADIGEST("Internet", "Internet digest")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetInternet(data1)) == "00008866");
	REQUIRE(theDigest.GetString(theDigest.GetInternet(data2)) == "0000f9f6");
	REQUIRE(theDigest.GetString(theDigest.GetInternet(data3)) == "00000000");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATADIGEST("DJB2", "DJB2 digest")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetDJB2(data1)) == "7cbd7e93");
	REQUIRE(theDigest.GetString(theDigest.GetDJB2(data2)) == "07f24354");
	REQUIRE(theDigest.GetString(theDigest.GetDJB2(data3)) == "00000000");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATADIGEST("Adler32", "Adler32 digest")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetAdler32(data1)) == "074e030e");
	REQUIRE(theDigest.GetString(theDigest.GetAdler32(data2)) == "0023000f");
	REQUIRE(theDigest.GetString(theDigest.GetAdler32(data3)) == "00000000");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATADIGEST("MD5", "MD5 digest")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetMD5(data1)) == "ca6ffbf95b47864fd4e73f2601326304");
	REQUIRE(theDigest.GetString(theDigest.GetMD5(data2)) == "d15ae53931880fd7b724dd7888b4b4ed");
	REQUIRE(theDigest.GetString(theDigest.GetMD5(data3)) == "00000000000000000000000000000000");
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATADIGEST("SHA1", "SHA1 digest")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetSHA1(data1)) == "a7b7e9592daa0896db0517bf8ad53e56b1246923");
	REQUIRE(theDigest.GetString(theDigest.GetSHA1(data2)) == "868460d98d09d8bbb93d7b6cdd15cc7fbec676b9");
	REQUIRE(theDigest.GetString(theDigest.GetSHA1(data3)) == "0000000000000000000000000000000000000000");
}


