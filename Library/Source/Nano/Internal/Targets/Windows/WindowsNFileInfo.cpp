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

// System
#include <io.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr NFileInfoFlags kNFileInfoMaskAttributes =
	kNFileInfoExists | kNFileInfoIsFile | kNFileInfoIsDirectory | kNFileInfoCreationTime |
	kNFileInfoModifiedTime | kNFileInfoFileSize;





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetFileStateAttributes : Get file attribute state.
//-----------------------------------------------------------------------------
static bool GetFileStateAttributes(const NString& thePath, NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(!thePath.IsEmpty());



	// Get the state we need
	WIN32_FILE_ATTRIBUTE_DATA theInfo{};

	bool wasOK = GetFileAttributesExW(thePath.GetUTF16(), GetFileExInfoStandard, &theInfo);



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
			theState.fileSize = ToUInt64(theInfo.nFileSizeHigh, theInfo.nFileSizeLow);
		}

		theState.creationTime =
			NTime(NSharedWindows::ToInterval(theInfo.ftCreationTime), kNanoEpochFrom1601);

		theState.modifiedTime =
			NTime(NSharedWindows::ToInterval(theInfo.ftLastWriteTime), kNanoEpochFrom1601);
	}

	return wasOK;
}





//=============================================================================
//		GetFileStateAccess : Get file state with _waccess().
//-----------------------------------------------------------------------------
void GetFileStateAccess(const NString& thePath, NFileInfoFlags theFlag, NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(!thePath.IsEmpty());

	NN_REQUIRE(theFlag == kNFileInfoCanRead || theFlag == kNFileInfoCanWrite ||
			   theFlag == kNFileInfoCanExecute);



	// Get the state we need
	int theMode = -1;

	switch (theFlag)
	{
		case kNFileInfoCanRead:
			theMode = 04;
			break;

		case kNFileInfoCanWrite:
			theMode = 02;
			break;

		case kNFileInfoCanExecute:
			NN_LOG_UNIMPLEMENTED();
			break;

		default:
			NN_LOG_ERROR("Unknown file info mode: {}", theFlag);
			break;
	}



	// Get the state
	if (theMode != -1)
	{
		int sysErr = _waccess(thePath.GetUTF8(), theMode);
		if (sysErr == 0)
		{
			theState.theFlags |= theFlag;
		}
		else
		{
			theState.theFlags &= ~theFlag;
		}
	}
}





#pragma mark NFileInfo
//=============================================================================
//		NFileInfo::FetchState : Fetch the requested state.
//-----------------------------------------------------------------------------
bool NFileInfo::FetchState(NFileInfoFlags theFlags)
{


	// Fetch with GetFileAttributes
	bool wasOK = true;

	if (wasOK && (theFlags & kNFileInfoMaskAttributes) != 0)
	{
		wasOK = GetFileStateAttributes(thePath, theState);
		if (wasOK)
		{
			validState |= kNFileInfoMaskAttributes;
		}
	}



	// Fetch with access
	if (wasOK)
	{
		if ((theFlags & kNFileInfoCanRead) != 0)
		{
			GetFileStateAccess(thePath, kNFileInfoCanRead, theState);
			validState |= kNFileInfoCanRead;
		}

		if ((theFlags & kNFileInfoCanWrite) != 0)
		{
			GetFileStateAccess(thePath, kNFileInfoCanWrite, theState);
			validState |= kNFileInfoCanWrite;
		}

		if ((theFlags & kNFileInfoCanExecute) != 0)
		{
			GetFileStateAccess(thePath, kNFileInfoCanExecute, theState);
			validState |= kNFileInfoCanExecute;
		}
	}

	return wasOK;
}
