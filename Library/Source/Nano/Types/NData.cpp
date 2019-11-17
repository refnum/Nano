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
static constexpr size_t kSizeSmallMax                       = NDataStorage::Small;
static constexpr size_t kSizeEmpty                          = NDataStorage::Small;

static constexpr uint8_t kStorageSmall                      = 0x3F;
static constexpr uint8_t kStorageShared                     = (1 << 6);
static constexpr uint8_t kStorageExternal                   = (1 << 7);





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(size_t theSize, const void* theData, NDataUsage theUsage)
	: mStorage({})
	, mFlags(kSizeEmpty)
{
	// Initialise ourselves
	SetData(theSize, theData, theUsage);
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData()
	: mStorage{}
	, mFlags(kSizeEmpty)
{
	// Validate our state
	static_assert(kSizeSmallMax == sizeof(mStorage));
	static_assert(kSizeSmallMax <= kStorageSmall);
}





//=============================================================================
//		NData::~NData : Destructor.
//-----------------------------------------------------------------------------
NData::~NData()
{


	// Clean up
	ReleaseData();
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(const NData& otherData)
	: mStorage{}
	, mFlags(kSizeEmpty)
{
	// TODO
	NN_UNUSED(otherData);
}





//=============================================================================
//		NData::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NData& NData::operator=(const NData& otherData)
{


	// TODO
	NN_UNUSED(otherData);

	return *this;
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(NData&& otherData)
	: mStorage{}
	, mFlags(kStorageSmall)
{
	// TODO
	NN_UNUSED(otherData);
}





//=============================================================================
//		NData::operator= : Assignment operator.
//-----------------------------------------------------------------------------
NData& NData::operator=(NData&& otherData)
{


	// TODO
	NN_UNUSED(otherData);

	return *this;
}





//=============================================================================
//		NData::GetSize : Get the size.
//-----------------------------------------------------------------------------
size_t NData::GetSize() const
{


	// Get the size
	size_t theSize = 0;

	switch (GetStorage())
	{
		case NDataStorage::Small:
			theSize = GetSizeSmall();
			break;

		case NDataStorage::Shared:
			theSize = GetSizeShared();
			break;

		case NDataStorage::External:
			theSize = GetSizeExternal();
			break;
	}

	return theSize;
}





//=============================================================================
//		NData::SetSize : Set the size.
//-----------------------------------------------------------------------------
void NData::SetSize(size_t theSize)
{


	// TODO
	NN_UNUSED(theSize);
}





//=============================================================================
//		NData::GetCapacity : Get the capacity.
//-----------------------------------------------------------------------------
size_t NData::GetCapacity() const
{


	// Get the capacity
	size_t theCapacity = 0;

	switch (GetStorage())
	{
		case NDataStorage::Small:
			theCapacity = GetCapacitySmall();
			break;

		case NDataStorage::Shared:
			theCapacity = GetCapacityShared();
			break;

		case NDataStorage::External:
			theCapacity = GetCapacityExternal();
			break;
	}

	return theCapacity;
}





//=============================================================================
//		NData::SetCapacity : Set the capacity.
//-----------------------------------------------------------------------------
void NData::SetCapacity(size_t theSize)
{


	// TODO
	NN_UNUSED(theSize);
}





//=============================================================================
//		NData::GetData : Get the data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetData(size_t theOffset) const
{


	// Get the data
	const uint8_t* theData = nullptr;

	if (!IsEmpty())
	{
		// Get the data
		switch (GetStorage())
		{
			case NDataStorage::Small:
				theData = GetDataSmall();
				break;

			case NDataStorage::Shared:
				theData = GetDataShared();
				break;

			case NDataStorage::External:
				theData = GetDataExternal();
				break;
		}



		// Apply the offset
		if (theData != nullptr)
		{
			theData += theOffset;
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
	uint8_t* theData = nullptr;

	if (!IsEmpty())
	{
		MakeMutable();

		theData = const_cast<uint8_t*>(GetData(theOffset));
	}

	return theData;
}





//=============================================================================
//		NData::SetData : Set the data.
//-----------------------------------------------------------------------------
void NData::SetData(size_t theSize, const void* theData, NDataUsage theUsage)
{


	// Set the data
	if (theUsage == NDataUsage::View)
	{
		SetDataExternal(theSize, theData, theUsage);
	}
	else if (theSize <= kSizeSmallMax)
	{
		SetDataSmall(theSize, theData, theUsage);
	}
	else
	{
		SetDataShared(theSize, theData, theUsage);
	}
}





//=============================================================================
//		NData::InsertData : Insert data.
//-----------------------------------------------------------------------------
uint8_t* NData::InsertData(size_t beforeIndex, const NData& theData)
{


	// Insert the data
	return InsertData(beforeIndex, theData.GetSize(), theData.GetData(), NDataUsage::Copy);
}





//=============================================================================
//		NData::InsertData : Insert data.
//-----------------------------------------------------------------------------
uint8_t* NData::InsertData(size_t      beforeIndex,
						   size_t      theSize,
						   const void* theData,
						   NDataUsage  theUsage)
{


	// TODO
	NN_UNUSED(beforeIndex);
	NN_UNUSED(theSize);
	NN_UNUSED(theData);
	NN_UNUSED(theUsage);

	return nullptr;
}





//=============================================================================
//		NData::AppendData : Append data.
//-----------------------------------------------------------------------------
uint8_t* NData::AppendData(const NData& theData)
{


	// Append the data
	return AppendData(theData.GetSize(), theData.GetData(), NDataUsage::Copy);
}





//=============================================================================
//		NData::AppendData : Append data.
//-----------------------------------------------------------------------------
uint8_t* NData::AppendData(size_t theSize, const void* theData, NDataUsage theUsage)
{


	// TODO
	NN_UNUSED(theSize);
	NN_UNUSED(theData);
	NN_UNUSED(theUsage);

	return nullptr;
}





//=============================================================================
//		NData::RemoveData : Remove data.
//-----------------------------------------------------------------------------
void NData::RemoveData(const NRange& theRange)
{


	// Remove the data
	ReplaceData(theRange, 0, nullptr, NDataUsage::Zero);
}





//=============================================================================
//		NData::ReplaceData : Replace data.
//-----------------------------------------------------------------------------
uint8_t* NData::ReplaceData(const NRange& theRange, const NData& theData)
{


	// Replace the data
	return ReplaceData(theRange, theData.GetSize(), theData.GetData(), NDataUsage::Copy);
}





//=============================================================================
//		NData::ReplaceData : Replace data.
//-----------------------------------------------------------------------------
uint8_t* NData::ReplaceData(const NRange& theRange,
							size_t        theSize,
							const void*   theData,
							NDataUsage    theUsage)
{


	// TODO
	NN_UNUSED(theRange);
	NN_UNUSED(theSize);
	NN_UNUSED(theData);
	NN_UNUSED(theUsage);

	return nullptr;
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
//		NData::GetStorage : Get the storage type.
//-----------------------------------------------------------------------------
NDataStorage NData::GetStorage() const
{


	// Get the storage
	if (mFlags & kStorageExternal)
	{
		return NDataStorage::External;
	}

	else if (mFlags & kStorageShared)
	{
		return NDataStorage::Shared;
	}

	return NDataStorage::Small;
}





//=============================================================================
//		NData::ReleaseData : Release the data.
//-----------------------------------------------------------------------------
void NData::ReleaseData()
{


	// Release the data
	//
	// Shared data is released by the last owner.
	if (GetStorage() == NDataStorage::Shared)
	{
		if (mStorage.Shared.theBlock->numOwners.fetch_sub(1) == 1)
		{
			free(mStorage.Shared.theBlock);
		}
	}



	// Reset our state
#if NN_DEBUG
	memset(&mStorage, 0xAA, sizeof(mStorage));
#endif

	mFlags = kSizeEmpty;
}





//=============================================================================
//		NData::MakeMutable : Make the data mutable.
//-----------------------------------------------------------------------------
void NData::MakeMutable()
{


	// Get the state we need
	NRange         srcSlice;
	const uint8_t* srcData = nullptr;

	switch (GetStorage())
	{
		case NDataStorage::Small:
			// Always mutable
			break;

		case NDataStorage::Shared:
			// Copy if shared
			//
			// If we're the only owner it's safe to mutate in place.
			if (mStorage.Shared.theBlock->numOwners != 1)
			{
				srcSlice = mStorage.Shared.theSlice;
				srcData  = mStorage.Shared.theBlock->theData;
			}
			break;

		case NDataStorage::External:
			// Always copy
			srcSlice = mStorage.External.theSlice;
			srcData  = mStorage.External.theData;
			break;
	}



	// Copy the data
	//
	// Our data is mutable if we own the only copy.
	if (!srcSlice.IsEmpty())
	{
		// Find the data
		size_t srcSize = srcSlice.GetSize();
		srcData        = srcData + srcSlice.GetLocation();



		// Allocate a new block
		size_t      blockSize = sizeof(NDataBlock) + srcSize;
		NDataBlock* newBlock  = static_cast<NDataBlock*>(malloc(blockSize));

		newBlock->numOwners = 1;
		newBlock->theSize   = srcSize;

		memcpy(newBlock->theData, srcData, srcSize);



		// Attach to the new block
		ReleaseData();

		mStorage.Shared.theBlock = newBlock;
		mStorage.Shared.theSlice.SetRange(0, srcSize);

		mFlags = kStorageShared;
	}
}





//=============================================================================
//		NData::GetSizeSmall : Get the small size.
//-----------------------------------------------------------------------------
size_t NData::GetSizeSmall() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::Small);



	// Get the size
	return size_t(mFlags & kStorageSmall);
}





//=============================================================================
//		NData::GetSizeShared : Get the shared size.
//-----------------------------------------------------------------------------
size_t NData::GetSizeShared() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::Shared);



	// Get the size
	return mStorage.Shared.theSlice.GetSize();
}





//=============================================================================
//		NData::GetSizeExternal : Get the external size.
//-----------------------------------------------------------------------------
size_t NData::GetSizeExternal() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::External);



	// Get the size
	return mStorage.External.theSlice.GetSize();
}





