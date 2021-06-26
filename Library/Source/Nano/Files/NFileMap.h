/*	NAME:
		NFileMap.h

	DESCRIPTION:
		Memory-mapped file.

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
#ifndef NFILE_MAP_H
#define NFILE_MAP_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFilePath.h"
#include "NMutex.h"

// System
#include <optional>
#include <unordered_map>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Map access
//
// Memory may be mapped for access in one mode at a time:
//
//		NMapAccess::ReadWrite		Memory may be read from and written to.
//
//		NMapAccess::ReadOnly		Memory may only be read from.
//
//		NMapAccess::CopyOnWrite		Memory will be copied if written to.
//
enum class NMapAccess
{
	ReadWrite,
	ReadOnly,
	CopyOnWrite,
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NFile;


// Mapped pages
struct NFileMapping
{
	size_t         theOffset;
	size_t         theSize;
	const uint8_t* thePtr;
	NMapAccess     theAccess;
};


// Native file handle
using NFileMapRef = std::optional<uintptr_t>;


// Containers
using NMapFileMappings = std::unordered_map<const uint8_t*, NFileMapping>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileMap
{
public:
										NFileMap();
									   ~NFileMap();


	// Is the mapping open?
	bool                                IsOpen() const;


	// Get the path
	NFilePath                           GetPath() const;


	// Open the file map
	//
	// A file can be mapped as read-only, read-write, or copy-on-write.
	//
	// Pages in a read-only file can only be mapped as read-only.
	//
	// Pages in a read-write file can be mapped as either read-only or read-write.
	// If mapped as read-write, and the pages are modified, the file on disk will
	// be updated.
	//
	// Pages in a copy-on-write file can be mapped as either read-only or read-write.
	// If mapped as read-write, and the pages are modified, the file on disk will not
	// be updated (as pages are mutable but transient).
	NStatus                             Open(const NFile& theFile, NMapAccess theAccess = NMapAccess::ReadOnly);

	NStatus                             Open(const NFilePath& thePath, NMapAccess theAccess = NMapAccess::ReadOnly);


	// Close the file map
	//
	// Open file maps are closed automatically when they go out of scope.
	void                                Close();


	// Map pages
	//
	// A size of 0 will create a mapping for the entire file.
	//
	// The const may only be cast away if mutable access was requested.
	const void*                         Map(uint64_t   theOffset = 0,
											size_t     theSize   = 0,
											NMapAccess theAccess = NMapAccess::ReadOnly);


	// Unmap pages
	void                                Unmap(const void* thePtr);


private:
	size_t                              MapPageSize();
	NFileMapRef                         MapOpen(const NFilePath& thePath, NMapAccess theAccess);
	void                                MapClose(  NFileMapRef theHandle);
	void                                MapFetch(  NFileMapRef theHandle,       NFileMapping& theMapping);
	void                                MapDiscard(NFileMapRef theHandle, const NFileMapping& theMapping);


private:
	mutable NMutex                      mLock;

	NFilePath                           mPath;
	NFileMapRef                         mHandle;
	NMapFileMappings                    mMappings;
	size_t                              mFileSize;
};



#endif // NFILE_MAP_H
