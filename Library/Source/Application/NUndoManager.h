/*	NAME:
		NUndoManager.h

	DESCRIPTION:
		Undo manager.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NUNDOMANAGER_HDR
#define NUNDOMANAGER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <list>

#include "NCarbonEventHandler.h"
#include "NMsgBroadcaster.h"
#include "NFunctor.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const BroadcastMsg kMsgUndoManagerUpdated					= 'undo';





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Undo group
typedef struct {
	NString				theName;
	NFunctorList		theActions;
} UndoGroup;


// Lists
typedef std::list<UndoGroup>										UndoGroupList;
typedef UndoGroupList::iterator										UndoGroupListIterator;
typedef UndoGroupList::const_iterator								UndoGroupListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NUndoManager : public NCarbonEventHandler, public NMsgBroadcaster {
public:
										NUndoManager( void);
	virtual								~NUndoManager(void);


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


	// Set the action name
	//
	// Each undo group has an associated name, used to adjust the appearance of
	// kHICommandUndo/kHICommandRedo menu commands.
	//
	// When a group is closed, the current action name is used to name the group
	// before the name is cleared for future groups.
	void								SetActionName(const NString &theName);


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


protected:
	// Handle events
	OSStatus							DoEventCommandProcess(NCarbonEvent &theEvent);


	// Get the status of a command
	CommandStatus						GetCommandStatus(const HICommandExtended &hiCmd);


private:
	void								UpdatedStacks(void);

	NString								GetCommandString(UInt32 theCmd, CommandStatus theStatus);
	NString								GetUndoName(void);
	NString								GetRedoName(void);


private:
	bool								mIsUndoing;
	bool								mIsRedoing;
	bool								mIsRecording;

	UInt32								mUndoLimit;
	bool								mGroupOpen;
	UndoGroup							mCurrentGroup;
	
	UndoGroupList						mStackUndo;
	UndoGroupList						mStackRedo;
};





#endif // NUNDOMANAGER_HDR
