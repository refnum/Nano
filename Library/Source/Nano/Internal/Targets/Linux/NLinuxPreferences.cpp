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
#include <errno.h>





//============================================================================
//      Internal globals
//----------------------------------------------------------------------------
static	NDictionary	gPreferences;
static	bool needsRead = true;
static	bool needsWrite = false;





//============================================================================
//		SyncFile
//----------------------------------------------------------------------------
static void SyncFile(bool save = false)
{
	if (needsRead || needsWrite)
	{
		NString preferencesName;

		NBundle bundle;
		if (bundle.IsValid())
		{
			preferencesName.Format("%s/%s/%s", bundle.GetInfoString("CompanyName"), bundle.GetInfoString("ProductName"), "user.conf");
		} else {
			preferencesName.Format("%s/%s/%s", "Nano", program_invocation_name, "user.conf");
		}

		NFile preferencesFile = NFileUtilities::GetDirectory(kNLocationPreferences, preferencesName, kNDomainUser, save);
		NPropertyList propertyList;

		if (save && needsWrite && !needsRead)
		{

			propertyList.Save(preferencesFile, gPreferences, kNPropertyListJSON);
			needsWrite = false;

		} else if (!save && needsRead)
		{

			gPreferences = propertyList.Load(preferencesFile);
			needsRead = false;

		}
	}
}




//============================================================================
//		NTargetPreferences::HasKey : Does a key exist?
//----------------------------------------------------------------------------
bool NTargetPreferences::HasKey(const NString &theKey)
{
	SyncFile();
	return gPreferences.HasKey(theKey);
}





//============================================================================
//		NTargetPreferences::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NTargetPreferences::RemoveKey(const NString &theKey)
{
	SyncFile();
	gPreferences.RemoveKey(theKey);
}





//============================================================================
//		NTargetPreferences::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NTargetPreferences::GetValue(const NString &theKey)
{
	SyncFile();
	return gPreferences.GetValue(theKey);
}





//============================================================================
//		NTargetPreferences::SetValue : Set a value.
//----------------------------------------------------------------------------
void NTargetPreferences::SetValue(const NString &theKey, const NVariant &theValue)
{
	SyncFile();
	gPreferences.SetValue(theKey, theValue);
	needsWrite = true;
}





//============================================================================
//		NTargetPreferences::Flush : Flush the preferences.
//----------------------------------------------------------------------------
void NTargetPreferences::Flush(void)
{
	SyncFile(true);
}




