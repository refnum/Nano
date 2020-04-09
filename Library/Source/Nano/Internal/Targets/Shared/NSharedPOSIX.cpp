/*	NAME:
		NSharedPOSIX.cpp

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NSharedPOSIX.h"

// Nano
#include "NDebug.h"
#include "NFileUtils.h"
#include "NString.h"
#include "NTimeUtils.h"

// System
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// AT_EACCESS
//
// Android does not support AT_EACCESS.
#if NN_TARGET_ANDROID
static constexpr int NN_AT_EACCESS                          = 0;
#else
static constexpr int NN_AT_EACCESS                          = AT_EACCESS;
#endif





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
		case NFileAccess::ReadWrite:
			return "w+";
			break;

		case NFileAccess::ReadOnly:
			return "r";
			break;

		case NFileAccess::WriteOnly:
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
static NStatus ApplyFileFlags(FILE* theFile, const NFilePath& thePath, NFileFlags theFlags)
{


	// Validate our parameters
	NN_REQUIRE_NOT_NULL(theFile);
	NN_REQUIRE(thePath.IsValid());


	// Get the state we need
	NStatus theErr   = NStatus::OK;
	int     fileDesc = fileno(theFile);
	int     sysErr   = 0;



	// Read-ahead
	if (theErr == NStatus::OK && (theFlags & kNFileWillRead))
	{
#if NN_PLATFORM_DARWIN
		struct radvisory readAhead
		{
		};
		struct stat theInfo
		{
		};

		sysErr = fstat(fileDesc, &theInfo);
		if (sysErr == 0 && theInfo.st_size != 0)
		{
			readAhead.ra_count =
				int(std::min(off_t(std::numeric_limits<int>::max()), theInfo.st_size));

			sysErr = fcntl(fileDesc, F_RDADVISE, &readAhead);
		}

#elif (NN_TARGET_LINUX || NN_TARGET_ANDROID)
		sysErr = posix_fadvise(fileDesc, 0, 0, POSIX_FADV_WILLNEED);
#endif

		theErr = NSharedPOSIX::GetErrno(sysErr);
		NN_EXPECT_NOT_ERR(theErr);
	}



	// Access
	if (theErr == NStatus::OK)
	{
		if (theFlags & kNFilePositionSequential)
		{
#if NN_PLATFORM_DARWIN
			sysErr = fcntl(fileDesc, F_RDAHEAD, 1);

#elif (NN_TARGET_LINUX || NN_TARGET_ANDROID)
			sysErr = posix_fadvise(fileDesc, 0, 0, POSIX_FADV_SEQUENTIAL);
#endif
		}

		else if (theFlags & kNFilePositionRandom)
		{
#if NN_PLATFORM_DARWIN
			sysErr = fcntl(fileDesc, F_RDAHEAD, 0);

#elif (NN_TARGET_LINUX || NN_TARGET_ANDROID)
			sysErr = posix_fadvise(fileDesc, 0, 0, POSIX_FADV_RANDOM);
#endif
		}

		theErr = NSharedPOSIX::GetErrno(sysErr);
		NN_EXPECT_NOT_ERR(theErr);
	}

	return theErr;
}





#pragma mark NSharedPOSIX
//=============================================================================
//		NSharedPOSIX::gettimeofday : Get the time of day.
//-----------------------------------------------------------------------------
NTime NSharedPOSIX::gettimeofday()
{


	// Get the time of day
	struct timeval timeVal = {};

	int sysErr = ::gettimeofday(&timeVal, nullptr);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeVal, 0x00, sizeof(timeVal));
	}



	// Get the time
	return NTime(ToInterval(timeVal), kNanoEpochFrom1970);
}





//=============================================================================
//		NSharedPOSIX::clock_gettime : Get a clock time.
//-----------------------------------------------------------------------------
NInterval NSharedPOSIX::clock_gettime(clockid_t theID)
{


	// Get the clock time
	struct timespec timeSpec = {};

	int sysErr = ::clock_gettime(theID, &timeSpec);
	NN_EXPECT_NOT_ERR(sysErr);

	if (sysErr != 0)
	{
		memset(&timeSpec, 0x00, sizeof(timeSpec));
	}



	// Get the time
	return NTimeUtils::ToInterval(timeSpec);
}





//=============================================================================
//		NSharedPOSIX::ToStatus : Convert an errno to an NStatus.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::ToStatus(int sysErr)
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
//		NSharedPOSIX::GetErrno : Get errno as an NStatus.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::GetErrno(int sysErr)
{


	// Get the value
	NStatus theErr = NStatus::OK;

	if (sysErr != 0)
	{
		theErr = ToStatus(errno);
	}

	return theErr;
}





//=============================================================================
//		NSharedPOSIX::GetFileStateAccess : Get file state with access().
//-----------------------------------------------------------------------------
void NSharedPOSIX::GetFileStateAccess(const NFilePath& thePath,
									  NFileInfoFlags   theFlag,
									  NFileInfoState&  theState)
{


	// Validate our parameters
	NN_REQUIRE(thePath.IsValid());

	NN_REQUIRE(theFlag == kNFileInfoCanRead || theFlag == kNFileInfoCanWrite ||
			   theFlag == kNFileInfoCanExecute);



	// Get the state we need
	int theMode = -1;

	switch (theFlag)
	{
		case kNFileInfoCanRead:
			theMode = R_OK;
			break;

		case kNFileInfoCanWrite:
			theMode = W_OK;
			break;

		case kNFileInfoCanExecute:
			theMode = X_OK;
			break;

		default:
			NN_LOG_ERROR("Unknown file info mode: {}", theFlag);
			break;
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
			theState.theFlags &= ~theFlag;
		}
	}
}





//=============================================================================
//		NSharedPOSIX::FileOpen : Open a file.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::FileOpen(const NFilePath& thePath,
							   NFileAccess      theAccess,
							   NFileFlags       theFlags,
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
		theErr = GetErrno();
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
//		NSharedPOSIX::FileClose : Close a file.
//-----------------------------------------------------------------------------
void NSharedPOSIX::FileClose(NFileHandleRef fileHandle)
{


	// Get the state we need
	FILE* theFile = static_cast<FILE*>(fileHandle);


	// Close the file
	int sysErr = fclose(theFile);
	NN_EXPECT_NOT_ERR(sysErr);
}





//=============================================================================
//		NSharedPOSIX::FileGetPosition : Get the file position.
//-----------------------------------------------------------------------------
uint64_t NSharedPOSIX::FileGetPosition(NFileHandleRef fileHandle)
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
//		NSharedPOSIX::FileSetPosition : Set the file position.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::FileSetPosition(NFileHandleRef fileHandle,
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

	return GetErrno(sysErr);
}





//=============================================================================
//		NSharedPOSIX::FileSetSize : Set the file size.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::FileSetSize(NFileHandleRef fileHandle, uint64_t theSize)
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

	return GetErrno(sysErr);
}





//=============================================================================
//		NSharedPOSIX::FileRead : Read from a file.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::FileRead(NFileHandleRef fileHandle,
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
			theErr = ToStatus(ferror(theFile));
		}
	}

	return theErr;
}





//=============================================================================
//		NSharedPOSIX::FileWrite : Write to a file.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::FileWrite(NFileHandleRef fileHandle,
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
//		NSharedPOSIX::FileFlush : Flush the file.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::FileFlush(NFileHandleRef fileHandle)
{


	// Get the state we need
	FILE* theFile = static_cast<FILE*>(fileHandle);



	// Flush the file
	int sysErr = fflush(theFile);
	NN_EXPECT_NOT_ERR(sysErr);

	return GetErrno(sysErr);
}





//=============================================================================
//		NSharedPOSIX::CreateDirectory : Create a directory.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::CreateDirectory(const NFilePath& thePath)
{


	// Create the directory
	int sysErr = mkdir(thePath.GetUTF8(), S_IRWXU | S_IRWXG);
	NN_EXPECT_NOT_ERR(sysErr);

	return GetErrno(sysErr);
}





//=============================================================================
//		NSharedPOSIX::GetChildren : Get the children of a directory.
//-----------------------------------------------------------------------------
NVectorFilePath NSharedPOSIX::GetChildren(const NFilePath& thePath)
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
//		NSharedPOSIX::DeletePath : Delete a path.
//-----------------------------------------------------------------------------
NStatus NSharedPOSIX::DeletePath(const NFilePath& thePath)
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

	return GetErrno(sysErr);
}





//=============================================================================
//		NSharedPOSIX::ToTimeval : Convert to a timeval.
//-----------------------------------------------------------------------------
struct timeval NSharedPOSIX::ToTimeval(NInterval theInterval)
{
	// Convert the value
	NInterval timeSecs = floor(theInterval);
	NInterval timeFrac = theInterval - timeSecs;

	return {time_t(timeSecs), suseconds_t(timeFrac / kNTimeMicrosecond)};
}





//=============================================================================
//		NSharedPOSIX::ToInterval : Convert to an NInterval.
//-----------------------------------------------------------------------------
NInterval NSharedPOSIX::ToInterval(const struct timeval& timeVal)
{


	// Convert the value
	NInterval timeSecs = NInterval(timeVal.tv_sec);
	NInterval timeFrac = NInterval(timeVal.tv_usec) * kNTimeMicrosecond;

	return timeSecs + timeFrac;
}
