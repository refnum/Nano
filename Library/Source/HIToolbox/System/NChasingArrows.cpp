/*	NAME:
		NChasingArrows.cpp

	DESCRIPTION:
		Chasing arrows control.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NChasingArrows.h"





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NChasingArrows, kHIChasingArrowsClassID, NULL);





//============================================================================
//		NChasingArrows::NChasingArrows : Constructor.
//----------------------------------------------------------------------------
NChasingArrows::NChasingArrows(HIWindowRef theWindow, const HIRect &theRect, bool isAnimating)
{	HIViewRef	theView;
	OSStatus	theErr;



	// Create the view
	theErr = CreateChasingArrowsControl(NULL, &kQDRectZero, &theView);
	NN_ASSERT_NOERR(theErr);
	
	ConstructView(theView, theWindow, &theRect);



	// Configure the view
	SetAnimating(isAnimating);
}





//============================================================================
//		NChasingArrows::~NChasingArrows : Destructor.
//----------------------------------------------------------------------------
NChasingArrows::~NChasingArrows(void)
{
}





//============================================================================
//		NChasingArrows::IsAnimating : Is the view animating?
//----------------------------------------------------------------------------
bool NChasingArrows::IsAnimating(void) const
{


	// Get the flag
	return(GetControlBoolean(kControlEntireControl, kControlChasingArrowsAnimatingTag));
}





//============================================================================
//		NChasingArrows::SetAnimating : Set the animating state.
//----------------------------------------------------------------------------
void NChasingArrows::SetAnimating(bool isAnimating)
{


	// Set the flag
	SetControlBoolean(kControlEntireControl, kControlChasingArrowsAnimatingTag, isAnimating);



	// Work around rdar://5037973
	//
	// On Mac OS X 10.4.8, restarting the animation of a chasing arrows control that
	// is smaller than 20x20 fails - the animation state is updated, but the animation
	// itself does not restart.
	//
	// This bug can be worked around by invalidating the view after changing the state.
	SetNeedsDisplay();
}

