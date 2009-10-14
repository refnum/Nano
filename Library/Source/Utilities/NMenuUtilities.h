/*	NAME:
		NMenuUtilities.h

	DESCRIPTION:
		Menu utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMENUUTILITIES_HDR
#define NMENUUTILITIES_HDR
//============================================================================
//		Include Menus
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMenuUtilities {
public:
	// Get a menu item's index
	static MenuItemIndex				GetItemIndex(MenuRef &theMenu, UInt32 theItem, bool isCmd);


	// Get/set the text of a menu item
	static NString						GetItemText(MenuRef theMenu, UInt32 theItem,                          bool isCmd=true);
	static void							SetItemText(MenuRef theMenu, UInt32 theItem, const NString &theValue, bool isCmd=true);


	// Get/set the mark of a menu item
	static UniChar						GetItemMark(MenuRef theMenu, UInt32 theItem,                   bool isCmd=true);
	static void							SetItemMark(MenuRef theMenu, UInt32 theItem, UniChar theValue, bool isCmd=true);
};

	
	


#endif // NMenuUTILITIES_HDR


