/*	NAME:
		NSharedDarwin.mm

	DESCRIPTION:
		Darwin support.

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
#include "NSharedDarwin.h"

// Nano
#include "NCFData.h"
#include "NCFString.h"
#include "NCFURL.h"
#include "NCGImage.h"
#include "NCoreFoundation.h"
#include "NData.h"
#include "NDebug.h"
#include "NDictionary.h"
#include "NFile.h"
#include "NFilePath.h"
#include "NFormat.h"
#include "NSemaphore.h"
#include "NSharedPOSIX.h"
#include "NString.h"
#include "NTime.h"
#include "NTimeUtils.h"
#include "NUTI.h"
#include "NVersion.h"

// System
#include <CoreFoundation/CoreFoundation.h>
#include <CoreServices/CoreServices.h>
#include <Foundation/Foundation.h>
#include <ImageIO/ImageIO.h>
#include <cxxabi.h>
#include <dispatch/semaphore.h>
#include <execinfo.h>
#include <fcntl.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <mach/thread_policy.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <unistd.h>





//=============================================================================
//		Function Prototypes
//-----------------------------------------------------------------------------
// mach/thread_policy.h
extern "C" kern_return_t thread_policy_set(thread_t               thread,
										   thread_policy_flavor_t flavor,
										   thread_policy_t        policy_info,
										   mach_msg_type_number_t count);

extern "C" kern_return_t thread_policy_get(thread_t                thread,
										   thread_policy_flavor_t  flavor,
										   thread_policy_t         policy_info,
										   mach_msg_type_number_t* count,
										   boolean_t*              get_default);





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Debugger
static const char*      kNDebuggerSymbolName                = "0x\\w+ (\\w+) \\+ \\d+$";
static constexpr size_t kNDebuggerMaxFrames                 = 512;

// File
static constexpr NFileInfoFlags kNFileInfoMaskStat =
	kNFileInfoExists | kNFileInfoIsFile | kNFileInfoIsDirectory | kNFileInfoCreationTime |
	kNFileInfoModifiedTime | kNFileInfoFileSize;





//=============================================================================
//		Internal Types
//-----------------------------------------------------------------------------
// Runloop
struct NDarwinRunLoop
{
	CFRunLoopRef      cfRunLoop;
	CFRunLoopTimerRef cfStopTimer;
};


// NCFObject helpers
using NCFBundle           = NCFObject<CFBundleRef>;
using NCGImageSource      = NCFObject<CGImageSourceRef>;
using NCGImageDestination = NCFObject<CGImageDestinationRef>;





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//      GetBundle : Get a bundle.
//----------------------------------------------------------------------------
static NCFBundle GetBundle(const NFile& theBundle)
{
	// Check our parameters
	NCFBundle cfBundle;

	if (!theBundle.IsDirectory())
	{
		return cfBundle;
	}



	// Get the bundle
	NCFURL cfURL;

	if (cfURL.Assign(CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
												   ToCF(theBundle.GetPath().GetPath()),
												   kCFURLPOSIXPathStyle,
												   true)))
	{
		cfBundle.Assign(CFBundleCreate(kCFAllocatorDefault, cfURL));
		NN_EXPECT(cfBundle.IsValid());
	}

	return cfBundle;
}





//=============================================================================
//		GetSysctl : Get a named sysctl value.
//-----------------------------------------------------------------------------
template<typename T>
T GetSysctl(const char* theName)
{


	// Get the value
	size_t theSize = sizeof(T);
	T      theResult{};

	int sysErr = sysctlbyname(theName, &theResult, &theSize, nullptr, 0);
	if (sysErr != 0)
	{
		NN_LOG_WARNING("sysctlbyname(\"{}\") failed with {}", theName, sysErr);
		memset(&theResult, 0x00, sizeof(theResult));
	}

	return theResult;
}





//=============================================================================
//		GetSysctl : Get a named sysctl value.
//-----------------------------------------------------------------------------
template<>
NString GetSysctl<NString>(const char* theName)
{


	// Get the size
	size_t  theSize = 0;
	NString theResult;

	int sysErr = sysctlbyname(theName, nullptr, &theSize, nullptr, 0);
	if (sysErr == 0)
	{
		NData theData(theSize, nullptr, NDataSource::Zero);

		sysErr = sysctlbyname(theName, theData.GetMutableData(), &theSize, nullptr, 0);
		if (sysErr == 0)
		{
			theResult = reinterpret_cast<const utf8_t*>(theData.GetData());
		}
		else
		{
			NN_LOG_WARNING("sysctlbyname(\"{}\") failed with {}", theName, sysErr);
		}
	}

	return theResult;
}





//=============================================================================
//		GetBootTime : Get the boot time.
//-----------------------------------------------------------------------------
static NTime GetBootTime()
{


	// Get the boot time
	struct timeval timeVal = GetSysctl<struct timeval>("kern.boottime");

	return NTime(NSharedPOSIX::TimeGetInterval(timeVal), kNanoEpochFrom2001);
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
static NFilePath GetNSSearchPath(NSSearchPathDomainMask theDomain,
								 NSSearchPathDirectory  theDirectory)
{


	// Get the location
	NFilePath thePath;

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





//=============================================================================
//		StopRunLoop : Stop a runloop.
//-----------------------------------------------------------------------------
static void StopRunLoop(CFRunLoopTimerRef /*cfTimer*/, void* /*userData*/)
{


	// Stop the runloop
	CFRunLoopStop(CFRunLoopGetCurrent());
}





