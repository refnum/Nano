/*	NAME:
		NRelevanceBar.cpp

	DESCRIPTION:
		Relevance bar control.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRelevanceBar.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NRelevanceBar, kHIRelevanceBarClassID, NULL);





//============================================================================
//		NRelevanceBar::NRelevanceBar : Constructor.
//----------------------------------------------------------------------------
NRelevanceBar::NRelevanceBar(HIWindowRef	theWindow,
							const HIRect	&theRect,
							SInt32			valueInitial,
							SInt32			valueMinimum,
							SInt32			valueMaximum)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateRelevanceBarControl(NULL, &kQDRectZero, valueInitial, valueMinimum, valueMaximum, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);
}





//============================================================================
//		NRelevanceBar::~NRelevanceBar : Destructor.
//----------------------------------------------------------------------------
NRelevanceBar::~NRelevanceBar(void)
{
}


