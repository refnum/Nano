/*	NAME:
		NDictionary.h

	DESCRIPTION:
		Dictionary object.

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
#ifndef NDICTIONARY_H
#define NDICTIONARY_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NAny.h"
#include "NMap.h"
#include "NMixinComparable.h"
#include "NMixinContainer.h"
#include "NString.h"

// System
#include <unordered_map>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NArray;
class NData;
class NNumber;
class NTime;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NDictionary final
	: public NMap<NString, NAny>
	, public NMixinComparable<NDictionary>
	, public NMixinContainer<NDictionary>
{
public:
	// Clear the dictionary
	void                                Clear();


	// Get the size
	size_t                              GetSize() const;


	// Is a key present?
	bool                                HasKey(const NString& theKey) const;


	// Remove a key
	void                                RemoveKey(const NString& theKey);


	// Get the keys
	//
	// Keys are returned in an undefined order.
	NVectorString                       GetKeys() const;


	// Get the values
	//
	// Values are returned in an undefined order.
	NVectorAny                          GetValues() const;


	// Invert the dictionary
	//
	// A dictionary can only be inverted if all of its values are unique,
	// non-empty, strings.
	bool                                Invert();


	// Get a value
	//
	// A value may only be fetched as a type if it is of that type.
	//
	// Returns 0/empty if there is no value for the key.
	bool                                GetBool(   const NString& theKey) const;
	uint32_t                            GetUInt32( const NString& theKey) const;
	uint64_t                            GetUInt64( const NString& theKey) const;
	int32_t                             GetInt32(  const NString& theKey) const;
	int64_t                             GetInt64(  const NString& theKey) const;
	float32_t                           GetFloat32(const NString& theKey) const;
	float64_t                           GetFloat64(const NString& theKey) const;

	NArray                              GetArray(     const NString& theKey) const;
	NData                               GetData(      const NString& theKey) const;
	NDictionary                         GetDictionary(const NString& theKey) const;
	NNumber                             GetNumber(    const NString& theKey) const;
	NString                             GetString(    const NString& theKey) const;
	NTime                               GetTime(      const NString& theKey) const;


public:
	// NMixinComparable
	bool                                CompareEqual(const NDictionary& theDictionary) const;
	NComparison                         CompareOrder(const NDictionary& theDictionary) const;


private:
	template<typename T>
	T                                   GetValue(const NString& theKey) const;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDictionary.inl"


#endif // NDICTIONARY_H
