/*	NAME:
		NPreferences.cpp

	DESCRIPTION:
		Preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetPreferences.h"
#include "NPreferences.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NString kNPrefChangedAllKey									= "*";





//============================================================================
//		NPreferences::NPreferences : Constructor.
//----------------------------------------------------------------------------
NPreferences::NPreferences(void)
{


	// Initialise ourselves
	AddListener(this);
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
		hasKey = NPropertyStore::HasKey(theKey, true);
	
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
		theValue = NPropertyStore::GetValue(theKey);
	
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
	BroadcastMessage(kMsgNPreferenceValueChanged, &theKey);
}





//============================================================================
//		NPreferences::Flush : Flush the preferences.
//----------------------------------------------------------------------------
void NPreferences::Flush(void)
{


	// Flush the prefs
	NTargetPreferences::Flush();
}





//============================================================================
//		NPreferences::Get : Get the instance.
//----------------------------------------------------------------------------
NPreferences *NPreferences::Get(void)
{	static NPreferences		sInstance;



	// Get the instance
	return(&sInstance);
}





#pragma mark private
//============================================================================
//		NPreferences::DoMessage : Handle messages.
//----------------------------------------------------------------------------
void NPreferences::DoMessage(NBroadcastMsg theMsg, const void *msgData)
{	const NString		*theKey = (const NString *) msgData;



	// Handle the message
	switch (theMsg) {
		case kMsgNPropertyStoreValueChanged:
			if (*theKey == kNPropertyStoreChangedAllKey)
				BroadcastMessage(kMsgNPreferenceValueChanged, &kNPrefChangedAllKey);
			break;
		
		default:
			break;
		}
}


