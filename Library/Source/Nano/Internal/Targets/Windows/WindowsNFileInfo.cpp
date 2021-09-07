/*	NAME:
		WindowsNFileInfo.cpp

	DESCRIPTION:
		Windows file info.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFileInfo.h"

// Nano
#include "NDebug.h"
#include "NWindows.h"

// System
#include <io.h>
#include <sys/stat.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
inline constexpr NFileStateFlags kNFileStateMaskStat =
	NFileState::CanRead | NFileState::CanWrite | NFileState::CanExecute;

inline constexpr NFileStateFlags kNFileStateMaskAttributes =
	NFileState::Exists | NFileState::IsFile | NFileState::IsDirectory | NFileState::CreationTime |
	NFileState::ModifiedTime | NFileState::FileSize;





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetFileStateStat : Get file state with stat().
//-----------------------------------------------------------------------------
bool GetFileStateStat(const NFilePath& thePath, NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());


	// Get the state we need
	struct _stat64 theInfo
	{
	};

	int  sysErr = _wstat64(LPCWSTR(thePath.GetUTF16()), &theInfo);
	bool wasOK  = (sysErr == 0);



	// Update the state
	if (wasOK)
	{
		if ((theInfo.st_mode & _S_IREAD) != 0)
		{
			theState.theFlags.Set(NFileState::CanRead);
		}
		else
		{
			theState.theFlags.Clear(NFileState::CanRead);
		}

		if ((theInfo.st_mode & _S_IWRITE) != 0)
		{
			theState.theFlags.Set(NFileState::CanWrite);
		}
		else
		{
			theState.theFlags.Clear(NFileState::CanWrite);
		}

		if ((theInfo.st_mode & _S_IEXEC) != 0)
		{
			theState.theFlags.Set(NFileState::CanExecute);
		}
		else
		{
			theState.theFlags.Clear(NFileState::CanExecute);
		}
	}

	return wasOK;
}





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetFileStateAttributes : Get file attribute state.
//-----------------------------------------------------------------------------
static bool GetFileStateAttributes(const NFilePath& thePath, NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state we need
	WIN32_FILE_ATTRIBUTE_DATA theInfo{};

	bool wasOK = GetFileAttributesExW(LPCWSTR(thePath.GetUTF16()), GetFileExInfoStandard, &theInfo);



	// Update the state
	if (wasOK)
	{
		theState.theFlags |= NFileState::Exists;

		if ((theInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			theState.theFlags.Set(NFileState::IsDirectory);
			theState.theFlags.Clear(NFileState::IsFile);
			theState.fileSize = 0;
		}
		else
		{
			theState.theFlags.Set(NFileState::IsFile);
			theState.theFlags.Clear(NFileState::IsDirectory);
			theState.fileSize = ToNN(theInfo.nFileSizeHigh, theInfo.nFileSizeLow);
		}

		theState.creationTime = NTime(ToNN(theInfo.ftCreationTime), kNanoEpochFrom1601);

		theState.modifiedTime = NTime(ToNN(theInfo.ftLastWriteTime), kNanoEpochFrom1601);
	}

	return wasOK;
}





#pragma mark NFileInfo
//=============================================================================
//		NFileInfo::FetchState : Fetch the requested state.
//-----------------------------------------------------------------------------
bool NFileInfo::FetchState(NFileStateFlags theFlags)
{


	// Validate our parameters
	NN_REQUIRE(mPath.IsValid());
	NN_REQUIRE(theFlags);



	// Fetch with stat
	bool wasOK = true;

	if (wasOK && (theFlags & kNFileStateMaskStat))
	{
		wasOK = GetFileStateStat(mPath, mState);
		if (wasOK)
		{
			mValid |= NFileState::MaskStat;
		}
	}



	// Fetch with GetFileAttributes
	if (wasOK && (theFlags & kNFileStateMaskAttributes))
	{
		wasOK = GetFileStateAttributes(mPath, mState);
		if (wasOK)
		{
			mValid |= NFileState::MaskAttributes;
		}
	}

	return wasOK;
}
