/*	NAME:
		NFileInfo.h

	DESCRIPTION:
		File information.

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
#ifndef NFILE_INFO_H
#define NFILE_INFO_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFilePath.h"
#include "NTime.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
using NFileInfoFlags                                        = uint16_t;

inline constexpr NFileInfoFlags kNFileInfoNone              = 0;
inline constexpr NFileInfoFlags kNFileInfoExists            = (1 << 0);
inline constexpr NFileInfoFlags kNFileInfoIsFile            = (1 << 1);
inline constexpr NFileInfoFlags kNFileInfoIsDirectory       = (1 << 2);
inline constexpr NFileInfoFlags kNFileInfoCanRead           = (1 << 3);
inline constexpr NFileInfoFlags kNFileInfoCanWrite          = (1 << 4);
inline constexpr NFileInfoFlags kNFileInfoCanExecute        = (1 << 5);
inline constexpr NFileInfoFlags kNFileInfoCreationTime      = (1 << 6);
inline constexpr NFileInfoFlags kNFileInfoModifiedTime      = (1 << 7);
inline constexpr NFileInfoFlags kNFileInfoFileSize          = (1 << 8);





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
struct NFileInfoState
{
	NFileInfoFlags theFlags;
	NTime          creationTime;
	NTime          modifiedTime;
	uint64_t       fileSize;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileInfo
{
public:
										NFileInfo(const NFilePath& thePath);
										NFileInfo();


	// Is the object valid?
	//
	// A valid object has a non-empty path.
	bool                                IsValid() const;


	// Clear the object
	//
	// Resets the object to an empty path.
	void                                Clear();


	// Get/set the path
	//
	// The path must be set before any state can be queried.
	NFilePath                           GetPath() const;
	void                                SetPath(  const NFilePath& thePath);


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


	// Get the file size
	//
	// Only valid for files.
	uint64_t                            GetFileSize() const;


private:
	bool                                TestFlag(NFileInfoFlags theFlag) const;

	void                                UpdateState(NFileInfoFlags theFlags) const;
	bool                                FetchState( NFileInfoFlags theFlags);


private:
	NFilePath                           mPath;
	NFileInfoFlags                      mValid;
	NFileInfoState                      mState;
};



#endif // NFILE_INFO_H
