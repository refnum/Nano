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
static const UInt8 kTestKey[]							= { 0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64 };
static const UInt8 kTestData[]							= { 0x6E, 0x6F, 0x20, 0x6D, 0x6F, 0x72, 0x65, 0x20, 0x73, 0x65, 0x63, 0x72, 0x65, 0x74, 0x73, 0x21 };





//============================================================================
//		TDataCipher::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDataCipher::Execute(void)
{	NData			dataKey, dataTest, dataEnc, dataRaw;
	UInt32			adlterValue;
	NDataDigest		theDigest;
	NDataCipher		theCipher;



	// Get the state we need
	dataKey  = NData(NN_ARRAY_SIZE(kTestKey),  kTestKey);
	dataTest = NData(NN_ARRAY_SIZE(kTestData), kTestData);

	adlterValue = theDigest.GetAdler32(dataTest);
	theCipher.SetKey(dataKey);



	// Null
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionNone);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionNone);
	
	NN_ASSERT(theDigest.GetAdler32(dataRaw) == adlterValue);



	// DES
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionDES);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionDES);
	
	NN_ASSERT(theDigest.GetAdler32(dataRaw) == adlterValue);



	// DES3
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionDES3);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionDES3);
	
	NN_ASSERT(theDigest.GetAdler32(dataRaw) == adlterValue);



	// Blowfish
	dataEnc = theCipher.Encrypt(dataTest, kNEncryptionBlowfish);
	dataRaw = theCipher.Decrypt(dataEnc,  kNEncryptionBlowfish);
	
	NN_ASSERT(theDigest.GetAdler32(dataRaw) == adlterValue);
}










