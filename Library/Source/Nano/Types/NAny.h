/*	NAME:
		NAny.h

	DESCRIPTION:
		std::any object.

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
#ifndef NANY_H
#define NANY_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinComparable.h"
#include "NanoMacros.h"

// System
#include <any>
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NAny;
class NDictionary;
class NArray;
class NNumber;
class NData;
class NString;
class NTime;

// Containers
using NVectorAny = std::vector<NAny>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NAny final
	: public std::any
	, public NMixinComparable<NAny>
{
public:
										NAny(const NAny& theValue);

										NAny(NAny&& theValue);

	template<typename T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, NAny>>>
										NAny(T&& theValue);

	NAny&                               operator=(const NAny&  theValue);
	NAny&                               operator=(      NAny&& theValue);

	template<typename T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, NAny>>>
	NAny&                               operator=(T&& theValue);

										NAny()  = default;
									   ~NAny()  = default;


	// Is there a value?
	bool                                IsEmpty() const;


	// Clear the value
	void                                Clear();


	// Get the type
	const std::type_info&               GetType() const;


	// Test the type
	//
	// Returns false if the value is not of the specified type,
	// or there is no value.
	template<typename T>
	bool                                Is() const;

	bool                                IsBool()    const;
	bool                                IsUInt32()  const;
	bool                                IsUInt64()  const;
	bool                                IsInt32()   const;
	bool                                IsInt64()   const;
	bool                                IsFloat32() const;
	bool                                IsFloat64() const;

	bool                                IsArray()      const;
	bool                                IsData()       const;
	bool                                IsDictionary() const;
	bool                                IsNumber()     const;
	bool                                IsString()     const;
	bool                                IsTime()       const;


	// Get the value
	//
	// A value may only be fetched as a type if it is of that type,
	// or there is no value.
	//
	// Returns 0/empty if there is no value.
	template<typename T>
	const T&                            Get() const;

	bool                                GetBool()    const;
	uint32_t                            GetUInt32()  const;
	uint64_t                            GetUInt64()  const;
	int32_t                             GetInt32()   const;
	int64_t                             GetInt64()   const;
	float32_t                           GetFloat32() const;
	float64_t                           GetFloat64() const;

	NArray                              GetArray()      const;
	NData                               GetData()       const;
	NDictionary                         GetDictionary() const;
	NNumber                             GetNumber()     const;
	NString                             GetString()     const;
	NTime                               GetTime()       const;


public:
	// NMixinComparable
	bool                                CompareEqual(const NAny& theValue) const;
	NComparison                         CompareOrder(const NAny& theValue) const;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NAny.inl"



#endif // NANY_H
