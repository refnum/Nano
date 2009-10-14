/*	NAME:
		NHelpUtilities.h

	DESCRIPTION:
		Help utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHELPUTILITIES_HDR
#define NHELPUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NMenuItem.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHelpUtilities {
public:
	// Get an empty help tag
	static HMHelpContentRec			GetEmptyHelp(void);


	// Get/set a help string
	static NString					GetHelpString(const HMHelpContentRec &helpTag,                          bool isExtended=false);
	static void						SetHelpString(      HMHelpContentRec &helpTag, const NString &theValue, bool isExtended=false);


	// Get the help menu
	//
	// Returns the first menu item to be used for application content.
	static NMenuItem				GetHelpMenu(void);
};





#endif // NHELPUTILITIES_HDR


