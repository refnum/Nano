/*	NAME:
		NSharedWindows.cpp

	DESCRIPTION:
		Windows support.

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
#include "NSharedWindows.h"

// Nano
#include "NTime.h"
#include "NanoConstants.h"





//=============================================================================
//		NSharedWindows::ToUInt64 : Convert two DWORDs to a uint64_t.
//-----------------------------------------------------------------------------
uint64_t NSharedWindows::ToUInt64(DWORD valueHigh, DWORD valueLow)
{
	return (uint64_t(valueHigh) << 32) | uint64_t(valueLow);
}





//=============================================================================
//		NSharedWindows::ToInt64 : Convert a LARGE_INTEGER to an int64_t.
//-----------------------------------------------------------------------------
int64_t NSharedWindows::ToInt64(LARGE_INTEGER theValue)
{
	return theValue.QuadPart;
}





//=============================================================================
//		NSharedWindows::ToLargeInteger : Convert an int64_t to a LARGE_INTEGER.
//-----------------------------------------------------------------------------
LARGE_INTEGER NSharedWindows::ToLargeInteger(int64_t theValue)
{
	LARGE_INTEGER largeInteger{};

	largeInteger.QuadPart = theValue;

	return largeInteger;
}





//=============================================================================
//		NSharedWindows::ToStatus : Convert an HRESULT to an NStatus.
//-----------------------------------------------------------------------------
NStatus NSharedWindows::ToStatus(HRESULT winErr)
{


	// Get the status
	NStatus theErr = NStatus::Internal;

	switch (winErr)
	{
		case HRESULT_FROM_WIN32(ERROR_SUCCESS):
			theErr = NStatus::OK;
			break;

		case HRESULT_FROM_WIN32(ERROR_SHARING_VIOLATION):
			theErr = NStatus::Busy;
			break;

		case HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND):
			theErr = NStatus::NotFound;
			break;

		case HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND):
			theErr = NStatus::NotFound;
			break;

		case HRESULT_FROM_WIN32(ERROR_ENVVAR_NOT_FOUND):
			theErr = NStatus::NotFound;
			break;

		case HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED):
			theErr = NStatus::Permission;
			break;

		case HRESULT_FROM_WIN32(ERROR_HANDLE_EOF):
			theErr = NStatus::ExhaustedSrc;
			break;

		case HRESULT_FROM_WIN32(ERROR_ALREADY_EXISTS):
			theErr = NStatus::Duplicate;
			break;

		case HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE):
			theErr = NStatus::Param;
			break;

		default:
			NN_LOG_UNIMPLEMENTED("Unknown error {}", winErr);
			break;
	}

	return theErr;
}





//=============================================================================
//		NSharedWindows::GetLastError : Get the last error.
//-----------------------------------------------------------------------------
NStatus NSharedWindows::GetLastError()
{


	// Get the error
	return ToStatus(HRESULT_FROM_WIN32(::GetLastError()));
}





//=============================================================================
//		NSharedWindows::ToInterval : Convert to an NInterval.
//-----------------------------------------------------------------------------
NInterval NSharedWindows::ToInterval(const FILETIME& fileTime)
{


	// Convert the value
	uint64_t  hectoNanoSecs = ToUInt64(fileTime.dwHighDateTime, fileTime.dwLowDateTime);
	NInterval theInterval   = NInterval(hectoNanoSecs) * (100.0 * kNTimeNanosecond);

	return theInterval;
}
