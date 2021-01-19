/*	NAME:
		NArray.h

	DESCRIPTION:
		Array object.

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
#ifndef NARRAY_H
#define NARRAY_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NAny.h"
#include "NMixinComparable.h"
#include "NMixinContainer.h"


// System
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NData;
class NDictionary;
class NRange;
class NString;
class NTime;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NArray final
	: public std::vector<NAny>
	, public NMixinComparable<NArray>
	, public NMixinContainer<NArray>
{
public:
	template<typename T>
										NArray(const std::vector<T>& theValues);

	template<typename T>
										NArray(std::initializer_list<T> theValues);

										NArray() = default;


	// Clear the array
	void                                Clear();


	// Get the size
	size_t                              GetSize() const;


	// Is a value present?
	bool                                HasValue(const NAny& theValue) const;


	// Remove a value
	void                                RemoveValue(size_t theIndex);
	void                                RemoveValues(const NRange& theRange);


	// Get a value
	//
	// Returns 0/empty if the value can not be returned as the specified type.
	bool                                GetBool(   size_t theIndex) const;
	uint32_t                            GetUInt32( size_t theIndex) const;
	uint64_t                            GetUInt64( size_t theIndex) const;
	int32_t                             GetInt32(  size_t theIndex) const;
	int64_t                             GetInt64(  size_t theIndex) const;
	float32_t                           GetFloat32(size_t theIndex) const;
	float64_t                           GetFloat64(size_t theIndex) const;

	NArray                              GetArray(     size_t theIndex) const;
	NData                               GetData(      size_t theIndex) const;
	NDictionary                         GetDictionary(size_t theIndex) const;
	NString                             GetString(    size_t theIndex) const;
	NTime                               GetTime(      size_t theIndex) const;


	// Get / set a vector
	//
	// The array must only contain values of the specified type.
	template<typename T>
	std::vector<T>                      GetVector() const;

	template<typename T>
	void                                SetVector(const std::vector<T>& theValues);


public:
	// NMixinComparable
	bool                                CompareEqual(const NArray& theArray) const;
	NComparison                         CompareOrder(const NArray& theArray) const;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NArray.inl"


#endif // NARRAY_H
