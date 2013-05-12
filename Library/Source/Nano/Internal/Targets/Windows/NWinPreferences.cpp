/*	NAME:
		NWindowsPreferences.cpp

	DESCRIPTION:
		Windows preferences support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"
#include "NWindows.h"
#include "NRegistry.h"
#include "NTargetPreferences.h"





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		GetAppRegistryPath : Get the registry path for the app.
//----------------------------------------------------------------------------
static NString GetAppRegistryPath(void)
{	static NString	sAppKey;

	TCHAR	theBuffer[MAX_PATH];
	NFile	theFile;



	// Create the app path
	//
	// This may need to be customisable at run-time, to allow apps to store
	// values under "Software\\Company Name\\Product Name" and to allow the
	// executable to be renamed without affecting its prefs.
	if (sAppKey.IsEmpty())
		{
		if (GetModuleFileName(NULL, theBuffer, MAX_PATH))
			{
			theFile = NFile(ToNN(theBuffer));
			if (theFile.IsFile())
				sAppKey.Format("Software\\%@", theFile.GetName(kNNameNoExtension));
			}

		NN_ASSERT(!sAppKey.IsEmpty());
		}
	
	return(sAppKey);
}





//============================================================================
//		GetAppRegistry : Get the registry for the app prefs.
//----------------------------------------------------------------------------
static bool GetAppRegistry(NRegistry &theRegistry, bool canCreate)
{	NStatus		theErr;



	// Open the registry
	theErr = theRegistry.Open(HKEY_CURRENT_USER, GetAppRegistryPath(), canCreate, false);
	
	if (canCreate)
		NN_ASSERT_NOERR(theErr);
	else
		NN_ASSERT(theErr == kNoErr || theErr == kNErrNotFound);

	return(theRegistry.IsOpen());		
}





//============================================================================
//      Public functions
//----------------------------------------------------------------------------
//		NTargetPreferences::HasKey : Does a key exist?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetPreferences::HasKey(const NString &theKey)
{	NRegistry	theRegistry;
	bool		hasKey;
	


	// Get the state we need
	if (!GetAppRegistry(theRegistry, false))
		return(false);



	// Query the registry
	hasKey = theRegistry.HasKey(theKey);
	
	return(hasKey);
}





//============================================================================
//		NTargetPreferences::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NTargetPreferences::RemoveKey(const NString &theKey)
{	NRegistry	theRegistry;



	// Get the state we need
	if (!GetAppRegistry(theRegistry, false))
		return;



	// Update the registry
	theRegistry.RemoveKey(theKey);
}





//============================================================================
//		NTargetPreferences::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NTargetPreferences::GetValue(const NString &theKey)
{	NRegistry	theRegistry;
	NVariant	theValue;



	// Get the state we need
	if (!GetAppRegistry(theRegistry, false))
		return(theValue);



	// Query the registry
	theValue = theRegistry.GetValue(theKey);
	
	return(theValue);
}





//============================================================================
//		NTargetPreferences::SetValue : Set a value.
//----------------------------------------------------------------------------
void NTargetPreferences::SetValue(const NString &theKey, const NVariant &theValue)
{	NRegistry	theRegistry;



	// Get the state we need
	if (!GetAppRegistry(theRegistry, true))
		return;



	// Update the registry
	theRegistry.SetValue(theKey, theValue);
}





//============================================================================
//		NTargetPreferences::Flush : Flush the preferences.
//----------------------------------------------------------------------------
void NTargetPreferences::Flush(void)
{	NRegistry	theRegistry;



	// Get the state we need
	if (!GetAppRegistry(theRegistry, false))
		return;



	// Update the registry
	theRegistry.Flush();
}



