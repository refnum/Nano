/*	NAME:
		NMenu.h

	DESCRIPTION:
		MenuRef object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMENU_HDR
#define NMENU_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
class NMenuItem;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMenu {
public:
										NMenu(MenuRef      theMenu);
										NMenu(const NMenu &theMenu);

										NMenu(void);
	virtual								~NMenu(void);


	// Is the menu valid?
	bool								IsValid(void) const;


	// Get the menu size
	UInt32								GetSize(void) const;


	// Clear the menu
	void								Clear(void);


	// Get the tracking data
	MenuTrackingData					GetTracking(void) const;


	// Get/set the attributes
	MenuAttributes						GetAttributes(void) const;
	void								SetAttributes(MenuAttributes setThese, MenuAttributes clearThese=0);
	

	// Append an item
	NMenuItem							AppendItem(const NString &theName, MenuCommand theCmd, MenuItemAttributes theAttributes=0);
	NMenuItem							AppendSeparator(void);


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


