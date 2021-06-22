/*	NAME:
		NPOSIX.cpp

	DESCRIPTION:
		POSIX glue.

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
#include "NPOSIX.h"


// Nano
#include "NFilePath.h"
#include "NanoTargets.h"


// System
#include <stdlib.h>

#if NN_TARGET_WINDOWS
	#include <io.h>
	#include <direct.h>
#else
	#include <unistd.h>
	#include <sys/param.h>
#endif





//=============================================================================
//		NPOSIX::gmtime : Convert a time_t to a struct tm in GMT (UTC).
//-----------------------------------------------------------------------------
struct tm NPOSIX::gmtime(time_t timeUnix)
{
	// Convert the time
	//
	// Windows has _s, rather than _r, and reverses the parameter order.
	struct tm gmTime
	{
	};

#if NN_TARGET_WINDOWS
	int sysErr = gmtime_s(&gmTime, &timeUnix);
	NN_EXPECT(sysErr == 0, "Unable to convert pre-1970 time_t");
#else
	gmtime_r(&timeUnix, &gmTime);
#endif

	return gmTime;
}





//=============================================================================
//		NPOSIX::localtime : Convert a time_t to a struct tm in local time.
//-----------------------------------------------------------------------------
struct tm NPOSIX::localtime(time_t timeUnix)
{
	// Convert the time
	//
	// Windows has _s, rather than _r, and reverses the parameter order.
	struct tm localTime
	{
	};

#if NN_TARGET_WINDOWS
	int sysErr = localtime_s(&localTime, &timeUnix);
	NN_EXPECT(sysErr == 0, "Unable to convert pre-1970 time_t");
#else
	localtime_r(&timeUnix, &localTime);
#endif

	return localTime;
}





//=============================================================================
//		NPOSIX::getcwd : Get the current directory.
//-----------------------------------------------------------------------------
NFilePath NPOSIX::getcwd()
{


	// Get the directory
#if NN_TARGET_WINDOWS
	utf16_t thePath[_MAX_PATH]{};
	void*   thePtr = _wgetcwd(reinterpret_cast<wchar_t*>(thePath), _MAX_PATH);
#else
	char  thePath[MAXPATHLEN]{};
	void* thePtr = ::getcwd(thePath, MAXPATHLEN);
#endif



	// Handle failure
	if (thePtr == nullptr)
	{
		thePath[0] = 0x00;
	}

	return NString(thePath);
}





//=============================================================================
//		NPOSIX::chdir : Set the current directory.
//-----------------------------------------------------------------------------
void NPOSIX::chdir(const NFilePath& thePath)
{


	// Set the directory
#if NN_TARGET_WINDOWS
	int sysErr = _wchdir(reinterpret_cast<const wchar_t*>(thePath.GetUTF16()));
	NN_EXPECT_NOT_ERR(sysErr);
#else
	int   sysErr = ::chdir(thePath.GetUTF8());
	NN_EXPECT_NOT_ERR(sysErr);
#endif
}