#pragma mark NSharedDarwin
//=============================================================================
//		NSharedDarwin::BundleGet : Get the bundle for an ID.
//-----------------------------------------------------------------------------
NFile NSharedDarwin::BundleGet(const NString& bundleID)
{


	// Get the bundle
	NCFBundle cfBundle;
	NFile     theFile;

	if (bundleID.IsEmpty())
	{
		cfBundle.Set(CFBundleGetMainBundle());
	}
	else
	{
		cfBundle.Set(CFBundleGetBundleWithIdentifier(ToCF(bundleID)));
	}



	// Get the file
	if (cfBundle.IsValid())
	{
		NCFURL cfURL;

		if (cfURL.Assign(CFBundleCopyBundleURL(cfBundle)))
		{
			NCFString cfString;

			if (cfString.Assign(CFURLCopyFileSystemPath(cfURL, kCFURLPOSIXPathStyle)))
			{
				theFile = NFile(cfString.GetString());
			}
		}
	}

	return theFile;
}





//=============================================================================
//		NSharedDarwin::BundleGetResources : Get the resources directory for the bundle.
//-----------------------------------------------------------------------------
NFile NSharedDarwin::BundleGetResources(const NFile& theBundle)
{


	// Get the state we need
	NCFBundle cfBundle = GetBundle(theBundle);
	NFile     theFile;

	NN_EXPECT(cfBundle.IsValid());


	// Get the resources
	if (cfBundle.IsValid())
	{
		NCFURL cfURL;

		if (cfURL.Assign(CFBundleCopyResourcesDirectoryURL(cfBundle)))
		{
			if (cfURL.Assign(CFURLCopyAbsoluteURL(cfURL)))
			{
				NCFString cfString;

				if (cfString.Assign(CFURLCopyFileSystemPath(cfURL, kCFURLPOSIXPathStyle)))
				{
					theFile = NFile(cfString.GetString());
				}
			}
		}
	}

	return theFile;
}





//=============================================================================
//		NSharedDarwin::BundleGetInfoDictionary : Get the Info.plist dictionary for a bundle.
//-----------------------------------------------------------------------------
NDictionary NSharedDarwin::BundleGetInfoDictionary(const NFile& theBundle)
{


	// Get the state we need
	NCFBundle     cfBundle = GetBundle(theBundle);
	NCFDictionary cfDictionary;

	NN_EXPECT(cfBundle.IsValid());



	// Get the info
	if (cfBundle.IsValid())
	{
		cfDictionary.Set(CFBundleGetInfoDictionary(cfBundle));
		NN_EXPECT(CFDictionaryGetCount(cfDictionary) != 0);
	}

	return cfDictionary.GetDictionary();
}





