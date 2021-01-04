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

/*
 #include "NEncoder.h"
 #include "NMathUtilities.h"
 #include "NNumber.h"
 #include "NTargetPOSIX.h"
 #include "NVariant.h"
 */





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
/*
   static const NString kNStringZero                        = "0";

   static const NIndex kDecimalsFloat32                     = 7;
   static const NIndex kDecimalsFloat64                     = 17;

   static const NString kFormatFloat32                      = "%.7g";
   static const NString kFormatFloat64                      = "%.17g";
 */



/*
   //=============================================================================
   //		NNumber::NNumber : Constructor.
   //-----------------------------------------------------------------------------
   NNumber::NNumber(const NVariant& theValue)
   {


	// Initialise ourselves
	SetUInt8(0);

	if (theValue.IsNumeric())
	{
		SetValue(theValue);
	}
   }




   //=============================================================================
   //		NNumber::NNumber : Constructor.
   //-----------------------------------------------------------------------------
   NNumber::NNumber(const NString& theValue)
   {


	// Initialise ourselves
	SetUInt8(0);

	if (!theValue.IsEmpty())
	{
		SetValue(theValue);
	}
   }
 */





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(uint8_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetUInt8(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(uint16_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetUInt16(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(uint32_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetUInt32(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(uint64_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetUInt64(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(int8_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetInt8(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(int16_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetInt16(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(int32_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetInt32(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(int64_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetInt64(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(float32_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetFloat32(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber(float64_t theValue)
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetFloat64(theValue);
}





//=============================================================================
//		NNumber::NNumber : Constructor.
//-----------------------------------------------------------------------------
NNumber::NNumber()
	: mPrecision(NPrecision::None)
	, mValue()
{


	// Initialise ourselves
	SetUInt8(0);
}





//=============================================================================
//		NNumber::IsValid : Is the number valid?
//-----------------------------------------------------------------------------
bool NNumber::IsValid() const
{


	// Check the precision
	return mPrecision != NPrecision::None;
}





//=============================================================================
//		NNumber::IsInteger : Is the number an integer?
//-----------------------------------------------------------------------------
bool NNumber::IsInteger() const
{


	// Check the precision
	bool isInteger = false;
	switch (mPrecision)
	{
		case NPrecision::UInt8:
		case NPrecision::UInt16:
		case NPrecision::UInt32:
		case NPrecision::UInt64:
		case NPrecision::Int8:
		case NPrecision::Int16:
		case NPrecision::Int32:
		case NPrecision::Int64:
			isInteger = true;
			break;

		default:
			break;
	}

	return isInteger;
}





//=============================================================================
//		NNumber::IsReal : Is the number a real number?
//-----------------------------------------------------------------------------
bool NNumber::IsReal() const
{


	// Check the precision
	bool isReal = false;

	switch (mPrecision)
	{
		case NPrecision::Float32:
		case NPrecision::Float64:
			isReal = true;
			break;

		default:
			break;
	}

	return isReal;
}





//=============================================================================
//		NNumber::IsSigned : Is the number a signed number?
//-----------------------------------------------------------------------------
bool NNumber::IsSigned() const
{


	// Check the precision
	bool isSigned = false;

	switch (mPrecision)
	{
		case NPrecision::Int8:
		case NPrecision::Int16:
		case NPrecision::Int32:
		case NPrecision::Int64:
		case NPrecision::Float32:
		case NPrecision::Float64:
			isSigned = true;
			break;

		default:
			break;
	}

	return isSigned;
}





//=============================================================================
//		NNumber::IsNegative : Is the number negative?
//-----------------------------------------------------------------------------
bool NNumber::IsNegative() const
{


	// Check the precision
	bool isNegative = false;

	switch (mPrecision)
	{
		case NPrecision::Int8:
		case NPrecision::Int16:
		case NPrecision::Int32:
		case NPrecision::Int64:
			isNegative = (GetInt64() < 0);

		case NPrecision::Float32:
		case NPrecision::Float64:
			isNegative = (GetFloat64() < 0.0);
			break;

		default:
			break;
	}

	return isNegative;
}





//=============================================================================
//		NNumber::GetPrecision : Get the precision.
//-----------------------------------------------------------------------------
NPrecision NNumber::GetPrecision() const
{


	// Get the precision
	return mPrecision;
}





//=============================================================================
//		NNumber::Compare : Compare the value.
//-----------------------------------------------------------------------------
NComparison NNumber::Compare(const NNumber& theValue) const
{
	float64_t   valueFloat64_1, valueFloat64_2;
	float32_t   valueFloat32_1, valueFloat32_2;
	int64_t     valueInt64_1, valueInt64_2;
	NComparison theResult;



	// Compare equal types
	if (mPrecision == theValue.mPrecision || (IsInteger() && theValue.IsInteger()))
	{
		switch (mPrecision)
		{
			case kNPrecisionInt8:
			case kNPrecisionInt16:
			case kNPrecisionInt32:
			case kNPrecisionInt64:
				theResult = GetComparison(mValue.integer, theValue.mValue.integer);
				break;

			case kNPrecisionFloat32:
				valueFloat32_1 = (float32_t) mValue.real;
				valueFloat32_2 = (float32_t) theValue.mValue.real;
				theResult      = GetComparison(valueFloat32_1, valueFloat32_2);
				break;

			case kNPrecisionFloat64:
				theResult = GetComparison(mValue.real, theValue.mValue.real);
				break;

			default:
				NN_LOG("Unknown precision: %d", mPrecision);
				theResult = kNCompareLessThan;
				break;
		}
	}


	// Compare dis-similar types
	else
	{
		switch (mPrecision)
		{
			case kNPrecisionInt8:
			case kNPrecisionInt16:
			case kNPrecisionInt32:
			case kNPrecisionInt64:
				valueInt64_1 = GetInt64();
				valueInt64_2 = theValue.GetInt64();
				theResult    = GetComparison(valueInt64_1, valueInt64_2);
				break;

			case kNPrecisionFloat32:
				valueFloat32_1 = GetFloat32();
				valueFloat32_2 = theValue.GetFloat32();
				theResult      = GetComparison(valueFloat32_1, valueFloat32_2);
				break;

			case kNPrecisionFloat64:
				valueFloat64_1 = GetFloat64();
				valueFloat64_2 = theValue.GetFloat64();
				theResult      = GetComparison(valueFloat64_1, valueFloat64_2);
				break;

			default:
				NN_LOG("Unknown precision: %d", mPrecision);
				theResult = kNCompareLessThan;
				break;
		}
	}

	return theResult;
}





//=============================================================================
//		NNumber::GetUInt8 : Get a uint8_t value.
//-----------------------------------------------------------------------------
uint8_t NNumber::GetUInt8() const
{


	// Get the value
	return (uint8_t) GetInt64();
}





//=============================================================================
//		NNumber::GetUInt16 : Get a uint16_t value.
//-----------------------------------------------------------------------------
uint16_t NNumber::GetUInt16() const
{


	// Get the value
	return (uint16_t) GetInt64();
}





//=============================================================================
//		NNumber::GetUInt32 : Get a uint32_t value.
//-----------------------------------------------------------------------------
uint32_t NNumber::GetUInt32() const
{


	// Get the value
	return (uint32_t) GetInt64();
}





//=============================================================================
//		NNumber::GetUInt64 : Get a uint64_t value.
//-----------------------------------------------------------------------------
uint64_t NNumber::GetUInt64() const
{


	// Get the value
	return (uint64_t) GetInt64();
}





//=============================================================================
//		NNumber::GetInt8 : Get an int8_t value.
//-----------------------------------------------------------------------------
int8_t NNumber::GetInt8() const
{


	// Get the value
	return (int8_t) GetInt64();
}





//=============================================================================
//		NNumber::GetInt16 : Get an int16_t value.
//-----------------------------------------------------------------------------
int16_t NNumber::GetInt16() const
{


	// Get the value
	return (int16_t) GetInt64();
}





//=============================================================================
//		NNumber::GetInt32 : Get an int32_t value.
//-----------------------------------------------------------------------------
int32_t NNumber::GetInt32() const
{


	// Get the value
	return (int32_t) GetInt64();
}





//=============================================================================
//		NNumber::GetInt64 : Get an int64_t value.
//-----------------------------------------------------------------------------
int64_t NNumber::GetInt64() const
{
	int64_t theValue;



	// Get the value
	if (IsInteger())
	{
		theValue = (int64_t) mValue.integer;
	}
	else
	{
		theValue = (int64_t) mValue.real;
	}

	return theValue;
}





//=============================================================================
//		NNumber::GetFloat32 : Get a float32_t value.
//-----------------------------------------------------------------------------
float32_t NNumber::GetFloat32() const
{
	float32_t theValue;



	// Get the value
	if (IsInteger())
	{
		theValue = (float32_t) mValue.integer;
	}
	else
	{
		theValue = (float32_t) mValue.real;
	}

	return theValue;
}





//=============================================================================
//		NNumber::GetFloat64 : Get a float64_t value.
//-----------------------------------------------------------------------------
float64_t NNumber::GetFloat64() const
{
	float64_t theValue;



	// Get the value
	if (IsInteger())
	{
		theValue = (float64_t) mValue.integer;
	}
	else
	{
		theValue = (float64_t) mValue.real;
	}

	return theValue;
}





//=============================================================================
//		NNumber::GetString : Get a string value.
//-----------------------------------------------------------------------------
NString NNumber::GetString() const
{
	NString valueText;



	// Get the string
	switch (mPrecision)
	{
		case kNPrecisionInt8:
		case kNPrecisionInt16:
		case kNPrecisionInt32:
		case kNPrecisionInt64:
			valueText.Format("%lld", mValue.integer);
			break;

		case kNPrecisionFloat32:
		case kNPrecisionFloat64:
			if (NTargetPOSIX::is_nan(mValue.real))
			{
				valueText = kNStringNaN;
			}

			else if (NTargetPOSIX::is_inf(mValue.real))
			{
				valueText = (mValue.real < 0.0) ? kNStringInfinityNeg : kNStringInfinityPos;
			}

			else if (NMathUtilities::IsZero(mValue.real))
			{
				valueText = kNStringZero;
			}

			else
			{
				if (mPrecision == kNPrecisionFloat32)
				{
					valueText.Format(kFormatFloat32, (float32_t) mValue.real);
				}
				else
				{
					valueText.Format(kFormatFloat64, mValue.real);
				}
			}
			break;

		default:
			NN_LOG("Unknown precision: %d", mPrecision);
			valueText = kNStringZero;
			break;
	}

	return valueText;
}





//=============================================================================
//		NNumber::SetUInt8 : Set a uint8_t value.
//-----------------------------------------------------------------------------
void NNumber::SetUInt8(uint8_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt8;
	mValue.integer = theValue;
}





//=============================================================================
//		NNumber::SetUInt16 : Set a uint16_t value.
//-----------------------------------------------------------------------------
void NNumber::SetUInt16(uint16_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt16;
	mValue.integer = theValue;
}





//=============================================================================
//		NNumber::SetUInt32 : Set a uint32_t value.
//-----------------------------------------------------------------------------
void NNumber::SetUInt32(uint32_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt32;
	mValue.integer = theValue;
}





//=============================================================================
//		NNumber::SetUInt64 : Set a uint64_t value.
//-----------------------------------------------------------------------------
void NNumber::SetUInt64(uint64_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt64;
	mValue.integer = theValue;
}





//=============================================================================
//		NNumber::SetInt8 : Set an int8_t value.
//-----------------------------------------------------------------------------
void NNumber::SetInt8(int8_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt8;
	mValue.integer = theValue;
}





//=============================================================================
//		NNumber::SetInt16 : Set an int16_t value.
//-----------------------------------------------------------------------------
void NNumber::SetInt16(int16_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt16;
	mValue.integer = theValue;
}





//=============================================================================
//		NNumber::SetInt32 : Set an int32_t value.
//-----------------------------------------------------------------------------
void NNumber::SetInt32(int32_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt32;
	mValue.integer = theValue;
}





//=============================================================================
//		NNumber::SetInt64 : Set an int64_t value.
//-----------------------------------------------------------------------------
void NNumber::SetInt64(int64_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt64;
	mValue.integer = theValue;
}





//=============================================================================
//		NNumber::SetFloat32 : Set a float32_t value.
//-----------------------------------------------------------------------------
void NNumber::SetFloat32(float32_t theValue)
{


	// Set the value
	mPrecision  = kNPrecisionFloat32;
	mValue.real = theValue;
}





//=============================================================================
//		NNumber::SetFloat64 : Set a float64_t value.
//-----------------------------------------------------------------------------
void NNumber::SetFloat64(float64_t theValue)
{


	// Set the value
	mPrecision  = kNPrecisionFloat64;
	mValue.real = theValue;
}



/*
   //=============================================================================
   //		NNumber::SetValue : Set the value.
   //-----------------------------------------------------------------------------
   bool NNumber::SetValue(const NVariant& theValue)
   {
	uint8_t   valueUInt8;
	uint16_t  valueUInt16;
	uint32_t  valueUInt32;
	uint64_t  valueUInt64;
	int8_t    valueSInt8;
	int16_t   valueSInt16;
	int32_t   valueSInt32;
	int64_t   valueSInt64;
	float32_t valueFloat32;
	float64_t valueFloat64;
	int       valueInt;
	long      valueLong;
	NString   valueString;



	// Set the value
	//
	// NVariant treats some unsized types as numeric, to support literal constants.
	if (theValue.GetValue(*this))
	{
		;    // Assigned to this
	}
	else if (theValue.GetValue(valueUInt8))
	{
		SetUInt8(valueUInt8);
	}

	else if (theValue.GetValue(valueUInt16))
	{
		SetUInt16(valueUInt16);
	}

	else if (theValue.GetValue(valueUInt32))
	{
		SetUInt32(valueUInt32);
	}

	else if (theValue.GetValue(valueUInt64))
	{
		SetUInt64(valueUInt64);
	}

	else if (theValue.GetValue(valueSInt8))
	{
		SetInt8(valueSInt8);
	}

	else if (theValue.GetValue(valueSInt16))
	{
		SetInt16(valueSInt16);
	}

	else if (theValue.GetValue(valueSInt32))
	{
		SetInt32(valueSInt32);
	}

	else if (theValue.GetValue(valueSInt64))
	{
		SetInt64(valueSInt64);
	}

	else if (theValue.GetValue(valueFloat32))
	{
		SetFloat32(valueFloat32);
	}

	else if (theValue.GetValue(valueFloat64))
	{
		SetFloat64(valueFloat64);
	}

	else if (theValue.GetValue(valueInt))
	{
		SetInt64(valueInt);
	}

	else if (theValue.GetValue(valueLong))
	{
		SetInt64(valueLong);
	}

	else if (theValue.GetValue(valueString))
	{
		return SetValue(valueString);
	}

	else
	{
		return false;
	}

	return true;
   }
 */





//=============================================================================
//		NNumber::SetString : Set a string value.
//-----------------------------------------------------------------------------
bool NNumber::SetString(const NString& theValue)
{
	NRange    foundDot, foundE;
	NIndex    thePrecision;
	int64_t   valueInteger;
	float64_t valueReal;



	// Parse the value
	//
	// Some integers will also pass parsing as floats, however we coerce these
	// back to integers when possible to allow us to use more tightly packed
	// types for storage in the future.
	if (sscanf(theValue.GetUTF8(), "%lf", &valueReal) == 1)
	{
		// Get the state we need
		foundDot = theValue.Find(".");
		foundE   = theValue.Find("e", kNStringNoCase);

		if (foundDot.IsEmpty() || !foundE.IsEmpty())
		{
			thePrecision = kDecimalsFloat64;
		}
		else
		{
			thePrecision = theValue.GetSize() - foundDot.GetNext();
		}



		// Cast the value
		if (foundDot.IsEmpty() && foundE.IsEmpty() && valueReal >= kInt64Min &&
			valueReal <= kInt64Max)
		{
			SetInt64((int64_t) valueReal);
		}

		else if (thePrecision <= kDecimalsFloat32 && valueReal >= kFloat32Min &&
				 valueReal <= kFloat32Max)
		{
			SetFloat32((float32_t) valueReal);
		}

		else
		{
			SetFloat64(valueReal);
		}

		return true;
	}

	else if (sscanf(theValue.GetUTF8(), "%lld", &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "%llx", &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "0x%llx", &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "0X%llx", &valueInteger) == 1)
	{
		SetInt64(valueInteger);
		return true;
	}

	return false;
}





#pragma mark NMixinComparable
//=============================================================================
//		NNumber::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NNumber::CompareEqual(const NNumber& theNumber) const
{


	// Compare the number
	return false;
}





//=============================================================================
//		NNumber::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NNumber::CompareOrder(const NNumber& theNumber) const
{


	// Order by value
	return NComparison::EqualTo;
}
