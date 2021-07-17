/*	NAME:
		NUndoManager.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NUndoManager.h"

// Nano
#include "NStdAlgorithm.h"





//=============================================================================
//		NUndoManager::NUndoManager : Constructor.
//-----------------------------------------------------------------------------
NUndoManager::NUndoManager()
	: mIsUndoing(false)
	, mIsRedoing(false)
	, mIsRecording(false)
	, mGroupOpen(false)
	, mUndoLimit(0)
{
}





//=============================================================================
//		NUndoManager::Clear : Clear the undo stacks.
//-----------------------------------------------------------------------------
void NUndoManager::Clear()
{


	// Validate our state
	NN_REQUIRE(!mIsUndoing && !mIsRedoing);



	// Clear the undo stacks
	mStackUndo.clear();
	mStackRedo.clear();

	UpdatedStacks();
}





//=============================================================================
//		NUndoManager::CanUndo : Can we undo?
//-----------------------------------------------------------------------------
bool NUndoManager::CanUndo() const
{


	// Can we undo?
	return !mStackUndo.empty();
}





//=============================================================================
//		NUndoManager::CanRedo : Can we redo?
//-----------------------------------------------------------------------------
bool NUndoManager::CanRedo() const
{


	// Can we redo?
	return !mStackRedo.empty();
}





//=============================================================================
//		NUndoManager::PerformUndo : Perform an undo.
//-----------------------------------------------------------------------------
void NUndoManager::PerformUndo()
{


	// Validate our state
	NN_REQUIRE(!mIsUndoing && !mIsRedoing);
	NN_REQUIRE(!mStackUndo.empty());



	// Prepare to undo
	mIsUndoing = true;

	NUndoGroup theGroup = nstd::extract_back(mStackUndo);
	nstd::reverse(theGroup.theActions);



	// Peform the undo
	//
	// To undo we take the top group of the undo stack, open a group, and
	// replay those actions in reverse order.
	//
	// Those actions will capture their undo's to the new group, and when
	// we close the group the new group will be added to the redo stack.
	//
	// Although the group actions may also set an undo name, the name of
	// the group overrides them (so that the user sees a consistent name
	// as a group is moved between the undo/redo menu items).
	size_t oldSize = mStackRedo.size();

	BeginGroup();

	for (const auto& theAction : theGroup.theActions)
	{
		theAction();
	}

	SetGroupName(theGroup.theName);
	EndGroup();

	NN_REQUIRE(mStackRedo.size() == (oldSize + 1));



	// Update our state
	mIsUndoing = false;
	UpdatedStacks();
}





//=============================================================================
//		NUndoManager::PerformRedo : Perform a redo.
//-----------------------------------------------------------------------------
void NUndoManager::PerformRedo()
{


	// Validate our state
	NN_REQUIRE(!mIsUndoing && !mIsRedoing);
	NN_REQUIRE(!mStackRedo.empty());



	// Prepare to redo
	mIsRedoing = true;

	NUndoGroup theGroup = nstd::extract_back(mStackRedo);
	nstd::reverse(theGroup.theActions);



	// Peform the redo
	//
	// To redo we take the top group of the redo stack, open a group, and
	// replay those actions in reverse order.
	//
	// Those actions will capture their undo's to the new group, and when
	// we close the group the new group will be added to the undo stack.
	//
	// Although the group actions may also set an undo name, the name of
	// the group overrides them (so that the user sees a consistent name
	// as a group is moved between the undo/redo menu items).
	size_t oldSize = mStackUndo.size();

	BeginGroup();

	for (const auto& theAction : theGroup.theActions)
	{
		theAction();
	}

	SetGroupName(theGroup.theName);
	EndGroup();

	NN_REQUIRE(mStackUndo.size() == (oldSize + 1));



	// Update our state
	mIsRedoing = false;
	UpdatedStacks();
}





//=============================================================================
//		NUndoManager::IsUndoing : Are we undoing?
//-----------------------------------------------------------------------------
bool NUndoManager::IsUndoing() const
{


	// Are we undoing?
	return mIsUndoing;
}





//=============================================================================
//		NUndoManager::IsRedoing : Are we redoing?
//-----------------------------------------------------------------------------
bool NUndoManager::IsRedoing() const
{


	// Are we redoing?
	return mIsRedoing;
}





//=============================================================================
//		NUndoManager::IsRecording : Are we recording?
//-----------------------------------------------------------------------------
bool NUndoManager::IsRecording() const
{


	// Are we recording?
	return mIsRecording;
}





//=============================================================================
//		NUndoManager::SetRecording : Set the recording status.
//-----------------------------------------------------------------------------
void NUndoManager::SetRecording(bool canRecord)
{


	// Set the recording status
	mIsRecording = canRecord;
}





//=============================================================================
//		NUndoManager::GetUndoLimit : Get the undo limit.
//-----------------------------------------------------------------------------
size_t NUndoManager::GetUndoLimit() const
{


	// Get the undo limit
	return mUndoLimit;
}





//=============================================================================
//		NUndoManager::SetUndoLimit : Set the undo limit.
//-----------------------------------------------------------------------------
void NUndoManager::SetUndoLimit(size_t theLimit)
{


	// Set the undo limit
	mUndoLimit = theLimit;
}





//=============================================================================
//		NUndoManager::GetUndoName : Get the current undo name.
//-----------------------------------------------------------------------------
NString NUndoManager::GetUndoName() const
{


	// Get the name
	NString theName;

	if (!mStackUndo.empty())
	{
		theName = mStackUndo.back().theName;
	}

	return theName;
}





//=============================================================================
//		NUndoManager::GetRedoName : Get the current redo name.
//-----------------------------------------------------------------------------
NString NUndoManager::GetRedoName() const
{


	// Get the name
	NString theName;

	if (!mStackRedo.empty())
	{
		theName = mStackRedo.back().theName;
	}

	return theName;
}





//=============================================================================
//		NUndoManager::BeginGroup : Begin an undo group.
//-----------------------------------------------------------------------------
void NUndoManager::BeginGroup()
{


	// Validate our state
	NN_REQUIRE(!mGroupOpen);



	// Open the group
	mGroupOpen = true;
	mCurrentGroup.theActions.clear();
}





//=============================================================================
//		NUndoManager::EndGroup : End an undo group.
//-----------------------------------------------------------------------------
void NUndoManager::EndGroup(bool flushGroup)
{


	// Validate our state
	NN_REQUIRE(mGroupOpen);
	NN_REQUIRE(!mCurrentGroup.theActions.empty());



	// Close the group
	//
	// A group is the basic primitive of the undo stack; each group contains
	// one or more actions, and each group corresponds to one user-visible
	// "undo" state change.
	//
	// Undo groups are typically recorded to the undo stack, building up a
	// history of groups that can revert to previous states.
	//
	// When a group is undone, a second "undo" group will be recorded (to
	// undo the undo). This group is pushed to the redo stack, allowing us
	// to rewind to previous states by applying the groups on that stack.
	//
	// This gives us three situations - we're undoing, we're redoing, or
	// we're just recording an action to undo.
	if (mIsUndoing)
	{
		mStackRedo.push_back(mCurrentGroup);
	}

	else if (mIsRedoing)
	{
		mStackUndo.push_back(mCurrentGroup);
	}

	else
	{
		// Flushing a group only captures the first action
		if (flushGroup)
		{
			mCurrentGroup.theActions.resize(1);
		}

		mStackUndo.push_back(mCurrentGroup);
		mStackRedo.clear();
	}



	// Update the stacks
	//
	// Each list is a FILO, so we pop from the front.
	if (mUndoLimit != 0 && mUndoLimit == mStackUndo.size())
	{
		nstd::pop_front(mStackUndo);
	}

	if (mUndoLimit != 0 && mUndoLimit == mStackRedo.size())
	{
		nstd::pop_front(mStackRedo);
	}

	UpdatedStacks();



	// Reset our state
	mGroupOpen = false;
	mCurrentGroup.theName.Clear();
	mCurrentGroup.theActions.clear();
}





//=============================================================================
//		NUndoManager::SetGroupName : Set the group name.
//-----------------------------------------------------------------------------
void NUndoManager::SetGroupName(const NString& theName)
{


	// Set the group name
	mCurrentGroup.theName = theName;
}





//=============================================================================
//		NUndoManager::RecordAction : Record an undo action.
//-----------------------------------------------------------------------------
void NUndoManager::RecordAction(const NFunction& theAction)
{


	// Validate our parameters
	NN_REQUIRE(theAction != NULL);



	// Record the action
	//
	// If no group is currently open, we open a new one around the action.
	if (mIsRecording)
	{
		bool needGroup = !mGroupOpen;
		if (needGroup)
		{
			BeginGroup();
		}

		mCurrentGroup.theActions.push_back(theAction);

		if (needGroup)
		{
			EndGroup();
		}
	}
}





#pragma mark private
//=============================================================================
//		NUndoManager::UpdatedStacks : The undo stacks have been modified.
//-----------------------------------------------------------------------------
void NUndoManager::UpdatedStacks()
{


	// Let everyone know
	Broadcast(kNUndoManagerUpdated);
}
