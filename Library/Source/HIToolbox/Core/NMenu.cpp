/*	NAME:
		NMenu.cpp

	DESCRIPTION:
		MenuRef object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMenuItem.h"
#include "NMenu.h"





//============================================================================
//		NMenu::NMenu : Constructor.
//----------------------------------------------------------------------------
NMenu::NMenu(MenuRef theMenu)
{


	// Initialize ourselves
	AcquireMenu(theMenu);
}





//============================================================================
//		NMenu::NMenu : Constructor.
//----------------------------------------------------------------------------
NMenu::NMenu(const NMenu &theMenu)
		: NContainer()
{


	// Initialize ourselves
	AcquireMenu(theMenu.mMenu);
}





//============================================================================
//		NMenu::NMenu : Constructor.
//----------------------------------------------------------------------------
NMenu::NMenu(void)
{


	// Initialize ourselves
	AcquireMenu(NULL);
}





//============================================================================
//		NMenu::~NMenu : Destructor.
//----------------------------------------------------------------------------
NMenu::~NMenu(void)
{


	// Clean up
	ReleaseMenu();
}





//============================================================================
//		NMenu::IsValid : Is the menu valid?
//----------------------------------------------------------------------------
bool NMenu::IsValid(void) const
{


	// Check our state
	return(mMenu != NULL);
}





//============================================================================
//		NMenu::InRange : Is a menu index in range?
//----------------------------------------------------------------------------
bool NMenu::InRange(MenuItemIndex theIndex) const
{


	// Check the range
	return(theIndex >= 1 && theIndex <= GetSize());
}





//============================================================================
//		NMenu::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NMenu::GetSize(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the size
	return(CountMenuItems(mMenu));
}





//============================================================================
//		NMenu::Clear : Clear the menu.
//----------------------------------------------------------------------------
void NMenu::Clear(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());
	


	// Clear the menu
	theErr = DeleteMenuItems(mMenu, 1, GetSize());

	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenu::GetTracking : Get the tracking data.
//----------------------------------------------------------------------------
MenuTrackingData NMenu::GetTracking(void) const
{	MenuTrackingData	theData;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the tracking data
	theErr = GetMenuTrackingData(mMenu, &theData);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		memset(&theData, 0x00, sizeof(theData));
	
	return(theData);
}





//============================================================================
//		NMenu::GetAttributes : Get the attributes.
//----------------------------------------------------------------------------
MenuAttributes NMenu::GetAttributes(void) const
{	MenuAttributes		theAttributes;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the attributes
	theErr = GetMenuAttributes(mMenu, &theAttributes);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		theAttributes = 0;
	
	return(theAttributes);
}





//============================================================================
//		NMenu::SetAttributes : Set the attributes.
//----------------------------------------------------------------------------
void NMenu::SetAttributes(MenuAttributes setThese, MenuAttributes clearThese)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the attributes
	theErr = ChangeMenuAttributes(mMenu, setThese, clearThese);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenu::AppendItem : Append an item to the menu.
//----------------------------------------------------------------------------
NMenuItem NMenu::AppendItem(const NString &theName, MenuCommand theCmd, MenuItemAttributes theAttributes)
{	MenuItemIndex	theIndex;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Append the item
	theIndex = 0;
	theErr   = AppendMenuItemTextWithCFString(mMenu, theName, theAttributes, theCmd, &theIndex);
	NN_ASSERT_NOERR(theErr);



	// Get the new item
	return(NMenuItem(mMenu, GetSize(), false));
}





//============================================================================
//		NMenu::AppendSeparator : Append a separator to the menu.
//----------------------------------------------------------------------------
NMenuItem NMenu::AppendSeparator(void)
{


	// Append a separator
	return(AppendItem("", 0, kMenuItemAttrSeparator));
}





//============================================================================
//		NMenu::CopyItems : Copy items into the menu.
//----------------------------------------------------------------------------
void NMenu::CopyItems(const NMenu &srcMenu, MenuItemIndex firstItem, UInt32 numItems, MenuItemIndex insertAfter)
{	OSStatus	theErr;



	// Validate our parameters and state
	NN_ASSERT(srcMenu.IsValid());
	NN_ASSERT(srcMenu.InRange(firstItem));
	NN_ASSERT(srcMenu.InRange(firstItem + numItems - 1));
	NN_ASSERT(InRange(insertAfter));
	NN_ASSERT(IsValid());



	// Copy the items
	theErr = CopyMenuItems(srcMenu, firstItem, numItems, mMenu, insertAfter);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMenu::operator = : Assignment operator.
//----------------------------------------------------------------------------
NMenu &NMenu::operator=(const NMenu &otherMenu)
{


	// Assign the menu
	if (this != &otherMenu)
		{
		ReleaseMenu();
		AcquireMenu(otherMenu.mMenu);
		}
		
	return(*this);
}





//============================================================================
//		NMenu::operator MenuRef : MenuRef-cast operator.
//----------------------------------------------------------------------------
NMenu::operator MenuRef(void) const
{


	// Get the menu
	return(mMenu);
}





//============================================================================
//		NMenu::AcquireMenu : Acquire a menu.
//----------------------------------------------------------------------------
#pragma mark -
void NMenu::AcquireMenu(MenuRef theMenu)
{	OSStatus	theErr;



	// Update our state
	mMenu = theMenu;



	// Retain the menu
	if (mMenu != NULL)
		{
		theErr = ::RetainMenu(mMenu);
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NMenu::ReleaseMenu : Release the menu.
//----------------------------------------------------------------------------
void NMenu::ReleaseMenu(void)
{	OSStatus	theErr;



	// Release the menu
	if (mMenu != NULL)
		{
		theErr = ::ReleaseMenu(mMenu);
		NN_ASSERT_NOERR(theErr);
		}



	// Reset our state
	mMenu = NULL;
}

