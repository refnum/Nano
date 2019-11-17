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
#include <stdatomic.h>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Data storage
//
// Small amounts of data are stored directly in the object.
//
// Larger data may be shared between objects, or managed externally.
//
enum NDataStorage
{
	Small                                                   = 32,
	Shared,
	External
};


// Data usage
//
// Data supplied to an object may be treated in several different ways:
//
//		NDataUsage::Copy			Copy the data
//
//		NDataUsage::Zero			Treat the data as zero-filled
//
//		NDataUsage::None			Treat the data as uninitialised
//
//		NDataUsage::View			Create a view onto external data.
//
// The pointer for Zero and None usage may be nullptr, as it is never read.
//
// The pointer for View usage must persist beyond the lifetime of any objects
// that are viewing that data.
//
enum NDataUsage
{
	Copy,
	Zero,
	None,
	View
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Data block
//
// A data block is a potentially shared block of data.
//
struct NDataBlock
{
	atomic_size_t numOwners;
	size_t        theSize;
	uint8_t       theData[];
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
										NData(size_t theSize, const void* theData, NDataUsage theUsage = NDataUsage::Copy);

										NData();
	virtual                            ~NData();

										NData(    const NData&);
	NData&                              operator=(const NData&);

										NData(    NData&&);
	NData&                              operator=(NData&&);


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
	// Increasing the capacity may reallocate the underlying storage,
	// invalidating any previously returned pointers into the data.
	size_t                              GetCapacity() const;
	void                                SetCapacity(size_t theSize);


	// Get the data
	//
	// Immutable access is preferred. Mutable access may need to copy the data.
	//
	// Returns nullptr if the data is empty.
	const uint8_t*                      GetData(       size_t theOffset = 0) const;
	uint8_t*                            GetMutableData(size_t theOffset = 0);


	// Set the data
	void                                SetData(size_t theSize, const void* theData, NDataUsage theUsage = NDataUsage::Copy);


	// Insert data
	//
	// Inserts data immediately before the specified offset.
	//
	// NDataUsage::View is not supported as a data source.
	//
	// Returns a pointer to the first byte of the newly inserted data,
	// or nullptr if no data was inserted.
	uint8_t*                            InsertData(size_t      beforeIndex, const NData& theData);
	uint8_t*                            InsertData(size_t      beforeIndex,
												   size_t      theSize,
												   const void* theData,
												   NDataUsage  theUsage = NDataUsage::Copy);


	// Append data
	//
	// Appends data to the end of the buffer.
	//
	// NDataUsage::View is not supported as a data source.
	//
	// Returns a pointer to the first byte of the newly appended data,
	// or nullptr if no data was inserted.
	uint8_t*                            AppendData(const NData& theData);
	uint8_t*                            AppendData(size_t      theSize,
												   const void* theData,
												   NDataUsage  theUsage = NDataUsage::Copy);


	// Remove data
	//
	// Removes the specified range from the data.
	void                                RemoveData(const NRange& theRange);


	// Replace data
	//
	// Replaces the specified range within the data.
	//
	// NDataUsage::View is not supported as a data source.
	//
	// Returns a pointer to the first byte of the replaced data,
	// or nullptr if the replacement was a removal from the end.
	uint8_t*                            ReplaceData(const NRange& theRange, const NData& theData);
	uint8_t*                            ReplaceData(const NRange& theRange,
													size_t        theSize,
													const void*   theData,
													NDataUsage    theUsage = NDataUsage::Copy);


	// Compare an object
	NComparison                         Compare(const NData& theData) const;


	// Operators
	const NData&                        operator+=(const NData& theValue);
	const NData                         operator+( const NData& theValue) const;


	// Calculate the hash
	size_t                              CalculateHash() const;


private:
	NDataStorage                        GetStorage() const;
	void                                MakeMutable();

	void                                AcquireData(const NData& otherData);

	size_t                              GetSizeSmall()    const;
	size_t                              GetSizeShared()   const;
	size_t                              GetSizeExternal() const;

	size_t                              GetCapacitySmall()    const;
	size_t                              GetCapacityShared()   const;
	size_t                              GetCapacityExternal() const;

	const uint8_t*                      GetDataSmall()    const;
	const uint8_t*                      GetDataShared()   const;
	const uint8_t*                      GetDataExternal() const;

	uint8_t*                            GetMutableSmall();
	uint8_t*                            GetMutableShared();
	uint8_t*                            GetMutableExternal();

	void                                SetDataSmall(   size_t theSize, const void* theData, NDataUsage theUsage);
	void                                SetDataShared(  size_t theSize, const void* theData, NDataUsage theUsage);
	void                                SetDataExternal(size_t theSize, const void* theData, NDataUsage theUsage);

	NDataBlock*                         CreateBlock(size_t theSize, const void* theData, NDataUsage theUsage);
	void                                AcquireBlock(NDataBlock* theBlock);
	void                                ReleaseBlock(NDataBlock* theBlock);


private:
	union
	{
		struct
		{
			uint8_t theData[NDataStorage::Small];
		} Small;

		struct
		{
			NDataBlock* theBlock;
			NRange      theSlice;
		} Shared;

		struct
		{
			const uint8_t* theData;
			NRange         theSlice;
		} External;
	} mStorage;

	uint8_t                             mFlags;
};



#endif // NDATA_H
