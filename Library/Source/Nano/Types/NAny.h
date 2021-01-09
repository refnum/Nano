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
#include "NData.h"
#include "NMixinComparable.h"
#include "NString.h"
#include "NTime.h"

// System
#include <any>





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NAny final
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
	bool                                Has() const;

	bool                                HasBool()    const;
	bool                                HasUInt8()   const;
	bool                                HasUInt16()  const;
	bool                                HasUInt32()  const;
	bool                                HasUInt64()  const;
	bool                                HasInt8()    const;
	bool                                HasInt16()   const;
	bool                                HasInt32()   const;
	bool                                HasInt64()   const;
	bool                                HasFloat32() const;
	bool                                HasFloat64() const;

	bool                                HasData()   const;
	bool                                HasString() const;
	bool                                HasTime()   const;


	// Get the value
	//
	// The value must be known to be of the specified type.
	template<typename T>
	T                                   Get() const;

	bool                                GetBool()    const;
	uint8_t                             GetUInt8()   const;
	uint16_t                            GetUInt16()  const;
	uint32_t                            GetUInt32()  const;
	uint64_t                            GetUInt64()  const;
	int8_t                              GetInt8()    const;
	int16_t                             GetInt16()   const;
	int32_t                             GetInt32()   const;
	int64_t                             GetInt64()   const;
	float32_t                           GetFloat32() const;
	float64_t                           GetFloat64() const;

	NData                               GetData()   const;
	NString                             GetString() const;
	NTime                               GetTime()   const;


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
