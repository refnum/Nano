/*	NAME:
		NData.cpp

	DESCRIPTION:
		Data object.

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
#include "NData.h"

// Nano
#include "NAssert.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kSmallSizeBit                       = 0b00000001;
static constexpr size_t kSmallSizeMask                      = 0b11111000;
static constexpr size_t kSmallSizeShift                     = 3;

static constexpr size_t kSmallSizeMax                       = 31;
static constexpr size_t kSmallSize0                         = kSmallSizeBit;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
// Data block
//
// The data block holds the state for shared data.
//
// As the block is allocated dynamically, the least significant bit of the
// block address is always zero.
//
// By placing a pointer to the block at the start of our storage union we
// can use that bit as a flag to indicate which representation is active.
struct NDataBlock
{
	std::atomic_size_t numOwners;
	bool               ownedData;
	size_t             theSize;
	const uint8_t*     theData;
};





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(size_t theSize, const void* theData, NDataSource theSource)
	: mSmall{kSmallSize0, {}}
{
	// Initialise ourselves
	SetData(theSize, theData, theSource);
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData()
	: mSmall{kSmallSize0, {}}
{
	// Validate our state
	static_assert(sizeof(NDataStorageSmall) == 32);
	static_assert(sizeof(NDataStorageShared) == 32);

	static_assert(offsetof(NDataStorageSmall, sizeFlags) == 0);
	static_assert(offsetof(NDataStorageShared, theBlock) == 0);

	static_assert(NN_ENDIAN_LITTLE, "NDataStorageSmall/Shared no longer overlap!");
	static_assert(alignof(std::max_align_t) > 1, "NDataBlock requires LSB be free!");
}





//=============================================================================
//		NData::~NData : Destructor.
//-----------------------------------------------------------------------------
NData::~NData()
{


	// Clean up
	Clear();
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(const NData& otherData)
	: mSmall{kSmallSize0, {}}
{
	// Initialise ourselves
	AdoptData(otherData);
}





//=============================================================================
//		NData::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NData& NData::operator=(const NData& otherData)
{


	// Assign the value
	if (this != &otherData)
	{
		AdoptData(otherData);
	}

	return *this;
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(NData&& otherData)
	: mSmall{kSmallSize0, {}}
{
	// Initialise ourselves
	AdoptData(otherData);
	otherData.Clear();
}





//=============================================================================
//		NData::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NData& NData::operator=(NData&& otherData)
{


	// Move the value
	if (this != &otherData)
	{
		Clear();
		AdoptData(otherData);
		otherData.Clear();
	}

	return *this;
}





//=============================================================================
//		NData::Clear : Clear the data.
//-----------------------------------------------------------------------------
void NData::Clear()
{


	// Release shared data
	if (IsShared())
	{
		ReleaseShared();
	}



	// Reset our state
#if NN_DEBUG
	memset(&mSmall, 0xAA, sizeof(mSmall));
#endif

	mSmall.sizeFlags = kSmallSize0;

	ClearHash();
}





//=============================================================================
//		NData::GetSize : Get the size.
//-----------------------------------------------------------------------------
size_t NData::GetSize() const
{


	// Get the size
	size_t theSize = 0;

	if (IsSmall())
	{
		theSize = GetSizeSmall();
	}
	else
	{
		theSize = GetSizeShared();
	}

	return theSize;
}





//=============================================================================
//		NData::SetSize : Set the size.
//-----------------------------------------------------------------------------
void NData::SetSize(size_t theSize)
{


	// Validate our parameters
	NN_EXPECT(theSize != GetSize());



	// Set the size
	size_t oldSize = GetSize();

	if (theSize != oldSize)
	{
		// Adjust the size
		if (IsSmall())
		{
			SetSizeSmall(theSize);
		}
		else
		{
			SetSizeShared(theSize);
		}



		// Zero-fill any new space
		if (theSize > oldSize)
		{
			uint8_t* thePtr = GetMutableData(oldSize);
			memset(thePtr, 0x00, theSize - oldSize);
		}



		// Update our state
		ClearHash();
	}
}





//=============================================================================
//		NData::GetCapacity : Get the capacity.
//-----------------------------------------------------------------------------
size_t NData::GetCapacity() const
{


	// Get the capacity
	size_t theCapacity = 0;

	if (IsSmall())
	{
		theCapacity = GetCapacitySmall();
	}
	else
	{
		theCapacity = GetCapacityShared();
	}

	return theCapacity;
}





//=============================================================================
//		NData::SetCapacity : Set the capacity.
//-----------------------------------------------------------------------------
void NData::SetCapacity(size_t theCapacity)
{


	// Validate our parameters
	NN_EXPECT(theCapacity != GetCapacity());



	// Set the capacity
	if (theCapacity != GetCapacity())
	{
		if (IsSmall())
		{
			SetCapacitySmall(theCapacity);
		}
		else
		{
			SetCapacityShared(theCapacity);
		}
	}
}





//=============================================================================
//		NData::GetData : Get the data.
//-----------------------------------------------------------------------------
NData NData::GetData(const NRange& theRange) const
{


	// Validate our parameters
	NN_REQUIRE(IsValidRange(theRange));

	NN_EXPECT(!theRange.IsEmpty());



	// Get the data
	NData theData;

	if (!theRange.IsEmpty())
	{
		if (IsSmall())
		{
			theData.SetData(theRange.GetSize(), GetData(theRange.GetLocation()));
		}
		else
		{
			theData.AdoptData(*this);
			NN_REQUIRE(theData.IsShared());

			theData.mShared.theSlice = theRange;
		}
	}

	return theData;
}





//=============================================================================
//		NData::GetData : Get the data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetData(size_t theOffset) const
{


	// Validate our parameters
	NN_EXPECT(IsValidOffset(theOffset));



	// Get the data
	const uint8_t* theData = nullptr;

	if (!IsEmpty() && theOffset < GetSize())
	{
		if (IsSmall())
		{
			theData = GetDataSmall(theOffset);
		}
		else
		{
			theData = GetDataShared(theOffset);
		}
	}

	return theData;
}





//=============================================================================
//		NData::GetMutableData : Get mutable data.
//-----------------------------------------------------------------------------
uint8_t* NData::GetMutableData(size_t theOffset)
{


	// Validate our parameters
	NN_EXPECT(IsValidOffset(theOffset));



	// Get the data
	//
	// Once mutable we can safely cast away the const.
	uint8_t* thePtr = nullptr;

	if (!IsEmpty() && theOffset < GetSize())
	{
		MakeMutable();
		thePtr = const_cast<uint8_t*>(GetData(theOffset));
	}

	return thePtr;
}





//=============================================================================
//		NData::SetData : Set the data.
//-----------------------------------------------------------------------------
void NData::SetData(size_t theSize, const void* theData, NDataSource theSource)
{


	// Validate our parameters
	NN_REQUIRE(IsValidSource(theSize, theData, theSource));



	// Set the data
	//
	// Views are implicitly shared.
	if (theSize <= kSmallSizeMax && theSource != NDataSource::View)
	{
		SetDataSmall(theSize, theData, theSource);
	}
	else
	{
		SetDataShared(theSize, theData, theSource);
	}

	ClearHash();
}





//=============================================================================
//		NData::InsertData : Insert data.
//-----------------------------------------------------------------------------
uint8_t* NData::InsertData(size_t beforeIndex, const NData& theData)
{


	// Insert the data
	return InsertData(beforeIndex, theData.GetSize(), theData.GetData(), NDataSource::Copy);
}





//=============================================================================
//		NData::InsertData : Insert data.
//-----------------------------------------------------------------------------
uint8_t* NData::InsertData(size_t      beforeIndex,
						   size_t      theSize,
						   const void* theData,
						   NDataSource theSource)
{


	// Validate our parameters
	NN_REQUIRE(IsValidOffset(beforeIndex) || beforeIndex == GetSize());
	NN_REQUIRE(IsValidSource(theSize, theData, theSource));

	NN_EXPECT(theSize != 0);


	// Insert the data
	uint8_t* newData = nullptr;

	if (theSize != 0)
	{
		// Grow the buffer
		size_t oldSize = GetSize();
		SetSize(oldSize + theSize);



		// Prepare to insert
		//
		// Data after the inserted area must be moved up towards the end.
		newData = GetMutableData(beforeIndex);

		uint8_t* movedPtr  = newData + theSize;
		size_t   movedSize = oldSize - beforeIndex;

		if (movedSize != 0)
		{
			memmove(movedPtr, newData, movedSize);
		}



		// Insert the data
		MemCopy(newData, theData, theSize, theSource);
	}

	return newData;
}





//=============================================================================
//		NData::AppendData : Append data.
//-----------------------------------------------------------------------------
uint8_t* NData::AppendData(const NData& theData)
{


	// Append the data
	return InsertData(GetSize(), theData);
}





//=============================================================================
//		NData::AppendData : Append data.
//-----------------------------------------------------------------------------
uint8_t* NData::AppendData(size_t theSize, const void* theData, NDataSource theSource)
{


	// Appemd the data
	return InsertData(GetSize(), theSize, theData, theSource);
}





//=============================================================================
//		NData::RemoveData : Remove data.
//-----------------------------------------------------------------------------
void NData::RemoveData(const NRange& theRange)
{


	// Validate our parameters
	NN_REQUIRE(IsValidRange(theRange));

	NN_EXPECT(!theRange.IsEmpty());



	// Remove the data
	if (IsSmall())
	{
		RemoveDataSmall(theRange);
	}
	else
	{
		RemoveDataShared(theRange);
	}
}





//=============================================================================
//		NData::ReplaceData : Replace data.
//-----------------------------------------------------------------------------
uint8_t* NData::ReplaceData(const NRange& theRange, const NData& theData)
{


	// Validate our parameters
	NN_REQUIRE(IsValidRange(theRange));

	NN_EXPECT(!theRange.IsEmpty());



	// Replace the data
	return ReplaceData(theRange, theData.GetSize(), theData.GetData(), NDataSource::Copy);
}





//=============================================================================
//		NData::ReplaceData : Replace data.
//-----------------------------------------------------------------------------
uint8_t* NData::ReplaceData(const NRange& theRange,
							size_t        theSize,
							const void*   theData,
							NDataSource   theSource)
{


	// Validate our parameters
	NN_REQUIRE(IsValidRange(theRange));
	NN_REQUIRE(IsValidSource(theSize, theData, theSource));

	NN_EXPECT(!theRange.IsEmpty());
	NN_EXPECT(theSize != 0);



	// Replace everything
	if (theRange == NRange(0, GetSize()))
	{
		SetData(theSize, theData, theSource);
	}


	// Replace a subrange
	//
	// If the range we're replacing is a different size than the data we are
	// replacing it with then we need to insert filler / remove data prior
	// to the replacement.
	//
	// When inserting filler we do so at the end of the area we're going to
	// overwrite, to minimise the amount of data that needs to be moved up.
	else
	{
		// Adjust the data
		size_t replacedSize = theRange.GetSize();

		if (replacedSize > theSize)
		{
			RemoveData(NRange(theRange.GetLocation(), replacedSize - theSize));
		}
		else
		{
			InsertData(theRange.GetNext(), theSize - replacedSize, nullptr, NDataSource::None);
		}



		// Replace the range
		if (theSize != 0)
		{
			uint8_t* dstPtr = GetMutableData(theRange.GetLocation());
			MemCopy(dstPtr, theData, theSize, theSource);
		}
	}



	// Get the replaced data
	//
	// If the replacement was a removal from the end then the range location
	// will now be after our last byte, so we'll return nullptr as required.
	return GetMutableData(theRange.GetLocation());
}





//=============================================================================
//		NData::Compare : Compare the object.
//-----------------------------------------------------------------------------
NComparison NData::Compare(const NData& theData) const
{


	// Compare the data
	//
	// Data can only be compared for equality / inequality, so we
	// can do a quick compare of our hash before checking the data.
	NComparison theResult = NCompare(GetHash(), theData.GetHash());

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(GetSize(), GetData(), theData.GetSize(), theData.GetData());
	}

	return theResult;
}





//=============================================================================
//		NData::+= : Concatenate operator.
//-----------------------------------------------------------------------------
const NData& NData::operator+=(const NData& theValue)
{


	// Append the data
	AppendData(theValue);

	return *this;
}





//=============================================================================
//		NData::+ : Concatenate operator.
//-----------------------------------------------------------------------------
const NData NData::operator+(const NData& theValue) const
{


	// Append the data
	NData theResult(*this);

	theResult += theValue;

	return theResult;
}





//=============================================================================
//		NData::CalculateHash : Calculate the hash.
//-----------------------------------------------------------------------------
size_t NData::CalculateHash() const
{


	// TODO
	return 0;
}





#pragma mark private
//=============================================================================
//		NData::IsSmall : Are we using small storage?
//-----------------------------------------------------------------------------
bool NData::IsSmall() const
{


	// Check the flag
	return (mSmall.sizeFlags & kSmallSizeBit) != 0;
}





//=============================================================================
//		NData::IsShared : Are we using shared storage?
//-----------------------------------------------------------------------------
bool NData::IsShared() const
{


	// Check the flag
	return (mSmall.sizeFlags & kSmallSizeBit) == 0;
}





//=============================================================================
//		NData::IsValidRange : Is a range valid?
//-----------------------------------------------------------------------------
bool NData::IsValidRange(const NRange& theRange) const
{


	// Check the range
	return IsValidOffset(theRange.GetFirst()) && IsValidOffset(theRange.GetLast());
}





//=============================================================================
//		NData::IsValidOffset : Is an offset valid?
//-----------------------------------------------------------------------------
bool NData::IsValidOffset(size_t theOffset) const
{


	// Check the offset
	//
	// We allow a zero offset when empty.
	return (theOffset == 0 && IsEmpty()) || (theOffset < GetSize());
}





//=============================================================================
//		NData::IsValidSource : Is a source combination valid?
//-----------------------------------------------------------------------------
bool NData::IsValidSource(size_t theSize, const void* theData, NDataSource theSource) const
{


	// Check the source
	switch (theSource)
	{
		case NDataSource::Copy:
		case NDataSource::View:
			return (theSize == 0) || (theSize != 0 && theData != nullptr);
			break;

		case NDataSource::Zero:
		case NDataSource::None:
			return theData == nullptr;
			break;
	}

	return false;
}





//=============================================================================
//		NData::MakeMutable : Make the data mutable.
//-----------------------------------------------------------------------------
void NData::MakeMutable()
{


	// Get the state we need
	NRange         srcSlice;
	const uint8_t* srcData = nullptr;

	if (IsShared())
	{
		// If we're a view, or we're shared, we need to copy the data
		//
		// Otherwise we are the only owner so can safely mutate in place.
		if (mShared.theBlock->numOwners != 1)
		{
			srcSlice = mShared.theSlice;
			srcData  = mShared.theData;
		}
	}



	// Copy the data
	//
	// Our data is mutable once we own the only copy.
	if (!srcSlice.IsEmpty())
	{
		size_t srcSize = srcSlice.GetSize();
		srcData        = srcData + srcSlice.GetLocation();

		MakeShared(srcSize, srcSize, srcData, NDataSource::Copy);
	}



	// Reset our hash
	//
	// We assume that a request for mutable access implies mutation.
	ClearHash();
}





//=============================================================================
//		NData::MakeShared : Make shared data.
//-----------------------------------------------------------------------------
void NData::MakeShared(size_t      theCapacity,
					   size_t      theSize,
					   const void* theData,
					   NDataSource theSource)
{


	// Validate our parameters and state
	NN_REQUIRE(theCapacity != 0 && theSize != 0);
	NN_REQUIRE(theCapacity >= theSize);
	NN_REQUIRE(IsValidSource(theSize, theData, theSource));



	// Create the data
	//
	// We use malloc to avoid unnecessary fill overhead when the data is
	// about to be overwritten / does not need initialisation.
	void* newData = nullptr;

	switch (theSource)
	{
		case NDataSource::Copy:
			newData = malloc(theCapacity);
			memcpy(newData, theData, theSize);
			break;

		case NDataSource::Zero:
			newData = calloc(1, theCapacity);
			break;

		case NDataSource::None:
			newData = malloc(theCapacity);
			break;

		case NDataSource::View:
			newData = const_cast<void*>(theData);
			break;
	}



	// Create the block
	NDataBlock* theBlock = new NDataBlock{};

	theBlock->numOwners = 1;
	theBlock->ownedData = (theSource != NDataSource::View);
	theBlock->theSize   = theCapacity;
	theBlock->theData   = static_cast<const uint8_t*>(newData);



	// Switch to shared data
	Clear();

	mShared.theBlock = theBlock;
	mShared.theData  = theBlock->theData;
	mShared.theSlice = NRange(0, theSize);

	NN_REQUIRE(IsShared());
}





//=============================================================================
//		NData::RetainShared : Retain the shared data.
//-----------------------------------------------------------------------------
void NData::RetainShared()
{


	// Validate our state
	NN_REQUIRE(IsShared());
	NN_REQUIRE(mShared.theBlock->numOwners != 0);



	// Retain the block
	mShared.theBlock->numOwners += 1;
}





//=============================================================================
//		NData::ReleaseShared : Release the shared data.
//-----------------------------------------------------------------------------
void NData::ReleaseShared()
{


	// Validate our state
	NN_REQUIRE(IsShared());
	NN_REQUIRE(mShared.theBlock->numOwners != 0);



	// Release the block
	//
	// The last owner releases the block and the data, if we own it.
	if (mShared.theBlock->numOwners.fetch_sub(1) == 1)
	{
		if (mShared.theBlock->ownedData)
		{
			free(const_cast<uint8_t*>(mShared.theBlock->theData));
		}

		delete mShared.theBlock;
	}
}





//=============================================================================
//		NData::AdoptData : Adopt the data from another object.
//-----------------------------------------------------------------------------
void NData::AdoptData(const NData& otherData)
{


	// Validate our parameters and state
	NN_REQUIRE(this != &otherData);
	NN_REQUIRE(mSmall.sizeFlags == kSmallSize0);



	// Adopt the data
	mSmall = otherData.mSmall;

	if (IsShared())
	{
		RetainShared();
	}


	// Update our state
	ClearHash();
}





//=============================================================================
//		NData::MemCopy : Copy bytes.
//-----------------------------------------------------------------------------
void NData::MemCopy(void* dstPtr, const void* srcPtr, size_t theSize, NDataSource theSource)
{


	// Copy the data
	switch (theSource)
	{
		case NDataSource::Copy:
		case NDataSource::View:
			memmove(dstPtr, srcPtr, theSize);
			break;

		case NDataSource::Zero:
			memset(dstPtr, 0x00, theSize);
			break;

		case NDataSource::None:
			// Do nothing
			break;
	}
}





//=============================================================================
//		NData::GetSizeSmall : Get the small size.
//-----------------------------------------------------------------------------
size_t NData::GetSizeSmall() const
{


	// Validate our state
	NN_REQUIRE(IsSmall());



	// Get the size
	//
	// The size is stored in the top bits of the storage flag byte.
	size_t theSize = ((mSmall.sizeFlags & kSmallSizeMask) >> kSmallSizeShift);
	return theSize;
}





//=============================================================================
//		NData::GetSizeShared : Get the shared size.
//-----------------------------------------------------------------------------
size_t NData::GetSizeShared() const
{


	// Validate our state
	NN_REQUIRE(IsShared());



	// Get the size
	return mShared.theSlice.GetSize();
}





//=============================================================================
//		NData::SetSizeSmall : Set the small size.
//-----------------------------------------------------------------------------
void NData::SetSizeSmall(size_t theSize)
{


	// Validate our state
	NN_REQUIRE(IsSmall());


	// Set small size
	//
	// The size is stored in the top bits of the storage flag byte.
	if (theSize <= kSmallSizeMax)
	{
		mSmall.sizeFlags = uint8_t((theSize << kSmallSizeShift) | kSmallSizeBit);
	}


	// Switch to shared
	//
	// Copy the small data into a shared block, then resize as required.
	else
	{
		MakeShared(theSize, GetSizeSmall(), mSmall.theData, NDataSource::Copy);
		SetSizeShared(theSize);
	}
}





//=============================================================================
//		NData::SetSizeShared : Set the shared size.
//-----------------------------------------------------------------------------
void NData::SetSizeShared(size_t theSize)
{


	// Validate our state
	NN_REQUIRE(IsShared());



	// Reduce the size
	if (theSize <= mShared.theSlice.GetSize())
	{
		mShared.theSlice.SetSize(theSize);
	}


	// Increase the size
	else
	{
		// We can use our storage
		//
		// If the requested size is within our block, and nobody else is
		// using it, we can just expand our slice.
		if (theSize <= mShared.theBlock->theSize && mShared.theBlock->numOwners == 1)
		{
			mShared.theSlice.SetSize(theSize);
		}


		// We require new storage
		//
		// Copy the shared data into a larger block, then adjust our slice.
		else
		{
			MakeShared(theSize, mShared.theSlice.GetSize(), mShared.theData, NDataSource::Copy);
			mShared.theSlice.SetSize(theSize);
		}
	}
}





//=============================================================================
//		NData::GetCapacitySmall : Get the small capacity.
//-----------------------------------------------------------------------------
size_t NData::GetCapacitySmall() const
{


	// Validate our state
	NN_REQUIRE(IsSmall());



	// Get the capacity
	return kSmallSizeMax;
}





//=============================================================================
//		NData::GetCapacityShared : Get the shared capacity.
//-----------------------------------------------------------------------------
size_t NData::GetCapacityShared() const
{


	// Validate our state
	NN_REQUIRE(IsShared());



	// Get the capacity
	return mShared.theBlock->theSize;
}





//=============================================================================
//		NData::SetCapacitySmall : Set the small capacity.
//-----------------------------------------------------------------------------
void NData::SetCapacitySmall(size_t theCapacity)
{


	// Validate our state
	NN_REQUIRE(IsSmall());



	// Set small capacity
	//
	// Small storage has a fixed capacity, but if the new capacity is
	// under our size we need to reduce our size to match the behaviour
	// of shared storage.
	size_t theSize = GetSizeSmall();

	if (theCapacity < theSize)
	{
		SetSizeSmall(theCapacity);
	}



	// Switch to shared
	//
	// Larger data must be stored in a shared block.
	else if (theCapacity > kSmallSizeMax)
	{
		MakeShared(theCapacity, theSize, mSmall.theData, NDataSource::Copy);
	}
}





//=============================================================================
//		NData::SetCapacityShared : Set the shared capacity.
//-----------------------------------------------------------------------------
void NData::SetCapacityShared(size_t theCapacity)
{


	// Validate our state
	NN_REQUIRE(IsShared());



	// Set the capacity
	size_t theSize = std::min(mShared.theSlice.GetSize(), theCapacity);

	MakeShared(theCapacity, theSize, mShared.theData, NDataSource::Copy);
}





//=============================================================================
//		NData::GetDataSmall : Get small data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataSmall(size_t theOffset) const
{


	// Validate our state
	NN_REQUIRE(IsSmall());
	NN_REQUIRE(theOffset <= kSmallSizeMax);



	// Get the data
	return &mSmall.theData[theOffset];
}





//=============================================================================
//		NData::GetDataShared : Get shared data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataShared(size_t theOffset) const
{


	// Validate our state
	NN_REQUIRE(IsShared());
	NN_REQUIRE(theOffset < mShared.theSlice.GetSize());



	// Get the data
	return &mShared.theData[mShared.theSlice.GetOffset(theOffset)];
}





//=============================================================================
//		NData::SetDataSmall : Set small data.
//-----------------------------------------------------------------------------
void NData::SetDataSmall(size_t theSize, const void* theData, NDataSource theSource)
{


	// Validate our parameters
	NN_REQUIRE(theSize <= kSmallSizeMax);
	NN_REQUIRE(theSource != NDataSource::View);



	// Set when small
	//
	// Even if theData is within our block we can just copy it to the start.
	if (IsSmall())
	{
		MemCopy(mSmall.theData, theData, theSize, theSource);
	}


	// Set when shared
	//
	// The data might be within our block and so would be lost when we
	// release the block. As it's small we just make a local copy then
	// copy it to small storage once we release the block.
	else
	{
		NDataStorageSmall tmpData;

		MemCopy(tmpData.theData, theData, theSize, theSource);
		ReleaseShared();
		MemCopy(mSmall.theData, tmpData.theData, theSize, theSource);
	}



	// Update our state
	SetSizeSmall(theSize);
}





//=============================================================================
//		NData::SetDataShared : Set shared data.
//-----------------------------------------------------------------------------
void NData::SetDataShared(size_t theSize, const void* theData, NDataSource theSource)
{


	// Set when small
	if (IsSmall())
	{
		MakeShared(theSize, theSize, theData, theSource);
	}



	// Set when shared
	else
	{
		// Get the state we need
		uintptr_t srcFirst = reinterpret_cast<uintptr_t>(theData);
		uintptr_t srcLast  = srcFirst + theSize - 1;

		uintptr_t blockFirst = reinterpret_cast<uintptr_t>(mShared.theData);
		uintptr_t blockLast  = blockFirst + mShared.theBlock->theSize - 1;

		bool withinBlock = (srcFirst >= blockFirst && srcFirst <= blockLast) &&
						   (srcLast >= blockFirst && srcLast <= blockLast);


		// Adjust our slice
		//
		// If the source is within our block, and we're not clearing the
		// data, we can simply adjust our slice.
		if (withinBlock && theSource != NDataSource::Zero)
		{
			mShared.theSlice.SetRange(srcFirst - blockFirst, theSize);
		}



		// Reuse our block
		//
		// If we're the only owner of our block, and the new data would
		// fit, copy into into our existing block.
		else if (mShared.theBlock->theSize >= theSize && mShared.theBlock->numOwners == 1)
		{
			MemCopy(const_cast<uint8_t*>(mShared.theData), theData, theSize, theSource);
			mShared.theSlice.SetRange(0, theSize);
		}



		// Create a block
		//
		// Otherwise we need to move the data into a new block.
		else
		{
			MakeShared(theSize, theSize, theData, theSource);
		}
	}
}





//=============================================================================
//		NData::RemoveDataSmall : Remove small data.
//-----------------------------------------------------------------------------
void NData::RemoveDataSmall(const NRange& theRange)
{


	// Validate our state
	NN_REQUIRE(IsSmall());



	// Remove from the front/middle
	//
	// Removal from the end is simply our final size adjustment.
	size_t oldSize = GetSizeSmall();
	size_t newSize = oldSize - theRange.GetSize();

	if (theRange.GetNext() != oldSize)
	{
		size_t dstoffset = theRange.GetFirst();
		size_t srcOffset = theRange.GetNext();
		size_t srcSize   = oldSize - srcOffset;

		memmove(&mSmall.theData[dstoffset], &mSmall.theData[srcOffset], srcSize);
	}

	SetSizeSmall(newSize);
}





//=============================================================================
//		NData::RemoveDataShared : Remove shared data.
//-----------------------------------------------------------------------------
void NData::RemoveDataShared(const NRange& theRange)
{


	// Validate our state
	NN_REQUIRE(IsShared());



	// Remove from the front
	//
	// Remove by adjusting our slice.
	if (theRange.GetLocation() == 0)
	{
		mShared.theSlice.SetLocation(theRange.GetNext());
		mShared.theSlice.SetSize(mShared.theSlice.GetSize() - theRange.GetSize());
	}


	// Remove from the end
	//
	// Remove by adjusting our slice
	else if (theRange.GetLast() == mShared.theSlice.GetLast())
	{
		mShared.theSlice.SetSize(mShared.theSlice.GetSize() - theRange.GetSize());
	}



	// Remove from the middle
	else
	{
		uint8_t* theData = GetMutableData();

		size_t dstoffset = theRange.GetFirst();
		size_t srcOffset = theRange.GetNext();
		size_t srcSize   = mShared.theSlice.GetSize() - srcOffset;

		memmove(&theData[dstoffset], &theData[srcOffset], srcSize);
		mShared.theSlice.SetSize(mShared.theSlice.GetSize() - theRange.GetSize());
	}
}
