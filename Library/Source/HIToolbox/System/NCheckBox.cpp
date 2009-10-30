/*	NAME:
		NCheckBox.cpp

	DESCRIPTION:
		Check box control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCheckBox.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NCheckBox, kHICheckBoxClassID, NULL);





//============================================================================
//		NCheckBox::NCheckBox : Constructor.
//----------------------------------------------------------------------------
NCheckBox::NCheckBox(HIWindowRef		theWindow,
						const HIRect	&theRect,
						const NString	&theTitle,
						SInt32			theValue,
						bool			autoToggle)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateCheckBoxControl(NULL, &kQDRectZero, theTitle, theValue, autoToggle, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NCheckBox::~NCheckBox : Destructor.
//----------------------------------------------------------------------------
NCheckBox::~NCheckBox(void)
{
}





//============================================================================
//		NCheckBox::IsChecked : Is the check box checked?
//----------------------------------------------------------------------------
bool NCheckBox::IsChecked(void) const
{


	// Get the status
	return(GetCurrentValue() != kControlCheckBoxUncheckedValue);
}





//============================================================================
//		NCheckBox::SetChecked : Set the checked state.
//----------------------------------------------------------------------------
void NCheckBox::SetChecked(bool isChecked)
{


	// Set the value
	SetCurrentValue(isChecked ? kControlCheckBoxCheckedValue : kControlCheckBoxUncheckedValue);
}





//============================================================================
//		NCheckBox::SetMixed : Set the mixed state.
//----------------------------------------------------------------------------
void NCheckBox::SetMixed(void)
{


	// Set the value
	SetCurrentValue(kControlCheckBoxMixedValue);
}
