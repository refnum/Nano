/*	NAME:
		NFile.cpp

	DESCRIPTION:
		File object.

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
#include "NFile.h"

// Nano
#include "NDebug.h"
#include "NFileHandle.h"
#include "NFileUtils.h"





//=============================================================================
//		NFile::NFile : Constructor.
//-----------------------------------------------------------------------------
NFile::NFile()
	: mInfo()
{
}





//=============================================================================
//		NFile::NFile : Constructor.
//-----------------------------------------------------------------------------
NFile::NFile(const NFilePath& thePath)
	: mInfo(thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());
}





//=============================================================================
//		NFile::IsValid : Is the file valid?
//-----------------------------------------------------------------------------
bool NFile::IsValid() const
{


	// Check our state
	return mInfo.IsValid();
}





//=============================================================================
//		NFile::Clear : Clear the file.
//-----------------------------------------------------------------------------
void NFile::Clear()
{


	// Reset our state
	mInfo.Clear();
}





//=============================================================================
//		NFile::GetPath : Get the path.
//-----------------------------------------------------------------------------
NFilePath NFile::GetPath() const
{


	// Get our state
	return mInfo.GetPath();
}





//=============================================================================
//		NFile::SetPath : Set the path.
//-----------------------------------------------------------------------------
void NFile::SetPath(const NFilePath& thePath)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Set our state
	mInfo.SetPath(thePath);
}





//=============================================================================
//		NFile::Refresh : Refresh the info.
//-----------------------------------------------------------------------------
void NFile::Refresh()
{


	// Validate our state
	NN_REQUIRE(IsValid());

	// Refresh our state
	mInfo.Refresh();
}





//=============================================================================
//		NFile::Exists : Does the file exists?
//-----------------------------------------------------------------------------
bool NFile::Exists() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get our state
	return mInfo.Exists();
}





//=============================================================================
//		NFile::IsFile : Is this a file?
//-----------------------------------------------------------------------------
bool NFile::IsFile() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get our state
	return mInfo.IsFile();
}





//=============================================================================
//		NFile::IsDirectory : Is this a directory?
//-----------------------------------------------------------------------------
bool NFile::IsDirectory() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get our state
	return mInfo.IsDirectory();
}





//=============================================================================
//		NFile::CanRead : Is the file readable?
//-----------------------------------------------------------------------------
bool NFile::CanRead() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get our state
	return mInfo.CanRead();
}





//=============================================================================
//		NFile::CanWrite : Is the file writeable?
//-----------------------------------------------------------------------------
bool NFile::CanWrite() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get our state
	return mInfo.CanWrite();
}





//=============================================================================
//		NFile::CanExecute : Is the file executable?
//-----------------------------------------------------------------------------
bool NFile::CanExecute() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get our state
	return mInfo.CanExecute();
}





//=============================================================================
//		NFile::GetCreationTime : Get the creation time.
//-----------------------------------------------------------------------------
NTime NFile::GetCreationTime() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get our state
	return mInfo.GetCreationTime();
}





//=============================================================================
//		NFile::GetModifiedTime : Get the modification time.
//-----------------------------------------------------------------------------
NTime NFile::GetModifiedTime() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get our state
	return mInfo.GetModifiedTime();
}





//=============================================================================
//		NFile::GetName : Get the file name.
//-----------------------------------------------------------------------------
NString NFile::GetName() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the file name
	return mInfo.GetPath().GetFilename();
}





//=============================================================================
//		NFile::SetName : Set the file name.
//-----------------------------------------------------------------------------
NStatus NFile::SetName(const NString& theName, bool renameFile)
{


	// Validate our parameters and state
	NN_REQUIRE(!theName.IsEmpty());

	NN_REQUIRE(IsValid());



	// Update the path
	NFilePath newPath = mInfo.GetPath();
	newPath.SetFilename(theName);

	return UpdatePath(newPath, renameFile);
}





//=============================================================================
//		NFile::GetExtension : Get the file extension.
//-----------------------------------------------------------------------------
NString NFile::GetExtension() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the file extension
	return mInfo.GetPath().GetExtension();
}





//=============================================================================
//		NFile::SetExtension : Set the file extension.
//-----------------------------------------------------------------------------
NStatus NFile::SetExtension(const NString& theExtension, bool renameFile)
{


	// Validate our parameters and state
	NN_REQUIRE(!theExtension.IsEmpty());
	NN_REQUIRE(!theExtension.StartsWith("."));

	NN_REQUIRE(IsValid());



	// Update the path
	NFilePath newPath = mInfo.GetPath();
	newPath.SetExtension(theExtension);

	return UpdatePath(newPath, renameFile);
}





//=============================================================================
//		NFile::GetChild : Get a child.
//-----------------------------------------------------------------------------
NFile NFile::GetChild(const NString& theName) const
{


	// Validate our parameters and state
	NN_REQUIRE(!theName.IsEmpty());
	NN_REQUIRE(IsValid());



	// Get the child
	return mInfo.GetPath().GetChild(theName);
}





//=============================================================================
//		NFile::GetParent : Get the parent.
//-----------------------------------------------------------------------------
NFile NFile::GetParent() const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the child
	return mInfo.GetPath().GetParent();
}





//=============================================================================
//		NFile::GetSize : Get the size of a file.
//-----------------------------------------------------------------------------
uint64_t NFile::GetSize() const
{


	// Validate our state
	NN_REQUIRE(IsValid());
	NN_REQUIRE(IsFile());



	// Get the size
	return mInfo.GetFileSize();
}





//=============================================================================
//		NFile::GetChildren : Get the children of a directory.
//-----------------------------------------------------------------------------
NVectorFile NFile::GetChildren() const
{


	// Validate our state
	NN_REQUIRE(IsValid());
	NN_REQUIRE(IsDirectory());



	// Get the children
	NVectorFilePath childPaths = NFileUtils::GetChildren(GetPath());
	NVectorFile     theChildren;

	theChildren.reserve(childPaths.size());

	for (const auto& childPath : childPaths)
	{
		theChildren.emplace_back(NFile(childPath));
	}

	return theChildren;
}





//=============================================================================
//		NFile::CreateFile : Create a file.
//-----------------------------------------------------------------------------
NStatus NFile::CreateFile()
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Create the directory
	NStatus theErr = NFileUtils::CreateFile(GetPath());
	NN_EXPECT_NOT_ERR(theErr);



	// Update our state
	mInfo.Refresh();

	return theErr;
}





//=============================================================================
//		NFile::CreateDirectory : Create a directory.
//-----------------------------------------------------------------------------
NStatus NFile::CreateDirectory()
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Create the directory
	NStatus theErr = NFileUtils::CreateDirectory(GetPath());
	NN_EXPECT_NOT_ERR(theErr);



	// Update our state
	mInfo.Refresh();

	return theErr;
}





//=============================================================================
//		NFile::Delete : Delete a file.
//-----------------------------------------------------------------------------
NStatus NFile::Delete(bool moveToTrash) const
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Delete the file
	NStatus theErr = NFileUtils::Delete(GetPath(), moveToTrash);
	NN_EXPECT_NOT_ERR(theErr);



	// Update our state
	mInfo.Refresh();

	return theErr;
}





//=============================================================================
//		NFile::DeleteChildren : Delete the children of a directory.
//-----------------------------------------------------------------------------
NStatus NFile::DeleteChildren(bool moveToTrash) const
{


	// Validate our state
	NN_REQUIRE(IsDirectory());



	// Delete the children
	return NFileUtils::DeleteChildren(GetPath(), moveToTrash);
}





//=============================================================================
//		NFile::MoveTo : Move a file to a new location.
//-----------------------------------------------------------------------------
NStatus NFile::MoveTo(const NFile& newParent)
{


	// Validate our parameters and stdate
	NN_REQUIRE(newParent.IsDirectory());
	NN_REQUIRE(Exists());



	// Move the file
	NFile newFile = newParent.GetChild(GetName());

	return UpdatePath(newFile.GetPath(), true);
}





#pragma mark NMixinComparable
//=============================================================================
//		NFile::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NFile::CompareEqual(const NFile& theFile) const
{


	// Compare the paths
	return mInfo.GetPath().CompareEqual(theFile.mInfo.GetPath());
}





//=============================================================================
//		NFile::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NFile::CompareOrder(const NFile& theFile) const
{


	// Order by path
	return mInfo.GetPath().CompareOrder(theFile.mInfo.GetPath());
}





#pragma mark private
//=============================================================================
//		NFile::UpdatePath : Update the path.
//-----------------------------------------------------------------------------
NStatus NFile::UpdatePath(const NFilePath& newPath, bool renameFile)
{


	// Rename the file
	NStatus theErr = NStatus::OK;

	if (renameFile)
	{
		if (NFileInfo(newPath).Exists())
		{
			theErr = NStatus::Duplicate;
		}
		else
		{
			theErr = NFileUtils::Rename(GetPath(), newPath);
		}
	}



	// Update our state
	if (theErr == NStatus::OK)
	{
		SetPath(newPath);
	}

	return theErr;
}
