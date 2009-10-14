/*	NAME:
		NPlacardView.cpp

	DESCRIPTION:
		Placard view.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPlacardView.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NPlacardView, kHIPlacardViewClassID, NULL);





//============================================================================
//		NPlacardView::NPlacardView : Constructor.
//----------------------------------------------------------------------------
NPlacardView::NPlacardView(HIWindowRef theWindow, const HIRect &theRect)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreatePlacardControl(NULL, &kQDRectZero, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NPlacardView::~NPlacardView : Destructor.
//----------------------------------------------------------------------------
NPlacardView::~NPlacardView(void)
{
}

