/*	NAME:
		NCommonLinux.cpp

	DESCRIPTION:
		Linux support.

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
#include "NCommonLinux.h"

// Nano
#include "NCommonPOSIX.h"
#include "NDebug.h"
#include "NDictionary.h"
#include "NFormat.h"
#include "NMachine.h"
#include "NNumber.h"
#include "NPropertyList.h"
#include "NSystem.h"
#include "NTimeUtils.h"
#include "NanoTargets.h"

// System
#include <fcntl.h>
#include <linux/fs.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

#if !defined(__statx_timestamp_defined)
	#include <linux/stat.h>
#endif // __statx_timestamp_defined





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Misc
static constexpr size_t kNBufferSize                        = 2 * kNKibibyte;



// Time
static constexpr uint64_t kNanosecondsPerSecond             = 1000000000ULL;

#if defined(CLOCK_MONOTONIC_RAW)
static constexpr clockid_t kSystemClockID                   = CLOCK_MONOTONIC_RAW;
#else
static constexpr clockid_t kSystemClockID                   = CLOCK_MONTONIC;
#endif


// File
static constexpr NFileInfoFlags kNFileInfoMaskStat          = kNFileInfoExists | kNFileInfoIsFile |
													 kNFileInfoIsDirectory |
													 kNFileInfoModifiedTime | kNFileInfoFileSize;

constexpr NFileInfoFlags kNFileInfoMaskStatX                = kNFileInfoCreationTime | kNFileInfoModifiedTime;


// fcntl.h
//
// We require both open (fcntl.h) and AT_STATX_SYNC_AS_STAT (linux/fcntl.h)
// but on some distributions both headers cannot be included simultaneously.
//
// As AT_STATX_SYNC_AS_STAT is simply 0 we provide our own.
#if defined(AT_STATX_SYNC_AS_STAT)
static_assert(AT_STATX_SYNC_AS_STAT == 0x0000);
#else
	#define AT_STATX_SYNC_AS_STAT                           0x0000
#endif



#if NN_TARGET_LINUX && defined(SYS_statx)
//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		ToTime : Convert to an NTime.
//-----------------------------------------------------------------------------
static NTime ToTime(const struct statx_timestamp& timeStamp)
{


	// Convert the value
	NInterval timeSecs = NInterval(timeStamp.tv_sec);
	NInterval timeFrac = NInterval(timeStamp.tv_nsec) * kNTimeNanosecond;

	return NTime(timeSecs + timeFrac, kNanoEpochFrom1970);
}
#endif





//=============================================================================
//		GetBundlePlatform : Get the bundle platform name.
//-----------------------------------------------------------------------------
static NString GetBundlePlatform()
{


	// Get the name
	NString theName = NSystem::GetName(NOSName::Platform);
	NN_REQUIRE(theName == "Android" || theName == "Linux", "Unexpected system '{}'", theName);

	return theName;
}





//=============================================================================
//		GetFileStateStat : Get file state with stat().
//-----------------------------------------------------------------------------
static bool GetFileStateStat(const NFilePath& thePath, NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



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
			theState.theFlags &= NFileInfoFlags(~kNFileInfoIsFile);
			theState.fileSize = 0;
		}

		if (S_ISDIR(theInfo.st_mode))
		{
			theState.theFlags |= kNFileInfoIsDirectory;
		}
		else
		{
			theState.theFlags &= NFileInfoFlags(~kNFileInfoIsDirectory);
		}

		theState.modifiedTime = NTime(NTimeUtils::ToInterval(theInfo.st_mtim), kNanoEpochFrom1970);
	}

	return wasOK;
}





//=============================================================================
//		GetFileStateStatX : Get file state with statx().
//-----------------------------------------------------------------------------
static bool GetFileStateStatX(const NFilePath& thePath, NFileInfoState& theState)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());



	// Get the state we need
	int  sysErr = 0;
	bool wasOK  = false;



#if NN_TARGET_LINUX && defined(SYS_statx)
	// Linux may have statx to query the birth time
	//
	// We can't avoid a potentially second call to stat so we fill both
	// birth and modification times.
	struct statx theInfo;

	sysErr = int(syscall(__NR_statx,
						 0,
						 thePath.GetUTF8(),
						 AT_STATX_SYNC_AS_STAT,
						 STATX_MTIME | STATX_BTIME,
						 &theInfo));

	wasOK = (sysErr == 0);



	// Update the state
	if (wasOK)
	{
		// Fix up broken creation times
		//
		// Not every filesystem supports creation times.
		//
		// Those that do can also report a creation time (the time this file
		// was created) later than the modification time (the time the content
		// of a file that this file was copied from was last modified).
		//
		// Creation is by definition earlier than modification so we use the
		// modification time if no creation time was available, or if the file
		// claims to have been modified before it was created.
		if (theInfo.stx_mask & STATX_MTIME)
		{
			theState.modifiedTime = ToTime(theInfo.stx_mtime);
			theState.creationTime = theState.modifiedTime;
		}

		if (theInfo.stx_mask & STATX_BTIME)
		{
			theState.modifiedTime = ToTime(theInfo.stx_btime);
			if (theState.creationTime > theState.modifiedTime)
			{
				theState.creationTime = theState.modifiedTime;
			}
		}
	}


#else
	// Android and older Linux does not provide statx
	//
	// We assume files are created when they were last modified, so
	// we can skip a stat if the modification time is already known.
	if ((theState.theFlags & kNFileInfoModifiedTime) == 0)
	{
		struct stat theInfo;

		sysErr = stat(thePath.GetUTF8(), &theInfo);
		wasOK  = (sysErr == 0);

		theState.creationTime = NTime(NTimeUtils::ToInterval(theInfo.st_mtim), kNanoEpochFrom1970);
		theState.modifiedTime = theState.creationTime;
	}
#endif

	return wasOK;
}





#pragma mark NCommonLinux
//=============================================================================
//		NCommonLinux::GetProcFile : Get a /proc file.
//-----------------------------------------------------------------------------
NString NCommonLinux::GetProcFile(const NFilePath& thePath)
{


	// Read the file
	NString theText;

	int theFD = open(thePath.GetUTF8(), O_RDONLY);

	if (theFD != -1)
	{
		uint8_t theBuffer[kNBufferSize];
		ssize_t numRead;

		do
		{
			numRead = read(theFD, theBuffer, kNBufferSize);
			if (numRead > 0)
			{
				theText += NString(NStringEncoding::UTF8, size_t(numRead), theBuffer);
			}
		} while (numRead > 0);

		close(theFD);
	}

	return theText;
}





//=============================================================================
//		NCommonLinux::BundleGet : Get the bundle for an ID.
//-----------------------------------------------------------------------------
NFile NCommonLinux::BundleGet(const NString& bundleID)
{


	// Locate the executable
	NFile theFile;

	if (bundleID.IsEmpty())
	{
		char    theBuffer[PATH_MAX];
		ssize_t theLen = readlink("/proc/self/exe", theBuffer, sizeof(theBuffer) - 1);

		if (theLen > 0)
		{
			theFile = NFile(NString(NStringEncoding::UTF8, size_t(theLen), theBuffer));
		}
	}
	else
	{
		NN_LOG_WARNING("Unable to locate bundle {}", bundleID);
	}



	// Locate the bundle
	//
	// If the executable is within a bundle then we return the root bundle
	// folder, otherwise we return the directory containing the executable.
	theFile = theFile.GetParent();

	if (theFile.GetName() == GetBundlePlatform())
	{
		NFile theParent = theFile.GetParent();
		if (theParent.GetName() == "Contents")
		{
			theFile = theParent.GetParent();
		}
	}

	return theFile;
}





//=============================================================================
//		NCommonLinux::BundleGetResources : Get the resources directory for the bundle.
//-----------------------------------------------------------------------------
NFile NCommonLinux::BundleGetResources(const NFile& theBundle)
{


	// Get the resources
	return theBundle.GetChild("Contents/Resources");
}





//=============================================================================
//		NCommonLinux::BundleGetInfoDictionary : Get the Info.plist dictionary for a bundle.
//-----------------------------------------------------------------------------
NDictionary NCommonLinux::BundleGetInfoDictionary(const NFile& theBundle)
{


	// Get the info
	NFile theFile = theBundle.GetChild("Contents/Info.plist");

	if (theFile.IsFile())
	{
		return NPropertyList::Load(theFile);
	}

	return {};
}





//=============================================================================
//		NCommonLinux::BundleGetExecutable : Get an executable from a bundle.
//-----------------------------------------------------------------------------
NFile NCommonLinux::BundleGetExecutable(const NFile& theBundle, const NString& theName)
{


	// Get the executable
	return theBundle.GetChild("Contents/" + GetBundlePlatform()).GetChild(theName);
}





//=============================================================================
//		NCommonLinux::DebuggerIsActive : Is a debugger active?
//-----------------------------------------------------------------------------
bool NCommonLinux::DebuggerIsActive()
{


	// Check for a tracer
	NString theText  = NCommonLinux::GetProcFile(NFilePath("/proc/self/status"));
	bool    isActive = !theText.Contains("TracerPid:\t0\n");

	if (isActive)
	{
		isActive = theText.Contains("TracerPid:\t");
	}

	return isActive;
}





//=============================================================================
//		NCommonLinux::FileGetState : Get file state.
//-----------------------------------------------------------------------------
bool NCommonLinux::FileGetState(const NFilePath& thePath,
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



	// Fetch with statx
	if (wasOK && (theFlags & kNFileInfoMaskStatX) != 0)
	{
		wasOK = GetFileStateStatX(thePath, theState);
		if (wasOK)
		{
			validState |= kNFileInfoMaskStatX;
		}
	}



	// Fetch with access
	if (wasOK)
	{
		if ((theFlags & kNFileInfoCanRead) != 0)
		{
			NCommonPOSIX::FileGetStateAccess(thePath, kNFileInfoCanRead, theState);
			validState |= kNFileInfoCanRead;
		}

		if ((theFlags & kNFileInfoCanWrite) != 0)
		{
			NCommonPOSIX::FileGetStateAccess(thePath, kNFileInfoCanWrite, theState);
			validState |= kNFileInfoCanWrite;
		}

		if ((theFlags & kNFileInfoCanExecute) != 0)
		{
			NCommonPOSIX::FileGetStateAccess(thePath, kNFileInfoCanExecute, theState);
			validState |= kNFileInfoCanExecute;
		}
	}

	return wasOK;
}





//=============================================================================
//		NCommonLinux::MachineMemory : Get the memory.
//-----------------------------------------------------------------------------
uint64_t NCommonLinux::MachineMemory()
{


	// Get the memory
	uint64_t sizeBytes = 0;
	NString  theText   = GetProcFile(NFilePath("/proc/meminfo"));
	NString  theValue  = theText.GetMatch("MemTotal:\\s*(\\d+)");

	if (!theValue.IsEmpty())
	{
		sizeBytes = NNumber(theValue).GetUInt64() * kNKibibyte;
	}

	return sizeBytes;
}





//=============================================================================
//		NCommonLinux::MachineCPUName : Get the CPU name.
//-----------------------------------------------------------------------------
NString NCommonLinux::MachineCPUName()
{


	// Get the name
	NString theText = GetProcFile(NFilePath("/proc/cpuinfo"));
	NString theName = theText.GetMatch("model name\\s*:\\s*(.*)");
	NN_EXPECT(!theName.IsEmpty());

	if (theName.IsEmpty())
	{
		theName = "Unknown";
	}

	return theName;
}





//=============================================================================
//		NCommonLinux::MachineCPUVendor : Get the CPU vendor.
//-----------------------------------------------------------------------------
NString NCommonLinux::MachineCPUVendor()
{


	// Get the vendor
	NString theText = GetProcFile(NFilePath("/proc/cpuinfo"));
	NString theName = theText.GetMatch("vendor_id\\s*:\\s*(.*)");
	NN_EXPECT(!theName.IsEmpty());

	if (theName.IsEmpty())
	{
		theName = "Unknown";
	}

	return theName;
}





//=============================================================================
//		NCommonLinux::MachineCPUHertz : Get the CPU speed.
//-----------------------------------------------------------------------------
uint64_t NCommonLinux::MachineCPUHertz()
{


	// Get the maxium speed
	uint64_t theSpeed = 0;
	NString  theText =
		GetProcFile(NFilePath("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq"));

	if (!theText.IsEmpty())
	{
		theSpeed = NNumber(theText).GetUInt64() * kNKilohertz;
	}



	// Fall back to the current speed
	if (theSpeed == 0)
	{
		theText = GetProcFile(NFilePath("/proc/cpuinfo"));
		theText = theText.GetMatch("cpu MHz\\s*:\\s*(\\d+)");

		if (!theText.IsEmpty())
		{
			theSpeed = NNumber(theText).GetUInt64() * kNMegahertz;
		}
	}

	NN_REQUIRE(theSpeed != 0);
	return theSpeed;
}





//=============================================================================
//		NCommonLinux::PathRename : Atomically rename a path.
//-----------------------------------------------------------------------------
NStatus NCommonLinux::PathRename(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Rename the path
	const utf8_t* oldUTF8 = oldPath.GetUTF8();
	const utf8_t* newUTF8 = newPath.GetUTF8();

	int sysErr = int(syscall(SYS_renameat2, 0, oldUTF8, 0, newUTF8, RENAME_NOREPLACE));
	NN_EXPECT_NOT_ERR(sysErr);

	return NCommonPOSIX::StatusErrno(sysErr);
}





//=============================================================================
//		NCommonLinux::PathExchange : Atomically exchange paths.
//-----------------------------------------------------------------------------
NStatus NCommonLinux::PathExchange(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Exchange the paths
	const utf8_t* oldUTF8 = oldPath.GetUTF8();
	const utf8_t* newUTF8 = newPath.GetUTF8();

	int sysErr = int(syscall(SYS_renameat2, 0, oldUTF8, 0, newUTF8, RENAME_EXCHANGE));
	NN_EXPECT_NOT_ERR(sysErr);

	return NCommonPOSIX::StatusErrno(sysErr);
}





//=============================================================================
//		NCommonLinux::ProcessName : Get the process name.
//-----------------------------------------------------------------------------
NString NCommonLinux::ProcessName()
{


	// Get the name
	NString theText = GetProcFile(NFilePath("/proc/self/status"));
	NString theName = theText.GetMatch("Name\\s*:\\s*([^\\n]*)");
	NN_EXPECT(!theName.IsEmpty());

	if (theName.IsEmpty())
	{
		theName = "Unknown";
	}

	return theName;
}





//=============================================================================
//		NCommonLinux::ProcessMemory : Get the process memory usage.
//-----------------------------------------------------------------------------
NMemoryInfo NCommonLinux::ProcessMemory()
{


	// Get the state we need
	NString     theText = GetProcFile(NFilePath("/proc/self/status"));
	NMemoryInfo theInfo{};
	NString     theValue;



	// Get the memory usage
	//
	// addressSpaceUsed is set to allocated memory, although this will
	// over-estimate as freed memory typically does not release its
	// address space.
	//
	// addressSpaceMax is hard-coded to the standard Linux limits.
	theValue = theText.GetMatch("VmRSS\\s*:\\s*([0-9]*) kB");
	if (!theValue.IsEmpty())
	{
		theInfo.memoryResident = NNumber(theValue).GetUInt64() * kNKibibyte;
	}

	theValue = theText.GetMatch("VmSize\\s*:\\s*([0-9]*) kB");
	if (!theValue.IsEmpty())
	{
		theInfo.addressSpaceUsed = NNumber(theValue).GetUInt64() * kNKibibyte;
	}

	theInfo.memoryAllocated = theInfo.addressSpaceUsed;
	theInfo.addressSpaceMax = NN_ARCH_64 ? (128 * kNTebibyte) : (3 * kNGibibyte);

	return theInfo;
}





//=============================================================================
//		NCommonLinux::RandomSecureData : Get random data.
//-----------------------------------------------------------------------------
void NCommonLinux::RandomSecureData(size_t theSize, void* thePtr)
{


	// Get the data
	FILE* theFile = fopen("/dev/urandom", "r");
	bool  wasOK   = (theFile != nullptr);
	NN_EXPECT(wasOK);

	if (theFile != nullptr)
	{
		size_t sizeRead = fread(thePtr, 1, theSize, theFile);
		fclose(theFile);

		wasOK = (sizeRead == theSize);
		NN_EXPECT(wasOK);
	}



	// Handle failure
	if (!wasOK)
	{
		memset(thePtr, 0x00, theSize);
	}
}





//=============================================================================
//		NCommonLinux::SemaphoreCreate : Create a semaphore.
//-----------------------------------------------------------------------------
NSemaphoreRef NCommonLinux::SemaphoreCreate(size_t theValue)
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
//		NCommonLinux::SemaphoreDestroy : Destroy a semaphore.
//-----------------------------------------------------------------------------
void NCommonLinux::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{


	// Destroy the semaphore
	sem_t* semRef = static_cast<sem_t*>(theSemaphore);
	int    sysErr = sem_destroy(semRef);

	NN_REQUIRE_NOT_ERR(sysErr);

	free(semRef);
}





//=============================================================================
//		NCommonLinux::SemaphoreWait : Wait for a semaphore.
//-----------------------------------------------------------------------------
bool NCommonLinux::SemaphoreWait(NSemaphoreRef theSemaphore, NInterval waitFor)
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
//		NCommonLinux::SemaphoreSignal : Signal a semaphore.
//-----------------------------------------------------------------------------
void NCommonLinux::SemaphoreSignal(NSemaphoreRef theSemaphore)
{


	// Signal the semaphore
	sem_t* semRef = static_cast<sem_t*>(theSemaphore);
	int    sysErr = sem_post(semRef);

	NN_EXPECT_NOT_ERR(sysErr);
}





//=============================================================================
//		NCommonLinux::SystemPageSize : Get the page size.
//-----------------------------------------------------------------------------
size_t NCommonLinux::SystemPageSize()
{


	// Get the page size
	long pageSize = sysconf(_SC_PAGE_SIZE);
	NN_REQUIRE(pageSize > 0);

	return size_t(pageSize);
}





//=============================================================================
//		NCommonLinux::ThreadStackSize : Get the thread stack size.
//-----------------------------------------------------------------------------
size_t NCommonLinux::ThreadStackSize()
{


	// Get the stack size
	pthread_attr_t threadAttr{};
	size_t         stackSize = 0;
	int            sysErr    = pthread_getattr_np(pthread_self(), &threadAttr);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr == 0)
	{
		void* stackBase = nullptr;
		sysErr          = pthread_attr_getstack(&threadAttr, &stackBase, &stackSize);
		NN_EXPECT_NOT_ERR(sysErr);

		if (sysErr != 0)
		{
			stackSize = 0;
		}

		sysErr = pthread_attr_destroy(&threadAttr);
		NN_EXPECT_NOT_ERR(sysErr);
	}

	return stackSize;
}





//=============================================================================
//		NCommonLinux::ThreadGetCores : Get the current thread's preferred cores.
//-----------------------------------------------------------------------------
NVectorUInt8 NCommonLinux::ThreadGetCores()
{


	// Get the thread affinity
	cpu_set_t affinityMask;
	CPU_ZERO(&affinityMask);

	int sysErr = sched_getaffinity(0, sizeof(affinityMask), &affinityMask);
	NN_EXPECT_NOT_ERR(sysErr);

	size_t numCores = size_t(CPU_COUNT(&affinityMask));
	NN_EXPECT(numCores != 0);



	// Convert to cores
	//
	// An empty list indicates no affinity, i.e. every core.
	NVectorUInt8 theCores;

	if (numCores != NMachine::GetCores())
	{
		for (size_t n = 0; n < CPU_SETSIZE; n++)
		{
			if (CPU_ISSET(n, &affinityMask))
			{
				theCores.push_back(uint8_t(n));
			}
		}
	}

	return theCores;
}





//=============================================================================
//		NCommonLinux::ThreadSetCores : Set the current thread's preferred cores.
//-----------------------------------------------------------------------------
void NCommonLinux::ThreadSetCores(const NVectorUInt8& theCores)
{


	// Validate our parameters
	NN_REQUIRE(theCores.size() <= CPU_SETSIZE);


	// Build the thread affinity
	//
	// An empty list indicates no affinity, i.e. every core.
	cpu_set_t affinityMask;
	CPU_ZERO(&affinityMask);

	if (theCores.empty())
	{
		size_t numCores = NMachine::GetCores();

		for (size_t n = 0; n < numCores; n++)
		{
			CPU_SET(n, &affinityMask);
		}
	}
	else
	{
		for (auto n : theCores)
		{
			CPU_SET(n, &affinityMask);
		}
	}



	// Set the thread affinity
	int sysErr = sched_setaffinity(0, sizeof(affinityMask), &affinityMask);
	NN_EXPECT_NOT_ERR(sysErr);
}





//=============================================================================
//		NCommonLinux::TimeGet : Get the current time.
//-----------------------------------------------------------------------------
NTime NCommonLinux::TimeGet()
{


	// Get the time
	struct timeval timeVal = {};

	int sysErr = ::gettimeofday(&timeVal, nullptr);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeVal, 0x00, sizeof(timeVal));
	}

	return NTime(NCommonPOSIX::TimeGetInterval(timeVal), kNanoEpochFrom1970);
}





//=============================================================================
//		NCommonLinux::TimeGetUpTime : Get the time since boot.
//-----------------------------------------------------------------------------
NInterval NCommonLinux::TimeGetUpTime()
{


	// Get the time since boot
	struct timespec timeSpec = {};

	int sysErr = ::clock_gettime(CLOCK_MONOTONIC, &timeSpec);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeSpec, 0x00, sizeof(timeSpec));
	}

	return NTimeUtils::ToInterval(timeSpec);
}





//=============================================================================
//		NCommonLinux::TimeGetClockTicks : Get the clock ticks.
//-----------------------------------------------------------------------------
uint64_t NCommonLinux::TimeGetClockTicks()
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
//		NCommonLinux::TimeGetClockFrequency : Get the clock frequency.
//-----------------------------------------------------------------------------
uint64_t NCommonLinux::TimeGetClockFrequency()
{


	// Get the clock frequency
	return kNanosecondsPerSecond;
}
