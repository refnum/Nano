/*	NAME:
		NRadioButton.cpp

	DESCRIPTION:
		Radio button control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRadioButton.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NRadioButton, kHIRadioButtonClassID, NULL);





//============================================================================
//		NRadioButton::NRadioButton : Constructor.
//----------------------------------------------------------------------------
NRadioButton::NRadioButton(HIWindowRef		theWindow,
							const HIRect	&theRect,
							const NString	&theTitle,
							SInt32			theValue,
							bool			autoToggle)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateRadioButtonControl(NULL, &kQDRectZero, theTitle, theValue, autoToggle, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NRadioButton::~NRadioButton : Destructor.
//----------------------------------------------------------------------------
NRadioButton::~NRadioButton(void)
{
}





//============================================================================
//		NRadioButton::IsSelected : Is the radio button selected?
//----------------------------------------------------------------------------
bool NRadioButton::IsSelected(void) const
{


	// Get the state
	return(GetCurrentValue() != kControlRadioButtonUncheckedValue);
}





//============================================================================
//		NRadioButton::SetSelected : Set the selected state.
//----------------------------------------------------------------------------
void NRadioButton::SetSelected(bool isSelected)
{


	// Set the value
	SetCurrentValue(isSelected ? kControlRadioButtonCheckedValue : kControlRadioButtonUncheckedValue);
}





//============================================================================
//		NRadioButton::SetMixed : Set the mixed state.
//----------------------------------------------------------------------------
void NRadioButton::SetMixed(void)
{


	// Set the value
	SetCurrentValue(kControlRadioButtonMixedValue);
}
