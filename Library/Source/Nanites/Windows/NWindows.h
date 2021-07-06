/*	NAME:
		NWindows.h

	DESCRIPTION:
		Windows support.

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
#ifndef NWINDOWS_H
#define NWINDOWS_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NPoint.h"
#include "NRectangle.h"
#include "NSize.h"
#include "NString.h"

// System
#include <Windows.h>
#include <objidl.h>





//=============================================================================
//		Inline functions
//-----------------------------------------------------------------------------
// Nano to Windows
POINT          ToWN(const NPoint& thePoint);
SIZE           ToWN(const NSize& theSize);
RECT           ToWN(const NRectangle& theRect);
const TCHAR*   ToWN(const NString& theString);
FILETIME       ToWN(NInterval theTime);
LARGE_INTEGER  ToWN(const int64_t& theValue);
ULARGE_INTEGER ToWN(const uint64_t& theValue);
void           ToWN(uint64_t theValue, DWORD& valueHigh, DWORD& valueLow);


// Windows to Nano
NPoint     ToNN(const POINT& thePoint);
NPoint     ToNN(const POINTS& thePoint);
NPoint     ToNN(const POINTL& thePoint);
NSize      ToNN(const SIZE& thePoint);
NRectangle ToNN(const RECT& theRect);
NRectangle ToNN(const RECTL& theRect);
NString    ToNN(const TCHAR* theString);
NInterval  ToNN(const FILETIME& theTime);
int64_t    ToNN(const LARGE_INTEGER& theValue);
uint64_t   ToNN(const ULARGE_INTEGER& theValue);
uint64_t   ToNN(DWORD valueHigh, DWORD valueLow);





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
// Win32
#define WNSafeCloseHandle(_handle)                          \
	do                                                      \
	{                                                       \
		BOOL _wasOK;                                        \
															\
		if ((_handle) != INVALID_HANDLE_VALUE)              \
		{                                                   \
			_wasOK = CloseHandle((HANDLE) (_handle));       \
			NN_ASSERT(_wasOK);                              \
															\
			(_handle) = INVALID_HANDLE_VALUE;               \
		}                                                   \
	} while (false)



// COM
#define WNSafeRelease(_object)                              \
	do                                                      \
	{                                                       \
		if ((_object) != NULL)                              \
		{                                                   \
			(_object)->Release();                           \
			(_object) = NULL;                               \
		}                                                   \
	} while (false)


// Nano
#define NN_REQUIRE_SUCCESS(_winErr)                         NN_REQUIRE(SUCCEEDED((_winErr)))
#define NN_EXPECT_SUCCESS(_winErr)                          NN_EXPECT(SUCCEEDED((_winErr)))

#define REQUIRE_SUCCESS(_winErr)                            REQUIRE(SUCCEEDED((_winErr)))

#if NN_ENABLE_LOGGING
	#define NN_LOG_LASTERR()                                                                \
		do                                                                                  \
		{                                                                                   \
			LPTSTR theStr = nullptr;                                                        \
			DWORD  theErr = GetLastError();                                                 \
																							\
			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,  \
							  nullptr,                                                      \
							  theErr,                                                       \
							  0,                                                            \
							  (LPTSTR) &theStr,                                             \
							  0,                                                            \
							  nullptr))                                                     \
			{                                                                               \
				NN_LOG("Windows Error: {}", ToNN(theStr));                                  \
				LocalFree(theStr);                                                          \
			}                                                                               \
			else                                                                            \
			{                                                                               \
				NN_LOG("Windows Error: {}", theErr);                                        \
			}                                                                               \
		} while (0)

#else
	#define NN_LOG_LASTERR()                                \
		do                                                  \
		{                                                   \
		} while (0)
#endif





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NScopedCOM
{
public:
										NScopedCOM(DWORD coInit = COINIT_MULTITHREADED);
									   ~NScopedCOM();

										NScopedCOM(const NScopedCOM& otherCOM) = delete;
	NScopedCOM&                         operator=( const NScopedCOM& otherCOM) = delete;

										NScopedCOM(NScopedCOM&& otherCOM) = delete;
	NScopedCOM&                         operator=( NScopedCOM&& otherCOM) = delete;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NWindows.inl"


#endif // NWINDOWS_H
