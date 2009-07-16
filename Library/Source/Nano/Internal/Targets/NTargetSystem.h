/*	NAME:
		NTargetSystem.h

	DESCRIPTION:
		System support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_SYSTEM_HDR
#define NTARGET_SYSTEM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetSystem {
public:
	// Find a bundle
	//
	// Returns the application bundle if bundleID is empty.
	static NFile						FindBundle(const NString &bundleID);
};





#endif // NTARGET_SYSTEM_HDR