//=============================================================================
//		NSharedDarwin::BundleGetExecutable : Get an executable from a bundle.
//-----------------------------------------------------------------------------
NFile NSharedDarwin::BundleGetExecutable(const NFile& theBundle, const NString& theName)
{


	// Get the state we need
	NCFBundle cfBundle = GetBundle(theBundle);
	NFile     theFile;

	NN_EXPECT(cfBundle.IsValid());



	// Get the executable
	if (cfBundle.IsValid())
	{
		NCFURL cfURL;

		if (cfURL.Assign(CFBundleCopyAuxiliaryExecutableURL(cfBundle, ToCF(theName))))
		{
			if (cfURL.Assign(CFURLCopyAbsoluteURL(cfURL)))
			{
				NCFString cfString;

				if (cfString.Assign(CFURLCopyFileSystemPath(cfURL, kCFURLPOSIXPathStyle)))
				{
					theFile = NFile(cfString.GetString());
				}
			}
		}
	}

	return theFile;
}





//=============================================================================
//		NSharedDarwin::TimeGet : Get the current time.
//-----------------------------------------------------------------------------
NTime NSharedDarwin::TimeGet()
{


	// Get the time
	return NTime(CFAbsoluteTimeGetCurrent(), kNanoEpochFrom2001);
}





//=============================================================================
//		NSharedDarwin::TimeGetUpTime : Get the time since boot.
//-----------------------------------------------------------------------------
NInterval NSharedDarwin::TimeGetUpTime()
{


	// Get the time since boot
	static NTime sBootTime = GetBootTime();

	return TimeGet() - sBootTime;
}





//=============================================================================
//		NSharedDarwin::TimeGetClockTicks : Get the clock ticks.
//-----------------------------------------------------------------------------
uint64_t NSharedDarwin::TimeGetClockTicks()
{


	// Get the clock ticks
	return uint64_t(mach_absolute_time());
}





//=============================================================================
//		NSharedDarwin::TimeGetClockFrequency : Get the clock frequency.
//-----------------------------------------------------------------------------
uint64_t NSharedDarwin::TimeGetClockFrequency()
{


	// Get the clock frequency
	static uint64_t sClockFrequency = GetMachClockFrequency();

	return sClockFrequency;
}





//=============================================================================
//		NSharedDarwin::DebuggerIsActive : Is a debugger active?
//-----------------------------------------------------------------------------
bool NSharedDarwin::DebuggerIsActive()
{


	// Get the process flags
	//
	// https://developer.apple.com/library/content/qa/qa1361/_index.html
	struct kinfo_proc theInfo    = {};
	size_t            theSize    = sizeof(theInfo);
	int               theName[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid()};

	int sysErr = sysctl(theName, std::size(theName), &theInfo, &theSize, nullptr, 0);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		theInfo = {};
	}



	// Check for P_TRACED
	return theInfo.kp_proc.p_flag & P_TRACED;
}





