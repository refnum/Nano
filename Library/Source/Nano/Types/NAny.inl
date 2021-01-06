/*	NAME:
		NAny.inl

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NDebug.h"





//=============================================================================
//		NAny::NAny : Constructor.
//-----------------------------------------------------------------------------
template<typename T>
NAny::NAny(T&& theValue)
	: any(theValue)
{
}





//=============================================================================
//		NAny::IsEmpty : Is there a value?
//-----------------------------------------------------------------------------
inline bool NAny::IsEmpty() const
{


	// Check our state
	return !has_value();
}





//=============================================================================
//		NAny::Clear : Clear the value.
//-----------------------------------------------------------------------------
inline void NAny::Clear()
{


	// Reset our state
	reset();
}





//=============================================================================
//		NAny::GetType : Get the type.
//-----------------------------------------------------------------------------
inline const std::type_info& NAny::GetType() const
{


	// Get the type
	return type();
}





//=============================================================================
//		NAny::Has : Do we have the specified type?
//-----------------------------------------------------------------------------
template<typename T>
bool NAny::Has() const
{


	// Check the type
	return type() == typeid(T);
}





//=============================================================================
//		NAny::HasBool : Do we have a bool_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasBool() const
{


	// Check the type
	return Has<bool>();
}





//=============================================================================
//		NAny::HasUInt8 : Do we have a uint8_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasUInt8() const
{


	// Check the type
	return Has<uint8_t>();
}





//=============================================================================
//		NAny::HasUInt16 : Do we have a uint16_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasUInt16() const
{


	// Check the type
	return Has<uint16_t>();
}





//=============================================================================
//		NAny::HasUInt32 : Do we have a uint32_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasUInt32() const
{


	// Check the type
	return Has<uint32_t>();
}





//=============================================================================
//		NAny::HasUInt64 : Do we have a uint64_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasUInt64() const
{


	// Check the type
	return Has<uint64_t>();
}





//=============================================================================
//		NAny::HasInt8 : Do we have a int8_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasInt8() const
{


	// Check the type
	return Has<int8_t>();
}





//=============================================================================
//		NAny::HasInt16 : Do we have a int16_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasInt16() const
{


	// Check the type
	return Has<int16_t>();
}





//=============================================================================
//		NAny::HasInt32 : Do we have a int32_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasInt32() const
{


	// Check the type
	return Has<int32_t>();
}





//=============================================================================
//		NAny::HasInt64 : Do we have a int64_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasInt64() const
{


	// Check the type
	return Has<int64_t>();
}





//=============================================================================
//		NAny::HasFloat32 : Do we have a float32_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasFloat32() const
{


	// Check the type
	return Has<float32_t>();
}





//=============================================================================
//		NAny::HasFloat64 : Do we have a float64_t?
//-----------------------------------------------------------------------------
inline bool NAny::HasFloat64() const
{


	// Check the type
	return Has<float64_t>();
}





//=============================================================================
//		NAny::Get : Get the specified type.
//-----------------------------------------------------------------------------
template<typename T>
T NAny::Get() const
{


	// Validate our state
	NN_REQUIRE(Has<T>());



	// Get the value
	return std::any_cast<T>(*this);
}





//=============================================================================
//		NAny::GetBool : Get a bool.
//-----------------------------------------------------------------------------
inline bool NAny::GetBool() const
{


	// Get the value
	return Get<bool>();
}





//=============================================================================
//		NAny::GetUInt8 : Get a uint8_t.
//-----------------------------------------------------------------------------
inline uint8_t NAny::GetUInt8() const
{


	// Get the value
	return Get<uint8_t>();
}





//=============================================================================
//		NAny::GetUInt16 : Get a uint16_t.
//-----------------------------------------------------------------------------
inline uint16_t NAny::GetUInt16() const
{


	// Get the value
	return Get<uint16_t>();
}





//=============================================================================
//		NAny::GetUInt32 : Get a uint32_t.
//-----------------------------------------------------------------------------
inline uint32_t NAny::GetUInt32() const
{


	// Get the value
	return Get<uint32_t>();
}





//=============================================================================
//		NAny::GetUInt64 : Get a uint64_t.
//-----------------------------------------------------------------------------
inline uint64_t NAny::GetUInt64() const
{


	// Get the value
	return Get<uint64_t>();
}





//=============================================================================
//		NAny::GetInt8 : Get a int8_t.
//-----------------------------------------------------------------------------
inline int8_t NAny::GetInt8() const
{


	// Get the value
	return Get<int8_t>();
}





//=============================================================================
//		NAny::GetInt16 : Get a int16_t.
//-----------------------------------------------------------------------------
inline int16_t NAny::GetInt16() const
{


	// Get the value
	return Get<int16_t>();
}





//=============================================================================
//		NAny::GetInt32 : Get a int32_t.
//-----------------------------------------------------------------------------
inline int32_t NAny::GetInt32() const
{


	// Get the value
	return Get<int32_t>();
}





//=============================================================================
//		NAny::GetInt64 : Get a int64_t.
//-----------------------------------------------------------------------------
inline int64_t NAny::GetInt64() const
{


	// Get the value
	return Get<int64_t>();
}





//=============================================================================
//		NAny::GetFloat32 : Get a float32_t.
//-----------------------------------------------------------------------------
inline float32_t NAny::GetFloat32() const
{


	// Get the value
	return Get<float32_t>();
}





//=============================================================================
//		NAny::GetFlota64 : Get a float64_t.
//-----------------------------------------------------------------------------
inline float64_t NAny::GetFloat64() const
{


	// Get the value
	return Get<float64_t>();
}
