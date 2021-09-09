/*	NAME:
		NDataDigest.cpp

	DESCRIPTION:
		Data digests.

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
#include "NDataDigest.h"

// Nano
#include "Nano_zlib.h"
#include "WjCryptLib_Md5.h"
#include "WjCryptLib_Sha1.h"
#include "WjCryptLib_Sha256.h"





//=============================================================================
//		NDataDigest::GetInternet : Get the internet digest.
//-----------------------------------------------------------------------------
uint16_t NDataDigest::GetInternet(const NData& theData, uint16_t prevValue)
{


	// Get the digest
	return GetInternet(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetAdler32 : Get an Adler32 digest.
//-----------------------------------------------------------------------------
uint32_t NDataDigest::GetAdler32(const NData& theData, uint32_t prevValue)
{


	// Get the digest
	return GetAdler32(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetXXHash32 : Get an XXH32 digest.
//-----------------------------------------------------------------------------
uint32_t NDataDigest::GetXXHash32(const NData& theData, uint32_t prevValue)
{


	// Get the digest
	return GetXXHash32(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetXXHash64 : Get an XXH64 digest.
//-----------------------------------------------------------------------------
uint64_t NDataDigest::GetXXHash64(const NData& theData, uint64_t prevValue)
{


	// Get the digest
	return GetXXHash64(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetXXHash3 : Get an XXH3 digest.
//-----------------------------------------------------------------------------
uint64_t NDataDigest::GetXXHash3(const NData& theData, uint64_t prevValue)
{


	// Get the digest
	return GetXXHash3(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetMD5 : Get an MD5 digest.
//-----------------------------------------------------------------------------
NDigest128 NDataDigest::GetMD5(const NData& theData, const NDigest128* prevValue)
{


	// Get the digest
	return GetMD5(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetSHA1 : Get an SHA1 digest.
//-----------------------------------------------------------------------------
NDigest160 NDataDigest::GetSHA1(const NData& theData, const NDigest160* prevValue)
{


	// Get the digest
	return GetSHA1(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetSHA256 : Get an SHA256 digest.
//-----------------------------------------------------------------------------
NDigest256 NDataDigest::GetSHA256(const NData& theData, const NDigest256* prevValue)
{


	// Get the digest
	return GetSHA256(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetInternet : Get the internet digest.
//-----------------------------------------------------------------------------
//		Note : From <http://www.rfc-editor.org/rfc/rfc1071.txt>.
//----------------------------------------------------------------------------
uint16_t NDataDigest::GetInternet(size_t theSize, const void* thePtr, uint16_t prevValue)
{


	// Get the digest
	uint32_t theDigest = prevValue;

	if (theSize != 0)
	{
		// Calculate the digest
		const uint8_t* dataPtr = static_cast<const uint8_t*>(thePtr);

		while (theSize > 1)
		{
			uint32_t hi = *dataPtr++;
			uint32_t lo = *dataPtr++;

			theDigest += ((hi << 8) | lo);
			theSize -= 2;
		}

		if (theSize > 0)
		{
			theDigest += *dataPtr++;
		}



		// Fold the 32-bit digest down to 16 bits
		while (theDigest >> 16)
		{
			theDigest = (theDigest & 0xffff) + (theDigest >> 16);
		}

		theDigest = ~theDigest;
		theDigest &= 0x0000FFFF;
	}

	return uint16_t(theDigest);
}





//=============================================================================
//		NDataDigest::GetAdler32 : Get an Adler32 digest.
//-----------------------------------------------------------------------------
uint32_t NDataDigest::GetAdler32(size_t theSize, const void* thePtr, uint32_t prevValue)
{


	// Get the digest
	uint32_t theDigest = prevValue;

	if (theSize != 0)
	{
		theDigest = uint32_t(adler32(prevValue, static_cast<const Bytef*>(thePtr), uInt(theSize)));
	}

	return theDigest;
}





//=============================================================================
//		NDataDigest::GetXXHash32 : Get an XXH32 digest.
//-----------------------------------------------------------------------------
uint32_t NDataDigest::GetXXHash32(size_t theSize, const void* thePtr, uint32_t prevValue)
{


	// Get the digest
	uint32_t theDigest = prevValue;

	if (theSize != 0)
	{
		NN_DIAGNOSTIC_PUSH();
		NN_DIAGNOSTIC_IGNORE_CLANG("-Wused-but-marked-unused");
		NN_DIAGNOSTIC_IGNORE_GCC("-Wpragmas");

		theDigest = XXH32(thePtr, theSize, prevValue);

		NN_DIAGNOSTIC_POP();
	}

	return theDigest;
}





//=============================================================================
//		NDataDigest::GetXXHash64 : Get an XXH64 digest.
//-----------------------------------------------------------------------------
uint64_t NDataDigest::GetXXHash64(size_t theSize, const void* thePtr, uint64_t prevValue)
{
	uint64_t theDigest = prevValue;

	if (theSize != 0)
	{
		NN_DIAGNOSTIC_PUSH();
		NN_DIAGNOSTIC_IGNORE_CLANG("-Wused-but-marked-unused");
		NN_DIAGNOSTIC_IGNORE_GCC("-Wpragmas");

		theDigest = XXH64(thePtr, theSize, prevValue);

		NN_DIAGNOSTIC_POP();
	}

	return theDigest;
}





//=============================================================================
//		NDataDigest::GetXXHash3 : Get an XXH33 digest.
//-----------------------------------------------------------------------------
uint64_t NDataDigest::GetXXHash3(size_t theSize, const void* thePtr, uint64_t prevValue)
{
	uint64_t theDigest = prevValue;

	if (theSize != 0)
	{
		NN_DIAGNOSTIC_PUSH();
		NN_DIAGNOSTIC_IGNORE_CLANG("-Wused-but-marked-unused");
		NN_DIAGNOSTIC_IGNORE_GCC("-Wpragmas");

		theDigest = XXH3_64bits_withSeed(thePtr, theSize, prevValue);

		NN_DIAGNOSTIC_POP();
	}

	return theDigest;
}





//=============================================================================
//		NDataDigest::GetMD5 : Get an MD5 digest.
//-----------------------------------------------------------------------------
NDigest128 NDataDigest::GetMD5(size_t theSize, const void* thePtr, const NDigest128* prevValue)
{


	// Validate our parameters
	NN_REQUIRE(theSize <= kNUInt32Max);



	// Get the state we need
	NDigest128 theDigest;

	if (prevValue != nullptr)
	{
		theDigest = *prevValue;
	}



	// Get the digest
	if (theSize != 0)
	{
		Md5Context theContext;
		Md5Initialise(&theContext);

		if (prevValue != nullptr)
		{
			Md5Update(&theContext, prevValue->GetBytes(), sizeof(theDigest));
		}

		Md5Update(&theContext, static_cast<const uint8_t*>(thePtr), uint32_t(theSize));
		Md5Finalise(&theContext, reinterpret_cast<MD5_HASH*>(theDigest.GetMutableBytes()));
	}

	return theDigest;
}





//=============================================================================
//		NDataDigest::GetSHA1 : Get an SHA1 digest.
//-----------------------------------------------------------------------------
NDigest160 NDataDigest::GetSHA1(size_t theSize, const void* thePtr, const NDigest160* prevValue)
{


	// Validate our parameters
	NN_REQUIRE(theSize <= kNUInt32Max);



	// Get the state we need
	NDigest160 theDigest;

	if (prevValue != nullptr)
	{
		theDigest = *prevValue;
	}



	// Get the digest
	if (theSize != 0)
	{
		Sha1Context theContext;
		Sha1Initialise(&theContext);

		if (prevValue != nullptr)
		{
			Sha1Update(&theContext, prevValue->GetBytes(), sizeof(theDigest));
		}

		Sha1Update(&theContext, static_cast<const uint8_t*>(thePtr), uint32_t(theSize));
		Sha1Finalise(&theContext, reinterpret_cast<SHA1_HASH*>(theDigest.GetMutableBytes()));
	}

	return theDigest;
}





//=============================================================================
//		NDataDigest::GetSHA256 : Get an SHA256 digest.
//-----------------------------------------------------------------------------
NDigest256 NDataDigest::GetSHA256(size_t theSize, const void* thePtr, const NDigest256* prevValue)
{


	// Validate our parameters
	NN_REQUIRE(theSize <= kNUInt32Max);



	// Get the state we need
	NDigest256 theDigest;

	if (prevValue != nullptr)
	{
		theDigest = *prevValue;
	}



	// Get the digest
	if (theSize != 0)
	{
		Sha256Context theContext;
		Sha256Initialise(&theContext);

		if (prevValue != nullptr)
		{
			Sha256Update(&theContext, prevValue->GetBytes(), sizeof(theDigest));
		}

		Sha256Update(&theContext, static_cast<const uint8_t*>(thePtr), uint32_t(theSize));
		Sha256Finalise(&theContext, reinterpret_cast<SHA256_HASH*>(theDigest.GetMutableBytes()));
	}

	return theDigest;
}
