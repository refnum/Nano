/*	NAME:
		NNumber.cpp

	DESCRIPTION:
		Number object.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
// NEncodable
//
// NNumber uses an explicit NEncodable implementation rather than
// NENCODABLE_DEFINE, to allow it to register for POD numeric types.
//
// This allows NEncoder to encode NVariants that contain POD numeric
// types just as if they were an NVariant that held an NNumber object.
//
// On decoding, these POD values will then be extracted as NNumbers.
bool NNumber::sEncodableRegistered = NNumber::EncodableRegister();

bool NNumber::EncodableRegister(void)
{	static bool				sIsRegistered;

	NEncoderClassInfo		classInfo;
	
	if (!sIsRegistered)
		{
		classInfo.canEncode    = BindFunction(NNumber::EncodableCanEncode,    _1);
		classInfo.encodeObject = BindFunction(NNumber::EncodableEncodeObject, _1, _2);
		classInfo.decodeObject = BindFunction(NNumber::EncodableDecodeObject, _1);
		
		NEncoder::RegisterClass("NNumber", classInfo);
		sIsRegistered = true;
		}
		
	return(sIsRegistered);
}

bool NNumber::EncodableCanEncode(const NVariant &theValue)
{	const NNumber	*theObject;

	if (theValue.IsNumeric())
		return(true);

	theObject = theValue.GetValue<NNumber>();
	return(theObject != NULL);
}

void NNumber::EncodableEncodeObject(NEncoder &theEncoder, const NVariant &theValue)
{	const NNumber		*theObject;
	const NVariant		*srcValue;
	NNumber				tmpNumber;
	NVariant			tmpValue;

	if (theValue.IsNumeric())
		{
		tmpNumber = NNumber(theValue);
		tmpValue  = NVariant(tmpNumber);
		srcValue  = &tmpValue;
		}
	else
		srcValue = &theValue;
		
	theObject = srcValue->GetValue<NNumber>();
	theObject->EncodeSelf(theEncoder);
}

NVariant NNumber::EncodableDecodeObject(const NEncoder &theEncoder)
{	NNumber		theObject;

	theObject.DecodeSelf(theEncoder);
	return(theObject);
}





#pragma mark NNumber
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
NNumber::NNumber(int64_t theValue)
{


	// Initialise ourselves
	SetInt64(theValue);
}





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(float32_t theValue)
{


	// Initialise ourselves
	SetFloat32(theValue);
}





//============================================================================
//		NNumber::NNumber : Constructor.
//----------------------------------------------------------------------------
NNumber::NNumber(float64_t theValue)
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
					valueText.Format(kFormatFloat32, (float32_t) mValue.real);
				else
					valueText.Format(kFormatFloat64,             mValue.real);
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
{	float64_t			valueFloat64_1, valueFloat64_2;
	float32_t			valueFloat32_1, valueFloat32_2;
	int64_t				valueInt64_1,  valueInt64_2;
	NComparison			theResult;



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
				valueFloat32_1 = (float32_t)          mValue.real;
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
		switch (mPrecision) {
			case kNPrecisionInt8:
			case kNPrecisionInt16:
			case kNPrecisionInt32:
			case kNPrecisionInt64:
				valueInt64_1 =          GetInt64();
				valueInt64_2 = theValue.GetInt64();
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
				theResult = kNCompareLessThan;
				break;
			}
		}

	return(theResult);
}





//============================================================================
//		NNumber::GetUInt8 : Get a uint8_t value.
//----------------------------------------------------------------------------
uint8_t NNumber::GetUInt8(void) const
{


	// Get the value
	return((uint8_t) GetInt64());
}





//============================================================================
//		NNumber::GetUInt16 : Get a uint16_t value.
//----------------------------------------------------------------------------
uint16_t NNumber::GetUInt16(void) const
{


	// Get the value
	return((uint16_t) GetInt64());
}





//============================================================================
//		NNumber::GetUInt32 : Get a uint32_t value.
//----------------------------------------------------------------------------
uint32_t NNumber::GetUInt32(void) const
{


	// Get the value
	return((uint32_t) GetInt64());
}





//============================================================================
//		NNumber::GetUInt64 : Get a uint64_t value.
//----------------------------------------------------------------------------
uint64_t NNumber::GetUInt64(void) const
{


	// Get the value
	return((uint64_t) GetInt64());
}





//============================================================================
//		NNumber::GetInt8 : Get an int8_t value.
//----------------------------------------------------------------------------
int8_t NNumber::GetInt8(void) const
{


	// Get the value
	return((int8_t) GetInt64());
}





//============================================================================
//		NNumber::GetInt16 : Get an int16_t value.
//----------------------------------------------------------------------------
int16_t NNumber::GetInt16(void) const
{


	// Get the value
	return((int16_t) GetInt64());
}





//============================================================================
//		NNumber::GetInt32 : Get an int32_t value.
//----------------------------------------------------------------------------
int32_t NNumber::GetInt32(void) const
{


	// Get the value
	return((int32_t) GetInt64());
}





//============================================================================
//		NNumber::GetInt64 : Get an int64_t value.
//----------------------------------------------------------------------------
int64_t NNumber::GetInt64(void) const
{	int64_t		theValue;



	// Get the value
	if (IsInteger())
		theValue = (int64_t) mValue.integer;
	else
		theValue = (int64_t) mValue.real;
	
	return(theValue);
}





//============================================================================
//		NNumber::GetFloat32 : Get a float32_t value.
//----------------------------------------------------------------------------
float32_t NNumber::GetFloat32(void) const
{	float32_t		theValue;



	// Get the value
	if (IsInteger())
		theValue = (float32_t) mValue.integer;
	else
		theValue = (float32_t) mValue.real;
	
	return(theValue);
}





//============================================================================
//		NNumber::GetFloat64 : Get a float64_t value.
//----------------------------------------------------------------------------
float64_t NNumber::GetFloat64(void) const
{	float64_t		theValue;



	// Get the value
	if (IsInteger())
		theValue = (float64_t) mValue.integer;
	else
		theValue = (float64_t) mValue.real;
	
	return(theValue);
}





//============================================================================
//		NNumber::SetUInt8 : Set a uint8_t value.
//----------------------------------------------------------------------------
void NNumber::SetUInt8(uint8_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt8;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetUInt16 : Set a uint16_t value.
//----------------------------------------------------------------------------
void NNumber::SetUInt16(uint16_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt16;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetUInt32 : Set a uint32_t value.
//----------------------------------------------------------------------------
void NNumber::SetUInt32(uint32_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt32;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetUInt64 : Set a uint64_t value.
//----------------------------------------------------------------------------
void NNumber::SetUInt64(uint64_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt64;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetInt8 : Set an int8_t value.
//----------------------------------------------------------------------------
void NNumber::SetInt8(int8_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt8;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetInt16 : Set an int16_t value.
//----------------------------------------------------------------------------
void NNumber::SetInt16(int16_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt16;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetInt32 : Set an int32_t value.
//----------------------------------------------------------------------------
void NNumber::SetInt32(int32_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt32;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetInt64 : Set an int64_t value.
//----------------------------------------------------------------------------
void NNumber::SetInt64(int64_t theValue)
{


	// Set the value
	mPrecision     = kNPrecisionInt64;
	mValue.integer = theValue;
}





//============================================================================
//		NNumber::SetFloat32 : Set a float32_t value.
//----------------------------------------------------------------------------
void NNumber::SetFloat32(float32_t theValue)
{


	// Set the value
	mPrecision  = kNPrecisionFloat32;
	mValue.real = theValue;
}





//============================================================================
//		NNumber::SetFloat64 : Set a float64_t value.
//----------------------------------------------------------------------------
void NNumber::SetFloat64(float64_t theValue)
{


	// Set the value
	mPrecision  = kNPrecisionFloat64;
	mValue.real = theValue;
}





//============================================================================
//		NNumber::SetValue : Set the value.
//----------------------------------------------------------------------------
bool NNumber::SetValue(const NVariant &theValue)
{	uint8_t					valueUInt8;
	uint16_t				valueUInt16;
	uint32_t				valueUInt32;
	uint64_t				valueUInt64;
	int8_t					valueSInt8;
	int16_t					valueSInt16;
	int32_t					valueSInt32;
	int64_t					valueSInt64;
	float32_t				valueFloat32;
	float64_t				valueFloat64;
	int						valueInt;
	long					valueLong;
	NString					valueString;



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
		SetInt8(valueSInt8);
	
	else if (theValue.GetValue(valueSInt16))
		SetInt16(valueSInt16);
	
	else if (theValue.GetValue(valueSInt32))
		SetInt32(valueSInt32);
	
	else if (theValue.GetValue(valueSInt64))
		SetInt64(valueSInt64);
	
	else if (theValue.GetValue(valueFloat32))
		SetFloat32(valueFloat32);
	
	else if (theValue.GetValue(valueFloat64))
		SetFloat64(valueFloat64);
	
	else if (theValue.GetValue(valueInt))
		SetInt64(valueInt);
	
	else if (theValue.GetValue(valueLong))
		SetInt64(valueLong);
	
	else if (theValue.GetValue(valueString))
		return(SetValue(valueString));

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
	int64_t		valueInteger;
	float64_t	valueReal;



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
			thePrecision = kDecimalsFloat64;
		else
			thePrecision = theValue.GetSize() - foundDot.GetNext();



		// Cast the value
		if (foundDot.IsEmpty() && foundE.IsEmpty() && valueReal >= kInt64Min  && valueReal <= kInt64Max)
			SetInt64((int64_t) valueReal);

		else if (thePrecision <= kDecimalsFloat32  && valueReal >= kFloat32Min && valueReal <= kFloat32Max)
			SetFloat32((float32_t) valueReal);

		else
			SetFloat64(valueReal);

		return(true);
		}

	else if (sscanf(theValue.GetUTF8(), "%lld",   &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "%llx",   &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "0x%llx", &valueInteger) == 1 ||
			 sscanf(theValue.GetUTF8(), "0X%llx", &valueInteger) == 1)
		{
		SetInt64(valueInteger);
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





#pragma mark protected
//============================================================================
//      NNumber::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
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
