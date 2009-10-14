/*	NAME:
		NDictionary.cpp

	DESCRIPTION:
		CFDictionaryRef object.
	
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





//============================================================================
//		NDictionary::NDictionary : Constructor.
//----------------------------------------------------------------------------
NDictionary::NDictionary(CFDictionaryRef cfDictionary)
{


	// Initialize ourselves
	//
	// We will release the object when we initialize ourselves, so
	// we retain it first to avoid releasing our caller's reference.
	CFSafeRetain(cfDictionary);
	
	InitializeSelf(cfDictionary);
}





//============================================================================
//		NDictionary::NDictionary : Constructor.
//----------------------------------------------------------------------------
NDictionary::NDictionary(void)
{


	// Initialize ourselves
	InitializeSelf(NULL);
}





//============================================================================
//		NDictionary::~NDictionary : Destructor.
//----------------------------------------------------------------------------
NDictionary::~NDictionary(void)
{
}





//============================================================================
//		NDictionary::IsEmpty : Is the dictionary empty?
//----------------------------------------------------------------------------
bool NDictionary::IsEmpty(void) const
{


	// Check our size
	return(GetSize() == 0);
}





//============================================================================
//		NDictionary::IsNotEmpty : Is the dictionary not empty?
//----------------------------------------------------------------------------
bool NDictionary::IsNotEmpty(void) const
{


	// Check our size
	return(GetSize() != 0);
}





//============================================================================
//		NDictionary::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NDictionary::GetSize(void) const
{


	// Get our size
	return(CFDictionaryGetCount(*this));
}





//============================================================================
//		NDictionary::Clear : Clear the dictionary.
//----------------------------------------------------------------------------
void NDictionary::Clear(void)
{


	// Clear the dictionary
	CFDictionaryRemoveAllValues(*this);
}





//============================================================================
//		NDictionary::Join : Join two dictionaries.
//----------------------------------------------------------------------------
void NDictionary::Join(const NDictionary &theValue)
{


	// Join the dictionaries
	CFDictionaryApplyFunction(theValue, JoinCallback, this);
}





//============================================================================
//		NDictionary::HasKey : Does a key exist?
//----------------------------------------------------------------------------
bool NDictionary::HasKey(const NString &theKey) const
{


	// Check the dictionary
	return(CFDictionaryContainsKey(*this, (CFTypeRef) theKey));
}





//============================================================================
//		NDictionary::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NDictionary::RemoveKey(const NString &theKey)
{


	// Remove the value
	CFDictionaryRemoveValue(*this, (CFTypeRef) theKey);
}





//============================================================================
//		NDictionary::GetValueBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NDictionary::GetValueBoolean(const NString &theKey) const
{	bool			theValue;
	CFBooleanRef	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue = false;
	cfValue  = (CFBooleanRef) GetValue(theKey);

	if (cfValue != NULL)
		theValue = (bool) CFBooleanGetValue(cfValue);
	
	return(theValue);
}





//============================================================================
//		NDictionary::GetValueOSType : Get an OSType value.
//----------------------------------------------------------------------------
OSType NDictionary::GetValueOSType(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetOSType(GetValueString(theKey)));
}





//============================================================================
//		NDictionary::GetValueUInt32 : Get a UInt32 value.
//----------------------------------------------------------------------------
UInt32 NDictionary::GetValueUInt32(const NString &theKey) const
{	UInt32			theValue;
	CFNumberRef		cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue = 0;
	cfValue  = (CFNumberRef) GetValue(theKey);

	if (cfValue != NULL)
		CFNumberGetValue(cfValue, kCFNumberSInt32Type, &theValue);
	
	return(theValue);
}





//============================================================================
//		NDictionary::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NDictionary::GetValueFloat64(const NString &theKey) const
{	Float64			theValue;
	CFNumberRef		cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue = 0.0;
	cfValue  = (CFNumberRef) GetValue(theKey);

	if (cfValue != NULL)
		CFNumberGetValue(cfValue, kCFNumberFloat64Type, &theValue);
	
	return(theValue);
}





//============================================================================
//		NDictionary::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NDictionary::GetValueString(const NString &theKey) const
{	NString		theValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue.Set(GetValue(theKey), false);
	
	return(theValue);
}





//============================================================================
//		NDictionary::GetValuePoint : Get a point value.
//----------------------------------------------------------------------------
HIPoint NDictionary::GetValuePoint(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetPoint(GetValueString(theKey)));
}





//============================================================================
//		NDictionary::GetValueSize : Get a size value.
//----------------------------------------------------------------------------
HISize NDictionary::GetValueSize(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetSize(GetValueString(theKey)));
}





//============================================================================
//		NDictionary::GetValueRect : Get a rectangle value.
//----------------------------------------------------------------------------
HIRect NDictionary::GetValueRect(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetRect(GetValueString(theKey)));
}





//============================================================================
//		NDictionary::GetValueColor : Get a color value.
//----------------------------------------------------------------------------
NColor NDictionary::GetValueColor(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetColor(GetValueString(theKey)));
}





//============================================================================
//		NDictionary::GetValueDate : Get a date value.
//----------------------------------------------------------------------------
NCFDate NDictionary::GetValueDate(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetDate(GetValueString(theKey)));
}





//============================================================================
//		NDictionary::GetValueArray : Get an array value.
//----------------------------------------------------------------------------
NArray NDictionary::GetValueArray(const NString &theKey) const
{	NArray		theValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue.Set(GetValue(theKey), false);
	
	return(theValue);
}





//============================================================================
//		NDictionary::GetValueDictionary : Get an dictionary value.
//----------------------------------------------------------------------------
NDictionary NDictionary::GetValueDictionary(const NString &theKey) const
{	NDictionary		theValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue.Set(GetValue(theKey), false);
	
	return(theValue);
}





//============================================================================
//		NDictionary::GetValueData : Get a data value.
//----------------------------------------------------------------------------
NData NDictionary::GetValueData(const NString &theKey) const
{	NData	theValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue.Set(GetValue(theKey), false);
	
	return(theValue);
}





//============================================================================
//		NDictionary::SetValueBoolean : Set a boolean value.
//----------------------------------------------------------------------------
void NDictionary::SetValueBoolean(const NString &theKey, bool theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetValue(theKey, theValue ? kCFBooleanTrue : kCFBooleanFalse);
}





//============================================================================
//		NDictionary::SetValueOSType : Set an OSType value.
//----------------------------------------------------------------------------
void NDictionary::SetValueOSType(const NString &theKey, OSType theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromOSType(theValue));
}





//============================================================================
//		NDictionary::SetValueUInt32 : Set a UInt32 value.
//----------------------------------------------------------------------------
void NDictionary::SetValueUInt32(const NString &theKey, UInt32 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	if (cfValue.Set(CFNumberCreate(NULL, kCFNumberSInt32Type, &theValue)))
		SetValue(theKey, cfValue);
}





//============================================================================
//		NDictionary::SetValueFloat64 : Set a Float64 value.
//----------------------------------------------------------------------------
void NDictionary::SetValueFloat64(const NString &theKey, Float64 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	if (cfValue.Set(CFNumberCreate(NULL, kCFNumberFloat64Type, &theValue)))
		SetValue(theKey, cfValue);
}





//============================================================================
//		NDictionary::SetValueString : Set a string value.
//----------------------------------------------------------------------------
void NDictionary::SetValueString(const NString &theKey, const NString &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetValue(theKey, theValue);
}





//============================================================================
//		NDictionary::SetValuePoint : Set a point value.
//----------------------------------------------------------------------------
void NDictionary::SetValuePoint(const NString &theKey, const HIPoint &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromPoint(theValue));
}





//============================================================================
//		NDictionary::SetValueSize : Set a size value.
//----------------------------------------------------------------------------
void NDictionary::SetValueSize(const NString &theKey, const HISize &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromSize(theValue));
}





//============================================================================
//		NDictionary::SetValueRect : Set a rectangle value.
//----------------------------------------------------------------------------
void NDictionary::SetValueRect(const NString &theKey, const HIRect &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromRect(theValue));
}





//============================================================================
//		NDictionary::SetValueColor : Set a color value.
//----------------------------------------------------------------------------
void NDictionary::SetValueColor(const NString &theKey, const NColor &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromColor(theValue));
}





//============================================================================
//		NDictionary::SetValueDate : Set a date value.
//----------------------------------------------------------------------------
void NDictionary::SetValueDate(const NString &theKey, const NCFDate &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromDate(theValue));
}





//============================================================================
//		NDictionary::SetValueArray : Set an array value.
//----------------------------------------------------------------------------
void NDictionary::SetValueArray(const NString &theKey, const NArray &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetValue(theKey, theValue);
}





//============================================================================
//		NDictionary::SetValueDictionary : Set a dictionary value.
//----------------------------------------------------------------------------
void NDictionary::SetValueDictionary(const NString &theKey, const NDictionary &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetValue(theKey, theValue);
}





//============================================================================
//		NDictionary::SetValueData : Set a data value.
//----------------------------------------------------------------------------
void NDictionary::SetValueData(const NString &theKey, const NData &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetValue(theKey, theValue);
}





//============================================================================
//		NDictionary::operator [] : Subscript operator.
//----------------------------------------------------------------------------
CFTypeRef NDictionary::operator [](const NString &theKey) const
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	if (!HasKey(theKey))
		return(NULL);

	return(GetValue(theKey));
}





//============================================================================
//		NDictionary::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NDictionary::GetNullValue(void)
{


	// Get the value
	return(CFDictionaryCreateMutable(NULL, 0, &kCFCopyStringDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks));
}





//============================================================================
//		NDictionary::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NDictionary::InitializeSelf(CFDictionaryRef cfDictionary)
{


	// Initialize ourselves
	SetMode(kCFDuplicate);
	
	Set(cfDictionary);



	// Validate our state
	NN_ASSERT(IsValid());
}





//============================================================================
//		NDictionary::GetValue : Get a value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NDictionary::GetValue(const NString &theKey) const
{	CFTypeRef	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());
	


	// Get the value
	cfValue = CFDictionaryGetValue(*this, (CFTypeRef) theKey);
	
	return(cfValue);
}





//============================================================================
//		NDictionary::SetValue : Set a value.
//----------------------------------------------------------------------------
void NDictionary::SetValue(const NString &theKey, CFTypeRef cfValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	CFDictionarySetValue(*this, (CFTypeRef) theKey, cfValue);
}





//============================================================================
//		NDictionary::JoinCallback : Join callback.
//----------------------------------------------------------------------------
void NDictionary::JoinCallback(const void *theKey, const void *theValue, void *theContext)
{	NDictionary		*thisPtr = (NDictionary *) theContext;



	// Add the value
	thisPtr->SetValue((CFStringRef) theKey, (CFTypeRef) theValue);
}

