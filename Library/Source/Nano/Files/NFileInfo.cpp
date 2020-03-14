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
//		NFileInfo::NFileInfo : Constructor.
//-----------------------------------------------------------------------------
NFileInfo::NFileInfo()
	: mState(kNFileInfoNone)
	, mPath()
	, mSize(0)
	, mFlags(kNFileInfoNone)
{
}





//=============================================================================
//		NFileInfo::GetState : Get the state.
//-----------------------------------------------------------------------------
NFileInfoState NFileInfo::GetState() const
{


	// Get the state
	return mState;
}





//=============================================================================
//		NFileInfo::SetState : Set the state.
//-----------------------------------------------------------------------------
void NFileInfo::SetState(NFileInfoState theState)
{


	// Set the state
	mState = theState;
	RefreshInfo();
}





//=============================================================================
//		NFileInfo::GetPath : Get the path.
//-----------------------------------------------------------------------------
NString NFileInfo::GetPath() const
{


	// Get the path
	return mPath;
}





//=============================================================================
//		NFileInfo::SetPath : Set the path.
//-----------------------------------------------------------------------------
void NFileInfo::SetPath(const NString& thePath)
{


	// Set the path
	mPath = thePath;
	RefreshInfo();
}





//=============================================================================
//		NFileInfo::Exists : Does the path exists?
//-----------------------------------------------------------------------------
bool NFileInfo::Exists() const
{


	// Get the state
	return HasFlag(kNFileInfoExists);
}





//=============================================================================
//		NFileInfo::IsFile : Is the path a file?
//-----------------------------------------------------------------------------
bool NFileInfo::IsFile() const
{


	// Get the state
	return HasFlag(kNFileInfoIsFile);
}





//=============================================================================
//		NFileInfo::IsDirectory : Is the path a directory?
//-----------------------------------------------------------------------------
bool NFileInfo::IsDirectory() const
{


	// Get the state
	return HasFlag(kNFileInfoIsDirectory);
}





//=============================================================================
//		NFileInfo::GetSizse : Get the size.
//-----------------------------------------------------------------------------
size_t NFileInfo::GetSize() const
{


	// Validate our state
	NN_REQUIRE((mState & kNFileInfoSize) != 0);



	// Get the state
	return mSize;
}





#pragma mark private
//=============================================================================
//		NFileInfo::HasFlag : Has a flag been set?
//-----------------------------------------------------------------------------
bool NFileInfo::HasFlag(NFileInfoState theFlag) const
{


	// Validate our state
	NN_REQUIRE((mState & theFlag) != 0);



	// Get the value
	return (mFlags & theFlag) != 0;
}