//=============================================================================
//		NData::GetCapacitySmall : Get the small capacity.
//-----------------------------------------------------------------------------
size_t NData::GetCapacitySmall() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::Small);



	// Get the capacity
	return NDataStorage::Small;
}





//=============================================================================
//		NData::GetCapacityShared : Get the shared capacity.
//-----------------------------------------------------------------------------
size_t NData::GetCapacityShared() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::Shared);



	// Get the capacity
	return mStorage.Shared.theBlock->theSize;
}





//=============================================================================
//		NData::GetCapacityExternal : Get the external capacity.
//-----------------------------------------------------------------------------
size_t NData::GetCapacityExternal() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::External);



	// Get the capacity
	return mStorage.External.theSlice.GetSize();
}





//=============================================================================
//		NData::GetDataSmall : Get small data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataSmall() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::Small);



	// Get the data
	return mStorage.Small.theData;
}





//=============================================================================
//		NData::GetDataShared : Get shared data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataShared() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::Shared);



	// Get the data
	return mStorage.Shared.theBlock->theData;
}





//=============================================================================
//		NData::GetDataExternal : Get external data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataExternal() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::External);



	// Get the data
	return mStorage.External.theData;
}





//=============================================================================
//		NData::SetDataSmall : Set small data.
//-----------------------------------------------------------------------------
void NData::SetDataSmall(size_t theSize, const void* theData, NDataUsage theUsage)
{


	// Validate our parameters
	NN_REQUIRE(theSize <= kSizeSmallMax);
	NN_REQUIRE(theUsage != NDataUsage::View);



	// Set the data
	//
	// memmove will handle the case where the data is already within our storage.
	switch (theUsage)
	{
		case NDataUsage::Copy:
			memmove(mStorage.Small.theData, theData, theSize);
			break;

		case NDataUsage::Zero:
			memset(mStorage.Small.theData, 0x00, theSize);
			break;

		case NDataUsage::None:
			// Do nothing
			break;

		case NDataUsage::View:
			NN_LOG_ERROR("NDataUsage::View is invalid!");
			break;
	}



	// Switch to small data
	ReleaseData();

	mFlags = uint8_t(theSize);
}