//=============================================================================
//		NSharedDarwin::DebuggerGetBacktrace : Get a backtrace.
//-----------------------------------------------------------------------------
NVectorString NSharedDarwin::DebuggerGetBacktrace(size_t skipFrames, size_t numFrames)
{


	// Get the state we need
	//
	// We increase skipFrames to skip ourselves.
	skipFrames++;

	if (numFrames != kNSizeMax)
	{
		numFrames += skipFrames;
	}

	numFrames = std::min(numFrames, kNDebuggerMaxFrames);



	// Get the frames
	void* theFrames[kNDebuggerMaxFrames];
	numFrames = size_t(backtrace(theFrames, int(numFrames)));

	char** theSymbols = backtrace_symbols(theFrames, int(numFrames));
	if (theSymbols == nullptr)
	{
		numFrames = 0;
	}



	// Process the symbols
	//
	// Example symbols:
	//
	//		"0   NanoTest_macOS                      0x0000000100e542bb _ZN13NSharedDarwin20DebuggerGetBacktraceEmm + 1083"
	//		"1   NanoTest_macOS                      0x0000000101072219 _ZN9NDebugger12GetBacktraceEmm + 57"
	//		"2   NanoTest_macOS                      0x0000000100c569f4 _ZN12_GLOBAL__N_129____C_A_T_C_H____T_E_S_T____24testEv + 1060"
	//		"3   NanoTest_macOS                      0x0000000100c55efb _ZN12_GLOBAL__N_129____C_A_T_C_H____T_E_S_T____210InvokeTestEv + 251"
	//		"4   NanoTest_macOS                      0x0000000100c5d5f5 _ZNK5Catch19TestInvokerAsMethodIN12_GLOBAL__N_129____C_A_T_C_H____T_E_S_T____2EE6invokeEv + 549"
	//		"5   NanoTest_macOS                      0x0000000100335e95 _ZNK5Catch8TestCase6invokeEv + 325"
	//		"6   NanoTest_macOS                      0x0000000100335b77 _ZN5Catch10RunContext20invokeActiveTestCaseEv + 423"
	//		"7   NanoTest_macOS                      0x000000010032a1cf _ZN5Catch10RunContext14runCurrentTestERNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEES8_ + 3471"
	//		"8   NanoTest_macOS                      0x0000000100326cff _ZN5Catch10RunContext7runTestERKNS_8TestCaseE + 2943"
	//		"9   NanoTest_macOS                      0x0000000100343fa1 _ZN5Catch12_GLOBAL__N_19TestGroup7executeEv + 2401"
	//		"10  NanoTest_macOS                      0x00000001003417fa _ZN5Catch7Session11runInternalEv + 2426"
	//		"11  NanoTest_macOS                      0x0000000100340cc0 _ZN5Catch7Session3runEv + 416"
	//		"12  NanoTest_macOS                      0x00000001003be38c _ZN5Catch7Session3runIcEEiiPKPKT_ + 284"
	//		"13  NanoTest_macOS                      0x00000001003be0ac main + 364"
	//		"14  libdyld.dylib                       0x00007fff709163d5 start + 1"
	//
	NVectorString theTrace;

	for (size_t n = 0; n < numFrames; n++)
	{
		if (n >= skipFrames)
		{
			NString theSymbol(theSymbols[n]);
			NString theName = theSymbol.GetMatch(kNDebuggerSymbolName);

			if (!theName.IsEmpty())
			{
				// Demangle the name
				//
				// C symbols can't be demangled so are used as-is.
				int   sysErr  = 0;
				char* cppName = abi::__cxa_demangle(theName.GetUTF8(), nullptr, nullptr, &sysErr);

				if (cppName != nullptr && sysErr == 0)
				{
					theName = cppName;
					free(cppName);
				}

				theTrace.emplace_back(theName);
			}
		}
	}



	// Clean up
	if (theSymbols != nullptr)
	{
		free(theSymbols);
	}

	return theTrace;
}





//=============================================================================
//		NSharedDarwin::FileGetState : Get file state.
//-----------------------------------------------------------------------------
bool NSharedDarwin::FileGetState(const NFilePath& thePath,
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
			NSharedPOSIX::FileGetStateAccess(thePath, kNFileInfoCanRead, theState);
			validState |= kNFileInfoCanRead;
		}

		if ((theFlags & kNFileInfoCanWrite) != 0)
		{
			NSharedPOSIX::FileGetStateAccess(thePath, kNFileInfoCanWrite, theState);
			validState |= kNFileInfoCanWrite;
		}

		if ((theFlags & kNFileInfoCanExecute) != 0)
		{
			NSharedPOSIX::FileGetStateAccess(thePath, kNFileInfoCanExecute, theState);
			validState |= kNFileInfoCanExecute;
		}
	}

	return wasOK;
}





//=============================================================================
//		NSharedDarwin::PathRename : Atomically rename a path.
//-----------------------------------------------------------------------------
NStatus NSharedDarwin::PathRename(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Rename the path
	int sysErr = renamex_np(oldPath.GetUTF8(), newPath.GetUTF8(), RENAME_EXCL);
	NN_EXPECT_NOT_ERR(sysErr);

	return NSharedPOSIX::StatusErrno(sysErr);
}





//=============================================================================
//		NSharedDarwin::PathExchange : Atomically exchange two paths.
//-----------------------------------------------------------------------------
NStatus NSharedDarwin::PathExchange(const NFilePath& oldPath, const NFilePath& newPath)
{


	// Exchange the paths
	int sysErr = renamex_np(oldPath.GetUTF8(), newPath.GetUTF8(), RENAME_SWAP);
	NN_EXPECT_NOT_ERR(sysErr);

	return NSharedPOSIX::StatusErrno(sysErr);
}





