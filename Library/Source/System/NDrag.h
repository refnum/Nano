/*	NAME:
		NDrag.h

	DESCRIPTION:
		Drag action.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDRAG_HDR
#define NDRAG_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEvent.h"
#include "NPasteboard.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NDrag {
public:
										NDrag(const NCarbonEvent &theEvent);
										NDrag(DragRef             theDrag=NULL);

	virtual								~NDrag(void);


	// Is the drag valid?
	bool								IsValid(void) const;


	// Get the drag pasteboard
	NPasteboard							GetPasteboard(void) const;


	// Get the mouse location
	//
	// The position is returned in global or view-relative coordinates.
	CGPoint								GetMouseLocation(HIViewRef relativeTo) const;


	// Get/set the allowable actions
	DragActions							GetAllowableActions(void) const;
	void								SetAllowableActions(DragActions theActions, bool isLocal=false);


	// Get/set the performed actions
	DragActions							GetPerformedActions(void) const;
	void								SetPerformedActions(DragActions theActions);


	// Operators
	operator							DragRef(void) const;


private:
	DragRef								mDrag;
};





#endif // NDRAG_HDR


