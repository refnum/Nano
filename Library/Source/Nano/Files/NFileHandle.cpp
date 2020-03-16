/*	NAME:
		NFileHandle.cpp

	DESCRIPTION:
		File handle.

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
#include "NFileHandle.h"

// Nano
#include "NFile.h"
#include "NFileInfo.h"





//=============================================================================
//		NFileHandle::NFileHandle : Constructor.
//-----------------------------------------------------------------------------
NFileHandle::NFileHandle()
	: mPath{}
	, mHandle(kNFileHandleNone)
{
}





//=============================================================================
//		NFileHandle::~NFileHandle : Destructor.
//-----------------------------------------------------------------------------
NFileHandle::~NFileHandle()
{


	// Clean up
	if (IsOpen())
	{
		Close();
	}
}





//=============================================================================
//		NFileHandle::IsOpen : Is the handle open?
//-----------------------------------------------------------------------------
bool NFileHandle::IsOpen() const
{


	// Check the state
	return mHandle != kNFileHandleNone;
}





//=============================================================================
//		NFileHandle::GetPath : Get the path.
//-----------------------------------------------------------------------------
NString NFileHandle::GetPath() const
{


	// Get the path
	return mPath;
}





//=============================================================================
//		NFileHandle::Open : Open the file handle.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Open(const NFile& theFile, NFilePermission thePermission)
{


	// Open the file
	return Open(theFile.GetPath(), thePermission);
}





//=============================================================================
//		NFileHandle::Open : Open the file handle.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Open(const NString& thePath, NFilePermission thePermission)
{


	// Validate our parameters and state
	NN_REQUIRE(!IsOpen());
	NN_REQUIRE(!thePath.IsEmpty());

	NN_REQUIRE(mPath.IsEmpty());
	NN_REQUIRE(mPosition == 0);



	// Open the file
	NStatus theErr = FileOpen(thePermission);
	NN_EXPECT_NOT_ERR(theErr);

	if (theErr == NStatus::NoErr)
	{
		NN_REQUIRE(mHandle != kNFileHandleNone);
		mPath = thePath;
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::Close : Close the file handle.
//-----------------------------------------------------------------------------
void NFileHandle::Close()
{


	// Validate our state
	NN_REQUIRE(IsOpen());
	NN_REQUIRE(!mPath.IsEmpty());



	// Close the handle
	FileClose();

	mPath.Clear();
	mHandle   = kNFileHandleNone;
	mPosition = 0;
}





//=============================================================================
//		NFileHandle::GetPosition : Get the position.
//-----------------------------------------------------------------------------
uint64_t NFileHandle::GetPosition() const
{


	// Get the position
	return FileGetPosition();
}





//=============================================================================
//		NFileHandle::SetPosition : Set the position.
//-----------------------------------------------------------------------------
NStatus NFileHandle::SetPosition(int64_t theOffset, NFileOffset relativeTo)
{


	// Set the position
	return FileSetPosition(theOffset, relativeTo);
}





//=============================================================================
//		NFileHandle::Read : Read from the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Read(uint64_t    theSize,
						  void*       thePtr,
						  uint64_t&   numRead,
						  int64_t     theOffset,
						  NFileOffset relativeTo)
{


	// Adjust the offset
	NStatus theErr = NStatus::NoErr;

	if (theOffset != 0 && relativeTo != NFileOffset::FromPosition)
	{
		theErr = SetPosition(theOffset, relativeTo);
		NN_EXPECT_NOT_ERR(theErr);
	}



	// Read from the file
	if (theErr == NStatus::NoErr)
	{
		theErr = FileRead(theSize, thePtr, numRead);
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::Write : Write to the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Write(uint64_t    theSize,
						   const void* thePtr,
						   uint64_t&   numWritten,
						   int64_t     theOffset,
						   NFileOffset relativeTo)
{


	// Adjust the offset
	NStatus theErr = NStatus::NoErr;

	if (theOffset != 0 && relativeTo != NFileOffset::FromPosition)
	{
		theErr = SetPosition(theOffset, relativeTo);
		NN_EXPECT_NOT_ERR(theErr);
	}



	// Write to the file
	if (theErr == NStatus::NoErr)
	{
		theErr = FileWrite(theSize, thePtr, numWritten);
	}

	return theErr;
}
