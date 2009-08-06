/*	NAME:
		NMacPreferences.cpp

	DESCRIPTION:
		Mac preferences support.
	
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
#include "NTargetSystem.h"
#include "NMacTarget.h"
#include "NCFString.h"





//============================================================================
//		NTargetPreferences::HasKey : Does a key exist?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetPreferences::HasKey(const NString &theKey)
{	NCFObject		cfObject;
	bool			hasKey;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Check the key
	hasKey = cfObject.SetObject(CFPreferencesCopyAppValue(NCFString(theKey).GetObject(), kCFPreferencesCurrentApplication));
	
	return(hasKey);
}





//============================================================================
//		NTargetPreferences::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NTargetPreferences::RemoveKey(const NString &theKey)
{


	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Remove the key
	CFPreferencesSetAppValue(NCFString(theKey).GetObject(), NULL, kCFPreferencesCurrentApplication);
}





//============================================================================
//		NTargetPreferences::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NTargetPreferences::GetValue(const NString &theKey)
{	NVariant		theValue;
	NCFObject		cfValue;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Get the value
	if (cfValue.SetObject(CFPreferencesCopyAppValue(NCFString(theKey).GetObject(), kCFPreferencesCurrentApplication)))
		theValue = NMacTarget::ConvertCFToObject(cfValue);
	
	return(theValue);
}





//============================================================================
//		NTargetPreferences::SetValue : Set a value.
//----------------------------------------------------------------------------
void NTargetPreferences::SetValue(const NString &theKey, const NVariant &theValue)
{	NCFObject		theObject;



	// Validate our parameters
	NN_ASSERT(theKey.IsNotEmpty());



	// Set the value
	theObject = NMacTarget::ConvertObjectToCF(theValue);
	if (theObject.IsValid())
		CFPreferencesSetAppValue(NCFString(theKey).GetObject(), theObject, kCFPreferencesCurrentApplication);
}




