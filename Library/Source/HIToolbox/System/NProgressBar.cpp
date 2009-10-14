/*	NAME:
		NProgressBar.cpp

	DESCRIPTION:
		Progress bar control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NProgressBar.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NProgressBar, kHIProgressBarClassID, NULL);





//============================================================================
//		NProgressBar::NProgressBar : Constructor.
//----------------------------------------------------------------------------
NProgressBar::NProgressBar(HIWindowRef		theWindow,
							const HIRect	&theRect,
							SInt32			valueInitial,
							SInt32			valueMinimum,
							SInt32			valueMaximum,
							bool			isIndeterminate)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateProgressBarControl(NULL, &kQDRectZero, valueInitial, valueMinimum,
										valueMaximum, isIndeterminate, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NProgressBar::~NProgressBar : Destructor.
//----------------------------------------------------------------------------
NProgressBar::~NProgressBar(void)
{
}





//============================================================================
//		NProgressBar::IsIndeterminate : Is this an indeterminate progress bar?
//----------------------------------------------------------------------------
bool NProgressBar::IsIndeterminate(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlProgressBarIndeterminateTag));
}





//============================================================================
//		NProgressBar::SetIndeterminate : Set the indeterminate state.
//----------------------------------------------------------------------------
void NProgressBar::SetIndeterminate(bool isIndeterminate)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlProgressBarIndeterminateTag, isIndeterminate);
}


