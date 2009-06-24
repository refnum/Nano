/*	NAME:
		NPreferences.cpp

	DESCRIPTION:
		Preferences.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPreferences.h"





//============================================================================
//		NPreferences::NPreferences : Constructor.
//----------------------------------------------------------------------------
NPreferences::NPreferences(void)
{
}





//============================================================================
//		NPreferences::~NPreferences : Destructor.
//----------------------------------------------------------------------------
NPreferences::~NPreferences(void)
{


	// Save the preferences
	Save();
}





//============================================================================
//		NPreferences::Save : Save the preferences.
//----------------------------------------------------------------------------
void NPreferences::Save(void)
{


	// Save the preferences
		// dair, to do
}





//============================================================================
//		NPreferences::Get : Get the preferences.
//----------------------------------------------------------------------------
NPreferences *NPreferences::Get(void)
{	static NPreferences	sPreferences;



	// Get the preferences
	return(&sPreferences);
}

