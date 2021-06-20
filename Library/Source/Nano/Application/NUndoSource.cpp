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
#include "NUndoSource.h"





//=============================================================================
//		NUndoSource::ClearUndo : Clear the undo stack.
//-----------------------------------------------------------------------------
void NUndoSource::ClearUndo()
{


	// Clear the undo stack
	if (mUndoManager != nullptr)
	{
		mUndoManager->Clear();
	}
}





//=============================================================================
//		NUndoSource::RecordUndo : Record an undo action.
//-----------------------------------------------------------------------------
void NUndoSource::RecordUndo(const NFunctionUndoAction& theAction,
							 const NString&             theName,
							 bool                       stringsAreKeys)
{


	// Get the state we need
	NString theValue = theName;

	// dair, TODO
	NN_UNUSED(stringsAreKeys);
	/*
	 if (stringsAreKeys && !theName.IsEmpty())
	 {
	  theValue = NBundleString(theName, theName);
	 }
   */


	// Record the action
	if (mUndoManager != nullptr)
	{
		if (!theValue.IsEmpty())
		{
			mUndoManager->SetGroupName(theValue);
		}

		mUndoManager->RecordAction(theAction);
	}
}





//=============================================================================
//		NUndoSource::TrackUndo : Track an undo group.
//-----------------------------------------------------------------------------
void NUndoSource::TrackUndo(bool beginTrack, bool canFlush)
{


	// Track the undo
	if (mUndoManager != nullptr)
	{
		if (beginTrack)
		{
			mUndoManager->BeginGroup();
		}
		else
		{
			mUndoManager->EndGroup(canFlush);
		}
	}
}





#pragma mark private
//=============================================================================
//		NUndoSource::SetUndoManager : Set the undo manager.
//-----------------------------------------------------------------------------
void NUndoSource::SetUndoManager(const std::shared_ptr<NUndoManager>& undoManager)
{


	// Set the undo manager
	mUndoManager = undoManager;
}
