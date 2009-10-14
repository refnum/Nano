/*	NAME:
		NPopupButton.cpp

	DESCRIPTION:
		Popup button control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NSystemUtilities.h"
#include "NMenuUtilities.h"
#include "NCFBundle.h"
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
MenuRef NPopupButton::GetMenu(void) const
{	MenuRef		theMenu;



	// Get the menu
	GetControlData(kControlEntireControl, kControlPopupButtonMenuRefTag, sizeof(theMenu), &theMenu);
	
	return(theMenu);
}





//============================================================================
//		NPopupButton::SetMenu : Set the menu.
//----------------------------------------------------------------------------
void NPopupButton::SetMenu(MenuRef theMenu)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theMenu != NULL);



	// Retain the menu
	//
	// The caller should be able to release their reference, so we retain the menu
	// for the control and transfer ownership with kControlPopupButtonOwnedMenuRefTag.
	theErr = RetainMenu(theMenu);
	NN_ASSERT_NOERR(theErr);



	// Set the menu
	SetControlData(kControlEntireControl, kControlPopupButtonOwnedMenuRefTag, sizeof(theMenu), &theMenu);



	// Update our state
	//
	// The values of a pop-up button correspond to the menu items, so we must adjust
	// our min/max values to match the menu. We also set the current value, so that
	// a pop-up button that's assigned a new menu always starts in a known state.
	SetMaximumValue(CountMenuItems(theMenu));
	SetMinimumValue(1);
	SetCurrentValue(1);
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
{	MenuItemIndex	theValue;
	MenuRef			theMenu;
	


	// Set the value
	theMenu  = GetMenu();
	theValue = NMenuUtilities::GetItemIndex(theMenu, theCmd, true);
	SetCurrentValue(theValue);
}





//============================================================================
//		NPopupButton::GetItemCommand : Get a menu item's command.
//----------------------------------------------------------------------------
MenuCommand NPopupButton::GetItemCommand(MenuItemIndex theIndex)
{	MenuCommand		theCmd;
	OSStatus		theErr;
	
	
	
	// Get the command
	theCmd = 0;
	theErr = GetMenuItemCommandID(GetMenu(), theIndex, &theCmd);
	NN_ASSERT_NOERR(theErr);
	
	return(theCmd);
}





//============================================================================
//		NPopupButton::SetItemCommand : Set a menu item's command.
//----------------------------------------------------------------------------
void NPopupButton::SetItemCommand(MenuItemIndex theIndex, MenuCommand theValue)
{	OSStatus		theErr;
	
	
	
	// Set the command
	theErr = SetMenuItemCommandID(GetMenu(), theIndex, theValue);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPopupButton::GetItemText : Get a menu item's text.
//----------------------------------------------------------------------------
NString NPopupButton::GetItemText(MenuItemIndex theIndex)
{


	// Get the text
	return(NMenuUtilities::GetItemText(GetMenu(), theIndex, false));
}





//============================================================================
//		NPopupButton::SetItemText : Set a menu item's text.
//----------------------------------------------------------------------------
void NPopupButton::SetItemText(MenuItemIndex theIndex, const NString &theValue)
{


	// Set the text
	NMenuUtilities::SetItemText(GetMenu(), theIndex, theValue, false);
}





//============================================================================
//		NPopupButton::GetItemImage : Get a menu item's image.
//----------------------------------------------------------------------------
NCGImage NPopupButton::GetItemImage(MenuItemIndex theIndex)
{	CGImageRef		cgImage;
	UInt8			theType;
	OSStatus		theErr;
	
	
	
	// Get the image
	cgImage = NULL;
	theErr  = GetMenuItemIconHandle(GetMenu(), theIndex, &theType, (Handle *) &cgImage);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(theType == kMenuCGImageRefType);
	
	if (theType != kMenuCGImageRefType)
		cgImage = NULL;
	
	return(cgImage);
}





//============================================================================
//		NPopupButton::SetItemImage : Set a menu item's image.
//----------------------------------------------------------------------------
void NPopupButton::SetItemImage(MenuItemIndex theIndex, const NCGImage &theValue)
{	CGImageRef		cgImage;
	OSStatus		theErr;
	
	
	
	// Set the image
	cgImage = theValue;
	theErr  = SetMenuItemIconHandle(GetMenu(), theIndex, kMenuCGImageRefType, (Handle) cgImage);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NPopupButton::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NPopupButton::InitializeView(void)
{	NCFBundle	appBundle;



	// Apply our properties
	if (HasProperty(                                             kPropertyNano, kTagPopupItem0Image))
		SetItemImage(1,  appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem0Image)));

	if (HasProperty(                                             kPropertyNano, kTagPopupItem1Image))
		SetItemImage(2,  appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem1Image)));

	if (HasProperty(                                             kPropertyNano, kTagPopupItem2Image))
		SetItemImage(3,  appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem2Image)));

	if (HasProperty(                                             kPropertyNano, kTagPopupItem3Image))
		SetItemImage(4,  appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem3Image)));

	if (HasProperty(                                             kPropertyNano, kTagPopupItem4Image))
		SetItemImage(5,  appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem4Image)));

	if (HasProperty(                                             kPropertyNano, kTagPopupItem5Image))
		SetItemImage(6,  appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem5Image)));

	if (HasProperty(                                             kPropertyNano, kTagPopupItem6Image))
		SetItemImage(7,  appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem6Image)));

	if (HasProperty(                                             kPropertyNano, kTagPopupItem7Image))
		SetItemImage(8,  appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem7Image)));

	if (HasProperty(                                             kPropertyNano, kTagPopupItem8Image))
		SetItemImage(9,  appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem8Image)));

	if (HasProperty(                                             kPropertyNano, kTagPopupItem9Image))
		SetItemImage(10, appBundle.GetResource(GetPropertyString(kPropertyNano, kTagPopupItem9Image)));
}



