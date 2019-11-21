/*	NAME:
		NDataDigest.h

	DESCRIPTION:
		Data digests.

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
#ifndef NDATA_DIGEST_H
#define NDATA_DIGEST_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NData.h"





//=============================================================================
//		Templates
//-----------------------------------------------------------------------------
// Data digest
//
// Template to implement a digest of a fixed size.
template <size_t S>
class NDigest
{
public:
	NDigest()
		: mBytes{}
	{
		static_assert(sizeof(*this) == (S / 8), "NDigest is not packed!");
	}

	NDigest(const NDigest& otherDigest)
		: mBytes{}
	{
		operator=(otherDigest);
	}

	NDigest<S>& operator=(const NDigest<S>& otherDigest)
	{
		if (this != &otherDigest)
		{
			memcpy(mBytes, otherDigest.mBytes, sizeof(mBytes));
		}
		return *this;
	}


	// Is the digest valid?
	bool IsValid() const
	{
		for (uint8_t theByte : mBytes)
		{
			if (theByte != 0)
			{
				return true;
			}
		}

		return false;
	}


	// Clear the digest
	void Clear()
	{
		memset(mBytes, 0x00, sizeof(mBytes));
	}


	// Get/set the data
	NData GetData() const
	{
		return NData(sizeof(mBytes), mBytes);
	}

	void SetData(const NData& theData)
	{
		NN_REQUIRE(    theData.GetSize() == S);
		memcpy(mBytes, theData.GetData(),   S);
	}


	// Get the bytes
	const uint8_t* GetBytes() const
	{
		return mBytes;
	}

	uint8_t* GetMutableBytes()
	{
		return mBytes;
	}


	// Operators
	bool operator<(const NDigest& otherDigest) const
	{
		// For std::map
		return memcmp(mBytes, &otherDigest.mBytes, sizeof(mBytes));
	}

	bool operator>(const NDigest& otherDigest) const
	{
		// For comparisons
		return memcmp(mBytes, &otherDigest.mBytes, sizeof(mBytes));
	}

	bool operator==(const NDigest& otherDigest) const
	{
		// For std::unordered_map
		return memcmp(mBytes, &otherDigest.mBytes, sizeof(mBytes)) == 0;
	}

	bool operator!=(const NDigest& otherDigest) const
	{
		// For comparisons
		return memcmp(mBytes, &otherDigest.mBytes, sizeof(mBytes)) != 0;
	}

private:
	uint8_t                             mBytes[S / 8];
};





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Data digest
//
// Defines an 'NDigestX' for a specific size of X.
//
// Also provides a std::hash specialization for each NDigestX to allow the
// digest to be used as a key for std::unordered_map. We assume the hash
// is uniformly distributed so we can just take a size_t-sized chunk.
#define NDIGEST(_size)                                                  \
																		\
	using NDigest##_size = NDigest<_size>;                              \
																		\
	namespace std                                                       \
	{                                                                   \
	template <>                                                         \
	struct hash<NDigest##_size>                                         \
	{                                                                   \
		size_t operator()(const NDigest##_size& theDigest) const        \
		{                                                               \
			/* For std::unordered_map */                                \
			static_assert(sizeof(size_t) < (_size / 8),                 \
						  "NDigest" #_size " didn't fill std::hash");   \
																		\
			size_t theHash = 0;                                         \
			memcpy(&theHash, theDigest.GetBytes(), sizeof(theHash));    \
																		\
			return theHash;                                             \
		}                                                               \
	};                                                                  \
	}





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Digests
NDIGEST(128);
NDIGEST(160);





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NDataDigest
{
public:
	// Get a digest
	//
	// The checksum of empty data is always 0.
	static uint16_t                     GetInternet(const NData& theData, uint16_t          prevValue = 0);
	static uint32_t                     GetAdler32( const NData& theData, uint32_t          prevValue = 0);
	static uint32_t                     GetXXHash32(const NData& theData, uint32_t          prevValue = 0);
	static uint64_t                     GetXXHash64(const NData& theData, uint64_t          prevValue = 0);
	static NDigest128                   GetMD5(     const NData& theData, const NDigest128* prevValue = nullptr);
	static NDigest160                   GetSHA1(    const NData& theData, const NDigest160* prevValue = nullptr);

	static uint16_t                     GetInternet(size_t            theSize, const void* thePtr, uint16_t prevValue = 0);
	static uint32_t                     GetAdler32( size_t            theSize, const void* thePtr, uint32_t prevValue = 0);
	static uint32_t                     GetXXHash32(size_t            theSize, const void* thePtr, uint32_t prevValue = 0);
	static uint64_t                     GetXXHash64(size_t            theSize, const void* thePtr, uint64_t prevValue = 0);
	static NDigest128                   GetMD5(     size_t            theSize,
											   const void*       thePtr,
											   const NDigest128* prevValue = nullptr);
	static NDigest160                   GetSHA1(size_t            theSize,
												const void*       thePtr,
												const NDigest160* prevValue = nullptr);
};



#endif // NDATA_DIGEST_H
