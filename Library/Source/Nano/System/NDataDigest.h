/*	NAME:
		NDataDigest.h

	DESCRIPTION:
		Data digests.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
template <size_t N>
class NDigestX
{
public:
										NDigestX(size_t theSize, const uint8_t* thePtr);

										NDigestX();

										NDigestX( const NDigestX&    otherDigest);
	NDigestX<N>&                        operator=(const NDigestX<N>& otherDigest);


	// Is the digest valid?
	bool                                IsValid() const;


	// Clear the digest
	void                                Clear();


	// Get/set the data
	NData                               GetData() const;
	void                                SetData(  const NData& theData);


	// Get the bytes
	const uint8_t*                      GetBytes() const;
	uint8_t*                            GetMutableBytes();


	// Operators
	bool                                operator<( const NDigestX& otherDigest) const;
	bool                                operator>( const NDigestX& otherDigest) const;
	bool                                operator==(const NDigestX& otherDigest) const;
	bool                                operator!=(const NDigestX& otherDigest) const;


private:
	uint8_t                             mBytes[N / 8];
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
	using NDigest##_size = NDigestX<_size>;                             \
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
	// Get a runtime digest
	//
	// The algorithm used for the runtime digest is selected at runtime,
	// choosing the most peformant digest for the given size of input.
	//
	// A runtime digest must never be serialised - the algorithm used will
	// vary between systems.
	static size_t                       GetRuntime(   const NData& theData, size_t            prevValue = 0);
	static uint32_t                     GetRuntime32( const NData& theData, uint32_t          prevValue = 0);
	static uint64_t                     GetRuntime64( const NData& theData, uint64_t          prevValue = 0);
	static NDigest128                   GetRuntime128(const NData& theData, const NDigest128* prevValue = nullptr);

	static size_t                       GetRuntime(   size_t            theSize, const void* thePtr, size_t   prevValue = 0);
	static uint32_t                     GetRuntime32( size_t            theSize, const void* thePtr, uint32_t prevValue = 0);
	static uint64_t                     GetRuntime64( size_t            theSize, const void* thePtr, uint64_t prevValue = 0);
	static NDigest128                   GetRuntime128(size_t            theSize,
													  const void*       thePtr,
													  const NDigest128* prevValue = nullptr);


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





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDataDigest.inl"



#endif // NDATA_DIGEST_H
