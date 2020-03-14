/*	NAME:
		NFileInfo.h

	DESCRIPTION:
		File information.

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
#ifndef NFILE_INFO_H
#define NFILE_INFO_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NString.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
using NFileInfoState                                        = uint32_t;

static constexpr NFileInfoState kNFileInfoNone              = 0;
static constexpr NFileInfoState kNFileInfoAll               = ~kNFileInfoNone;

static constexpr NFileInfoState kNFileInfoExists            = (1 << 0);
static constexpr NFileInfoState kNFileInfoIsFile            = (1 << 1);
static constexpr NFileInfoState kNFileInfoIsDirectory       = (1 << 2);
static constexpr NFileInfoState kNFileInfoSize              = (1 << 3);





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileInfo final
{
public:
										NFileInfo();
										NFileInfo(const NString& thePath, NFileInfoState theState);


	// Get/set the state
	//
	// Changing the state will refresh the info.
	NFileInfoState                      GetState() const;
	void                                SetState(NFileInfoState theState);


	// Get/set the path
	//
	// CHanging the path will refresh the info.
	NString                             GetPath() const;
	void                                SetPath(  const NString& thePath);


	// Does the path exist?
	bool                                Exists() const;


	// Is the path a file?
	bool                                IsFile() const;


	// Is the path a directory?
	bool                                IsDirectory() const;


	// Get the size
	size_t                              GetSize() const;


private:
	bool                                HasFlag(NFileInfoState theFlag) const;

	void                                RefreshInfo();


private:
	NFileInfoState                      mState;
	NString                             mPath;

	uint64_t                            mSize;
	NFileInfoState                      mFlags;
};



#endif // NFILE_INFO_H
