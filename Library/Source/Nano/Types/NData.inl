/*	NAME:
		NData.inl

	DESCRIPTION:
		Data object.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
// System
#include <cstddef>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint8_t kNDataFlagIsLarge                  = 0b10000000;
static constexpr uint8_t kNDataFlagSmallSizeMask            = 0b00011111;

static constexpr uint8_t kNDataSmallSizeMax                 = 27;





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
inline NData::NData()
	: mData{}
{
	// Validate our state
	static_assert(sizeof(NDataStorage) == 32);
	static_assert(sizeof(mData.Small.theData) == kNDataSmallSizeMax);

	static_assert(offsetof(NDataStorage, Small.theData) == 0);
	static_assert(offsetof(NDataStorage, Large.theState) == 0);
	static_assert(offsetof(NDataStorage, Large.theSlice) == 8);

	static_assert(offsetof(NDataStorage, theFlags) == 27);
	static_assert(offsetof(NDataStorage, theHash) == 28);
}





//=============================================================================
//		NData::~NData : Destructor.
//-----------------------------------------------------------------------------
inline NData::~NData()
{


	// Clean up
	if (IsLarge())
	{
		ReleaseLarge();
	}
}





//=============================================================================
//		NData::GetSize : Get the size.
//-----------------------------------------------------------------------------
inline size_t NData::GetSize() const
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
//		NData::GetCapacity : Get the capacity.
//-----------------------------------------------------------------------------
inline size_t NData::GetCapacity() const
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
//		NData::GetData : Get the data.
//-----------------------------------------------------------------------------
inline const uint8_t* NData::GetData(size_t theOffset) const
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





#pragma mark private
//=============================================================================
//		NData::IsSmall : Are we using small storage?
//-----------------------------------------------------------------------------
inline bool NData::IsSmall() const
{


	// Check the flag
	return !IsLarge();
}





//=============================================================================
//		NData::IsLarge : Are we using large storage?
//-----------------------------------------------------------------------------
inline bool NData::IsLarge() const
{


	// Check the flag
	return (mData.theFlags & kNDataFlagIsLarge) != 0;
}





//=============================================================================
//		NData::GetSizeSmall : Get the small size.
//-----------------------------------------------------------------------------
inline size_t NData::GetSizeSmall() const
{


	// Validate our state
	NN_REQUIRE(IsSmall());



	// Get the size
	//
	// The small size is stored in the flag byte.
	return size_t(mData.theFlags & kNDataFlagSmallSizeMask);
}





//=============================================================================
//		NData::GetSizeLarge : Get the large size.
//-----------------------------------------------------------------------------
inline size_t NData::GetSizeLarge() const
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the size
	return mData.Large.theSlice.GetSize();
}





//=============================================================================
//		NData::GetCapacitySmall : Get the small capacity.
//-----------------------------------------------------------------------------
inline size_t NData::GetCapacitySmall() const
{


	// Validate our state
	NN_REQUIRE(IsSmall());



	// Get the capacity
	return kNDataSmallSizeMax;
}





//=============================================================================
//		NData::GetCapacityLarge : Get the large capacity.
//-----------------------------------------------------------------------------
inline size_t NData::GetCapacityLarge() const
{


	// Validate our state
	NN_REQUIRE(IsLarge());



	// Get the capacity
	return mData.Large.theState->theSize;
}





//=============================================================================
//		NData::GetDataSmall : Get small data.
//-----------------------------------------------------------------------------
inline const uint8_t* NData::GetDataSmall(size_t theOffset) const
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
inline const uint8_t* NData::GetDataLarge(size_t theOffset) const
{


	// Validate our state
	NN_REQUIRE(IsLarge());
	NN_REQUIRE(theOffset < mData.Large.theSlice.GetSize());



	// Get the data
	return &mData.Large.theState->theData[mData.Large.theSlice.GetOffset(theOffset)];
}
