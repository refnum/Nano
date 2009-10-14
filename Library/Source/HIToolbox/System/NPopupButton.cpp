/*	NAME:
		NPopupButton.cpp

	DESCRIPTION:
		Popup button control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NSystemUtilities.h"
#include "NCFBundle.h"
#include "NMenuItem.h"
#include "NPopupButton.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Misc
static const SInt16 kControlPopupNoMenuID							= -12345;


// Tags
static const OSType kTagPopupItem0Image								= 'img0';
static const OSType kTagPopupItem1Image								= 'img1';
static const OSType kTagPopupItem2Image								= 'img2';
static const OSType kTagPopupItem3Image								= 'img3';
static const OSType kTagPopupItem4Image								= 'img4';
static const OSType kTagPopupItem5Image								= 'img5';
static const OSType kTagPopupItem6Image								= 'img6';
static const OSType kTagPopupItem7Image								= 'img7';
static const OSType kTagPopupItem8Image								= 'img8';
static const OSType kTagPopupItem9Image								= 'img9';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NPopupButton, kHIPopupButtonClassID, NULL);





//============================================================================
//		NPopupButton::NPopupButton : Constructor.
//----------------------------------------------------------------------------
NPopupButton::NPopupButton(HIWindowRef			theWindow,
							const HIRect		&theRect,
							MenuRef				theMenu,
							const NString		&theTitle,
							bool				fixedWidth,
							SInt16				titleWidth,
							SInt16				titleJust)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreatePopupButtonControl(NULL, &kQDRectZero, theTitle, kControlPopupNoMenuID,
										!fixedWidth, titleWidth, titleJust, normal, &theView);
	NN_ASSERT_NOERR(theErr);

	ConstructView(theView, theWindow, &theRect);



	// Configure the view
	if (theMenu != NULL)
		SetMenu(theMenu);
}





//============================================================================
//		NPopupButton::~NPopupButton : Destructor.
//----------------------------------------------------------------------------
NPopupButton::~NPopupButton(void)
{
}





//============================================================================
//		NPopupButton::GetMenu : Get the menu.
//----------------------------------------------------------------------------
NMenu NPopupButton::GetMenu(void) const
{	MenuRef		theMenu;



	// Get the menu
	GetControlData(kControlEntireControl, kControlPopupButtonMenuRefTag, sizeof(theMenu), &theMenu);
	
	return(theMenu);
}





//============================================================================
//		NPopupButton::SetMenu : Set the menu.
//----------------------------------------------------------------------------
void NPopupButton::SetMenu(const NMenu &theMenu)
{	MenuRef		menuRef;
	OSStatus	theErr;


	// Validate our parameters
	NN_ASSERT(theMenu.IsValid());



	// Retain the menu
	//
	// The caller should be able to release their reference, so we retain the menu
	// for the control and transfer ownership with kControlPopupButtonOwnedMenuRefTag.
	menuRef = theMenu;
	theErr  = RetainMenu(menuRef);
	NN_ASSERT_NOERR(theErr);



	// Set the menu
	SetControlData(kControlEntireControl, kControlPopupButtonOwnedMenuRefTag, sizeof(menuRef), &menuRef);



	// Update our state
	//
	// The values of a pop-up button correspond to the menu items, so we must adjust
	// our min/max values to match the menu. We also set the current value, so that
	// a pop-up button that's assigned a new menu always starts in a known state.
	SetMaximumValue(theMenu.GetSize());
	SetMinimumValue(1);
	SetCurrentValue(1);
}





//============================================================================
//      NPopupButton::GetMenuItem : Get a menu item.
//----------------------------------------------------------------------------
NMenuItem NPopupButton::GetMenuItem(UInt32 theItem, bool isCmd) const
{   NMenuItem       menuItem(GetMenu(), theItem, isCmd);



    // Get the menu item
    return(menuItem);
}





//============================================================================
//		NPopupButton::GetExtraheight : Get the extra height.
//----------------------------------------------------------------------------
UInt16 NPopupButton::GetExtraHeight(void) const
{


	// Get the extra height
	return(GetControlUInt16(kControlEntireControl, kControlPopupButtonExtraHeightTag));
}





//============================================================================
//		NPopupButton::SetExtraHeight : Set the extra height.
//----------------------------------------------------------------------------
void NPopupButton::SetExtraHeight(UInt16 theValue)
{


	// Set the extra height
	SetControlUInt16(kControlEntireControl, kControlPopupButtonExtraHeightTag, theValue);
}





//============================================================================
//		NPopupButton::IsSelectionMarked : Is the selected item marked?
//----------------------------------------------------------------------------
bool NPopupButton::IsSelectionMarked(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlPopupButtonCheckCurrentTag));
}





//============================================================================
//		NPopupButton::SetSelectionMarked : Set the selection mark state.
//----------------------------------------------------------------------------
void NPopupButton::SetSelectionMarked(bool isMarked)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlPopupButtonCheckCurrentTag, isMarked);
}





//============================================================================
//		NPopupButton::SetCurrentValueByCommand : Set the current value.
//----------------------------------------------------------------------------
void NPopupButton::SetCurrentValueByCommand(UInt32 theCmd)
{	NMenuItem		menuItem(GetMenu(), theCmd);



	// Set the value
	SetCurrentValue(menuItem.GetIndex());
}





//============================================================================
//		NPopupButton::GetItemCommand : Get a menu item's command.
//----------------------------------------------------------------------------
MenuCommand NPopupButton::GetItemCommand(MenuItemIndex theIndex)
{	NMenuItem		menuItem(GetMenu(), theIndex, false);



	// Get the command
	return(menuItem.GetCommand());
}





//============================================================================
//		NPopupButton::SetItemCommand : Set a menu item's command.
//----------------------------------------------------------------------------
void NPopupButton::SetItemCommand(MenuItemIndex theIndex, MenuCommand theValue)
{	NMenuItem		menuItem(GetMenu(), theIndex, false);



	// Set the command
	menuItem.SetCommand(theValue);
}





//============================================================================
//		NPopupButton::GetItemText : Get a menu item's text.
//----------------------------------------------------------------------------
NString NPopupButton::GetItemText(MenuItemIndex theIndex)
{	NMenuItem		menuItem(GetMenu(), theIndex, false);



	// Get the text
	return(menuItem.GetText());
}





//============================================================================
//		NPopupButton::SetItemText : Set a menu item's text.
//----------------------------------------------------------------------------
void NPopupButton::SetItemText(MenuItemIndex theIndex, const NString &theValue)
{	NMenuItem		menuItem(GetMenu(), theIndex, false);



	// Set the text
	menuItem.SetText(theValue);
}





//============================================================================
//		NPopupButton::GetItemImage : Get a menu item's image.
//----------------------------------------------------------------------------
NCGImage NPopupButton::GetItemImage(MenuItemIndex theIndex)
{	NMenuItem		menuItem(GetMenu(), theIndex, false);



	// Get the image
	return(menuItem.GetImage());
}





//============================================================================
//		NPopupButton::SetItemImage : Set a menu item's image.
//----------------------------------------------------------------------------
void NPopupButton::SetItemImage(MenuItemIndex theIndex, const NCGImage &theValue)
{	NMenuItem		menuItem(GetMenu(), theIndex, false);



	// Set the image
	menuItem.SetImage(theValue);
}





//============================================================================
//		NPopupButton::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NPopupButton::InitializeView(void)
{


	// Apply our properties
	if (HasProperty(                                       kPropertyNano, kTagPopupItem0Image))
		SetItemImage(1,  NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem0Image)));

	if (HasProperty(                                       kPropertyNano, kTagPopupItem1Image))
		SetItemImage(2,  NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem1Image)));

	if (HasProperty(                                       kPropertyNano, kTagPopupItem2Image))
		SetItemImage(3,  NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem2Image)));

	if (HasProperty(                                       kPropertyNano, kTagPopupItem3Image))
		SetItemImage(4,  NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem3Image)));

	if (HasProperty(                                       kPropertyNano, kTagPopupItem4Image))
		SetItemImage(5,  NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem4Image)));

	if (HasProperty(                                       kPropertyNano, kTagPopupItem5Image))
		SetItemImage(6,  NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem5Image)));

	if (HasProperty(                                       kPropertyNano, kTagPopupItem6Image))
		SetItemImage(7,  NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem6Image)));

	if (HasProperty(                                       kPropertyNano, kTagPopupItem7Image))
		SetItemImage(8,  NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem7Image)));

	if (HasProperty(                                       kPropertyNano, kTagPopupItem8Image))
		SetItemImage(9,  NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem8Image)));

	if (HasProperty(                                       kPropertyNano, kTagPopupItem9Image))
		SetItemImage(10, NBundleResource(GetPropertyString(kPropertyNano, kTagPopupItem9Image)));
}



