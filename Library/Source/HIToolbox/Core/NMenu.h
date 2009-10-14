/*	NAME:
		NMenu.h

	DESCRIPTION:
		MenuRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMENU_HDR
#define NMENU_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NContainer.h"
#include "NString.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NMenuItem;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMenu : public NContainer {
public:
										NMenu(MenuRef      theMenu);
										NMenu(const NMenu &theMenu);

										NMenu(void);
	virtual								~NMenu(void);


	// Is the menu valid?
	bool								IsValid(void) const;


	// Is an index in range?
	bool								InRange(MenuItemIndex theIndex) const;
	

	// Get the size
	UInt32								GetSize(void) const;


	// Clear the menu
	void								Clear(void);


	// Get the tracking data
	MenuTrackingData					GetTracking(void) const;


	// Get/set the attributes
	MenuAttributes						GetAttributes(void) const;
	void								SetAttributes(MenuAttributes setThese, MenuAttributes clearThese=0);
	

	// Append items to the menu
	NMenuItem							AppendItem(const NString &theName, MenuCommand theCmd, MenuItemAttributes theAttributes=0);
	NMenuItem							AppendSeparator(void);


	// Copy items into the menu
	void								CopyItems(const NMenu &srcMenu, MenuItemIndex firstItem, UInt32 numItems, MenuItemIndex insertAfter);


	// Operators
	NMenu								&operator = (const NMenu &otherMenu);
	operator							MenuRef(void) const;


private:
	void								AcquireMenu(MenuRef theMenu);
	void								ReleaseMenu(void);


private:
	MenuRef								mMenu;
};



#endif // NMENU_HDR


