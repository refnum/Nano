/*	NAME:
		WindowsNFileHandle.cpp

	DESCRIPTION:
		Windows file handle.

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
#include "NFileHandle.h"





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetPermisson : Get a permission string.
//-----------------------------------------------------------------------------
static constexpr UINT GetPermission(NFilePermission thePermission)
{


	// Get the permission
	switch (thePermission)
	{
		case NFilePermission::Read:
			return FILE_GENERIC_READ;
			break;

		case NFilePermission::Write:
			return FILE_GENERIC_WRITE;
			break;

		case NFilePermission::Update:
			return FILE_GENERIC_READ | FILE_GENERIC_WRITE;
			break;
	}
}





#pragma mark NFileHandle
//=============================================================================
//		NFileHandle::FileOpen : Open the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileOpen(NFilePermission thePermission)
{


	// Validate our state
	static_assert(sizeof(NFileHandleRef) >= sizeof(HANDLE));



	// Open the file
	NStatus theErr = NStatus::Permission;

	HANDLE theFile = CreateFile(LPCWSTR(mPath.GetUTF8()),
								GetPermission(thePermission),
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								nullptr,
								OPEN_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								nullptr);

	if (theFile != INVALID_HANDLE_VALUE)
	{
		mHandle = NFileHandleRef(theFile);
		theErr  = NStatus::NoErr;
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::FileClose : Close the file.
//-----------------------------------------------------------------------------
void NFileHandle::FileClose()
{


	// Get the state we need
	HANDLE theFile = static_cast<HANDLE>(mHandle);



	// Close the file
	BOOL wasOK = CloseHandle(theFile);
	NN_EXPECT(wasOK);
}





//=============================================================================
//		NFileHandle::FileSeek : Seek into the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileSeek(uint64_t thePosition)
{


	// Validate our state
	static_assert(sizeof(LARGE_INTEGER) <= sizeof(uint64_t));


	// Get the state we need
	HANDLE  theFile = static_cast<HANDLE>(mHandle);
	NStatus theErr  = NStatus::NoErr;

	LARGE_INTEGER theOffset;
	theOffset.QuadPart = thePosition;


	// Seek into the file
	if (!SetFilePointerEx(theFile, theOffset, nullptr, FILE_BEGIN))
	{
		theErr = NStatus::Param;
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::FileRead : Read from the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileRead(uint64_t theSize, void* thePtr, uint64_t& numRead)
{


	// Validate our parameters
	NN_ASSERT(theSize <= kNInt32Max);



	// Get the state we need
	HANDLE  theFile   = static_cast<HANDLE>(mHandle);
	NStatus theErr    = NStatus::NoErr;
	DWORD   bytesRead = 0;



	// Read from the file
	if (!ReadFile(theFile, thePtr, DWORD(theSize), &bytesRead, nullptr))
	{
		theErr = NStatus::Param;
	}

	if (theErr == NStatus::NoErr && bytesRead == 0)
	{
		theErr = NStatus::ExhaustedSrc;
	}

	if (theErr == NStatus::NoErr)
	{
		numRead = bytesRead;
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::FileWrite : Write to the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileWrite(uint64_t theSize, const void* thePtr, uint64_t& numWritten)
{


	// Validate our parameters
	NN_ASSERT(theSize <= kNInt32Max);



	// Get the state we need
	HANDLE  theFile      = static_cast<HANDLE>(mHandle);
	NStatus theErr       = NStatus::NoErr;
	DWORD   bytesWritten = 0;


	// Write to the file
	if (!WriteFile(theFile, thePtr, DWORD(theSize), &bytesWritten, nullptr))
	{
		theErr = NStatus::Param;
	}

	if (theErr == NStatus::NoErr)
	{
		numWritten = bytesWritten;
	}

	if (numWritten != theSize)
	{
		theErr = NStatus::DiskFull;
	}

	return theErr;
}