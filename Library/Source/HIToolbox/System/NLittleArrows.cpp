/*	NAME:
		NLittleArrows.cpp

	DESCRIPTION:
		Little arrows control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLittleArrows.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NLittleArrows, kHILittleArrowsClassID, NULL);





//============================================================================
//		NLittleArrows::NLittleArrows : Constructor.
//----------------------------------------------------------------------------
NLittleArrows::NLittleArrows(HIWindowRef		theWindow,
								const HIRect	&theRect,
								SInt32			valueInitial,
								SInt32			valueMinimum,
								SInt32			valueMaximum,
								SInt32			valueIncrement)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateLittleArrowsControl(NULL, &kQDRectZero, valueInitial, valueMinimum,
										valueMaximum, valueIncrement, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NLittleArrows::~NLittleArrows : Destructor.
//----------------------------------------------------------------------------
NLittleArrows::~NLittleArrows(void)
{
}





//============================================================================
//		NLittleArrows::GetIncrementValue : Get the increment value.
//----------------------------------------------------------------------------
SInt32 NLittleArrows::GetIncrementValue(void) const
{


	// Get the value
	return(GetControlUInt32(kControlEntireControl, kControlLittleArrowsIncrementValueTag));
}





//============================================================================
//		NLittleArrows::SetIncrementValue : Set the increment value.
//----------------------------------------------------------------------------
void NLittleArrows::SetIncrementValue(SInt32 theValue)
{


	// Set the value
	SetControlUInt32(kControlEntireControl, kControlLittleArrowsIncrementValueTag, theValue);
}

