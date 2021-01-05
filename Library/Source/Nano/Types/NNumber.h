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

// System
#include <variant>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Value
using NNumberValue                                          = std::variant<uint64_t, int64_t, float64_t>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NNumber : public NMixinComparable<NNumber>
{
public:
	explicit                            NNumber(uint8_t  theValue);
	explicit                            NNumber(uint16_t theValue);
	explicit                            NNumber(uint32_t theValue);
	explicit                            NNumber(uint64_t theValue);

	explicit                            NNumber(int8_t  theValue);
	explicit                            NNumber(int16_t theValue);
	explicit                            NNumber(int32_t theValue);
	explicit                            NNumber(int64_t theValue);

	explicit                            NNumber(float32_t theValue);
	explicit                            NNumber(float64_t theValue);

										NNumber();


	// Is this an integer number?
	bool                                IsInteger() const;


	// Is this a real number?
	bool                                IsReal() const;


	// Is this a signed number?
	bool                                IsSigned() const;


	// Is this a positive / negative number?
	bool                                IsPositive() const;
	bool                                IsNegative() const;


	// Get / set the value
	//
	// A number may only be obtained as a specific type if it can be
	// converted to that type without loss of information.
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

	void                                SetUInt8(uint8_t     theValue);
	void                                SetUInt16(uint16_t   theValue);
	void                                SetUInt32(uint32_t   theValue);
	void                                SetUInt64(uint64_t   theValue);
	void                                SetInt8(int8_t       theValue);
	void                                SetInt16(int16_t     theValue);
	void                                SetInt32(int32_t     theValue);
	void                                SetInt64(int64_t     theValue);
	void                                SetFloat32(float32_t theValue);
	void                                SetFloat64(float64_t theValue);



public:
	// NMixinComparable
	bool                                CompareEqual(const NNumber& theNumber) const;
	NComparison                         CompareOrder(const NNumber& theNumber) const;


private:
	NComparison                         CompareIntReal(const NNumber& theNumber) const;


private:
	NNumberValue                        mValue;
};



#endif // NNUMBER_H
