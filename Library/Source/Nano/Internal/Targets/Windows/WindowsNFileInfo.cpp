/*	NAME:
		WindowsNFileInfo.cpp

	DESCRIPTION:
		Windows file info.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFileInfo.h"

// Nano
#include "NDebug.h"
#include "NSharedWindows.h"

// System
#include <io.h>
#include <sys/stat.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr NFileInfoFlags kNFileInfoMaskStat =
	kNFileInfoCanRead | kNFileInfoCanWrite | kNFileInfoCanExecute;

static constexpr NFileInfoFlags kNFileInfoMaskAttributes =
	kNFileInfoExists | kNFileInfoIsFile | kNFileInfoIsDirectory | kNFileInfoCreationTime |
	kNFileInfoModifiedTime | kNFileInfoFileSize;





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
			theState.theFlags |= kNFileInfoCanRead;
		}
		else
		{
			theState.theFlags &= ~kNFileInfoCanRead;
		}

		if ((theInfo.st_mode & _S_IWRITE) != 0)
		{
			theState.theFlags |= kNFileInfoCanWrite;
		}
		else
		{
			theState.theFlags &= ~kNFileInfoCanWrite;
		}

		if ((theInfo.st_mode & _S_IEXEC) != 0)
		{
			theState.theFlags |= kNFileInfoCanExecute;
		}
		else
		{
			theState.theFlags &= ~kNFileInfoCanExecute;
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
		theState.theFlags |= kNFileInfoExists;

		if ((theInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{
			theState.theFlags |= kNFileInfoIsDirectory;
			theState.theFlags &= ~kNFileInfoIsFile;
			theState.fileSize = 0;
		}
		else
		{
			theState.theFlags |= kNFileInfoIsFile;
			theState.theFlags &= ~kNFileInfoIsDirectory;
			theState.fileSize =
				NSharedWindows::ToUInt64(theInfo.nFileSizeHigh, theInfo.nFileSizeLow);
		}

		theState.creationTime =
			NTime(NSharedWindows::ToInterval(theInfo.ftCreationTime), kNanoEpochFrom1601);

		theState.modifiedTime =
			NTime(NSharedWindows::ToInterval(theInfo.ftLastWriteTime), kNanoEpochFrom1601);
	}

	return wasOK;
}





#pragma mark NFileInfo
//=============================================================================
//		NFileInfo::FetchState : Fetch the requested state.
//-----------------------------------------------------------------------------
bool NFileInfo::FetchState(NFileInfoFlags theFlags)
{


	// Validate our parameters
	NN_REQUIRE(mPath.IsValid());
	NN_REQUIRE(theFlags != kNFileInfoNone);



	// Fetch with stat
	bool wasOK = true;

	if (wasOK && (theFlags & kNFileInfoMaskStat) != 0)
	{
		wasOK = GetFileStateStat(mPath, mState);
		if (wasOK)
		{
			mValid |= kNFileInfoMaskStat;
		}
	}



	// Fetch with GetFileAttributes
	if (wasOK && (theFlags & kNFileInfoMaskAttributes) != 0)
	{
		wasOK = GetFileStateAttributes(mPath, mState);
		if (wasOK)
		{
			mValid |= kNFileInfoMaskAttributes;
		}
	}

	return wasOK;
}
