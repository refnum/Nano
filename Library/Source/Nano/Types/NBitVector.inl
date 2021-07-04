/*	NAME:
		NBitVector.inl

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





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
struct NBitPointer
{
	uint8_t* theByte;
	uint8_t  bitIndex;
};





//=============================================================================
//		NBitVector::NBitVector : Constructor.
//-----------------------------------------------------------------------------
inline NBitVector::NBitVector()
	: mData{}
	, mSize(0)
	, mBytes(nullptr)
{
}





//=============================================================================
//		NBitVector::NBitVector : Constructor.
//-----------------------------------------------------------------------------
inline NBitVector::NBitVector(const NBitVector& otherVector)
	: mData(otherVector.mData)
	, mSize(otherVector.mSize)
	, mBytes(mData.GetMutableData())
{
}





//=============================================================================
//		NBitVector::NBitVector : Constructor.
//-----------------------------------------------------------------------------
inline NBitVector& NBitVector::operator=(const NBitVector& otherVector)
{


	// Assign the vector
	if (this != &otherVector)
	{
		mData  = otherVector.mData;
		mSize  = otherVector.mSize;
		mBytes = mData.GetMutableData();
	}

	return *this;
}





//=============================================================================
//		NBitVector::NBitVector : Constructor.
//-----------------------------------------------------------------------------
inline NBitVector::NBitVector(NBitVector&& otherVector)
	: mData(std::exchange(otherVector.mData, {}))
	, mSize(std::exchange(otherVector.mSize, 0u))
	, mBytes(mData.GetMutableData())
{


	// Reset the other object
	otherVector.mBytes = nullptr;
}





//=============================================================================
//		NBitVector::NBitVector : Constructor.
//-----------------------------------------------------------------------------
inline NBitVector& NBitVector::operator=(NBitVector&& otherVector)
{


	// Move the vector
	if (this != &otherVector)
	{
		mData  = std::exchange(otherVector.mData, {});
		mSize  = std::exchange(otherVector.mSize, 0u);
		mBytes = mData.GetMutableData();

		otherVector.mBytes = nullptr;
	}

	return *this;
}





//=============================================================================
//		NBitVector::Clear : Clear the vector.
//-----------------------------------------------------------------------------
inline void NBitVector::Clear()
{


	// Clear the vector
	mData.Clear();
	mSize  = 0;
	mBytes = nullptr;
}





//=============================================================================
//		NBitVector::GetSize : Get the size.
//-----------------------------------------------------------------------------
inline size_t NBitVector::GetSize() const
{


	// Get the size
	return mSize;
}





//=============================================================================
//		NBitVector::GetCapacity : Get the capacity.
//-----------------------------------------------------------------------------
inline size_t NBitVector::GetCapacity() const
{


	// Get the size
	return mData.GetCapacity() * 8;
}





//=============================================================================
//		NBitVector::GetBit : Get a bit.
//-----------------------------------------------------------------------------
inline bool NBitVector::GetBit(size_t theIndex) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex <= mSize);



	// Get the bit
	NBitPointer bitPointer = GetBitPointer(theIndex);

	return (*bitPointer.theByte >> bitPointer.bitIndex) & 0x01;
}





//=============================================================================
//		NBitVector::SetBit : Set a bit.
//-----------------------------------------------------------------------------
inline void NBitVector::SetBit(size_t theIndex, bool theValue)
{


	// Validate our parameters
	NN_REQUIRE(theIndex <= mSize);



	// Get the bit
	NBitPointer bitPointer = GetBitPointer(theIndex);

	if (theValue)
	{
		*bitPointer.theByte |= uint8_t(1 << bitPointer.bitIndex);
	}
	else
	{
		*bitPointer.theByte &= ~uint8_t(1 << bitPointer.bitIndex);
	}
}





//=============================================================================
//		NBitVector::FlipBit : Flip a bit.
//-----------------------------------------------------------------------------
inline void NBitVector::FlipBit(size_t theIndex)
{


	// Validate our parameters
	NN_REQUIRE(theIndex < GetSize());



	// Flip the bit
	NBitPointer bitPointer = GetBitPointer(theIndex);

	*bitPointer.theByte ^= uint8_t(1 << bitPointer.bitIndex);
}





//=============================================================================
//		NBitVector::operator~ : Bitwise NOT operator.
//-----------------------------------------------------------------------------
inline NBitVector NBitVector::operator~() const
{
	NBitVector theResult(*this);

	theResult.FlipBits();

	return theResult;
}





//=============================================================================
//		NBitVector::operator&= : Bitwise AND operator.
//-----------------------------------------------------------------------------
inline NBitVector& NBitVector::operator&=(const NBitVector& theVector)
{
	*this = *this & theVector;

	return *this;
}





//=============================================================================
//		NBitVector::operator|= : Bitwise OR operator.
//-----------------------------------------------------------------------------
inline NBitVector& NBitVector::operator|=(const NBitVector& theVector)
{
	*this = *this | theVector;

	return *this;
}





//=============================================================================
//		NBitVector::operator^= : Bitwise XOR operator.
//-----------------------------------------------------------------------------
inline NBitVector& NBitVector::operator^=(const NBitVector& theVector)
{
	*this = *this ^ theVector;

	return *this;
}





#pragma mark private
//=============================================================================
//		NBitVector::GetBitPointer : Get a pointer to a bit.
//-----------------------------------------------------------------------------
inline NBitPointer NBitVector::GetBitPointer(size_t theIndex) const
{


	// Validate our parameters
	NN_REQUIRE(theIndex <= mSize);



	// Get the bit
	NBitPointer bitPointer{};

	size_t byteIndex    = theIndex / 8;
	bitPointer.bitIndex = uint8_t(7 - (theIndex - (byteIndex * 8)));
	bitPointer.theByte  = &mBytes[byteIndex];

	return bitPointer;
}
