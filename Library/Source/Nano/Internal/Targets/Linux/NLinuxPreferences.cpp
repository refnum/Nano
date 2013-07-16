/*	NAME:
		NLinuxPreferences.cpp

	DESCRIPTION:
		Linux preferences support.
	
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
#include "NSystemUtilities.h"
#include "NMutex.h"





//============================================================================
//      Internal globals
//----------------------------------------------------------------------------
static NMutex		gPreferencesLock;
static bool			gPreferencesDirty;
static NDictionary	gPreferences;





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		GetPrefsFile : Get the preferences file.
//----------------------------------------------------------------------------
static NFile GetPrefsFile(bool createDir)
{	NBundle		appBundle;
	NString		fileName;
	NFile		theFile;



	// Get the state we need
	if (appBundle.IsValid())
		fileName.Format("%@.json", appBundle.GetIdentifier());
	else
		fileName.Format("%@.json", NSystemUtilities::GetProcessName());



	// Get the file
	theFile = NFileUtilities::GetDirectory(kNLocationPreferences, fileName, kNDomainUser, createDir);
	
	return(theFile);
}





//============================================================================
//		LoadPrefs : Load the preferences.
//----------------------------------------------------------------------------
static void LoadPrefs(void)
{	NPropertyList	propertyList;
	NFile			theFile;



	// Get the state we need
	theFile = GetPrefsFile(false);



	// Load the prefs
	gPreferencesDirty = false;

	if (theFile.IsFile())
		gPreferences = propertyList.Load(theFile);
}





//============================================================================
//		SavePrefs : Save the preferences.
//----------------------------------------------------------------------------
static void SavePrefs(void)
{	NPropertyList	propertyList;
	NFile			theFile;



	// Get the state we need
	theFile = GetPrefsFile(true);



	// Save the prefs
	if (gPreferencesDirty)
		{
		propertyList.Save(theFile, gPreferences, kNPropertyListJSON);
		gPreferencesDirty = false;
		}
}





//============================================================================
//		ChangedPrefs : Mark the preferences as changed.
//----------------------------------------------------------------------------
static void ChangedPrefs(void)
{


	// Update our state
	gPreferencesDirty = true;
}





#pragma NTargetPreferences
//============================================================================
//		NTargetPreferences::HasKey : Does a key exist?
//----------------------------------------------------------------------------
bool NTargetPreferences::HasKey(const NString &theKey)
{	StLock		acquireLock(gPreferencesLock);



	// Check the key
	LoadPrefs();

	return(gPreferences.HasKey(theKey));
}





//============================================================================
//		NTargetPreferences::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NTargetPreferences::RemoveKey(const NString &theKey)
{	StLock		acquireLock(gPreferencesLock);



	// Remove the key
	LoadPrefs();

	gPreferences.RemoveKey(theKey);

	ChangedPrefs();
}





//============================================================================
//		NTargetPreferences::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NTargetPreferences::GetValue(const NString &theKey)
{	StLock		acquireLock(gPreferencesLock);



	// Get the value
	LoadPrefs();
	
	return(gPreferences.GetValue(theKey));
}





//============================================================================
//		NTargetPreferences::SetValue : Set a value.
//----------------------------------------------------------------------------
void NTargetPreferences::SetValue(const NString &theKey, const NVariant &theValue)
{	StLock		acquireLock(gPreferencesLock);



	// Set the value
	LoadPrefs();

	gPreferences.SetValue(theKey, theValue);

	ChangedPrefs();
}





//============================================================================
//		NTargetPreferences::Flush : Flush the preferences.
//----------------------------------------------------------------------------
void NTargetPreferences::Flush(void)
{	StLock		acquireLock(gPreferencesLock);



	// Flush the preferences
	SavePrefs();
}




