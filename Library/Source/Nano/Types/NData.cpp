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
#include "NDataDigest.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr size_t kSmallFlagIsSmall                   = 0b00000001;
static constexpr size_t kSmallSizeMask                      = 0b11111000;
static constexpr size_t kSmallSizeShift                     = 3;
static constexpr size_t kSmallSizeMax                       = 31;

static constexpr size_t kSmallEmpty                         = kSmallFlagIsSmall;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
// Data state
//
// Holds the state for large data.
//
// As the state is allocated dynamically with natural alignment we know that
// the least significant bit of its address will always be zero.
//
// By placing a pointer to the state at the start of our storage union we
// can use that bit as a flag to indicate which representation is active.
struct NDataState
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
	: mSmall{kSmallEmpty, {}}
	, mHash(kNHashNone)
{
	// Initialise ourselves
	SetData(theSize, theData, theSource);
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData()
	: mSmall{kSmallEmpty, {}}
	, mHash(kNHashNone)
{
	// Validate our state
	static_assert(sizeof(NDataStorageSmall) == 32);
	static_assert(sizeof(NDataStorageLarge) == 32);

	static_assert(offsetof(NDataStorageSmall, sizeFlags) == 0);
	static_assert(offsetof(NDataStorageLarge, theState) == 0);

	static_assert(NN_ENDIAN_LITTLE, "NDataStorageSmall/Large no longer overlap!");
	static_assert(alignof(std::max_align_t) > 1, "NDataState requires LSB be free!");
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
	: mSmall{kSmallEmpty, {}}
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
	: mSmall{kSmallEmpty, {}}
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


	// Release large data
	if (IsLarge())
	{
		ReleaseLarge();
	}



	// Reset our state
#if NN_DEBUG
	memset(&mSmall, 0xAA, sizeof(mSmall));
#endif

	mSmall.sizeFlags = kSmallEmpty;

	ClearHash();
}





//=============================================================================
//		NData::GetSize : Get the size.
//-----------------------------------------------------------------------------
size_t NData::GetSize() const
{


	// Get the size
	if (IsSmall())
	{
		return GetSizeSmall();
	}
	else
	{
		return GetSizeLarge();
	}
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
			SetSizeLarge(theSize);
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
	if (IsSmall())
	{
		return GetCapacitySmall();
	}
	else
	{
		return GetCapacityLarge();
	}
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
			SetCapacityLarge(theCapacity);
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
			NN_REQUIRE(theData.IsLarge());

			theData.mLarge.theSlice = theRange;
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
			theData = GetDataLarge(theOffset);
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
	// Views are implicitly shared, so use large data.
	if (theSize <= kSmallSizeMax && theSource != NDataSource::View)
	{
		SetDataSmall(theSize, theData, theSource);
	}
	else
	{
		SetDataLarge(theSize, theData, theSource);
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
		RemoveDataLarge(theRange);
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
//		NData::FetchHash : Fetch the hash.
//-----------------------------------------------------------------------------
size_t& NData::FetchHash(bool updateHash) const
{


	// Fetch the hash
	if (updateHash)
	{
		mHash = NDataDigest::GetRuntime(GetSize(), GetData());
	}

	return mHash;
}





#pragma mark private
//=============================================================================
//		NData::IsSmall : Are we using small storage?
//-----------------------------------------------------------------------------
bool NData::IsSmall() const
{


	// Check the flag
	return (mSmall.sizeFlags & kSmallFlagIsSmall) != 0;
}





//=============================================================================
//		NData::IsLarge : Are we using large storage?
//-----------------------------------------------------------------------------
bool NData::IsLarge() const
{


	// Check the flag
	return (mSmall.sizeFlags & kSmallFlagIsSmall) == 0;
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

	if (IsLarge())
	{
		// If we're a view, or we're large, we need to copy the data
		//
		// Otherwise we are the only owner so can safely mutate in place.
		if (mLarge.theState->numOwners != 1)
		{
			srcSlice = mLarge.theSlice;
			srcData  = mLarge.theData;
		}
	}



	// Copy the data
	//
	// Our data is mutable once we own the only copy.
	if (!srcSlice.IsEmpty())
	{
		size_t srcSize = srcSlice.GetSize();
		srcData        = srcData + srcSlice.GetLocation();

		MakeLarge(srcSize, srcSize, srcData, NDataSource::Copy);
	}



	// Reset our hash
	//
	// We assume that a request for mutable access implies mutation.
	ClearHash();
}





//=============================================================================
//		NData::MakeLarge : Make large data.
//-----------------------------------------------------------------------------
void NData::MakeLarge(size_t      theCapacity,
					  size_t      theSize,
					  const void* theData,
					  NDataSource theSource)
{


	// Validate our parameters and state
	NN_REQUIRE(theCapacity != 0 && theSize != 0);
	NN_REQUIRE(theCapacity >= theSize);
	NN_REQUIRE(IsValidSource(theSize, theData, theSource));



	// Check for existing state
	//
	// If we are the sole owner of some existing state then we can reuse
	// that state (and potentially its data) when adjusting the capacity.
	NDataState* existingState = nullptr;
	const void* existingData  = nullptr;

	if (IsLarge() && mLarge.theState->numOwners == 1)
	{
		// Switching to a view
		//
		// If we are switching to a view onto external data then we must
		// release any existing data owned by the state.
		if (theSource == NDataSource::View && mLarge.theState->ownedData)
		{
			free(const_cast<uint8_t*>(mLarge.theState->theData));
			mLarge.theState->theData = nullptr;
		}


		// Get the state we need
		existingState = mLarge.theState;
		existingData  = mLarge.theState->theData;
	}



	// Create the data
	void* newData = nullptr;

	switch (theSource)
	{
		case NDataSource::Copy:
			newData = MemAllocate(theCapacity, existingData, false);
			memcpy(newData, theData, theSize);
			break;

		case NDataSource::Zero:
			newData = MemAllocate(theCapacity, existingData, true);
			break;

		case NDataSource::None:
			newData = MemAllocate(theCapacity, existingData, false);
			break;

		case NDataSource::View:
			newData = const_cast<void*>(theData);
			break;
	}



	// Create the state
	NDataState* theState = existingState;

	if (theState == nullptr)
	{
		theState = new NDataState{};
	}

	theState->numOwners = 1;
	theState->ownedData = (theSource != NDataSource::View);
	theState->theSize   = theCapacity;
	theState->theData   = static_cast<uint8_t*>(newData);



	// Switch to large data
	if (existingState == nullptr)
	{
		Clear();
	}

	mLarge.theState = theState;
	mLarge.theData  = theState->theData;
	mLarge.theSlice = NRange(0, theSize);

	NN_REQUIRE(IsLarge());
}





//=============================================================================
//		NData::RetainLarge : Retain the large state.
//-----------------------------------------------------------------------------
void NData::RetainLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());
	NN_REQUIRE(mLarge.theState->numOwners != 0);



	// Retain the state
	mLarge.theState->numOwners += 1;
}





//=============================================================================
//		NData::ReleaseLarge : Release the large state.
//-----------------------------------------------------------------------------
void NData::ReleaseLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());
	NN_REQUIRE(mLarge.theState->numOwners != 0);



	// Release the state
	//
	// The last owner releases the state and the data, if we own it.
	if (mLarge.theState->numOwners.fetch_sub(1) == 1)
	{
		if (mLarge.theState->ownedData)
		{
			free(const_cast<uint8_t*>(mLarge.theState->theData));
		}

		delete mLarge.theState;
	}
}





