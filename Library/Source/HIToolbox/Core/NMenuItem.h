/*	NAME:
		NMenuItem.h

	DESCRIPTION:
		Menu item object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMENUITEM_HDR
#define NMENUITEM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEvent.h"
#include "NCGImage.h"
#include "NIcon.h"
#include "NMenu.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMenuItem {
public:
										NMenuItem(const NCarbonEvent &theEvent);
										NMenuItem(MenuRef theMenu, UInt32 theItem, bool isCmd=true);
										NMenuItem(UInt32 theCmd);

										NMenuItem(void);
	virtual								~NMenuItem(void);


	// Is the menu item valid?
	bool								IsValid(void) const;


	// Get the menu/index
	NMenu								GetMenu( void) const;
	MenuItemIndex						GetIndex(void) const;


	// Get/set the enabled state
	bool								IsEnabled(void) const;
	void								SetEnabled(bool isEnabled);


	// Get/set the text
	NString								GetText(void) const;
	void								SetText(const NString &theValue);


	// Get/set the command
	MenuCommand							GetCommand(void) const;
	void								SetCommand(MenuCommand theValue);


	// Get/set the mark
	UniChar								GetMark(void) const;
	void								SetMark(UniChar theValue);


	// Get/set the indent
	UInt32								GetIndent(void) const;
	void								SetIndent(UInt32 theValue);


	// Get/set the icon
	NIcon								GetIcon(void) const;
	void								SetIcon(const NIcon &theValue);
	

	// Get/set the image
	NCGImage							GetImage(void) const;
	void								SetImage(const NCGImage &theValue);


	// Get/set the sub-menu
	NMenu								GetSubMenu(void) const;
	void								SetSubMenu(const NMenu &theValue);


	// Get/set the attributes
	MenuItemAttributes					GetAttributes(void) const;
	void								SetAttributes(MenuItemAttributes setThese, MenuItemAttributes clearThese=0);
	

	// Get/set the help tag
	NString								GetHelpTag(                         bool isExtended=false) const;
	void								SetHelpTag(const NString &theValue, bool isExtended=false);


	// Operators
	const bool							operator == (const NMenuItem &theItem) const;
	const bool							operator != (const NMenuItem &theItem) const;


private:
	MenuItemIndex						FindMenuItem(MenuRef &theMenu, UInt32 theItem, bool isCmd);


private:
	MenuItemIndex						mIndex;
	NMenu								mMenu;
};



#endif // NMENUITEM_HDR


