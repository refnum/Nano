/*	NAME:
		NUndoSource.cpp

	DESCRIPTION:
		Objects that generate undos should derive from this class and record or
		track undos based on their state.
		
		The sub-class can either be assigned an NundoManager from some external
		source, or it can override GetUndoManager to dynamically determine the
		undo manager to use.


		The former approach is useful for objects that have no direct connection
		with a view hierarchy, and are owned by some other object which controls
		where undo commands are sent.
		
		The former approach is useful for objects that are part of some kind of
		hierarchy (such as views), where an object may query some kind of chain
		of parents/grand-parents to determine the undo manager.
		
		E.g., a view may return the window's undo manager, which in turn may
		return the application's undo manager.
		
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


	// Initialise ourselves
	mUndoManager = NULL;
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
	return(mUndoManager);
}





//============================================================================
//		NUndoSource::SetUndoManager : Set the undo manager.
//----------------------------------------------------------------------------
void NUndoSource::SetUndoManager(NUndoManager *undoManager)
{


	// Set the undo manager
	mUndoManager = undoManager;
}


