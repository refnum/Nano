/*	NAME:
		NDataCipher.h

	DESCRIPTION:
		Data encryption.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
	kNEncryptionNone			= 0x6E6E756C,
	kNEncryptionDES3			= 0x6E646573,
	kNEncryptionBlowfish		= 0x6E626C6F
} NEncryption;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDataCipher {
public:
										 NDataCipher(void);
	virtual								~NDataCipher(void);


	// Get/set the key
	//
	// A key must be assigned before data can be encrypted/decrypted.
	NData								GetKey(void) const;
	void								SetKey(const NData &theKey);


	// Encrypt/decrypt data
	//
	// Requirements are:
	//
	//		kNEncryptionNone			None
	//		kNEncryptionDES3			None
	//		kNEncryptionBlowfish		Data must be multiple of 8 bytes in size
	//
	NData								Encrypt(const NData &srcData, NEncryption theAlgorithm=kNEncryptionBlowfish);
	NData								Decrypt(const NData &srcData, NEncryption theAlgorithm=kNEncryptionBlowfish);


private:
	NStatus								Null_Encrypt(NData &theData);
	NStatus								Null_Decrypt(NData &theData);

	NStatus								DES3_Encrypt(NData &theData);
	NStatus								DES3_Decrypt(NData &theData);

	NStatus								Blowfish_Encrypt(NData &theData);
	NStatus								Blowfish_Decrypt(NData &theData);


private:
	NData								mKey;
};





#endif // NDATACOMPRESSOR_HDR


