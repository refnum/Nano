/*	NAME:
		NTargetPreferences.h

	DESCRIPTION:
		Preferences support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_PREFERENCES_HDR
#define NTARGET_PREFERENCES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NVariant.h"
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetPreferences {
public:
	// Does a key exist?
	static bool							HasKey(const NString &theKey);


	// Remove a key
	static void							RemoveKey(const NString &theKey);


	// Get/set a value
	static NVariant						GetValue(const NString &theKey);
	static void							SetValue(const NString &theKey, const NVariant &theValue);
};





#endif // NTARGET_PREFERENCES_HDR

