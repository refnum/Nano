/*	NAME:
		NPopupArrow.cpp

	DESCRIPTION:
		Popup arrow control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPopupArrow.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
#define kHIPopupArrowClassID					CFSTR("com.apple.hipopuparrow")





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NPopupArrow, kHIPopupArrowClassID, NULL);





//============================================================================
//		NPopupArrow::NPopupArrow : Constructor.
//----------------------------------------------------------------------------
NPopupArrow::NPopupArrow(HIWindowRef						theWindow,
							const HIRect					&theRect,
							ControlPopupArrowOrientation	theOrientation,
							ControlPopupArrowSize			theSize)
{	HIViewRef		theView;
	OSStatus		theErr;



	// Create the view
	theErr = CreatePopupArrowControl(NULL, &kQDRectZero, theOrientation, theSize, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NPopupArrow::~NPopupArrow : Destructor.
//----------------------------------------------------------------------------
NPopupArrow::~NPopupArrow(void)
{
}



