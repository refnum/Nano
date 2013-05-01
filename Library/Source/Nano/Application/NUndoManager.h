/*	NAME:
		NUndoManager.h

	DESCRIPTION:
		Undo manager.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUNDOMANAGER_HDR
#define NUNDOMANAGER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBroadcaster.h"
#include "NFunctor.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const NBroadcastMsg kMsgNUndoManagerUpdated					= 0x756E646F;	// 'undo'





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Undo group
typedef struct {
	NString				theName;
	NFunctorList		theActions;
} NUndoGroup;


// Lists
typedef std::list<NUndoGroup>										NUndoGroupList;
typedef NUndoGroupList::iterator									NUndoGroupListIterator;
typedef NUndoGroupList::const_iterator								NUndoGroupListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUndoManager : public NBroadcaster {
public:
										NUndoManager(void);
	virtual							   ~NUndoManager(void);


	// Clear the undo state
	void								Clear(void);


	// Can an undo be performed?
	bool								CanUndo(void) const;
	bool								CanRedo(void) const;


	// Perform an undo
	void								PerformUndo(void);
	void								PerformRedo(void);


	// Is an undo being performed?
	bool								IsUndoing(void) const;
	bool								IsRedoing(void) const;


	// Enable/disable undo recording
	//
	// Calls to RecordUndo will have no effect when recording is disabled.
	bool								IsRecording(void) const;
	void								SetRecording(bool canRecord);


	// Get/set the undo limit
	//
	// The undo limit controls the maximum number of groups that will be saved
	// to the undo/redo stacks. A limit of 0 removes the limit entirely.
	UInt32								GetUndoLimit(void) const;
	void								SetUndoLimit(UInt32 theLimit);


	// Get the current undo/redo name
	//
	// Returns the name of the most recent undo/redo group, if any.
	//
	// For some languages the name may be sufficient to localise the UI by simple
	// text substitution:
	//
	//		NString		menuText;
	//
	//		menuText.Format("Undo %@", GetUndoName());
	//
	// Other languages may require a more complex substitution, where the 'name'
	// is used to construct an action-specific key that identifies an undo/redo
	// string:
	//
	//		NString		menuText, theKey;
	//
	//		theKey   = "Undo_" + GetUndoName();
	//		menuText = NBundleString(theKey);
	//
	//			"Undo_Rename"  => "Undoify Renamen"
	//			"Undo_Archive" => "Archivius Maximums"
	//
	// In this fictional language an undo command is written as 'verb noun', but
	// a redo command is written as 'noun verb'.
	NString								GetUndoName(void) const;
	NString								GetRedoName(void) const;


	// Begin/end an undo group
	//
	// The undo/redo stacks contain a list of groups, where each group contains
	// one or more actions to perform.
	//
	// Groups can be opened/closed explicitly to group multiple actions together
	// into one undo-able group, but may not be nested.
	//
	//
	// Groups can also be "flushed" when closed, which discards all but the first
	// item in the group.
	//
	// This can be used to consolidate mouse-driven undo, where a large number of
	// undo actions are recorded but only the first action needs to be retained
	// to perform an undo.
	void								BeginGroup(void);
	void								EndGroup(bool flushGroup=false);


	// Set the group name
	//
	// Each undo group has an associated name, used to adjust the appearance of
	// "Undo XXX" or "Redo XXX" menu commands.
	//
	// When a group is closed, the current value is used to name the group before
	// the value is cleared for future groups.
	void								SetGroupName(const NString &theName);


	// Record an undo action
	//
	// Changes that can be undone should be preceded by recording an action that
	// will, when executed, revert the upcoming change.
	//
	// For example, one-line undo can be achieved with:
	//
	//		void CMyClass::SetPosition(const HIPoint &thePosition)
	//		{
	//			mUndoManager.RecordAction(BindSelf(CMyClass::SetPosition, mPosition));
	//			mPosition = thePosition;
	//		}
	//
	// Since SetPosition records an action that will apply the current state, undo
	// is achieved by executing that action (thus reverting the change).
	//
	// Since NUndoManager keeps track of when actions are recorded within an undo
	// or redo operation, it can ensure that the subsequent call to RecordAction
	// is added to the correct stack.
	//
	//
	// Note that since actions are arbitrary functors, any non-static method can
	// be captured as the action. When the action is executed, the method will be
	// invoked on the object that was bound into the functor. 
	//
	// If no group is open when an action is recorded, a new group will be opened
	// closed around the action automatically.
	void								RecordAction(const NFunctor &theAction);


private:
	void								UpdatedStacks(void);


private:
	bool								mIsUndoing;
	bool								mIsRedoing;
	bool								mIsRecording;
	
	bool								mGroupOpen;
	UInt32								mUndoLimit;
	NUndoGroup							mCurrentGroup;
	
	NUndoGroupList						mStackUndo;
	NUndoGroupList						mStackRedo;
};





#endif // NUNDOMANAGER_HDR
