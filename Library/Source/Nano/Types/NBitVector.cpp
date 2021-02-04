/*	NAME:
		NBitVector.cpp

	DESCRIPTION:
		Bit vector.

		Although std::vector<bool> provides a specialization that uses 1 bit per
		value, it does not provide an efficient way to get/set the contents as a
		block of memory (and std::bitset requires a fixed size on creation).

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
#include "NBitVector.h"
#include "NEncoder.h"
#include "NString.h"





//=============================================================================
//		Implementation
//-----------------------------------------------------------------------------
NENCODABLE_DEFINE(NBitVector);





//=============================================================================
//		NBitVector::NBitVector : Constructor.
//-----------------------------------------------------------------------------
NBitVector::NBitVector(const NBitVector& theValue)
	: NContainer()
	, NEncodable()
	, NComparable<NBitVector>()
{


	// Initialize ourselves
	mData = theValue.mData;
	UpdateSize(theValue.mSize);
}





//=============================================================================
//		NBitVector::NBitVector : Constructor.
//-----------------------------------------------------------------------------
NBitVector::NBitVector()
{


	// Initialize ourselves
	mSize  = 0;
	mBytes = NULL;
}





//=============================================================================
//		NBitVector::~NBitVector : Destructor.
//-----------------------------------------------------------------------------
NBitVector::~NBitVector()
{
}





//=============================================================================
//		NBitVector::Clear : Clear the vector.
//-----------------------------------------------------------------------------
void NBitVector::Clear()
{


	// Clear the vector
	SetSize(0);
}





//=============================================================================
//		NBitVector::Compare : Compare the value.
//-----------------------------------------------------------------------------
NComparison NBitVector::Compare(const NBitVector& theValue) const
{
	NComparison theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = CompareData(mSize / 8, mBytes, theValue.mSize / 8, theValue.mBytes);

	return theResult;
}





//=============================================================================
//		NBitVector::GetSize : Get the size.
//-----------------------------------------------------------------------------
NIndex NBitVector::GetSize() const
{


	// Get the size
	return mSize;
}





//=============================================================================
//		NBitVector::SetSize : Set the vector size.
//-----------------------------------------------------------------------------
void NBitVector::SetSize(NIndex theSize, NIndex extraCapacity)
{
	NIndex numBytes;



	// Set the size
	numBytes = GetByteSize(theSize) + GetByteSize(extraCapacity);
	mData.SetSize(numBytes);

	UpdateSize(theSize);
}





//=============================================================================
//		NBitVector::GetBits : Get the bits.
//-----------------------------------------------------------------------------
NData NBitVector::GetBits() const
{
	NBitVector* thisPtr;
	NIndex      theSize;



	// Shrink the buffer
	//
	// Since the buffer may have been over-allocated to minimise resize costs,
	// we may need to compact it to contain just the valid bytes.
	theSize = GetByteSize(GetSize());
	if (mData.GetSize() != theSize)
	{
		thisPtr = const_cast<NBitVector*>(this);

		thisPtr->mData.SetSize(theSize);
		thisPtr->UpdateSize(mSize);
	}



	// Get the bits
	return mData;
}





//=============================================================================
//		NBitVector::SetBits : Set the bits.
//-----------------------------------------------------------------------------
void NBitVector::SetBits(bool theValue)
{


	// Set the bits
	memset(mData.GetData(), theValue ? 0xFF : 0x00, mData.GetSize());
}





//=============================================================================
//		NBitVector::SetBits : Set the bits.
//-----------------------------------------------------------------------------
void NBitVector::SetBits(const NData& theData)
{


	// Set the bits
	mData = theData;
	UpdateSize(mData.GetSize() * 8);
}





//=============================================================================
//		NBitVector::AppendBit : Append a bit.
//-----------------------------------------------------------------------------
void NBitVector::AppendBit(bool theValue)
{


	// Add the bit
	AppendBits(theValue ? 0x01 : 0x00, 1);
}





//=============================================================================
//		NBitVector::AppendBits : Append bits.
//-----------------------------------------------------------------------------
void NBitVector::AppendBits(uint32_t theValue, NIndex numBits)
{
	NIndex   n, theSize;
	uint32_t theMask;



	// Validate our parameters
	NN_ASSERT(numBits >= 1 && numBits <= 32);



	// Get the state we need
	theSize = GetSize();
	theMask = (1 << (numBits - 1));



	// Grow the buffer
	//
	// To reduce the cost of repeatedly growing the buffer, we
	// pre-allocate to ensure most appends do not need to grow.
	SetSize(theSize + numBits, kNKilobyte * 8);



	// Append the bits
	for (n = 0; n < numBits; n++)
	{
		SetBit(theSize + n, (theValue & theMask) != 0);
		theMask >>= 1;
	}
}





//=============================================================================
//		NBitVector::FlipBit : Flip a bit.
//-----------------------------------------------------------------------------
void NBitVector::FlipBit(NIndex theIndex)
{


	// Flip the bit
	SetBit(theIndex, !GetBit(theIndex));
}





//=============================================================================
//		NBitVector::FlipBits : Flip the bits.
//-----------------------------------------------------------------------------
void NBitVector::FlipBits(const NRange& theRange)
{
	NIndex n, firstBit, lastBit;
	NRange finalRange;



	// Get the state we need
	finalRange = GetNormalized(theRange);
	firstBit   = finalRange.GetFirst();
	lastBit    = finalRange.GetLast();



	// Flip the bits
	for (n = firstBit; n <= lastBit; n++)
	{
		SetBit(n, !GetBit(n));
	}
}





//=============================================================================
//		NBitVector::CountBits : Count the bits with a specific value.
//-----------------------------------------------------------------------------
NIndex NBitVector::CountBits(bool theValue, const NRange& theRange) const
{
	NIndex n, firstBit, lastBit, numFound;
	NRange finalRange;



	// Get the state we need
	finalRange = GetNormalized(theRange);
	firstBit   = finalRange.GetFirst();
	lastBit    = finalRange.GetLast();



	// Count the bits
	numFound = 0;

	for (n = firstBit; n <= lastBit; n++)
	{
		if (GetBit(n) == theValue)
		{
			numFound++;
		}
	}

	return numFound;
}





//=============================================================================
//		NBitVector::ContainsBit : Does a range contain a value?
//-----------------------------------------------------------------------------
bool NBitVector::ContainsBit(bool theValue, const NRange& theRange) const
{
	NIndex n, firstBit, lastBit;
	NRange finalRange;



	// Get the state we need
	finalRange = GetNormalized(theRange);
	firstBit   = finalRange.GetFirst();
	lastBit    = finalRange.GetLast();



	// Test the bits
	for (n = firstBit; n <= lastBit; n++)
	{
		if (GetBit(n) == theValue)
		{
			return true;
		}
	}

	return false;
}





//=============================================================================
//		NBitVector::FindFirstBit : Find the first bit with a specific value.
//-----------------------------------------------------------------------------
NIndex NBitVector::FindFirstBit(bool theValue, const NRange& theRange) const
{
	NIndex n, firstBit, lastBit;
	NRange finalRange;



	// Get the state we need
	finalRange = GetNormalized(theRange);
	firstBit   = finalRange.GetFirst();
	lastBit    = finalRange.GetLast();



	// Find the bit
	for (n = firstBit; n <= lastBit; n++)
	{
		if (GetBit(n) == theValue)
		{
			return n;
		}
	}

	return kNIndexNone;
}





//=============================================================================
//		NBitVector::FindLastBit : Find the last bit with a specific value.
//-----------------------------------------------------------------------------
NIndex NBitVector::FindLastBit(bool theValue, const NRange& theRange) const
{
	NIndex n, firstBit, lastBit;
	NRange finalRange;



	// Get the state we need
	finalRange = GetNormalized(theRange);
	firstBit   = finalRange.GetFirst();
	lastBit    = finalRange.GetLast();



	// Find the bit
	for (n = lastBit; n >= firstBit; n--)
	{
		if (n >= 0 && GetBit(n) == theValue)
		{
			return n;
		}
	}

	return kNIndexNone;
}





//=============================================================================
//		NBitVector::= : Assignment operator.
//-----------------------------------------------------------------------------
const NBitVector& NBitVector::operator=(const NBitVector& theValue)
{


	// Assign the object
	if (this != &theValue)
	{
		mData = theValue.mData;
		UpdateSize(theValue.mSize);
	}

	return *this;
}





//=============================================================================
//		NBitVector::NFormatArgument : NFormatArgument operator.
//-----------------------------------------------------------------------------
NBitVector::operator NFormatArgument() const
{
	NString theResult;



	// Get the value
	theResult.Format("{size=%ld}", GetSize());

	return theResult;
}





#pragma mark protected
//=============================================================================
//		NBitVector::EncodeSelf : Encode the object.
//-----------------------------------------------------------------------------
void NBitVector::EncodeSelf(NEncoder& theEncoder) const
{


	// Encode the object
	theEncoder.EncodeData(kNEncoderValueKey, GetBits());
}





//=============================================================================
//		NBitVector::DecodeSelf : Decode the object.
//-----------------------------------------------------------------------------
void NBitVector::DecodeSelf(const NEncoder& theEncoder)
{


	// Decode the object
	SetBits(theEncoder.DecodeData(kNEncoderValueKey));
}





#pragma mark private
//=============================================================================
//		NBitVector::UpdateSize : Update the size.
//-----------------------------------------------------------------------------
void NBitVector::UpdateSize(NIndex numBits)
{


	// Update our state
	mSize  = numBits;
	mBytes = (mSize == 0) ? NULL : mData.GetData();
}





//=============================================================================
//		NBitVector::GetByteSize : Get the byte count for a bit count.
//-----------------------------------------------------------------------------
NIndex NBitVector::GetByteSize(NIndex numBits) const
{
	NIndex numBytes;



	// Count the number of bytes
	numBytes = numBits / 8;

	if ((numBytes * 8) < numBits)
	{
		numBytes++;
	}

	return numBytes;
}