//=============================================================================
//		NSharedDarwin::PathLocation : Get a location as a path.
//-----------------------------------------------------------------------------
NFilePath NSharedDarwin::PathLocation(NFileLocation theLocation)
{


	// Get the location
	NFilePath thePath;

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
			if (thePath.IsValid())
			{
				thePath = thePath.GetChild("Logs");
			}
			break;

		case NFileLocation::UserPictures:
			thePath = GetNSSearchPath(NSUserDomainMask, NSPicturesDirectory);
			break;

		case NFileLocation::UserPreferences:
			thePath = GetNSSearchPath(NSUserDomainMask, NSLibraryDirectory);
			if (thePath.IsValid())
			{
				thePath = thePath.GetChild("Preferences");
			}
			break;
	}

	return thePath;
}





//=============================================================================
//		NSharedDarwin::ImageDecode : Decode image data.
//-----------------------------------------------------------------------------
NStatus NSharedDarwin::ImageDecode(NImage& theImage, const NData& theData)
{


	// Get the state we need
	NCGImageSource cgImageSource;
	NCFData        cfData;

	if (!cfData.Assign(CFDataCreateWithBytesNoCopy(kCFAllocatorDefault,
												   theData.GetData(),
												   CFIndex(theData.GetSize()),
												   kCFAllocatorNull)))
	{
		return NStatus::Memory;
	}

	if (!cgImageSource.Assign(CGImageSourceCreateWithData(cfData, nullptr)))
	{
		return NStatus::Memory;
	}



	// Decode the image
	NCGImage cgImage;

	if (cgImage.Assign(CGImageSourceCreateImageAtIndex(cgImageSource, 0, nullptr)))
	{
		theImage = cgImage.GetImage();
		return NStatus::OK;
	}

	return NStatus::NotSupported;
}





//=============================================================================
//		NSharedDarwin::ImageEncode : Encode image data.
//-----------------------------------------------------------------------------
NData NSharedDarwin::ImageEncode(const NImage& theImage, const NUTI& theType)
{


	// Validate our parameters and state
	NN_REQUIRE(theImage.IsValid());
	NN_REQUIRE(theType.IsValid());



	// Get the state we need
	CFStringRef dstFormat = nullptr;

	if (theType == kNUTTypePNG)
	{
		dstFormat = kUTTypePNG;
	}
	else if (theType == kNUTTypeJPEG)
	{
		dstFormat = kUTTypeJPEG;
	}
	else if (theType == kNUTTypeTIFF)
	{
		dstFormat = kUTTypeTIFF;
	}
	else if (theType == kNUTTypeGIF)
	{
		dstFormat = kUTTypeGIF;
	}
	else
	{
		NN_LOG_ERROR("Unsupported format: {}", theType);
		return NData();
	}



	// Encode the image
	NCGImageDestination cgImageDst;
	NCGImage            cgImage;
	NCFMutableData      cfData;

	if (cfData.Assign(CFDataCreateMutable(kCFAllocatorDefault, 0)) &&
		cgImageDst.Assign(CGImageDestinationCreateWithData(cfData, dstFormat, 1, nullptr)) &&
		cgImage.SetImage(theImage))
	{
		CGImageDestinationAddImage(cgImageDst, cgImage, nullptr);

		if (CGImageDestinationFinalize(cgImageDst))
		{
			return cfData.GetData();
		}
	}

	return NData();
}





//=============================================================================
//		NSharedDarwin::ThreadIsMain : Is this the main thread?
//-----------------------------------------------------------------------------
bool NSharedDarwin::ThreadIsMain()
{


	// Check the thread
	int sysErr = pthread_main_np();
	NN_REQUIRE(sysErr == 1 || sysErr == 0);

	return sysErr == 1;
}





//=============================================================================
//		NSharedDarwin::ThreadStackSize : Get the thread stack size.
//-----------------------------------------------------------------------------
size_t NSharedDarwin::ThreadStackSize()
{


	// Get the stack size
	return pthread_get_stacksize_np(pthread_self());
}





