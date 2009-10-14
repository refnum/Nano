/*	NAME:
		NRadioGroup.cpp

	DESCRIPTION:
		Radio button group control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRadioGroup.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NRadioGroup, kHIRadioGroupClassID, NULL);





//============================================================================
//		NRadioGroup::NRadioGroup : Constructor.
//----------------------------------------------------------------------------
NRadioGroup::NRadioGroup(HIWindowRef theWindow, const HIRect &theRect)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateRadioGroupControl(NULL, &kQDRectZero, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NRadioGroup::~NRadioGroup : Destructor.
//----------------------------------------------------------------------------
NRadioGroup::~NRadioGroup(void)
{
}

