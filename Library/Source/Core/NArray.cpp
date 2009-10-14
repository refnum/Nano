/*	NAME:
		NArray.cpp

	DESCRIPTION:
		Array object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"
#include "NDictionary.h"
#include "NArray.h"





//============================================================================
//		NArray::NArray : Constructor.
//----------------------------------------------------------------------------
NArray::NArray(CFArrayRef cfArray)
{


	// Initialize ourselves
	//
	// We will release the object when we initialize ourselves, so
	// we retain it first to avoid releasing our caller's reference.
	CFSafeRetain(cfArray);
	
	InitializeSelf(cfArray);
}





//============================================================================
//		NArray::NArray : Constructor.
//----------------------------------------------------------------------------
NArray::NArray(CFTypeRef cfValue, ...)
{	va_list		argList;



	// Initialize ourselves
	InitializeSelf(NULL);



	// Add the arguments
	//
	// The NArray object can be constructed with an optional list of
	// CFTypeRefs, to be added to the newly-created array.
	va_start(argList, cfValue);
	
	while (cfValue != NULL)
		{
		AppendValue(cfValue);
		cfValue = va_arg(argList, CFTypeRef);
		}

	va_end(argList);
}





//============================================================================
//		NArray::~NArray : Destructor.
//----------------------------------------------------------------------------
NArray::~NArray(void)
{
}





//============================================================================
//		NArray::IsEmpty : Is the array empty?
//----------------------------------------------------------------------------
bool NArray::IsEmpty(void) const
{


	// Check our size
	return(GetSize() == 0);
}





//============================================================================
//		NArray::IsNotEmpty : Is the array not empty?
//----------------------------------------------------------------------------
bool NArray::IsNotEmpty(void) const
{


	// Check our size
	return(GetSize() != 0);
}





//============================================================================
//		NArray::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NArray::GetSize(void) const
{


	// Get our size
	return(CFArrayGetCount(*this));
}





//============================================================================
//		NArray::Clear : Clear the array.
//----------------------------------------------------------------------------
void NArray::Clear(void)
{


	// Clear the array
	CFArrayRemoveAllValues(*this);
}





//============================================================================
//		NArray::Join : Join two arrays.
//----------------------------------------------------------------------------
void NArray::Join(const NArray &theValue)
{


	// Join the arrays
	CFArrayAppendArray(*this, theValue, CFRangeMake(0, theValue.GetSize()));
}





//============================================================================
//		NArray::HasValue : Does a value exist?
//----------------------------------------------------------------------------
bool NArray::HasValue(CFTypeRef theValue) const
{	bool	hasValue;



	// Check our state
	hasValue = CFArrayContainsValue(*this, CFRangeMake(0, GetSize()), theValue);
	
	return(hasValue);
}





//============================================================================
//		NArray::GetValueBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NArray::GetValueBoolean(UInt32 theIndex) const
{	bool			theValue;
	CFBooleanRef	cfValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	theValue = false;
	cfValue  = (CFBooleanRef) GetValue(theIndex);

	if (cfValue != NULL)
		theValue = (bool) CFBooleanGetValue(cfValue);
	
	return(theValue);
}





//============================================================================
//		NArray::GetValueOSType : Get an OSType value.
//----------------------------------------------------------------------------
OSType NArray::GetValueOSType(UInt32 theIndex) const
{


	// Get the value
	return(NTextUtilities::GetOSType(GetValueString(theIndex)));
}





//============================================================================
//		NArray::GetValueUInt32 : Get a UInt32 value.
//----------------------------------------------------------------------------
UInt32 NArray::GetValueUInt32(UInt32 theIndex) const
{	UInt32			theValue;
	CFNumberRef		cfValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	theValue = 0;
	cfValue  = (CFNumberRef) GetValue(theIndex);

	if (cfValue != NULL)
		CFNumberGetValue(cfValue, kCFNumberSInt32Type, &theValue);
	
	return(theValue);
}





//============================================================================
//		NArray::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NArray::GetValueFloat64(UInt32 theIndex) const
{	Float64			theValue;
	CFNumberRef		cfValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	theValue = 0.0;
	cfValue  = (CFNumberRef) GetValue(theIndex);

	if (cfValue != NULL)
		CFNumberGetValue(cfValue, kCFNumberFloat64Type, &theValue);
	
	return(theValue);
}





//============================================================================
//		NArray::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NArray::GetValueString(UInt32 theIndex) const
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	theValue.Set(GetValue(theIndex), false);
	
	return(theValue);
}





//============================================================================
//		NArray::GetValuePoint : Get a point value.
//----------------------------------------------------------------------------
HIPoint NArray::GetValuePoint(UInt32 theIndex) const
{


	// Get the value
	return(NTextUtilities::GetPoint(GetValueString(theIndex)));
}





//============================================================================
//		NArray::GetValueSize : Get a size value.
//----------------------------------------------------------------------------
HISize NArray::GetValueSize(UInt32 theIndex) const
{


	// Get the value
	return(NTextUtilities::GetSize(GetValueString(theIndex)));
}





//============================================================================
//		NArray::GetValueRect : Get a rectangle value.
//----------------------------------------------------------------------------
HIRect NArray::GetValueRect(UInt32 theIndex) const
{


	// Get the value
	return(NTextUtilities::GetRect(GetValueString(theIndex)));
}





//============================================================================
//		NArray::GetValueColor : Get a color value.
//----------------------------------------------------------------------------
NColor NArray::GetValueColor(UInt32 theIndex) const
{


	// Get the value
	return(NTextUtilities::GetColor(GetValueString(theIndex)));
}





//============================================================================
//		NArray::GetValueDate : Get a date value.
//----------------------------------------------------------------------------
NCFDate NArray::GetValueDate(UInt32 theIndex) const
{


	// Get the value
	return(NTextUtilities::GetDate(GetValueString(theIndex)));
}





//============================================================================
//		NArray::GetValueArray : Get an array value.
//----------------------------------------------------------------------------
NArray NArray::GetValueArray(UInt32 theIndex) const
{	NArray		theValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	theValue.Set(GetValue(theIndex), false);
	
	return(theValue);
}





//============================================================================
//		NArray::GetValueDictionary : Get a dictionary value.
//----------------------------------------------------------------------------
NDictionary NArray::GetValueDictionary(UInt32 theIndex) const
{	NDictionary		theValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	theValue.Set(GetValue(theIndex), false);
	
	return(theValue);
}





//============================================================================
//		NArray::GetValueData : Get a data value.
//----------------------------------------------------------------------------
NData NArray::GetValueData(UInt32 theIndex) const
{	NData	theValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	theValue.Set(GetValue(theIndex), false);
	
	return(theValue);
}





//============================================================================
//		NArray::SetValueBoolean : Set a boolean value.
//----------------------------------------------------------------------------
void NArray::SetValueBoolean(UInt32 theIndex, bool theValue)
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Set the value
	SetValue(theIndex, theValue ? kCFBooleanTrue : kCFBooleanFalse);
}





//============================================================================
//		NArray::SetValueOSType : Set an OSType value.
//----------------------------------------------------------------------------
void NArray::SetValueOSType(UInt32 theIndex, OSType theValue)
{


	// Set the value
	SetValueString(theIndex, NTextUtilities::FromOSType(theValue));
}





//============================================================================
//		NArray::SetValueUInt32 : Set a UInt32 value.
//----------------------------------------------------------------------------
void NArray::SetValueUInt32(UInt32 theIndex, UInt32 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Set the value
	if (cfValue.Set(CFNumberCreate(NULL, kCFNumberSInt32Type, &theValue)))
		SetValue(theIndex, cfValue);
}





//============================================================================
//		NArray::SetValueFloat64 : Set a Float64 value.
//----------------------------------------------------------------------------
void NArray::SetValueFloat64(UInt32 theIndex, Float64 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Set the value
	if (cfValue.Set(CFNumberCreate(NULL, kCFNumberFloat64Type, &theValue)))
		SetValue(theIndex, cfValue);
}





//============================================================================
//		NArray::SetValueString : Set a string value.
//----------------------------------------------------------------------------
void NArray::SetValueString(UInt32 theIndex, const NString &theValue)
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Set the value
	SetValue(theIndex, theValue);
}





//============================================================================
//		NArray::SetValuePoint : Set a point value.
//----------------------------------------------------------------------------
void NArray::SetValuePoint(UInt32 theIndex, const HIPoint &theValue)
{


	// Set the value
	SetValueString(theIndex, NTextUtilities::FromPoint(theValue));
}





//============================================================================
//		NArray::SetValueSize : Set a size value.
//----------------------------------------------------------------------------
void NArray::SetValueSize(UInt32 theIndex, const HISize &theValue)
{


	// Set the value
	SetValueString(theIndex, NTextUtilities::FromSize(theValue));
}





//============================================================================
//		NArray::SetValueRect : Set a rectangle value.
//----------------------------------------------------------------------------
void NArray::SetValueRect(UInt32 theIndex, const HIRect &theValue)
{


	// Set the value
	SetValueString(theIndex, NTextUtilities::FromRect(theValue));
}





//============================================================================
//		NArray::SetValueColor : Set a color value.
//----------------------------------------------------------------------------
void NArray::SetValueColor(UInt32 theIndex, const NColor &theValue)
{


	// Set the value
	SetValueString(theIndex, NTextUtilities::FromColor(theValue));
}





//============================================================================
//		NArray::SetValueDate : Set a date value.
//----------------------------------------------------------------------------
void NArray::SetValueDate(UInt32 theIndex, const NCFDate &theValue)
{


	// Set the value
	SetValueString(theIndex, NTextUtilities::FromDate(theValue));
}





//============================================================================
//		NArray::SetValueArray : Set an array value.
//----------------------------------------------------------------------------
void NArray::SetValueArray(UInt32 theIndex, const NArray &theValue)
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Set the value
	SetValue(theIndex, theValue);
}





//============================================================================
//		NArray::SetValueDictionary : Set a dictionary value.
//----------------------------------------------------------------------------
void NArray::SetValueDictionary(UInt32 theIndex, const NDictionary &theValue)
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Set the value
	SetValue(theIndex, theValue);
}





//============================================================================
//		NArray::SetValueData : Set a data value.
//----------------------------------------------------------------------------
void NArray::SetValueData(UInt32 theIndex, const NData &theValue)
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Set the value
	SetValue(theIndex, theValue);
}





//============================================================================
//		NArray::AppendValueBoolean : Append a value.
//----------------------------------------------------------------------------
void NArray::AppendValueBoolean(bool theValue)
{


	// Append the value
	AppendValue(theValue ? kCFBooleanTrue : kCFBooleanFalse);
}





//============================================================================
//		NArray::AppendValueOSType : Append an OSType value.
//----------------------------------------------------------------------------
void NArray::AppendValueOSType(OSType theValue)
{


	// Append the value
	AppendValueString(NTextUtilities::FromOSType(theValue));
}





//============================================================================
//		NArray::AppendValueUInt32 : Append a UInt32 value.
//----------------------------------------------------------------------------
void NArray::AppendValueUInt32(UInt32 theValue)
{	NCFObject	cfValue;



	// Append the value
	if (cfValue.Set(CFNumberCreate(NULL, kCFNumberSInt32Type, &theValue)))
		AppendValue(cfValue);
}





//============================================================================
//		NArray::AppendValueFloat64 : Append a Float64 value.
//----------------------------------------------------------------------------
void NArray::AppendValueFloat64(Float64 theValue)
{	NCFObject	cfValue;



	// Append the value
	if (cfValue.Set(CFNumberCreate(NULL, kCFNumberSInt32Type, &theValue)))
		AppendValue(cfValue);
}





//============================================================================
//		NArray::AppendValueString : Append a string value.
//----------------------------------------------------------------------------
void NArray::AppendValueString(const NString &theValue)
{


	// Append the value
	AppendValue(theValue);
}





//============================================================================
//		NArray::AppendValuePoint : Append a point value.
//----------------------------------------------------------------------------
void NArray::AppendValuePoint(const HIPoint &theValue)
{


	// Append the value
	AppendValueString(NTextUtilities::FromPoint(theValue));
}





//============================================================================
//		NArray::AppendValueSize : Append a size value.
//----------------------------------------------------------------------------
void NArray::AppendValueSize(const HISize &theValue)
{


	// Append the value
	AppendValueString(NTextUtilities::FromSize(theValue));
}





//============================================================================
//		NArray::AppendValueRect : Append a rectangle value.
//----------------------------------------------------------------------------
void NArray::AppendValueRect(const HIRect &theValue)
{


	// Append the value
	AppendValueString(NTextUtilities::FromRect(theValue));
}





//============================================================================
//		NArray::AppendValueColor : Append a color value.
//----------------------------------------------------------------------------
void NArray::AppendValueColor(const NColor &theValue)
{


	// Append the value
	AppendValueString(NTextUtilities::FromColor(theValue));
}





//============================================================================
//		NArray::AppendValueDate : Append a date value.
//----------------------------------------------------------------------------
void NArray::AppendValueDate(const NCFDate &theValue)
{


	// Append the value
	AppendValueString(NTextUtilities::FromDate(theValue));
}





//============================================================================
//		NArray::AppendValueArray : Append an array value.
//----------------------------------------------------------------------------
void NArray::AppendValueArray(const NArray &theValue)
{


	// Append the value
	AppendValue(theValue);
}





//============================================================================
//		NArray::AppendValueDictionary : Append a dictionary value.
//----------------------------------------------------------------------------
void NArray::AppendValueDictionary(const NDictionary &theValue)
{


	// Append the value
	AppendValue(theValue);
}





//============================================================================
//		NArray::AppendValueData : Append a data value.
//----------------------------------------------------------------------------
void NArray::AppendValueData(const NData &theValue)
{


	// Append the value
	AppendValue(theValue);
}





//============================================================================
//		NArray::operator [] : Subscript operator.
//----------------------------------------------------------------------------
CFTypeRef NArray::operator [](int theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex < (int) GetSize());



	// Get the value
	return(GetValue(theIndex));
}





//============================================================================
//		NArray::operator [] : Subscript operator.
//----------------------------------------------------------------------------
CFTypeRef NArray::operator [](UInt32 theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	return(GetValue(theIndex));
}





//============================================================================
//		NArray::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NArray::GetNullValue(void)
{


	// Get the value
	return(CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks));
}





//============================================================================
//		NArray::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NArray::InitializeSelf(CFArrayRef cfArray)
{


	// Initialize ourselves
	SetMode(kCFDuplicate);
	
	Set(cfArray);



	// Validate our state
	NN_ASSERT(IsValid());
}





//============================================================================
//		NArray::GetValue : Get a value.
//----------------------------------------------------------------------------
CFTypeRef NArray::GetValue(UInt32 theIndex) const
{	CFTypeRef	cfValue;



	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Get the value
	cfValue = CFArrayGetValueAtIndex(*this, theIndex);
	
	return(cfValue);
}





//============================================================================
//		NArray::SetValue : Set a value.
//----------------------------------------------------------------------------
void NArray::SetValue(UInt32 theIndex, CFTypeRef cfValue)
{


	// Validate our parameters
	NN_ASSERT(theIndex < GetSize());



	// Set the value
	CFArraySetValueAtIndex(*this, theIndex, cfValue);
}





//============================================================================
//		NArray::AppendValue : Append a value.
//----------------------------------------------------------------------------
void NArray::AppendValue(CFTypeRef cfValue)
{


	// Append the value
	CFArrayAppendValue(*this, cfValue);
}

