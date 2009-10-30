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
#include "NComparable.h"
#include "NCGImage.h"
#include "NHelpTag.h"
#include "NIcon.h"
#include "NMenu.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const MenuItemAttributes kMenuItemAttrNone						= 0;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMenuItem : public NComparable {
public:
										NMenuItem(const NCarbonEvent &theEvent);
										NMenuItem(const HICommandExtended &theCmd);
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


	// Get/set the checked state
	bool								IsChecked(void) const;
	void								SetChecked(bool isChecked);


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
	void								SetAttributes(MenuItemAttributes setThese, MenuItemAttributes clearThese=kMenuItemAttrNone);
	

	// Get/set the help tag
	NHelpTag							GetHelpTag(void) const;
	void								SetHelpTag(const NHelpTag &theTag);


	// Get/set a property
	//
	// A zero'd result is returned on error.
	bool								GetPropertyBoolean(OSType theCreator, OSType theTag) const;
	UInt32								GetPropertyUInt32( OSType theCreator, OSType theTag) const;
	UInt64								GetPropertyUInt64( OSType theCreator, OSType theTag) const;
	NString								GetPropertyString( OSType theCreator, OSType theTag) const;
	NData								GetPropertyData(   OSType theCreator, OSType theTag) const;
	void							   *GetPropertyPointer(OSType theCreator, OSType theTag) const;

	void								SetPropertyBoolean(OSType theCreator, OSType theTag, bool           theValue);
	void								SetPropertyUInt32( OSType theCreator, OSType theTag, UInt32         theValue);
	void								SetPropertyUInt64( OSType theCreator, OSType theTag, UInt64         theValue);
	void								SetPropertyString( OSType theCreator, OSType theTag, const NString &theValue);
	void								SetPropertyData(   OSType theCreator, OSType theTag, const NData   &theValue);
	void								SetPropertyPointer(OSType theCreator, OSType theTag, void          *theValue);


protected:
	// Compare two objects
	CFComparisonResult					Compare(const NComparable &theObject) const;


private:
	MenuItemIndex						FindMenuItem(MenuRef &theMenu, UInt32 theItem, bool isCmd);


private:
	MenuItemIndex						mIndex;
	NMenu								mMenu;
};



#endif // NMENUITEM_HDR


