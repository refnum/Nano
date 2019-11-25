/*	NAME:
		NDataDigest.inl

	DESCRIPTION:
		NDataDigest inline methods.

		Inlining hash methods with a known input size can produce significantly
		faster hashing for small input sizes:

		https://fastcompression.blogspot.com/2018/03/xxhash-for-small-keys-impressive-power.html.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
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
#include "Nano_xxHash.h"



#pragma mark NDigest





//=============================================================================
//		NDigestX::NDigestX : Constructor.
//-----------------------------------------------------------------------------
template <size_t N>
NDigestX<N>::NDigestX(size_t theSize, const uint8_t* thePtr)
	: mBytes{}
{
	NN_REQUIRE(theSize == N);
	NN_UNUSED(theSize);

	memcpy(&mBytes, thePtr, sizeof(mBytes));
}





//=============================================================================
//		NDigestX::NDigestX : Constructor.
//-----------------------------------------------------------------------------
template <size_t N>
NDigestX<N>::NDigestX()
	: mBytes{}
{
	static_assert(sizeof(*this) == (N / 8), "NDigestX is not packed!");
}





//=============================================================================
//		NDigestX::NDigestX : Constructor.
//-----------------------------------------------------------------------------
template <size_t N>
NDigestX<N>::NDigestX(const NDigestX& otherDigest)
	: mBytes{}
{
	operator=(otherDigest);
}





//=============================================================================
//		NDigestX::operator= : Assignment operator.
//-----------------------------------------------------------------------------
template <size_t N>
NDigestX<N>& NDigestX<N>::operator=(const NDigestX<N>& otherDigest)
{


	// Assign the digest
	if (this != &otherDigest)
	{
		memcpy(mBytes, otherDigest.mBytes, sizeof(mBytes));
	}

	return *this;
}





//=============================================================================
//		NDigestX::IsValid : Is the digest valid?
//-----------------------------------------------------------------------------
template <size_t N>
bool NDigestX<N>::IsValid() const
{


	// Check the bytes
	for (uint8_t theByte : mBytes)
	{
		if (theByte != 0)
		{
			return true;
		}
	}

	return false;
}





//=============================================================================
//		NDigestX::Clear : Clear the digest.
//-----------------------------------------------------------------------------
template <size_t N>
void NDigestX<N>::Clear()
{


	// Clear the digest
	memset(mBytes, 0x00, sizeof(mBytes));
}





//=============================================================================
//		NDigestX::GetData : Get the data.
//-----------------------------------------------------------------------------
template <size_t N>
NData NDigestX<N>::GetData() const
{


	// Get the data
	return NData(sizeof(mBytes), mBytes);
}





//=============================================================================
//		NDigestX::SetData : Set the data.
//-----------------------------------------------------------------------------
template <size_t N>
void NDigestX<N>::SetData(const NData& theData)
{


	// Validate our parameters
	NN_REQUIRE(theData.GetSize() == N);



	// Set the data
	memcpy(mBytes, theData.GetData(), N);
}





//=============================================================================
//		NDigestX::GetBytes : Get the bytes.
//-----------------------------------------------------------------------------
template <size_t N>
const uint8_t* NDigestX<N>::GetBytes() const
{


	// Get the bytes
	return mBytes;
}





//=============================================================================
//		NDigestX::GetMutableBytes : Get mutable access to the bytes.
//-----------------------------------------------------------------------------
template <size_t N>
uint8_t* NDigestX<N>::GetMutableBytes()
{


	// Get the bytes
	return mBytes;
}





//=============================================================================
//		NDigestX::operator< : Less-than operator.
//-----------------------------------------------------------------------------
template <size_t N>
bool NDigestX<N>::operator<(const NDigestX<N>& otherDigest) const
{


	// For std::map
	return memcmp(mBytes, &otherDigest.mBytes, sizeof(mBytes));
}





//=============================================================================
//		NDigestX::operator> : Greater-than operator.
//-----------------------------------------------------------------------------
template <size_t N>
bool NDigestX<N>::operator>(const NDigestX<N>& otherDigest) const
{


	// For comparisons
	return memcmp(mBytes, &otherDigest.mBytes, sizeof(mBytes));
}





//=============================================================================
//		NDigestX::operator== : Equality operator.
//-----------------------------------------------------------------------------
template <size_t N>
bool NDigestX<N>::operator==(const NDigestX<N>& otherDigest) const
{


	// For std::unordered_map
	return memcmp(mBytes, &otherDigest.mBytes, sizeof(mBytes)) == 0;
}





//=============================================================================
//		NDigestX::operator!= : Inequaliy operator.
//-----------------------------------------------------------------------------
template <size_t N>
bool NDigestX<N>::operator!=(const NDigestX<N>& otherDigest) const
{


	// For comparisons
	return memcmp(mBytes, &otherDigest.mBytes, sizeof(mBytes)) != 0;
}





#pragma mark NDataDigest
//=============================================================================
//		NDataDigest::GetRuntime : Get the runtime digest.
//-----------------------------------------------------------------------------
size_t NDataDigest::GetRuntime(const NData& theData, size_t prevValue)
{


	// Get the digest
#if NN_ARCH_64
	return GetRuntime64(theData.GetSize(), theData.GetData(), prevValue);
#else
	return GetRuntime32(theData.GetSize(), theData.GetData(), prevValue);
#endif
}





//=============================================================================
//		NDataDigest::GetRuntime32 : Get the runtime digest.
//-----------------------------------------------------------------------------
uint32_t NDataDigest::GetRuntime32(const NData& theData, uint32_t prevValue)
{
	return GetRuntime32(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetRuntime64 : Get the runtime digest.
//-----------------------------------------------------------------------------
uint64_t NDataDigest::GetRuntime64(const NData& theData, uint64_t prevValue)
{
	return GetRuntime64(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetRuntime128 : Get the runtime digest.
//-----------------------------------------------------------------------------
NDigest128 NDataDigest::GetRuntime128(const NData& theData, const NDigest128* prevValue)
{


	// Get the digest
	return GetRuntime128(theData.GetSize(), theData.GetData(), prevValue);
}





//=============================================================================
//		NDataDigest::GetRuntime : Get the runtime digest.
//-----------------------------------------------------------------------------
size_t NDataDigest::GetRuntime(size_t theSize, const void* thePtr, size_t prevValue)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"

	// Get the digest
	size_t theDigest = prevValue;

	if (theSize != 0)
	{
#if NN_ARCH_64
		theDigest = XXH64(thePtr, theSize, prevValue);
#else
		theDigest      = XXH32(thePtr, theSize, prevValue);
#endif
	}

	return theDigest;

#pragma clang diagnostic pop
}





//=============================================================================
//		NDataDigest::GetRuntime32 : Get the runtime digest.
//-----------------------------------------------------------------------------
uint32_t NDataDigest::GetRuntime32(size_t theSize, const void* thePtr, uint32_t prevValue)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"

	// Get the digest
	uint32_t theDigest = prevValue;

	if (theSize != 0)
	{
#if NN_ARCH_64
		theDigest = uint32_t(XXH64(thePtr, theSize, prevValue));
#else
		theDigest      = XXH32(thePtr, theSize, prevValue);
#endif
	}

	return theDigest;

#pragma clang diagnostic pop
}





//=============================================================================
//		NDataDigest::GetRuntime64 : Get the runtime digest.
//-----------------------------------------------------------------------------
uint64_t NDataDigest::GetRuntime64(size_t theSize, const void* thePtr, uint64_t prevValue)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"


	// Get the digest
	uint64_t theDigest = prevValue;

	if (theDigest != 0)
	{
#if NN_ARCH_64
		theDigest = XXH64(thePtr, theSize, prevValue);
#else
		uint32_t hash1 = XXH32(thePtr, theSize, prevValue);
		uint32_t hash2 = XXH32(thePtr, theSize, hash1);

		theDigest = (uint64_t(hash1) << 32) | uint64_t(hash2);
#endif
	}

	return theDigest;

#pragma clang diagnostic pop
}





//=============================================================================
//		NDataDigest::GetRuntime128 : Get the runtime digest.
//-----------------------------------------------------------------------------
NDigest128 NDataDigest::GetRuntime128(size_t            theSize,
									  const void*       thePtr,
									  const NDigest128* prevValue)
{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wused-but-marked-unused"

	// Get the state we need
	NDigest128   theDigest;
	XXH64_hash_t xxSeed64 = 0;

	if (prevValue != nullptr)
	{
		theDigest = *prevValue;

		static_assert(sizeof(xxSeed64) < sizeof(theDigest));
		memcpy(&xxSeed64, theDigest.GetBytes(), sizeof(xxSeed64));
	}


	// Get the digest
	//
	// xxh3 has yet to be finalised but is safe to use as a runtime hash.
	if (theSize != 0)
	{
		XXH128_hash_t xxDigest128 = XXH128(thePtr, theSize, xxSeed64);

		static_assert(sizeof(xxDigest128) == sizeof(NDigest128));
		theDigest = NDigest128(sizeof(xxDigest128), reinterpret_cast<const uint8_t*>(&xxDigest128));
	}

	return theDigest;

#pragma clang diagnostic pop
}
