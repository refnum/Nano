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
#include "NEncoder.h"
#include "NNumber.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNStringZero										= "0.0";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
DEFINE_NENCODABLE(NNumber);





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
NNumber::NNumber(const NString &theValue)
{	bool	wasOK;



	// Validate our parameters
	NN_ASSERT(!theValue.IsEmpty());



	// Initialise ourselves
	wasOK = SetValue(theValue);
	NN_ASSERT(wasOK);
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
	mType          = kNNumberInteger;
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
{


	// Get the type
	return(mType == kNNumberInteger);
}





//============================================================================
//		NNumber::GetString : Get the number as a string.
//----------------------------------------------------------------------------
NString NNumber::GetString(void) const
{	NString		valueText;



	// Get the string
	//
	// Having separate 32 and 64-bit float types allows us to ensure Float32
	// values do not get expanded to Float64 precision due to rounding.
	switch (mType) {
		case kNNumberInteger:
			valueText.Format("%lld", mValue.integer);
			break;
			
		case kNNumberFloat32:
		case kNNumberFloat64:
			if (NTargetPOSIX::is_nan(mValue.real))
				valueText = kNStringNaN;

			else if (NTargetPOSIX::is_inf(mValue.real))
				valueText = (mValue.real < 0.0) ? kNStringInfinityNeg : kNStringInfinityPos;

			else if (NMathUtilities::IsZero(mValue.real))
				valueText = kNStringZero;

			else
				{
				if (mType == kNNumberFloat32)
					valueText.Format("%.7g", (Float32) mValue.real);
				else
					valueText.Format("%.17g",          mValue.real);
				}
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
{	Float64				valueFloat64_1, valueFloat64_2;
	Float32				valueFloat32_1, valueFloat32_2;
	SInt64				valueInteger;
	NComparison			theResult;



	// Get the state we need
	theResult = kNCompareLessThan;



	// Compare equal types
	if (mType == theValue.mType)
		{
		switch (mType) {
			case kNNumberInteger:
				theResult = GetComparison(mValue.integer, theValue.mValue.integer);
				break;
			
			case kNNumberFloat32:
			case kNNumberFloat64:
				if (NMathUtilities::AreEqual( mValue.real, theValue.mValue.real))
					theResult = kNCompareEqualTo;
				else
					theResult = GetComparison(mValue.real, theValue.mValue.real);
				break;

			default:
				NN_LOG("Unknown number type: %d", mType);
				break;
			}
		}
	
	
	// Compare dis-similar types
	//
	// Floating point comparisons are truncated to the smallest precision of the two.
	else
		{
		switch (mType) {
			case kNNumberInteger:
				if (theValue.GetSInt64(valueInteger))
					theResult = GetComparison(mValue.integer, valueInteger);
				else
					NN_LOG("Unable to compare numbers across types");
				break;
			
			case kNNumberFloat32:
			case kNNumberFloat64:
				if (mType == kNNumberFloat32 || theValue.mType == kNNumberFloat32)
					{
					valueFloat32_1 = (Float32)          mValue.real;
					valueFloat32_2 = (Float32) theValue.mValue.real;

					if (NMathUtilities::AreEqual(valueFloat32_1, valueFloat32_2))
						theResult = kNCompareEqualTo;
					else
						theResult = GetComparison(valueFloat32_1, valueFloat32_2);
					}
				else
					{
					valueFloat64_1 =          mValue.real;
					valueFloat64_2 = theValue.mValue.real;

					if (NMathUtilities::AreEqual(valueFloat64_1, valueFloat64_2))
						theResult = kNCompareEqualTo;
					else
						theResult = GetComparison(valueFloat64_1, valueFloat64_2);
					}
				break;

			default:
				NN_LOG("Unknown number type: %d", mType);
				break;
			}
		}

	return(theResult);
}





//============================================================================
//		NNumber::GetUInt8 : Get a UInt8 value.
//----------------------------------------------------------------------------
UInt8 NNumber::GetUInt8(void) const
{	UInt8		theValue;
	bool		canCast;



	// Get the value
	canCast = GetUInt8(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetUInt16 : Get a UInt16 value.
//----------------------------------------------------------------------------
UInt16 NNumber::GetUInt16(void) const
{	UInt16		theValue;
	bool		canCast;



	// Get the value
	canCast = GetUInt16(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetUInt32 : Get a UInt32 value.
//----------------------------------------------------------------------------
UInt32 NNumber::GetUInt32(void) const
{	UInt32		theValue;
	bool		canCast;



	// Get the value
	canCast = GetUInt32(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetUInt64 : Get a UInt64 value.
//----------------------------------------------------------------------------
UInt64 NNumber::GetUInt64(void) const
{	UInt64		theValue;
	bool		canCast;



	// Get the value
	canCast = GetUInt64(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetSInt8 : Get a SInt8 value.
//----------------------------------------------------------------------------
SInt8 NNumber::GetSInt8(void) const
{	SInt8		theValue;
	bool		canCast;



	// Get the value
	canCast = GetSInt8(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetSInt16 : Get a SInt16 value.
//----------------------------------------------------------------------------
SInt16 NNumber::GetSInt16(void) const
{	SInt16		theValue;
	bool		canCast;



	// Get the value
	canCast = GetSInt16(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetSInt32 : Get a SInt32 value.
//----------------------------------------------------------------------------
SInt32 NNumber::GetSInt32(void) const
{	SInt32		theValue;
	bool		canCast;



	// Get the value
	canCast = GetSInt32(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetSInt64 : Get a SInt64 value.
//----------------------------------------------------------------------------
SInt64 NNumber::GetSInt64(void) const
{	SInt64		theValue;
	bool		canCast;



	// Get the value
	canCast = GetSInt64(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NNumber::GetFloat32(void) const
{	Float32		theValue;
	bool		canCast;



	// Get the value
	canCast = GetFloat32(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NNumber::GetFloat64(void) const
{	Float64		theValue;
	bool		canCast;



	// Get the value
	canCast = GetFloat64(theValue);
	NN_ASSERT(canCast);
	
	return(theValue);
}





//============================================================================
//		NNumber::GetUInt8 : Get a UInt8 value.
//----------------------------------------------------------------------------
bool NNumber::GetUInt8(UInt8 &theValue) const
{	SInt8		valueInteger;
	bool		canCast;



	// Get the value
	theValue = 0;
	canCast  = GetSInt8(valueInteger);

	if (canCast)
		theValue = (UInt8) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetUInt16 : Get a UInt16 value.
//----------------------------------------------------------------------------
bool NNumber::GetUInt16(UInt16 &theValue) const
{	SInt16		valueInteger;
	bool		canCast;



	// Get the value
	theValue = 0;
	canCast  = GetSInt16(valueInteger);

	if (canCast)
		theValue = (UInt16) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetUInt32 : Get a UInt32 value.
//----------------------------------------------------------------------------
bool NNumber::GetUInt32(UInt32 &theValue) const
{	SInt32		valueInteger;
	bool		canCast;



	// Get the value
	theValue = 0;
	canCast  = GetSInt32(valueInteger);

	if (canCast)
		theValue = (UInt32) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetUInt64 : Get a UInt64 value.
//----------------------------------------------------------------------------
bool NNumber::GetUInt64(UInt64 &theValue) const
{	SInt64		valueInteger;
	bool		canCast;



	// Get the value
	valueInteger = 0;
	theValue     = 0;
	canCast      = GetSInt64(valueInteger);

	if (canCast)
		theValue = (UInt64) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetSInt8 : Get an SInt8 value.
//----------------------------------------------------------------------------
bool NNumber::GetSInt8(SInt8 &theValue) const
{	SInt64		valueInteger;
	bool		canCast;



	// Get the value
	valueInteger = 0;
	theValue     = 0;
	canCast      = GetSInt64(valueInteger);

	if (canCast && valueInteger >= kSInt8Min && valueInteger <= kSInt8Max)
		theValue = (SInt8) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetSInt16 : Get an SInt16 value.
//----------------------------------------------------------------------------
bool NNumber::GetSInt16(SInt16 &theValue) const
{	SInt64		valueInteger;
	bool		canCast;



	// Get the value
	valueInteger = 0;
	theValue     = 0;
	canCast      = GetSInt64(valueInteger);

	if (canCast && valueInteger >= kSInt16Min && valueInteger <= kSInt16Max)
		theValue = (SInt16) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetSInt32 : Get an SInt32 value.
//----------------------------------------------------------------------------
bool NNumber::GetSInt32(SInt32 &theValue) const
{	SInt64		valueInteger;
	bool		canCast;



	// Get the value
	valueInteger = 0;
	theValue     = 0;
	canCast      = GetSInt64(valueInteger);

	if (canCast && valueInteger >= kSInt32Min && valueInteger <= kSInt32Max)
		theValue = (SInt32) valueInteger;

	return(canCast);
}





//============================================================================
//		NNumber::GetSInt64 : Get an SInt64 value.
//----------------------------------------------------------------------------
bool NNumber::GetSInt64(SInt64 &theValue) const
{	bool	canCast;



	// Get the value
	switch (mType) {
		case kNNumberInteger:
			canCast  = true;
			theValue = mValue.integer;
			break;

		case kNNumberFloat32:
			canCast = NMathUtilities::AreEqual(floorf(mValue.real), (Float32) mValue.real);
			if (canCast && mValue.real >= kFloat32Min && mValue.real <= kFloat32Max)
				theValue = (SInt64) mValue.real;
			break;

		case kNNumberFloat64:
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
//		NNumber::GetFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
bool NNumber::GetFloat32(Float32 &theValue) const
{	Float64		valueReal;
	bool		canCast;



	// Get the value
	valueReal = 0.0;
	theValue  = 0;

	if (mType == kNNumberFloat32)
		{
		canCast  = true;
		theValue = (Float32) mValue.real;
		}
	else
		{
		canCast = GetFloat64(valueReal);
		if (canCast && valueReal >= kFloat32Min && valueReal <= kFloat32Max)
			theValue = (Float32) valueReal;
		}
		
	return(canCast);
}





//============================================================================
//		NNumber::GetFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
bool NNumber::GetFloat64(Float64 &theValue) const
{	bool	canCast;



	// Get the value
	switch (mType) {
		case kNNumberInteger:
			canCast = (mValue.integer >= kFloat64Min && mValue.integer <= kFloat64Max);
			if (canCast)
				theValue = (Float64) mValue.integer;
			break;

		case kNNumberFloat32:
		case kNNumberFloat64:
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
//		NNumber::SetUInt8 : Set a UInt8 value.
//----------------------------------------------------------------------------
void NNumber::SetUInt8(UInt8 theValue)
{


	// Set the value
	SetSInt64(theValue);
}





//============================================================================
//		NNumber::SetUInt16 : Set a UInt16 value.
//----------------------------------------------------------------------------
void NNumber::SetUInt16(UInt16 theValue)
{


	// Set the value
	SetSInt64(theValue);
}





//============================================================================
//		NNumber::SetUInt32 : Set a UInt32 value.
//----------------------------------------------------------------------------
void NNumber::SetUInt32(UInt32 theValue)
{


	// Set the value
	SetSInt64(theValue);
}





//============================================================================
//		NNumber::SetUInt64 : Set a UInt64 value.
//----------------------------------------------------------------------------
void NNumber::SetUInt64(UInt64 theValue)
{


	// Set the value
	SetSInt64((SInt64) theValue);
}





//============================================================================
//		NNumber::SetSInt8 : Set a SInt8 value.
//----------------------------------------------------------------------------
void NNumber::SetSInt8(SInt8 theValue)
{


	// Set the value
	SetSInt64(theValue);
}





//============================================================================
//		NNumber::SetSInt16 : Set a SInt16 value.
//----------------------------------------------------------------------------
void NNumber::SetSInt16(SInt16 theValue)
{


	// Set the value
	SetSInt64(theValue);
}





//============================================================================
//		NNumber::SetSInt32 : Set a SInt32 value.
//----------------------------------------------------------------------------
void NNumber::SetSInt32(SInt32 theValue)
{


	// Set the value
	SetSInt64(theValue);
}





//============================================================================
//		NNumber::SetSInt64 : Set a SInt64 value.
//----------------------------------------------------------------------------
void NNumber::SetSInt64(SInt64 theValue)
{


	// Set the value
	mType          = kNNumberInteger;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetFloat32 : Set a Float32 value.
//----------------------------------------------------------------------------
void NNumber::SetFloat32(Float32 theValue)
{


	// Set the value
	mType       = kNNumberFloat32;
	mValue.real = theValue;
}





//============================================================================
//		NNumber::SetFloat64 : Set a Float64 value.
//----------------------------------------------------------------------------
void NNumber::SetFloat64(Float64 theValue)
{


	// Set the value
	mType       = kNNumberFloat64;
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
{	SInt64		valueInteger;
	Float64		valueReal;



	// Parse the value
	//
	// Some integers will also pass parsing as floats, however we coerce these
	// back to integers when possible to allow us to use more tightly packed
	// types for storage in the future.
	if (sscanf(theValue.GetUTF8(), "%lf", &valueReal) == 1)
		{
		SetFloat64(valueReal);

		if (GetSInt64(valueInteger))
			SetSInt64(valueInteger);

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
