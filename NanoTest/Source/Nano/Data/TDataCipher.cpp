/*	NAME:
		TDataCipher.cpp

	DESCRIPTION:
		NDataCipher tests.

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
#include "NDataCipher.h"
#include "NDataDigest.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint64_t kTestKey_32[]                     = {0x11223344, 0x55667788, 0xAABBCCDD, 0xEEFF0011};
static constexpr uint64_t kTestData_16[]                    = {0x12345678, 0x87654321, 0xABCDEF01, 0x10FEDCBA};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TDataCipher)
{
	NData       dataInput, dataCipher, dataOutput;
	NDataCipher theCipher;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataCipher, "Null")
{


	// Perform the test
	theCipher.SetKey({sizeof(kTestKey_32), kTestKey_32});

	dataInput  = NData(sizeof(kTestData_16), kTestData_16);
	dataCipher = theCipher.Encrypt(NCipher::Null, dataInput);
	dataOutput = theCipher.Decrypt(NCipher::Null, dataCipher);

	REQUIRE(NDataDigest::GetXXHash64(dataInput) == 0x4dd4479d1c7941b1);
	REQUIRE(NDataDigest::GetXXHash64(dataOutput) == 0x4dd4479d1c7941b1);
	REQUIRE(NDataDigest::GetXXHash64(dataCipher) == 0x4dd4479d1c7941b1);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TDataCipher, "AES_256")
{


	// Perform the test
	theCipher.SetKey({sizeof(kTestKey_32), kTestKey_32});

	dataInput  = NData(sizeof(kTestData_16), kTestData_16);
	dataCipher = theCipher.Encrypt(NCipher::AES_256, dataInput);
	dataOutput = theCipher.Decrypt(NCipher::AES_256, dataCipher);

	REQUIRE(NDataDigest::GetXXHash64(dataInput) == 0x4dd4479d1c7941b1);
	REQUIRE(NDataDigest::GetXXHash64(dataOutput) == 0x4dd4479d1c7941b1);
	REQUIRE(NDataDigest::GetXXHash64(dataCipher) == 0x822cff44153b9606);
}
