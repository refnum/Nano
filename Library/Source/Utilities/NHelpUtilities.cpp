/*	NAME:
		NHelpUtilities.cpp

	DESCRIPTION:
		Help utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHelpUtilities.h"





//============================================================================
//      NHelpUtilities::GetHelpTagDelay : Get the help tag delay.
//----------------------------------------------------------------------------
EventTime NHelpUtilities::GetHelpTagDelay(void)
{	Duration	theDelay;
	OSStatus	theErr;



	// Get the delay
	//
	// The delay is documented to be in milliseconds (+ve) or microseconds (-ve),
	// but since a microsecond delay is pointless we assert it's never used.
	theDelay = 0;
	theErr   = HMGetTagDelay(&theDelay);

	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theDelay > 0);
	
	return(((EventTime) theDelay) * kEventDurationMillisecond);
}





//============================================================================
//      NHelpUtilities::SetHelpTagDelay : Set the help tag delay.
//----------------------------------------------------------------------------
void NHelpUtilities::SetHelpTagDelay(EventTime theDelay)
{	OSStatus	theErr;



	// Set the delay
	theErr = HMSetTagDelay((Duration) (theDelay / kEventDurationMillisecond));
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NHelpUtilities::GetHelpMenu : Get the help menu.
//----------------------------------------------------------------------------
NMenuItem NHelpUtilities::GetHelpMenu(void)
{	MenuItemIndex	theIndex;
	MenuRef			theMenu;
	NMenuItem		theItem;
	OSStatus		theErr;



	// Get the state we need
	theErr  = HMGetHelpMenu(&theMenu, &theIndex);
	NN_ASSERT_NOERR(theErr);



	// Get the menu item
	if (theErr == noErr)
		theItem = NMenuItem(theMenu, theIndex, false);
	
	return(theItem);
}

