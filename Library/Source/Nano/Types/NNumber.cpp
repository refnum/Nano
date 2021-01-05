/*	NAME:
		NNumber.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NNumber.h"

// Nano
#include "NDebug.h"
#include "NanoConstants.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Limits
static constexpr float64_t kNSafeIntegerMin                 = -9007199254740991.0;
static constexpr float64_t kNSafeIntegerMax                 = 9007199254740991.0;





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(uint8_t theValue)
	: mValue(uint64_t(theValue))
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(uint16_t theValue)
	: mValue(uint64_t(theValue))
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(uint32_t theValue)
	: mValue(uint64_t(theValue))
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(uint64_t theValue)
	: mValue(theValue)
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(int8_t theValue)
	: mValue(int64_t(theValue))
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(int16_t theValue)
	: mValue(int64_t(theValue))
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(int32_t theValue)
	: mValue(int64_t(theValue))
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(int64_t theValue)
	: mValue(theValue)
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(float32_t theValue)
	: mValue(float64_t(theValue))
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(float64_t theValue)
	: mValue(theValue)
{
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber()
	: mValue()
{
}





//=============================================================================
//		NNumber::IsInteger : Is the number an integer?
//-----------------------------------------------------------------------------
bool NNumber::IsInteger() const
{


	// Check the value
	bool isInteger = true;

	if (mValue.IsFloat64())
	{
		float64_t theValue = GetFloat64();
		isInteger          = (fmod(theValue, 1.0) == 0.0);
	}

	return isInteger;
}





//=============================================================================
//		NNumber::IsReal : Is the number a real number?
//-----------------------------------------------------------------------------
bool NNumber::IsReal() const
{


	// Check the value
	return mValue.IsFloat64();
}





//=============================================================================
//		NNumber::IsSigned : Is the number a signed number?
//-----------------------------------------------------------------------------
bool NNumber::IsSigned() const
{


	// Check the value
	return mValue.IsInt64() || mValue.IsFloat64();
}





//=============================================================================
//		NNumber::IsPositive : Is the number positive?
//-----------------------------------------------------------------------------
bool NNumber::IsPositive() const
{


	// Check the value
	return !IsNegative();
}





//=============================================================================
//		NNumber::IsNegative : Is the number negative?
//-----------------------------------------------------------------------------
bool NNumber::IsNegative() const
{


	// Check the value
	bool isNegative = false;

	if (mValue.IsInt64())
	{
		isNegative = (mValue.GetInt64() < 0);
	}
	else if (mValue.IsFloat64())
	{
		isNegative = (mValue.GetFloat64() < 0);
	}

	return isNegative;
}





//=============================================================================
//		NNumber::GetUInt8 : Get a uint8_t value.
//-----------------------------------------------------------------------------
uint8_t NNumber::GetUInt8() const
{


	// Get the value
	uint64_t theValue = GetUInt64();
	NN_REQUIRE(theValue <= kNUInt8Max);

	return uint8_t(theValue);
}





//=============================================================================
//		NNumber::GetUInt16 : Get a uint16_t value.
//-----------------------------------------------------------------------------
uint16_t NNumber::GetUInt16() const
{


	// Get the value
	uint64_t theValue = GetUInt64();
	NN_REQUIRE(theValue <= kNUInt16Max);

	return uint16_t(theValue);
}





//=============================================================================
//		NNumber::GetUInt32 : Get a uint32_t value.
//-----------------------------------------------------------------------------
uint32_t NNumber::GetUInt32() const
{


	// Get the value
	uint64_t theValue = GetUInt64();
	NN_REQUIRE(theValue <= kNUInt32Max);

	return uint32_t(theValue);
}





//=============================================================================
//		NNumber::GetUInt64 : Get a uint64_t value.
//-----------------------------------------------------------------------------
uint64_t NNumber::GetUInt64() const
{


	// Validate our state
	NN_REQUIRE(IsPositive());


	// Get the value
	uint64_t theValue = 0;

	if (mValue.IsUInt64())
	{
		theValue = mValue.GetUInt64();
	}
	else if (mValue.IsInt64())
	{
		theValue = uint64_t(mValue.GetInt64());
	}
	else if (mValue.IsFloat64())
	{
		theValue = uint64_t(mValue.GetFloat64());
	}
	else
	{
		NN_LOG_UNIMPLEMENTED();
	}

	return theValue;
}





//=============================================================================
//		NNumber::GetInt8 : Get an int8_t value.
//-----------------------------------------------------------------------------
int8_t NNumber::GetInt8() const
{


	// Get the value
	int64_t theValue = GetInt64();
	NN_REQUIRE(theValue >= kNInt8Min && theValue <= kNInt8Max);

	return int8_t(theValue);
}





//=============================================================================
//		NNumber::GetInt16 : Get an int16_t value.
//-----------------------------------------------------------------------------
int16_t NNumber::GetInt16() const
{


	// Get the value
	int64_t theValue = GetInt64();
	NN_REQUIRE(theValue >= kNInt16Min && theValue <= kNInt16Max);

	return int16_t(theValue);
}





//=============================================================================
//		NNumber::GetInt32 : Get an int32_t value.
//-----------------------------------------------------------------------------
int32_t NNumber::GetInt32() const
{


	// Get the value
	int64_t theValue = GetInt64();
	NN_REQUIRE(theValue >= kNInt32Min && theValue <= kNInt32Max);

	return int32_t(theValue);
}





//=============================================================================
//		NNumber::GetInt64 : Get an int64_t value.
//-----------------------------------------------------------------------------
int64_t NNumber::GetInt64() const
{


	// Get the value
	int64_t theValue = 0;

	if (mValue.IsUInt64())
	{
		theValue = int64_t(mValue.GetUInt64());
	}
	else if (mValue.IsInt64())
	{
		theValue = mValue.GetInt64();
	}
	else if (mValue.IsFloat64())
	{
		theValue = int64_t(mValue.GetFloat64());
	}
	else
	{
		NN_LOG_UNIMPLEMENTED();
	}

	return theValue;
}





//=============================================================================
//		NNumber::GetFloat32 : Get a float32_t value.
//-----------------------------------------------------------------------------
float32_t NNumber::GetFloat32() const
{


	// Get the value
	float64_t theValue = GetFloat64();
	NN_REQUIRE(theValue >= float64_t(kNFloat32Min) && theValue <= float64_t(kNFloat32Max));

	return float32_t(theValue);
}





//=============================================================================
//		NNumber::GetFloat64 : Get a float64_t value.
//-----------------------------------------------------------------------------
float64_t NNumber::GetFloat64() const
{


	// Get the value
	float64_t theValue = 0.0;

	if (mValue.IsUInt64())
	{
		theValue = float64_t(mValue.GetUInt64());
	}
	else if (mValue.IsInt64())
	{
		theValue = float64_t(mValue.GetInt64());
	}
	else if (mValue.IsFloat64())
	{
		theValue = mValue.GetFloat64();
	}
	else
	{
		NN_LOG_UNIMPLEMENTED();
	}

	return theValue;
}





//=============================================================================
//		NNumber::SetUInt8 : Set a uint8_t value.
//-----------------------------------------------------------------------------
void NNumber::SetUInt8(uint8_t theValue)
{


	// Set the value
	mValue = uint64_t(theValue);
}





//=============================================================================
//		NNumber::SetUInt16 : Set a uint16_t value.
//-----------------------------------------------------------------------------
void NNumber::SetUInt16(uint16_t theValue)
{


	// Set the value
	mValue = uint64_t(theValue);
}





//=============================================================================
//		NNumber::SetUInt32 : Set a uint32_t value.
//-----------------------------------------------------------------------------
void NNumber::SetUInt32(uint32_t theValue)
{


	// Set the value
	mValue = uint64_t(theValue);
}





//=============================================================================
//		NNumber::SetUInt64 : Set a uint64_t value.
//-----------------------------------------------------------------------------
void NNumber::SetUInt64(uint64_t theValue)
{


	// Set the value
	mValue = theValue;
}





//=============================================================================
//		NNumber::SetInt8 : Set a int8_t value.
//-----------------------------------------------------------------------------
void NNumber::SetInt8(int8_t theValue)
{


	// Set the value
	mValue = int64_t(theValue);
}





//=============================================================================
//		NNumber::SetInt16 : Set a int16_t value.
//-----------------------------------------------------------------------------
void NNumber::SetInt16(int16_t theValue)
{


	// Set the value
	mValue = int64_t(theValue);
}





//=============================================================================
//		NNumber::SetInt32 : Set a int32_t value.
//-----------------------------------------------------------------------------
void NNumber::SetInt32(int32_t theValue)
{


	// Set the value
	mValue = int64_t(theValue);
}





//=============================================================================
//		NNumber::SetInt64 : Set a int64_t value.
//-----------------------------------------------------------------------------
void NNumber::SetInt64(int64_t theValue)
{


	// Set the value
	mValue = theValue;
}





//=============================================================================
//		NNumber::SetFloat32 : Set a float32_t value.
//-----------------------------------------------------------------------------
void NNumber::SetFloat32(float32_t theValue)
{


	// Set the value
	mValue = float64_t(theValue);
}





//=============================================================================
//		NNumber::SetFloat64 : Set a float64_t value.
//-----------------------------------------------------------------------------
void NNumber::SetFloat64(float64_t theValue)
{


	// Set the value
	mValue = theValue;
}





#pragma mark NMixinComparable
//=============================================================================
//		NNumber::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NNumber::CompareEqual(const NNumber& theNumber) const
{


	// Compare the number
	return CompareOrder(theNumber) == NComparison::EqualTo;
}





//=============================================================================
//		NNumber::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NNumber::CompareOrder(const NNumber& theNumber) const
{


	// Compare by sign
	//
	// Negative numbers are ordered before positive numbers.
	NComparison theResult = NComparison::EqualTo;

	if (IsNegative() && !theNumber.IsNegative())
	{
		theResult = NComparison::LessThan;
	}
	else if (!IsNegative() && theNumber.IsNegative())
	{
		theResult = NComparison::GreaterThan;
	}


	// Compare integer / integer
	else if (IsInteger() && theNumber.IsInteger())
	{
		if (IsNegative())
		{
			theResult = NCompare(GetInt64(), theNumber.GetInt64());
		}
		else
		{
			theResult = NCompare(GetUInt64(), theNumber.GetUInt64());
		}
	}


	// Compare real / real
	else if (IsReal() && theNumber.IsReal())
	{
		theResult = NCompare(GetFloat64(), theNumber.GetFloat64());
	}


	// Compare integer / real
	else
	{
		if (IsInteger())
		{
			theResult = CompareIntReal(theNumber);
		}
		else
		{
			theResult = theNumber.CompareIntReal(*this);
		}
	}

	return theResult;
}





#pragma mark private
//=============================================================================
//		NNumber::CompareIntReal : Compare an integer to a real.
//-----------------------------------------------------------------------------
NComparison NNumber::CompareIntReal(const NNumber& theNumber) const
{


	// Validate our parameters and state
	NN_REQUIRE(IsNegative() == theNumber.IsNegative());
	NN_REQUIRE(IsInteger() && theNumber.IsReal());



	// Compare integer / real
	//
	// A float64_t can only represent 53-bit integers exactly, which range from:
	//
	//		kNSafeIntegerMin			-9007199254740991
	//		kNSafeIntegerMax			 9007199254740991
	//
	// As a float64_t can hold values outside that range this gives us two
	// cases where always-correct comparisons can be made:
	//
	//	o A real outside the maximum integer range can be ordered correctly
	//	regardless of its value.
	//
	//	o A real within the safe integer range can be ordered correctly
	//	based on its value.
	//
	// Comparing integers to reals between the safe integer range (2^53) and
	// the maximum integer range (2^64) may give inconsistent results vs
	// comparisons between two numbers of the same type.
	//
	// We log a warning for this case as the caller may need to normalise
	// types before attempting a comparison.
	NComparison theResult = NComparison::EqualTo;

	if (IsNegative())
	{
		// Real is outside integer range
		if (theNumber.GetFloat64() < float64_t(kNInt64Min))
		{
			theResult = NComparison::GreaterThan;
		}

		// Real is within integer range
		else
		{
			// Real is outside safe range
			if (theNumber.GetFloat64() < kNSafeIntegerMin)
			{
				NN_LOG_WARNING("Integer/real comparison outside safe integer range");
			}

			theResult = NCompare(GetFloat64(), theNumber.GetFloat64());
		}
	}

	else
	{
		// Real is outside integer range
		if (theNumber.GetFloat64() > float64_t(kNUInt64Max))
		{
			theResult = NComparison::LessThan;
		}

		// Real is within integer range
		else
		{
			// Real is outside safe range
			if (theNumber.GetFloat64() > kNSafeIntegerMax)
			{
				NN_LOG_WARNING("Integer/real comparison outside safe integer range");
			}

			theResult = NCompare(GetFloat64(), theNumber.GetFloat64());
		}
	}

	return theResult;
}
