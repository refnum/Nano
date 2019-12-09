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
#include "NDataDigest.h"
#include "NDebug.h"

// System
#include <algorithm>
#include <cstddef>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint8_t kNDataFlagIsLarge                  = 0b00000001;
static constexpr uint8_t kNDataSmallSizeMask                = 0b11111000;
static constexpr uint8_t kNDataSmallSizeShift               = 3;
static constexpr uint8_t kNDataSmallSizeMax                 = 23;

static constexpr uint8_t kNDataEmpty                        = 0;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
// Large data state
//
// Holds the state for large data.
struct NDataState
{
	std::atomic_size_t numOwners;
	bool               isView;
	size_t             theSize;
	const uint8_t*     theData;
};





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(size_t theSize, const void* theData, NDataSource theSource)
	: mData{}
{


	// Initialise ourselves
	SetData(theSize, theData, theSource);
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData()
	: mData{}
{


	// Validate our state
	static_assert(sizeof(NDataStorage) == 32);
	static_assert(offsetof(NDataStorage, Small.sizeFlags) == 0);
	static_assert(offsetof(NDataStorage, Small.theData) == 1);
	static_assert(offsetof(NDataStorage, Large.theState) == 0);
	static_assert(offsetof(NDataStorage, Large.theSlice) == 8);
	static_assert(offsetof(NDataStorage, theHash) == 24);

	static_assert(sizeof(mData.Small.theData) == kNDataSmallSizeMax);

	static_assert(alignof(std::max_align_t) > 1, "Large flag requires LSB be free");
	static_assert(NN_ENDIAN_LITTLE, "Small/Large flag no longer overlap!");
	static_assert(kNDataEmpty == 0, "Small should be zero-initialised");
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
	: mData{}
{


	// Initialise ourselves
	MakeCopy(otherData);
}





//=============================================================================
//		NData::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NData& NData::operator=(const NData& otherData)
{


	// Assign the value
	if (this != &otherData)
	{
		MakeCopy(otherData);
	}

	return *this;
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(NData&& otherData)
	: mData{}
{


	// Initialise ourselves
	MakeCopy(otherData);
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
		MakeCopy(otherData);
		otherData.Clear();
	}

	return *this;
}





//=============================================================================
//		NData::Clear : Clear the data.
//-----------------------------------------------------------------------------
void NData::Clear()
{


	// Clear the data
	if (IsLarge())
	{
		ReleaseLarge();
	}

	memset(&mData, 0x00, sizeof(mData));
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
			theData.MakeCopy(*this);
			NN_REQUIRE(theData.IsLarge());

			theData.mData.Large.theSlice = theRange;
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

	ClearHash();

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
	// Views are implicitly shared, so must use large data.
	if (theSize <= kNDataSmallSizeMax && theSource != NDataSource::View)
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



	// Replace the data
	if (theRange == NRange(0, GetSize()))
	{
		// Replace everything
		SetData(theSize, theData, theSource);
	}
	else
	{
		// Adjust the data
		//
		// If the range we're replacing is a different size than the data we are
		// replacing it with then we need to insert filler / remove data prior
		// to overwriting.
		//
		// Any filter is inserted at the end of the area we're going to overwrite,
		// to minimise the amount of data that needs to be moved up.
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
		mData.theHash = NDataDigest::GetRuntime(GetSize(), GetData());
	}

	return mData.theHash;
}





#pragma mark private
//=============================================================================
//		NData::IsSmall : Are we using small storage?
//-----------------------------------------------------------------------------
bool NData::IsSmall() const
{


	// Check the flag
	return (mData.Small.sizeFlags & kNDataFlagIsLarge) == 0;
}





//=============================================================================
//		NData::IsLarge : Are we using large storage?
//-----------------------------------------------------------------------------
bool NData::IsLarge() const
{


	// Check the flag
	return (mData.Small.sizeFlags & kNDataFlagIsLarge) != 0;
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
//		NData::MakeCopy : Make a copy of another object.
//-----------------------------------------------------------------------------
void NData::MakeCopy(const NData& otherData)
{


	// Validate our parameters and state
	NN_REQUIRE(this != &otherData);
	NN_REQUIRE(IsSmall());



	// Copy the data
	mData = otherData.mData;

	if (IsLarge())
	{
		RetainLarge();
	}
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

	if (IsLarge())
	{
		NDataState* largeState = GetLarge();

		if (largeState->numOwners == 1)
		{
			// Switching to a view
			//
			// If we're switching to a view, and we don't currently have
			// a view, we need to release the existing data first.
			if (theSource == NDataSource::View && !largeState->isView)
			{
				free(const_cast<uint8_t*>(largeState->theData));
				largeState->theData = nullptr;
			}


			// Get the state we need
			existingState = largeState;
			existingData  = largeState->theData;
		}
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
	theState->isView    = (theSource == NDataSource::View);
	theState->theSize   = theCapacity;
	theState->theData   = static_cast<uint8_t*>(newData);



	// Switch to large data
	if (existingState == nullptr)
	{
		Clear();
	}

	SetLarge(theState, theSize);
}





//=============================================================================
//		NData::MakeMutable : Make the data mutable.
//-----------------------------------------------------------------------------
void NData::MakeMutable()
{


	// Copy the data
	//
	// Small data is always mutable, as we own the only copy.
	//
	// Large data of which we are the only owner can be mutated in place.
	//
	// Large data that's shared by other objects, or which is a view onto
	// exernal data, must be copied to allow mutation.
	if (IsLarge())
	{
		const NDataState* largeState = GetLarge();

		if (largeState->isView || largeState->numOwners != 1)
		{
			NRange         srcSlice = mData.Large.theSlice;
			size_t         srcSize  = srcSlice.GetSize();
			const uint8_t* srcData  = largeState->theData + srcSlice.GetLocation();

			MakeLarge(srcSize, srcSize, srcData, NDataSource::Copy);
		}
	}
}





//=============================================================================
//		NData::GetLarge : Get the large state.
//-----------------------------------------------------------------------------
NDataState* NData::GetLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the large state
	const NDataState* theState = mData.Large.theState;
	uintptr_t statePtr = reinterpret_cast<uintptr_t>(theState) & ~uintptr_t(kNDataFlagIsLarge);

	return reinterpret_cast<NDataState*>(statePtr);
}





//=============================================================================
//		NData::SetLarge : Get the large state.
//-----------------------------------------------------------------------------
void NData::SetLarge(NDataState* theState, size_t theSize)
{


	// Set the large state
	uintptr_t statePtr = reinterpret_cast<uintptr_t>(theState) | uintptr_t(kNDataFlagIsLarge);

	mData.Large.theState = reinterpret_cast<NDataState*>(statePtr);
	mData.Large.theSlice = NRange(0, theSize);



	// Validate our state
	NN_REQUIRE(IsLarge());
}





//=============================================================================
//		NData::RetainLarge : Retain the large state.
//-----------------------------------------------------------------------------
void NData::RetainLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the state we need
	NDataState* largeState = GetLarge();
	NN_REQUIRE(largeState->numOwners != 0);



	// Retain the state
	largeState->numOwners += 1;
}





//=============================================================================
//		NData::ReleaseLarge : Release the large state.
//-----------------------------------------------------------------------------
void NData::ReleaseLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the state we need
	NDataState* largeState = GetLarge();
	NN_REQUIRE(largeState->numOwners != 0);



	// Release the state
	//
	// The last owner releases the state, and any data that we own.
	if (largeState->numOwners.fetch_sub(1) == 1)
	{
		if (!largeState->isView)
		{
			free(const_cast<uint8_t*>(largeState->theData));
		}

		delete largeState;
	}
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
	// Other allocations use realloc to avoid any unnecessary fill when the
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
	return size_t((mData.Small.sizeFlags & kNDataSmallSizeMask) >> kNDataSmallSizeShift);
}





//=============================================================================
//		NData::GetSizeLarge : Get the large size.
//-----------------------------------------------------------------------------
size_t NData::GetSizeLarge() const
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the size
	return mData.Large.theSlice.GetSize();
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
	if (theSize <= kNDataSmallSizeMax)
	{
		mData.Small.sizeFlags = uint8_t(theSize << kNDataSmallSizeShift);
	}


	// Switch to large
	//
	// Copy the small data into new large storage, then resize as required.
	else
	{
		MakeLarge(theSize, GetSizeSmall(), mData.Small.theData, NDataSource::Copy);
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



	// Switch to small size
	//
	// A reduction below the small threshold means a change in storage.
	if (theSize <= kNDataSmallSizeMax)
	{
		SetDataSmall(theSize, GetData(), NDataSource::Copy);
	}


	// Reduce large size
	//
	// A reduction below our current size means a change in slice.
	else if (theSize <= mData.Large.theSlice.GetSize())
	{
		mData.Large.theSlice.SetSize(theSize);
	}


	// Increase large size
	//
	// Anything else increases our slice, and potentially our storage.
	else
	{
		// We can use our storage
		//
		// If the requested size is within our data, and we are the only
		// owner of this state, we can just expand our slice.
		NDataState* largeState = GetLarge();

		if (theSize <= largeState->theSize && largeState->numOwners == 1)
		{
			mData.Large.theSlice.SetSize(theSize);
		}


		// We require new storage
		//
		// Copy the large data into a new larger copy, then adjust our slice.
		else
		{
			MakeLarge(theSize,
					  mData.Large.theSlice.GetSize(),
					  largeState->theData,
					  NDataSource::Copy);
			mData.Large.theSlice.SetSize(theSize);
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
	return kNDataSmallSizeMax;
}





//=============================================================================
//		NData::GetCapacityLarge : Get the large capacity.
//-----------------------------------------------------------------------------
size_t NData::GetCapacityLarge() const
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the capacity
	//
	// We can cast away const as we only need to read the capacity.
	NData*      thisData   = const_cast<NData*>(this);
	NDataState* largeState = thisData->GetLarge();

	return largeState->theSize;
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
	// If the requested capacity is less than our current size then
	// our size must be reduced accordingly.
	//
	// This gives us consistent behaviour with large storage, even
	// though small storage always reports a fixed capacity.
	size_t theSize = GetSizeSmall();

	if (theCapacity < theSize)
	{
		SetSizeSmall(theCapacity);
	}



	// Switch to large
	//
	// Larger data must be stored externally.
	else if (theCapacity > kNDataSmallSizeMax)
	{
		MakeLarge(theCapacity, theSize, mData.Small.theData, NDataSource::Copy);
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
	size_t      theSize    = std::min(mData.Large.theSlice.GetSize(), theCapacity);
	NDataState* largeState = GetLarge();

	MakeLarge(theCapacity, theSize, largeState->theData, NDataSource::Copy);
}





//=============================================================================
//		NData::GetDataSmall : Get small data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataSmall(size_t theOffset) const
{


	// Validate our state
	NN_REQUIRE(IsSmall());
	NN_REQUIRE(theOffset <= kNDataSmallSizeMax);



	// Get the data
	return &mData.Small.theData[theOffset];
}





//=============================================================================
//		NData::GetDataLarge : Get large data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataLarge(size_t theOffset) const
{


	// Validate our state
	NN_REQUIRE(IsLarge());
	NN_REQUIRE(theOffset < mData.Large.theSlice.GetSize());



	// Get the data
	//
	// We can cast away const as we restore it in our result.
	NData*      thisData   = const_cast<NData*>(this);
	NDataState* largeState = thisData->GetLarge();

	return &largeState->theData[mData.Large.theSlice.GetOffset(theOffset)];
}





//=============================================================================
//		NData::SetDataSmall : Set small data.
//-----------------------------------------------------------------------------
void NData::SetDataSmall(size_t theSize, const void* theData, NDataSource theSource)
{


	// Validate our parameters
	NN_REQUIRE(theSize <= kNDataSmallSizeMax);
	NN_REQUIRE(theSource != NDataSource::View);



	// Set when small
	//
	// Even if the new data is within our data we can just copy it to the start.
	if (IsSmall())
	{
		MemCopy(mData.Small.theData, theData, theSize, theSource);
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
		uint8_t tmpData[kNDataSmallSizeMax];

		MemCopy(tmpData, theData, theSize, theSource);
		ReleaseLarge();
		MemCopy(mData.Small.theData, tmpData, theSize, theSource);
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

		NDataState* largeState = GetLarge();
		uintptr_t   largeFirst = reinterpret_cast<uintptr_t>(largeState->theData);
		uintptr_t   largeLast  = largeFirst + largeState->theSize - 1;

		bool withinLargeData = (srcFirst >= largeFirst && srcFirst <= largeLast) &&
							   (srcLast >= largeFirst && srcLast <= largeLast);


		// Adjust our slice
		//
		// If the source is within our data, and we're not clearing the
		// data, we can simply adjust our slice.
		if (withinLargeData && theSource != NDataSource::Zero)
		{
			mData.Large.theSlice.SetRange(srcFirst - largeFirst, theSize);
		}



		// Reuse our data
		//
		// If we're the only owner of our data, and the new data would
		// fit, copy into into our existing allocation.
		else if (largeState->theSize >= theSize && largeState->numOwners == 1)
		{
			MemCopy(const_cast<uint8_t*>(largeState->theData), theData, theSize, theSource);
			mData.Large.theSlice.SetRange(0, theSize);
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
	// Removal from the end is handled by our final size adjustment.
	size_t oldSize = GetSizeSmall();
	size_t newSize = oldSize - theRange.GetSize();

	if (theRange.GetNext() != oldSize)
	{
		size_t dstoffset = theRange.GetFirst();
		size_t srcOffset = theRange.GetNext();
		size_t srcSize   = oldSize - srcOffset;

		memmove(&mData.Small.theData[dstoffset], &mData.Small.theData[srcOffset], srcSize);
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



	// Get the state we need
	NRange& largeSlice = mData.Large.theSlice;


	// Remove from the front
	//
	// Remove by adjusting our slice.
	if (theRange.GetLocation() == 0)
	{
		largeSlice.SetLocation(theRange.GetNext());
		largeSlice.SetSize(largeSlice.GetSize() - theRange.GetSize());
	}



	// Remove from the end
	//
	// Remove by adjusting our slice
	else if (theRange.GetLast() == largeSlice.GetLast())
	{
		largeSlice.SetSize(largeSlice.GetSize() - theRange.GetSize());
	}



	// Remove from the middle
	else
	{
		uint8_t* theData = GetMutableData();

		size_t dstoffset = theRange.GetFirst();
		size_t srcOffset = theRange.GetNext();
		size_t srcSize   = largeSlice.GetSize() - srcOffset;

		memmove(&theData[dstoffset], &theData[srcOffset], srcSize);
		largeSlice.SetSize(largeSlice.GetSize() - theRange.GetSize());
	}
}
