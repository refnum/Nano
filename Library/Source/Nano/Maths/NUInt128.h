/*	NAME:
		NUInt128.h

	DESCRIPTION:
		uint128_t type.

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
#ifndef NUINT128_H
#define NUINT128_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMathUtils.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
struct NDivision128;


// uint128_t
//
// The uint128_t type is either a native compiler-supported type,
// or an alias for NUInt128.
//
//		using uint128_t = <<< defined by NUInt128.inl >>>;
//
// NN_UINT128_IS_NATIVE is reset to 1 if uint128_t is a native compiler type.
#define NN_UINT128_IS_NATIVE                                0





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NUInt128
{
public:
	constexpr                           NUInt128(uint64_t valueHi, uint64_t valueLo);

	constexpr                           NUInt128();

	constexpr                           NUInt128( const NUInt128& theValue);
	constexpr NUInt128&                 operator=(const NUInt128& theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr                           NUInt128(T theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator=(T theValue);


	// Arithmetic
	constexpr NUInt128&                 operator++();
	constexpr NUInt128                  operator++(int);

	constexpr NUInt128&                 operator--();
	constexpr NUInt128                  operator--(int);

	constexpr NUInt128                  operator+(const NUInt128& theValue) const;
	constexpr NUInt128                  operator-(const NUInt128& theValue) const;
	constexpr NUInt128                  operator*(const NUInt128& theValue) const;
	constexpr NUInt128                  operator/(const NUInt128& theValue) const;
	constexpr NUInt128                  operator%(const NUInt128& theValue) const;

	constexpr NUInt128&                 operator+=(const NUInt128& theValue);
	constexpr NUInt128&                 operator-=(const NUInt128& theValue);
	constexpr NUInt128&                 operator*=(const NUInt128& theValue);
	constexpr NUInt128&                 operator/=(const NUInt128& theValue);
	constexpr NUInt128&                 operator%=(const NUInt128& theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator+(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator-(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator*(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator/(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator%(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator+=(T theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator-=(T theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator*=(T theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator/=(T theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator%=(T theValue);


	// Bitwise
	constexpr NUInt128                  operator~() const;

	constexpr NUInt128                  operator&( const NUInt128& theValue) const;
	constexpr NUInt128                  operator|( const NUInt128& theValue) const;
	constexpr NUInt128                  operator^( const NUInt128& theValue) const;
	constexpr NUInt128                  operator<<(const NUInt128& theValue) const;
	constexpr NUInt128                  operator>>(const NUInt128& theValue) const;

	constexpr NUInt128&                 operator&=( const NUInt128& theValue);
	constexpr NUInt128&                 operator|=( const NUInt128& theValue);
	constexpr NUInt128&                 operator^=( const NUInt128& theValue);
	constexpr NUInt128&                 operator<<=(const NUInt128& theValue);
	constexpr NUInt128&                 operator>>=(const NUInt128& theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator&(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator|(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator^(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator<<(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128                  operator>>(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator&=(T theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator|=(T theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator^=(T theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator<<=(T theValue);

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr NUInt128&                 operator>>=(T theValue);


	// Logical
	constexpr bool                      operator!() const;
	constexpr bool                      operator&&( const NUInt128& theValue) const;
	constexpr bool                      operator||( const NUInt128& theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr bool                      operator&&(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr bool                      operator||(T theValue) const;


	// Comparison
	constexpr bool                      operator<( const NUInt128& theValue) const;
	constexpr bool                      operator>( const NUInt128& theValue) const;
	constexpr bool                      operator<=(const NUInt128& theValue) const;
	constexpr bool                      operator>=(const NUInt128& theValue) const;
	constexpr bool                      operator==(const NUInt128& theValue) const;
	constexpr bool                      operator!=(const NUInt128& theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr bool                      operator<(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr bool                      operator>(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr bool                      operator<=(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr bool                      operator>=(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr bool                      operator==(T theValue) const;

	template<typename T,                NN_ENABLE_IF_UNSIGNED_INTEGER(T)>
	constexpr bool                      operator!=(T theValue) const;


	// Cast
	constexpr explicit                  operator bool()     const;
	constexpr explicit                  operator uint8_t()  const;
	constexpr explicit                  operator uint16_t() const;
	constexpr explicit                  operator uint32_t() const;
	constexpr explicit                  operator uint64_t() const;


	// Accessors
	constexpr uint64_t                  GetHi() const;
	constexpr uint64_t                  GetLo() const;


private:
	constexpr size_t                    CountLeadingZeros() const;

	constexpr NUInt128                  Multiply64x64(uint64_t          valueA, uint64_t        valueB) const;
	constexpr NDivision128              DivideModulo128(const NUInt128& valueA, const NUInt128& valueB) const;


private:
#if NN_ENDIAN_LITTLE
	uint64_t                            mLo;
	uint64_t                            mHi;
#else
	uint64_t                            mHi;
	uint64_t                            mLo;
#endif
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NUInt128.inl"





//=============================================================================
//		Public Functions
//-----------------------------------------------------------------------------
// Construct a uint128_t
constexpr uint128_t NMakeUInt128(uint64_t valueHi, uint64_t valueLo);



#endif // NUINT128_H
