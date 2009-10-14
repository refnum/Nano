/*	NAME:
		NMenuItem.cpp

	DESCRIPTION:
		Menu item object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHelpUtilities.h"
#include "NMenuItem.h"





//============================================================================
//		NMenuItem::NMenuItem : Constructor.
//----------------------------------------------------------------------------
NMenuItem::NMenuItem(const NCarbonEvent &theEvent)
{


	// Initialize ourselves
	mIndex = theEvent.GetParameterMenuItemIndex(kEventParamMenuItemIndex);
	mMenu  = theEvent.GetMenu();
}





//============================================================================
//		NMenuItem::NMenuItem : Constructor.
//----------------------------------------------------------------------------
NMenuItem::NMenuItem(MenuRef theMenu, UInt32 theItem, bool isCmd)
{


	// Initialize ourselves
	mMenu  = NULL;
	mIndex = FindMenuItem(theMenu, theItem, isCmd);
	mMenu  = NMenu(theMenu);
}





//============================================================================
//		NMenuItem::NMenuItem : Constructor.
//----------------------------------------------------------------------------
NMenuItem::NMenuItem(UInt32 theCmd)
{	MenuRef		theMenu;



	// Initialize ourselves
	theMenu = NULL;
	mIndex  = FindMenuItem(theMenu, theCmd, true);
	mMenu   = NMenu(theMenu);
}





//============================================================================
//		NMenuItem::NMenuItem : Constructor.
//----------------------------------------------------------------------------
NMenuItem::NMenuItem(void)
{


	// Initialize ourselves
	mIndex = 0;
}





//============================================================================
//		NMenuItem::~NMenuItem : Destructor.
//----------------------------------------------------------------------------
NMenuItem::~NMenuItem(void)
{
}





//============================================================================
//		NMenuItem::IsValid : Is the menu item valid?
//----------------------------------------------------------------------------
bool NMenuItem::IsValid(void) const
{


	// Check our state
	return(mMenu.IsValid() && mIndex != 0);
}





//============================================================================
//		NMenuItem::GetMenu : Get the menu.
//----------------------------------------------------------------------------
NMenu NMenuItem::GetMenu(void) const
{


	// Get the menu
	return(mMenu);
}





//============================================================================
//		NMenuItem::GetIndex : Get the index.
//----------------------------------------------------------------------------
MenuItemIndex NMenuItem::GetIndex(void) const
{


	// Get the index
	return(mIndex);
}





//============================================================================
//		NMenuItem::IsEnabled : Get the enabled state.
//----------------------------------------------------------------------------
bool NMenuItem::IsEnabled(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());
	


	// Get the enabled state
	return(IsMenuItemEnabled(mMenu, mIndex));
}





//============================================================================
//		NMenuItem::SetEnabled : Set the enabled state.
//----------------------------------------------------------------------------
void NMenuItem::SetEnabled(bool isEnabled)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the enabled state
	if (isEnabled)
		EnableMenuItem(mMenu, mIndex);
	else
		DisableMenuItem(mMenu, mIndex);
}





//============================================================================
//		NMenuItem::GetText : Get the text.
//----------------------------------------------------------------------------
NString NMenuItem::GetText(void) const
{	NString			theValue;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the text
	cfString = NULL;
	theErr   = CopyMenuItemTextAsCFString(mMenu, mIndex, &cfString);
	NN_ASSERT_NOERR(theErr);
	
	if (cfString != NULL)
		theValue.Set(cfString);
	
	return(theValue);
}





//============================================================================
//		NMenuItem::SetText : Set the text.
//----------------------------------------------------------------------------
void NMenuItem::SetText(const NString &theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the text
	theErr = SetMenuItemTextWithCFString(mMenu, mIndex, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenuItem::GetCommand : Get the command.
//----------------------------------------------------------------------------
MenuCommand NMenuItem::GetCommand(void) const
{	MenuCommand		theValue;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the command
	theValue = 0;
	theErr   = GetMenuItemCommandID(mMenu, mIndex, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NMenuItem::SetCommand : Set the command.
//----------------------------------------------------------------------------
void NMenuItem::SetCommand(MenuCommand theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the command
	theErr = SetMenuItemCommandID(mMenu, mIndex, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenuItem::GetMark : Get the mark.
//----------------------------------------------------------------------------
UniChar NMenuItem::GetMark(void) const
{	CharParameter	theValue;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the mark
	theValue = 0;
	GetItemMark(mMenu, mIndex, &theValue);
	
	return((UniChar) theValue);
}





//============================================================================
//		NMenuItem::SetMark : Set the mark.
//----------------------------------------------------------------------------
void NMenuItem::SetMark(UniChar theValue)
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Set the mark
	SetItemMark(mMenu, mIndex, (CharParameter) theValue);
}





//============================================================================
//		NMenuItem::GetIndent : Get the indent.
//----------------------------------------------------------------------------
UInt32 NMenuItem::GetIndent(void) const
{	UInt32		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the indent
	theValue = 0;
	theErr   = GetMenuItemIndent(mMenu, mIndex, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NMenuItem::SetIndent : Set the indent.
//----------------------------------------------------------------------------
void NMenuItem::SetIndent(UInt32 theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the indent
	theErr = SetMenuItemIndent(mMenu, mIndex, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenuItem::GetIcon : Get the icon.
//----------------------------------------------------------------------------
NIcon NMenuItem::GetIcon(void) const
{	IconRef		theValue;
	UInt8		theType;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the icon
	theValue = NULL;
	theErr   = GetMenuItemIconHandle(mMenu, mIndex, &theType, (Handle *) &theValue);

	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theType == kMenuIconRefType);
	
	if (theErr != noErr)
		theValue = NULL;
	
	return(theValue);
}





//============================================================================
//		NMenuItem::SetIcon : Set the icon.
//----------------------------------------------------------------------------
void NMenuItem::SetIcon(const NIcon &theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the icon
	theErr = SetMenuItemIconHandle(mMenu, mIndex, kMenuIconRefType, (Handle) ((IconRef) theValue));
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenuItem::GetImage : Get the image.
//----------------------------------------------------------------------------
NCGImage NMenuItem::GetImage(void) const
{	CGImageRef		theValue;
	UInt8			theType;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the image
	theValue = NULL;
	theErr   = GetMenuItemIconHandle(mMenu, mIndex, &theType, (Handle *) &theValue);

	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theType == kMenuCGImageRefType);
	
	if (theErr != noErr)
		theValue = NULL;
	
	return(theValue);
}





//============================================================================
//		NMenuItem::SetImage : Set the image.
//----------------------------------------------------------------------------
void NMenuItem::SetImage(const NCGImage &theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the icon
	theErr = SetMenuItemIconHandle(mMenu, mIndex, kMenuCGImageRefType, (Handle) ((CGImageRef) theValue));
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenuItem::GetSubMenu : Get the sub-menu.
//----------------------------------------------------------------------------
NMenu NMenuItem::GetSubMenu(void) const
{	MenuRef		theValue;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the sub-menu
	theValue = NULL;
	theErr   = GetMenuItemHierarchicalMenu(mMenu, mIndex, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NMenuItem::SetSubMenu : Set the sub-menu.
//----------------------------------------------------------------------------
void NMenuItem::SetSubMenu(const NMenu &theValue)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the sub-menu
	theErr = SetMenuItemHierarchicalMenu(mMenu, mIndex, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenuItem::GetAttributes : Get the attributes.
//----------------------------------------------------------------------------
MenuItemAttributes NMenuItem::GetAttributes(void) const
{	MenuItemAttributes		theValue;
	OSStatus				theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the attributes
	theValue = 0;
	theErr   = GetMenuItemAttributes(mMenu, mIndex, &theValue);
	NN_ASSERT_NOERR(theErr);
	
	return(theValue);
}





//============================================================================
//		NMenuItem::SetAttributes : Set the attributes.
//----------------------------------------------------------------------------
void NMenuItem::SetAttributes(MenuItemAttributes setThese, MenuItemAttributes clearThese)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the attributes
	theErr = ChangeMenuItemAttributes(mMenu, mIndex, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenuItem::GetHelpTag : Get the help tag.
//----------------------------------------------------------------------------
NString NMenuItem::GetHelpTag(bool isExtended) const
{	HMHelpContentRec	helpTag;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the help tag
	helpTag = NHelpUtilities::GetEmptyHelp();
	theErr  = HMGetMenuItemHelpContent(mMenu, mIndex, &helpTag);
	NN_ASSERT(theErr == noErr || theErr == menuPropertyNotFoundErr);

	return(NHelpUtilities::GetHelpString(helpTag, isExtended));
}





//============================================================================
//		NMenuItem::SetHelpTag : Set the help tag.
//----------------------------------------------------------------------------
void NMenuItem::SetHelpTag(const NString &theValue, bool isExtended)
{	HMHelpContentRec	helpTag;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the current value
	helpTag = NHelpUtilities::GetEmptyHelp();
	theErr  = HMGetMenuItemHelpContent(mMenu, mIndex, &helpTag);
	NN_ASSERT(theErr == noErr || theErr == menuPropertyNotFoundErr);
	


	// Set the help tag
	NHelpUtilities::SetHelpString(helpTag, theValue, isExtended);

	theErr = HMSetMenuItemHelpContent(mMenu, mIndex, &helpTag);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenuItem::== : Are two menu items equal?
//----------------------------------------------------------------------------
const bool NMenuItem::operator == (const NMenuItem &theMenu) const
{


	// Compare the items
	return(mIndex == theMenu.mIndex && mMenu == theMenu.mMenu);
}





//============================================================================
//		NMenuItem::!= : Are two menu items not equal?
//----------------------------------------------------------------------------
const bool NMenuItem::operator != (const NMenuItem &theMenu) const
{


	// Compare the items
	return(mIndex != theMenu.mIndex || mMenu != theMenu.mMenu);
}





//============================================================================
//		NMenuItem::FindMenuItem : Find the menu item index.
//----------------------------------------------------------------------------
#pragma mark -
MenuItemIndex NMenuItem::FindMenuItem(MenuRef &theMenu, UInt32 theItem, bool isCmd)
{	MenuItemIndex	theIndex;
	OSStatus		theErr;



	// Get the index
	theIndex = isCmd ? 0 : theItem;
	
	if (isCmd)
		{
		theErr = GetIndMenuItemWithCommandID(theMenu, theItem, 1, &theMenu, &theIndex);
		if (theErr != noErr)
			theIndex = 0;
		}
	
	return(theIndex);
}
