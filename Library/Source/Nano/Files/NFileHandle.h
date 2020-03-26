/*	NAME:
		NFileHandle.h

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
#ifndef NFILE_HANDLE_H
#define NFILE_HANDLE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NString.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NFile;

// Native file handle
using NFileHandleRef = void*;





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Native file handle
static constexpr NFileHandleRef kNFileHandleNone = nullptr;


// File access
//
// A file may be opened for access in one mode at a time:
//
//		NFileAccess::ReadWrite		The file may be read from and written to.
//
//		NFileAccess::ReadOnly		The file may only be read from.
//
//		NFileAccess::WriteOnly		The file may only be written to.
//
// Opening a file in NFileAccess::ReadWrite or NFileAccess::WriteOnly mode
// will create the file if it does not exist.
//
// Opening a file in NFileAccess::ReadOnly mode will return NStatus::NotFound
// if the file does not exist.
//
enum class NFileAccess
{
	ReadWrite,
	ReadOnly,
	WriteOnly,
};


// File flags
//
// A file may be opened with flags to indicate its likely usage:
//
//		kNFileDefault				Flags are determined by the access mode.
//
//		kNFileWillRead				The file will definitely be read.
//
//		kNFileDeleteOnClose			The file is deleted when closed.
//
//		kNFilePositionSequential	The position will typically increase.
//
//		kNFilePositionRandom		The position may change at random.
//
// The kNFileDefault flag chooses an appropriate set of flags for the access
// mode that the file was opened with:
//
//		NFileAccess::ReadWrite		kNFileWillRead + kNFilePositionSequential
//
//		NFileAccess::ReadOnly		kNFileWillRead + kNFilePositionSequential
//
//		NFileAccess::WriteOnly		kNFilePositionSequential
//
using NFileFlags                                            = uint8_t;

static constexpr NFileFlags kNFileDefault                   = 0;
static constexpr NFileFlags kNFileWillRead                  = (1 << 1);
static constexpr NFileFlags kNFileDeleteOnClose             = (1 << 2);
static constexpr NFileFlags kNFilePositionSequential        = (1 << 3);
static constexpr NFileFlags kNFilePositionRandom            = (1 << 4);


// File offsets
//
// The file offset mode determines how a relative offset is converted to an
// absolute position.
enum class NFileOffset
{
	FromStart,
	FromPosition,
	FromEnd
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileHandle final
{
public:
										NFileHandle();
									   ~NFileHandle();

										NFileHandle(const NFileHandle&) = delete;
	NFileHandle&                        operator=(  const NFileHandle&) = delete;

										NFileHandle(NFileHandle&&) = delete;
	NFileHandle&                        operator=(  NFileHandle&&) = delete;


	// Is the handle open?
	bool                                IsOpen() const;


	// Get the path
	NString                             GetPath() const;


	// Open/close the handle
	//
	// An open handle must be closed to open it in a different access
	// mode. An open handle will be closed when it goes out of scope.
	NStatus                             Open(const NFile& theFile,
											 NFileAccess  theAccess = NFileAccess::ReadOnly,
											 NFileFlags   theFlags  = kNFileDefault);

	NStatus                             Open(const NString& thePath,
											 NFileAccess    theAccess = NFileAccess::ReadOnly,
											 NFileFlags     theFlags  = kNFileDefault);

	void                                Close();


	// Get/set the position
	//
	// The default position is 0.
	//
	// The file must be open to access its position.
	uint64_t                            GetPosition() const;
	NStatus                             SetPosition(int64_t theOffset, NFileOffset relativeTo = NFileOffset::FromStart);


	// Get/set the size
	//
	// The file may be closed to query the size, but must be
	// opened for writing to adjust its size.
	//
	// Extending the file size will zero-fill the new content.
	uint64_t                            GetSize() const;
	NStatus                             SetSize(uint64_t theSize);


	// Read from the file
	//
	// The file must be opened for reading.
	NStatus                             Read(uint64_t    theSize,
											 void*       thePtr,
											 uint64_t&   numRead,
											 int64_t     theOffset  = 0,
											 NFileOffset relativeTo = NFileOffset::FromPosition);


	// Write to the file
	//
	// The file must be opened for writing.
	NStatus                             Write(uint64_t    theSize,
											  const void* thePtr,
											  uint64_t&   numWritten,
											  int64_t     theOffset  = 0,
											  NFileOffset relativeTo = NFileOffset::FromPosition);


	// Flush the file
	//
	// Flush unwritten data to disk.
	//
	// The file must be opened for writing.
	NStatus                             Flush();


private:
	bool                                CanRead()  const;
	bool                                CanWrite() const;

	NFileFlags                          GetOpenFlags(NFileAccess theAccess, NFileFlags theFlags);

	NStatus                             FileOpen(const NString& thePath, NFileAccess theAccess, NFileFlags theFlags);
	void                                FileClose();
	uint64_t                            FileGetPosition() const;
	NStatus                             FileSetPosition(int64_t thePosition, NFileOffset relativeTo);
	NStatus                             FileSetSize(uint64_t theSize);
	NStatus                             FileRead(   uint64_t theSize,       void* thePtr, uint64_t& numRead);
	NStatus                             FileWrite(  uint64_t theSize, const void* thePtr, uint64_t& numWritten);
	NStatus                             FileFlush();


private:
	NString                             mPath;
	NFileAccess                         mAccess;
	NFileHandleRef                      mHandle;
};



#endif // NFILE_HANDLE_H
