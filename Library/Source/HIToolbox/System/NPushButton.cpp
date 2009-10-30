/*	NAME:
		NPushButton.cpp

	DESCRIPTION:
		Push button control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPushButton.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NPushButton, kHIPushButtonClassID, NULL);





//============================================================================
//		NPushButton::NPushButton : Constructor.
//----------------------------------------------------------------------------
NPushButton::NPushButton(HIWindowRef		theWindow,
							const HIRect	&theRect,
							const NString	&theTitle,
							UInt32			theCmd)
{	HIViewRef		theView;
	OSStatus		theErr;



	// Create the view
	theErr = CreatePushButtonControl(NULL, &kQDRectZero, theTitle, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);



	// Configure the view
	SetCommandID(theCmd);
}





//============================================================================
//		NPushButton::~NPushButton : Destructor.
//----------------------------------------------------------------------------
NPushButton::~NPushButton(void)
{
}





//============================================================================
//		NPushButton::IsDefault : Is this a default button?
//----------------------------------------------------------------------------
bool NPushButton::IsDefault(void) const
{


	// Get the state
	return(GetControlBoolean(kControlEntireControl, kControlPushButtonDefaultTag));
}





//============================================================================
//		NPushButton::SetDefault : Set the default button state.
//----------------------------------------------------------------------------
void NPushButton::SetDefault(bool isDefault)
{	NHIWindow	theWindow(GetHIWindow());



	// Set the state
	SetControlBoolean(kControlEntireControl, kControlPushButtonDefaultTag, isDefault);



	// Update the window
	if (theWindow.IsValid())
		theWindow.SetDefaultButton(isDefault ? this : NULL);
}





//============================================================================
//		NPushButton::IsCancel : Is this a cancel button?
//----------------------------------------------------------------------------
bool NPushButton::IsCancel(void) const
{


	// Get the state
	return(GetControlBoolean(kControlEntireControl, kControlPushButtonCancelTag));
}





//============================================================================
//		NPushButton::SetCancel : Set the cancel button state.
//----------------------------------------------------------------------------
void NPushButton::SetCancel(bool isCancel)
{	NHIWindow	theWindow(GetHIWindow());



	// Set the state
	SetControlBoolean(kControlEntireControl, kControlPushButtonCancelTag, isCancel);



	// Update the window
	if (theWindow.IsValid())
		theWindow.SetCancelButton(isCancel ? this : NULL);
}

