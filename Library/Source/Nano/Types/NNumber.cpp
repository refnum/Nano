/*	NAME:
		NNumber.cpp

	DESCRIPTION:
		Number object.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
#include "NEncoder.h"
#include "NNumber.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNStringZero										= "0";

static const NIndex kDecimalsFloat32									= 7;
static const NIndex kDecimalsFloat64									= 17;

static const NString kFormatFloat32										= "%.7g";
static const NString kFormatFloat64										= "%.17g";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NNumber);





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(const NVariant &theValue)
{


	// Initialise ourselves
	SetUInt8(0);

	if (theValue.IsNumeric())
		SetValue(theValue);
}





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(const NString &theValue)
{


	// Initialise ourselves
	SetUInt8(0);

	if (!theValue.IsEmpty())
		SetValue(theValue);
}





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(SInt64 theValue)
{


	// Initialise ourselves
	SetSInt64(theValue);
}





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(Float32 theValue)
{


	// Initialise ourselves
	SetFloat32(theValue);
}





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(Float64 theValue)
{


	// Initialise ourselves
	SetFloat64(theValue);
}





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(void)
{


	// Initialise ourselves
	mPrecision     = kNPrecisionInt8;
	mValue.integer = 0;
}





//============================================================================
//		NNumber::~NNumber : Destructor.
//----------------------------------------------------------------------------
NNumber::~NNumber(void)
{
}





//============================================================================
//		NNumber::IsInteger : Is the number an integer?
//----------------------------------------------------------------------------
bool NNumber::IsInteger(void) const
{	bool	isInteger;



	// Check the type
	switch (mPrecision) {
		case kNPrecisionInt8:
		case kNPrecisionInt16:
		case kNPrecisionInt32:
		case kNPrecisionInt64:
			isInteger = true;
			break;
		
		case kNPrecisionFloat32:
		case kNPrecisionFloat64:
			isInteger = false;
			break;
		
		default:
			NN_LOG("Unknown precision: %d", mPrecision);
			isInteger = false;
			break;
		}
	
	return(isInteger);
}





//============================================================================
//		NNumber::GetPrecision : Get the precision.
//----------------------------------------------------------------------------
NPrecision NNumber::GetPrecision(void) const
{


	// Get the precision
	return(mPrecision);
}





//============================================================================
//		NNumber::GetString : Get the number as a string.
//----------------------------------------------------------------------------
NString NNumber::GetString(void) const
{	NString		valueText;



	// Get the string
	switch (mPrecision) {
		case kNPrecisionInt8:
		case kNPrecisionInt16:
		case kNPrecisionInt32:
		case kNPrecisionInt64:
			valueText.Format("%lld", mValue.integer);
			break;
			
		case kNPrecisionFloat32:
		case kNPrecisionFloat64:
			if (NTargetPOSIX::is_nan(mValue.real))
				valueText = kNStringNaN;

			else if (NTargetPOSIX::is_inf(mValue.real))
				valueText = (mValue.real < 0.0) ? kNStringInfinityNeg : kNStringInfinityPos;

			else if (NMathUtilities::IsZero(mValue.real))
				valueText = kNStringZero;

			else
				{
				if (mPrecision == kNPrecisionFloat32)
					valueText.Format(kFormatFloat32, (Float32) mValue.real);
				else
					valueText.Format(kFormatFloat64,           mValue.real);
				}
			break;

		default:
			NN_LOG("Unknown precision: %d", mPrecision);
			valueText = kNStringZero;
			break;
		}

	return(valueText);
}





//============================================================================
//		NNumber::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NNumber::Compare(const NNumber &theValue) const
{	Float64				valueFloat64_1, valueFloat64_2;
	Float32				valueFloat32_1, valueFloat32_2;
	SInt64				valueInt64_1,  valueInt64_2;
	NComparison			theResult;



	// Get the state we need
	theResult = kNCompareLessThan;



	// Compare equal types
	if (mPrecision == theValue.mPrecision || (IsInteger() && theValue.IsInteger()))
		{
		switch (mPrecision) {
			case kNPrecisionInt8:
			case kNPrecisionInt16:
			case kNPrecisionInt32:
			case kNPrecisionInt64:
				theResult = GetComparison(mValue.integer, theValue.mValue.integer);
				break;
			
			case kNPrecisionFloat32:
				valueFloat32_1 = (Float32)          mValue.real;
				valueFloat32_2 = (Float32) theValue.mValue.real;
				theResult      = GetComparison(valueFloat32_1, valueFloat32_2);
				break;

			case kNPrecisionFloat64:
				theResult = GetComparison(mValue.real, theValue.mValue.real);
				break;

			default:
				NN_LOG("Unknown precision: %d", mPrecision);
				break;
			}
		}


	// Compare dis-similar types
	else
		{
		switch (mPrecision) {
			case kNPrecisionInt8:
			case kNPrecisionInt16:
			case kNPrecisionInt32:
			case kNPrecisionInt64:
				valueInt64_1 =          GetSInt64();
				valueInt64_2 = theValue.GetSInt64();
				theResult    = GetComparison(valueInt64_1, valueInt64_2);
				break;

			case kNPrecisionFloat32:
				valueFloat32_1 =          GetFloat32();
				valueFloat32_2 = theValue.GetFloat32();
				theResult      = GetComparison(valueFloat32_1, valueFloat32_2);
				break;

			case kNPrecisionFloat64:
				valueFloat64_1 =          GetFloat64();
				valueFloat64_2 = theValue.GetFloat64();
				theResult      = GetComparison(valueFloat64_1, valueFloat64_2);
				break;

			default:
				NN_LOG("Unknown precision: %d", mPrecision);
				break;
			}
		}

	return(theResult);
}





//============================================================================
//		NNumber::GetUInt8 : Get a UInt8 value.
//----------------------------------------------------------------------------
UInt8 NNumber::GetUInt8(void) const
{


	// Get the value
	return((UInt8) GetSInt64());
}





//============================================================================
//		NNumber::GetUInt16 : Get a UInt16 value.
//----------------------------------------------------------------------------
UInt16 NNumber::GetUInt16(void) const
{


	// Get the value
	return((UInt16) GetSInt64());
}





//============================================================================
//		NNumber::GetUInt32 : Get a UInt32 value.
//----------------------------------------------------------------------------
UInt32 NNumber::GetUInt32(void) const
{


	// Get the value
	return((UInt32) GetSInt64());
}





//============================================================================
//		NNumber::GetUInt64 : Get a UInt64 value.
//----------------------------------------------------------------------------
UInt64 NNumber::GetUInt64(void) const
{


	// Get the value
	return((UInt64) GetSInt64());
}





//============================================================================
//		NNumber::GetSInt8 : Get a SInt8 value.
//----------------------------------------------------------------------------
SInt8 NNumber::GetSInt8(void) const
{


	// Get the value
	return((SInt8) GetSInt64());
}





//============================================================================
//		NNumber::GetSInt16 : Get a SInt16 value.
//----------------------------------------------------------------------------
SInt16 NNumber::GetSInt16(void) const
{


	// Get the value
	return((SInt16) GetSInt64());
}





//============================================================================
//		NNumber::GetSInt32 : Get a SInt32 value.
//----------------------------------------------------------------------------
SInt32 NNumber::GetSInt32(void) const
{


	// Get the value
	return((SInt32) GetSInt64());
}





//============================================================================
//		NNumber::GetSInt64 : Get a SInt64 value.
//----------------------------------------------------------------------------
SInt64 NNumber::GetSInt64(void) const
{	SInt64		theValue;



	// Get the value
	if (IsInteger())
		theValue = (SInt64) mValue.integer;
	else
		theValue = (SInt64) mValue.real;
	
	return(theValue);
}





//============================================================================
//		NNumber::GetFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NNumber::GetFloat32(void) const
{	Float32		theValue;



	// Get the value
	if (IsInteger())
		theValue = (Float32) mValue.integer;
	else
		theValue = (Float32) mValue.real;
	
	return(theValue);
}





//============================================================================
//		NNumber::GetFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NNumber::GetFloat64(void) const
{	Float64		theValue;



	// Get the value
	if (IsInteger())
		theValue = (Float64) mValue.integer;
	else
		theValue = (Float64) mValue.real;
	
	return(theValue);
}





//============================================================================
//		NNumber::SetUInt8 : Set a UInt8 value.
//----------------------------------------------------------------------------
void NNumber::SetUInt8(UInt8 theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt8;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetUInt16 : Set a UInt16 value.
//----------------------------------------------------------------------------
void NNumber::SetUInt16(UInt16 theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt16;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetUInt32 : Set a UInt32 value.
//----------------------------------------------------------------------------
void NNumber::SetUInt32(UInt32 theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt32;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetUInt64 : Set a UInt64 value.
//----------------------------------------------------------------------------
void NNumber::SetUInt64(UInt64 theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt64;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetSInt8 : Set a SInt8 value.
//----------------------------------------------------------------------------
void NNumber::SetSInt8(SInt8 theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt8;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetSInt16 : Set a SInt16 value.
//----------------------------------------------------------------------------
void NNumber::SetSInt16(SInt16 theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt16;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetSInt32 : Set a SInt32 value.
//----------------------------------------------------------------------------
void NNumber::SetSInt32(SInt32 theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt32;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetSInt64 : Set a SInt64 value.
//----------------------------------------------------------------------------
void NNumber::SetSInt64(SInt64 theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt64;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetFloat32 : Set a Float32 value.
//----------------------------------------------------------------------------
void NNumber::SetFloat32(Float32 theValue)
{


	// Set the value
	mPrecision  = kNPrecisionFloat32;
	mValue.real = theValue;
}





//============================================================================
//		NNumber::SetFloat64 : Set a Float64 value.
//----------------------------------------------------------------------------
void NNumber::SetFloat64(Float64 theValue)
{


	// Set the value
	mPrecision  = kNPrecisionFloat64;
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
	if (theValue.GetValue(*this))
		; // Assigned to this

	else if (theValue.GetValue(valueUInt8))
		SetUInt8(valueUInt8);

	else if (theValue.GetValue(valueUInt16))
		SetUInt16(valueUInt16);
	
	else if (theValue.GetValue(valueUInt32))
		SetUInt32(valueUInt32);
	
	else if (theValue.GetValue(valueUInt64))
		SetUInt64(valueUInt64);
	
	else if (theValue.GetValue(valueSInt8))
		SetSInt8(valueSInt8);
	
	else if (theValue.GetValue(valueSInt16))
		SetSInt16(valueSInt16);
	
	else if (theValue.GetValue(valueSInt32))
		SetSInt32(valueSInt32);
	
	else if (theValue.GetValue(valueSInt64))
		SetSInt64(valueSInt64);
	
	else if (theValue.GetValue(valueFloat32))
		SetFloat32(valueFloat32);
	
	else if (theValue.GetValue(valueFloat64))
		SetFloat64(valueFloat64);
	
	else if (theValue.GetValue(valueInt))
		SetSInt64(valueInt);
	
	else if (theValue.GetValue(valueLong))
		SetSInt64(valueLong);
	
	else
		return(false);

	return(true);
}





//============================================================================
//		NNumber::SetValue : Set the value.
//----------------------------------------------------------------------------
bool NNumber::SetValue(const NString &theValue)
{	NRange		foundDot, foundE;
	NIndex		thePrecision;
	SInt64		valueInteger;
	Float64		valueReal;



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
		
		thePrecision = theValue.GetSize() - foundDot.GetNext();
		if (foundDot.IsEmpty() || !foundE.IsEmpty())
			thePrecision = kDecimalsFloat64;



		// Cast the value
		if (foundDot.IsEmpty() && foundE.IsEmpty() && valueReal >= kSInt64Min  && valueReal <= kSInt64Max)
			SetSInt64((SInt64) valueReal);
		else if (thePrecision <= kDecimalsFloat32  && valueReal >= kFloat32Min && valueReal <= kFloat32Max)
			SetFloat32((Float32) valueReal);
		else
			SetFloat64(valueReal);

		return(true);
		}

	else if (sscanf(theValue.GetUTF8(), "%lld",   &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "%llx",   &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "0x%llx", &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "0X%llx", &valueInteger) == 1)
		{
		SetSInt64(valueInteger);
		return(true);
		}

	return(false);
}





//============================================================================
//		NNumber::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NNumber::operator NFormatArgument(void) const
{


	// Get the value
	return(GetString());
}





//============================================================================
//      NNumber::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
#pragma mark -
void NNumber::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNEncoderValueKey, *this);
}





//============================================================================
//      NNumber::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NNumber::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	*this = theEncoder.DecodeNumber(kNEncoderValueKey);
}
