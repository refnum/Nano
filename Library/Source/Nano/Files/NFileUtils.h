/*	NAME:
		NFileUtils.h

	DESCRIPTION:
		File utilities.

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
#ifndef NFILE_UTILS_H
#define NFILE_UTILS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFilePath.h"
#include "NString.h"
#include "NanoConstants.h"
#include "NanoTargets.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Locations
//
// A platform-specific location for files of a particular purpose:
//
//		NFileLocation::AppCaches			Cache files.
//
//				Typically preserved across reboots, but may be purged by
//				the system at any time.
//
//		NFileLocation::AppSupport			Support files.
//
//				Non-preference files installed with, or created by, the app.
//
//		NFileLocation::AppTemporaries		Temporary files.
//
//				Typically deleted between reboots, but may be purged by
//				the system at any time.
//
//		NFileLocation::SharedSupport		Shared support files.
//
//				Shared equivalent to NFileLocation::AppSupport.
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


// Paths
//
// A path can be broken into distinct parts:
//
//		NPath::Parent				The parent directory of the item
//
//		NPath::Name					The name of the item
//
//		NPath::Extension			The extension of the item
//
enum class NPathPart
{
	Parent,
	Name,
	Extension,
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileUtils
{
public:
	// Create a file / directory
	//
	// Creates an empty file / directory, and any required parent directories.
	//
	// Returns NStatus::Duplicate if an item already exists, unless any such
	// existing item is to be deleted.
	static NStatus                      CreateFile(     const NString& thePath, bool deleteExisting = false);
	static NStatus                      CreateDirectory(const NString& thePath, bool deleteExisting = false);


	// Delete a path
	//
	// Deleting a path, rather than moving it to the trash, is permanent.
	static NStatus                      Delete(const NString& thePath, bool moveToTrash = false);


	// Delete the children of a directory
	static NStatus                      DeleteChildren(const NString& thePath, bool moveToTrash = false);


	// Get the children of a directory
	static NVectorString                GetChildren(const NString& thePath);


	// Get a location
	//
	// Get a location, with an optional path to a child directory.
	//
	// The final location can be created if it does not exist.
	//
	// Returns an empty path if the directory does not exist, or cannot be created.
	static NString                      GetLocation(NFileLocation  theLocation,
													const NString& pathChild = "",
													bool           canCreate = false);


	// Get part of a path
	static NString                      GetPathPart(const NString& thePath, NPathPart thePart);


	// Atomically rename a path
	static NStatus                      Rename(const NString& oldPath, const NString& newPath);


	// Atomically exchange two paths
	static NStatus                      Exchange(const NString& oldPath, const NString& newPath);


private:
	static NStatus                      MakeDirectory(const NString& thePath);
	static NStatus                      DeletePath(   const NString& thePath, bool moveToTrash);
	static NStatus                      GetLocation(NFileLocation theLocation, NString& thePath);
};



#endif // NFILE_UTILS_H
