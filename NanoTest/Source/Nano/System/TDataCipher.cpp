/*	NAME:
		TDataCipher.cpp

	DESCRIPTION:
		NDataCipher tests.

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
#include "NDataCipher.h"
#include "NDataDigest.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const uint8_t kTestKey[]										= { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
static const uint8_t kTestData[]									= { 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x68, 0x6F, 0x77, 0x61, 0x72, 0x65, 0x79, 0x6F, 0x75, 0x74, 0x6F, 0x64, 0x61, 0x79 };

static const uint32_t kAdlerTest									= 0x7F7D0A40;
static const uint32_t kAdlerNull									= 0x7F7D0A40;
static const uint32_t kAdlerDES										= 0x9C260C01;
static const uint32_t kAdlerDES3									= 0x8E1D0AD4;
static const uint32_t kAdlerBlowfish								= 0x8B360A2B;





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDATACIPHER(...)										TEST_NANO(TDataCipher, ##__VA_ARGS__)

FIXTURE_NANO(TDataCipher)
{
	NData			dataKey, dataTest, dataEnc, dataRaw;
	NDataDigest		theDigest;
	NDataCipher		theCipher;
	
	SETUP
	{
		dataKey  = NData(NN_ARRAY_SIZE(kTestKey),  kTestKey);
		dataTest = NData(NN_ARRAY_SIZE(kTestData), kTestData);

		REQUIRE(theDigest.GetAdler32(dataTest) == kAdlerTest);
		theCipher.SetKey(dataKey);
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATACIPHER("Null")
{


	// Perform the test
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionNone);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionNone);
	
	REQUIRE(theDigest.GetAdler32(dataEnc) == kAdlerNull);
	REQUIRE(theDigest.GetAdler32(dataRaw) == kAdlerTest);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATACIPHER("DES")
{


	// Perform the test
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionDES);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionDES);

	REQUIRE(theDigest.GetAdler32(dataEnc) == kAdlerDES);
	REQUIRE(theDigest.GetAdler32(dataRaw) == kAdlerTest);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATACIPHER("DES3")
{


	// Perform the test
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionDES3);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionDES3);

	REQUIRE(theDigest.GetAdler32(dataEnc) == kAdlerDES3);
	REQUIRE(theDigest.GetAdler32(dataRaw) == kAdlerTest);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATACIPHER("Blowfish")
{


	// Perform the test
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionBlowfish);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionBlowfish);

	REQUIRE(theDigest.GetAdler32(dataEnc) == kAdlerBlowfish);
	REQUIRE(theDigest.GetAdler32(dataRaw) == kAdlerTest);
}










