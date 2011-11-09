/*	NAME:
		NDataCipher.h

	DESCRIPTION:
		Data encryption.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATACIPHER_HDR
#define NDATACIPHER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Encryption algorithms
//
// These values are considered to be fixed, and will never change.
typedef enum {
	kNEncryptionNone			= 0x6E756C6C,	// 'null'
	kNEncryptionDES				= 0x64657331,	// 'des1'
	kNEncryptionDES3			= 0x64657333,	// 'des3'
	kNEncryptionBlowfish		= 0x626C6F66	// 'blof'
} NEncryption;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDataCipher {
public:
										NDataCipher(void);
	virtual							   ~NDataCipher(void);


	// Get/set the key
	//
	// A key must be assigned before data can be encrypted/decrypted.
	//
	// Requirements are:
	//
	//		kNEncryptionNone			None
	//		kNEncryptionDES				Only first  8 bytes of key are used
	//		kNEncryptionDES3			Only first 24 bytes of key are used
	//		kNEncryptionBlowfish		None
	//
	NData								GetKey(void) const;
	void								SetKey(const NData &theKey);


	// Encrypt/decrypt data
	//
	// Requirements are:
	//
	//		kNEncryptionNone			None
	//		kNEncryptionDES				Data must be multiple of 8 bytes in size
	//		kNEncryptionDES3			Data must be multiple of 8 bytes in size
	//		kNEncryptionBlowfish		Data must be multiple of 8 bytes in size
	//
	// Additional notes:
	//
	//		DES/DES3 encryption treats the key, input data, and output data as
	//		an array of big-endian 32-bit words.
	//
	//		These are swapped to native-endian internally for processing, then
	//		back to big-endian for the result.
	//
	//		This behaviour was chosen to match the openssl command-line tool.
	//
	NData								Encrypt(const NData &srcData, NEncryption theAlgorithm=kNEncryptionBlowfish);
	NData								Decrypt(const NData &srcData, NEncryption theAlgorithm=kNEncryptionBlowfish);


private:
	NStatus								Null_Encrypt(NData &theData);
	NStatus								Null_Decrypt(NData &theData);

	NStatus								DES_Encrypt(NData &theData);
	NStatus								DES_Decrypt(NData &theData);

	NStatus								DES3_Encrypt(NData &theData);
	NStatus								DES3_Decrypt(NData &theData);

	NStatus								Blowfish_Encrypt(NData &theData);
	NStatus								Blowfish_Decrypt(NData &theData);


private:
	NData								mKey;
};





#endif // NDATACOMPRESSOR_HDR


