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
#include "NMenuItem.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHelpUtilities {
public:
	// Get/set the help tag delay
	static EventTime				GetHelpTagDelay(void);
	static void						SetHelpTagDelay(EventTime theDelay);


	// Get the help menu
	//
	// Returns the first menu item to be used for application content.
	static NMenuItem				GetHelpMenu(void);
};





#endif // NHELPUTILITIES_HDR


