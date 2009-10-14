/*	NAME:
		NMenuUtilities.cpp

	DESCRIPTION:
		Menu utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include Menus
//----------------------------------------------------------------------------
#include "NMenuUtilities.h"





//============================================================================
//		NMenuUtilities::GetItemIndex : Get a menu item's index.
//----------------------------------------------------------------------------
MenuItemIndex NMenuUtilities::GetItemIndex(MenuRef &theMenu, UInt32 theItem, bool isCmd)
{	MenuItemIndex	theIndex;
	OSStatus		theErr;



	// Get the index
	theIndex = theItem;
	
	if (isCmd)
		{
		theErr = GetIndMenuItemWithCommandID(theMenu, theItem, 1, &theMenu, &theIndex);
		NN_ASSERT_NOERR(theErr);
		}
	
	return(theIndex);
}





//============================================================================
//		NMenuUtilities::GetItemText : Get a menu item's text.
//----------------------------------------------------------------------------
NString NMenuUtilities::GetItemText(MenuRef theMenu, UInt32 theItem, bool isCmd)
{	MenuItemIndex		theIndex;
	NString				theValue;
	CFStringRef			cfString;
	OSStatus			theErr;



	// Get the item text
	cfString = NULL;
	theIndex = GetItemIndex(theMenu, theItem, isCmd);
	theErr   = CopyMenuItemTextAsCFString(theMenu, theIndex, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NMenuUtilities::SetItemText : Set a menu item's text.
//----------------------------------------------------------------------------
void NMenuUtilities::SetItemText(MenuRef theMenu, UInt32 theItem, const NString &theValue, bool isCmd)
{	MenuItemIndex	theIndex;
	OSStatus		theErr;



	// Set the item text
	theIndex = GetItemIndex(theMenu, theItem, isCmd);
	theErr   = SetMenuItemTextWithCFString(theMenu, theIndex, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenuUtilities::GetItemMark : Get a menu item's mark.
//----------------------------------------------------------------------------
UniChar NMenuUtilities::GetItemMark(MenuRef theMenu, UInt32 theItem, bool isCmd)
{	MenuItemIndex		theIndex;
	CharParameter		theValue;



	// Get the item mark
	theIndex = GetItemIndex(theMenu, theItem, isCmd);

	::GetItemMark(theMenu, theIndex, &theValue);
	
	return((UniChar) theValue);
}





//============================================================================
//		NMenuUtilities::SetItemMark : Set a menu item's mark.
//----------------------------------------------------------------------------
void NMenuUtilities::SetItemMark(MenuRef theMenu, UInt32 theItem, UniChar theValue, bool isCmd)
{	MenuItemIndex	theIndex;



	// Set the item mark
	theIndex = GetItemIndex(theMenu, theItem, isCmd);

	::SetItemMark(theMenu, theIndex, (CharParameter) theValue);
}




