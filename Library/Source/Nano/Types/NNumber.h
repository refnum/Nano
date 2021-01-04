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
#include "NMixinComparable.h"
#include "NString.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Precision
enum class NPrecision
{
	None,
	UInt8,
	UInt16,
	UInt32,
	UInt64,
	Int8,
	Int16,
	Int32,
	Int64,
	Float32,
	Float64,
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Values
union NNumberValue
{
	uint64_t  uint64;
	int64_t   int64;
	float64_t float64;
};





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


	// Test the number
	bool                                IsValid()    const;
	bool                                IsInteger()  const;
	bool                                IsReal()     const;
	bool                                IsSigned()   const;
	bool                                IsNegative() const;


	// Get the precision
	//
	// Numbers may be stored with a greater precision than an assigned
	// value, however the internal precision will never be lossy.
	NPrecision                          GetPrecision() const;


	// Get the number as a string
	//
	// May return kNStringNaN or kNStringInfinityXXX.
	NString                             GetString() const;


	// Compare the value
	NComparison                         Compare(const NNumber& theValue) const;


	// Get/set the value
	//
	// If the value needs to be cast from a different precision, conversion
	// is performed using C's normal (potentially lossy) promotion rules.
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


	// Set a value
	/*
   bool                                SetValue(const NVariant &theValue);
   bool                                SetValue(const NString  &theValue);
 */


public:
	// NMixinComparable
	bool                                CompareEqual(const NNumber& theNumber) const;
	NComparison                         CompareOrder(const NNumber& theNumber) const;


private:
	NPrecision                          mPrecision;
	NNumberValue                        mValue;
};



#endif // NNUMBER_H
