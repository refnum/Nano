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
#include "NFile.h"
#include "NanoConstants.h"
#include "NanoTargets.h"





//=============================================================================
//		Internalonstants
//-----------------------------------------------------------------------------
// Directory separator
#if NN_TARGET_WINDOWS
static constexpr const char* kNPathSeparator                = "\\";
#else
static constexpr const char* kNPathSeparator                = "/";
#endif


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
enum NPathPart
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
	// Get part of a path
	static NString                      GetPathPart(const NString& thePath, NPathPart thePart);


	// Get the children of a directory
	static NVectorFile                  GetChildren(const NString& thePath);


	// Create a file / directory
	//
	// Creates an empty file / directory, and any required parent directories.
	//
	// Returns NStatus::Duplicate if an item already exists, unless any
	// existing item is to be deleted.
	static NStatus                      CreateFile(     const NString& thePath, bool deleteExisting = false);
	static NStatus                      CreateDirectory(const NString& thePath, bool deleteExisting = false);


	// Atomically rename a file
	static NStatus                      Rename(const NString& oldPath, const NString& newPath);


	// Atomically exchange two files.
	static NStatus                      Exchange(const NString& oldPath, const NString& newPath);


	// Delete a file
	static NStatus                      Delete(const NString& thePath, bool moveToTrash = false);


private:
	static NStatus                      MakeDirectory(const NString& thePath);
};



#endif // NFILE_UTILS_H