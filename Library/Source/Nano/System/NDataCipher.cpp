/*	NAME:
		NDataCipher.cpp

	DESCRIPTION:
		Data encryption.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "tero_des.h"
#include "blowfish.h"

#include "NDataCipher.h"





//============================================================================
//		NDataCipher::NDataCipher : Constructor.
//----------------------------------------------------------------------------
NDataCipher::NDataCipher(void)
{
}





//============================================================================
//		NDataCipher::~NDataCipher : Destructor.
//----------------------------------------------------------------------------
NDataCipher::~NDataCipher(void)
{
}





//============================================================================
//		NDataCipher::GetKey : Get the key.
//----------------------------------------------------------------------------
NData NDataCipher::GetKey(void) const
{


	// Get the key
	return(mKey);
}





//============================================================================
//		NDataCipher::SetKey : Set the key.
//----------------------------------------------------------------------------
void NDataCipher::SetKey(const NData &theKey)
{


	// Set the key
	mKey = theKey;
}





//============================================================================
//		NDataCipher::Encrypt : Encrypt data.
//----------------------------------------------------------------------------
NData NDataCipher::Encrypt(const NData &srcData, NEncryption theAlgorithm)
{	NData		dstData;
	NStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(!srcData.IsEmpty());
	NN_ASSERT(!mKey.IsEmpty());



	// Get the state we need
	if (mKey.IsEmpty())
		return(dstData);

	dstData = srcData;



	// Encrypt the data
	switch (theAlgorithm) {
		case kNEncryptionNone:
			theErr = Null_Encrypt(dstData);
			break;

		case kNEncryptionDES:
			theErr = DES_Encrypt(dstData);
			break;

		case kNEncryptionDES3:
			theErr = DES3_Encrypt(dstData);
			break;

		case kNEncryptionBlowfish:
			theErr = Blowfish_Encrypt(dstData);
			break;
	
		default:
			NN_LOG("Unknown algorithm: %d", theAlgorithm);
			theErr = kNErrParam;
			break;
		}

	NN_ASSERT_NOERR(theErr);



	// Handle failure
	if (theErr != kNoErr)
		dstData.Clear();
	
	return(dstData);
}





//============================================================================
//		NDataCipher::Decrypt : Decrypt data.
//----------------------------------------------------------------------------
NData NDataCipher::Decrypt(const NData &srcData, NEncryption theAlgorithm)
{	NData		dstData;
	NStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(!srcData.IsEmpty());
	NN_ASSERT(!mKey.IsEmpty());



	// Get the state we need
	if (mKey.IsEmpty())
		return(dstData);

	dstData = srcData;



	// Decrypt the data
	switch (theAlgorithm) {
		case kNEncryptionNone:
			theErr = Null_Decrypt(dstData);
			break;

		case kNEncryptionDES:
			theErr = DES_Decrypt(dstData);
			break;

		case kNEncryptionDES3:
			theErr = DES3_Decrypt(dstData);
			break;

		case kNEncryptionBlowfish:
			theErr = Blowfish_Decrypt(dstData);
			break;
	
		default:
			NN_LOG("Unknown algorithm: %d", theAlgorithm);
			theErr = kNErrParam;
			break;
		}

	NN_ASSERT_NOERR(theErr);



	// Handle failure
	if (theErr != kNoErr)
		dstData.Clear();
	
	return(dstData);
}			





//============================================================================
//		NDataCipher::Null_Encrypt : Encrypt using null encryption.
//----------------------------------------------------------------------------
#pragma mark -
NStatus NDataCipher::Null_Encrypt(NData &/*theData*/)
{


	// Encrypt the data
	return(kNoErr);
}





//============================================================================
//		NDataCipher::Null_Decrypt : Decrypt using null encryption.
//----------------------------------------------------------------------------
NStatus NDataCipher::Null_Decrypt(NData &/*theData*/)
{


	// Decrypt the data
	return(kNoErr);
}





//============================================================================
//		NDataCipher::DES_Encrypt : Encrypt using DES encryption.
//----------------------------------------------------------------------------
NStatus NDataCipher::DES_Encrypt(NData &theData)
{


	// Encrypt the data
	::DES_Encrypt(mKey.GetSize(), mKey.GetData(), theData.GetSize(), theData.GetData());
	
	return(kNoErr);
}





//============================================================================
//		NDataCipher::DES_Decrypt : Decrypt using DES encryption.
//----------------------------------------------------------------------------
NStatus NDataCipher::DES_Decrypt(NData &theData)
{


	// Decrypt the data
	::DES_Decrypt(mKey.GetSize(), mKey.GetData(), theData.GetSize(), theData.GetData());
	
	return(kNoErr);
}





//============================================================================
//		NDataCipher::DES3_Encrypt : Encrypt using DES3 encryption.
//----------------------------------------------------------------------------
NStatus NDataCipher::DES3_Encrypt(NData &theData)
{


	// Encrypt the data
	::DES3_Encrypt(mKey.GetSize(), mKey.GetData(), theData.GetSize(), theData.GetData());
	
	return(kNoErr);
}





//============================================================================
//		NDataCipher::DES3_Decrypt : Decrypt using DES3 encryption.
//----------------------------------------------------------------------------
NStatus NDataCipher::DES3_Decrypt(NData &theData)
{


	// Decrypt the data
	::DES3_Decrypt(mKey.GetSize(), mKey.GetData(), theData.GetSize(), theData.GetData());
	
	return(kNoErr);
}





//============================================================================
//		NDataCipher::Blowfish_Encrypt : Encrypt using Blowfish encryption.
//----------------------------------------------------------------------------
NStatus NDataCipher::Blowfish_Encrypt(NData &theData)
{	NIndex					n, numWords;
	UInt32					*dataPtr;
	blowfish_context_t		bfState;



	// Check our parameters
	if ((theData.GetSize() % 8) != 0)
		return(kNErrParam);



	// Get the state we need
	dataPtr  = (UInt32 *) theData.GetData();
	numWords =            theData.GetSize() / sizeof(UInt32);



	// Encrypt the data
	blowfish_initiate(&bfState, mKey.GetData(), mKey.GetSize());
	
	for (n = 0; n < numWords; n += 2)
		blowfish_encryptblock(&bfState, &dataPtr[n+0], &dataPtr[n+1]);

	blowfish_clean(&bfState);

	return(kNoErr);
}





//============================================================================
//		NDataCipher::Blowfish_Decrypt : Decrypt using Blowfish encryption.
//----------------------------------------------------------------------------
NStatus NDataCipher::Blowfish_Decrypt(NData &theData)
{	NIndex					n, numWords;
	UInt32					*dataPtr;
	blowfish_context_t		bfState;



	// Check our parameters
	if ((theData.GetSize() % 8) != 0)
		return(kNErrParam);



	// Get the state we need
	dataPtr  = (UInt32 *) theData.GetData();
	numWords =            theData.GetSize() / sizeof(UInt32);



	// Encrypt the data
	blowfish_initiate(&bfState, mKey.GetData(), mKey.GetSize());
	
	for (n = 0; n < numWords; n += 2)
		blowfish_decryptblock(&bfState, &dataPtr[n+0], &dataPtr[n+1]);

	blowfish_clean(&bfState);

	return(kNoErr);
}








