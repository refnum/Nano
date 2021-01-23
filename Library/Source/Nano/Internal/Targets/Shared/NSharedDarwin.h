/*	NAME:
		NSharedDarwin.h

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
#ifndef NSHARED_DARWIN_H
#define NSHARED_DARWIN_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NFileInfo.h"
#include "NFilePath.h"
#include "NFileUtils.h"
#include "NMachine.h"
#include "NProcess.h"
#include "NRunLoop.h"
#include "NSemaphore.h"
#include "NString.h"
#include "NTime.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NSharedDarwin
{
public:
	// Time
	static NTime                        GetTime();
	static NInterval                    GetUpTime();
	static uint64_t                     GetClockTicks();
	static uint64_t                     GetClockFrequency();


	// Debugger
	static bool                         DebuggerIsActive();
	static NVectorString                DebuggerGetBacktrace(size_t skipFrames, size_t numFrames);


	// Get file state
	static bool                         GetFileState(const NFilePath& thePath,
													 NFileInfoFlags   theFlags,
													 NFileInfoFlags&  validState,
													 NFileInfoState&  theState);


	// File paths
	static NStatus                      PathRename(  const NFilePath& oldPath, const NFilePath& newPath);
	static NStatus                      PathExchange(const NFilePath& oldPath, const NFilePath& newPath);
	static NFilePath                    PathLocation(NFileLocation theLocation);


	// Threads
	static bool                         ThreadIsMain();
	static size_t                       ThreadStackSize();
	static NVectorUInt8                 ThreadGetCores();
	static void                         ThreadSetCores(const NVectorUInt8& theCores);


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


	// Process
	static NString                      ProcessName();
	static NMemoryInfo                  ProcessMemory();


	// System
	static size_t                       SystemPageSize();


	// Machine
	static size_t                       MachineCores(NCoreType theType);
	static uint64_t                     MachineMemory();
	static NString                      MachineCPUName();
	static NString                      MachineCPUVendor();
	static uint64_t                     MachineCPUHertz();
};



#endif // NSHARED_DARWIN_H
