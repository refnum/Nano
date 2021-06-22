/*	NAME:
		WindowsNFileMap.cpp

	DESCRIPTION:
		Android file map.

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
#include "NFileMap.h"

// Nano
#include "NDebug.h"
#include "NSharedWindows.h"





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetMappingFlag : Get a file mapping flag.
//----------------------------------------------------------------------------
DWORD GetMappingFlag(NMapAccess theAccess, bool forPage)
{


	// Get the flag
	//
	// Windows has two parallel sets of constants for creating a file map
	// (PAGE_xxxx) and for mapping pages within that file (FILE_MAP_xxxx).
	//
	// Confusingly, the names are reversed: PAGE_xxx is for the file and
	// FILE_MAP_xxxx is for the pages.
	//
	// Return the appropriate flag for our access request based on usage.
	switch (theAccess)
	{
		case NMapAccess::ReadOnly:
			return DWORD(forPage ? FILE_MAP_READ : PAGE_READONLY);
			break;

		case NMapAccess::ReadWrite:
			return DWORD(forPage ? FILE_MAP_WRITE : PAGE_READWRITE);
			break;

		case NMapAccess::CopyOnWrite:
			return DWORD(forPage ? FILE_MAP_COPY : PAGE_WRITECOPY);
			break;
	}

	NN_UNREACHABLE();
}





#pragma mark NFileMap
//=============================================================================
//		NFileMap::MapPageSize : Get the file mapping page size.
//-----------------------------------------------------------------------------
size_t NFileMap::MapPageSize()
{


	// Get the page size
	SYSTEM_INFO systemInfo{};

	GetSystemInfo(&systemInfo);
	NN_REQUIRE(systemInfo.dwAllocationGranularity > 0);

	return size_t(systemInfo.dwAllocationGranularity);
}





//=============================================================================
//		NFileMap::MapOpen : Open a file for mapping.
//-----------------------------------------------------------------------------
NFileMapRef NFileMap::MapOpen(const NFilePath& thePath, NMapAccess theAccess)
{


	// Get the state we need
	DWORD theFlags = GENERIC_READ;

	if (theAccess == NMapAccess::ReadWrite)
	{
		theFlags |= GENERIC_WRITE;
	}



	// Open the file
	//
	// The file handle is retained by the mapping handle.
	NFileMapRef theHandle;

	HANDLE hndFile = CreateFileW(LPCWSTR(thePath.GetUTF16()),
								 theFlags,
								 FILE_SHARE_READ,
								 nullptr,
								 OPEN_ALWAYS,
								 FILE_ATTRIBUTE_NORMAL,
								 nullptr);

	if (hndFile != INVALID_HANDLE_VALUE)
	{
		HANDLE hndMap =
			CreateFileMapping(hndFile, nullptr, GetMappingFlag(theAccess, false), 0, 0, nullptr);

		if (hndMap != INVALID_HANDLE_VALUE)
		{
			theHandle = uintptr_t(hndMap);
		}

		bool wasOK = CloseHandle(hndFile);
		NN_EXPECT(wasOK);
	}

	return theHandle;
}





//=============================================================================
//		NFileMap::MapClose : Close a mapped file.
//-----------------------------------------------------------------------------
void NFileMap::MapClose(NFileMapRef theHandle)
{


	// Close the file
	HANDLE hndMap = HANDLE(theHandle.value());
	NN_REQUIRE(hndMap != INVALID_HANDLE_VALUE);

	bool wasOK = CloseHandle(hndMap);
	NN_EXPECT(wasOK);
}





//=============================================================================
//		NFileMap::MapFetch : Fetch mapped pages.
//-----------------------------------------------------------------------------
void NFileMap::MapFetch(NFileMapRef theHandle, NFileMapping& theMapping)
{


	// Get the state we need
	HANDLE hndMap = HANDLE(theHandle.value());
	NN_REQUIRE(hndMap != INVALID_HANDLE_VALUE);

	LARGE_INTEGER theOffset = NSharedWindows::ToLargeInteger(int64_t(theMapping.theOffset));



	// Map the pages
	theMapping.thePtr =
		static_cast<const uint8_t*>(MapViewOfFile(hndMap,
												  GetMappingFlag(theMapping.theAccess, true),
												  DWORD(theOffset.HighPart),
												  theOffset.LowPart,
												  theMapping.theSize));
}





//=============================================================================
//		NFileMap::MapDiscard : Discard mapped pages.
//-----------------------------------------------------------------------------
void NFileMap::MapDiscard(NFileMapRef theHandle, const NFileMapping& theMapping)
{


	// Get the state we need
	HANDLE hndMap = HANDLE(theHandle.value());
	NN_REQUIRE(hndMap != INVALID_HANDLE_VALUE);

	bool wasOK = false;



	// Flush mutable pages
	if (theMapping.theAccess == NMapAccess::ReadWrite)
	{
		wasOK = FlushViewOfFile(theMapping.thePtr, theMapping.theSize);
		NN_EXPECT(wasOK);
	}



	// Discard the pages
	wasOK = UnmapViewOfFile(theMapping.thePtr);
	NN_EXPECT(wasOK);
}
