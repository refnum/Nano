/*	NAME:
		NBitVector.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NBitVector.h"

// Nano
#include "NRange.h"





//=============================================================================
//		NBitVector::SetSize : Set the size.
//-----------------------------------------------------------------------------
void NBitVector::SetSize(size_t numBits)
{


	// Clear unused bits
	//
	// Growing the storage will always append zero-filled bits.
	//
	// To ensure unued bits are always zero then we also need to clear
	// any previously used bits in the byte containing our final bit.
	if (numBits < mSize)
	{
		size_t unusedBits = 8 - (numBits % 8);
		SetBits(false, NRange(numBits, unusedBits));
	}



	// Set the size
	mData.SetSize(GetByteSize(numBits));

	mSize  = numBits;
	mBytes = mData.GetMutableData();
}





//=============================================================================
//		NBitVector::SetCapacity : Set the capacity.
//-----------------------------------------------------------------------------
void NBitVector::SetCapacity(size_t numBits)
{


	// Set the capacity
	size_t numBytes = GetByteSize(mSize + numBits);

	mData.SetCapacity(numBytes);
	mBytes = mData.GetMutableData();
}





//=============================================================================
//		NBitVector::GetData : Get the bits as an NData.
//-----------------------------------------------------------------------------
NData NBitVector::GetData() const
{


	// Get the bits
	return mData;
}





//=============================================================================
//		NBitVector::SetData : Set the bits as an NData.
//-----------------------------------------------------------------------------
void NBitVector::SetData(const NData& theData)
{


	// Set the bits
	mData  = theData;
	mSize  = mData.GetSize() * 8;
	mBytes = mData.GetMutableData();
}





//=============================================================================
//		NBitVector::HasBit : Test the bits in a range.
//-----------------------------------------------------------------------------
bool NBitVector::HasBit(bool theValue, const NRange& theRange) const
{


	// Count the bits
	for (size_t n : theRange.GetNormalized(GetSize()))
	{
		if (GetBit(n) == theValue)
		{
			return true;
		}
	}

	return false;
}





//=============================================================================
//		NBitVector::SetBits : Set the bits in a range.
//-----------------------------------------------------------------------------
void NBitVector::SetBits(bool theValue, const NRange& theRange)
{


	// Set the bits
	for (size_t n : theRange.GetNormalized(GetSize()))
	{
		SetBit(n, theValue);
	}
}





//=============================================================================
//		NBitVector::AppendBit : Append a bit.
//-----------------------------------------------------------------------------
void NBitVector::AppendBit(bool theValue)
{


	// Append the bit
	SetSize(GetSize() + 1);
	SetBit(GetSize() - 1, theValue);
}





//=============================================================================
//		NBitVector::FindFirst : Find the first bit with a value.
//-----------------------------------------------------------------------------
size_t NBitVector::FindFirst(bool theValue, const NRange& theRange) const
{


	// Find the bit
	for (size_t n : theRange.GetNormalized(GetSize()))
	{
		if (GetBit(n) == theValue)
		{
			return n;
		}
	}

	return kNNotFound;
}





//=============================================================================
//		NBitVector::FindLast : Find the last bit with a value.
//-----------------------------------------------------------------------------
size_t NBitVector::FindLast(bool theValue, const NRange& theRange) const
{


	// Find the bit
	NRange finalRange = theRange.GetNormalized(GetSize());

	for (size_t n = finalRange.GetLast(); n >= finalRange.GetFirst(); n--)
	{
		if (GetBit(n) == theValue)
		{
			return n;
		}

		if (n == 0)
		{
			break;
		}
	}

	return kNNotFound;
}





//=============================================================================
//		NBitVector::CountBits : Count the bits with a value.
//-----------------------------------------------------------------------------
size_t NBitVector::CountBits(bool theValue, const NRange& theRange) const
{


	// Count the bits
	size_t numFound = 0;

	for (size_t n : theRange.GetNormalized(GetSize()))
	{
		if (GetBit(n) == theValue)
		{
			numFound++;
		}
	}

	return numFound;
}





//=============================================================================
//		NBitVector::CountLeadingBits : Count the leading bits with a value.
//-----------------------------------------------------------------------------
size_t NBitVector::CountLeadingBits(bool theValue) const
{


	// Count the bits
	size_t numFound = 0;

	for (size_t n = 0; n < mSize; n++)
	{
		if (GetBit(n) != theValue)
		{
			break;
		}

		numFound++;
	}

	return numFound;
}





//=============================================================================
//		NBitVector::CountTrailingBits : Count the trailing bits with a value.
//-----------------------------------------------------------------------------
size_t NBitVector::CountTrailingBits(bool theValue) const
{


	// Count the bits
	size_t numFound = 0;

	for (int64_t n = int64_t(mSize) - 1; n >= 0; n--)
	{
		if (GetBit(size_t(n)) != theValue)
		{
			break;
		}

		numFound++;

		if (n == 0)
		{
			break;
		}
	}

	return numFound;
}





//=============================================================================
//		NBitVector::FlipBits : Flip the bits.
//-----------------------------------------------------------------------------
void NBitVector::FlipBits(const NRange& theRange)
{


	// Flip the bits
	for (size_t n : theRange.GetNormalized(GetSize()))
	{
		FlipBit(n);
	}
}





//=============================================================================
//		NBitVector::operator& : Bitwise AND operator.
//-----------------------------------------------------------------------------
NBitVector NBitVector::operator&(const NBitVector& theVector) const
{


	// Validate our parameters
	NN_REQUIRE(theVector.GetSize() == GetSize());



	// Combine the vectors
	NBitVector theResult(*this);

	for (size_t n = 0; n < mData.GetSize(); n++)
	{
		theResult.mBytes[n] &= theVector.mBytes[n];
	}

	return theResult;
}





//=============================================================================
//		NBitVector::operator| : Bitwise OR operator.
//-----------------------------------------------------------------------------
NBitVector NBitVector::operator|(const NBitVector& theVector) const
{


	// Validate our parameters
	NN_REQUIRE(theVector.GetSize() == GetSize());



	// Combine the vectors
	NBitVector theResult(*this);

	for (size_t n = 0; n < mData.GetSize(); n++)
	{
		theResult.mBytes[n] |= theVector.mBytes[n];
	}

	return theResult;
}





//=============================================================================
//		NBitVector::operator^ : Bitwise XOR operator.
//-----------------------------------------------------------------------------
NBitVector NBitVector::operator^(const NBitVector& theVector) const
{


	// Validate our parameters
	NN_REQUIRE(theVector.GetSize() == GetSize());



	// Combine the vectors
	NBitVector theResult(*this);

	for (size_t n = 0; n < mData.GetSize(); n++)
	{
		theResult.mBytes[n] ^= theVector.mBytes[n];
	}

	return theResult;
}





#pragma mark NMixinAppendable
//=============================================================================
//		NBitVector::Append : Append a value.
//-----------------------------------------------------------------------------
void NBitVector::Append(const NBitVector& theVector)
{


	// Append as bytes
	if ((mSize % 8) == 0)
	{
		mData += theVector.mData;
		mSize += theVector.mSize;
		mBytes = mData.GetMutableData();
	}


	// Append as bits
	else
	{
		SetCapacity(GetSize() + theVector.GetSize());

		for (size_t n = 0; n < theVector.GetSize(); n++)
		{
			AppendBit(theVector.GetBit(n));
		}
	}
}





#pragma mark NMixinComparable
//=============================================================================
//		NBitVector::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NBitVector::CompareEqual(const NBitVector& theVector) const
{


	// Compare the size
	//
	// A different size means no equality.
	if (GetSize() != theVector.GetSize())
	{
		return false;
	}



	// Compare the bits
	return mData.CompareEqual(theVector.mData);
}





//=============================================================================
//		NBitVector::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NBitVector::CompareOrder(const NBitVector& theVector) const
{


	// Order by bits
	return mData.CompareOrder(theVector.mData);
}





#pragma mark private
//=============================================================================
//		NBitVector::GetByteSize : Get the byte count for a bit count.
//-----------------------------------------------------------------------------
size_t NBitVector::GetByteSize(size_t numBits) const
{


	// Get the byte count
	size_t numBytes = numBits / 8;

	if ((numBytes * 8) != numBits)
	{
		numBytes++;
	}

	return numBytes;
}
