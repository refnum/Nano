/*	NAME:
		NMacPreferences.cpp

	DESCRIPTION:
		Mac preferences support.
	
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
#include "NTargetSystem.h"
#include "NCoreFoundation.h"
#include "NMacTarget.h"





//============================================================================
//		NTargetPreferences::HasKey : Does a key exist?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetPreferences::HasKey(const NString &theKey)
{	NCFObject		cfObject;
	bool			hasKey;



	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// Check the key
	hasKey = cfObject.SetObject(CFPreferencesCopyAppValue(ToCF(theKey), kCFPreferencesCurrentApplication));
	
	return(hasKey);
}





//============================================================================
//		NTargetPreferences::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NTargetPreferences::RemoveKey(const NString &theKey)
{


	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// Remove the key
	CFPreferencesSetAppValue(ToCF(theKey), NULL, kCFPreferencesCurrentApplication);
}





//============================================================================
//		NTargetPreferences::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NTargetPreferences::GetValue(const NString &theKey)
{	NVariant		theValue;
	NCFObject		cfValue;



	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// Get the value
	if (cfValue.SetObject(CFPreferencesCopyAppValue(ToCF(theKey), kCFPreferencesCurrentApplication)))
		theValue = NMacTarget::ConvertCFToObject(cfValue);
	
	return(theValue);
}





//============================================================================
//		NTargetPreferences::SetValue : Set a value.
//----------------------------------------------------------------------------
void NTargetPreferences::SetValue(const NString &theKey, const NVariant &theValue)
{	NCFObject		theObject;



	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// Set the value
	theObject = NMacTarget::ConvertObjectToCF(theValue);
	if (theObject.IsValid())
		CFPreferencesSetAppValue(ToCF(theKey), theObject, kCFPreferencesCurrentApplication);
}





//============================================================================
//		NTargetPreferences::Flush : Flush the preferences.
//----------------------------------------------------------------------------
void NTargetPreferences::Flush(void)
{	bool	wasOK;



	// Flush the preferences
	wasOK = CFPreferencesAppSynchronize(kCFPreferencesCurrentApplication);
	NN_ASSERT(wasOK);
}




