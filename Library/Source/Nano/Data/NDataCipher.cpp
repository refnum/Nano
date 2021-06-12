/*	NAME:
		NDataCipher.cpp

	DESCRIPTION:
		Data encryption.

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

// Nano
#include "WjCryptLib_Aes.h"





//=============================================================================
//		NDataCipher::GetKey : Get the key.
//-----------------------------------------------------------------------------
NData NDataCipher::GetKey() const
{


	// Get the key
	return mKey;
}





//=============================================================================
//		NDataCipher::SetKey : Set the key.
//-----------------------------------------------------------------------------
void NDataCipher::SetKey(const NData& theKey)
{


	// Set the key
	mKey = theKey;
}





//=============================================================================
//		NDataCipher::Encrypt : Encrypt data.
//-----------------------------------------------------------------------------
NData NDataCipher::Encrypt(NCipher theCipher, const NData& srcData)
{


	// Validate our parameters and state
	NN_REQUIRE(!srcData.IsEmpty());
	NN_REQUIRE(!mKey.IsEmpty() || theCipher == NCipher::Null);



	// Encrypt the data
	NData dstData = srcData;

	switch (theCipher)
	{
		case NCipher::Null:
			Null_Encrypt(dstData);
			break;

		case NCipher::AES_256:
			AES256_Encrypt(dstData);
			break;
	}

	return dstData;
}





//=============================================================================
//		NDataCipher::Decrypt : Decrypt data.
//-----------------------------------------------------------------------------
NData NDataCipher::Decrypt(NCipher theCipher, const NData& srcData)
{


	// Validate our parameters and state
	NN_REQUIRE(!srcData.IsEmpty());
	NN_REQUIRE(!mKey.IsEmpty() || theCipher == NCipher::Null);



	// Decrypt the data
	NData dstData = srcData;

	switch (theCipher)
	{
		case NCipher::Null:
			Null_Decrypt(dstData);
			break;

		case NCipher::AES_256:
			AES256_Decrypt(dstData);
			break;
	}

	return dstData;
}





#pragma mark private
//=============================================================================
//		NDataCipher::Null_Encrypt : Null encryption.
//-----------------------------------------------------------------------------
void NDataCipher::Null_Encrypt(NData& /*theData*/)
{
}





//=============================================================================
//		NDataCipher::Null_Decrypt : Null dencryption.
//-----------------------------------------------------------------------------
void NDataCipher::Null_Decrypt(NData& /*theData*/)
{
}





//=============================================================================
//		NDataCipher::AES256_Encrypt : AES-256 encryption.
//-----------------------------------------------------------------------------
void NDataCipher::AES256_Encrypt(NData& theData)
{


	// Validate our parameters and state
	NN_REQUIRE((theData.GetSize() % AES_BLOCK_SIZE) == 0);
	NN_REQUIRE(mKey.GetSize() == 32);



	// Prepare the context
	AesContext aesContext{};

	int sysErr = AesInitialise(&aesContext, mKey.GetData(), uint32_t(mKey.GetSize()));
	NN_REQUIRE(sysErr == 0);



	// Encrypt the data
	uint8_t* theBlock  = theData.GetMutableData();
	size_t   numBlocks = theData.GetSize() / AES_BLOCK_SIZE;

	for (size_t n = 0; n < numBlocks; n++)
	{
		AesEncryptInPlace(&aesContext, theBlock);
		theBlock += AES_BLOCK_SIZE;
	}
}





//=============================================================================
//		NDataCipher::AES256_Decrypt : AES-256 decryption.
//-----------------------------------------------------------------------------
void NDataCipher::AES256_Decrypt(NData& theData)
{


	// Validate our parameters and state
	NN_REQUIRE((theData.GetSize() % AES_BLOCK_SIZE) == 0);
	NN_REQUIRE(mKey.GetSize() == 32);



	// Prepare the context
	AesContext aesContext{};

	int sysErr = AesInitialise(&aesContext, mKey.GetData(), uint32_t(mKey.GetSize()));
	NN_REQUIRE(sysErr == 0);



	// Decrypt the data
	uint8_t* theBlock  = theData.GetMutableData();
	size_t   numBlocks = theData.GetSize() / AES_BLOCK_SIZE;

	for (size_t n = 0; n < numBlocks; n++)
	{
		AesDecryptInPlace(&aesContext, theBlock);
		theBlock += AES_BLOCK_SIZE;
	}
}
