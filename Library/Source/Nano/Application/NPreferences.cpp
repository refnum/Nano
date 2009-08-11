/*	NAME:
		NPreferences.cpp

	DESCRIPTION:
		Preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NTargetPreferences.h"
#include "NPreferences.h"





//============================================================================
//		NPreferences::NPreferences : Constructor.
//----------------------------------------------------------------------------
NPreferences::NPreferences(void)
{
}





//============================================================================
//		NPreferences::~NPreferences : Destructor.
//----------------------------------------------------------------------------
NPreferences::~NPreferences(void)
{
}





//============================================================================
//		NPreferences::HasKey : Does a key exist?
//----------------------------------------------------------------------------
bool NPreferences::HasKey(const NString &theKey, bool checkDefaults) const
{	bool	hasKey;



	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// Check the key
	hasKey = NTargetPreferences::HasKey(theKey);
	if (!hasKey && checkDefaults)
		hasKey = mDefaults.HasKey(theKey);
	
	return(hasKey);
}





//============================================================================
//		NPreferences::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NPreferences::RemoveKey(const NString &theKey)
{


	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// Remove the key
	NTargetPreferences::RemoveKey(theKey);
}





//============================================================================
//		NPreferences::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NPreferences::GetValue(const NString &theKey) const
{	NVariant		theValue;



	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());
	


	// Get the value
	theValue = NTargetPreferences::GetValue(theKey);
	if (!theValue.IsValid())
		theValue = mDefaults.GetValue(theKey);
	
	return(theValue);
}





//============================================================================
//		NPreferences::SetValue : Set a value.
//----------------------------------------------------------------------------
void NPreferences::SetValue(const NString &theKey, const NVariant &theValue)
{


	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());
	


	// Set the value
	NTargetPreferences::SetValue(theKey, theValue);
	BroadcastMessage(kMsgPreferenceValueChanged, &theKey);
}





//============================================================================
//		NPreferences::GetValueBoolean : Get a boolean value.
//----------------------------------------------------------------------------
bool NPreferences::GetValueBoolean(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetBoolean(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueSInt32 : Get a SInt32 value.
//----------------------------------------------------------------------------
SInt32 NPreferences::GetValueSInt32(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetSInt32(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueSInt64 : Get a SInt64 value.
//----------------------------------------------------------------------------
SInt64 NPreferences::GetValueSInt64(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetSInt64(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueFloat32 : Get a Float32 value.
//----------------------------------------------------------------------------
Float32 NPreferences::GetValueFloat32(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetFloat32(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueFloat64 : Get a Float64 value.
//----------------------------------------------------------------------------
Float64 NPreferences::GetValueFloat64(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetFloat64(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueString : Get a string value.
//----------------------------------------------------------------------------
NString NPreferences::GetValueString(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetString(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueData : Get a data value.
//----------------------------------------------------------------------------
NData NPreferences::GetValueData(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetData(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueDate : Get a date value.
//----------------------------------------------------------------------------
NDate NPreferences::GetValueDate(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetDate(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueColor : Get a color value.
//----------------------------------------------------------------------------
NColor NPreferences::GetValueColor(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetColor(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValuePoint : Get a point value.
//----------------------------------------------------------------------------
NPoint NPreferences::GetValuePoint(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetPoint(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueSize : Get a size value.
//----------------------------------------------------------------------------
NSize NPreferences::GetValueSize(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetSize(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueRectangle : Get a rectangle value.
//----------------------------------------------------------------------------
NRectangle NPreferences::GetValueRectangle(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetRectangle(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueArray : Get an array value.
//----------------------------------------------------------------------------
NArray NPreferences::GetValueArray(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetArray(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetValueDictionary : Get a dictionary value.
//----------------------------------------------------------------------------
NDictionary NPreferences::GetValueDictionary(const NString &theKey) const
{


	// Get the value
	return(NSystemUtilities::GetDictionary(GetValue(theKey), theKey));
}





//============================================================================
//		NPreferences::GetDefaults : Get the defaults.
//----------------------------------------------------------------------------
NDictionary NPreferences::GetDefaults(void)
{


	// Get the defaults
	return(mDefaults);
}





//============================================================================
//		NPreferences::SetDefaults : Set the defaults.
//----------------------------------------------------------------------------
void NPreferences::SetDefaults(const NDictionary &theDefaults)
{


	// Set the defaults
	mDefaults = theDefaults;
	BroadcastMessage(kMsgPreferenceValueChanged, &kPrefChangedAllKey);
}





//============================================================================
//		NPreferences::Get : Get the preferences.
//----------------------------------------------------------------------------
NPreferences *NPreferences::Get(void)
{	static NPreferences		sPreferences;



	// Get the preferences
	return(&sPreferences);
}

