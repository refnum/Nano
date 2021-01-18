/*	NAME:
		NNumber.h

	DESCRIPTION:
		Number object.

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
#ifndef NNUMBER_H
#define NNUMBER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinComparable.h"
#include "NVariant.h"
#include "NanoMacros.h"

// System
#include <type_traits>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Limits
//
// The maximum integer values that can be round-tripped to a real value.
static constexpr float64_t kNIntegerSafeMin                 = -9007199254740991.0;
static constexpr float64_t kNIntegerSafeMax                 = 9007199254740991.0;





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NAny;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NNumber final : public NMixinComparable<NNumber>
{
public:
										NNumber() = default;

	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
										NNumber(T theValue);

	template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	NNumber&                            operator=(T theValue);


	// Is this an integer number?
	bool                                IsInteger() const;


	// Is this a real number?
	bool                                IsReal() const;


	// Is this a signed number?
	bool                                IsSigned() const;


	// Is this a positive / negative number?
	bool                                IsPositive() const;
	bool                                IsNegative() const;


	// Get the value
	//
	// A number can only be fetched as a particular type if the
	// value can be cast to that type without loss of information.
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


	// Set a value
	//
	// Returns true if the value was a recognised numeric type.
	bool                                SetValue(const NAny& theValue);


public:
	// NMixinComparable
	bool                                CompareEqual(const NNumber& theNumber) const;
	NComparison                         CompareOrder(const NNumber& theNumber) const;


private:
	NComparison                         CompareIntReal(const NNumber& theNumber) const;


private:
	NVariant<uint64_t, int64_t, float64_t> mValue;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NNumber.inl"



#endif // NNUMBER_H
