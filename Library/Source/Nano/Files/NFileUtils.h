/*	NAME:
		NFileUtils.h

	DESCRIPTION:
		File utilities.

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
#ifndef NFILE_UTILS_H
#define NFILE_UTILS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFilePath.h"
#include "NanoConstants.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Actions
//
// Determines what actions can be taken on a file:
//
//		NFileAction::CanCreate				Creation is allowed.
//
//		NFileAction::CanDelete				Deletion is allowed.
//
//		NFileAction::CanTrash				Moving to the trash is allowed.
//
//		NFileAction::DontCreate				Nothing should be created.
//
//		NFileAction::DontDelete				Nothing should be deleted.
//
// Methods that take an action typically only support the subset of
// actions that make sense for their particular operation.
//
// The exact behaviour for each action is method-specific.
enum class NFileAction
{
	CanCreate,
	CanDelete,
	CanTrash,
	DontCreate,
	DontDelete
};


// Locations
//
// A platform-specific location for files of a particular purpose:
//
//		NFileLocation::AppCaches			Cache files.
//
//			Typically preserved across reboots, but may be purged by
//			the system at any time.
//
//		NFileLocation::AppSupport			Support files.
//
//			Non-preference files installed with, or created by, the app.
//
//		NFileLocation::AppTemporaries		Temporary files.
//
//			Typically deleted between reboots, but may be purged by
//			the system at any time.
//
//		NFileLocation::SharedSupport		Shared support files.
//
//			Shared equivalent to NFileLocation::AppSupport.
//
//		NFileLocation::UserDesktop			The user's desktop directory.
//
//		NFileLocation::UserDocuments		The user's documents directory.
//
//		NFileLocation::UserDownloads		The user's downloads directory.
//
//		NFileLocation::UserHome				The user's home directory.
//
//		NFileLocation::UserLogs				The user's log directory.
//
//		NFileLocation::UserPictures			The user's pictures directory.
//
//		NFileLocation::UserPreferences		The user's preferences directory.
//
enum class NFileLocation
{
	AppCaches,
	AppSupport,
	AppTemporaries,
	SharedSupport,
	UserDesktop,
	UserDocuments,
	UserDownloads,
	UserHome,
	UserLogs,
	UserPictures,
	UserPreferences
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileUtils
{
public:
	// Create a file / directory
	//
	// Supported actions:
	//
	//		NFileAction::CanDelete
	//		NFileAction::CanTrash
	//		NFileAction::DontDelete
	//
	// Any existing item will be deleted, or moved to the trash, before a
	// new item is created at the path.
	//
	// Passing NFileAction::DontDelete will return NStatus::Duplicate if
	// the path already exists.
	//
	// Parent directories in the path are created as required.
	static NStatus                      CreateFile(const NFilePath& thePath,
												   NFileAction      theAction = NFileAction::DontDelete);

	static NStatus                      CreateDirectory(const NFilePath& thePath,
														NFileAction      theAction = NFileAction::DontDelete);


	// Delete a path
	//
	// Supported actions:
	//
	//		NFileAction::CanDelete
	//		NFileAction::CanTrash
	//
	// Platforms that do not support a trash will always delete.
	static NStatus                      Delete(const NFilePath& thePath, NFileAction theAction = NFileAction::CanDelete);


	// Delete the children of a directory
	//
	// Supported actions:
	//
	//		NFileAction::CanDelete
	//		NFileAction::CanTrash
	//
	static NStatus                      DeleteChildren(const NFilePath& thePath,
													   NFileAction      theAction = NFileAction::CanDelete);


	// Get the children of a directory
	//
	// Returns the immediate children of a directory.
	static NVectorFilePath              GetChildren(const NFilePath& thePath);


	// Get a uniquely named file
	//
	// Returns a uniquely named file within the specified directory.
	//
	// A base name can be provided to control the file stem and extension.
	static NFilePath                    GetUniqueChild(const NFilePath& thePath, const NString& baseName = "");


	// Get a location
	//
	// Get the path to a location, or a directory within that location.
	//
	// Supported actions:
	//
	//		NFileAction::CanCreate
	//		NFileAction::DontCreate
	//
	// Returns an empty path if the location is not supported, or creation was
	// requested but the final path could not be created.
	static NFilePath                    GetLocation(NFileLocation  theLocation,
													const NString& theChild  = "",
													NFileAction    theAction = NFileAction::CanCreate);


	// Get a temporary file
	//
	// Returns a unqiuely named temporary file.
	static NFilePath                    GetTemporary(const NString& baseName = "");


	// Atomically rename a path
	static NStatus                      Rename(const NFilePath& pathOld, const NFilePath& pathNew);


	// Atomically exchange two paths
	static NStatus                      Exchange(const NFilePath& oldPath, const NFilePath& newPath);


private:
	static NStatus                      PathCreate(  const NFilePath& thePath);
	static NStatus                      PathDelete(  const NFilePath& thePath, NFileAction theAction);
	static NVectorFilePath              PathChildren(const NFilePath& thePath);
	static NStatus                      PathRename(  const NFilePath& pathOld, const NFilePath& pathNew);
	static NStatus                      PathExchange(const NFilePath& oldPath, const NFilePath& newPath);
	static NFilePath                    PathLocation(NFileLocation theLocation);
};



#endif // NFILE_UTILS_H
