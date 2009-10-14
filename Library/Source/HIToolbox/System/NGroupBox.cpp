/*	NAME:
		NGroupBox.cpp

	DESCRIPTION:
		Group box control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGroupBox.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NGroupBox,      kHIGroupBoxClassID,      NULL);
DEFINE_HIVIEW_ALTCLASS(NCheckBoxGroup, kHICheckBoxGroupClassID, NULL, NGroupBox);





//============================================================================
//		NGroupBox::NGroupBox : Constructor.
//----------------------------------------------------------------------------
NGroupBox::NGroupBox(HIWindowRef		theWindow,
						const HIRect	&theRect,
						const NString	&theTitle,
						bool			isPrimary)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateGroupBoxControl(NULL, &kQDRectZero, theTitle, isPrimary, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NGroupBox::NGroupBox : Constructor.
//----------------------------------------------------------------------------
NGroupBox::NGroupBox(HIWindowRef		theWindow,
						const HIRect	&theRect,
						const NString	&theTitle,
						bool			isPrimary,
						SInt32			theValue,
						bool			autoToggle)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateCheckGroupBoxControl(NULL, &kQDRectZero, theTitle, theValue, isPrimary, autoToggle, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NGroupBox::NGroupBox : Constructor.
//----------------------------------------------------------------------------
NGroupBox::NGroupBox(HIWindowRef		theWindow,
						const HIRect	&theRect,
						const NString	&theTitle,
						bool			isPrimary,
						bool			variableWidth,
						SInt16			titleWidth,
						SInt16			titleJust)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreatePopupGroupBoxControl(NULL, &kQDRectZero, theTitle, isPrimary, 0, variableWidth,
										titleWidth, titleJust, normal, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NGroupBox::~NGroupBox : Destructor.
//----------------------------------------------------------------------------
NGroupBox::~NGroupBox(void)
{
}





//============================================================================
//		NGroupBox::GetTitleBox : Get the title box for the group box.
//----------------------------------------------------------------------------
HIRect NGroupBox::GetTitleBox(void) const
{	HIRect	theRect;
	Rect	qdRect;



	// Get the title box
	GetControlData(kControlEntireControl, kControlGroupBoxTitleRectTag, sizeof(qdRect), &qdRect);
	NGeometryUtilities::ConvertRect(qdRect, theRect);
	
	return(theRect);
}





//============================================================================
//		NGroupBox::GetMenu : Get the menu.
//----------------------------------------------------------------------------
MenuRef NGroupBox::GetMenu(void) const
{	MenuRef		theMenu;



	// Get the menu
	GetControlData(kControlEntireControl, kControlGroupBoxMenuRefTag, sizeof(theMenu), &theMenu);
	
	return(theMenu);
}





//============================================================================
//		NGroupBox::SetMenu : Set the menu.
//----------------------------------------------------------------------------
void NGroupBox::SetMenu(MenuRef theMenu)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theMenu != NULL);



	// Retain the menu
	theErr = RetainMenu(theMenu);
	NN_ASSERT_NOERR(theErr);



	// Set the menu
	SetControlData(kControlEntireControl, kControlGroupBoxMenuRefTag, sizeof(theMenu), &theMenu);
}

