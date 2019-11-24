/*	NAME:
		NData.h

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
#ifndef NDATA_H
#define NDATA_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinComparable.h"
#include "NMixinContainer.h"
#include "NMixinHashable.h"
#include "NRange.h"

// System
#include <atomic>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Data source
//
// Data may be supplied to an object in several different ways:
//
//		NDataNDataSource::Copy			Copy the data
//
//		NDataNDataSource::Zero			Treat the source as zero-filled
//
//		NDataNDataSource::None			Treat the source as uninitialised
//
//		NDataNDataSource::View			Create a view onto external data
//
// The pointer for Zero and None is unused and so may be nullptr.
//
// The pointer for a View must persist beyond the lifetime of any NData
// objects viewing that data.
//
enum NDataSource
{
	Copy,
	Zero,
	None,
	View
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// NData storage
//
// Small amounts of data are stored directly within an NData object.
//
// Larger amounts are shared between objects, using copy-on-write.
struct NDataStorageSmall
{
	uint8_t sizeFlags;
	uint8_t theData[31];
};

struct NDataStorageLarge
{
	struct NDataState* theState;
	const uint8_t*     theData;
	NRange             theSlice;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NData
	: public NMixinContainer<NData>
	, public NMixinComparable<NData>
	, public NMixinHashable<NData>
{
public:
										NData(size_t theSize, const void* theData, NDataSource theSource = NDataSource::Copy);

										NData();
									   ~NData();

										NData(    const NData& otherData);
	NData&                              operator=(const NData& otherData);

										NData(    NData&& otherData);
	NData&                              operator=(NData&& otherData);


	// Clear the data
	void                                Clear();


	// Get/set the size
	//
	// Increasing the size will zero-fill any new content.
	size_t                              GetSize() const;
	void                                SetSize(size_t theSize);


	// Get/set the capacity
	//
	// The capacity is the maximum size available before reallocation.
	//
	// Reducing the capacity below the current size will also reduce
	// the current size.
	//
	// Adjusting the capacity may reallocate the underlying storage,
	// invalidating any previously returned pointers into the data.
	size_t                              GetCapacity() const;
	void                                SetCapacity(size_t theCapacity);


	// Get the data
	//
	// Immutable access is preferred. Mutable access may need to copy the data.
	//
	// Returns nullptr if the offset is outside the buffer.
	NData                               GetData(const NRange& theRange)      const;
	const uint8_t*                      GetData(       size_t theOffset = 0) const;
	uint8_t*                            GetMutableData(size_t theOffset = 0);


	// Set the data
	void                                SetData(size_t theSize, const void* theData, NDataSource theSource = NDataSource::Copy);


	// Insert data
	//
	// Inserts data immediately before the specified offset.
	//
	// NDataSource::View is not supported as a source.
	//
	// Returns a pointer to the first byte of the newly inserted data,
	// or nullptr if no data was inserted.
	uint8_t*                            InsertData(size_t      beforeIndex, const NData& theData);
	uint8_t*                            InsertData(size_t      beforeIndex,
												   size_t      theSize,
												   const void* theData,
												   NDataSource theSource = NDataSource::Copy);


	// Append data
	//
	// Appends data to the end of the buffer.
	//
	// NDataSource::View is not supported as a source.
	//
	// Returns a pointer to the first byte of the newly appended data,
	// or nullptr if no data was inserted.
	uint8_t*                            AppendData(const NData& theData);
	uint8_t*                            AppendData(size_t      theSize,
												   const void* theData,
												   NDataSource theSource = NDataSource::Copy);


	// Remove data
	//
	// Removes the specified range from the data.
	void                                RemoveData(const NRange& theRange);


	// Replace data
	//
	// Replaces the specified range within the data.
	//
	// NDataSource::View is not supported as a source.
	//
	// Returns a pointer to the first byte of the replaced data,
	// or nullptr if the replacement was a removal from the end.
	uint8_t*                            ReplaceData(const NRange& theRange, const NData& theData);
	uint8_t*                            ReplaceData(const NRange& theRange,
													size_t        theSize,
													const void*   theData,
													NDataSource   theSource = NDataSource::Copy);


	// Compare an object
	NComparison                         Compare(const NData& theData) const;


	// Operators
	const NData&                        operator+=(const NData& theValue);
	const NData                         operator+( const NData& theValue) const;


	// Calculate the hash
	size_t                              CalculateHash() const;


private:
	bool                                IsSmall() const;
	bool                                IsLarge() const;

	bool                                IsValidRange(const NRange& theRange) const;
	bool                                IsValidOffset(size_t theOffset)      const;
	bool                                IsValidSource(size_t theSize,   const void* theData, NDataSource theSource) const;

	void                                MakeMutable();
	void                                MakeLarge(size_t theCapacity, size_t theSize, const void* theData, NDataSource theSource);

	void                                RetainLarge();
	void                                ReleaseLarge();

	void                                AdoptData(const NData& otherData);
	void                                MemCopy(void* dstPtr,       const void* srcPtr, size_t theSize, NDataSource theSource);
	void*                               MemAllocate(size_t theSize, const void* existingPtr, bool zeroMem);

	size_t                              GetSizeSmall() const;
	size_t                              GetSizeLarge() const;

	void                                SetSizeSmall(size_t theSize);
	void                                SetSizeLarge(size_t theSize);

	size_t                              GetCapacitySmall() const;
	size_t                              GetCapacityLarge() const;

	void                                SetCapacitySmall(size_t theCapacity);
	void                                SetCapacityLarge(size_t theCapacity);

	const uint8_t*                      GetDataSmall(size_t theOffset) const;
	const uint8_t*                      GetDataLarge(size_t theOffset) const;

	void                                SetDataSmall(size_t theSize, const void* theData, NDataSource theSource);
	void                                SetDataLarge(size_t theSize, const void* theData, NDataSource theSource);

	void                                RemoveDataSmall(const NRange& theRange);
	void                                RemoveDataLarge(const NRange& theRange);


private:
	union
	{
		NDataStorageSmall mSmall;
		NDataStorageLarge mLarge;
	};
};



#endif // NDATA_H
