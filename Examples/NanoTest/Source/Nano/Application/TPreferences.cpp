/*	NAME:
		TPreferences.cpp

	DESCRIPTION:
		NPreferences tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPreferences.h"

#include "TPreferences.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Keys
static const NString kKeyBoolean							= "Test Boolean";
static const NString kKeyNumber								= "Test Number";
static const NString kKeyString								= "Test String";
static const NString kKeyData								= "Test Data";
static const NString kKeyDate								= "Test Date";
static const NString kKeyColor								= "Test Color";
static const NString kKeyArray								= "Test Array";
static const NString kKeyDictionary							= "Test Dictionary";


// Values
static const bool    kValueBoolean1							= true;
static const bool    kValueBoolean2							= false;
static const NString kValueString1							= "First String";
static const NString kValueString2							= "Second String";





//============================================================================
//		TPreferences::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TPreferences::Execute(void)
{	NDictionary			theDefaults;
	NPreferences		*thePrefs;



	// Empty
	thePrefs = NPreferences::Get();
	
	NN_ASSERT( thePrefs->GetDefaults().IsEmpty());
	NN_ASSERT(!thePrefs->HasKey(kValueString1));
	NN_ASSERT(!thePrefs->HasKey(kValueString2));



	// Defaults
	theDefaults.SetValue(kKeyBoolean, kValueBoolean1);
	theDefaults.SetValue(kKeyString,  kValueString1);
	thePrefs->SetDefaults(theDefaults);
	
	NN_ASSERT(thePrefs->HasKey(kKeyBoolean));
	NN_ASSERT(thePrefs->HasKey(kKeyString));

	NN_ASSERT(!thePrefs->HasKey(kKeyBoolean, false));
	NN_ASSERT(!thePrefs->HasKey(kKeyString,  false));

	NN_ASSERT(thePrefs->GetValueBoolean(kKeyBoolean) == kValueBoolean1);
	NN_ASSERT(thePrefs->GetValueString (kKeyString)  == kValueString1);



	// Set
	thePrefs->SetValue(kKeyString, kValueString2);
	NN_ASSERT(thePrefs->HasKey(kKeyString));
	NN_ASSERT(thePrefs->GetValueString(kKeyString) == kValueString2);



	// Remove
	thePrefs->RemoveKey(kKeyString);
	NN_ASSERT( thePrefs->HasKey(kKeyString, true));
	NN_ASSERT(!thePrefs->HasKey(kKeyString, false));
	NN_ASSERT( thePrefs->GetValueString(kKeyString) == kValueString1);
}

