/*	NAME:
		NSharedPOSIX.cpp

	DESCRIPTION:
		POSIX support.

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
#include "NSharedPOSIX.h"

// Nano
#include "NDebug.h"
#include "NString.h"
#include "NTimeUtils.h"

// System
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// AT_EACCESS
//
// Android does not support AT_EACCESS.
#if NN_TARGET_ANDROID
static constexpr int NN_AT_EACCESS                          = 0;
#else
static constexpr int NN_AT_EACCESS                          = AT_EACCESS;
#endif





//=============================================================================
//		NSharedPOSIX::gettimeofday : Get the time of day.
//-----------------------------------------------------------------------------
NTime NSharedPOSIX::gettimeofday()
{


	// Get the time of day
	struct timeval timeVal = {};

	int sysErr = ::gettimeofday(&timeVal, nullptr);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeVal, 0x00, sizeof(timeVal));
	}



	// Get the time
	return NTime(ToInterval(timeVal), kNanoEpochFrom1970);
}





//=============================================================================
//		NSharedPOSIX::clock_gettime : Get a clock time.
//-----------------------------------------------------------------------------
NInterval NSharedPOSIX::clock_gettime(clockid_t theID)
{


	// Get the clock time
	struct timespec timeSpec = {};

	int sysErr = ::clock_gettime(theID, &timeSpec);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeSpec, 0x00, sizeof(timeSpec));
	}



	// Get the time
	return NTimeUtils::ToInterval(timeSpec);
}





//=============================================================================
//		NSharedPOSIX::GetFileStateAccess : Get file state with access().
//-----------------------------------------------------------------------------
void NSharedPOSIX::GetFileStateAccess(const NString&  thePath,
									  NFileInfoFlags  theFlag,
									  NFileInfoState& theState)
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
			theMode = R_OK;
			break;

		case kNFileInfoCanWrite:
			theMode = W_OK;
			break;

		case kNFileInfoCanExecute:
			theMode = X_OK;
			break;

		default:
			NN_LOG_ERROR("Unknown file info mode: {}", theFlag);
			break;
	}



	// Get the state
	if (theMode != -1)
	{
		int sysErr = faccessat(0, thePath.GetUTF8(), theMode, NN_AT_EACCESS);
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





//=============================================================================
//		NSharedPOSIX::ToTimeval : Convert to a timeval.
//-----------------------------------------------------------------------------
struct timeval NSharedPOSIX::ToTimeval(NInterval theInterval)
{
	// Convert the value
	NInterval timeSecs = floor(theInterval);
	NInterval timeFrac = theInterval - timeSecs;

	return {time_t(timeSecs), suseconds_t(timeFrac / kNTimeMicrosecond)};
}





//=============================================================================
//		NSharedPOSIX::ToInterval : Convert to an NInterval.
//-----------------------------------------------------------------------------
NInterval NSharedPOSIX::ToInterval(const struct timeval& timeVal)
{


	// Convert the value
	NInterval timeSecs = NInterval(timeVal.tv_sec);
	NInterval timeFrac = NInterval(timeVal.tv_usec) * kNTimeMicrosecond;

	return timeSecs + timeFrac;
}
