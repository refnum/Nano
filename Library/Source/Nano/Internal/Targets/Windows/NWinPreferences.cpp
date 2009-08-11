/*	NAME:
		NWindowsPreferences.cpp

	DESCRIPTION:
		Windows preferences support.
	
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





//============================================================================
//		NTargetPreferences::HasKey : Does a key exist?
//----------------------------------------------------------------------------
bool NTargetPreferences::HasKey(const NString &theKey)
{

	// dair, to do
	return(false);
}





//============================================================================
//		NTargetPreferences::RemoveKey : Remove a key.
//----------------------------------------------------------------------------
void NTargetPreferences::RemoveKey(const NString &theKey)
{
}





//============================================================================
//		NTargetPreferences::GetValue : Get a value.
//----------------------------------------------------------------------------
NVariant NTargetPreferences::GetValue(const NString &theKey)
{


	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// dair, to do
}





//============================================================================
//		NTargetPreferences::SetValue : Set a value.
//----------------------------------------------------------------------------
void NTargetPreferences::SetValue(const NString &theKey, const NVariant &theValue)
{


	// Validate our parameters
	NN_ASSERT(!theKey.IsEmpty());



	// dair, to do
}


