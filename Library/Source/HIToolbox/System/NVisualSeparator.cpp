/*	NAME:
		NVisualSeparator.cpp

	DESCRIPTION:
		Visual separator.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NVisualSeparator.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NVisualSeparator, kHIVisualSeparatorClassID, NULL);





//============================================================================
//		NVisualSeparator::NVisualSeparator : Constructor.
//----------------------------------------------------------------------------
NVisualSeparator::NVisualSeparator(HIWindowRef theWindow, const HIRect &theRect)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateSeparatorControl(NULL, &kQDRectZero, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NVisualSeparator::~NVisualSeparator : Destructor.
//----------------------------------------------------------------------------
NVisualSeparator::~NVisualSeparator(void)
{
}