//=============================================================================
//		NSharedDarwin::ThreadGetCores : Get the current thread's preferred cores.
//-----------------------------------------------------------------------------
NVectorUInt8 NSharedDarwin::ThreadGetCores()
{


	// Get the thread affinity
	thread_affinity_policy_data_t threadAffinity{};
	mach_msg_type_number_t        numInfo    = 1;
	boolean_t                     getDefault = false;


	kern_return_t machErr = thread_policy_get(pthread_mach_thread_np(pthread_self()),
											  THREAD_AFFINITY_POLICY,
											  thread_policy_t(&threadAffinity),
											  &numInfo,
											  &getDefault);
	NN_EXPECT_NOT_ERR(machErr);
	NN_EXPECT(threadAffinity.affinity_tag <= kNUInt8Max);



	// Convert to cores
	//
	// An empty list indicates no affinity, i.e. every core.
	//
	// We use core+1 as our affinity tag so that a tag of 0 represents no preference.
	NVectorUInt8 theCores;

	if (threadAffinity.affinity_tag != 0)
	{
		theCores.push_back(uint8_t(threadAffinity.affinity_tag - 1));
	}

	return theCores;
}





//=============================================================================
//		NSharedDarwin::ThreadSetCores : Set the current thread's preferred cores.
//-----------------------------------------------------------------------------
void NSharedDarwin::ThreadSetCores(const NVectorUInt8& theCores)
{


	// Get the state we need
	//
	// We use core+1 as our affinity tag so that a tag of 0 represents no preference.
	thread_affinity_policy_data_t threadAffinity{};

	switch (theCores.size())
	{
		case 0:
			threadAffinity.affinity_tag = 0;
			break;

		case 1:
			threadAffinity.affinity_tag = theCores[0] + 1;
			break;

		default:
			threadAffinity.affinity_tag = theCores[0] + 1;
			NN_LOG_WARNING("NSharedDarwin::ThreadSetCores only supports one core!");
			break;
	}



	// Set the thread affinity
	kern_return_t machErr = thread_policy_set(pthread_mach_thread_np(pthread_self()),
											  THREAD_AFFINITY_POLICY,
											  thread_policy_t(&threadAffinity),
											  1);

	NN_EXPECT_NOT_ERR(machErr);
}





//=============================================================================
//		NSharedDarwin::RandomSecureData : Get random data.
//-----------------------------------------------------------------------------
void NSharedDarwin::RandomSecureData(size_t theSize, void* thePtr)
{


	// Get the data
	int sysErr = SecRandomCopyBytes(kSecRandomDefault, theSize, thePtr);
	NN_EXPECT_NOT_ERR(sysErr);



	// Handle failure
	if (sysErr != 0)
	{
		memset(thePtr, 0x00, theSize);
	}
}





//=============================================================================
//		NSharedDarwin::RunLoopCreate : Create a runloop.
//-----------------------------------------------------------------------------
NRunLoopHandle NSharedDarwin::RunLoopCreate(bool isMain)
{


	// Validate our state
	static_assert(sizeof(NRunLoopHandle) >= sizeof(NDarwinRunLoop*));



	// Create the runloop
	NDarwinRunLoop* darwinRunLoop = new NDarwinRunLoop{};

	darwinRunLoop->cfRunLoop = isMain ? CFRunLoopGetMain() : CFRunLoopGetCurrent();
	CFRetain(darwinRunLoop->cfRunLoop);



	// Create the stop timer
	//
	// CFRunLoop does not track stop requests across threads so a call to
	// CFRunLoopStop on one thread may be lost if it occurs just before a
	// CFRunLoopRunInMode on another thread.
	//
	// To avoid this we stop runloops by using a timer that performs the
	// stop on the runloop's own thread.
	//
	// This timer is initialised to repeat in the far future and is reset
	// to fire whenever we want to stop the runloop.
	//
	//
	// In addition CFRunLoopRunInMode returns kCFRunLoopRunFinished immediately
	// if called on an empty runloop, regardless of the timeout parameter.
	//
	// The presence of our stop timer ensures CFRunLoopRunInMode honours its
	// timeout parameter.
	darwinRunLoop->cfStopTimer =
		CFRunLoopTimerCreate(kCFAllocatorDefault,
							 CFAbsoluteTimeGetCurrent() + kNTimeDistantFuture,
							 kNTimeDistantFuture,
							 0,
							 0,
							 StopRunLoop,
							 nullptr);

	CFRunLoopAddTimer(darwinRunLoop->cfRunLoop, darwinRunLoop->cfStopTimer, kCFRunLoopCommonModes);

	return NRunLoopHandle(darwinRunLoop);
}





