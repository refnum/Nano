/*	NAME:
		TDataDigest.cpp

	DESCRIPTION:
		NDataDigest tests.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
// Nano
#include "NDataDigest.h"
#include "NTestFixture.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static const uint8_t kTestData1[]                           = {0xAA, 0xBB, 0xCC, 0xDD};
static const uint8_t kTestData2[]                           = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TDataDigest)
{
	NData       data1, data2, data3;
	NDataDigest theDigest;

	SETUP
	{
		data1 = NData(NN_ARRAY_SIZE(kTestData1), kTestData1);
		data2 = NData(NN_ARRAY_SIZE(kTestData2), kTestData2);
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "Internet")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetInternet(data1)) == "00008866");
	REQUIRE(theDigest.GetString(theDigest.GetInternet(data2)) == "0000f9f6");
	REQUIRE(theDigest.GetString(theDigest.GetInternet(data3)) == "00000000");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "DJB2")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetDJB2(data1)) == "7cbd7e93");
	REQUIRE(theDigest.GetString(theDigest.GetDJB2(data2)) == "07f24354");
	REQUIRE(theDigest.GetString(theDigest.GetDJB2(data3)) == "00000000");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "Adler32")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetAdler32(data1)) == "074e030e");
	REQUIRE(theDigest.GetString(theDigest.GetAdler32(data2)) == "0023000f");
	REQUIRE(theDigest.GetString(theDigest.GetAdler32(data3)) == "00000000");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "MD5")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetMD5(data1)) == "ca6ffbf95b47864fd4e73f2601326304");
	REQUIRE(theDigest.GetString(theDigest.GetMD5(data2)) == "d15ae53931880fd7b724dd7888b4b4ed");
	REQUIRE(theDigest.GetString(theDigest.GetMD5(data3)) == "00000000000000000000000000000000");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "SHA1")
{


	// Perform the test
	REQUIRE(theDigest.GetString(theDigest.GetSHA1(data1)) ==
			"a7b7e9592daa0896db0517bf8ad53e56b1246923");
	REQUIRE(theDigest.GetString(theDigest.GetSHA1(data2)) ==
			"868460d98d09d8bbb93d7b6cdd15cc7fbec676b9");
	REQUIRE(theDigest.GetString(theDigest.GetSHA1(data3)) ==
			"0000000000000000000000000000000000000000");
}
