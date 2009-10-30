/*	NAME:
		NCFPreferences.cpp

	DESCRIPTION:
		CoreFoundation preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFPreferences.h"





//============================================================================
//		NCFPreferences::NCFPreferences : Constructor.
//----------------------------------------------------------------------------
NCFPreferences::NCFPreferences(const NString &appID)
{


	// Initialize ourselves
	//
	// CFPreferences special-cases the kCFPreferencesCurrentApplication pointer,
	// rather than using its value (rdar://4993874), so we need to work with a
	// raw CFStringRef that can hold this address rather than using an NString.
	mAppID = appID;
	CFSafeRetain(mAppID);

	if (mAppID == NULL || CFStringGetLength(mAppID) == 0)
		mAppID = kCFPreferencesCurrentApplication;
}





//============================================================================
//		NCFPreferences::~NCFPreferences : Destructor.
//----------------------------------------------------------------------------
NCFPreferences::~NCFPreferences(void)
{


	// Save the preferences
	Save();



	// Clean up
	CFSafeRelease(mAppID);
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
//		NCFPreferences::GetPrefs : Get the application preferences.
//----------------------------------------------------------------------------
NCFPreferences *NCFPreferences::GetPrefs(void)
{	static NCFPreferences	sAppPreferences;



	// Get the preferences
	return(&sAppPreferences);
}





//============================================================================
//		NCFPreferences::RemovePropertyKey : Remove a property key.
//----------------------------------------------------------------------------
#pragma mark -
void NCFPreferences::RemovePropertyKey(const NString &theKey)
{


	// Remove the key
	CFPreferencesSetAppValue(theKey, NULL, mAppID);
	
	BroadcastMessage(kMsgPreferenceValueChanged, (void *) &theKey);
}





//============================================================================
//		NCFPreferences::GetPropertyValue : Get a property value.
//----------------------------------------------------------------------------
NCFObject NCFPreferences::GetPropertyValue(const NString &theKey) const
{	NCFObject	cfValue;



	// Get the value
	if (theKey == kPropertyAllValuesKey)
		cfValue.Set(CFPreferencesCopyMultiple(NULL, mAppID, kCFPreferencesCurrentUser, kCFPreferencesCurrentHost));
	else
		cfValue.Set(CFPreferencesCopyAppValue(theKey, mAppID));
	
	return(cfValue);
}





//============================================================================
//		NCFPreferences::SetPropertyValue : Set a property value.
//----------------------------------------------------------------------------
void NCFPreferences::SetPropertyValue(const NString &theKey, const NCFObject &theValue)
{


	// Set the value
	if (theKey == kPropertyAllValuesKey)
		CFPreferencesSetMultiple(theValue, NULL, mAppID, kCFPreferencesCurrentUser, kCFPreferencesCurrentHost);
	else
		{
		CFPreferencesSetAppValue(theKey, theValue, mAppID);
		BroadcastMessage(kMsgPreferenceValueChanged, (void *) &theKey);
		}
}



