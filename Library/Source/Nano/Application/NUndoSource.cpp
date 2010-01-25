/*	NAME:
		NUndoSource.cpp

	DESCRIPTION:
		Objects that generate undos should derive from this class and provide
		an implementation of GetUndoManager.
		
		This may involve walking up a hierarchy of objects, e.g., so that views
		can return the undo manager of their window (which may in turn query
		the application to obtain a global undo manager).
		
		Window-specific undo managers are also supported, where a suitable parent
		(e.g., a document) can provide an undo manager for their context.
		
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBundle.h"
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
	
	 if (stringsAreKeys && !theName.IsEmpty())
		theValue = NBundleString(theName, theName);



	// Record the action
	if (undoManager != NULL)
		{
		if (!theValue.IsEmpty())
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
{


	// Get the undo manager
	return(NULL);
}

