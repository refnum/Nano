/*	NAME:
		NFile.h

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
#ifndef NFILE_H
#define NFILE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NFileInfo.h"
#include "NMixinComparable.h"
#include "NString.h"

// System
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NFile;


// Containers
using NVectorFile = std::vector<NFile>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NFile final : public NMixinComparable<NFile>
{
public:
										NFile();
										NFile(const NString& thePath);


	// Is the file valid?
	//
	// A valid file has a non-empty path.
	bool                                IsValid() const;


	// Clear the file
	//
	// Resets the file to an empty path.
	void                                Clear();


	// Get/set the path
	NString                             GetPath() const;
	void                                SetPath(  const NString& thePath);


	// Refresh the state
	void                                Refresh();


	// Get the status
	bool                                Exists()      const;
	bool                                IsFile()      const;
	bool                                IsDirectory() const;


	// Get the permissions
	bool                                CanRead()    const;
	bool                                CanWrite()   const;
	bool                                CanExecute() const;


	// Get the timestamps
	NTime                               GetCreationTime() const;
	NTime                               GetModifiedTime() const;


	// Get/set the name
	//
	// Returns NStatus::Duplicate if a file already exists with the new name.
	NString                             GetName() const;
	NStatus                             SetName(  const NString& theName, bool renameFile = false);


	// Get/set the extension
	//
	// Returns NStatus::Duplicate if a file already exists with the new name.
	NString                             GetExtension() const;
	NStatus                             SetExtension(  const NString& theExtension, bool renameFile = false);


	// Get the child/parent
	//
	// Only directories have children, however all files/directories except
	// the root directory have a parent.
	//
	// The child name may include path components.
	NFile                               GetChild(   const NString& theName) const;
	NFile                               GetParent() const;


	// Get the size of a file
	//
	// Only valid for files.
	uint64_t                            GetSize() const;


	// Get the children of a directory
	//
	// Only valid for directories.
	NVectorFile                         GetChildren() const;


	// Create an empty file / directory
	//
	// Parent directories will be created as necessary.
	NStatus                             CreateFile();
	NStatus                             CreateDirectory();


public:
	// NMixinComparable
	bool                                CompareEqual(const NFile& theFile) const;
	NComparison                         CompareOrder(const NFile& theFile) const;


private:
	NString                             GetPathComponent(const NString& thePattern) const;
	NStatus                             UpdatePath(      const NString& newPath, bool renameFile);


private:
	NFileInfo                           mInfo;
};



#endif // NFILE_H
