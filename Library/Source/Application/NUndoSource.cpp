/*	NAME:
		NUndoSource.cpp

	DESCRIPTION:
		Objects that generate undos should derive from this class to locate
		their undo undo manager.
		
		The undo manager is determined by sending a kEventNAppGetUndoManager,
		which is dispatched up the target hierarchy to locate the most
		appropriate undo manager.

		
		E.g., views re-send this event to their window, and windows send it
		to the application - which returns a global undo manager.
		
		To support window-specific undo managers (e.g., for documents), the
		window controller class intercepts the event between the window and
		the application in order to return their own undo manager instead.
		
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"
#include "NUndoSource.h"





//============================================================================
//		NUndoSource::NUndoSource : Constructor.
//----------------------------------------------------------------------------
NUndoSource::NUndoSource(void)
{
}





//============================================================================
//		NUndoSource::~NUndoSource : Destructor.
//----------------------------------------------------------------------------
NUndoSource::~NUndoSource(void)
{
}





//============================================================================
//		NUndoSource::ClearUndo : Clear the undo stack.
//----------------------------------------------------------------------------
void NUndoSource::ClearUndo(void)
{	NUndoManager	*undoManager;



	// Get the state we need
	undoManager = GetUndoManager();
	
	
	
	// Clear the undo stack
	if (undoManager != NULL)
		undoManager->Clear();
}





//============================================================================
//		NUndoSource::RecordUndo : Record an undo action.
//----------------------------------------------------------------------------
void NUndoSource::RecordUndo(const NFunctor	&theAction, const NString &theName, bool stringsAreKeys)
{	NUndoManager	*undoManager;
	NString			theValue;



	// Get the state we need
	undoManager = GetUndoManager();
	theValue    = theName;
	
	 if (stringsAreKeys && theName.IsNotEmpty())
		theValue = NBundleString(theName, theName);



	// Record the action
	if (undoManager != NULL)
		{
		if (theValue.IsNotEmpty())
			undoManager->SetActionName(theValue);

		undoManager->RecordAction(theAction);
		}
}





//============================================================================
//		NUndoSource::TrackUndo : Track an undo group.
//----------------------------------------------------------------------------
void NUndoSource::TrackUndo(bool beginTrack, bool canFlush)
{	NUndoManager	*undoManager;



	// Get the state we need
	undoManager = GetUndoManager();



	// Track the undo
	if (undoManager != NULL)
		{
		if (beginTrack)
			undoManager->BeginGroup();
		else
			undoManager->EndGroup(canFlush);
		}
}





//============================================================================
//		NUndoSource::GetUndoManager : Get the undo manager.
//----------------------------------------------------------------------------
#pragma mark -
NUndoManager *NUndoSource::GetUndoManager(void) const
{	NCarbonEvent				theEvent(kEventClassNApplication, kEventNAppGetUndoManager);
	NCarbonEventHandler			*eventHandler;
	NUndoManager				*undoManager;
	OSStatus					theErr;



	// Get the state we need
	eventHandler = GetUndoTarget();



	// Get the undo manager
	//
	// By default, the undo manager is obtained by sending an event to an event
	// handler. Sub-classes can override this method to use an alternative source.
	//
	// Note that the undo manager may not be accessible; a view that is not in
	// a window will be unable to retrieve the window's undo manager.
	theErr = eventHandler->DispatchEvent(theEvent);
	NN_ASSERT(theErr == noErr || theErr == eventNotHandledErr);
	
	undoManager = (NUndoManager *) theEvent.GetParameterPointer(kEventParamDirectObject);

	return(undoManager);
}





//============================================================================
//		NUndoSource::GetUndoTarget : Get the undo target.
//----------------------------------------------------------------------------
NCarbonEventHandler *NUndoSource::GetUndoTarget(void) const
{	NCarbonEventHandler			*eventHandler;



	// Get the undo target
	//
	// By default, undo sources are assumed to be an event handler and thus can
	// be used as the undo target.
	//
	// Sub-classes can override this method to identify an alternative event
	// handler to provide the undo manager.
	eventHandler = dynamic_cast<NCarbonEventHandler*>((NUndoSource *) this);
	NN_ASSERT(eventHandler != NULL);

	return(eventHandler);
}


