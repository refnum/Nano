/*	NAME:
		NPropertyStore.cpp

	DESCRIPTION:
		A property store is repository for key/value pairs.
		
		It supports notifications when a value is changed, default values that
		will be returned in leu of missing values, and batch access.
		
		The default store will hold values in an NDictionary, however this can
		be overriden to use alternative storage.

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
#include "NPropertyStore.h"





//============================================================================
//		NPropertyStore::NPropertyStore : Constructor.
//----------------------------------------------------------------------------
NPropertyStore::NPropertyStore(void)
{
}





//============================================================================
//		NPropertyStore::~NPropertyStore : Destructor.
//----------------------------------------------------------------------------
NPropertyStore::~NPropertyStore(void)
{
}





//============================================================================
//		NPropertyStore::HasKey : Does a key exist?
//----------------------------------------------------------------------------
bool NPropertyStore::HasKey(const NString &theKey, bool checkDefaults)
{	bool	hasKey;



	// Check our state
	hasKey = GetPropertyValue(theKey).IsValid();
	
	if (!hasKey && checkDefaults)
		hasKey = mDefaults.HasKey(theKey);
	
	return(hasKey);
}





//============================================================================
//		NPropertyStore::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NPropertyStore::RemoveKey(const NString &theKey)
{


	// Remove the key
	RemovePropertyKey(theKey);
	mDefaults.RemoveKey(theKey);

	BroadcastMessage(kMsgPropertyValueChanged, (void *) &theKey);
}





//============================================================================
//		NPropertyStore::GetValueBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NPropertyStore::GetValueBoolean(const NString &theKey) const
{	bool			theValue;
	NCFObject		cfValue;



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
//		NPropertyStore::GetValueOSType : Get an OSType value.
//----------------------------------------------------------------------------
OSType NPropertyStore::GetValueOSType(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetOSType(GetValueString(theKey)));
}





//============================================================================
//		NPropertyStore::GetValueUInt32 : Get a UInt32 value.
//----------------------------------------------------------------------------
UInt32 NPropertyStore::GetValueUInt32(const NString &theKey) const
{	UInt32			theValue;
	NCFObject		cfValue;



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
//		NPropertyStore::GetValueUInt64 : Get a UInt64 value.
//----------------------------------------------------------------------------
UInt64 NPropertyStore::GetValueUInt64(const NString &theKey) const
{	UInt64			theValue;
	NCFObject		cfValue;



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
//		NPropertyStore::GetValueFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NPropertyStore::GetValueFloat32(const NString &theKey) const
{	Float32			theValue;
	NCFObject		cfValue;



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
//		NPropertyStore::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NPropertyStore::GetValueFloat64(const NString &theKey) const
{	Float64			theValue;
	NCFObject		cfValue;



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
//		NPropertyStore::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NPropertyStore::GetValueString(const NString &theKey) const
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	return(NString(GetValue(theKey)));
}





//============================================================================
//		NPropertyStore::GetValuePoint : Get a point value.
//----------------------------------------------------------------------------
HIPoint NPropertyStore::GetValuePoint(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetPoint(GetValueString(theKey)));
}





//============================================================================
//		NPropertyStore::GetValueSize : Get a size value.
//----------------------------------------------------------------------------
HISize NPropertyStore::GetValueSize(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetSize(GetValueString(theKey)));
}





//============================================================================
//		NPropertyStore::GetValueRect : Get a rectangle value.
//----------------------------------------------------------------------------
HIRect NPropertyStore::GetValueRect(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetRect(GetValueString(theKey)));
}





//============================================================================
//		NPropertyStore::GetValueColor : Get a color value.
//----------------------------------------------------------------------------
NColor NPropertyStore::GetValueColor(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetColor(GetValueString(theKey)));
}





//============================================================================
//		NPropertyStore::GetValueAlias : Get an alias value.
//----------------------------------------------------------------------------
NAlias NPropertyStore::GetValueAlias(const NString &theKey) const
{	NAlias	theValue;



	// Get the value
	theValue.SetData(GetValueData(theKey));
	
	return(theValue);
}





//============================================================================
//		NPropertyStore::GetValueDate : Get a date value.
//----------------------------------------------------------------------------
NDate NPropertyStore::GetValueDate(const NString &theKey) const
{


	// Get the value
	return(NDate(GetValueFloat64(theKey)));
}





//============================================================================
//		NPropertyStore::GetValueArray : Get an array value.
//----------------------------------------------------------------------------
NArray NPropertyStore::GetValueArray(const NString &theKey) const
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	return(NArray((CFArrayRef) GetValue(theKey)));
}





//============================================================================
//		NPropertyStore::GetValueDictionary : Get an dictionary value.
//----------------------------------------------------------------------------
NDictionary NPropertyStore::GetValueDictionary(const NString &theKey) const
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	return(NDictionary(GetValue(theKey)));
}





//============================================================================
//		NPropertyStore::GetValueData : Get a data value.
//----------------------------------------------------------------------------
NData NPropertyStore::GetValueData(const NString &theKey) const
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	return(NData(GetValue(theKey)));
}





//============================================================================
//		NPropertyStore::SetValueBoolean : Set a boolean value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueBoolean(const NString &theKey, bool theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	cfValue.Set(theValue ? kCFBooleanTrue : kCFBooleanFalse, false);
	
	SetValue(theKey, cfValue);
}





//============================================================================
//		NPropertyStore::SetValueOSType : Set an OSType value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueOSType(const NString &theKey, OSType theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromOSType(theValue));
}





//============================================================================
//		NPropertyStore::SetValueUInt32 : Set a UInt32 value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueUInt32(const NString &theKey, UInt32 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	if (cfValue.Set(CFNumberCreate(kCFAllocatorNano, kCFNumberSInt32Type, &theValue)))
		SetValue(theKey, cfValue);
}





//============================================================================
//		NPropertyStore::SetValueUInt64 : Set a UInt64 value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueUInt64(const NString &theKey, UInt64 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	if (cfValue.Set(CFNumberCreate(kCFAllocatorNano, kCFNumberSInt64Type, &theValue)))
		SetValue(theKey, cfValue);
}





//============================================================================
//		NPropertyStore::SetValueFloat32 : Set a Float32 value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueFloat32(const NString &theKey, Float32 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	if (cfValue.Set(CFNumberCreate(kCFAllocatorNano, kCFNumberFloat32Type, &theValue)))
		SetValue(theKey, cfValue);
}





//============================================================================
//		NPropertyStore::SetValueFloat64 : Set a Float64 value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueFloat64(const NString &theKey, Float64 theValue)
{	NCFObject	cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	if (cfValue.Set(CFNumberCreate(kCFAllocatorNano, kCFNumberFloat64Type, &theValue)))
		SetValue(theKey, cfValue);
}





//============================================================================
//		NPropertyStore::SetValueString : Set a string value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueString(const NString &theKey, const NString &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetValue(theKey, theValue);
}





//============================================================================
//		NPropertyStore::SetValuePoint : Set a point value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValuePoint(const NString &theKey, const HIPoint &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromPoint(theValue));
}





//============================================================================
//		NPropertyStore::SetValueSize : Set a size value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueSize(const NString &theKey, const HISize &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromSize(theValue));
}





//============================================================================
//		NPropertyStore::SetValueRect : Set a rectangle value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueRect(const NString &theKey, const HIRect &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromRect(theValue));
}





//============================================================================
//		NPropertyStore::SetValueColor : Set a color value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueColor(const NString &theKey, const NColor &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromColor(theValue));
}





//============================================================================
//		NPropertyStore::SetValueAlias : Set an alias value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueAlias(const NString &theKey, const NAlias &theValue)
{


	// Set the value
	SetValueData(theKey, theValue.GetData());
}





//============================================================================
//		NPropertyStore::SetValueDate : Set a date value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueDate(const NString &theKey, const NDate &theValue)
{


	// Set the value
	SetValueFloat64(theKey, theValue.GetTime());
}





//============================================================================
//		NPropertyStore::SetValueArray : Set an array value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueArray(const NString &theKey, const NArray &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetValue(theKey, theValue);
}





//============================================================================
//		NPropertyStore::SetValueDictionary : Set a dictionary value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueDictionary(const NString &theKey, const NDictionary &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetValue(theKey, theValue);
}





//============================================================================
//		NPropertyStore::SetValueData : Set a data value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValueData(const NString &theKey, const NData &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetValue(theKey, theValue);
}





//============================================================================
//		NPropertyStore::GetDefaults : Get the defaults.
//----------------------------------------------------------------------------
NDictionary NPropertyStore::GetDefaults(void) const
{


	// Get the defaults
	return(mDefaults);
}





//============================================================================
//		NPropertyStore::SetDefaults : Set the defaults.
//----------------------------------------------------------------------------
void NPropertyStore::SetDefaults(const NDictionary &theDefaults)
{


	// Set the defaults
	mDefaults = theDefaults;
}





//============================================================================
//		NPropertyStore::GetProperties : Get the properties.
//----------------------------------------------------------------------------
NDictionary NPropertyStore::GetProperties(void) const
{


	// Get the properties
	return(NDictionary(GetPropertyValue(kPropertyAllValuesKey)));
}





//============================================================================
//		NPropertyStore::SetProperties : Set the properties.
//----------------------------------------------------------------------------
void NPropertyStore::SetProperties(const NDictionary &theProperties)
{


	// Set the properties
	SetPropertyValue(kPropertyAllValuesKey, theProperties);
}





//============================================================================
//		NPropertyStore::RemovePropertyKey : Remove a property key.
//----------------------------------------------------------------------------
#pragma mark -
void NPropertyStore::RemovePropertyKey(const NString &theKey)
{


	// Remove the key
	mProperties.RemoveKey(theKey);
}





//============================================================================
//		NPropertyStore::GetPropertyValue : Get a property value.
//----------------------------------------------------------------------------
NCFObject NPropertyStore::GetPropertyValue(const NString &theKey) const
{	NCFObject	cfValue;



	// Get the value
	if (theKey == kPropertyAllValuesKey)
		cfValue = mProperties;
	else
		cfValue = mProperties.GetValue(theKey);

	return(cfValue);
}





//============================================================================
//		NPropertyStore::SetPropertyValue : Set a property value.
//----------------------------------------------------------------------------
void NPropertyStore::SetPropertyValue(const NString &theKey, const NCFObject &theValue)
{


	// Set the value
	if (theKey == kPropertyAllValuesKey)
		mProperties.Set(theValue, false);
	else
		mProperties.SetValue(theKey, theValue);
}





//============================================================================
//		NPropertyStore::GetValue : Get a value.
//----------------------------------------------------------------------------
#pragma mark -
NCFObject NPropertyStore::GetValue(const NString &theKey) const
{	NCFObject	theValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	theValue = GetPropertyValue(theKey);

	if (!theValue.IsValid())
		theValue = mDefaults.GetValue(theKey);

	return(theValue);
}





//============================================================================
//		NPropertyStore::SetValue : Set a value.
//----------------------------------------------------------------------------
void NPropertyStore::SetValue(const NString &theKey, const NCFObject &theValue)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	SetPropertyValue(theKey, theValue);

	BroadcastMessage(kMsgPropertyValueChanged, (void *) &theKey);
}

