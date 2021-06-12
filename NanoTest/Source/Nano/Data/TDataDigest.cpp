/*	NAME:
		TDataDigest.cpp

	DESCRIPTION:
		NDataDigest tests.

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
// Nano
#include "NDataDigest.h"
#include "NTestFixture.h"

// System
#include <map>
#include <unordered_map>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static const uint8_t kBytes1[]                              = {0xAA, 0xBB, 0xCC, 0xDD};
static const uint8_t kBytes2[]                              = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

static const NData kData1(sizeof(kBytes1), kBytes1);
static const NData kData2(sizeof(kBytes2), kBytes2);
static const NData kData3;





//=============================================================================
//		Structures
//-----------------------------------------------------------------------------
struct TestStructure
{
	uint64_t valueA;
	uint64_t valueB;
};

NBYTES_STD_EQUAL_TO(TestStructure);
NBYTES_STD_HASH(TestStructure);
NBYTES_STD_LESS(TestStructure);





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TDataDigest){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "Internet")
{
	// Perform the test
	REQUIRE(NDataDigest::GetInternet(kData1) == 0x8866);
	REQUIRE(NDataDigest::GetInternet(kData2) == 0xf9f6);
	REQUIRE(NDataDigest::GetInternet(kData3) == 0x0000);

	REQUIRE(NDataDigest::GetInternet(std::size(kBytes1), kBytes1) == 0x8866);
	REQUIRE(NDataDigest::GetInternet(std::size(kBytes2), kBytes2) == 0xf9f6);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "Adler32")
{


	// Perform the test
	REQUIRE(NDataDigest::GetAdler32(kData1) == 0x074e030e);
	REQUIRE(NDataDigest::GetAdler32(kData2) == 0x0023000f);
	REQUIRE(NDataDigest::GetAdler32(kData3) == 0x00000000);

	REQUIRE(NDataDigest::GetAdler32(std::size(kBytes1), kBytes1) == 0x074e030e);
	REQUIRE(NDataDigest::GetAdler32(std::size(kBytes2), kBytes2) == 0x0023000f);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "XXHash32")
{


	// Perform the test
	REQUIRE(NDataDigest::GetXXHash32(kData1) == 0x653fda5e);
	REQUIRE(NDataDigest::GetXXHash32(kData2) == 0x874131df);
	REQUIRE(NDataDigest::GetXXHash32(kData3) == 0x00000000);

	REQUIRE(NDataDigest::GetXXHash32(std::size(kBytes1), kBytes1) == 0x653fda5e);
	REQUIRE(NDataDigest::GetXXHash32(std::size(kBytes2), kBytes2) == 0x874131df);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "XXHash64")
{


	// Perform the test
	REQUIRE(NDataDigest::GetXXHash64(kData1) == 0x781c641b331c6481);
	REQUIRE(NDataDigest::GetXXHash64(kData2) == 0x438e2507bf950423);
	REQUIRE(NDataDigest::GetXXHash64(kData3) == 0x0000000000000000);

	REQUIRE(NDataDigest::GetXXHash64(std::size(kBytes1), kBytes1) == 0x781c641b331c6481);
	REQUIRE(NDataDigest::GetXXHash64(std::size(kBytes2), kBytes2) == 0x438e2507bf950423);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "MD5")
{


	// Perform the test
	REQUIRE(NDataDigest::GetMD5(kData1).GetString() == "ca6ffbf95b47864fd4e73f2601326304");
	REQUIRE(NDataDigest::GetMD5(kData2).GetString() == "d15ae53931880fd7b724dd7888b4b4ed");
	REQUIRE(NDataDigest::GetMD5(kData3).GetString() == "00000000000000000000000000000000");

	REQUIRE(NDataDigest::GetMD5(std::size(kBytes1), kBytes1).GetString() ==
			"ca6ffbf95b47864fd4e73f2601326304");

	REQUIRE(NDataDigest::GetMD5(std::size(kBytes2), kBytes2).GetString() ==
			"d15ae53931880fd7b724dd7888b4b4ed");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "SHA1")
{


	// Perform the test
	REQUIRE(NDataDigest::GetSHA1(kData1).GetString() == "a7b7e9592daa0896db0517bf8ad53e56b1246923");
	REQUIRE(NDataDigest::GetSHA1(kData2).GetString() == "868460d98d09d8bbb93d7b6cdd15cc7fbec676b9");
	REQUIRE(NDataDigest::GetSHA1(kData3).GetString() == "0000000000000000000000000000000000000000");

	REQUIRE(NDataDigest::GetSHA1(std::size(kBytes1), kBytes1).GetString() ==
			"a7b7e9592daa0896db0517bf8ad53e56b1246923");

	REQUIRE(NDataDigest::GetSHA1(std::size(kBytes2), kBytes2).GetString() ==
			"868460d98d09d8bbb93d7b6cdd15cc7fbec676b9");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "SHA256")
{


	// Perform the test
	REQUIRE(NDataDigest::GetSHA256(kData1).GetString() ==
			"8d70d691c822d55638b6e7fd54cd94170c87d19eb1f628b757506ede5688d297");

	REQUIRE(NDataDigest::GetSHA256(kData2).GetString() ==
			"17e88db187afd62c16e5debf3e6527cd006bc012bc90b51a810cd80c2d511f43");

	REQUIRE(NDataDigest::GetSHA256(kData3).GetString() ==
			"0000000000000000000000000000000000000000000000000000000000000000");

	REQUIRE(NDataDigest::GetSHA256(std::size(kBytes1), kBytes1).GetString() ==
			"8d70d691c822d55638b6e7fd54cd94170c87d19eb1f628b757506ede5688d297");

	REQUIRE(NDataDigest::GetSHA256(std::size(kBytes2), kBytes2).GetString() ==
			"17e88db187afd62c16e5debf3e6527cd006bc012bc90b51a810cd80c2d511f43");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "NDigestX")
{


	// Perform the test
	NDigest160 theDigest, otherDigest;

	REQUIRE(!theDigest.IsValid());

	theDigest = NDataDigest::GetSHA1(kData1);
	REQUIRE(theDigest.IsValid());

	theDigest.Clear();
	REQUIRE(!theDigest.IsValid());

	theDigest = NDataDigest::GetSHA1(kData1);
	REQUIRE(!theDigest.GetData().IsEmpty());

	theDigest.SetData(theDigest.GetData());
	REQUIRE(!theDigest.GetData().IsEmpty());

	REQUIRE(theDigest.GetBytes() != nullptr);
	REQUIRE(theDigest.GetMutableBytes() != nullptr);

	REQUIRE(theDigest.GetString() == "a7b7e9592daa0896db0517bf8ad53e56b1246923");

	REQUIRE(otherDigest < theDigest);
	REQUIRE(theDigest > otherDigest);
	REQUIRE(theDigest == theDigest);
	REQUIRE(theDigest != otherDigest);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataDigest, "NBYTES_STD")
{


	// Perform the test
	TestStructure theValue{};

	std::unordered_map<TestStructure, bool> testA;
	std::map<TestStructure, bool>           testB;

	testA[theValue] = true;
	testB[theValue] = true;
}
