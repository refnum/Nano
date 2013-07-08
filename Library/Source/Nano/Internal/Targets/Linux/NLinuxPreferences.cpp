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
	NN_LOG("NLinuxPreferences SyncFile unfinished implementation!");
	if (needsRead || needsWrite)
	{
		NFile preferencesFile = NFileUtilities::GetDirectory(kNLocationPreferences, "user.conf", kNDomainUser, save);
		if (save && needsWrite && !needsRead)
		{
			NEncoder encoder;
			NData outputData = encoder.Encode(gPreferences, kNEncoderXML);
			NFileUtilities::SetFileData(preferencesFile, outputData);
			needsWrite = false;
		} else if (!save && needsRead)
		{
			NData inputData = NFileUtilities::GetFileData(preferencesFile);
			NEncoder encoder;
			// TO-DO Implement decoder
			//gPreferences = (NDictionary)encoder.Decode(inputData);
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




