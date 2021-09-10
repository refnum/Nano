/*	NAME:
		NFileHandle.cpp

	DESCRIPTION:
		File handle.

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
#include "NFileHandle.h"

// Nano
#include "NData.h"
#include "NFile.h"
#include "NFileInfo.h"
#include "NFileUtils.h"
#include "NStringEncoder.h"





//=============================================================================
//		NFileHandle::NFileHandle : Constructor.
//-----------------------------------------------------------------------------
NFileHandle::NFileHandle()
	: mPathDest{}
	, mPathOpen{}
	, mHandle(nullptr)
	, mAccess(NFileAccess::ReadOnly)
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
	return mHandle != nullptr;
}





//=============================================================================
//		NFileHandle::GetPath : Get the path.
//-----------------------------------------------------------------------------
NFilePath NFileHandle::GetPath() const
{


	// Get the path
	return mPathDest;
}





//=============================================================================
//		NFileHandle::Open : Open the file handle.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Open(const NFile& theFile, NFileAccess theAccess, NFileUsageFlags theFlags)
{


	// Open the file
	return Open(theFile.GetPath(), theAccess, theFlags);
}





//=============================================================================
//		NFileHandle::Open : Open the file handle.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Open(const NFilePath& thePath, NFileAccess theAccess, NFileUsageFlags theFlags)
{


	// Validate our parameters and state
	NN_REQUIRE(!IsOpen());
	NN_REQUIRE(thePath.IsValid());

	NN_REQUIRE(!mPathDest.IsValid());
	NN_REQUIRE(!mPathOpen.IsValid());



	// Get the state we need
	NFilePath       pathOpen  = GetOpenPath(theAccess, thePath);
	NFileUsageFlags flagsOpen = GetOpenFlags(theAccess, theFlags);



	// Open the file
	NStatus theErr = FileOpen(pathOpen, theAccess, flagsOpen);
	if (theErr == NStatus::OK)
	{
		// Update our state
		NN_REQUIRE(mHandle != nullptr);

		mPathDest = thePath;
		mPathOpen = pathOpen;
		mAccess   = theAccess;



		// Initialise the position
		theErr = SetPosition(0, NFileOffset::FromStart);
		NN_EXPECT_NOT_ERR(theErr);

		if (theErr != NStatus::OK)
		{
			Close();
		}
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::OpenTemporary : Open a temporary file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::OpenTemporary(const NString& baseName)
{


	// Open the file
	NFileInfo tmpDir(NFileUtils::GetLocation(NFileLocation::AppTemporaries));
	NFilePath thePath(NFileUtils::GetUniqueChild(tmpDir.GetPath(), baseName));

	NStatus theErr = NStatus::Permission;

	if (tmpDir.IsDirectory())
	{
		theErr = Open(thePath, NFileAccess::ReadWrite);
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::Close : Close the file handle.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Close()
{


	// Validate our state
	NN_REQUIRE(IsOpen());
	NN_REQUIRE(mPathDest.IsValid());
	NN_REQUIRE(mPathOpen.IsValid());



	// Close the handle
	NStatus theErr = CloseHandle();



	// Reset our state
	mPathDest.Clear();
	mPathOpen.Clear();

	mAccess = NFileAccess::ReadOnly;
	mHandle = nullptr;

	return theErr;
}





//=============================================================================
//		NFileHandle::GetPosition : Get the position.
//-----------------------------------------------------------------------------
uint64_t NFileHandle::GetPosition() const
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Get the position
	return FileGetPosition();
}





//=============================================================================
//		NFileHandle::SetPosition : Set the position.
//-----------------------------------------------------------------------------
NStatus NFileHandle::SetPosition(int64_t theOffset, NFileOffset relativeTo)
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Set the position
	return FileSetPosition(theOffset, relativeTo);
}





//=============================================================================
//		NFileHandle::GetSize : Get the size.
//-----------------------------------------------------------------------------
uint64_t NFileHandle::GetSize() const
{


	// Get the size
	if (IsOpen())
	{
		return NFileInfo(mPathOpen).GetFileSize();
	}
	else
	{
		return NFileInfo(mPathDest).GetFileSize();
	}
}





//=============================================================================
//		NFileHandle::SetSize : Set the size.
//-----------------------------------------------------------------------------
NStatus NFileHandle::SetSize(uint64_t theSize)
{


	// Validate our state
	NN_REQUIRE(IsOpen());
	NN_REQUIRE(CanWrite());



	// Set the size
	return FileSetSize(theSize);
}





//=============================================================================
//		NFileHandle::Read : Read from the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Read(uint64_t    theSize,
						  void*       thePtr,
						  uint64_t&   sizeRead,
						  int64_t     theOffset,
						  NFileOffset relativeTo)
{


	// Validate our state
	NN_REQUIRE(IsOpen());
	NN_REQUIRE(CanRead());


	// Get the state we need
	NStatus theErr = NStatus::OK;
	sizeRead       = 0;



	// Adjust the offset
	if (theOffset != 0 && relativeTo != NFileOffset::FromPosition)
	{
		theErr = SetPosition(theOffset, relativeTo);
		NN_EXPECT_NOT_ERR(theErr);
	}



	// Read from the file
	if (theErr == NStatus::OK)
	{
		theErr = FileRead(theSize, thePtr, sizeRead);
	}

	NN_REQUIRE(theErr != NStatus::OK || (sizeRead == theSize));

	return theErr;
}





//=============================================================================
//		NFileHandle::Write : Write to the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Write(uint64_t    theSize,
						   const void* thePtr,
						   uint64_t&   sizeWritten,
						   int64_t     theOffset,
						   NFileOffset relativeTo)
{


	// Validate our state
	NN_REQUIRE(IsOpen());
	NN_REQUIRE(CanWrite());



	// Get the state we need
	NStatus theErr = NStatus::OK;
	sizeWritten    = 0;



	// Adjust the offset
	if (theOffset != 0 && relativeTo != NFileOffset::FromPosition)
	{
		theErr = SetPosition(theOffset, relativeTo);
		NN_EXPECT_NOT_ERR(theErr);
	}



	// Write to the file
	if (theErr == NStatus::OK)
	{
		theErr = FileWrite(theSize, thePtr, sizeWritten);
	}

	NN_REQUIRE(theErr != NStatus::OK || (sizeWritten == theSize));

	return theErr;
}





//=============================================================================
//		NFileHandle::Flush : Flush the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::Flush()
{


	// Validate our state
	NN_REQUIRE(IsOpen());
	NN_REQUIRE(CanWrite());


	// Flush the file
	return FileFlush();
}





//=============================================================================
//		NFileHandle::ReadText : Read text from a file.
//-----------------------------------------------------------------------------
NString NFileHandle::ReadText(const NFile& theFile, NStringEncoding theEncoding)
{


	// Validate our parameters
	NN_REQUIRE(theFile.IsValid());



	// Read the text
	NData   theData = ReadData(theFile);
	NString theText;

	if (!theData.IsEmpty())
	{
		// Determine the encoding
		if (theEncoding == NStringEncoding::Unknown)
		{
			theEncoding = NStringEncoder::GetUnknownEncoding(theData);
		}


		// Set the text
		if (theEncoding != NStringEncoding::Unknown)
		{
			theText.SetData(theEncoding, theData);
		}
	}

	return theText;
}





//=============================================================================
//		NFileHandle::WriteText : Write text to a file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::WriteText(const NFile&    theFile,
							   const NString&  theText,
							   NStringEncoding theEncoding)
{


	// Validate our parameters
	NN_REQUIRE(theFile.IsValid());
	NN_EXPECT(!theText.IsEmpty());



	// Write the text
	return WriteData(theFile, theText.GetData(theEncoding));
}





//=============================================================================
//		NFileHandle::ReadData : Read data from a file.
//-----------------------------------------------------------------------------
NData NFileHandle::ReadData(const NFile& theFile)
{


	// Validate our parameters
	NN_REQUIRE(theFile.IsValid());



	// Get the state we need
	NFileHandle fileHnd;
	NData       theData;



	// Read the file data
	NStatus theErr = fileHnd.Open(theFile, NFileAccess::ReadOnly);
	NN_EXPECT(theErr == NStatus::OK || theErr == NStatus::NotFound);

	if (theErr == NStatus::OK)
	{
		uint64_t theSize  = uint64_t(theFile.GetSize());
		uint64_t sizeRead = 0;

		(void) theData.Append(theSize, nullptr, NDataSource::None);

		theErr = fileHnd.Read(theSize, theData.GetMutableData(), sizeRead);
		NN_EXPECT_NOT_ERR(theErr);
	}



	// Clean up
	if (theErr != NStatus::OK)
	{
		theData.Clear();
	}

	return theData;
}





//=============================================================================
//		NFileHandle::WriteData : Write data to a file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::WriteData(const NFile& theFile, const NData& theData)
{


	// Validate our parameters
	NN_REQUIRE(theFile.IsValid());
	NN_EXPECT(!theData.IsEmpty());


	// Get the state we need
	NFileHandle fileHnd;



	// Write the file data
	NStatus theErr = fileHnd.Open(theFile, NFileAccess::WriteAtomic);
	NN_EXPECT_NOT_ERR(theErr);

	if (theErr == NStatus::OK)
	{
		uint64_t theSize     = uint64_t(theData.GetSize());
		uint64_t sizeWritten = 0;

		theErr = fileHnd.Write(theSize, theData.GetData(), sizeWritten);
		NN_EXPECT_NOT_ERR(theErr);

		if (theErr == NStatus::OK)
		{
			theErr = fileHnd.SetSize(theSize);
			NN_EXPECT_NOT_ERR(theErr);
		}

		fileHnd.Close();
	}



	// Clean up
	if (theErr != NStatus::OK)
	{
		theFile.Delete();
	}

	return theErr;
}





#pragma mark private
//=============================================================================
//		NFileHandle::CanRead : Can the file be read from?
//-----------------------------------------------------------------------------
bool NFileHandle::CanRead() const
{


	// Check our state
	return mAccess == NFileAccess::ReadOnly || mAccess == NFileAccess::ReadWrite;
}





//=============================================================================
//		NFileHandle::CanWrite : Can the file be written to?
//-----------------------------------------------------------------------------
bool NFileHandle::CanWrite() const
{


	// Check our state
	return mAccess == NFileAccess::WriteOnly || mAccess == NFileAccess::ReadWrite ||
		   mAccess == NFileAccess::WriteAtomic;
}





//=============================================================================
//		NFileHandle::GetOpenFlags : Get the open flags.
//-----------------------------------------------------------------------------
NFileUsageFlags NFileHandle::GetOpenFlags(NFileAccess theAccess, NFileUsageFlags theFlags)
{


	// Get the flags
	if (!theFlags)
	{
		switch (theAccess)
		{
			case NFileAccess::ReadOnly:
				theFlags = NFileUsage::ReadEarly | NFileUsage::ReadAhead;
				break;

			case NFileAccess::WriteOnly:
				theFlags = NFileUsage::NoCache;
				break;

			case NFileAccess::ReadWrite:
				theFlags = NFileUsage::ReadEarly | NFileUsage::ReadAhead;
				break;

			case NFileAccess::WriteAtomic:
				theFlags = NFileUsage::NoCache;
				break;
		}
	}

	return theFlags;
}





//=============================================================================
//		NFileHandle::GetOpenPath : Get the path to open.
//-----------------------------------------------------------------------------
NFilePath NFileHandle::GetOpenPath(NFileAccess theAccess, const NFilePath& thePath)
{


	// Get the flags
	if (theAccess == NFileAccess::WriteAtomic)
	{
		NString baseName = "." + thePath.GetFilename() + ".tmp";
		return NFileUtils::GetUniqueChild(thePath.GetParent(), baseName);
	}
	else
	{
		return thePath;
	}
}





//=============================================================================
//		NFileHandle::CloseHandle : Close the handle.
//-----------------------------------------------------------------------------
NStatus NFileHandle::CloseHandle()
{


	// Validate our state
	NN_REQUIRE(IsOpen());



	// Close the file
	NStatus theErr = FileClose();

	if (mAccess == NFileAccess::WriteAtomic)
	{
		// An atomic write involves two paths:
		//
		//		mPathDest is the path that was passed to Open.
		//
		//		mPathOpen is the path that we opened for writing.
		//
		// If the destination path does not exist then we can just rename
		// the output file to the expected name.
		//
		// Otherwise, if the destination does exist (or if the rename fails),
		// we exchange the two paths and delete the old file.
		theErr = NStatus::Permission;

		if (!NFileInfo(mPathDest).Exists())
		{
			theErr = NFileUtils::Rename(mPathOpen, mPathDest);
			NN_EXPECT_NOT_ERR(theErr);
		}

		if (theErr != NStatus::OK)
		{
			theErr = NFileUtils::Exchange(mPathOpen, mPathDest);
			NN_EXPECT_NOT_ERR(theErr);

			NStatus deleteErr = NFileUtils::Delete(mPathOpen);
			NN_EXPECT_NOT_ERR(deleteErr);
		}
	}

	return theErr;
}
