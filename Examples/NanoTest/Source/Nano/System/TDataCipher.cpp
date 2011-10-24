/*	NAME:
		TDataCipher.cpp

	DESCRIPTION:
		NDataCipher tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDataCipher.h"
#include "NDataDigest.h"

#include "TDataCipher.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8 kTestKey[]							= { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
static const UInt8 kTestData[]							= { 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x68, 0x6F, 0x77, 0x61, 0x72, 0x65, 0x79, 0x6F, 0x75, 0x74, 0x6F, 0x64, 0x61, 0x79 };

static const UInt32 kAdlerTest							= 0x7F7D0A40;
static const UInt32 kAdlerNull							= 0x7F7D0A40;
static const UInt32 kAdlerDES							= 0x9C260C01;
static const UInt32 kAdlerDES3							= 0x8E1D0AD4;
static const UInt32 kAdlerBlowfish						= 0x8B360A2B;





//============================================================================
//		TDataCipher::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDataCipher::Execute(void)
{	NData			dataKey, dataTest, dataEnc, dataRaw;
	NDataDigest		theDigest;
	NDataCipher		theCipher;



	// Get the state we need
	dataKey  = NData(NN_ARRAY_SIZE(kTestKey),  kTestKey);
	dataTest = NData(NN_ARRAY_SIZE(kTestData), kTestData);

	NN_ASSERT(theDigest.GetAdler32(dataTest) == kAdlerTest);
	theCipher.SetKey(dataKey);



	// Null
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionNone);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionNone);
	
	NN_ASSERT(theDigest.GetAdler32(dataEnc) == kAdlerNull);
	NN_ASSERT(theDigest.GetAdler32(dataRaw) == kAdlerTest);



	// DES
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionDES);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionDES);

	NN_ASSERT(theDigest.GetAdler32(dataEnc) == kAdlerDES);
	NN_ASSERT(theDigest.GetAdler32(dataRaw) == kAdlerTest);



	// DES3
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionDES3);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionDES3);

	NN_ASSERT(theDigest.GetAdler32(dataEnc) == kAdlerDES3);
	NN_ASSERT(theDigest.GetAdler32(dataRaw) == kAdlerTest);



	// Blowfish
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionBlowfish);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionBlowfish);

	NN_ASSERT(theDigest.GetAdler32(dataEnc) == kAdlerBlowfish);
	NN_ASSERT(theDigest.GetAdler32(dataRaw) == kAdlerTest);
}