//=============================================================================
//		NSharedDarwin::RunLoopDestroy : Destroy a runloop.
//-----------------------------------------------------------------------------
void NSharedDarwin::RunLoopDestroy(NRunLoopHandle runLoop)
{


	// Get the state we need
	NDarwinRunLoop* darwinRunLoop = reinterpret_cast<NDarwinRunLoop*>(runLoop);



	// Destroy the runloop
	CFRelease(darwinRunLoop->cfRunLoop);
	CFRelease(darwinRunLoop->cfStopTimer);
}





//=============================================================================
//		NSharedDarwin::RunLoopSleep : Sleep a runloop.
//-----------------------------------------------------------------------------
void NSharedDarwin::RunLoopSleep(NRunLoopHandle runLoop, NInterval sleepFor)
{


	// Get the state we need
	NDarwinRunLoop* darwinRunLoop = reinterpret_cast<NDarwinRunLoop*>(runLoop);

	NN_REQUIRE(darwinRunLoop->cfRunLoop == CFRunLoopGetCurrent());
	NN_UNUSED(darwinRunLoop);



	// Sleep the runloop
	if (sleepFor == kNTimeForever)
	{
		CFRunLoopRun();
	}
	else
	{
		CFRunLoopRunInMode(kCFRunLoopDefaultMode, sleepFor, false);
	}
}





