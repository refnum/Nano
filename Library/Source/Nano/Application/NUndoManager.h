/*	NAME:
		NUndoManager.h

	DESCRIPTION:
		Undo manager.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
#ifndef NUNDO_MANAGER_H
#define NUNDO_MANAGER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NBroadcaster.h"
#include "NString.h"

// System
#include <vector>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
inline constexpr const char* kNUndoManagerUpdated           = "NUndoManager.Updated";





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Undo Action
using NFunctionUndoAction                                   = std::function<void()>;
using NVectorUndoAction                                     = std::vector<NFunctionUndoAction>;


// Undo Group
struct NUndoGroup
{
	NString           theName;
	NVectorUndoAction theActions;
};

using NVectorUndoGroup = std::vector<NUndoGroup>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NUndoManager : public NBroadcaster
{
public:
										NUndoManager();


	// Clear the undo state
	void                                Clear();


	// Can an undo be performed?
	bool                                CanUndo() const;
	bool                                CanRedo() const;


	// Perform an undo
	void                                PerformUndo();
	void                                PerformRedo();


	// Is an undo being performed?
	bool                                IsUndoing() const;
	bool                                IsRedoing() const;


	// Enable/disable undo recording
	//
	// Calls to RecordUndo will have no effect when recording is disabled.
	bool                                IsRecording() const;
	void                                SetRecording(bool canRecord);


	// Get/set the undo limit
	//
	// The undo limit controls the maximum number of groups that will be saved
	// to the undo/redo stacks. A limit of 0 removes the limit entirely.
	size_t                              GetUndoLimit() const;
	void                                SetUndoLimit(size_t theLimit);


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
	NString                             GetUndoName() const;
	NString                             GetRedoName() const;


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
	void                                BeginGroup();
	void                                EndGroup(bool flushGroup = false);


	// Set the group name
	//
	// Each undo group has an associated name, used to adjust the appearance of
	// "Undo XXX" or "Redo XXX" menu commands.
	//
	// When a group is closed, the current value is used to name the group before
	// the value is cleared for future groups.
	void                                SetGroupName(const NString& theName);


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
	void                                RecordAction(const NFunctionUndoAction& theAction);


private:
	void                                UpdatedStacks();


private:
	bool                                mIsUndoing;
	bool                                mIsRedoing;
	bool                                mIsRecording;

	bool                                mGroupOpen;
	size_t                              mUndoLimit;
	NUndoGroup                          mCurrentGroup;

	NVectorUndoGroup                    mStackUndo;
	NVectorUndoGroup                    mStackRedo;
};



#endif // NUNDO_MANAGER_H
