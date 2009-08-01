/*	NAME:
		NNumber.cpp

	DESCRIPTION:
		Number object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NTargetPOSIX.h"
#include "NVariant.h"
#include "NNumber.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNStringZero										= "0.0";





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(const NVariant &theValue)
{	bool	wasOK;



	// Validate our parameters
	NN_ASSERT(theValue.IsNumeric());



	// Initialise ourselves
	wasOK = SetValue(theValue);
	NN_ASSERT(wasOK);
}





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(void)
{


	// Initialise ourselves
	mType          = kNumberInteger;
	mValue.integer = 0;
}





//============================================================================
//		NNumber::~NNumber : Destructor.
//----------------------------------------------------------------------------
NNumber::~NNumber(void)
{
}





//============================================================================
//		NNumber::GetType : Get the type.
//----------------------------------------------------------------------------
NumberType NNumber::GetType(void) const
{


	// Get the type
	return(mType);
}





//============================================================================
//		NNumber::GetString : Get the number as a string.
//----------------------------------------------------------------------------
NString NNumber::GetString(void) const
{	NString		valueText;



	// Get the string
	switch (mType) {
		case kNumberInteger:
			valueText.Format("%lld", mValue.integer);
			break;
			
		case kNumberReal:
			if (NTargetPOSIX::is_nan(mValue.real))
				valueText = kNStringNaN;

			else if (NTargetPOSIX::is_inf(mValue.real))
				valueText = (mValue.real < 0.0) ? kNStringInfinityNeg : kNStringInfinityPos;

			else if (NMathUtilities::IsZero(mValue.real))
				valueText = kNStringZero;

			else
				valueText.Format("%.17g", mValue.real);
			break;

		default:
			NN_LOG("Unknown number type: %d", mType);
			valueText = kNStringZero;
			break;
		}

	return(valueText);
}





//============================================================================
//		NNumber::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NNumber::Compare(const NNumber &theValue) const
{	SInt64				valueInteger;
	Float64				valueReal;
	NComparison			theResult;



	// Get the state we need
	theResult = kNCompareLessThan;



	// Compare equal types
	if (mType == theValue.mType)
		{
		switch (mType) {
			case kNumberInteger:
				theResult = GetComparison(mValue.integer, theValue.mValue.integer);
				break;
			
			case kNumberReal:
				theResult = GetComparison(mValue.real,    theValue.mValue.real);
				break;

			default:
				NN_LOG("Unknown number type: %d", mType);
				break;
			}
		}
	else
		{
		switch (mType) {
			case kNumberInteger:
				if (theValue.GetValueSInt64(valueInteger))
					theResult = GetComparison(mValue.integer, valueInteger);
				else
					NN_LOG("Unable to compare numbers across types");
				break;
			
			case kNumberReal:
				if (theValue.GetValueFloat64(valueReal))
					theResult = GetComparison(mValue.real, valueReal);
				else
					NN_LOG("Unable to compare numbers across types");
				break;

			default:
				NN_LOG("Unknown number type: %d", mType);
				break;
			}
		}

	return(theResult);
}





//============================================================================
//		NNumber::GetValueUInt8 : Get a UInt8 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueUInt8(UInt8 &theValue) const
{	SInt8		valueInteger;
	bool		canCast;



	// Get the value
	theValue = 0;
	canCast  = GetValueSInt8(valueInteger);

	if (canCast)
		theValue = (UInt8) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetValueUInt16 : Get a UInt16 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueUInt16(UInt16 &theValue) const
{	SInt16		valueInteger;
	bool		canCast;



	// Get the value
	theValue = 0;
	canCast  = GetValueSInt16(valueInteger);

	if (canCast)
		theValue = (UInt16) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetValueUInt32 : Get a UInt32 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueUInt32(UInt32 &theValue) const
{	SInt32		valueInteger;
	bool		canCast;



	// Get the value
	theValue = 0;
	canCast  = GetValueSInt32(valueInteger);

	if (canCast)
		theValue = (UInt32) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetValueUInt64 : Get a UInt64 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueUInt64(UInt64 &theValue) const
{	SInt64		valueInteger;
	bool		canCast;



	// Get the value
	valueInteger = 0;
	theValue     = 0;
	canCast      = GetValueSInt64(valueInteger);

	if (canCast)
		theValue = (UInt64) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetValueSInt8 : Get an SInt8 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueSInt8(SInt8 &theValue) const
{	SInt64		valueInteger;
	bool		canCast;



	// Get the value
	valueInteger = 0;
	theValue     = 0;
	canCast      = GetValueSInt64(valueInteger);

	if (canCast && valueInteger >= kSInt8Min && valueInteger <= kSInt8Max)
		theValue = (SInt8) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetValueSInt16 : Get an SInt16 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueSInt16(SInt16 &theValue) const
{	SInt64		valueInteger;
	bool		canCast;



	// Get the value
	valueInteger = 0;
	theValue     = 0;
	canCast      = GetValueSInt64(valueInteger);

	if (canCast && valueInteger >= kSInt16Min && valueInteger <= kSInt16Max)
		theValue = (SInt16) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetValueSInt32 : Get an SInt32 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueSInt32(SInt32 &theValue) const
{	SInt64		valueInteger;
	bool		canCast;



	// Get the value
	valueInteger = 0;
	theValue     = 0;
	canCast      = GetValueSInt64(valueInteger);

	if (canCast && valueInteger >= kSInt32Min && valueInteger <= kSInt32Max)
		theValue = (SInt32) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetValueSInt64 : Get an SInt64 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueSInt64(SInt64 &theValue) const
{	bool	canCast;



	// Get the value
	switch (mType) {
		case kNumberInteger:
			canCast  = true;
			theValue = mValue.integer;
			break;

		case kNumberReal:
			canCast = NMathUtilities::AreEqual(floor(mValue.real), mValue.real);
			if (canCast && mValue.real >= kSInt64Min && mValue.real <= kSInt64Max)
				theValue = (SInt64) mValue.real;
			break;

		default:
			NN_LOG("Unknown number type: %d", mType);

			canCast  = false;
			theValue = 0;
			break;
		}

	return(canCast);
}





//============================================================================
//		NNumber::GetValueFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueFloat32(Float32 &theValue) const
{	Float64		valueReal;
	bool		canCast;



	// Get the value
	valueReal = 0.0;
	theValue  = 0;
	canCast   = GetValueFloat64(valueReal);

	if (canCast && valueReal >= kFloat32Min && valueReal <= kFloat32Max)
		theValue = (Float32) valueReal;

	return(canCast);
}





//============================================================================
//		NNumber::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
bool NNumber::GetValueFloat64(Float64 &theValue) const
{	bool	canCast;



	// Get the value
	switch (mType) {
		case kNumberInteger:
			canCast = (mValue.integer >= kFloat64Min && mValue.integer <= kFloat64Max);
			if (canCast)
				theValue = (Float64) mValue.integer;
			break;

		case kNumberReal:
			canCast  = true;
			theValue = mValue.real;
			break;

		default:
			NN_LOG("Unknown number type: %d", mType);

			canCast  = false;
			theValue = 0.0;
			break;
		}

	return(canCast);
}





//============================================================================
//		NNumber::SetValueUInt8 : Set a UInt8 value.
//----------------------------------------------------------------------------
void NNumber::SetValueUInt8(UInt8 theValue)
{


	// Set the value
	SetValueSInt64(theValue);
}





//============================================================================
//		NNumber::SetValueUInt16 : Set a UInt16 value.
//----------------------------------------------------------------------------
void NNumber::SetValueUInt16(UInt16 theValue)
{


	// Set the value
	SetValueSInt64(theValue);
}





//============================================================================
//		NNumber::SetValueUInt32 : Set a UInt32 value.
//----------------------------------------------------------------------------
void NNumber::SetValueUInt32(UInt32 theValue)
{


	// Set the value
	SetValueSInt64(theValue);
}





//============================================================================
//		NNumber::SetValueUInt64 : Set a UInt64 value.
//----------------------------------------------------------------------------
void NNumber::SetValueUInt64(UInt64 theValue)
{


	// Set the value
	SetValueSInt64((SInt64) theValue);
}





//============================================================================
//		NNumber::SetValueSInt8 : Set a SInt8 value.
//----------------------------------------------------------------------------
void NNumber::SetValueSInt8(SInt8 theValue)
{


	// Set the value
	SetValueSInt64(theValue);
}





//============================================================================
//		NNumber::SetValueSInt16 : Set a SInt16 value.
//----------------------------------------------------------------------------
void NNumber::SetValueSInt16(SInt16 theValue)
{


	// Set the value
	SetValueSInt64(theValue);
}





//============================================================================
//		NNumber::SetValueSInt32 : Set a SInt32 value.
//----------------------------------------------------------------------------
void NNumber::SetValueSInt32(SInt32 theValue)
{


	// Set the value
	SetValueSInt64(theValue);
}





//============================================================================
//		NNumber::SetValueSInt64 : Set a SInt64 value.
//----------------------------------------------------------------------------
void NNumber::SetValueSInt64(SInt64 theValue)
{


	// Set the value
	mType          = kNumberInteger;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetValueFloat32 : Set a Float32 value.
//----------------------------------------------------------------------------
void NNumber::SetValueFloat32(Float32 theValue)
{


	// Set the value
	SetValueFloat64(theValue);
}





//============================================================================
//		NNumber::SetValueFloat64 : Set a Float64 value.
//----------------------------------------------------------------------------
void NNumber::SetValueFloat64(Float64 theValue)
{


	// Set the value
	mType       = kNumberReal;
	mValue.real = theValue;
}





//============================================================================
//		NNumber::SetValue : Set the value.
//----------------------------------------------------------------------------
bool NNumber::SetValue(const NVariant &theValue)
{	UInt8					valueUInt8;
	UInt16					valueUInt16;
	UInt32					valueUInt32;
	UInt64					valueUInt64;
	SInt8					valueSInt8;
	SInt16					valueSInt16;
	SInt32					valueSInt32;
	SInt64					valueSInt64;
	Float32					valueFloat32;
	Float64					valueFloat64;
	int						valueInt;
	long					valueLong;



	// Set the value
	//
	// NVariant treats some unsized types as numeric, to support literal constants.
	if (theValue.GetValue(valueUInt8))
		SetValueUInt8(valueUInt8);

	else if (theValue.GetValue(valueUInt16))
		SetValueUInt16(valueUInt16);
	
	else if (theValue.GetValue(valueUInt32))
		SetValueUInt32(valueUInt32);
	
	else if (theValue.GetValue(valueUInt64))
		SetValueUInt64(valueUInt64);
	
	else if (theValue.GetValue(valueSInt8))
		SetValueSInt8(valueSInt8);
	
	else if (theValue.GetValue(valueSInt16))
		SetValueSInt16(valueSInt16);
	
	else if (theValue.GetValue(valueSInt32))
		SetValueSInt32(valueSInt32);
	
	else if (theValue.GetValue(valueSInt64))
		SetValueSInt64(valueSInt64);
	
	else if (theValue.GetValue(valueFloat32))
		SetValueFloat32(valueFloat32);
	
	else if (theValue.GetValue(valueFloat64))
		SetValueFloat64(valueFloat64);
	
	else if (theValue.GetValue(valueInt))
		SetValueSInt64(valueInt);
	
	else if (theValue.GetValue(valueLong))
		SetValueSInt64(valueLong);
	
	else
		return(false);

	return(true);
}





//============================================================================
//		NNumber::SetValue : Set the value.
//----------------------------------------------------------------------------
bool NNumber::SetValue(const NString &theValue)
{	SInt64		valueInteger;
	Float64		valueReal;



	// Parse the value
	//
	// Some integers will also pass parsing as floats, however we coerce these
	// back to integers when possible to allow us to use more tightly packed
	// types for storage in the future.
	if (sscanf(theValue.GetUTF8(), "%lf", &valueReal) == 1)
		{
		SetValueFloat64(valueReal);

		if (GetValueSInt64(valueInteger))
			SetValueSInt64(valueInteger);

		return(true);
		}

	else if (sscanf(theValue.GetUTF8(), "%lld",   &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "%llx",   &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "0x%llx", &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "0X%llx", &valueInteger) == 1)
		{
		SetValueSInt64(valueInteger);
		return(true);
		}

	return(false);
}