//=============================================================================
//		NSharedDarwin::RunLoopWake : Wake a runloop.
//-----------------------------------------------------------------------------
void NSharedDarwin::RunLoopWake(NRunLoopHandle runLoop)
{


	// Get the state we need
	NDarwinRunLoop* darwinRunLoop = reinterpret_cast<NDarwinRunLoop*>(runLoop);



	// Wake the runloop
	CFRunLoopTimerSetNextFireDate(darwinRunLoop->cfStopTimer, CFAbsoluteTimeGetCurrent());
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





//=============================================================================
//		NSharedDarwin::ProcessName : Get the process name.
//-----------------------------------------------------------------------------
NString NSharedDarwin::ProcessName()
{


	// Get the name
	@autoreleasepool
	{
		return NString([[NSProcessInfo processInfo] processName].UTF8String);
	}
}





//=============================================================================
//		NSharedDarwin::ProcessMemory : Get the process memory usage.
//-----------------------------------------------------------------------------
NMemoryInfo NSharedDarwin::ProcessMemory()
{


	// Get the state we need
	task_vm_info_data_t    vmInfo{};
	mach_msg_type_number_t theCount = TASK_VM_INFO_COUNT;

	kern_return_t kernErr =
		task_info(mach_task_self(), TASK_VM_INFO, task_info_t(&vmInfo), &theCount);

	NN_EXPECT_NOT_ERR(kernErr);



	// Get the memory usage
	NMemoryInfo theInfo{};

	if (kernErr == KERN_SUCCESS)
	{
		NN_REQUIRE(vmInfo.max_address > vmInfo.min_address);

		theInfo.memoryAllocated  = vmInfo.phys_footprint;
		theInfo.memoryResident   = vmInfo.resident_size;
		theInfo.addressSpaceUsed = vmInfo.virtual_size;
		theInfo.addressSpaceMax  = vmInfo.max_address - vmInfo.min_address;
	}

	return theInfo;
}





//=============================================================================
//		NSharedDarwin::SystemPageSize : Get the page size.
//-----------------------------------------------------------------------------
size_t NSharedDarwin::SystemPageSize()
{


	// Get the page size
	int pageSize = getpagesize();
	NN_REQUIRE(pageSize > 0);

	return size_t(pageSize);
}





//=============================================================================
//		NSharedDarwin::SystemVersion : Get the OS version.
//-----------------------------------------------------------------------------
NVersion NSharedDarwin::SystemVersion()
{


	// Validate our state
	static_assert(NN_TARGET_MACOS || NN_TARGET_IOS || NN_TARGET_TVOS);



	// Get the state we need
	NSOperatingSystemVersion osVersion{};

	@autoreleasepool
	{
		osVersion = NSProcessInfo.processInfo.operatingSystemVersion;
	}

	NN_EXPECT(osVersion.majorVersion <= kNUInt8Max);
	NN_EXPECT(osVersion.minorVersion <= kNUInt8Max);
	NN_EXPECT(osVersion.patchVersion <= kNUInt8Max);



	// Get the version
	NVersion theVersion(kNOSmacOS,
						uint8_t(osVersion.majorVersion),
						uint8_t(osVersion.minorVersion),
						uint8_t(osVersion.patchVersion));

	if constexpr (NN_TARGET_IOS)
	{
		theVersion.SetProduct(kNOSiOS);
	}
	else if constexpr (NN_TARGET_TVOS)
	{
		theVersion.SetProduct(kNOStvOS);
	}

	return theVersion;
}





//=============================================================================
//		NSharedDarwin::SystemName : Get the OS name.
//-----------------------------------------------------------------------------
NString NSharedDarwin::SystemName(NOSName theName)
{


	// Validate our parameters and state
	NN_REQUIRE(theName == NOSName::Build || theName == NOSName::Maximum);



	// Get the name
	NString theText;

	if (theName == NOSName::Build)
	{
		theText = GetSysctl<NString>("kern.osversion");
	}
	else
	{
		theText = NFormat("{} [{}]",
						  NSystem::GetName(NOSName::Detailed),
						  GetSysctl<NString>("kern.version"));
	}

	return theText;
}





//=============================================================================
//		NSharedDarwin::MachineCores : Get the number of cores.
//-----------------------------------------------------------------------------
size_t NSharedDarwin::MachineCores(NCoreType theType)
{


	// Get the cores
	bool    getPhysical = (theType == NCoreType::Physical);
	int32_t numCores = GetSysctl<int32_t>(getPhysical ? "hw.physicalcpu_max" : "hw.logicalcpu_max");
	NN_REQUIRE(numCores >= 1);

	return size_t(numCores);
}





//=============================================================================
//		NSharedDarwin::MachineMemory : Get the memory.
//-----------------------------------------------------------------------------
uint64_t NSharedDarwin::MachineMemory()
{


	// Get the memory
	int64_t sizeBytes = GetSysctl<int64_t>("hw.memsize");
	NN_REQUIRE(sizeBytes > 0);

	return uint64_t(sizeBytes);
}





//=============================================================================
//		NSharedDarwin::MachineCPUName : Get the CPU name.
//-----------------------------------------------------------------------------
NString NSharedDarwin::MachineCPUName()
{


	// Get the name
	//
	// CPU brand name is only available on macOS.
	NString theName = GetSysctl<NString>("machdep.cpu.brand_string");
	NN_EXPECT(!theName.IsEmpty());

	if (theName.IsEmpty())
	{
		theName = "Unknown";
	}

	return theName;
}





//=============================================================================
//		NSharedDarwin::MachineCPUVendor : Get the CPU vendor.
//-----------------------------------------------------------------------------
NString NSharedDarwin::MachineCPUVendor()
{


	// Get the vendor
	//
	// CPU vendor is only available on macOS.
	NString theName = GetSysctl<NString>("machdep.cpu.vendor");
	NN_EXPECT(!theName.IsEmpty());

	if (theName.IsEmpty())
	{
		theName = "Unknown";
	}

	return theName;
}





//=============================================================================
//		NSharedDarwin::MachineCPUHertz : Get the CPU speed.
//-----------------------------------------------------------------------------
uint64_t NSharedDarwin::MachineCPUHertz()
{


	// Get the speed
	//
	// CPU frequency is only available on macOS.
#if NN_TARGET_MACOS
	int64_t speedHz = GetSysctl<int64_t>("hw.cpufrequency_max");
	NN_REQUIRE(speedHz > 0);
#else
	int64_t speedHz = 2 * kNGigahertz;
#endif

	return uint64_t(speedHz);
}