//=============================================================================
//		NData::AdoptData : Adopt the data from another object.
//-----------------------------------------------------------------------------
void NData::AdoptData(const NData& otherData)
{


	// Validate our parameters and state
	NN_REQUIRE(this != &otherData);
	NN_REQUIRE(mSmall.sizeFlags == kSmallEmpty);



	// Adopt the data
	mSmall = otherData.mSmall;

	if (IsLarge())
	{
		RetainLarge();
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
//		NData::MemAllocate : Allocate bytes.
//-----------------------------------------------------------------------------
void* NData::MemAllocate(size_t theSize, const void* existingPtr, bool zeroMem)
{


	// Validate our parameters
	NN_REQUIRE(theSize != 0);



	// Allocate the memory
	//
	// Zero'd memory is allocated with calloc so that larger allocations
	// can be provided by zero'd pages rather than malloc+memset.
	//
	// Otherwise we use realloc to avoid any unnecessary fill when the
	// data is about to be overwritten or does not need initialisation.
	//
	// realloc can also avoid a malloc+memcpy+free if the existing data
	// sits within some larger underlying allocation.
	void* newPtr = nullptr;

	if (zeroMem)
	{
		if (existingPtr == nullptr)
		{
			newPtr = calloc(1, theSize);
		}
		else
		{
			newPtr = realloc(const_cast<void*>(existingPtr), theSize);
			memset(newPtr, 0x00, theSize);
		}
	}
	else
	{
		newPtr = realloc(const_cast<void*>(existingPtr), theSize);
	}

	return newPtr;
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
//		NData::GetSizeLarge : Get the large size.
//-----------------------------------------------------------------------------
size_t NData::GetSizeLarge() const
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the size
	return mLarge.theSlice.GetSize();
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
		mSmall.sizeFlags = uint8_t((theSize << kSmallSizeShift) | kSmallFlagIsSmall);
	}


	// Switch to large
	//
	// Copy the small data into large storage, then resize as required.
	else
	{
		MakeLarge(theSize, GetSizeSmall(), mSmall.theData, NDataSource::Copy);
		SetSizeLarge(theSize);
	}
}





//=============================================================================
//		NData::SetSizeLarge : Set the large size.
//-----------------------------------------------------------------------------
void NData::SetSizeLarge(size_t theSize)
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Reduce the size
	if (theSize <= mLarge.theSlice.GetSize())
	{
		mLarge.theSlice.SetSize(theSize);
	}


	// Increase the size
	else
	{
		// We can use our storage
		//
		// If the requested size is within our data, and we are the only
		// owner of this state, we can just expand our slice.
		if (theSize <= mLarge.theState->theSize && mLarge.theState->numOwners == 1)
		{
			mLarge.theSlice.SetSize(theSize);
		}


		// We require new storage
		//
		// Copy the large data into a new larger copy, then adjust our slice.
		else
		{
			MakeLarge(theSize, mLarge.theSlice.GetSize(), mLarge.theData, NDataSource::Copy);
			mLarge.theSlice.SetSize(theSize);
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
//		NData::GetCapacityLarge : Get the large capacity.
//-----------------------------------------------------------------------------
size_t NData::GetCapacityLarge() const
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the capacity
	return mLarge.theState->theSize;
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
	// of large storage.
	size_t theSize = GetSizeSmall();

	if (theCapacity < theSize)
	{
		SetSizeSmall(theCapacity);
	}



	// Switch to large
	//
	// Larger data must be stored externally.
	else if (theCapacity > kSmallSizeMax)
	{
		MakeLarge(theCapacity, theSize, mSmall.theData, NDataSource::Copy);
	}
}





//=============================================================================
//		NData::SetCapacityLarge : Set the large capacity.
//-----------------------------------------------------------------------------
void NData::SetCapacityLarge(size_t theCapacity)
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Set the capacity
	size_t theSize = std::min(mLarge.theSlice.GetSize(), theCapacity);

	MakeLarge(theCapacity, theSize, mLarge.theData, NDataSource::Copy);
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
//		NData::GetDataLarge : Get large data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataLarge(size_t theOffset) const
{


	// Validate our state
	NN_REQUIRE(IsLarge());
	NN_REQUIRE(theOffset < mLarge.theSlice.GetSize());



	// Get the data
	return &mLarge.theData[mLarge.theSlice.GetOffset(theOffset)];
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
	// Even if the new data is within our data we can just copy it to the start.
	if (IsSmall())
	{
		MemCopy(mSmall.theData, theData, theSize, theSource);
	}


	// Set when large
	//
	// If the new data was within our data it would be lost when we released
	// the current state.
	//
	// As the new data is small we just make a local copy then copy it back
	// to small storage once we release the state.
	else
	{
		NDataStorageSmall tmpData;

		MemCopy(tmpData.theData, theData, theSize, theSource);
		ReleaseLarge();
		MemCopy(mSmall.theData, tmpData.theData, theSize, theSource);
	}



	// Update our state
	SetSizeSmall(theSize);
}





//=============================================================================
//		NData::SetDataLarge : Set large data.
//-----------------------------------------------------------------------------
void NData::SetDataLarge(size_t theSize, const void* theData, NDataSource theSource)
{


	// Set when small
	if (IsSmall())
	{
		MakeLarge(theSize, theSize, theData, theSource);
	}



	// Set when large
	else
	{
		// Get the state we need
		uintptr_t srcFirst = reinterpret_cast<uintptr_t>(theData);
		uintptr_t srcLast  = srcFirst + theSize - 1;

		uintptr_t ourFirst = reinterpret_cast<uintptr_t>(mLarge.theData);
		uintptr_t ourLast  = ourFirst + mLarge.theState->theSize - 1;

		bool withinOurData = (srcFirst >= ourFirst && srcFirst <= ourLast) &&
							 (srcLast >= ourFirst && srcLast <= ourLast);


		// Adjust our slice
		//
		// If the source is within our data, and we're not clearing the
		// data, we can simply adjust our slice.
		if (withinOurData && theSource != NDataSource::Zero)
		{
			mLarge.theSlice.SetRange(srcFirst - ourFirst, theSize);
		}



		// Reuse our data
		//
		// If we're the only owner of our data, and the new data would
		// fit, copy into into our existing allocation.
		else if (mLarge.theState->theSize >= theSize && mLarge.theState->numOwners == 1)
		{
			MemCopy(const_cast<uint8_t*>(mLarge.theData), theData, theSize, theSource);
			mLarge.theSlice.SetRange(0, theSize);
		}



		// Create new data
		//
		// Otherwise we need to move the data into some new storage.
		else
		{
			MakeLarge(theSize, theSize, theData, theSource);
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
//		NData::RemoveDataLarge : Remove large data.
//-----------------------------------------------------------------------------
void NData::RemoveDataLarge(const NRange& theRange)
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Remove from the front
	//
	// Remove by adjusting our slice.
	if (theRange.GetLocation() == 0)
	{
		mLarge.theSlice.SetLocation(theRange.GetNext());
		mLarge.theSlice.SetSize(mLarge.theSlice.GetSize() - theRange.GetSize());
	}


	// Remove from the end
	//
	// Remove by adjusting our slice
	else if (theRange.GetLast() == mLarge.theSlice.GetLast())
	{
		mLarge.theSlice.SetSize(mLarge.theSlice.GetSize() - theRange.GetSize());
	}



	// Remove from the middle
	else
	{
		uint8_t* theData = GetMutableData();

		size_t dstoffset = theRange.GetFirst();
		size_t srcOffset = theRange.GetNext();
		size_t srcSize   = mLarge.theSlice.GetSize() - srcOffset;

		memmove(&theData[dstoffset], &theData[srcOffset], srcSize);
		mLarge.theSlice.SetSize(mLarge.theSlice.GetSize() - theRange.GetSize());
	}
}
