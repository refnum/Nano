/*	NAME:
		NMouseUtilities.h

	DESCRIPTION:
		Mouse utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMOUSEUTILITIES_HDR
#define NMOUSEUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMouseUtilities {
public:
	// Get the mouse position
	//
	// Returns the mouse position in the given coordinate system, relative
	// to a view or window (of which at most one may be supplied).
	static HIPoint						GetPosition(HICoordinateSpace	theSpace  = kHICoordSpaceScreenPixel,
													HIViewRef			theView   = NULL,
													HIWindowRef			theWindow = NULL);


	// Get the double-click time
	static EventTime					GetDoubleClickTime(void);
};

	
	


#endif // NMOUSEUTILITIES_HDR


