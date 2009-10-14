/*	NAME:
		NCFPreferences.cpp

	DESCRIPTION:
		CoreFoundation preferences.
	
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
#include "NCFPreferences.h"





//============================================================================
//		NCFPreferences::NCFPreferences : Constructor.
//----------------------------------------------------------------------------
NCFPreferences::NCFPreferences(const NString &appID)
{


	// Initialize ourselves
	mAppID = appID;
	
	if (mAppID.IsEmpty())
		mAppID = kCFPreferencesCurrentApplication;
}





//============================================================================
//		NCFPreferences::~NCFPreferences : Destructor.
//----------------------------------------------------------------------------
NCFPreferences::~NCFPreferences(void)
{


	// Clean up
	Save();
}





//============================================================================
//		NCFPreferences::Save : Save the preferences.
//----------------------------------------------------------------------------
void NCFPreferences::Save(void)
{


	// Save the preferences
	CFPreferencesAppSynchronize(mAppID);
}





//============================================================================
//		NCFPreferences::HasValue : Does a preference exist?
//----------------------------------------------------------------------------
bool NCFPreferences::HasValue(const NString &theKey, bool checkDefaults)
{	bool		hasValue;
	NCFObject	cfValue;



	// Get the value
	hasValue = cfValue.Set(CFPreferencesCopyAppValue(theKey, mAppID));

	if (!hasValue && checkDefaults)
		hasValue = mDefaults.HasKey(theKey);

	return(hasValue);
}





//============================================================================
//		NCFPreferences::RemoveValue : Remove a preference.
//----------------------------------------------------------------------------
void NCFPreferences::RemoveValue(const NString &theKey)
{


	// Remove the value
	CFPreferencesSetAppValue(theKey, NULL, mAppID);
	
	DoPrefChanged(theKey);
}





//============================================================================
//		NCFPreferences::GetValueBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NCFPreferences::GetValueBoolean(const NString &theKey) const
{	Boolean		prefExists;
	bool		theResult;



	// Get the value
	theResult = CFPreferencesGetAppBooleanValue(theKey, mAppID, &prefExists);
	if (!prefExists)
		theResult = mDefaults.GetValueBoolean(theKey);
	
	return(theResult);
}





//============================================================================
//		NCFPreferences::GetValueOSType : Get an OSType value.
//----------------------------------------------------------------------------
OSType NCFPreferences::GetValueOSType(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetOSType(GetValueString(theKey)));
}





//============================================================================
//		NCFPreferences::GetValueUInt32 : Get a UInt32 value.
//----------------------------------------------------------------------------
UInt32 NCFPreferences::GetValueUInt32(const NString &theKey) const
{	Boolean		prefExists;
	UInt32		theResult;



	// Get the value
	theResult = (UInt32) CFPreferencesGetAppIntegerValue(theKey, mAppID, &prefExists);
	if (!prefExists)
		theResult = mDefaults.GetValueUInt32(theKey);
	
	return(theResult);
}





//============================================================================
//		NCFPreferences::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NCFPreferences::GetValueFloat64(const NString &theKey) const
{	Float64		theResult;
	NCFObject	cfValue;



	// Get the value
	if (cfValue.Set(CFPreferencesCopyAppValue(theKey, mAppID)))
		{
		if (!CFNumberGetValue(cfValue, kCFNumberFloat64Type, &theResult))
			theResult = mDefaults.GetValueFloat64(theKey);
		}
	else
		theResult = mDefaults.GetValueFloat64(theKey);
	
	return(theResult);
}





//============================================================================
//		NCFPreferences::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NCFPreferences::GetValueString(const NString &theKey) const
{	NString		theResult;
	NCFObject	cfValue;



	// Get the value
	if (cfValue.Set(CFPreferencesCopyAppValue(theKey, mAppID)))
		theResult = (CFStringRef) cfValue;
	else
		theResult = mDefaults.GetValueString(theKey);
	
	return(theResult);
}





//============================================================================
//		NCFPreferences::GetValuePoint : Get a point value.
//----------------------------------------------------------------------------
HIPoint NCFPreferences::GetValuePoint(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetPoint(GetValueString(theKey)));
}





//============================================================================
//		NCFPreferences::GetValueSize : Get a size value.
//----------------------------------------------------------------------------
HISize NCFPreferences::GetValueSize(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetSize(GetValueString(theKey)));
}





//============================================================================
//		NCFPreferences::GetValueRect : Get a rectangle value.
//----------------------------------------------------------------------------
HIRect NCFPreferences::GetValueRect(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetRect(GetValueString(theKey)));
}





//============================================================================
//		NCFPreferences::GetValueColor : Get a color value.
//----------------------------------------------------------------------------
NColor NCFPreferences::GetValueColor(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetColor(GetValueString(theKey)));
}





//============================================================================
//		NCFPreferences::GetValueDate : Get a date value.
//----------------------------------------------------------------------------
NCFDate NCFPreferences::GetValueDate(const NString &theKey) const
{


	// Get the value
	return(NTextUtilities::GetDate(GetValueString(theKey)));
}





//============================================================================
//		NCFPreferences::GetValueArray : Get an array value.
//----------------------------------------------------------------------------
NArray NCFPreferences::GetValueArray(const NString &theKey) const
{	NArray		theResult;
	NCFObject	cfValue;



	// Get the value
	if (cfValue.Set(CFPreferencesCopyAppValue(theKey, mAppID)))
		theResult = (CFArrayRef) cfValue;
	else
		theResult = mDefaults.GetValueArray(theKey);
	
	return(theResult);
}





//============================================================================
//		NCFPreferences::GetValueDictionary : Get a dictionary value.
//----------------------------------------------------------------------------
NDictionary NCFPreferences::GetValueDictionary(const NString &theKey) const
{	NDictionary		theResult;
	NCFObject		cfValue;



	// Get the value
	if (cfValue.Set(CFPreferencesCopyAppValue(theKey, mAppID)))
		theResult = (CFDictionaryRef) cfValue;
	else
		theResult = mDefaults.GetValueDictionary(theKey);
	
	return(theResult);
}





//============================================================================
//		NCFPreferences::SetValueBoolean : Set a boolean value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueBoolean(const NString &theKey, bool theValue)
{


	// Set the value
	CFPreferencesSetAppValue(theKey, theValue ? kCFBooleanTrue : kCFBooleanFalse, mAppID);
	DoPrefChanged(theKey);
}





//============================================================================
//		NCFPreferences::SetValueOSType : Set an OSType value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueOSType(const NString &theKey, OSType theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromOSType(theValue));
}





//============================================================================
//		NCFPreferences::SetValueUInt32 : Set a UInt32 value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueUInt32(const NString &theKey, UInt32 theValue)
{	NCFObject	cfValue;



	// Set the value
	if (cfValue.Set(CFNumberCreate(NULL, kCFNumberSInt32Type, &theValue)))
		{
		CFPreferencesSetAppValue(theKey, cfValue, mAppID);
		DoPrefChanged(theKey);
		}
}





//============================================================================
//		NCFPreferences::SetValueFloat64 : Set a Float64 value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueFloat64(const NString &theKey, Float64 theValue)
{	NCFObject	cfValue;



	// Set the value
	if (cfValue.Set(CFNumberCreate(NULL, kCFNumberFloat64Type, &theValue)))
		{
		CFPreferencesSetAppValue(theKey, cfValue, mAppID);
		DoPrefChanged(theKey);
		}
}





//============================================================================
//		NCFPreferences::SetValueString : Set a string value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueString(const NString &theKey, const NString &theValue)
{


	// Set the value
	CFPreferencesSetAppValue(theKey, theValue, mAppID);
	DoPrefChanged(theKey);
}





//============================================================================
//		NCFPreferences::SetValuePoint : Set a point value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValuePoint(const NString &theKey, const HIPoint &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromPoint(theValue));
}





//============================================================================
//		NCFPreferences::SetValueSize : Set a size value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueSize(const NString &theKey, const HISize &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromSize(theValue));
}





//============================================================================
//		NCFPreferences::SetValueRect : Set a rectangle value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueRect(const NString &theKey, const HIRect &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromRect(theValue));
}





//============================================================================
//		NCFPreferences::SetValueColor : Set a color value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueColor(const NString &theKey, const NColor &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromColor(theValue));
}





//============================================================================
//		NCFPreferences::SetValueDate : Set a date value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueDate(const NString &theKey, const NCFDate &theValue)
{


	// Set the value
	SetValueString(theKey, NTextUtilities::FromDate(theValue));
}





//============================================================================
//		NCFPreferences::SetValueArray : Set an array value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueArray(const NString &theKey, const NArray &theValue)
{


	// Set the value
	CFPreferencesSetAppValue(theKey, theValue, mAppID);
	DoPrefChanged(theKey);
}





//============================================================================
//		NCFPreferences::SetValueDictionary : Set a dictionary value.
//----------------------------------------------------------------------------
void NCFPreferences::SetValueDictionary(const NString &theKey, const NDictionary &theValue)
{


	// Set the value
	CFPreferencesSetAppValue(theKey, theValue, mAppID);
	DoPrefChanged(theKey);
}





//============================================================================
//		NCFPreferences::GetDefaults : Get the default preferences.
//----------------------------------------------------------------------------
NDictionary NCFPreferences::GetDefaults(void) const
{


	// Get the defaults
	return(mDefaults);
}





//============================================================================
//		NCFPreferences::SetDefaults : Set the default preferences.
//----------------------------------------------------------------------------
void NCFPreferences::SetDefaults(const NDictionary &theDefaults)
{


	// Set the defaults
	mDefaults = theDefaults;
}





//============================================================================
//		NCFPreferences::GetPrefs : Get the application preferences.
//----------------------------------------------------------------------------
NCFPreferences *NCFPreferences::GetPrefs(void)
{	static NCFPreferences	sAppPreferences;



	// Get the preferences
	return(&sAppPreferences);
}





//============================================================================
//		NCFPreferences::DoPrefChanged : A value has been changed.
//----------------------------------------------------------------------------
#pragma mark -
void NCFPreferences::DoPrefChanged(const NString &theKey)
{


	// Let everyone know
	BroadcastMessage(kMsgPreferenceValueChanged, (void *) &theKey);
}



