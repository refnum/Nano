/*	NAME:
		NVariant.inl

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NDebug.h"





//=============================================================================
//		NVariant::Is : Do we contain the specified type?
//-----------------------------------------------------------------------------
template<typename... Types>
template<typename T>
bool NVariant<Types...>::Is() const
{


	// Check the type
	return std::holds_alternative<T>(*this);
}





//=============================================================================
//		NVariant::IsUInt8 : Do we contain a uint8_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsUInt8() const
{


	// Check the type
	return Is<uint8_t>();
}





//=============================================================================
//		NVariant::IsUInt16 : Do we contain a uint16_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsUInt16() const
{


	// Check the type
	return Is<uint16_t>();
}





//=============================================================================
//		NVariant::IsUInt32 : Do we contain a uint32_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsUInt32() const
{


	// Check the type
	return Is<uint32_t>();
}





//=============================================================================
//		NVariant::IsUInt64 : Do we contain a uint64_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsUInt64() const
{


	// Check the type
	return Is<uint64_t>();
}





//=============================================================================
//		NVariant::IsInt8 : Do we contain an int8_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsInt8() const
{


	// Check the type
	return Is<int8_t>();
}





//=============================================================================
//		NVariant::IsInt16 : Do we contain an int16_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsInt16() const
{


	// Check the type
	return Is<int16_t>();
}





//=============================================================================
//		NVariant::IsInt32 : Do we contain an int32_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsInt32() const
{


	// Check the type
	return Is<int32_t>();
}





//=============================================================================
//		NVariant::IsInt64 : Do we contain an int64_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsInt64() const
{


	// Check the type
	return Is<int64_t>();
}





//=============================================================================
//		NVariant::IsFloat32 : Do we contain a float32_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsFloat32() const
{


	// Check the type
	return Is<float32_t>();
}





//=============================================================================
//		NVariant::IsFloat64 : Do we contain a float64_t?
//-----------------------------------------------------------------------------
template<typename... Types>
inline bool NVariant<Types...>::IsFloat64() const
{


	// Check the type
	return Is<float64_t>();
}





//=============================================================================
//		NVariant::Get : Get the specified type.
//-----------------------------------------------------------------------------
template<typename... Types>
template<typename T>
T NVariant<Types...>::Get() const
{


	// Validate our state
	NN_REQUIRE(Is<T>());



	// Get the value
	return std::get<T>(*this);
}





//=============================================================================
//		NVariant::GetUInt8 : Get a uint8_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline uint8_t NVariant<Types...>::GetUInt8() const
{


	// Get the value
	return Get<uint8_t>();
}





//=============================================================================
//		NVariant::GetUInt16 : Get a uint16_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline uint16_t NVariant<Types...>::GetUInt16() const
{


	// Get the value
	return Get<uint16_t>();
}





//=============================================================================
//		NVariant::GetUInt32 : Get a uint32_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline uint32_t NVariant<Types...>::GetUInt32() const
{


	// Get the value
	return Get<uint32_t>();
}





//=============================================================================
//		NVariant::GetUInt64 : Get a uint64_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline uint64_t NVariant<Types...>::GetUInt64() const
{


	// Get the value
	return Get<uint64_t>();
}





//=============================================================================
//		NVariant::GetInt8 : Get an int8_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline int8_t NVariant<Types...>::GetInt8() const
{


	// Get the value
	return Get<int8_t>();
}





//=============================================================================
//		NVariant::GetInt16 : Get an int16_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline int16_t NVariant<Types...>::GetInt16() const
{


	// Get the value
	return Get<int16_t>();
}





//=============================================================================
//		NVariant::GetInt32 : Get an int32_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline int32_t NVariant<Types...>::GetInt32() const
{


	// Get the value
	return Get<int32_t>();
}





//=============================================================================
//		NVariant::GetInt64 : Get an int64_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline int64_t NVariant<Types...>::GetInt64() const
{


	// Get the value
	return Get<int64_t>();
}





//=============================================================================
//		NVariant::GetFloat32 : Get a float32_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline float32_t NVariant<Types...>::GetFloat32() const
{


	// Get the value
	return Get<float32_t>();
}





//=============================================================================
//		NVariant::GetFlota64 : Get a float64_t.
//-----------------------------------------------------------------------------
template<typename... Types>
inline float64_t NVariant<Types...>::GetFloat64() const
{


	// Get the value
	return Get<float64_t>();
}
