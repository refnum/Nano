/*	NAME:
		NVariant.h

	DESCRIPTION:
		std::variant object.

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
#ifndef NVARIANT_H
#define NVARIANT_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoTypes.h"

// System
#include <variant>





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
template<typename... Types>
class NVariant final : public std::variant<Types...>
{
public:
	// std::variant
	using std::variant<Types...>::variant;
	using std::variant<Types...>::operator=;


	// Test the type
	//
	// Returns as the variant currently contains the specified type.
	//
	// As a std::variant's possible types are fixed at compile time, typed
	// methods are only available when the variant could contain that type.
	//
	// Accessing an unavailable method is a compile error.
	template<typename T>
	bool                                Is() const;

	bool                                IsUInt8()   const;
	bool                                IsUInt16()  const;
	bool                                IsUInt32()  const;
	bool                                IsUInt64()  const;
	bool                                IsInt8()    const;
	bool                                IsInt16()   const;
	bool                                IsInt32()   const;
	bool                                IsInt64()   const;
	bool                                IsFloat32() const;
	bool                                IsFloat64() const;


	// Get the value
	//
	// Returns the specified type from the variant.
	//
	// The variant must currently hold the specified type.
	//
	// As a std::variant's possible types are fixed at compile time, typed
	// methods are only available when the variant could contain that type.
	//
	// Accessing an unavailable method is a compile error.
	template<typename T>
	T                                   Get() const;

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
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NVariant.inl"



#endif // NVARIANT_H