//=============================================================================
//		NData::SetDataShared : Set shared data.
//-----------------------------------------------------------------------------
void NData::SetDataShared(size_t theSize, const void* theData, NDataUsage theUsage)
{


	// Validate our parameters
	NN_REQUIRE(theSize > kSizeSmallMax);
	NN_REQUIRE(theUsage != NDataUsage::View);



	// Get the state we need
	bool usingShared  = (GetStorage() == NDataStorage::Shared);
	bool withinShared = false;

	if (usingShared)
	{
		uintptr_t srcFirst = reinterpret_cast<uintptr_t>(theData);
		uintptr_t srcLast  = srcFirst + theSize - 1;

		uintptr_t sharedFirst = reinterpret_cast<uintptr_t>(mStorage.Shared.theBlock->theData);
		uintptr_t sharedLast  = sharedFirst + mStorage.Shared.theBlock->theSize - 1;

		withinShared = (srcFirst >= sharedFirst && srcFirst <= sharedLast) &&
					   (srcLast >= sharedFirst && srcLast <= sharedLast);
	}



	// Adjust our slice
	//
	// If the source data is already within our data we just need to adjust our slice.
	if (withinShared)
	{
		uintptr_t blockFirst = reinterpret_cast<uintptr_t>(mStorage.Shared.theBlock->theData);
		uintptr_t srcFirst   = reinterpret_cast<uintptr_t>(theData);

		NN_REQUIRE(srcFirst >= blockFirst);
		mStorage.Shared.theSlice.SetRange(srcFirst - blockFirst, theSize);
	}


	// Adjust our block
	//
	// If the data is external we need to copy it into our block.
	else
	{
		// Reuse the existing block
		//
		// If we're using shared data, and we're the only owner, and the new data
		// would fit then we can just copy the data into our existing block and
		// adjust our slice.
		bool reuseBlock = (usingShared && theSize <= mStorage.Shared.theBlock->theSize &&
						   mStorage.Shared.theBlock->numOwners == 1);

		if (reuseBlock)
		{
			mStorage.Shared.theSlice.SetRange(0, theSize);
			switch (theUsage)
			{
				case NDataUsage::Copy:
					memcpy(mStorage.Shared.theBlock->theData, theData, theSize);
					break;

				case NDataUsage::Zero:
					memset(mStorage.Shared.theBlock->theData, 0x00, theSize);
					break;

				case NDataUsage::None:
					// Do nothing
					break;

				case NDataUsage::View:
					NN_LOG_ERROR("NDataUsage::View is invalid!");
					break;
			}
		}


		// Create a new block
		//
		// Move the data into a new block, where we are the only owner.
		else
		{
			// Release any existing block
			//
			// This is safe to do because, if we were using shared data before, we
			// have already checked to make sure that the new data isn't in our block.
			ReleaseData();


			// Create a new block
			size_t blockSize = sizeof(NDataBlock) + theSize;

			switch (theUsage)
			{
				case NDataUsage::Copy:
					mStorage.Shared.theBlock = static_cast<NDataBlock*>(malloc(blockSize));
					memcpy(mStorage.Shared.theBlock->theData, theData, theSize);
					break;

				case NDataUsage::Zero:
					mStorage.Shared.theBlock = static_cast<NDataBlock*>(calloc(1, blockSize));
					break;

				case NDataUsage::None:
					mStorage.Shared.theBlock = static_cast<NDataBlock*>(malloc(blockSize));
					break;

				case NDataUsage::View:
					NN_LOG_ERROR("NDataUsage::View is invalid!");
					break;
			}

			mStorage.Shared.theBlock->numOwners = 1;
			mStorage.Shared.theBlock->theSize   = theSize;
			mFlags                              = kStorageShared;
		}
	}
}





//=============================================================================
//		NData::SetDataExternal : Set external data.
//-----------------------------------------------------------------------------
void NData::SetDataExternal(size_t theSize, const void* theData, NDataUsage theUsage)
{


	// Validate our parameters
	NN_REQUIRE(theUsage == NDataUsage::View);



	// Switch to external data
	ReleaseData();

	mStorage.External.theData = static_cast<const uint8_t*>(theData);
	mStorage.External.theSlice.SetRange(0, theSize);
	mFlags = kStorageExternal;
}
