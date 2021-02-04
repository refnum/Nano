/*	NAME:
		NBitVector.h

	DESCRIPTION:
		Bit vector.

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
#ifndef NBIT_VECTOR_H
#define NBIT_VECTOR_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NComparable.h"
#include "NContainer.h"
#include "NData.h"
#include "NEncodable.h"
#include "NRange.h"
#include "NStringFormatter.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NBitVector
	: public NContainer
	, public NEncodable
	, public NComparable<NBitVector>
{
public:
										NENCODABLE_DECLARE(NBitVector);

										NBitVector(const NBitVector& theValue);

										NBitVector();
	virtual                            ~NBitVector();


	// Clear the vector
	void                                Clear();


	// Compare the value
	NComparison                         Compare(const NBitVector& theValue) const;


	// Get/set the vector size
	//
	// Increasing the size will append 0'd bits to the vector.
	//
	// To amortize the cost of repeatedly growing a bit vector, extra capacity can
	// be requested to reserve storage for at least that many extra bits. These bits
	// can not be accessed until a subsequent SetSize increases the vector size.
	NIndex                              GetSize() const;
	void                                SetSize(NIndex theSize, NIndex extraCapacity = 0);


	// Get/set a bit
	inline bool                         GetBit(NIndex theIndex) const;
	inline void                         SetBit(NIndex theIndex, bool theValue);


	// Get/set the bits
	//
	// Bits are returned in the least-most-significant numBits bits of the result.
	NData                               GetBits() const;
	inline uint32_t                     GetBits(NIndex theIndex, NIndex numBits) const;

	void                                SetBits(bool theValue);
	void                                SetBits(const NData& theData);


	// Append bits
	//
	// Bits are read from the least-most-significant numBits of theValue.
	void                                AppendBit(bool      theValue);
	void                                AppendBits(uint32_t theValue, NIndex numBits);


	// Flip bits
	void                                FlipBit(NIndex theIndex);
	void                                FlipBits(const NRange& theRange = kNRangeAll);


	// Test bits
	NIndex                              CountBits(  bool theValue, const NRange& theRange = kNRangeAll) const;
	bool                                ContainsBit(bool theValue, const NRange& theRange = kNRangeAll) const;


	// Find bits
	NIndex                              FindFirstBit(bool theValue, const NRange& theRange = kNRangeAll) const;
	NIndex                              FindLastBit( bool theValue, const NRange& theRange = kNRangeAll) const;


	// Operators
	const NBitVector&                   operator=(const NBitVector& theObject);
					  operator NFormatArgument()  const;


protected:
	// Encode/decode the object
	void                                EncodeSelf(      NEncoder& theEncoder) const;
	void                                DecodeSelf(const NEncoder& theEncoder);


private:
	void                                UpdateSize( NIndex numBits);
	NIndex                              GetByteSize(NIndex numBits) const;

	inline uint8_t*                     GetByteForBit(NIndex theIndex, NIndex& bitIndex) const;


private:
	NData                               mData;
	NIndex                              mSize;
	mutable uint8_t*                    mBytes;
};





//=============================================================================
//		NBitVector::GetByteForBit : Get the byte that contains a bit.
//-----------------------------------------------------------------------------
//		Note : Inlined for performance.
//----------------------------------------------------------------------------
inline uint8_t* NBitVector::GetByteForBit(NIndex theIndex, NIndex& bitIndex) const
{
	NIndex                              byteIndex;


	// Validate our parameters
										NN_ASSERT(theIndex >= 0 && theIndex < GetSize());


	// Get the byte
	//
	// The bit index as the index within the byte, not the vector.
	byteIndex = theIndex /              8;
	bitIndex  = 7 - (theIndex - (byteIndex * 8));

	return &mBytes[byteIndex];
}





//=============================================================================
//		NBitVector::GetBit : Get a bit.
//-----------------------------------------------------------------------------
//		Note : Inlined for performance.
//----------------------------------------------------------------------------
inline bool NBitVector::GetBit(NIndex theIndex) const
{
	NIndex  bitIndex;
	uint8_t theByte;



	// Get the bit
	theByte = *GetByteForBit(theIndex, bitIndex);

	return (theByte >> bitIndex) & 0x01;
}





//=============================================================================
//		NBitVector::GetBits : Get the bits.
//-----------------------------------------------------------------------------
//		Note : Inlined for performance.
//----------------------------------------------------------------------------
inline uint32_t NBitVector::GetBits(NIndex theIndex, NIndex numBits) const
{
	uint32_t theResult, theMask;
	NIndex   n;



	// Validate our parameters
	NN_ASSERT((theIndex + numBits) <= GetSize());
	NN_ASSERT(numBits >= 1 && numBits <= 32);



	// Get the bits
	theResult = 0;
	theMask   = (1 << (numBits - 1));

	for (n = 0; n < numBits; n++)
	{
		if (GetBit(theIndex + n))
		{
			theResult |= theMask;
		}

		theMask >>= 1;
	}

	return theResult;
}





//=============================================================================
//		NBitVector::SetBit : Set a bit.
//-----------------------------------------------------------------------------
//		Note : Inlined for performance.
//----------------------------------------------------------------------------
inline void NBitVector::SetBit(NIndex theIndex, bool theValue)
{
	NIndex   bitIndex;
	uint8_t* bytePtr;



	// Set the bit
	bytePtr = GetByteForBit(theIndex, bitIndex);

	if (theValue)
	{
		*bytePtr |= (1 << bitIndex);
	}
	else
	{
		*bytePtr &= ~(1 << bitIndex);
	}
}



#endif // NBIT_VECTOR_H
