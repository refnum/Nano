/*	NAME:
		NSharedLinux.cpp

	DESCRIPTION:
		Linux support.

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
#include "NSharedLinux.h"

// Nano
#include "NDebug.h"
#include "NSharedPOSIX.h"
#include "NTimeUtils.h"

// System
#include <semaphore.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Time
static constexpr uint64_t kNanosecondsPerSecond             = 1000000000ULL;

#if defined(CLOCK_MONOTONIC_RAW)
static constexpr clockid_t kSystemClockID                   = CLOCK_MONOTONIC_RAW;
#else
static constexpr clockid_t kSystemClockID                   = CLOCK_MONTONIC;
#endif


// File info
static constexpr NFileInfoFlags kNFileInfoMaskStat          = kNFileInfoExists | kNFileInfoIsFile |
													 kNFileInfoIsDirectory |
													 kNFileInfoModifiedTime | kNFileInfoFileSize;





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetFileStateStat : Get file state with stat().
//-----------------------------------------------------------------------------
static bool GetFileStateStat(const NString& thePath, NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(!thePath.IsEmpty());



	// Get the state we need
	struct stat theInfo;

	int  sysErr = stat(thePath.GetUTF8(), &theInfo);
	bool wasOK  = (sysErr == 0);


	// Update the state
	if (wasOK)
	{
		theState.theFlags |= kNFileInfoExists;

		if (S_ISREG(theInfo.st_mode))
		{
			theState.theFlags |= kNFileInfoIsFile;
			theState.fileSize = uint64_t(theInfo.st_size);
		}
		else
		{
			theState.theFlags &= ~kNFileInfoIsFile;
			theState.fileSize = 0;
		}

		if (S_ISDIR(theInfo.st_mode))
		{
			theState.theFlags |= kNFileInfoIsDirectory;
		}
		else
		{
			theState.theFlags &= ~kNFileInfoIsDirectory;
		}

		theState.modifiedTime = NTime(NTimeUtils::ToInterval(theInfo.st_mtim), kNanoEpochFrom1970);
	}

	return wasOK;
}





//=============================================================================
//		GetFileStateCreationTime : Get the file creation time.
//-----------------------------------------------------------------------------
static void GetFileStateCreationTime(const NString& thePath, NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(!thePath.IsEmpty());



	// Get the state we need
	//
	// Android does not provide statx so we always use the modified time
#if NN_TARGET_ANDROID
	NN_REQUIRE(theState.modifiedTime != 0.0);
	theState.creationTime = theState.modifiedTime;

#else
	struct statx theInfo;

	int  sysErr = SYS_statx(0, thePath.GetUTF8(), AT_STATX_SYNC_AS_STAT, STATX_BTIME, &theInfo);
	bool wasOK  = (sysErr == 0);



	// Update the state
	if (wasOK)
	{
		NInterval timeSecs = NInterval(timeStamp.tv_sec);
		NInterval timeFrac = NInterval(timeStamp.tv_nsec) * kNTimeNanosecond;

		theState.creationTime = NTime(timeSecs + timeFrac, kNanoEpochFrom1970);
		theState.theFlags |= kNFileInfoCreationTime;
	}
	else
	{
		theState.theFlags &= ~kNFileInfoCreationTime;
	}
#endif
}





#pragma mark NSharedLinux
//=============================================================================
//		NSharedLinux::GetClockTicks : Get the clock ticks.
//-----------------------------------------------------------------------------
uint64_t NSharedLinux::GetClockTicks()
{


	// Get the clock ticks
	struct timespec timeSpec = {};

	int sysErr = ::clock_gettime(kSystemClockID, &timeSpec);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeSpec, 0x00, sizeof(timeSpec));
	}

	return (uint64_t(timeSpec.tv_sec) * kNanosecondsPerSecond) + uint64_t(timeSpec.tv_nsec);
}





//=============================================================================
//		NSharedLinux::GetClockFrequency : Get the clock frequency.
//-----------------------------------------------------------------------------
uint64_t NSharedLinux::GetClockFrequency()
{


	// Get the clock frequency
	return kNanosecondsPerSecond;
}





//=============================================================================
//		NSharedLinux::GetFileState : Get file state.
//-----------------------------------------------------------------------------
bool NSharedLinux::GetFileState(const NString&  thePath,
								NFileInfoFlags  theFlags,
								NFileInfoFlags& validState,
								NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(!thePath.IsEmpty());
	NN_REQUIRE(theFlags != kNFileInfoNone);


	// Fetch with stat
	bool wasOK = true;

	if (wasOK && (theFlags & kNFileInfoMaskStat) != 0)
	{
		wasOK = GetFileStateStat(thePath, theState);
		if (wasOK)
		{
			validState |= kNFileInfoMaskStat;
		}
	}



	// Fetch with statx
	if (wasOK)
	{
		if ((theFlags & kNFileInfoCreationTime) != 0)
		{
			GetFileStateCreationTime(thePath, theState);
			validState |= kNFileInfoCreationTime;
		}
	}



	// Fetch with access
	if (wasOK)
	{
		if ((theFlags & kNFileInfoCanRead) != 0)
		{
			NSharedPOSIX::GetFileStateAccess(thePath, kNFileInfoCanRead, theState);
			validState |= kNFileInfoCanRead;
		}

		if ((theFlags & kNFileInfoCanWrite) != 0)
		{
			NSharedPOSIX::GetFileStateAccess(thePath, kNFileInfoCanWrite, theState);
			validState |= kNFileInfoCanWrite;
		}

		if ((theFlags & kNFileInfoCanExecute) != 0)
		{
			NSharedPOSIX::GetFileStateAccess(thePath, kNFileInfoCanExecute, theState);
			validState |= kNFileInfoCanExecute;
		}
	}

	return wasOK;
}





//=============================================================================
//		NSharedLinux::SemaphoreCreate : Create a semaphore.
//-----------------------------------------------------------------------------
NSemaphoreRef NSharedLinux::SemaphoreCreate(size_t theValue)
{


	// Validate our parameters and state
	NN_REQUIRE(theValue <= size_t(UINT_MAX));

	static_assert(sizeof(NSemaphoreRef) >= sizeof(sem_t*));



	// Create the semaphore
	sem_t* semRef = static_cast<sem_t*>(malloc(sizeof(sem_t)));
	NN_REQUIRE_NOT_NULL(semRef);

	int sysErr = sem_init(semRef, 0, static_cast<unsigned int>(theValue));
	NN_REQUIRE_NOT_ERR(sysErr);

	return NSemaphoreRef(semRef);
}





//=============================================================================
//		NSharedLinux::SemaphoreDestroy : Destroy a semaphore.
//-----------------------------------------------------------------------------
void NSharedLinux::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{


	// Destroy the semaphore
	sem_t* semRef = static_cast<sem_t*>(theSemaphore);
	int    sysErr = sem_destroy(semRef);

	NN_REQUIRE_NOT_ERR(sysErr);

	free(semRef);
}





//=============================================================================
//		NSharedLinux::SemaphoreWait : Wait for a semaphore.
//-----------------------------------------------------------------------------
bool NSharedLinux::SemaphoreWait(NSemaphoreRef theSemaphore, NInterval waitFor)
{


	// Get the state we need
	sem_t* semRef = static_cast<sem_t*>(theSemaphore);
	int    sysErr = 0;



	// Wait for the semaphore
	if (waitFor == kNTimeForever)
	{
		sysErr = sem_wait(semRef);
	}
	else
	{
		struct timespec semWait = NTimeUtils::ToTimespec(waitFor);
		sysErr                  = sem_timedwait(semRef, &semWait);
	}

	return sysErr == 0;
}





//=============================================================================
//		NSharedLinux::SemaphoreSignal : Signal a semaphore.
//-----------------------------------------------------------------------------
void NSharedLinux::SemaphoreSignal(NSemaphoreRef theSemaphore)
{


	// Signal the semaphore
	sem_t* semRef = static_cast<sem_t*>(theSemaphore);
	int    sysErr = sem_post(semRef);

	NN_EXPECT_NOT_ERR(sysErr);
}
