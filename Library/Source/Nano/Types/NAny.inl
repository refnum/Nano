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
inline NAny::NAny(const NAny& theValue)
	: any(*(static_cast<const std::any*>(&theValue)))
{
}





//=============================================================================
//		NAny::NAny : Constructor.
//-----------------------------------------------------------------------------
inline NAny::NAny(NAny&& theValue)
	: any(*(static_cast<const std::any*>(&theValue)))
{
}





//=============================================================================
//		NAny::NAny : Constructor.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
NAny::NAny(T&& theValue)
	: any(theValue)
{
}





//=============================================================================
//		NAny::operator= : Assignment operator.
//-----------------------------------------------------------------------------
inline NAny& NAny::operator=(const NAny& theValue)
{
	any::operator=(*(static_cast<const std::any*>(&theValue)));
	return *this;
}





//=============================================================================
//		NAny::operator= : Assignment operator.
//-----------------------------------------------------------------------------
inline NAny& NAny::operator=(NAny&& theValue)
{
	any::operator=(*(static_cast<const std::any*>(&theValue)));
	return *this;
}





//=============================================================================
//		NAny::operator= : Assignment operator.
//-----------------------------------------------------------------------------
template<typename T, typename Enabled>
NAny& NAny::operator=(T&& theValue)
{
	any::operator=(theValue);
	return *this;
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
//		NAny::Is : Is the value the specified type?
//-----------------------------------------------------------------------------
template<typename T>
bool NAny::Is() const
{


	// Check the type
	return type() == typeid(T);
}





//=============================================================================
//		NAny::IsBool : Is the value a bool_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsBool() const
{


	// Check the type
	return Is<bool>();
}





//=============================================================================
//		NAny::IsUInt8 : Is the value a uint8_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsUInt8() const
{


	// Check the type
	return Is<uint8_t>();
}





//=============================================================================
//		NAny::IsUInt16 : Is the value a uint16_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsUInt16() const
{


	// Check the type
	return Is<uint16_t>();
}





//=============================================================================
//		NAny::IsUInt32 : Is the value a uint32_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsUInt32() const
{


	// Check the type
	return Is<uint32_t>();
}





//=============================================================================
//		NAny::IsUInt64 : Is the value a uint64_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsUInt64() const
{


	// Check the type
	return Is<uint64_t>();
}





//=============================================================================
//		NAny::IsInt8 : Is the value a int8_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsInt8() const
{


	// Check the type
	return Is<int8_t>();
}





//=============================================================================
//		NAny::IsInt16 : Is the value a int16_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsInt16() const
{


	// Check the type
	return Is<int16_t>();
}





//=============================================================================
//		NAny::IsInt32 : Is the value a int32_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsInt32() const
{


	// Check the type
	return Is<int32_t>();
}





//=============================================================================
//		NAny::IsInt64 : Is the value a int64_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsInt64() const
{


	// Check the type
	return Is<int64_t>();
}





//=============================================================================
//		NAny::IsFloat32 : Is the value a float32_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsFloat32() const
{


	// Check the type
	return Is<float32_t>();
}





//=============================================================================
//		NAny::IsFloat64 : Is the value a float64_t?
//-----------------------------------------------------------------------------
inline bool NAny::IsFloat64() const
{


	// Check the type
	return Is<float64_t>();
}





//=============================================================================
//		NAny::IsData : Is the value an NData?
//-----------------------------------------------------------------------------
inline bool NAny::IsData() const
{


	// Check the type
	return Is<NData>();
}





//=============================================================================
//		NAny::IsString : Is the value an NString?
//-----------------------------------------------------------------------------
inline bool NAny::IsString() const
{


	// Check the type
	return Is<NString>();
}





//=============================================================================
//		NAny::IsTime : Is the value an NTime?
//-----------------------------------------------------------------------------
inline bool NAny::IsTime() const
{


	// Check the type
	return Is<NTime>();
}





//=============================================================================
//		NAny::Get : Get the specified type.
//-----------------------------------------------------------------------------
template<typename T>
const T& NAny::Get() const
{


	// Validate our state
	NN_REQUIRE(Is<T>());



	// Get the value
	return *(std::any_cast<T>(this));
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
//		NAny::GetFloat64 : Get a float64_t.
//-----------------------------------------------------------------------------
inline float64_t NAny::GetFloat64() const
{


	// Get the value
	return Get<float64_t>();
}





//=============================================================================
//		NAny::GetData : Get an NData.
//-----------------------------------------------------------------------------
inline NData NAny::GetData() const
{


	// Get the value
	return Get<NData>();
}





//=============================================================================
//		NAny::GetString : Get an NString.
//-----------------------------------------------------------------------------
inline NString NAny::GetString() const
{


	// Get the value
	return Get<NString>();
}





//=============================================================================
//		NAny::GetTime : Get an NTime.
//-----------------------------------------------------------------------------
inline NTime NAny::GetTime() const
{


	// Get the value
	return Get<NTime>();
}
