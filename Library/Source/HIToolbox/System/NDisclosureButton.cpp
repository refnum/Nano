/*	NAME:
		NDisclosureButton.cpp

	DESCRIPTION:
		Disclosure button control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDisclosureButton.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NDisclosureButton, kHIDisclosureButtonClassID, NULL);





//============================================================================
//		NDisclosureButton::NDisclosureButton : Constructor.
//----------------------------------------------------------------------------
NDisclosureButton::NDisclosureButton(HIWindowRef theWindow, const HIRect &theRect, UInt32 theValue, bool autoToggle)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateDisclosureButtonControl(NULL, &kQDRectZero, theValue, autoToggle, &theView);
	NN_ASSERT_NOERR(theErr);

	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NDisclosureButton::~NDisclosureButton : Destructor.
//----------------------------------------------------------------------------
NDisclosureButton::~NDisclosureButton(void)
{
}





//============================================================================
//		NDisclosureButton::IsOpen : Is the button in the open state?
//----------------------------------------------------------------------------
bool NDisclosureButton::IsOpen(void) const
{


	// Get the state
	return(GetCurrentValue() != kControlDisclosureButtonClosed);
}





//============================================================================
//		NDisclosureButton::SetOpen : Set the open state.
//----------------------------------------------------------------------------
void NDisclosureButton::SetOpen(bool isOpen)
{


	// Set the value
	SetCurrentValue(isOpen ? kControlDisclosureButtonDisclosed : kControlDisclosureButtonClosed);
}
