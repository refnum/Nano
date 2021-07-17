/*	NAME:
		NCommonLinux.h

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
#ifndef NCOMMON_LINUX_H
#define NCOMMON_LINUX_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFileInfo.h"
#include "NProcess.h"
#include "NSemaphore.h"
#include "NString.h"





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NDictionary;
class NFile;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NCommonLinux
{
public:
	// Get a /proc file
	static NString                      GetProcFile(const NFilePath& thePath);


	// Bundles
	static NFile                        BundleGet(              const NString& bundleID);
	static NFile                        BundleGetResources(     const NFile& theBundle);
	static NDictionary                  BundleGetInfoDictionary(const NFile& theBundle);
	static NFile                        BundleGetExecutable(    const NFile& theBundle, const NString& theName);


	// Debugger
	static bool                         DebuggerIsActive();


	// File state
	static bool                         FileGetState(const NFilePath& thePath,
													 NFileInfoFlags   theFlags,
													 NFileInfoFlags&  validState,
													 NFileInfoState&  theState);


	// Machine
	static uint64_t                     MachineMemory();
	static NString                      MachineCPUName();
	static NString                      MachineCPUVendor();
	static uint64_t                     MachineCPUHertz();


	// File paths
	static NStatus                      PathRename(  const NFilePath& oldPath, const NFilePath& newPath);
	static NStatus                      PathExchange(const NFilePath& oldPath, const NFilePath& newPath);


	// Process
	static NString                      ProcessName();
	static NMemoryInfo                  ProcessMemory();


	// Random
	static void                         RandomSecureData(size_t theSize, void* thePtr);


	// Semaphores
	static NSemaphoreRef                SemaphoreCreate(size_t theValue);
	static void                         SemaphoreDestroy(NSemaphoreRef theSemaphore);
	static bool                         SemaphoreWait(   NSemaphoreRef theSemaphore, NInterval waitFor);
	static void                         SemaphoreSignal( NSemaphoreRef theSemaphore);


	// System
	static size_t                       SystemPageSize();


	// Threads
	static size_t                       ThreadStackSize();
	static NVectorUInt8                 ThreadGetCores();
	static void                         ThreadSetCores(const NVectorUInt8& theCores);


	// Time
	static NTime                        TimeGet();
	static NInterval                    TimeGetUpTime();
	static uint64_t                     TimeGetClockTicks();
	static uint64_t                     TimeGetClockFrequency();
};



#endif // NCOMMON_LINUX_H
