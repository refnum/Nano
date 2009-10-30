/*	NAME:
		NDrag.cpp

	DESCRIPTION:
		Drag action.
	
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
#include "NDrag.h"





//============================================================================
//		NDrag::NDrag : Constructor.
//----------------------------------------------------------------------------
NDrag::NDrag(const NCarbonEvent &theEvent)
{


	// Initialize ourselves
	mDrag = theEvent.GetParameterDrag(kEventParamDragRef);
}





//============================================================================
//		NDrag::NDrag : Constructor.
//----------------------------------------------------------------------------
NDrag::NDrag(DragRef theDrag)
{


	// Initialize ourselves
	mDrag = theDrag;
}





//============================================================================
//		NDrag::~NDrag : Destructor.
//----------------------------------------------------------------------------
NDrag::~NDrag(void)
{
}





//============================================================================
//		NDrag::IsValid : Is the drag valid?
//----------------------------------------------------------------------------
bool NDrag::IsValid(void) const
{


	// Check our state
	return(mDrag != NULL);
}





//============================================================================
//		NDrag::GetPasteboard : Get the pasteboard.
//----------------------------------------------------------------------------
NPasteboard NDrag::GetPasteboard(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the pasteboard
	return(mDrag);
}





//============================================================================
//		NDrag::GetMouseLocation : Get the mouse location.
//----------------------------------------------------------------------------
CGPoint NDrag::GetMouseLocation(HIViewRef relativeTo) const
{	CGPoint			thePoint;
	Point			qdPoint;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the state we need
	theErr = GetDragMouse(mDrag, NULL, &qdPoint);
	NN_ASSERT_NOERR(theErr);



	// Get the mouse location
	NGeometryUtilities::ConvertPoint(qdPoint, thePoint);
	
	if (relativeTo != NULL)
		thePoint = NGeometryUtilities::Transform(thePoint, kHICoordSpace72DPIGlobal, NULL, kHICoordSpaceView, relativeTo);

	return(thePoint);
}





//============================================================================
//		NDrag::GetAllowableActions : Get the allowable actions.
//----------------------------------------------------------------------------
DragActions NDrag::GetAllowableActions(void) const
{	DragActions		theActions;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the actions
	theActions = kDragActionNothing;
	theErr     = GetDragAllowableActions(mDrag, &theActions);
	NN_ASSERT_NOERR(theErr);
	
	return(theActions);
}





//============================================================================
//		NDrag::SetAllowableActions : Set the allowable actions.
//----------------------------------------------------------------------------
void NDrag::SetAllowableActions(DragActions theActions, bool isLocal)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the actions
	theErr = SetDragAllowableActions(mDrag, theActions, isLocal);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDrag::GetPerformedActions : Get the performed actions.
//----------------------------------------------------------------------------
DragActions NDrag::GetPerformedActions(void) const
{	DragActions		theActions;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the actions
	theActions = kDragActionNothing;
	theErr     = GetDragDropAction(mDrag, &theActions);
	NN_ASSERT_NOERR(theErr);
	
	return(theActions);
}





//============================================================================
//		NDrag::SetPerformedActions : Set the performed actions.
//----------------------------------------------------------------------------
void NDrag::SetPerformedActions(DragActions theActions)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsValid());



	// Set the actions
	theErr = SetDragDropAction(mDrag, theActions);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NDrag::operator DragRef : DragRef-cast operator.
//----------------------------------------------------------------------------
NDrag::operator DragRef(void) const
{


	// Get the drag
	return(mDrag);
}


