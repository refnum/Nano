/*	NAME:
		NDictionary.cpp

	DESCRIPTION:
		CFDictionaryRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
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
	Set(cfDictionary, false);
	
	if (cfDictionary != NULL)
		NN_ASSERT(IsValid());
}





//============================================================================
//		NDictionary::~NDictionary : Destructor.
//----------------------------------------------------------------------------
NDictionary::~NDictionary(void)
{
}





//============================================================================
//		NDictionary::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NDictionary::GetSize(void) const
{


	// Get the size
	return(CFDictionaryGetCount(*this));
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
	if (MakeMutable())
		CFDictionaryRemoveValue(*this, (CFTypeRef) theKey);
}





//============================================================================
//		NDictionary::GetKeys : Get the keys.
//----------------------------------------------------------------------------
NStringList NDictionary::GetKeys(void) const
{	UInt32			n, numKeys;
	NStringList		theResult;
	CFStringRef		*keyPtrs;
	NData			theData;



	// Get the keys
	numKeys = GetSize();
	keyPtrs = (CFStringRef *) theData.AppendData(numKeys * sizeof(CFStringRef));
	
	if (keyPtrs != NULL)
		{
		CFDictionaryGetKeysAndValues(*this, (const void **) keyPtrs, NULL);
	
		for (n = 0; n < numKeys; n++)
			theResult.push_back(NString(keyPtrs[n]));
		}

	return(theResult);
}





//============================================================================
//		NDictionary::GetValue : Get a value.
//----------------------------------------------------------------------------
NCFObject NDictionary::GetValue(const NString &theKey) const
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());
	


	// Get the value
	cfValue.Set(CFDictionaryGetValue(*this, theKey), false);
	
	return(cfValue);
}





//============================================================================
//		NDictionary::GetValueBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NDictionary::GetValueBoolean(const NString &theKey) const
{	bool		theValue;
	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue = false;
	cfValue  = GetValue(theKey);

	if (cfValue.IsValid())
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
{	UInt32		theValue;
	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue = 0;
	cfValue  = GetValue(theKey);

	if (cfValue.IsValid())
		CFNumberGetValue(cfValue, kCFNumberSInt32Type, &theValue);
	
	return(theValue);
}





//============================================================================
//		NDictionary::GetValueUInt64 : Get a UInt64 value.
//----------------------------------------------------------------------------
UInt64 NDictionary::GetValueUInt64(const NString &theKey) const
{	UInt64		theValue;
	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue = 0;
	cfValue  = GetValue(theKey);

	if (cfValue.IsValid())
		CFNumberGetValue(cfValue, kCFNumberSInt64Type, &theValue);
	
	return(theValue);
}





//============================================================================
//		NDictionary::GetValueFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NDictionary::GetValueFloat32(const NString &theKey) const
{	Float32		theValue;
	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue = 0.0f;
	cfValue  = GetValue(theKey);

	if (cfValue.IsValid())
		CFNumberGetValue(cfValue, kCFNumberFloat32Type, &theValue);
	
	return(theValue);
}





//============================================================================
//		NDictionary::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NDictionary::GetValueFloat64(const NString &theKey) const
{	Float64		theValue;
	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue = 0.0;
	cfValue  = GetValue(theKey);

	if (cfValue.IsValid())
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
NDate NDictionary::GetValueDate(const NString &theKey) const
{


	// Get the value
	return(NDate(GetValueFloat64(theKey)));
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
//		NDictionary::SetValue : Set a value.
//----------------------------------------------------------------------------
void NDictionary::SetValue(const NString &theKey, const NCFObject &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	if (MakeMutable())
		CFDictionarySetValue(*this, theKey, theValue);
}





//============================================================================
//		NDictionary::SetValueBoolean : Set a boolean value.
//----------------------------------------------------------------------------
void NDictionary::SetValueBoolean(const NString &theKey, bool theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	cfValue.Set(theValue ? kCFBooleanTrue : kCFBooleanFalse, false);
	
	SetValue(theKey, cfValue);
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
	if (cfValue.Set(CFNumberCreate(kCFAllocatorNano, kCFNumberSInt32Type, &theValue)))
		SetValue(theKey, cfValue);
}





//============================================================================
//		NDictionary::SetValueUInt64 : Set a UInt64 value.
//----------------------------------------------------------------------------
void NDictionary::SetValueUInt64(const NString &theKey, UInt64 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	if (cfValue.Set(CFNumberCreate(kCFAllocatorNano, kCFNumberSInt64Type, &theValue)))
		SetValue(theKey, cfValue);
}





//============================================================================
//		NDictionary::SetValueFloat32 : Set a Float32 value.
//----------------------------------------------------------------------------
void NDictionary::SetValueFloat32(const NString &theKey, Float32 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	if (cfValue.Set(CFNumberCreate(kCFAllocatorNano, kCFNumberFloat32Type, &theValue)))
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
	if (cfValue.Set(CFNumberCreate(kCFAllocatorNano, kCFNumberFloat64Type, &theValue)))
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
void NDictionary::SetValueDate(const NString &theKey, const NDate &theValue)
{


	// Set the value
	SetValueFloat64(theKey, theValue.GetTime());
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
//		NDictionary::GetNullValue : Get the null value.
//----------------------------------------------------------------------------
#pragma mark -
CFTypeRef NDictionary::GetNullValue(void)
{	static CFDictionaryRef		sNullValue = CFDictionaryCreate(kCFAllocatorNano, NULL, NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);



	// Get the value
	return(sNullValue);
}





//============================================================================
//		NDictionary::JoinCallback : Join callback.
//----------------------------------------------------------------------------
#pragma mark -
void NDictionary::JoinCallback(const void *cfKey, const void *cfValue, void *theContext)
{	NDictionary		*thisPtr = (NDictionary *) theContext;
	NCFObject		theValue;
	NString			theKey;



	// Add the value
	theKey.Set(  (CFStringRef) cfKey,   false);
	theValue.Set((CFTypeRef)   cfValue, false);
	
	thisPtr->SetValue(theKey, theValue);
}


