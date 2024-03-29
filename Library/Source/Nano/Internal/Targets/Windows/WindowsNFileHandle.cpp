/*	NAME:
		WindowsNFileHandle.cpp

	DESCRIPTION:
		Windows file handle.

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
#include "NFileHandle.h"

// Nano
#include "NCommonWindows.h"
#include "NWindows.h"





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetFileAccess : Get a CreateFile access mode.
//-----------------------------------------------------------------------------
static constexpr DWORD GetFileAccess(NFileAccess theAccess)
{


	// Get the access mode
	switch (theAccess)
	{
		case NFileAccess::ReadWrite:
			return FILE_GENERIC_READ | FILE_GENERIC_WRITE;
			break;

		case NFileAccess::ReadOnly:
			return FILE_GENERIC_READ;
			break;

		case NFileAccess::WriteOnly:
			return FILE_GENERIC_WRITE;
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		GetFileCreation : Get a CreateFile creation mode.
//-----------------------------------------------------------------------------
static constexpr DWORD GetFileCreation(NFileAccess theAccess)
{


	// Get the creation mode
	switch (theAccess)
	{
		case NFileAccess::ReadWrite:
		case NFileAccess::WriteOnly:
			return OPEN_ALWAYS;
			break;

		case NFileAccess::ReadOnly:
			return OPEN_EXISTING;
			break;
	}

	NN_UNREACHABLE();
}





//=============================================================================
//		GetFileUsage : Get CreateFile attributes.
//-----------------------------------------------------------------------------
static constexpr DWORD GetFileAttributes(NFileUsageFlags theFlags)
{


	// Get the flags
	DWORD theAttributes = FILE_ATTRIBUTE_NORMAL;

	if (theFlags != 0)
	{
		theAttributes = 0;

		if (theFlags & NFileUsage::NoCache)
		{
			theAttributes |= FILE_FLAG_NO_BUFFERING;
		}

		if (theFlags & NFileUsage::ReadEarly)
		{
			// Not supported
		}

		if (theFlags & NFileUsage::ReadAhead)
		{
			theAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
		}
		else
		{
			theAttributes |= FILE_FLAG_RANDOM_ACCESS;
		}
	}

	return theAttributes;
}





//=============================================================================
//		GetFileMove : Get a SetFilePointerEx move value.
//-----------------------------------------------------------------------------
static constexpr DWORD GetFileMove(NFileOffset relativeTo)
{


	// Get the permission
	switch (relativeTo)
	{
		case NFileOffset::FromStart:
			return FILE_BEGIN;
			break;

		case NFileOffset::FromPosition:
			return FILE_CURRENT;
			break;

		case NFileOffset::FromEnd:
			return FILE_END;
			break;
	}

	return SEEK_END;
}





#pragma mark NFileHandle
//=============================================================================
//		NFileHandle::FileOpen : Open the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileOpen(const NFilePath& thePath, NFileAccess theAccess, NFileFlags theFlags)
{


	// Validate our state
	static_assert(sizeof(NFileHandleRef) >= sizeof(HANDLE));



	// Open the file
	HANDLE hFile = CreateFileW(LPCWSTR(thePath.GetUTF16()),
							   GetFileAccess(theAccess),
							   FILE_SHARE_READ | FILE_SHARE_WRITE,
							   nullptr,
							   GetFileCreation(theAccess),
							   GetFileAttributes(theFlags),
							   nullptr);

	bool wasOK = (hFile != INVALID_HANDLE_VALUE);
	if (wasOK)
	{
		mHandle = NFileHandleRef(hFile);
	}

	return NCommonWindows::StatusLastError(wasOK);
}





//=============================================================================
//		NFileHandle::FileClose : Close the file.
//-----------------------------------------------------------------------------
void NFileHandle::FileClose()
{


	// Get the state we need
	HANDLE hFile = static_cast<HANDLE>(mHandle);



	// Close the file
	BOOL wasOK = CloseHandle(hFile);
	NN_EXPECT(wasOK);
}





//=============================================================================
//		NFileHandle::FileGetPosition : Get the file position.
//-----------------------------------------------------------------------------
uint64_t NFileHandle::FileGetPosition() const
{


	// Validate our state
	static_assert(sizeof(LARGE_INTEGER) == sizeof(uint64_t));



	// Get the state we need
	HANDLE        hFile = static_cast<HANDLE>(mHandle);
	LARGE_INTEGER theOffset{};



	// Get the file position
	BOOL wasOK = SetFilePointerEx(hFile, theOffset, &theOffset, FILE_CURRENT);
	NN_EXPECT(wasOK);

	int64_t thePosition = ToNN(theOffset);
	NN_REQUIRE(thePosition >= 0);

	if (!wasOK)
	{
		thePosition = 0;
	}

	return uint64_t(thePosition);
}





//=============================================================================
//		NFileHandle::FileSetPosition : Set the file position.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileSetPosition(int64_t thePosition, NFileOffset relativeTo)
{


	// Validate our state
	static_assert(sizeof(LARGE_INTEGER) == sizeof(int64_t));



	// Get the state we need
	HANDLE        hFile     = static_cast<HANDLE>(mHandle);
	LARGE_INTEGER theOffset = ToWN(thePosition);



	// Set the file position
	BOOL wasOK = SetFilePointerEx(hFile, theOffset, nullptr, GetFileMove(relativeTo));
	NN_EXPECT(wasOK);

	return NCommonWindows::StatusLastError(wasOK);
}





//=============================================================================
//		NFileHandle::FileSetSize : Set the size.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileSetSize(uint64_t theSize)
{


	// Validate our parameters
	NN_REQUIRE(theSize <= uint64_t(kNInt64Max));


	// Get the state we need
	HANDLE hFile = static_cast<HANDLE>(mHandle);



	// Set the size
	NStatus theErr = FileSetPosition(int64_t(theSize), NFileOffset::FromStart);
	NN_EXPECT_NOT_ERR(theErr);

	if (theErr == NStatus::OK)
	{
		BOOL wasOK = SetEndOfFile(hFile);
		theErr     = NCommonWindows::StatusLastError(wasOK);
		NN_EXPECT_NOT_ERR(theErr);
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::FileRead : Read from the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileRead(uint64_t theSize, void* thePtr, uint64_t& sizeRead)
{


	// Validate our parameters
	NN_REQUIRE(theSize <= kNInt32Max);



	// Get the state we need
	HANDLE  hFile     = static_cast<HANDLE>(mHandle);
	NStatus theErr    = NStatus::OK;
	DWORD   bytesRead = 0;



	// Read from the file
	BOOL wasOK = ReadFile(hFile, thePtr, DWORD(theSize), &bytesRead, nullptr);
	theErr     = NCommonWindows::StatusLastError(wasOK);
	NN_EXPECT_NOT_ERR(theErr);

	if (wasOK)
	{
		sizeRead = bytesRead;
		if (sizeRead == 0)
		{
			theErr = NStatus::ExhaustedSrc;
		}
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::FileWrite : Write to the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileWrite(uint64_t theSize, const void* thePtr, uint64_t& sizeWritten)
{


	// Validate our parameters
	NN_REQUIRE(theSize <= kNInt32Max);



	// Get the state we need
	HANDLE  hFile        = static_cast<HANDLE>(mHandle);
	NStatus theErr       = NStatus::OK;
	DWORD   bytesWritten = 0;


	// Write to the file
	BOOL wasOK = WriteFile(hFile, thePtr, DWORD(theSize), &bytesWritten, nullptr);
	theErr     = NCommonWindows::StatusLastError(wasOK);
	NN_EXPECT_NOT_ERR(theErr);

	if (wasOK)
	{
		sizeWritten = bytesWritten;
		if (sizeWritten != theSize)
		{
			theErr = NStatus::DiskFull;
		}
	}

	return theErr;
}





//=============================================================================
//		NFileHandle::FileFlush : Flush the file.
//-----------------------------------------------------------------------------
NStatus NFileHandle::FileFlush()
{


	// Get the state we need
	HANDLE hFile = static_cast<HANDLE>(mHandle);


	// Flush the file
	BOOL wasOK = FlushFileBuffers(hFile);
	NN_EXPECT(wasOK);

	return NCommonWindows::StatusLastError(wasOK);
}
