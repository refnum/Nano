/*	NAME:
		NUndoSource.h

	DESCRIPTION:
		Undo source object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUNDOSOURCE_HDR
#define NUNDOSOURCE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUndoManager.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUndoSource {
public:
										NUndoSource(void);
	virtual							   ~NUndoSource(void);


	// Clear the undo stack
	void								ClearUndo(void);


	// Record an undo action
	void								RecordUndo(const NFunctor	&theAction,
												   const NString	&theName       = "",
												   bool				stringsAreKeys = true);


	// Track an undo group
	//
	// Undo-able actions that are driven by mouse-tracking (e.g., dragging an object,
	// or using a color picker) should typically be grouped into a single operation
	// so that the user can undo/redo the entire sequence atomically.
	//
	// Since these operations can typically be represented by the first action in the
	// group, the group may also be flushed to minimize the number of actions that are
	// saved to the undo stacks.
	void								TrackUndo(bool beginTrack, bool canFlush=true);


	// Get/set the undo manager
	//
	// Sub-classes can override to determine the current undo manager dynamically.
	virtual NUndoManager			   *GetUndoManager(void) const;
	void								SetUndoManager(NUndoManager *undoManager);


private:
	NUndoManager					   *mUndoManager;
};





#endif // NUNDOSOURCE_HDR


