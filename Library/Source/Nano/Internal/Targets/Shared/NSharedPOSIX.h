/*	NAME:
		NSharedPOSIX.h

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
#ifndef NSHARED_POSIX_H
#define NSHARED_POSIX_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFile.h"
#include "NFileHandle.h"
#include "NFileInfo.h"
#include "NTime.h"

// System
#include <sys/stat.h>
#include <sys/time.h>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
class NString;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NSharedPOSIX
{
public:
	// Get the time of day
	//
	// Returns gettimeofday() in the Nano epoch.
	static NTime                        gettimeofday();


	// Get a clock time
	//
	// Returns clock_gettime for the specified clock.
	static NInterval                    clock_gettime(clockid_t theID);


	// Get a POSIX error
	//
	// GetErrno will return NStatus::OK if passed 0, or the
	// current value of errno translated to an NStatus.
	static NStatus                      ToStatus(int sysErr);
	static NStatus                      GetErrno(int sysErr = -1);


	// Get an access() state
	static void                         GetFileStateAccess(const NString&  thePath,
														   NFileInfoFlags  theFlag,
														   NFileInfoState& theState);


	// File handle
	static NStatus                      FileOpen(const NString&  thePath,
												 NFileAccess     theAccess,
												 NFileHandleRef& fileHandle);

	static void                         FileClose(NFileHandleRef fileHandle);

	static uint64_t                     FileGetPosition(NFileHandleRef fileHandle);

	static NStatus                      FileSetPosition(NFileHandleRef fileHandle,
														int64_t        thePosition,
														NFileOffset    relativeTo);

	static NStatus                      FileSetSize(NFileHandleRef fileHandle, uint64_t theSize);

	static NStatus                      FileRead(NFileHandleRef fileHandle,
												 uint64_t       theSize,
												 void*          thePtr,
												 uint64_t&      numRead);

	static NStatus                      FileWrite(NFileHandleRef fileHandle,
												  uint64_t       theSize,
												  const void*    thePtr,
												  uint64_t&      numWritten);

	static NStatus                      FileFlush(NFileHandleRef fileHandle);


	// Get the children of a directory
	static NVectorFile                  GetChildren(const NString& thePath);


	// Create a directory
	static NStatus                      CreateDirectory(const NString& thePath);


	// Delete a file
	static NStatus                      Delete(const NString& thePath);


	// Convert between time formats
	//
	// No epoch conversion is performed on intervals.
	static struct timeval               ToTimeval(NInterval theInterval);

	static NInterval                    ToInterval(const struct timeval& timeVal);
};



#endif // NSHARED_POSIX_H
