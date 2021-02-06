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
// Nano
#include "NData.h"
#include "NMixinAppendable.h"
#include "NMixinComparable.h"
#include "NMixinContainer.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NRange;
struct NBitPointer;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NBitVector final
	: public NMixinAppendable<NBitVector>
	, public NMixinComparable<NBitVector>
	, public NMixinContainer<NBitVector>
{
public:
										NBitVector();
									   ~NBitVector() = default;

										NBitVector(const NBitVector& otherVector);
	NBitVector&                         operator=( const NBitVector& otherVector);

										NBitVector(NBitVector&& otherVector);
	NBitVector&                         operator=( NBitVector&& otherVector);


	// Clear the vector
	void                                Clear();


	// Get / set the size
	//
	// Increasing the size will zero-fill any new bits.
	size_t                              GetSize() const;
	void                                SetSize(size_t numBits);


	// Get/set the capacity
	//
	// The capacity is the maximum size available before reallocation.
	//
	// Reducing the capacity below the current size will also reduce
	// the current size.
	//
	// Increasing the capacity above the current size may round the
	// requested capacity upwards.
	size_t                              GetCapacity() const;
	void                                SetCapacity(size_t numBits);


	// Get / set the data
	//
	// GetData rounds the size up to the nearest byte, with bytes outside
	// the vector being set to 0.
	NData                               GetData() const;
	void                                SetData(  const NData& theData);


	// Get / set a bit
	bool                                GetBit(size_t theIndex) const;
	void                                SetBit(size_t theIndex, bool theValue);


	// Get / set bits in a range
	//
	// HasBit returns as any bit in the range has the specified value.
	bool                                HasBit( bool theValue, const NRange& theRange = kNRangeAll) const;
	void                                SetBits(bool theValue, const NRange& theRange = kNRangeAll);


	// Append a bit
	void                                AppendBit(bool theValue);


	// Find a bit
	//
	// Returns the first / last instance of a bit in a range.
	size_t                              FindFirst(bool theValue, const NRange& theRange = kNRangeAll) const;
	size_t                              FindLast( bool theValue, const NRange& theRange = kNRangeAll) const;


	// Count bits
	size_t                              CountBits(        bool theValue, const NRange& theRange = kNRangeAll) const;
	size_t                              CountLeadingBits( bool theValue) const;
	size_t                              CountTrailingBits(bool theValue) const;


	// Flip bits
	void                                FlipBit(size_t theIndex);
	void                                FlipBits(const NRange& theRange = kNRangeAll);


	// Operators
	NBitVector                          operator~() const;

	NBitVector                          operator&(const NBitVector& theVector) const;
	NBitVector                          operator|(const NBitVector& theVector) const;
	NBitVector                          operator^(const NBitVector& theVector) const;

	NBitVector&                         operator&=(const NBitVector& theVector);
	NBitVector&                         operator|=(const NBitVector& theVector);
	NBitVector&                         operator^=(const NBitVector& theVector);


public:
	// NMixinAppendable
	void                                Append(const NBitVector& theVector);


	// NMixinComparable
	bool                                CompareEqual(const NBitVector& theVector) const;
	NComparison                         CompareOrder(const NBitVector& theVector) const;



private:
	NBitPointer                         GetBitPointer(size_t theIndex) const;
	size_t                              GetByteSize(  size_t numBits)  const;


private:
	NData                               mData;
	size_t                              mSize;
	mutable uint8_t*                    mBytes;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NBitVector.inl"


#endif // NBIT_VECTOR_H
