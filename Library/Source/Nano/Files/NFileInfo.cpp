/*	NAME:
		NFileInfo.cpp

	DESCRIPTION:
		File information.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
static constexpr NFileInfoFlags kNFileInfoMaskError         = kNFileInfoExists | kNFileInfoIsFile |
													  kNFileInfoIsDirectory | kNFileInfoCanRead |
													  kNFileInfoCanWrite | kNFileInfoCanExecute;





//=============================================================================
//		NFileInfo::NFileInfo : Constructor.
//-----------------------------------------------------------------------------
NFileInfo::NFileInfo()
	: mPath{}
	, mValid(kNFileInfoNone)
	, mState{}
{
}





//=============================================================================
//		NFileInfo::NFileInfo : Constructor.
//-----------------------------------------------------------------------------
NFileInfo::NFileInfo(const NString& thePath)
	: mPath(thePath)
	, mValid(kNFileInfoNone)
	, mState{}
{


	// Validate our parameters
	NN_REQUIRE(!thePath.IsEmpty());
}





//=============================================================================
//		NFileInfo::IsValid : Is the object valid?
//-----------------------------------------------------------------------------
bool NFileInfo::IsValid() const
{


	// Check our state
	return !mPath.IsEmpty();
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
NString NFileInfo::GetPath() const
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Get the path
	return mPath;
}





//=============================================================================
//		NFileInfo::SetPath : Set the path.
//-----------------------------------------------------------------------------
void NFileInfo::SetPath(const NString& thePath)
{


	// Validate our parameters
	NN_REQUIRE(!thePath.IsEmpty());


	// Set the path
	mPath = thePath;

	Refresh();
}





//=============================================================================
//		NFileInfo::Refresh : Refresh the info.
//-----------------------------------------------------------------------------
void NFileInfo::Refresh()
{


	// Validate our state
	NN_REQUIRE(IsValid());

	// Update our state
	mValid = kNFileInfoNone;
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
	UpdateState(kNFileInfoExists);

	return TestFlag(kNFileInfoExists);
}





//=============================================================================
//		NFileInfo::IsFile : Is the path a file?
//-----------------------------------------------------------------------------
bool NFileInfo::IsFile() const
{


	// Validate our state
	NN_REQUIRE(IsValid());

	// Get the state
	UpdateState(kNFileInfoIsFile);

	return TestFlag(kNFileInfoIsFile);
}





//=============================================================================
//		NFileInfo::IsDirectory : Is the path a directory?
//-----------------------------------------------------------------------------
bool NFileInfo::IsDirectory() const
{


	// Validate our state
	NN_REQUIRE(IsValid());

	// Get the state
	UpdateState(kNFileInfoIsDirectory);

	return TestFlag(kNFileInfoIsDirectory);
}





//=============================================================================
//		NFileInfo::CanRead : Can the path be read?
//-----------------------------------------------------------------------------
bool NFileInfo::CanRead() const
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Get the state
	UpdateState(kNFileInfoCanRead);

	return TestFlag(kNFileInfoCanRead);
}





//=============================================================================
//		NFileInfo::CanWrite : Can the path be written?
//-----------------------------------------------------------------------------
bool NFileInfo::CanWrite() const
{


	// Validate our state
	NN_REQUIRE(IsValid());

	// Get the state
	UpdateState(kNFileInfoCanWrite);

	return TestFlag(kNFileInfoCanWrite);
}





//=============================================================================
//		NFileInfo::CanExecute : Can the path be executed / searched?
//-----------------------------------------------------------------------------
bool NFileInfo::CanExecute() const
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Get the state
	UpdateState(kNFileInfoCanExecute);

	return TestFlag(kNFileInfoCanExecute);
}





//=============================================================================
//		NFileInfo::GetCreationTime : Get the creation time.
//-----------------------------------------------------------------------------
NTime NFileInfo::GetCreationTime() const
{


	// Validate our state
	NN_REQUIRE(IsValid());

	// Get the state
	UpdateState(kNFileInfoCreationTime);

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
	UpdateState(kNFileInfoModifiedTime);

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
	UpdateState(kNFileInfoFileSize);

	NN_REQUIRE(IsFile());
	return mState.fileSize;
}





#pragma mark private
//=============================================================================
//		NFileInfo::TestFlag : Test a flag.
//-----------------------------------------------------------------------------
bool NFileInfo::TestFlag(NFileInfoFlags theFlag) const
{


	// Validate our state
	NN_REQUIRE((mValid & theFlag) != 0);



	// Test the flag
	return (mState.theFlags & theFlag) != 0;
}





//=============================================================================
//		NFileInfo::UpdateState : Update the file state.
//-----------------------------------------------------------------------------
void NFileInfo::UpdateState(NFileInfoFlags theFlags) const
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
			thisPtr->mValid = kNFileInfoMaskError;
			thisPtr->mState = {};
		}
	}
}
