/*	NAME:
		NData.cpp

	DESCRIPTION:
		Data object.

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
#include "NData.h"

// Nano
#include "NDataDigest.h"
#include "NDebug.h"
#include "NStdAlgorithm.h"





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
NData::NData(const NData& otherData)
	: mData{otherData.mData}
{


	// Initialise ourselves
	if (IsLarge())
	{
		RetainLarge();
	}
}





//=============================================================================
//		NData::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NData& NData::operator=(const NData& otherData)
{


	// Assign the data
	if (this != &otherData)
	{
		if (IsLarge())
		{
			ReleaseLarge();
		}

		mData = otherData.mData;

		if (IsLarge())
		{
			RetainLarge();
		}
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
	std::swap(mData, otherData.mData);
}





//=============================================================================
//		NData::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NData& NData::operator=(NData&& otherData)
{


	// Move the data
	if (this != &otherData)
	{
		std::swap(mData, otherData.mData);
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

	mData = {};
}





//=============================================================================
//		NData::SetSize : Set the size.
//-----------------------------------------------------------------------------
void NData::SetSize(size_t theSize)
{


	// Set the size
	size_t oldSize = GetSize();

	if (theSize != oldSize)
	{
		// Adjust the size
		//
		// We zero-fill any new space.
		SetSizeRaw(theSize);

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
//		NData::SetCapacity : Set the capacity.
//-----------------------------------------------------------------------------
void NData::SetCapacity(size_t theCapacity)
{


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


	// Get the data
	NRange finalRange = theRange.GetNormalized(GetSize());
	NData  theData;

	if (!finalRange.IsEmpty() && IsValidRange(finalRange))
	{
		if (IsSmall())
		{
			theData.SetData(finalRange.GetSize(), GetData(finalRange.GetLocation()));
		}
		else
		{
			theData = *this;
			NN_REQUIRE(theData.IsLarge());

			auto& theSlice = theData.mData.Large.theSlice;
			theSlice       = finalRange.GetOffset(theSlice.GetLocation());
		}
	}

	return theData;
}





//=============================================================================
//		NData::GetMutableData : Get mutable data.
//-----------------------------------------------------------------------------
uint8_t* NData::GetMutableData(size_t theOffset)
{


	// Get the data
	uint8_t* thePtr = nullptr;

	if (theOffset < GetSize())
	{
		// Get the data
		//
		// Once mutable we can safely cast away the const as we know
		// we are now the only owner of the data.
		MakeMutable();

		thePtr = const_cast<uint8_t*>(GetData(theOffset));



		// Update our state
		//
		// Mutable access implicitly resets our hash.
		ClearHash();
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
	// Views are implicitly shared, so must use large data.
	if (theSize <= kNDataSmallSizeMax && theSource != NDataSource::View)
	{
		SetSmall(theSize, theData, theSource);
	}
	else
	{
		SetLarge(theSize, theData, theSource);
	}



	// Update our state
	ClearHash();
}





//=============================================================================
//		NData::Insert : Insert data.
//-----------------------------------------------------------------------------
uint8_t* NData::Insert(size_t beforeIndex, const NData& theData)
{


	// Insert the data
	return Insert(beforeIndex, theData.GetSize(), theData.GetData(), NDataSource::Copy);
}





//=============================================================================
//		NData::Insert : Insert data.
//-----------------------------------------------------------------------------
uint8_t* NData::Insert(size_t      beforeIndex,
					   size_t      theSize,
					   const void* theData,
					   NDataSource theSource)
{


	// Validate our parameters
	NN_REQUIRE(beforeIndex <= GetSize());
	NN_REQUIRE(IsValidSource(theSize, theData, theSource));



	// Insert the data
	uint8_t* newData = nullptr;

	if (theSize != 0)
	{
		// Grow the buffer
		//
		// New space will be filled by MemCopy based on the source.
		size_t oldSize = GetSize();
		size_t newSize = oldSize + theSize;

		SetSizeRaw(newSize);



		// Insert the data
		//
		// Data after the inserted area must first be moved towards the end.
		newData = GetMutableData(beforeIndex);

		uint8_t* movedPtr  = newData + theSize;
		size_t   movedSize = oldSize - beforeIndex;

		if (movedSize != 0)
		{
			memmove(movedPtr, newData, movedSize);
		}

		MemCopy(newData, theData, theSize, theSource);



		// Update our state
		ClearHash();
	}

	return newData;
}





//=============================================================================
//		NData::Append : Append data.
//-----------------------------------------------------------------------------
uint8_t* NData::Append(const NData& theData)
{


	// Append the data
	return Insert(GetSize(), theData);
}





//=============================================================================
//		NData::Append : Append data.
//-----------------------------------------------------------------------------
uint8_t* NData::Append(size_t theSize, const void* theData, NDataSource theSource)
{


	// Appemd the data
	return Insert(GetSize(), theSize, theData, theSource);
}





//=============================================================================
//		NData::Remove : Remove data.
//-----------------------------------------------------------------------------
void NData::Remove(const NRange& theRange)
{


	// Remove the data
	NRange finalRange = theRange.GetNormalized(GetSize());

	if (!finalRange.IsEmpty())
	{
		NN_REQUIRE(IsValidRange(finalRange));

		if (IsSmall())
		{
			RemoveSmall(finalRange);
		}
		else
		{
			RemoveLarge(finalRange);
		}
	}


	// Update our state
	ClearHash();
}





//=============================================================================
//		NData::Replace : Replace data.
//-----------------------------------------------------------------------------
uint8_t* NData::Replace(const NRange& theRange, const NData& theData)
{


	// Replace the data
	return Replace(theRange, theData.GetSize(), theData.GetData(), NDataSource::Copy);
}





//=============================================================================
//		NData::Replace : Replace data.
//-----------------------------------------------------------------------------
uint8_t* NData::Replace(const NRange& theRange,
						size_t        theSize,
						const void*   theData,
						NDataSource   theSource)
{


	// Validate our parameters
	NN_REQUIRE(IsValidSource(theSize, theData, theSource));



	// Replace everything
	NRange finalRange = theRange.GetNormalized(GetSize());

	if (finalRange == NRange(0, GetSize()))
	{
		SetData(theSize, theData, theSource);
	}


	// Replace a range
	else
	{
		// Replace from the end
		//
		// If the range we're replacing is at the end we can just
		// adjust our size then fill the new contents with MemCopy.
		if (finalRange.GetNext() == GetSize())
		{
			size_t oldSize = GetSize();
			size_t newSize = oldSize + theSize - finalRange.GetSize();

			SetSizeRaw(newSize);
		}

		// Replace elsewhere
		else
		{
			// Adjust the data
			//
			// If the range we're replacing is a different size than the data we are
			// replacing it with then we need to insert filler / remove data prior
			// to overwriting.
			//
			// Any filler is inserted at the end of the area we're going to overwrite,
			// to minimise the amount of data that needs to be moved up.
			size_t replacedSize = finalRange.GetSize();

			if (replacedSize > theSize)
			{
				Remove(NRange(finalRange.GetLocation(), replacedSize - theSize));
			}
			else
			{
				Insert(finalRange.GetNext(), theSize - replacedSize, nullptr, NDataSource::None);
			}
		}



		// Replace the range
		if (theSize != 0)
		{
			uint8_t* dstPtr = GetMutableData(finalRange.GetLocation());
			MemCopy(dstPtr, theData, theSize, theSource);
		}
	}



	// Update our state
	ClearHash();



	// Get the replaced data
	//
	// If the replacement was a removal from the end then the range location
	// will now be after our last byte so we must return nullptr.
	size_t theOffset = finalRange.GetLocation();

	if (theOffset >= GetSize())
	{
		return nullptr;
	}

	return GetMutableData(theOffset);
}





//=============================================================================
//		NData::ReplaceAppend : Replace the end of the data.
//-----------------------------------------------------------------------------
//		Note : Optimised for NString::Append usage.
//-----------------------------------------------------------------------------
void NData::ReplaceAppend(size_t sizeReplace, size_t theSize, const void* theData)
{


	// Validate our parameters
	//
	// We require a non-zero amount of data that is external to us.
	NN_REQUIRE(sizeReplace != 0 && sizeReplace < GetSize());
	NN_REQUIRE(theSize != 0 && sizeReplace <= theSize);
	NN_REQUIRE_NOT_NULL(theData);

	NN_REQUIRE(((uintptr_t(theData) + theSize) < uintptr_t(GetData())) ||
			   (uintptr_t(theData) > uintptr_t(GetData() + GetSize())));



	// Get the state we need
	size_t oldSize = GetSize();
	size_t newSize = oldSize - sizeReplace + theSize;

	size_t   dstOffset = oldSize - sizeReplace;
	uint8_t* dstPtr    = nullptr;



	// Replace the data
	//
	// We test once up front for the small / large decisions.
	if (IsSmall())
	{
		// Small data is always mutable
		SetSizeSmall(newSize);

		dstPtr = const_cast<uint8_t*>(GetSmall(dstOffset));
	}
	else
	{
		// Large data only needs to be mutable for views or shared data.
		SetSizeLarge(newSize);
		if (mData.Large.theState->isView || mData.Large.theState->numOwners != 1)
		{
			MakeMutable();
		}

		dstPtr = const_cast<uint8_t*>(GetLarge(dstOffset));
	}

	memcpy(dstPtr, theData, theSize);



	// Update our state
	ClearHash();
}





//=============================================================================
//		NData::Find : Find the first instance of data within the data.
//-----------------------------------------------------------------------------
NRange NData::Find(const NData& theData, const NRange& theRange) const
{


	// Validate our parameters
	NN_REQUIRE(!theData.IsEmpty());



	// Get the state we need
	NRange         searchRange = theRange.GetNormalized(GetSize());
	size_t         searchSize  = searchRange.GetSize();
	const uint8_t* searchBegin = GetData(searchRange.GetLocation());
	const uint8_t* searchEnd   = searchBegin + searchSize;

	size_t         needleSize  = theData.GetSize();
	const uint8_t* needleBegin = theData.GetData();
	const uint8_t* needleEnd   = needleBegin + needleSize;



	// Find the data
	NRange theResult;

	if (searchSize != 0)
	{
		auto findBegin = nstd::search(searchBegin, searchEnd, needleBegin, needleEnd);
		if (findBegin != searchEnd)
		{
			theResult.SetRange(size_t(findBegin - searchBegin), needleSize);
		}
	}

	return theResult;
}





//=============================================================================
//		NData::FindAll : Find data within the data.
//-----------------------------------------------------------------------------
NVectorRange NData::FindAll(const NData& theData, const NRange& theRange) const
{


	// Get the state we need
	NRange         searchRange = theRange.GetNormalized(GetSize());
	size_t         searchSize  = searchRange.GetSize();
	const uint8_t* searchBase  = GetData();
	const uint8_t* searchBegin = searchBase + searchRange.GetLocation();
	const uint8_t* searchEnd   = searchBegin + searchSize;

	size_t         needleSize  = theData.GetSize();
	const uint8_t* needleBegin = theData.GetData();
	const uint8_t* needleEnd   = needleBegin + needleSize;



	// Find the data
	NVectorRange theResult;

	while (searchBegin < searchEnd)
	{
		auto findBegin = nstd::search(searchBegin, searchEnd, needleBegin, needleEnd);
		if (findBegin == searchEnd)
		{
			break;
		}

		theResult.push_back({size_t(findBegin - searchBase), needleSize});
		searchBegin = findBegin + needleSize;
	}

	return theResult;
}





//=============================================================================
//		NData::StartsWith : Does the data start with some data?
//-----------------------------------------------------------------------------
bool NData::StartsWith(const NData& theData) const
{


	// Validate our parameters
	NN_REQUIRE(!theData.IsEmpty());



	// Find the data
	size_t thisSize  = GetSize();
	size_t otherSize = theData.GetSize();
	bool   isMatch   = (thisSize >= otherSize);

	if (isMatch)
	{
		isMatch = (memcmp(GetData(), theData.GetData(), otherSize) == 0);
	}

	return isMatch;
}





//=============================================================================
//		NData::EndsWith : Does the data end with some data?
//-----------------------------------------------------------------------------
bool NData::EndsWith(const NData& theData) const
{


	// Validate our parameters
	NN_REQUIRE(!theData.IsEmpty());



	// Find the data
	size_t thisSize  = GetSize();
	size_t otherSize = theData.GetSize();
	bool   isMatch   = (thisSize >= otherSize);

	if (isMatch)
	{
		isMatch = (memcmp(GetData(thisSize - otherSize), theData.GetData(), otherSize) == 0);
	}

	return isMatch;
}





//=============================================================================
//		NData::Contains : Does the data contain some data?
//-----------------------------------------------------------------------------
bool NData::Contains(const NData& theData, const NRange& theRange) const
{


	// Validate our parameters
	NN_REQUIRE(!theData.IsEmpty());



	// Find the data
	return !Find(theData, theRange).IsEmpty();
}





#pragma mark NMixinComparable
//=============================================================================
//		NData::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NData::CompareEqual(const NData& theData) const
{


	// Compare the size
	//
	// A different size means no equality
	if (GetSize() != theData.GetSize())
	{
		return false;
	}


	// Compare the hash
	//
	// A different hash means no equality.
	if (GetHash() != theData.GetHash())
	{
		return false;
	}



	// Compare the data
	//
	// A hash collision could produce equal hashes so check the content.
	return CompareOrder(theData) == NComparison::EqualTo;
}





//=============================================================================
//		NData::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NData::CompareOrder(const NData& theData) const
{


	// Order by hash
	//
	// Data objects have no intrinsic ordering so we order by size first,
	// then fall back to a byte-wise comparison.
	NComparison theResult = NCompare(GetSize(), theData.GetSize());

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(GetSize(), GetData(), theData.GetSize(), theData.GetData());
	}

	return theResult;
}





#pragma mark NMixinHashable
//=============================================================================
//		NData::HashUpdate : Update the hash.
//-----------------------------------------------------------------------------
size_t NData::HashUpdate()
{


	// Update the hash
	mData.theHash = NDataDigest::GetRuntime32(GetSize(), GetData());

	return mData.theHash;
}





#pragma mark private
//=============================================================================
//		NData::IsValidRange : Is a range valid?
//-----------------------------------------------------------------------------
bool NData::IsValidRange(const NRange& theRange) const
{


	// Validate our parameters
	NN_REQUIRE(!theRange.IsMeta());



	// Check the range
	return IsValidOffset(theRange.GetFirst()) && IsValidOffset(theRange.GetLast());
}





//=============================================================================
//		NData::IsValidOffset : Is an offset valid?
//-----------------------------------------------------------------------------
bool NData::IsValidOffset(size_t theOffset) const
{


	// Check the offset
	return theOffset < GetSize();
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
//		NData::MakeLarge : Make large data.
//-----------------------------------------------------------------------------
void NData::MakeLarge(size_t      theCapacity,
					  size_t      theSize,
					  const void* theData,
					  NDataSource theSource)
{


	// Validate our parameters and state
	NN_REQUIRE(theCapacity != 0);
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
		if (mData.Large.theState->numOwners == 1)
		{
			// Switching to a view
			//
			// If we're switching to a view, and we're not currently a
			// view, we need to release our existing data first.
			if (theSource == NDataSource::View && !mData.Large.theState->isView)
			{
				free(const_cast<uint8_t*>(mData.Large.theState->theData));
				mData.Large.theState->theData = nullptr;
			}


			// Get the state we need
			existingState = mData.Large.theState;
			existingData  = mData.Large.theState->theData;
		}
	}



	// Create the data
	void* newData = nullptr;

	switch (theSource)
	{
		case NDataSource::Copy:
			// Handle data within our data
			//
			// If the supplied data is within our existing data then we can't
			// read from it after a MemAllocate as that may reallocate the
			// existing data (and invalidating the supplied pointer).
			//
			// As such we "copy" it by moving it to the start of the existing
			// data, assuming it doesn't already fall at the start.
			if (existingData != nullptr)
			{
				uintptr_t dataFirst     = reinterpret_cast<uintptr_t>(theData);
				uintptr_t existingFirst = reinterpret_cast<uintptr_t>(existingData);
				uintptr_t existingLast  = existingFirst + existingState->theSize;

				if (dataFirst >= existingFirst && dataFirst <= existingLast)
				{
					if (dataFirst != existingFirst)
					{
						memmove(const_cast<void*>(existingData), theData, theSize);
					}

					theData = nullptr;
				}
			}


			// Copy the data
			//
			// In general we resize our existing data then copy the supplied
			// data into it.
			//
			// If the supplied data fell within our existing data then we will
			// have already moved it to the start and we don't need to copy
			// even if we do end up reallocating.
			newData = MemAllocate(theCapacity, existingData, false);
			if (theData != nullptr)
			{
				memcpy(newData, theData, theSize);
			}
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
	// Small data is always mutable as we own the only copy.
	//
	// Large data of which we are the only owner can be mutated in place.
	//
	// Large data that's shared by other objects, or which is a view onto
	// exernal data, must be copied to allow mutation.
	if (IsLarge())
	{
		if (mData.Large.theState->isView || mData.Large.theState->numOwners != 1)
		{
			NRange         srcSlice = mData.Large.theSlice;
			size_t         srcSize  = srcSlice.GetSize();
			const uint8_t* srcData  = mData.Large.theState->theData + srcSlice.GetLocation();

			MakeLarge(srcSize, srcSize, srcData, NDataSource::Copy);
		}
	}
}





//=============================================================================
//		NData::SetLarge : Get the large state.
//-----------------------------------------------------------------------------
void NData::SetLarge(NDataState* theState, size_t theSize)
{


	// Set the large state
	mData.Large.theState = theState;
	mData.Large.theSlice = NRange(0, theSize);

	mData.theFlags = kNDataFlagIsLarge;
}





//=============================================================================
//		NData::RetainLarge : Retain the large state.
//-----------------------------------------------------------------------------
void NData::RetainLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());
	NN_REQUIRE(mData.Large.theState->numOwners != 0);



	// Retain the state
	mData.Large.theState->numOwners += 1;
}





//=============================================================================
//		NData::ReleaseLarge : Release the large state.
//-----------------------------------------------------------------------------
void NData::ReleaseLarge()
{


	// Validate our state
	NN_REQUIRE(IsLarge());
	NN_REQUIRE(mData.Large.theState->numOwners != 0);



	// Release the state
	//
	// The last owner releases the state, and any data that we own.
	if (mData.Large.theState->numOwners.fetch_sub(1) == 1)
	{
		if (!mData.Large.theState->isView)
		{
			free(const_cast<uint8_t*>(mData.Large.theState->theData));
		}

		delete mData.Large.theState;
		mData.Large.theState = nullptr;
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
	// realloc can also often avoid a malloc+memcpy+free if the existing data
	// is using a larger underlying allocation that can be expanded / shrunk
	// to fulfill the request.
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
//		NData::SetSizeRaw : Set the size.
//-----------------------------------------------------------------------------
void NData::SetSizeRaw(size_t theSize)
{


	// Set the size
	if (IsSmall())
	{
		SetSizeSmall(theSize);
	}
	else
	{
		SetSizeLarge(theSize);
	}
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
	// The small size is stored in the flag byte.
	if (theSize <= kNDataSmallSizeMax)
	{
		mData.theFlags = uint8_t(theSize);
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



	// Reduce large size
	//
	// A reduction below our current size means a change in slice.
	//
	// Large data is left as large data, rather than shrunk back to small,
	// as an object that was large once is likely to be large again.
	if (theSize <= mData.Large.theSlice.GetSize())
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
		if (theSize <= mData.Large.theState->theSize && mData.Large.theState->numOwners == 1)
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
					  mData.Large.theState->theData + mData.Large.theSlice.GetLocation(),
					  NDataSource::Copy);

			mData.Large.theSlice.SetSize(theSize);
		}
	}
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
	size_t theSize = std::min(mData.Large.theSlice.GetSize(), theCapacity);

	MakeLarge(theCapacity,
			  theSize,
			  mData.Large.theState->theData + mData.Large.theSlice.GetLocation(),
			  NDataSource::Copy);
}





//=============================================================================
//		NData::SetSmall : Set small data.
//-----------------------------------------------------------------------------
void NData::SetSmall(size_t theSize, const void* theData, NDataSource theSource)
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
		Clear();

		MemCopy(mData.Small.theData, tmpData, theSize, theSource);
	}



	// Update our state
	SetSizeSmall(theSize);
}





//=============================================================================
//		NData::SetLarge : Set large data.
//-----------------------------------------------------------------------------
void NData::SetLarge(size_t theSize, const void* theData, NDataSource theSource)
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

		uintptr_t largeFirst = reinterpret_cast<uintptr_t>(mData.Large.theState->theData);
		uintptr_t largeLast  = largeFirst + mData.Large.theState->theSize - 1;

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
		else if (mData.Large.theState->theSize >= theSize && mData.Large.theState->numOwners == 1)
		{
			MemCopy(const_cast<uint8_t*>(mData.Large.theState->theData),
					theData,
					theSize,
					theSource);
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
//		NData::RemoveSmall : Remove small data.
//-----------------------------------------------------------------------------
void NData::RemoveSmall(const NRange& theRange)
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
//		NData::RemoveLarge : Remove large data.
//-----------------------------------------------------------------------------
void NData::RemoveLarge(const NRange& theRange)
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the state we need
	NRange& largeSlice = mData.Large.theSlice;



	// Remove all
	if (theRange == NRange(0, largeSlice.GetSize()))
	{
		Clear();
	}



	// Remove from the front
	//
	// Remove by adjusting our slice.
	else if (theRange.GetLocation() == 0)
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
