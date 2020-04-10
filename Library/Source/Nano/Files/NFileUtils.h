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
#include "NanoConstants.h"





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





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileUtils
{
public:
	// Create a file / directory
	//
	// Creates any parent directories as required.
	//
	// Returns NStatus::Duplicate if the path already exists and
	// may not be deleted.
	static NStatus                      CreateFile(     const NFilePath& thePath, bool deleteExisting = false);
	static NStatus                      CreateDirectory(const NFilePath& thePath, bool deleteExisting = false);


	// Delete a path
	//
	// Platforms that do not support a trash will always delete.
	static NStatus                      Delete(const NFilePath& thePath, bool moveToTrash = false);


	// Delete the children of a directory
	//
	// Recursively deletes the children of a directory, leaving
	// the directory itself in place.
	static NStatus                      DeleteChildren(const NFilePath& thePath, bool moveToTrash = false);


	// Get the children of a directory
	//
	// Returns the immediate children of a directory.
	static NVectorFilePath              GetChildren(const NFilePath& thePath);


	// Get a uniquely named file
	//
	// Returns a uniquely named file within the specified directory.
	//
	// A base name can be provided to control the file stem and extension.
	static NFilePath                    GetUniqueChild(const NFilePath& thePath, const NString baseName = "");


	// Get a location
	//
	// Get the path to a location, with an optional child directory.
	//
	// The canCreate flag indicates if the final path should be created if
	// does not exist.
	//
	// Returns an empty path if the location is not supported, or the final
	// path cannot be created.
	static NFilePath                    GetLocation(NFileLocation  theLocation,
													const NString& theChild  = "",
													bool           canCreate = false);


	// Atomically rename a path
	static NStatus                      Rename(const NFilePath& oldPath, const NFilePath& newPath);


	// Atomically exchange two paths
	static NStatus                      Exchange(const NFilePath& oldPath, const NFilePath& newPath);


private:
	static NStatus                      PathCreate(  const NFilePath& thePath);
	static NStatus                      PathDelete(  const NFilePath& thePath, bool moveToTrash);
	static NVectorFilePath              PathChildren(const NFilePath& thePath);
	static NStatus                      PathRename(  const NFilePath& oldPath, const NFilePath& newPath);
	static NStatus                      PathExchange(const NFilePath& oldPath, const NFilePath& newPath);
	static NFilePath                    PathLocation(NFileLocation theLocation);
};



#endif // NFILE_UTILS_H
