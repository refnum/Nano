/*	NAME:
		TPreferences.cpp

	DESCRIPTION:
		NPreferences tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NPreferences.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Keys
static const NString kKeyBoolean									= "Test Boolean";
static const NString kKeyNumber										= "Test Number";
static const NString kKeyString										= "Test String";
static const NString kKeyData										= "Test Data";
static const NString kKeyDate										= "Test Date";
static const NString kKeyColor										= "Test Color";
static const NString kKeyArray										= "Test Array";
static const NString kKeyDictionary									= "Test Dictionary";


// Values
static const bool    kValueBoolean1									= true;
static const bool    kValueBoolean2									= false;
static const NString kValueString1									= "First String";
static const NString kValueString2									= "Second String";





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NPREFERENCES(_name, _desc)								NANO_TEST(TPreferences, _name, _desc)

NANO_FIXTURE(TPreferences)
{
	NPreferences	*thePrefs;

	SETUP
	{
		thePrefs = NPreferences::Get();
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NPREFERENCES("Empty", "Empty preferences")
{


	// Perform the test
	REQUIRE( thePrefs->GetDefaults().IsEmpty());
	REQUIRE(!thePrefs->HasKey(kValueString1));
	REQUIRE(!thePrefs->HasKey(kValueString2));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NPREFERENCES("Defaults", "Default preferences")
{	NDictionary		theDefaults;



	// Perform the test
	theDefaults.SetValue(kKeyBoolean, kValueBoolean1);
	theDefaults.SetValue(kKeyString,  kValueString1);
	thePrefs->SetDefaults(theDefaults);
	
	REQUIRE(thePrefs->HasKey(kKeyBoolean));
	REQUIRE(thePrefs->HasKey(kKeyString));

	REQUIRE(!thePrefs->HasKey(kKeyBoolean, false));
	REQUIRE(!thePrefs->HasKey(kKeyString,  false));

	REQUIRE(thePrefs->GetValueBoolean(kKeyBoolean) == kValueBoolean1);
	REQUIRE(thePrefs->GetValueString (kKeyString)  == kValueString1);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NPREFERENCES("Modify", "Modify the preferences")
{


	// Set
	thePrefs->SetValue(kKeyString, kValueString2);
	thePrefs->Flush();

	REQUIRE(thePrefs->HasKey(kKeyString));
	REQUIRE(thePrefs->GetValueString(kKeyString) == kValueString2);



	// Remove
	thePrefs->RemoveKey(kKeyString);
	thePrefs->Flush();

	REQUIRE( thePrefs->HasKey(kKeyString, true));
	REQUIRE(!thePrefs->HasKey(kKeyString, false));
	REQUIRE( thePrefs->GetValueString(kKeyString) == kValueString1);

	thePrefs->SetDefaults(NDictionary());
}


