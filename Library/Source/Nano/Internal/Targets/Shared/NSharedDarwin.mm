/*	NAME:
		NSharedDarwin.mm

	DESCRIPTION:
		Darwin support.

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
#include "NSharedDarwin.h"

// Nano
#include "NDebug.h"
#include "NSharedPOSIX.h"
#include "NString.h"
#include "NTimeUtils.h"

// System
#include <CoreFoundation/CoreFoundation.h>
#include <Foundation/Foundation.h>
#include <dispatch/semaphore.h>
#include <fcntl.h>
#include <mach/mach_time.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <unistd.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr NFileInfoFlags kNFileInfoMaskStat =
	kNFileInfoExists | kNFileInfoIsFile | kNFileInfoIsDirectory | kNFileInfoCreationTime |
	kNFileInfoModifiedTime | kNFileInfoFileSize;





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//      GetBootTime : Get the boot time.
//-----------------------------------------------------------------------------
static NTime GetBootTime()
{


	// Get the boot time
	struct timeval timeVal     = {};
	size_t         theSize     = sizeof(timeVal);
	int            theNames[2] = {CTL_KERN, KERN_BOOTTIME};

	int sysErr = sysctl(theNames, 2, &timeVal, &theSize, nullptr, 0);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeVal, 0x00, sizeof(timeVal));
	}



	// Convert the time
	return NTime(NSharedPOSIX::ToInterval(timeVal), kNanoEpochFrom2001);
}





//=============================================================================
//		GetMachClockFrequency : Get the Mach clock frequency.
//-----------------------------------------------------------------------------
static uint64_t GetMachClockFrequency()
{


	// Get the clock frequency
	mach_timebase_info_data_t timebaseInfo{};

	kern_return_t sysErr = mach_timebase_info(&timebaseInfo);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != KERN_SUCCESS)
	{
		return 0;
	}

	float64_t nanoPerTick = float64_t(timebaseInfo.numer) / float64_t(timebaseInfo.denom);
	uint64_t  ticksPerSec = uint64_t(float64_t(NSEC_PER_SEC) / nanoPerTick);

	return ticksPerSec;
}





//=============================================================================
//		GetFileStateStat : Get file state with stat().
//-----------------------------------------------------------------------------
static bool GetFileStateStat(const NFilePath& thePath, NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());

	// Get the state we need
	struct stat theInfo
	{
	};

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

		theState.creationTime =
			NTime(NTimeUtils::ToInterval(theInfo.st_birthtimespec), kNanoEpochFrom1970);

		theState.modifiedTime =
			NTime(NTimeUtils::ToInterval(theInfo.st_mtimespec), kNanoEpochFrom1970);
	}

	return wasOK;
}





//=============================================================================
//		GetNSSearchPath : Get an NSSearchPath location.
//-----------------------------------------------------------------------------
static NString GetNSSearchPath(NSSearchPathDomainMask theDomain, NSSearchPathDirectory theDirectory)
{


	// Get the location
	NString thePath;

	@autoreleasepool
	{
		NSArray<NSString*>* nsPaths =
			NSSearchPathForDirectoriesInDomains(theDirectory, theDomain, YES);
		if (nsPaths != nil && nsPaths.count != 0)
		{
			NSString* nsPath = [nsPaths objectAtIndex:0];
			if (nsPath != nil)
			{
				thePath = NString(nsPath.UTF8String);
			}
		}
	}

	return thePath;
}





#pragma mark NSharedDarwin
//=============================================================================
//		NSharedDarwin::GetTime : Get the time.
//-----------------------------------------------------------------------------
NTime NSharedDarwin::GetTime()
{


	// Get the time
	return NTime(CFAbsoluteTimeGetCurrent(), kNanoEpochFrom2001);
}





//=============================================================================
//		NSharedDarwin::GetUpTime : Get the time since boot.
//-----------------------------------------------------------------------------
NInterval NSharedDarwin::GetUpTime()
{


	// Get the time since boot
	static NTime sBootTime = GetBootTime();

	return GetTime() - sBootTime;
}





//=============================================================================
//		NSharedDarwin::GetClockTicks : Get the clock ticks.
//-----------------------------------------------------------------------------
uint64_t NSharedDarwin::GetClockTicks()
{


	// Get the clock ticks
	return uint64_t(mach_absolute_time());
}





//=============================================================================
//		NSharedDarwin::GetClockFrequency : Get the clock frequency.
//-----------------------------------------------------------------------------
uint64_t NSharedDarwin::GetClockFrequency()
{


	// Get the clock frequency
	static uint64_t sClockFrequency = GetMachClockFrequency();

	return sClockFrequency;
}





//=============================================================================
//		NSharedDarwin::GetFileState : Get file state.
//-----------------------------------------------------------------------------
bool NSharedDarwin::GetFileState(const NFilePath& thePath,
								 NFileInfoFlags   theFlags,
								 NFileInfoFlags&  validState,
								 NFileInfoState&  theState)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());
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
//		NSharedDarwin::FileRename : Atomically rename a file.
//-----------------------------------------------------------------------------
NStatus NSharedDarwin::FileRename(const NString& oldPath, const NString& newPath)
{


	// Rename the file
	int sysErr = renamex_np(oldPath.GetUTF8(), newPath.GetUTF8(), RENAME_EXCL);
	NN_EXPECT_NOT_ERR(sysErr);

	return NSharedPOSIX::GetErrno(sysErr);
}





//=============================================================================
//		NSharedDarwin::FileExchange : Atomically exchange files.
//-----------------------------------------------------------------------------
NStatus NSharedDarwin::FileExchange(const NString& oldPath, const NString& newPath)
{


	// Exchange the files
	int sysErr = renamex_np(oldPath.GetUTF8(), newPath.GetUTF8(), RENAME_SWAP);
	NN_EXPECT_NOT_ERR(sysErr);

	return NSharedPOSIX::GetErrno(sysErr);
}





//=============================================================================
//		NSharedDarwin::GetLocation : Get a location.
//-----------------------------------------------------------------------------
NStatus NSharedDarwin::GetLocation(NFileLocation theLocation, NString& thePath)
{


	// Get the location
	NStatus theErr = NStatus::OK;

	switch (theLocation)
	{
		case NFileLocation::AppCaches:
			thePath = GetNSSearchPath(NSUserDomainMask, NSCachesDirectory);
			break;

		case NFileLocation::AppSupport:
			thePath = GetNSSearchPath(NSUserDomainMask, NSApplicationSupportDirectory);
			break;

		case NFileLocation::AppTemporaries:
			thePath = NString(NSTemporaryDirectory().UTF8String);
			break;

		case NFileLocation::SharedSupport:
			thePath = GetNSSearchPath(NSLocalDomainMask, NSApplicationSupportDirectory);
			break;

		case NFileLocation::UserDesktop:
			thePath = GetNSSearchPath(NSUserDomainMask, NSDesktopDirectory);
			break;

		case NFileLocation::UserDocuments:
			thePath = GetNSSearchPath(NSUserDomainMask, NSDocumentDirectory);
			break;

		case NFileLocation::UserDownloads:
			thePath = GetNSSearchPath(NSUserDomainMask, NSDownloadsDirectory);
			break;

		case NFileLocation::UserHome:
			thePath = NString(NSHomeDirectory().UTF8String);
			break;

		case NFileLocation::UserLogs:
			thePath = GetNSSearchPath(NSUserDomainMask, NSLibraryDirectory);
			if (!thePath.IsEmpty())
			{
				thePath += "/Logs";
			}
			break;

		case NFileLocation::UserPictures:
			thePath = GetNSSearchPath(NSUserDomainMask, NSPicturesDirectory);
			break;

		case NFileLocation::UserPreferences:
			thePath = GetNSSearchPath(NSUserDomainMask, NSLibraryDirectory);
			if (!thePath.IsEmpty())
			{
				thePath += "/Preferences";
			}
			break;
	}

	return theErr;
}





//=============================================================================
//		NSharedDarwin::SemaphoreCreate : Create a semaphore.
//-----------------------------------------------------------------------------
NSemaphoreRef NSharedDarwin::SemaphoreCreate(size_t theValue)
{


	// Validate our parameters and state
	NN_REQUIRE(theValue <= size_t(LONG_MAX));

	static_assert(sizeof(NSemaphoreRef) >= sizeof(dispatch_semaphore_t));



	// Create the semaphore
	//
	// A dispatch semaphore can avoid a trip to the kernel and so
	// performs substantially better than a Mach semaphore.
	//
	//	A signal/wait loop that runs 10,000,000 times takes:
	//
	//		Mach semaphore:			4.7s
	//		Dispatch semaphore:		0.22s
	//
	// Timings from a 2.2Ghz Core i7.
	dispatch_semaphore_t dispatchSem = dispatch_semaphore_create(long(theValue));
	NN_REQUIRE_NOT_NULL(dispatchSem);

	return NSemaphoreRef(dispatchSem);
}





//=============================================================================
//		NSharedDarwin::SemaphoreDestroy : Destroy a semaphore.
//-----------------------------------------------------------------------------
void NSharedDarwin::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{


	// Destroy the semaphore
	dispatch_semaphore_t dispatchSem = dispatch_semaphore_t(theSemaphore);

	dispatch_release(dispatchSem);
}





//=============================================================================
//		NSharedDarwin::SemaphoreWait : Wait for a semaphore.
//-----------------------------------------------------------------------------
bool NSharedDarwin::SemaphoreWait(NSemaphoreRef theSemaphore, NInterval waitFor)
{


	// Get the state we need
	dispatch_semaphore_t dispatchSem  = dispatch_semaphore_t(theSemaphore);
	dispatch_time_t      dispatchWait = DISPATCH_TIME_FOREVER;

	if (waitFor != kNTimeForever)
	{
		dispatchWait = dispatch_time(DISPATCH_TIME_NOW, int64_t(waitFor / kNTimeNanosecond));
	}



	// Wait for the semaphore
	long sysErr = dispatch_semaphore_wait(dispatchSem, dispatchWait);
	NN_EXPECT_NOT_ERR(sysErr);

	return sysErr == 0;
}





//=============================================================================
//		NSharedDarwin::SemaphoreSignal : Signal a semaphore.
//-----------------------------------------------------------------------------
void NSharedDarwin::SemaphoreSignal(NSemaphoreRef theSemaphore)
{


	// Signal the semaphore
	dispatch_semaphore_t dispatchSem = dispatch_semaphore_t(theSemaphore);

	(void) dispatch_semaphore_signal(dispatchSem);
}
