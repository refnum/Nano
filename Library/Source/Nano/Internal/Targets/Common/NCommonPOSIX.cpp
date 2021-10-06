/*	NAME:
		NCommonPOSIX.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NCommonPOSIX.h"

// Nano
#include "NDebug.h"
#include "NFileUtils.h"
#include "NFormat.h"
#include "NString.h"
#include "NSystem.h"
#include "NTimeUtils.h"

// System
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// AT_EACCESS
//
// Android does not support AT_EACCESS.
#if NN_TARGET_ANDROID
inline constexpr int NN_AT_EACCESS                          = 0;
#else
inline constexpr int    NN_AT_EACCESS                       = AT_EACCESS;
#endif


// Thread name
#if NN_PLATFORM_DARWIN
inline constexpr size_t kNThreadNameMax                     = 64;
#else
inline constexpr size_t kNThreadNameMax                     = 16;
#endif // NN_PLATFORM_DARWIN





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetFileAccess : Get a fopen access mode.
//-----------------------------------------------------------------------------
static constexpr const char* GetFileAccess(NFileAccess theAccess)
{


	// Get the permission
	switch (theAccess)
	{
		case NFileAccess::ReadOnly:
			return "r";
			break;

		case NFileAccess::WriteOnly:
			return "w";
			break;

		case NFileAccess::ReadWrite:
			return "w+";
			break;

		case NFileAccess::WriteAtomic:
			return "w";
			break;
	}

	return nullptr;
}





//=============================================================================
//		GetFileWhence : Get an fseeko whence value.
//-----------------------------------------------------------------------------
static constexpr int GetFileWhence(NFileOffset relativeTo)
{


	// Get the permission
	switch (relativeTo)
	{
		case NFileOffset::FromStart:
			return SEEK_SET;
			break;

		case NFileOffset::FromPosition:
			return SEEK_CUR;
			break;

		case NFileOffset::FromEnd:
			return SEEK_END;
			break;
	}

	return SEEK_END;
}





//=============================================================================
//		ApplyFileFlags : Apply a file's flags.
//-----------------------------------------------------------------------------
static NStatus ApplyFileFlags(FILE* theFile, const NFilePath& thePath, NFileUsageFlags theFlags)
{


	// Validate our parameters
	NN_REQUIRE_NOT_NULL(theFile);
	NN_REQUIRE(thePath.IsValid());


	// Get the state we need
	NStatus theErr   = NStatus::OK;
	int     fileDesc = fileno(theFile);
	int     sysErr   = 0;



	// Cache
	if (theErr == NStatus::OK)
	{
#if NN_PLATFORM_DARWIN
		if (theFlags & NFileUsage::NoCache)
		{
			sysErr = fcntl(fileDesc, F_NOCACHE, 1);
		}
		else
		{
			sysErr = fcntl(fileDesc, F_NOCACHE, 0);
		}

#elif (NN_TARGET_LINUX || NN_TARGET_ANDROID)
		if (theFlags & NFileUsage::NoCache)
		{
			sysErr = posix_fadvise(fileDesc, 0, 0, POSIX_FADV_DONTNEED);
		}
#endif

		theErr = NCommonPOSIX::StatusErrno(sysErr);
		NN_EXPECT_NOT_ERR(theErr);
	}



	// Advisory read
	if (theErr == NStatus::OK)
	{
#if NN_PLATFORM_DARWIN
		if (theFlags & NFileUsage::ReadEarly)
		{
			struct stat theInfo;

			sysErr = fstat(fileDesc, &theInfo);
			if (sysErr == 0 && theInfo.st_size != 0)
			{
				struct radvisory readAdvisory;

				readAdvisory.ra_offset = 0;
				readAdvisory.ra_count =
					int(std::min(off_t(std::numeric_limits<int>::max()), theInfo.st_size));

				sysErr = fcntl(fileDesc, F_RDADVISE, &readAdvisory);
			}
		}

#elif (NN_TARGET_LINUX || NN_TARGET_ANDROID)
		if (theFlags & NFileUsage::ReadEarly)
		{
			sysErr = posix_fadvise(fileDesc, 0, 0, POSIX_FADV_WILLNEED);
		}
#endif

		theErr = NCommonPOSIX::StatusErrno(sysErr);
		NN_EXPECT_NOT_ERR(theErr);
	}



	// Read-ahead
	if (theErr == NStatus::OK)
	{
#if NN_PLATFORM_DARWIN
		if (theFlags & NFileUsage::ReadAhead)
		{
			sysErr = fcntl(fileDesc, F_RDAHEAD, 1);
		}
		else
		{
			sysErr = fcntl(fileDesc, F_RDAHEAD, 0);
		}

#elif (NN_TARGET_LINUX || NN_TARGET_ANDROID)
		if (theFlags & NFileUsage::ReadAhead)
		{
			sysErr = posix_fadvise(fileDesc, 0, 0, POSIX_FADV_SEQUENTIAL);
		}
		else
		{
			sysErr = posix_fadvise(fileDesc, 0, 0, POSIX_FADV_RANDOM);
		}
#endif

		theErr = NCommonPOSIX::StatusErrno(sysErr);
		NN_EXPECT_NOT_ERR(theErr);
	}

	return theErr;
}





//=============================================================================
//		NThreadEntry : Thread entry point.
//-----------------------------------------------------------------------------
static void* NThreadEntry(void* theParam)
{


	// Get the state we need
	NThreadContext* theContext = reinterpret_cast<NThreadContext*>(theParam);



	// Invoke the thread
	theContext->threadEntry();

	delete theContext;
	return nullptr;
}





#pragma mark NCommonPOSIX
//=============================================================================
//		NCommonPOSIX::EnvGet : Get an environment variable.
//-----------------------------------------------------------------------------
NString NCommonPOSIX::EnvGet(const NString& theName)
{


	// Valiate our parameters
	NN_REQUIRE(!theName.IsEmpty());



	// Get the variable
	NString       theValue;
	const utf8_t* envVar = getenv(theName.GetUTF8());

	if (envVar != nullptr)
	{
		theValue = NString(envVar);
	}

	return theValue;
}





//=============================================================================
//		NCommonPOSIX::EnvSet : Set an environment variable.
//-----------------------------------------------------------------------------
void NCommonPOSIX::EnvSet(const NString& theName, const NString& theValue)
{


	// Valiate our parameters
	NN_REQUIRE(!theName.IsEmpty());



	// Set the variable
	if (theValue.IsEmpty())
	{
		int sysErr = unsetenv(theName.GetUTF8());
		NN_EXPECT_NOT_ERR(sysErr);
	}
	else
	{
		int sysErr = setenv(theName.GetUTF8(), theValue.GetUTF8(), 1);
		NN_EXPECT_NOT_ERR(sysErr);
	}
}





//=============================================================================
//		NCommonPOSIX::FileGetStateAccess : Get file state with access().
//-----------------------------------------------------------------------------
void NCommonPOSIX::FileGetStateAccess(const NFilePath& thePath,
									  NFileStateFlags  theFlag,
									  NFileInfoState&  theState)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());

	NN_REQUIRE(theFlag == NFileStateFlags(NFileState::CanRead) ||
			   theFlag == NFileStateFlags(NFileState::CanWrite) ||
			   theFlag == NFileStateFlags(NFileState::CanExecute));



	// Get the state we need
	int theMode = -1;

	if (theFlag & NFileState::CanRead)
	{
		theMode = R_OK;
	}

	else if (theFlag & NFileState::CanWrite)
	{
		theMode = W_OK;
	}

	else if (theFlag & NFileState::CanExecute)
	{
		theMode = X_OK;
	}

	else
	{
		NN_LOG_ERROR("Unknown file info mode!");
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
			theState.theFlags &= NFileStateFlags(~theFlag);
		}
	}
}





//=============================================================================
//		NCommonPOSIX::FileOpen : Open a file.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::FileOpen(const NFilePath& thePath,
							   NFileAccess      theAccess,
							   NFileUsageFlags  theFlags,
							   NFileHandleRef&  fileHandle)
{


	// Validate our parameters and state
	NN_REQUIRE(thePath.IsValid());

	static_assert(sizeof(NFileHandleRef) >= sizeof(FILE*));



	// Open the file
	NStatus theErr  = NStatus::OK;
	FILE*   theFile = fopen(thePath.GetUTF8(), GetFileAccess(theAccess));

	if (theFile == nullptr)
	{
		theErr = StatusErrno();
	}
	else
	{
		theErr = ApplyFileFlags(theFile, thePath, theFlags);
		if (theErr == NStatus::OK)
		{
			fileHandle = static_cast<NFileHandleRef>(theFile);
		}
	}

	return theErr;
}





//=============================================================================
//		NCommonPOSIX::FileClose : Close a file.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::FileClose(NFileHandleRef fileHandle)
{


	// Get the state we need
	FILE* theFile = static_cast<FILE*>(fileHandle);


	// Close the file
	int sysErr = fclose(theFile);
	NN_EXPECT_NOT_ERR(sysErr);

	return StatusErrno(sysErr);
}





//=============================================================================
//		NCommonPOSIX::FileGetPosition : Get the file position.
//-----------------------------------------------------------------------------
uint64_t NCommonPOSIX::FileGetPosition(NFileHandleRef fileHandle)
{


	// Validate our state
	static_assert(sizeof(off_t) == sizeof(uint64_t));



	// Get the state we need
	FILE* theFile = static_cast<FILE*>(fileHandle);



	// Get the position
	off_t thePosition = ftello(theFile);
	NN_REQUIRE(thePosition >= 0);

	return uint64_t(thePosition);
}





//=============================================================================
//		NCommonPOSIX::FileSetPosition : Set the file position.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::FileSetPosition(NFileHandleRef fileHandle,
									  int64_t        thePosition,
									  NFileOffset    relativeTo)
{


	// Validate our state
	static_assert(sizeof(off_t) == sizeof(int64_t));



	// Get the state we need
	FILE* theFile = static_cast<FILE*>(fileHandle);



	// Set the position
	int sysErr = fseeko(theFile, off_t(thePosition), GetFileWhence(relativeTo));
	NN_EXPECT_NOT_ERR(sysErr);

	return StatusErrno(sysErr);
}





//=============================================================================
//		NCommonPOSIX::FileSetSize : Set the file size.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::FileSetSize(NFileHandleRef fileHandle, uint64_t theSize)
{


	// Validate our parameters and state
	NN_REQUIRE(theSize <= uint64_t(kNInt64Max));

	static_assert(sizeof(off_t) == sizeof(int64_t));



	// Get the state we need
	FILE* theFile  = static_cast<FILE*>(fileHandle);
	int   fileDesc = fileno(theFile);



	// Set the file size
	int sysErr = ftruncate(fileDesc, off_t(theSize));
	NN_EXPECT_NOT_ERR(sysErr);

	return StatusErrno(sysErr);
}





//=============================================================================
//		NCommonPOSIX::FileRead : Read from a file.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::FileRead(NFileHandleRef fileHandle,
							   uint64_t       theSize,
							   void*          thePtr,
							   uint64_t&      sizeRead)
{


	// Validate our parameters
	static_assert(sizeof(uint64_t) == sizeof(size_t));



	// Get the state we need
	FILE*   theFile = static_cast<FILE*>(fileHandle);
	NStatus theErr  = NStatus::OK;



	// Read from the file
	sizeRead = fread(thePtr, 1, size_t(theSize), theFile);

	if (sizeRead != theSize)
	{
		if (feof(theFile))
		{
			theErr = NStatus::ExhaustedSrc;
		}
		else
		{
			theErr = StatusSysErr(ferror(theFile));
		}
	}

	return theErr;
}





//=============================================================================
//		NCommonPOSIX::FileWrite : Write to a file.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::FileWrite(NFileHandleRef fileHandle,
								uint64_t       theSize,
								const void*    thePtr,
								uint64_t&      sizeWritten)
{


	// Validate our parameters
	static_assert(sizeof(uint64_t) == sizeof(size_t));



	// Get the state we need
	FILE*   theFile = static_cast<FILE*>(fileHandle);
	NStatus theErr  = NStatus::OK;


	// Write to the file
	sizeWritten = fwrite(thePtr, 1, size_t(theSize), theFile);

	if (sizeWritten != theSize)
	{
		theErr = NStatus::DiskFull;
	}

	return theErr;
}





//=============================================================================
//		NCommonPOSIX::FileFlush : Flush the file.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::FileFlush(NFileHandleRef fileHandle)
{


	// Get the state we need
	FILE* theFile = static_cast<FILE*>(fileHandle);



	// Flush the file
	int sysErr = fflush(theFile);
	NN_EXPECT_NOT_ERR(sysErr);

	return StatusErrno(sysErr);
}





//=============================================================================
//		NCommonPOSIX::MapPageSize : Get the file mapping page size.
//-----------------------------------------------------------------------------
size_t NCommonPOSIX::MapPageSize()
{


	// Get the page size
	return NSystem::GetPageSize();
}





//=============================================================================
//		NCommonPOSIX::MapOpen : Open a file for mapping.
//-----------------------------------------------------------------------------
NFileMapRef NCommonPOSIX::MapOpen(const NFilePath& thePath, NMapAccess theAccess)
{


	// Validate our state
	static_assert(sizeof(uintptr_t) >= sizeof(int));



	// Open the file
	NFileMapRef theHandle;

	int theFlags = (theAccess == NMapAccess::ReadWrite ? O_RDWR : O_RDONLY);
	int theFD    = open(thePath.GetUTF8(), theFlags, 0);

	if (theFD != -1)
	{
		theHandle = uintptr_t(theFD);
	}

	return theHandle;
}





//=============================================================================
//		NCommonPOSIX::MapClose : Close a mapped file.
//-----------------------------------------------------------------------------
void NCommonPOSIX::MapClose(NFileMapRef theHandle)
{


	// Close the file
	int theFD = int(theHandle.value());
	NN_REQUIRE(theFD != -1);

	int sysErr = close(theFD);
	NN_EXPECT_NOT_ERR(sysErr);
}





//=============================================================================
//		NCommonPOSIX::MapFetch : Fetch mapped pages.
//-----------------------------------------------------------------------------
void NCommonPOSIX::MapFetch(NFileMapRef theHandle, NFileMapping& theMapping)
{


	// Get the state we need
	int theFD = int(theHandle.value());
	NN_REQUIRE(theFD != -1);

	int pagePerm  = PROT_READ;
	int pageFlags = MAP_FILE;

	if (theMapping.theAccess != NMapAccess::ReadOnly)
	{
		pagePerm |= PROT_WRITE;
	}

	if (theMapping.theAccess == NMapAccess::CopyOnWrite)
	{
		pageFlags |= MAP_PRIVATE;
	}
	else
	{
		pageFlags |= MAP_SHARED;
	}



	// Map the pages
	theMapping.thePtr = static_cast<const uint8_t*>(
		mmap(nullptr, theMapping.theSize, pagePerm, pageFlags, theFD, off_t(theMapping.theOffset)));
}





//=============================================================================
//		NCommonPOSIX::MapDiscard : Discard mapped pages.
//-----------------------------------------------------------------------------
void NCommonPOSIX::MapDiscard(NFileMapRef theHandle, const NFileMapping& theMapping)
{


	// Get the state we need
	int theFD = int(theHandle.value());
	NN_REQUIRE(theFD != -1);

	uint8_t* thePtr = const_cast<uint8_t*>(theMapping.thePtr);
	int      sysErr = 0;



	// Flush mutable pages
	if (theMapping.theAccess == NMapAccess::ReadWrite)
	{
		sysErr = msync(thePtr, theMapping.theSize, MS_SYNC);
		NN_EXPECT_NOT_ERR(sysErr);
	}



	// Discard the pages
	sysErr = munmap(thePtr, theMapping.theSize);
	NN_EXPECT_NOT_ERR(sysErr);
}





//=============================================================================
//		NCommonPOSIX::PathCreate : Create a path.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::PathCreate(const NFilePath& thePath)
{


	// Create the path
	int sysErr = mkdir(thePath.GetUTF8(), S_IRWXU | S_IRWXG);
	NN_EXPECT_NOT_ERR(sysErr);

	return StatusErrno(sysErr);
}





//=============================================================================
//		NCommonPOSIX::PathDelete : Delete a path.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::PathDelete(const NFilePath& thePath)
{


	// Get the state we need
	struct stat theInfo;

	const utf8_t* pathUTF8 = thePath.GetUTF8();
	int           sysErr   = stat(pathUTF8, &theInfo);
	NN_EXPECT_NOT_ERR(sysErr);



	// Delete the path
	if (sysErr == 0)
	{
		int dirFlag = S_ISDIR(theInfo.st_mode) ? AT_REMOVEDIR : 0;

		sysErr = unlinkat(0, pathUTF8, dirFlag);
		NN_EXPECT_NOT_ERR(sysErr);
	}

	return StatusErrno(sysErr);
}





//=============================================================================
//		NCommonPOSIX::PathChildren : Get the children of a path.
//-----------------------------------------------------------------------------
NVectorFilePath NCommonPOSIX::PathChildren(const NFilePath& thePath)
{


	// Open the directory
	NVectorFilePath theChildren;

	DIR* theDir = opendir(thePath.GetUTF8());
	NN_EXPECT(theDir != nullptr);

	if (theDir == nullptr)
	{
		return theChildren;
	}



	// Collect the files
	const struct dirent* dirEntry = nullptr;

	do
	{
		// Read the entry
		dirEntry = readdir(theDir);
		if (dirEntry != nullptr)
		{
			if (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0)
			{
				theChildren.emplace_back(thePath.GetChild(dirEntry->d_name));
			}
		}
	} while (dirEntry != nullptr);



	// Clean up
	closedir(theDir);

	return theChildren;
}





//=============================================================================
//		NCommonPOSIX::StatusSysErr : Convert an errno code to an NStatus.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::StatusSysErr(int sysErr)
{


	// Get the status
	NStatus theErr = NStatus::Internal;

	switch (sysErr)
	{
		case 0:
			// No error
			theErr = NStatus::OK;
			break;

		case EPERM:
			// Operation not permitted
			theErr = NStatus::Permission;
			break;

		case ENOENT:
			// No such file or directory
			theErr = NStatus::NotFound;
			break;

		case ESRCH:
			// No such process
			theErr = NStatus::NotFound;
			break;

		case EINTR:
			// Interrupted system call
			theErr = NStatus::Internal;
			break;

		case EIO:
			// Input/output error
			theErr = NStatus::Internal;
			break;

		case ENXIO:
			// Device not configured
			theErr = NStatus::Param;
			break;

		case E2BIG:
			// Argument list too long
			theErr = NStatus::Param;
			break;

		case ENOEXEC:
			// Exec format error
			theErr = NStatus::Param;
			break;

		case EBADF:
			// Bad file descriptor
			theErr = NStatus::NotFound;
			break;

		case ECHILD:
			// No child processes
			theErr = NStatus::NotFound;
			break;

		case EDEADLK:
			// Resource deadlock avoided
			theErr = NStatus::Busy;
			break;

		case ENOMEM:
			// Cannot allocate memory
			theErr = NStatus::Memory;
			break;

		case EACCES:
			// Permission denied
			theErr = NStatus::Permission;
			break;

		case EFAULT:
			// Bad address
			theErr = NStatus::NotFound;
			break;

		case ENOTBLK:
			// Block device required
			theErr = NStatus::NotSupported;
			break;

		case EBUSY:
			// Device / Resource busy
			theErr = NStatus::Busy;
			break;

		case EEXIST:
			// File exists
			theErr = NStatus::Permission;
			break;

		case EXDEV:
			// Cross-device link
			theErr = NStatus::Permission;
			break;

		case ENODEV:
			// Operation not supported by device
			theErr = NStatus::NotSupported;
			break;

		case ENOTDIR:
			// Not a directory
			theErr = NStatus::Param;
			break;

		case EISDIR:
			// Is a directory
			theErr = NStatus::Param;
			break;

		case EINVAL:
			// Invalid argument
			theErr = NStatus::Param;
			break;

		case ENFILE:
			// Too many open files in system
			theErr = NStatus::ExhaustedDst;
			break;

		case EMFILE:
			// Too many open files
			theErr = NStatus::ExhaustedDst;
			break;

		case ENOTTY:
			// Inappropriate ioctl for device
			theErr = NStatus::Param;
			break;

		case ETXTBSY:
			// Text file busy
			theErr = NStatus::Busy;
			break;

		case EFBIG:
			// File too large
			theErr = NStatus::ExhaustedDst;
			break;

		case ENOSPC:
			// No space left on device
			theErr = NStatus::DiskFull;
			break;

		case ESPIPE:
			// Illegal seek
			theErr = NStatus::Param;
			break;

		case EROFS:
			// Read-only file system
			theErr = NStatus::Permission;
			break;

		case EMLINK:
			// Too many links
			theErr = NStatus::ExhaustedDst;
			break;

		case EPIPE:
			// Broken pipe
			theErr = NStatus::Param;
			break;

		case EDOM:
			// Numerical argument out of domain
			theErr = NStatus::Param;
			break;

		case ERANGE:
			// Result too large
			theErr = NStatus::Param;
			break;

		case EAGAIN:
			// Resource temporarily unavailable
			theErr = NStatus::Busy;
			break;

		case EINPROGRESS:
			// Operation now in progress
			theErr = NStatus::Busy;
			break;

		case EALREADY:
			// Operation already in progress
			theErr = NStatus::Busy;
			break;

		case ENOTSOCK:
			// Socket operation on non-socket
			theErr = NStatus::Param;
			break;

		case EDESTADDRREQ:
			// Destination address required
			theErr = NStatus::Param;
			break;

		case EMSGSIZE:
			// Message too long
			theErr = NStatus::Param;
			break;

		case EPROTOTYPE:
			// Protocol wrong type for socket
			theErr = NStatus::Param;
			break;

		case ENOPROTOOPT:
			// Protocol not available
			theErr = NStatus::Param;
			break;

		case EPROTONOSUPPORT:
			// Protocol not supported
			theErr = NStatus::NotSupported;
			break;

		case ESOCKTNOSUPPORT:
			// Socket type not supported
			theErr = NStatus::NotSupported;
			break;

		case ENOTSUP:
			// Operation not supported
			theErr = NStatus::NotSupported;
			break;

		case EPFNOSUPPORT:
			// Protocol family not supported
			theErr = NStatus::NotSupported;
			break;

		case EAFNOSUPPORT:
			// Address family not supported by protocol family
			theErr = NStatus::NotSupported;
			break;

		case EADDRINUSE:
			// Address already in use
			theErr = NStatus::Busy;
			break;

		case EADDRNOTAVAIL:
			// Can't assign requested address
			theErr = NStatus::Param;
			break;

		case ENETDOWN:
			// Network is down
			theErr = NStatus::Param;
			break;

		case ENETUNREACH:
			// Network is unreachable
			theErr = NStatus::NotFound;
			break;

		case ENETRESET:
			// Network dropped connection on reset
			theErr = NStatus::Param;
			break;

		case ECONNABORTED:
			// Software caused connection abort
			theErr = NStatus::Param;
			break;

		case ECONNRESET:
			// Connection reset by peer
			theErr = NStatus::Param;
			break;

		case ENOBUFS:
			// No buffer space available
			theErr = NStatus::Param;
			break;

		case EISCONN:
			// Socket is already connected
			theErr = NStatus::Param;
			break;

		case ENOTCONN:
			// Socket is not connected
			theErr = NStatus::Param;
			break;

		case ESHUTDOWN:
			// Can't send after socket shutdown
			theErr = NStatus::Param;
			break;

		case ETOOMANYREFS:
			// Too many references: can't splice
			theErr = NStatus::Param;
			break;

		case ETIMEDOUT:
			// Operation timed out
			theErr = NStatus::Timeout;
			break;

		case ECONNREFUSED:
			// Connection refused
			theErr = NStatus::Param;
			break;

		case ELOOP:
			// Too many levels of symbolic links
			theErr = NStatus::Param;
			break;

		case ENAMETOOLONG:
			// File name too long
			theErr = NStatus::Param;
			break;

		case EHOSTDOWN:
			// Host is down
			theErr = NStatus::Param;
			break;

		case EHOSTUNREACH:
			// No route to host
			theErr = NStatus::Param;
			break;

		case ENOTEMPTY:
			// Directory not empty
			theErr = NStatus::Duplicate;
			break;

		case EUSERS:
			// Too many users
			theErr = NStatus::Param;
			break;

		case EDQUOT:
			// Disc quota exceeded
			theErr = NStatus::DiskFull;
			break;

		case ESTALE:
			// Stale NFS file handle
			theErr = NStatus::Param;
			break;

		case EREMOTE:
			// Too many levels of remote in path
			theErr = NStatus::Param;
			break;

		case ENOLCK:
			// No locks available
			theErr = NStatus::Param;
			break;

		case ENOSYS:
			// Function not implemented
			theErr = NStatus::Param;
			break;

		case EOVERFLOW:
			// Value too large to be stored in data type
			theErr = NStatus::Param;
			break;

		case ECANCELED:
			// Operation canceled
			theErr = NStatus::Cancelled;
			break;

		case EIDRM:
			// Identifier removed
			theErr = NStatus::Param;
			break;

		case ENOMSG:
			// No message of desired type
			theErr = NStatus::NotFound;
			break;

		case EILSEQ:
			// Illegal byte sequence
			theErr = NStatus::Malformed;
			break;

		case EBADMSG:
			// Bad message
			theErr = NStatus::Param;
			break;

		case EMULTIHOP:
			// Reserved
			theErr = NStatus::Param;
			break;

		case ENODATA:
			// No message available on STREAM
			theErr = NStatus::ExhaustedSrc;
			break;

		case ENOLINK:
			// Reserved
			theErr = NStatus::Param;
			break;

		case ENOSR:
			// No STREAM resources
			theErr = NStatus::Param;
			break;

		case ENOSTR:
			// Not a STREAM
			theErr = NStatus::Param;
			break;

		case EPROTO:
			// Protocol error
			theErr = NStatus::Param;
			break;

		case ETIME:
			// STREAM ioctl timeout
			theErr = NStatus::Timeout;
			break;

		case ENOTRECOVERABLE:
			// State not recoverable
			theErr = NStatus::Internal;
			break;

		case EOWNERDEAD:
			// Previous owner died
			theErr = NStatus::NotFound;
			break;

#if NN_PLATFORM_DARWIN
		case EPROCLIM:
			// Too many processes
			theErr = NStatus::Param;
			break;

		case EBADRPC:
			// RPC struct is bad
			theErr = NStatus::Param;
			break;

		case ERPCMISMATCH:
			// RPC version wrong
			theErr = NStatus::Param;
			break;

		case EPROGUNAVAIL:
			// RPC prog. not avail
			theErr = NStatus::Param;
			break;

		case EPROGMISMATCH:
			// Program version wrong
			theErr = NStatus::Param;
			break;

		case EPROCUNAVAIL:
			// Bad procedure for program
			theErr = NStatus::Param;
			break;

		case EFTYPE:
			// Inappropriate file type or format
			theErr = NStatus::Param;
			break;

		case EAUTH:
			// Authentication error
			theErr = NStatus::Permission;
			break;

		case ENEEDAUTH:
			// Need authenticator
			theErr = NStatus::Permission;
			break;

		case EPWROFF:
			// Device power is off
			theErr = NStatus::Param;
			break;

		case EDEVERR:
			// Device error, e.g. paper out
			theErr = NStatus::Param;
			break;

		case EBADEXEC:
			// Bad executable
			theErr = NStatus::Malformed;
			break;

		case EBADARCH:
			// Bad CPU type in executable
			theErr = NStatus::Malformed;
			break;

		case ESHLIBVERS:
			// Shared library version mismatch
			theErr = NStatus::Malformed;
			break;

		case EBADMACHO:
			// Malformed Macho file
			theErr = NStatus::Malformed;
			break;

		case ENOATTR:
			// Attribute not found
			theErr = NStatus::NotFound;
			break;

		case ENOPOLICY:
			// No such policy registered
			theErr = NStatus::Param;
			break;

		case EQFULL:
			// Interface output queue is full
			theErr = NStatus::ExhaustedDst;
			break;
#endif //NN_PLATFORM_DARWIN

		default:
			NN_LOG_UNIMPLEMENTED("Unknown error {}", sysErr);
			break;
	}

	return theErr;
}





//=============================================================================
//		NCommonPOSIX::StatusErrno : Get errno as an NStatus.
//-----------------------------------------------------------------------------
NStatus NCommonPOSIX::StatusErrno(int sysErr)
{


	// Get the value
	NStatus theErr = NStatus::OK;

	if (sysErr != 0)
	{
		theErr = StatusSysErr(errno);
	}

	return theErr;
}





//=============================================================================
//		NCommonPOSIX::ThreadCreate : Create a native thread.
//-----------------------------------------------------------------------------
NThreadHandle NCommonPOSIX::ThreadCreate(NThreadContext* theContext)
{


	// Validate our state
	static_assert(sizeof(NThreadHandle) >= sizeof(pthread_t));



	// Create the attributes
	pthread_t      threadID;
	pthread_attr_t threadAttrs;

	int sysErr = pthread_attr_init(&threadAttrs);
	NN_EXPECT_NOT_ERR(sysErr);

	if (theContext->stackSize != 0)
	{
		sysErr = pthread_attr_setstacksize(&threadAttrs, theContext->stackSize);
		NN_EXPECT_NOT_ERR(sysErr);
	}



	// Create the thread
	sysErr = pthread_create(&threadID, &threadAttrs, NThreadEntry, theContext);
	NN_EXPECT_NOT_ERR(sysErr);

	sysErr = pthread_attr_destroy(&threadAttrs);
	NN_EXPECT_NOT_ERR(sysErr);

	return NThreadHandle(threadID);
}





//=============================================================================
//		NCommonPOSIX::ThreadJoin : Join a native thread.
//-----------------------------------------------------------------------------
void NCommonPOSIX::ThreadJoin(NThreadHandle theThread)
{


	// Join the thread
	pthread_t threadID = pthread_t(theThread);

	int sysErr = pthread_join(threadID, nullptr);
	NN_EXPECT(sysErr == 0 || sysErr == ESRCH);
}





//=============================================================================
//		NCommonPOSIX::ThreadGetName : Get the current thread's name.
//-----------------------------------------------------------------------------
NString NCommonPOSIX::ThreadGetName()
{


	// Get the thread name
	char theBuffer[NAME_MAX]{};

	int sysErr = pthread_getname_np(pthread_self(), theBuffer, sizeof(theBuffer));
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		theBuffer[0] = 0x00;
	}

	return NString(NStringEncoding::UTF8, strlen(theBuffer), theBuffer);
}





//=============================================================================
//		NCommonPOSIX::ThreadSetName : Set the current thread's name.
//-----------------------------------------------------------------------------
void NCommonPOSIX::ThreadSetName(const NString& theName)
{


	// Prepare the name
	//
	// The supplied name is truncated to fit within the maximum thread
	// name length supported by this platform.
	char theBuffer[kNThreadNameMax]{};

	const char* textUTF8 = theName.GetUTF8();
	size_t      textLen  = std::min(strlen(textUTF8), kNThreadNameMax - 1);

	memcpy(theBuffer, textUTF8, textLen);

	if constexpr (NN_ENABLE_LOGGING)
	{
		if (strlen(textUTF8) > textLen)
		{
			NN_LOG_WARNING("NCommonPOSIX::ThreadSetName truncated '{}' to '{}'",
						   theName,
						   theBuffer);
		}
	}



	// Set the thread name
#if NN_PLATFORM_DARWIN
	int sysErr = pthread_setname_np(theBuffer);
	NN_EXPECT_NOT_ERR(sysErr);
#else
	int sysErr = pthread_setname_np(pthread_self(), theBuffer);
	NN_EXPECT_NOT_ERR(sysErr);
#endif // NN_PLATFORM_DARWIN
}





//=============================================================================
//		NCommonPOSIX::ThreadGetPriority : Get the current thread's priority.
//-----------------------------------------------------------------------------
float NCommonPOSIX::ThreadGetPriority()
{


	// Get the state we need
	struct sched_param schedParams;
	float              thePriority = 0.5f;
	int                schedPolicy = 0;

	int sysErr = pthread_getschedparam(pthread_self(), &schedPolicy, &schedParams);
	NN_EXPECT_NOT_ERR(sysErr);



	// Get the priority
	if (sysErr == 0)
	{
		// Get the range
		int valueMin = sched_get_priority_min(schedPolicy);
		int valueMax = sched_get_priority_max(schedPolicy);

		NN_REQUIRE(schedParams.sched_priority >= valueMin);
		NN_REQUIRE(schedParams.sched_priority <= valueMax);
		NN_REQUIRE(valueMax >= valueMin);



		// Calculate the priority
		if (valueMin != valueMax)
		{
			thePriority = float(schedParams.sched_priority - valueMin) / float(valueMax - valueMin);
			NN_REQUIRE(thePriority >= 0.0f && thePriority <= 1.0f);
		}
	}

	return thePriority;
}





//=============================================================================
//		NCommonPOSIX::ThreadSetPriority : Set the current thread's priority.
//-----------------------------------------------------------------------------
void NCommonPOSIX::ThreadSetPriority(float thePriority)
{


	// Validate our parameters
	NN_REQUIRE(thePriority >= 0.0f && thePriority <= 1.0f);



	// Get the state we need
	struct sched_param schedParams;
	int                schedPolicy = 0;

	int sysErr = pthread_getschedparam(pthread_self(), &schedPolicy, &schedParams);
	NN_EXPECT_NOT_ERR(sysErr);



	// Set the priority
	if (sysErr == 0)
	{
		// Adjust the policy
		//
		// On Darwin we can switch to SCHED_RR without additional privileges.
		if constexpr (NN_PLATFORM_DARWIN)
		{
			schedPolicy = SCHED_RR;
		}



		// Calculate the priority
		int valueMin = sched_get_priority_min(schedPolicy);
		int valueMax = sched_get_priority_max(schedPolicy);
		NN_REQUIRE(valueMax >= valueMin);

		schedParams.sched_priority = valueMin + int(thePriority * float(valueMax - valueMin));
		NN_REQUIRE(schedParams.sched_priority >= valueMin);
		NN_REQUIRE(schedParams.sched_priority <= valueMax);



		// Set the priority
		sysErr = pthread_setschedparam(pthread_self(), schedPolicy, &schedParams);
		NN_EXPECT_NOT_ERR(sysErr);
	}
}





//=============================================================================
//		NCommonPOSIX::TimeGetTimeval : Convert to a timeval.
//-----------------------------------------------------------------------------
struct timeval NCommonPOSIX::TimeGetTimeval(NInterval theInterval)
{
	// Convert the value
	NInterval timeSecs = floor(theInterval);
	NInterval timeFrac = theInterval - timeSecs;

	return {time_t(timeSecs), suseconds_t(timeFrac / kNTimeMicrosecond)};
}





//=============================================================================
//		NCommonPOSIX::TimeGetInterval : Convert to an NInterval.
//-----------------------------------------------------------------------------
NInterval NCommonPOSIX::TimeGetInterval(const struct timeval& timeVal)
{


	// Convert the value
	NInterval timeSecs = NInterval(timeVal.tv_sec);
	NInterval timeFrac = NInterval(timeVal.tv_usec) * kNTimeMicrosecond;

	return timeSecs + timeFrac;
}
