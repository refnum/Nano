/*	NAME:
		NCommonDarwin.h

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
#ifndef NCOMMON_DARWIN_H
#define NCOMMON_DARWIN_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NFileInfo.h"
#include "NFileUtils.h"
#include "NMachine.h"
#include "NProcess.h"
#include "NRunLoop.h"
#include "NSystem.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NDictionary;
class NFile;
class NFilePath;
class NImage;
class NSemaphore;
class NString;
class NTime;
class NUTI;
class NVersion;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NCommonDarwin
{
public:
	// Bundles
	static NFile                        BundleGet(              const NString& bundleID);
	static NFile                        BundleGetResources(     const NFile& theBundle);
	static NDictionary                  BundleGetInfoDictionary(const NFile& theBundle);
	static NFile                        BundleGetExecutable(    const NFile& theBundle, const NString& theName);


	// Debugger
	static bool                         DebuggerIsActive();
	static NVectorString                DebuggerGetBacktrace(size_t skipFrames, size_t numFrames);


	// File state
	static bool                         FileGetState(const NFilePath& thePath,
													 NFileStateFlags  theFlags,
													 NFileStateFlags& validState,
													 NFileInfoState&  theState);


	// Images
	static NStatus                      ImageDecode(      NImage& theImage, const NData& theData);
	static NData                        ImageEncode(const NImage& theImage, const NUTI& theType);


	// Machine
	static size_t                       MachineCores(NCoreType theType);
	static uint64_t                     MachineMemory();
	static NString                      MachineCPUName();
	static NString                      MachineCPUVendor();
	static uint64_t                     MachineCPUHertz();


	// File paths
	static NStatus                      PathRename(  const NFilePath& pathOld, const NFilePath& pathNew);
	static NStatus                      PathExchange(const NFilePath& oldPath, const NFilePath& newPath);
	static NFilePath                    PathLocation(NFileLocation theLocation);


	// Process
	static NString                      ProcessName();
	static NMemoryInfo                  ProcessMemory();


	// Random
	static void                         RandomSecureData(size_t theSize, void* thePtr);


	// Runloops
	static NRunLoopHandle               RunLoopCreate(bool isMain);
	static void                         RunLoopDestroy(NRunLoopHandle runLoop);
	static void                         RunLoopSleep(  NRunLoopHandle runLoop, NInterval sleepFor);
	static void                         RunLoopWake(   NRunLoopHandle runLoop);


	// Semaphores
	static NSemaphoreRef                SemaphoreCreate(size_t theValue);
	static void                         SemaphoreDestroy(NSemaphoreRef theSemaphore);
	static bool                         SemaphoreWait(   NSemaphoreRef theSemaphore, NInterval waitFor);
	static void                         SemaphoreSignal( NSemaphoreRef theSemaphore);


	// System
	static size_t                       SystemPageSize();
	static NVersion                     SystemVersion();
	static NString                      SystemName(NOSName theName);


	// Threads
	static bool                         ThreadIsMain();
	static size_t                       ThreadStackSize();
	static NVectorUInt8                 ThreadGetCores();
	static void                         ThreadSetCores(const NVectorUInt8& theCores);


	// Time
	static NTime                        TimeGet();
	static NInterval                    TimeGetUpTime();
	static uint64_t                     TimeGetClockTicks();
	static uint64_t                     TimeGetClockFrequency();
};



#endif // NCOMMON_DARWIN_H
