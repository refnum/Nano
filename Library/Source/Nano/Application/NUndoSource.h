/*	NAME:
		NUndoSource.h

	DESCRIPTION:
		Undo source object.

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
#ifndef NUNDO_SOURCE_H
#define NUNDO_SOURCE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFunction.h"
#include "NUndoManager.h"

// System
#include <memory>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Undo Manager
using NSharedUndoManager                                    = std::shared_ptr<NUndoManager>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NUndoSource
{
public:
	// Clear the undo stack
	void                                ClearUndo();


	// Record an undo action
	void                                RecordUndo(const NFunction& theAction,
												   const NString&   theName        = "",
												   bool             stringsAreKeys = true);


	// Track an undo group
	//
	// Undo-able actions that are driven by mouse-tracking (e.g., dragging an object,
	// or using a color picker) should typically be grouped into a single operation
	// so that the user can undo/redo the entire sequence atomically.
	//
	// Since these operations can typically be represented by the first action in the
	// group, the group may also be flushed to minimize the number of actions that are
	// saved to the undo stacks.
	void                                TrackUndo(bool beginTrack, bool canFlush = true);


	// Set the undo manager
	//
	// Undo actions recorded without an undo manager are discarded.
	void                                SetUndoManager(const NSharedUndoManager& undoManager);


private:
	NSharedUndoManager                  mUndoManager;
};



#endif // NUNDO_SOURCE_H
