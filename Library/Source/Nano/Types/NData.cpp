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
static constexpr size_t kSmallSizeMax                       = NDataStorage::Small;

static constexpr uint8_t kStorageSmall                      = 0x3F;
static constexpr uint8_t kStorageShared                     = (1 << 6);
static constexpr uint8_t kStorageExternal                   = (1 << 7);





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(size_t theSize, const void* theData, NDataUsage theUsage)
	: mStorage({})
	, mFlags(kStorageSmall)
{
	// Validate our state
	static_assert(sizeof(mStorage) == kSmallSizeMax);



	// Initialise ourselves
	SetData(theSize, theData, theUsage);
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData()
	: mStorage{}
	, mFlags(kStorageSmall)
{
	// Validate our state
	static_assert(sizeof(mStorage) == kSmallSizeMax);
}





//=============================================================================
//		NData::~NData : Destructor.
//-----------------------------------------------------------------------------
NData::~NData()
{


	// TODO
}





//=============================================================================
//		NData::NData : Constructor.
//-----------------------------------------------------------------------------
NData::NData(const NData& otherData)
	: mStorage{}
	, mFlags(kStorageSmall)
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


	// TODO
	return 0;
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

	return theData;
}





//=============================================================================
//		NData::GetMutableData : Get mutable data.
//-----------------------------------------------------------------------------
uint8_t* NData::GetMutableData(size_t theOffset)
{


	// TODO
	NN_UNUSED(theOffset);

	return nullptr;
}





//=============================================================================
//		NData::SetData : Set the data.
//-----------------------------------------------------------------------------
void NData::SetData(size_t theSize, const void* theData, NDataUsage theUsage)
{


	// TODO
	NN_UNUSED(theSize);
	NN_UNUSED(theData);
	NN_UNUSED(theUsage);
}





//=============================================================================
//		NData::InsertData : Insert data.
//-----------------------------------------------------------------------------
uint8_t* NData::InsertData(size_t beforeIndex, const NData& theData)
{


	// TODO
	NN_UNUSED(beforeIndex);
	NN_UNUSED(theData);

	return nullptr;
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


	// TODO
	NN_UNUSED(theData);

	return nullptr;
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


	// TODO
	NN_UNUSED(theRange);
}





//=============================================================================
//		NData::ReplaceData : Replace data.
//-----------------------------------------------------------------------------
uint8_t* NData::ReplaceData(const NRange& theRange, const NData& theData)
{


	// TODO
	NN_UNUSED(theRange);
	NN_UNUSED(theData);

	return nullptr;
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


	// TODO
	NN_UNUSED(theData);

	return NComparison::EqualTo;
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
//		NData::GetDataSmall : Get the small data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataSmall() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::Small);



	// Get the size
	return mStorage.Small.theData;
}





//=============================================================================
//		NData::GetDataShared : Get the shared data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataShared() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::Shared);



	// Get the size
	return mStorage.Shared.theBlock->theData;
}





//=============================================================================
//		NData::GetDataExternal : Get the external data.
//-----------------------------------------------------------------------------
const uint8_t* NData::GetDataExternal() const
{


	// Validate our state
	NN_REQUIRE(GetStorage() == NDataStorage::External);



	// Get the sizse
	return mStorage.External.theData;
}
