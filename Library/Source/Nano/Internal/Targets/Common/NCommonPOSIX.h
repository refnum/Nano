/*	NAME:
		NCommonPOSIX.h

	DESCRIPTION:
		POSIX support.

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
#ifndef NCOMMON_POSIX_H
#define NCOMMON_POSIX_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFile.h"
#include "NFileHandle.h"
#include "NFileInfo.h"
#include "NFileMap.h"
#include "NThread.h"
#include "NTime.h"

// System
#include <sys/stat.h>
#include <sys/time.h>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NString;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NCommonPOSIX
{
public:
	// Environment
	static NString                      EnvGet(const NString& theName);
	static void                         EnvSet(const NString& theName, const NString& theValue);


	// File state
	static void                         FileGetStateAccess(const NFilePath& thePath,
														   NFileStateFlags  theFlag,
														   NFileInfoState&  theState);


	// File handle
	static NStatus                      FileOpen(const NFilePath& thePath,
												 NFileAccess      theAccess,
												 NFileUsageFlags  theFlags,
												 NFileHandleRef&  fileHandle);

	static void                         FileClose(NFileHandleRef fileHandle);

	static uint64_t                     FileGetPosition(NFileHandleRef fileHandle);

	static NStatus                      FileSetPosition(NFileHandleRef fileHandle,
														int64_t        thePosition,
														NFileOffset    relativeTo);

	static NStatus                      FileSetSize(NFileHandleRef fileHandle, uint64_t theSize);

	static NStatus                      FileRead(NFileHandleRef fileHandle,
												 uint64_t       theSize,
												 void*          thePtr,
												 uint64_t&      sizeRead);

	static NStatus                      FileWrite(NFileHandleRef fileHandle,
												  uint64_t       theSize,
												  const void*    thePtr,
												  uint64_t&      sizeWritten);

	static NStatus                      FileFlush(NFileHandleRef fileHandle);


	// File maps
	static size_t                       MapPageSize();
	static NFileMapRef                  MapOpen(const NFilePath& thePath, NMapAccess theAccess);
	static void                         MapClose(  NFileMapRef theHandle);
	static void                         MapFetch(  NFileMapRef theHandle,       NFileMapping& theMapping);
	static void                         MapDiscard(NFileMapRef theHandle, const NFileMapping& theMapping);


	// File paths
	static NStatus                      PathCreate(  const NFilePath& thePath);
	static NStatus                      PathDelete(  const NFilePath& thePath);
	static NVectorFilePath              PathChildren(const NFilePath& thePath);


	// Error conversion
	//
	// StatusErrno returns NStatus::OK if passed 0, otherwise it
	// returns the current value of errno as an NStatus.
	static NStatus                      StatusSysErr(int sysErr);
	static NStatus                      StatusErrno( int sysErr = -1);


	// Threads
	static NThreadHandle                ThreadCreate(NThreadContext* theContext);
	static void                         ThreadJoin(NThreadHandle theThread);
	static NString                      ThreadGetName();
	static void                         ThreadSetName(const NString& theName);
	static float                        ThreadGetPriority();
	static void                         ThreadSetPriority(float thePriority);


	// Time conversion
	//
	// No epoch conversion is performed on intervals.
	static struct timeval               TimeGetTimeval(NInterval theInterval);
	static NInterval                    TimeGetInterval(const struct timeval& timeVal);
};



#endif // NCOMMON_POSIX_H
