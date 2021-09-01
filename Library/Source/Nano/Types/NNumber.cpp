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
#include "NAny.h"
#include "NDebug.h"
#include "NString.h"
#include "NanoConstants.h"
#include "Nano_fast_float.h"





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
#define NN_LOG_INEXACT_COMPARISON                           0





//=============================================================================
//		NNumber::IsInteger : Is the number an integer?
//-----------------------------------------------------------------------------
bool NNumber::IsInteger() const
{


	// Check the value
	return mValue.IsUInt64() || mValue.IsInt64();
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
//		NNumber::GetBool : Get a bool value.
//-----------------------------------------------------------------------------
bool NNumber::GetBool() const
{


	// Get the value
	uint64_t theValue = GetUInt64();
	NN_REQUIRE(theValue <= 1);

	return theValue != 0;
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
		NN_REQUIRE(IsPositive());
		theValue = uint64_t(mValue.GetInt64());
	}
	else if (mValue.IsFloat64())
	{
		NN_REQUIRE(IsPositive() && IsInteger());
		NN_REQUIRE(mValue.GetFloat64() < float64_t(kNUInt64Max));
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
		NN_REQUIRE(mValue.GetUInt64() <= uint64_t(kNInt64Max));
		theValue = int64_t(mValue.GetUInt64());
	}
	else if (mValue.IsInt64())
	{
		theValue = mValue.GetInt64();
	}
	else if (mValue.IsFloat64())
	{
		NN_REQUIRE(IsInteger());
		NN_REQUIRE(mValue.GetFloat64() >= float64_t(kNInt64Min));
		NN_REQUIRE(mValue.GetFloat64() <= float64_t(kNInt64Max));
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
		NN_REQUIRE(uint64_t(theValue) == mValue.GetUInt64());
	}
	else if (mValue.IsInt64())
	{
		theValue = float64_t(mValue.GetInt64());
		NN_REQUIRE(int64_t(theValue) == mValue.GetInt64());
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
//		NNumber::SetValue : Set a value.
//-----------------------------------------------------------------------------
bool NNumber::SetValue(const NAny& theValue)
{


	// Set the value
	bool didSet = true;

	if (theValue.IsEmpty())
	{
		didSet = false;
	}

	else if (theValue.Is<NNumber>())
	{
		mValue = theValue.GetNumber().mValue;
	}

	else if (theValue.IsBool())
	{
		mValue = uint64_t(theValue.GetBool());
	}


	else if (theValue.Is<uint8_t>())
	{
		mValue = uint64_t(theValue.Get<uint8_t>());
	}
	else if (theValue.Is<uint16_t>())
	{
		mValue = uint64_t(theValue.Get<uint16_t>());
	}
	else if (theValue.IsUInt32())
	{
		mValue = uint64_t(theValue.GetUInt32());
	}
	else if (theValue.IsUInt64())
	{
		mValue = uint64_t(theValue.GetUInt64());
	}


	else if (theValue.Is<int8_t>())
	{
		mValue = int64_t(theValue.Get<int8_t>());
	}
	else if (theValue.Is<int16_t>())
	{
		mValue = int64_t(theValue.Get<int16_t>());
	}
	else if (theValue.IsInt32())
	{
		mValue = int64_t(theValue.GetInt32());
	}
	else if (theValue.IsInt64())
	{
		mValue = int64_t(theValue.GetInt64());
	}


	else if (theValue.IsFloat32())
	{
		mValue = float64_t(theValue.GetFloat32());
	}
	else if (theValue.IsFloat64())
	{
		mValue = float64_t(theValue.GetFloat64());
	}


	else if (theValue.Is<unsigned short>())
	{
		mValue = uint64_t(theValue.Get<unsigned short>());
	}
	else if (theValue.Is<unsigned int>())
	{
		mValue = uint64_t(theValue.Get<unsigned int>());
	}
	else if (theValue.Is<unsigned long>())
	{
		mValue = uint64_t(theValue.Get<unsigned long>());
	}
	else if (theValue.Is<unsigned long long>())
	{
		mValue = uint64_t(theValue.Get<unsigned long long>());
	}


	else if (theValue.Is<short>())
	{
		mValue = int64_t(theValue.Get<short>());
	}
	else if (theValue.Is<int>())
	{
		mValue = int64_t(theValue.Get<int>());
	}
	else if (theValue.Is<long>())
	{
		mValue = int64_t(theValue.Get<long>());
	}
	else if (theValue.Is<long long>())
	{
		mValue = int64_t(theValue.Get<long long>());
	}


	else if (theValue.Is<size_t>())
	{
		mValue = uint64_t(theValue.Get<size_t>());
	}
	else if (theValue.Is<uintptr_t>())
	{
		mValue = uint64_t(theValue.Get<uintptr_t>());
	}

	else
	{
		didSet = false;
	}

	return didSet;
}





//=============================================================================
//		NNumber::SetValue : Set a value.
//-----------------------------------------------------------------------------
bool NNumber::SetValue(const NString& theValue)
{


	// Set the value
	//
	// Reals must be parsed first as their prefix may look like an integer.
	const utf8_t* textStart = theValue.GetUTF8();
	bool          isValid   = ParseReal(textStart) || ParseInteger(textStart);

	return isValid;
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
		theResult = CompareIntReal(theNumber);
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
	NN_REQUIRE(IsInteger() != theNumber.IsInteger());



	// Get the state we need
	NNumber numberA = *this;
	NNumber numberB = theNumber;

	NComparison theResult = NComparison::EqualTo;
	bool        didSwap   = !numberA.IsInteger();

	if (didSwap)
	{
		std::swap(numberA, numberB);
	}



	// Compare integer / real
	//
	// A float64_t can represent a 53-bit range of consecutive integers:
	//
	//		kNIntegerSafeMin			-9007199254740991
	//		kNIntegerSafeMax			 9007199254740991
	//
	// As a float64_t can also hold values outside that range this gives
	// two cases where we can make always-correct comparisons:
	//
	//	o	A real outside the maximum integer range can be ordered
	//		correctly regardless of its real value.
	//
	//	o	A real within the safe integer range can be ordered
	//		correctly against integers by casting it to an integer.
	//
	// Reals between the safe integer range (2^53 - 1) and the maximum integer
	// range (2^64 - 1) can not represent consecutive integers.
	//
	// Casting integers in that range to reals will map multiple integers to
	// the same real:
	//
	//                  uint64_t               float64_t
	//                  --------               ---------
	//      18446744073709551615    18446744073709552000    <-- kNUInt64Max
	//      18446744073709551614    18446744073709552000
	//      18446744073709551613    18446744073709552000
	//      1844674407370955....    18446744073709552000
	//      18446744073709550592    18446744073709552000
	//      18446744073709550591    18446744073709550000
	//      18446744073709550590    18446744073709550000
	//      18446744073709550...    18446744073709550...
	//                    ......                  ......
	//          9007199254740991        9007199254740991    <-- kNIntegerSafeMax
	//          9007199254740990        9007199254740990
	//          9007199254740989        9007199254740989
	//          9007199254740988        9007199254740988
	//          9007199254740987        9007199254740987
	//
	// Casting reals in this range to integers will map each distinct real
	// value to a distinct integer value.
	//
	// As such these comparisons are performed as integers, giving us the
	// 'best' result in the sense of the the least information loss.
	//
	//
	// The outer edges of the maximum integer range (kNInt64Min / kNUInt64Max)
	// are rounded when cast to a float.
	//
	// We special case these edge values, rather than casting a real to an
	// integer, as the rounded value will exceed the maximum integer value
	// which is UB.
	float64_t realB = numberB.GetFloat64();

	if (IsNegative())
	{
		// Real outside integer range
		if (realB < float64_t(kNInt64Min))
		{
			theResult = NComparison::GreaterThan;
		}

		// Real within safe integer range
		else if (realB >= kNIntegerSafeMin)
		{
			int64_t intA = numberA.GetInt64();
			NN_REQUIRE(int64_t(float64_t(intA)) == intA);

			theResult = NCompare(float64_t(intA), realB);
		}

		// Real outwith safe integer range
		else
		{
			int64_t intA = numberA.GetInt64();
			int64_t intB = (realB == float64_t(kNInt64Min) ? kNInt64Min : int64_t(realB));

			theResult = NCompare(intA, intB);
			if constexpr (NN_LOG_INEXACT_COMPARISON)
			{
				NN_LOG_INFO("Int / real comparison outside safe zone, results may be misleading");
			}
		}
	}

	else
	{
		// Real outside integer range
		if (realB > float64_t(kNUInt64Max))
		{
			theResult = NComparison::LessThan;
		}

		// Real within safe integer range
		else if (realB <= kNIntegerSafeMax)
		{
			uint64_t uintA = numberA.GetUInt64();
			NN_REQUIRE(uint64_t(float64_t(uintA)) == uintA);

			theResult = NCompare(float64_t(uintA), realB);
		}

		// Real outwith safe integer zone
		else
		{
			uint64_t uintA = numberA.GetUInt64();
			uint64_t uintB = (realB == float64_t(kNUInt64Max) ? kNUInt64Max : uint64_t(realB));

			theResult = NCompare(uintA, uintB);
			if constexpr (NN_LOG_INEXACT_COMPARISON)
			{
				NN_LOG_INFO("Int / real comparison outside safe zone, results may be misleading");
			}
		}
	}



	// Swap the result
	//
	// If numberA wasn't the integer then the result is reversed.
	if (didSwap && theResult != NComparison::EqualTo)
	{
		if (theResult == NComparison::LessThan)
		{
			theResult = NComparison::GreaterThan;
		}
		else
		{
			theResult = NComparison::LessThan;
		}
	}

	return theResult;
}





//=============================================================================
//		NNumber::ParseInteger : Attempt to parse an integer value.
//-----------------------------------------------------------------------------
bool NNumber::ParseInteger(const utf8_t* textStart)
{


	// Validate our parameters
	NN_REQUIRE(textStart != nullptr);


	// Parse a negative integer
	bool isNegative = (strchr(textStart, '-') != nullptr);
	bool isValid    = false;

	if (isNegative)
	{
		utf8_t* textEnd    = const_cast<utf8_t*>(textStart);
		int64_t valueInt64 = strtoll(textStart, &textEnd, 0);

		isValid = (errno != ERANGE && textEnd != textStart);
		if (isValid)
		{
			mValue = valueInt64;
		}
	}



	// Parse a positive integer
	if (!isValid)
	{
		utf8_t*  textEnd     = const_cast<utf8_t*>(textStart);
		uint64_t valueUInt64 = strtoull(textStart, &textEnd, 0);

		isValid = (errno != ERANGE && textEnd != textStart);
		if (isValid)
		{
			mValue = valueUInt64;
		}
	}

	return isValid;
}





//=============================================================================
//		NNumber::ParseReal : Attempt to parse a real value.
//-----------------------------------------------------------------------------
bool NNumber::ParseReal(const utf8_t* textStart)
{


	// Validate our parameters
	NN_REQUIRE(textStart != nullptr);



	// Check the text
	bool isValid = (strchr(textStart, '.') != nullptr);

	if (!isValid)
	{
		isValid = HasExponent(textStart, 'e') || HasExponent(textStart, 'E') ||
				  HasExponent(textStart, 'p') || HasExponent(textStart, 'P');
	}



	// Attempt to convert
	if (isValid)
	{
		// Get the state we need
		float64_t valueFloat64 = 0.0;
		size_t    textLen      = strlen(textStart);

		bool isHex = (textLen >= 2 && textStart[0] == '0' && tolower(textStart[1]) == 'x') ||
					 (textLen >= 3 && textStart[1] == '0' && tolower(textStart[2]) == 'x');


		// Parse the value
		//
		// fast_float does not currently support hex so we must fall
		// back to strtod to parse these numbers.
		if (!isHex)
		{
			NN_DIAGNOSTIC_PUSH();
			NN_DIAGNOSTIC_IGNORE_MSVC(C4365_signed_unsigned_mismatch);

			const utf8_t* textEnd   = textStart + strlen(textStart);
			auto          theResult = fast_float::from_chars(textStart, textEnd, valueFloat64);
			isValid                 = (theResult.ec == std::errc());

			NN_DIAGNOSTIC_POP();
		}
		else
		{
			utf8_t* textEnd = const_cast<utf8_t*>(textStart);
			valueFloat64    = strtod(textStart, &textEnd);
			isValid         = (errno != ERANGE && textEnd != textStart);
		}


		// Update the value
		if (isValid)
		{
			mValue = valueFloat64;
		}
	}

	return isValid;
}





//=============================================================================
//		NNumber::ParseFloat64 : Attempt to parse a float64_t.
//-----------------------------------------------------------------------------
bool NNumber::HasExponent(const utf8_t* textStart, char theToken) const
{


	// Validate our parameters
	NN_REQUIRE(theToken == 'e' || theToken == 'E' || theToken == 'p' || theToken == 'P');



	// Check for an exponent
	//
	// An exponent is an 'eEpP' character, an optional '+-' sign,
	// and one or more decimal digits.
	const char* exponentStart = strchr(textStart, theToken);
	bool        hasExponent   = (exponentStart != nullptr);

	if (hasExponent)
	{
		if (exponentStart[1] == '+' || exponentStart[1] == '-')
		{
			hasExponent = (isdigit(exponentStart[2]) != 0);
		}
		else
		{
			hasExponent = (isdigit(exponentStart[1]) != 0);
		}
	}

	return hasExponent;
}
