/*	NAME:
		NPreferences.cpp

	DESCRIPTION:
		Preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetPreferences.h"
#include "NSystemUtilities.h"
#include "NPreferences.h"





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
	BroadcastMessage(kMsgPreferenceValueChanged, &theKey);
}





//============================================================================
//		NPreferences::Get : Get the preferences.
//----------------------------------------------------------------------------
NPreferences *NPreferences::Get(void)
{	static NPreferences		sPreferences;



	// Get the preferences
	return(&sPreferences);
}





//============================================================================
//		NPreferences::DoMessage : Handle messages.
//----------------------------------------------------------------------------
#pragma mark -
void NPreferences::DoMessage(BroadcastMsg theMsg, const void *msgData)
{	const NString		*theKey = (const NString *) msgData;



	// Handle the message
	switch (theMsg) {
		case kMsgPropertyValueChanged:
			if (*theKey == kPropertyChangedAllKey)
				BroadcastMessage(kMsgPreferenceValueChanged, &kPrefChangedAllKey);
			break;
		
		default:
			break;
		}
}


