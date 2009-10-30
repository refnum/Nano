/*	NAME:
		NMouseUtilities.cpp

	DESCRIPTION:
		Mouse utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NMouseUtilities.h"





//============================================================================
//		NMouseUtilities::GetPosition : Get the mouse position.
//----------------------------------------------------------------------------
HIPoint NMouseUtilities::GetPosition(HICoordinateSpace theSpace, HIViewRef theView, HIWindowRef theWindow)
{	HIPoint		thePoint;
	QDPoint		qdPoint;



	// Validate our parameters
	NN_ASSERT((theView == NULL && theWindow == NULL) ||
			  (theView != NULL && theWindow == NULL) ||
			  (theView == NULL && theWindow != NULL));



	// Get the mouse position
	GetGlobalMouse(&qdPoint);
	NGeometryUtilities::ConvertPoint(qdPoint, thePoint);



	// Convert to the required coordinate space
	//
	// GetGlobalMouse returns mouse coordinates in kHICoordSpace72DPIGlobal.
	if (theView != NULL)
		thePoint = NGeometryUtilities::Transform(thePoint,
													kHICoordSpace72DPIGlobal, NULL,
													kHICoordSpaceView,        theView);

	else if (theWindow != NULL)
		thePoint = NGeometryUtilities::Transform(thePoint,
													kHICoordSpace72DPIGlobal, NULL,
													kHICoordSpaceWindow,      theWindow);
	
	else
		thePoint = NGeometryUtilities::Transform(thePoint,
													kHICoordSpace72DPIGlobal, NULL,
													theSpace,                 NULL);

	return(thePoint);
}





//============================================================================
//		NMouseUtilities::GetDoubleClickTime : Get the double-click time.
//----------------------------------------------------------------------------
EventTime NMouseUtilities::GetDoubleClickTime(void)
{


	// Get the double-click time
	return(TicksToEventTime(GetDblTime()));
}





//============================================================================
//		NMouseUtilities::GetDoubleClickSlop : Get the double-click slop.
//----------------------------------------------------------------------------
float NMouseUtilities::GetDoubleClickSlop(void)
{


	// Get the double-click slop
	return(3.0f);
}
