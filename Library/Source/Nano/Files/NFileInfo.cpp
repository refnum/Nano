/*	NAME:
		NFileInfo.cpp

	DESCRIPTION:
		File information.

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
#include "NFileInfo.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
inline constexpr NFileStateFlags kNFileStateMaskError =
	NFileState::Exists | NFileState::IsFile | NFileState::IsDirectory | NFileState::CanRead |
	NFileState::CanWrite | NFileState::CanExecute;





//=============================================================================
//		NFileInfo::NFileInfo : Constructor.
//-----------------------------------------------------------------------------
NFileInfo::NFileInfo(const NFilePath& thePath)
	: mPath(thePath)
	, mValid{}
	, mState{}
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());
}





//=============================================================================
//		NFileInfo::NFileInfo : Constructor.
//-----------------------------------------------------------------------------
NFileInfo::NFileInfo()
	: mPath{}
	, mValid{}
	, mState{}
{
}





//=============================================================================
//		NFileInfo::IsValid : Is the object valid?
//-----------------------------------------------------------------------------
bool NFileInfo::IsValid() const
{


	// Check our state
	return mPath.IsValid();
}





//=============================================================================
//		NFileInfo::Clear : Clear the object.
//-----------------------------------------------------------------------------
void NFileInfo::Clear()
{


	// Clear the object
	mPath.Clear();
	Refresh();
}





//=============================================================================
//		NFileInfo::GetPath : Get the path.
//-----------------------------------------------------------------------------
NFilePath NFileInfo::GetPath() const
{


	// Get the path
	return mPath;
}





//=============================================================================
//		NFileInfo::SetPath : Set the path.
//-----------------------------------------------------------------------------
void NFileInfo::SetPath(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Set the path
	mPath = thePath;

	Refresh();
}





//=============================================================================
//		NFileInfo::Refresh : Refresh the info.
//-----------------------------------------------------------------------------
void NFileInfo::Refresh()
{


	// Update our state
	mValid = {};
	mState = {};
}





//=============================================================================
//		NFileInfo::Exists : Does the path exist?
//-----------------------------------------------------------------------------
bool NFileInfo::Exists() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	UpdateState(NFileState::Exists);

	return TestFlag(NFileState::Exists);
}





//=============================================================================
//		NFileInfo::IsFile : Is the path a file?
//-----------------------------------------------------------------------------
bool NFileInfo::IsFile() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	UpdateState(NFileState::IsFile);

	return TestFlag(NFileState::IsFile);
}





//=============================================================================
//		NFileInfo::IsDirectory : Is the path a directory?
//-----------------------------------------------------------------------------
bool NFileInfo::IsDirectory() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	UpdateState(NFileState::IsDirectory);

	return TestFlag(NFileState::IsDirectory);
}





//=============================================================================
//		NFileInfo::CanRead : Can the path be read?
//-----------------------------------------------------------------------------
bool NFileInfo::CanRead() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	UpdateState(NFileState::CanRead);

	return TestFlag(NFileState::CanRead);
}





//=============================================================================
//		NFileInfo::CanWrite : Can the path be written?
//-----------------------------------------------------------------------------
bool NFileInfo::CanWrite() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	UpdateState(NFileState::CanWrite);

	return TestFlag(NFileState::CanWrite);
}





//=============================================================================
//		NFileInfo::CanExecute : Can the path be executed / searched?
//-----------------------------------------------------------------------------
bool NFileInfo::CanExecute() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	UpdateState(NFileState::CanExecute);

	return TestFlag(NFileState::CanExecute);
}





//=============================================================================
//		NFileInfo::GetCreationTime : Get the creation time.
//-----------------------------------------------------------------------------
NTime NFileInfo::GetCreationTime() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	UpdateState(NFileState::CreationTime);

	return mState.creationTime;
}





//=============================================================================
//		NFileInfo::GetModifiedTime : Get the last-modified time.
//-----------------------------------------------------------------------------
NTime NFileInfo::GetModifiedTime() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	UpdateState(NFileState::ModifiedTime);

	return mState.modifiedTime;
}





//=============================================================================
//		NFileInfo::GetFileSize : Get the file size.
//-----------------------------------------------------------------------------
uint64_t NFileInfo::GetFileSize() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the state
	UpdateState(NFileState::FileSize);

	NN_REQUIRE(IsFile());
	return mState.fileSize;
}





//=============================================================================
//		NFileInfo::Exists : Does the path exist?
//-----------------------------------------------------------------------------
bool NFileInfo::Exists(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state
	return NFileInfo(thePath).Exists();
}





//=============================================================================
//		NFileInfo::IsFile : Is the path a file?
//-----------------------------------------------------------------------------
bool NFileInfo::IsFile(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state
	return NFileInfo(thePath).IsFile();
}





//=============================================================================
//		NFileInfo::IsDirectory : Is the path a directory?
//-----------------------------------------------------------------------------
bool NFileInfo::IsDirectory(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state
	return NFileInfo(thePath).IsDirectory();
}





//=============================================================================
//		NFileInfo::CanRead : Can the path be read?
//-----------------------------------------------------------------------------
bool NFileInfo::CanRead(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state
	return NFileInfo(thePath).CanRead();
}





//=============================================================================
//		NFileInfo::CanWrite : Can the path be written?
//-----------------------------------------------------------------------------
bool NFileInfo::CanWrite(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state
	return NFileInfo(thePath).CanWrite();
}





//=============================================================================
//		NFileInfo::CanExecute : Can the path be executed / searched?
//-----------------------------------------------------------------------------
bool NFileInfo::CanExecute(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state
	return NFileInfo(thePath).CanExecute();
}





//=============================================================================
//		NFileInfo::GetCreationTime : Get the creation time.
//-----------------------------------------------------------------------------
NTime NFileInfo::GetCreationTime(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state
	return NFileInfo(thePath).GetCreationTime();
}





//=============================================================================
//		NFileInfo::GetModifiedTime : Get the last-modified time.
//-----------------------------------------------------------------------------
NTime NFileInfo::GetModifiedTime(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state
	return NFileInfo(thePath).GetModifiedTime();
}





//=============================================================================
//		NFileInfo::GetFileSize : Get the file size.
//-----------------------------------------------------------------------------
uint64_t NFileInfo::GetFileSize(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state
	return NFileInfo(thePath).GetFileSize();
}





#pragma mark private
//=============================================================================
//		NFileInfo::TestFlag : Test a flag.
//-----------------------------------------------------------------------------
bool NFileInfo::TestFlag(NFileStateFlags theFlag) const
{


	// Validate our state
	NN_REQUIRE(mValid & theFlag);



	// Test the flag
	return mState.theFlags & theFlag;
}





//=============================================================================
//		NFileInfo::UpdateState : Update the file state.
//-----------------------------------------------------------------------------
void NFileInfo::UpdateState(NFileStateFlags theFlags) const
{


	// Update the state
	//
	// We can cast away const as our state is immutable once fetched.
	if ((mValid & theFlags) != theFlags)
	{
		NFileInfo* thisPtr = const_cast<NFileInfo*>(this);

		if (!thisPtr->FetchState(theFlags))
		{
			// Handle failure
			//
			// If we fail to acquire any information we assume we can't trust
			// any of the state, to avoid the platform-specific code from having
			// to return partial results.
			//
			// However, if an error does occur, the one thing we can say is that
			// the is-this / can-this states can be reported as false.
			thisPtr->mValid = kNFileStateMaskError;
			thisPtr->mState = {};
		}
	}
}
