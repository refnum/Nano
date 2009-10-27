/*	NAME:
		NMacFile.cpp

	DESCRIPTION:
		Mac file support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <dirent.h>

#include "NNSAutoReleasePool.h"
#include "NCFString.h"

#include "NMacTarget.h"
#include "NTargetFile.h"





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//      GetDirectoryForDomain
//----------------------------------------------------------------------------
static NString GetDirectoryForDomain(NDirectoryDomain theDomain, NSSearchPathDirectory theDirectory)
{	NSSearchPathDomainMask		nsDomain;
	NSArray						*nsPaths;
	NSString					*nsPath;
	NCFString					thePath;



	// Get the state we need
	switch (theDomain) {
		case kNDomainUser:		nsDomain = NSUserDomainMask;		break;
		case kNDomainUsers:		nsDomain = NSLocalDomainMask;		break;
		case kNDomainSystem:	nsDomain = NSSystemDomainMask;		break;
		default:
			NN_LOG("Unknown domain: %d", theDomain);
			return(kNStringEmpty);
		}



	// Get the directory
	nsPaths = NSSearchPathForDirectoriesInDomains(theDirectory, nsDomain, YES);
	nsPath  = [nsPaths objectAtIndex:0];
	thePath = NCFString(nsPath, false);
	
	return(thePath);
}





//============================================================================
//      NTargetFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetFile::IsFile(const NString &thePath)
{	struct stat		fileInfo;
	int				sysErr;
	bool			isFile;



	// Check the path
	sysErr = stat(thePath.GetUTF8(), &fileInfo);
	isFile = (sysErr == kNoErr && S_ISREG(fileInfo.st_mode));
	
	return(isFile);
}





//============================================================================
//      NTargetFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NTargetFile::IsDirectory(const NString &thePath)
{	struct stat		fileInfo;
	int				sysErr;
	bool			isDir;



	// Check the path
	sysErr = stat(thePath.GetUTF8(), &fileInfo);
	isDir  = (sysErr == kNoErr && S_ISDIR(fileInfo.st_mode));
	
	return(isDir);
}





//============================================================================
//      NTargetFile::IsLink : Is this a link?
//----------------------------------------------------------------------------
bool NTargetFile::IsLink(const NString &thePath)
{
#if NN_TARGET_MAC
	Boolean			isAlias, isFolder;
	FSRef			theFSRef;
	OSStatus		theErr;



	// Get the FSRef
	theErr = FSPathMakeRef((const UInt8 *) thePath.GetUTF8(), &theFSRef, NULL);
	if (theErr != noErr)
		return(false);



	// Check the flags
	theErr = FSIsAliasFile(&theFSRef, &isAlias, &isFolder);
	if (theErr != noErr)
		isAlias = false;

	return(isAlias);


#elif NN_TARGET_IPHONE
	NN_UNUSED(thePath);

	return(false);
#endif
}





//============================================================================
//      NTargetFile::IsWriteable : Is a file writeable?
//----------------------------------------------------------------------------
bool NTargetFile::IsWriteable(const NString &thePath)
{	bool			isWriteable;
	struct stat		fileInfo;
	FILE			*tmpFile;
	DIR				*tmpDir;
	int				sysErr;



	// Check the path
	isWriteable = false;
	sysErr      = stat(thePath.GetUTF8(), &fileInfo);

	if (sysErr != kNoErr)
		return(isWriteable);



	// Check a file
	if (S_ISREG(fileInfo.st_mode))
		{
		tmpFile = fopen(thePath.GetUTF8(), "a");
		if (tmpFile != NULL)
			{
			isWriteable = true;
			fclose(tmpFile);
			}
		}
	
	
	// Check a directory
	else if (S_ISDIR(fileInfo.st_mode))
		{
		tmpDir = opendir(thePath.GetUTF8());
		if (tmpDir != NULL)
			{
			isWriteable = true;
			closedir(tmpDir);
			}
		}


	// Handle failure
	else
		NN_LOG("Don't know how to open '%@'", thePath);
	
	return(isWriteable);
}





//============================================================================
//      NTargetFile::Exists : Does a file exist?
//----------------------------------------------------------------------------
bool NTargetFile::Exists(const NString &thePath)
{	bool			doesExist;
	struct stat		fileInfo;
	int				sysErr;



	// Check the path
	sysErr    = stat(thePath.GetUTF8(), &fileInfo);
	doesExist = (sysErr == kNoErr);
	
	return(doesExist);
}





//============================================================================
//      NTargetFile::GetName : Get a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::GetName(const NString &thePath, bool displayName)
{	CFStringRef		cfString;
	NRange			slashPos;
	NString			theName;
	NStatus			theErr;
	NCFObject		cfURL;



	// Get the display name
	//
	// Only files that exist have a display name, so we fall through on errors.
#if NN_TARGET_MAC
	if (displayName)
		{
		if (cfURL.SetObject(CFURLCreateWithFileSystemPath(NULL, NCFString(thePath).GetObject(), kCFURLPOSIXPathStyle, IsDirectory(thePath))))
			{
			theErr = LSCopyDisplayNameForURL(cfURL, &cfString);
			if (theErr == noErr)
				theName = NCFString(cfString, true);
			}
		}


#elif NN_TARGET_IPHONE
	NN_UNUSED(displayName);
	NN_UNUSED(cfString);
	NN_UNUSED(theErr);
#endif



	// Get the file name
	if (theName.IsEmpty())
		{
		slashPos = thePath.Find("/", kNStringBackwards);
		if (!slashPos.IsEmpty())
			theName = thePath.GetRight(thePath.GetSize() - slashPos.GetNext());
		}

	return(theName);
}





//============================================================================
//      NTargetFile::SetName : Set a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::SetName(const NString &thePath, const NString &fileName, bool renameFile)
{	NString		newPath;
	int			sysErr;



	// Get the new path
	newPath = GetChild(GetParent(thePath), fileName);



	// Rename the file
	if (renameFile)
		{
		sysErr = rename(thePath.GetUTF8(), newPath.GetUTF8());
		NN_ASSERT_NOERR(sysErr);
		}
	
	return(newPath);
}





//============================================================================
//      NTargetFile::GetSize : Get a file's size.
//----------------------------------------------------------------------------
UInt64 NTargetFile::GetSize(const NString &thePath)
{	struct stat		fileInfo;
	UInt64			theSize;
	int				sysErr;



	// Validate our state
	NN_ASSERT(sizeof(fileInfo.st_size) >= sizeof(UInt64));



	// Get the file size
	theSize = 0;
	sysErr  = stat(thePath.GetUTF8(), &fileInfo);
	NN_ASSERT_NOERR(sysErr);

	if (sysErr == kNoErr)
		theSize = fileInfo.st_size;

	return(theSize);
}





//============================================================================
//      NTargetFile::SetSize : Set a file's size.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetSize(const NString &thePath, UInt64 theSize)
{	int		sysErr;



	// Set the file size
	sysErr = truncate(thePath.GetUTF8(), theSize);
	NN_ASSERT_NOERR(sysErr);

	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::GetChild : Get the child of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetChild(const NString &thePath, const NString &fileName)
{	NString		theChild;



	// Validate our parameters
	NN_ASSERT(!fileName.StartsWith("/"));



	// Get the child
	theChild = thePath;
	theChild.TrimRight("/");

	theChild += "/";
	theChild += fileName;
	
	return(theChild);
}





//============================================================================
//      NTargetFile::GetParent : Get the parent of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetParent(const NString &thePath)
{	NString		theParent;
	NRange		slashPos;



	// Get the parent
	slashPos = thePath.Find("/", kNStringBackwards);

	if (!slashPos.IsEmpty())
		theParent = thePath.GetLeft(slashPos.GetLocation());
	else
		theParent = thePath;
	
	return(theParent);
}





//============================================================================
//      NTargetFile::GetTarget : Get the target of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetTarget(const NString &thePath)
{
#if NN_TARGET_MAC
	Boolean			wasFolder, wasAlias;
	NCFString		theResult;
	FSRef			theFSRef;
	OSStatus		theErr;
	NCFObject		cfURL;



	// Get the FSRef
	theErr = FSPathMakeRef((const UInt8 *) thePath.GetUTF8(), &theFSRef, NULL);
	if (theErr != noErr)
		return(thePath);



	// Resolve the alias
	theErr = FSResolveAliasFile(&theFSRef, true, &wasFolder, &wasAlias);
	if (theErr != noErr || !wasAlias)
		return(thePath);
	
	
	
	// Get the path
	theResult = thePath;
	
	if (cfURL.SetObject(CFURLCreateFromFSRef(kCFAllocatorDefault, &theFSRef)))
		theResult.SetObject(CFURLCopyFileSystemPath(cfURL, kCFURLPOSIXPathStyle));

	return(theResult);


#elif NN_TARGET_IPHONE
	return(thePath);
#endif
}





//============================================================================
//      NTargetFile::GetChildren : Get the chilren of a path.
//----------------------------------------------------------------------------
NFileList NTargetFile::GetChildren(const NString &thePath)
{	struct dirent		dirEntry, *dirResult;
	NString				filePath;
	NFileList			theFiles;
	DIR					*theDir;
	int					sysErr;



	// Get the state we need
	theDir = opendir(thePath.GetUTF8());
	if (theDir == NULL)
		return(theFiles);
	
	
	
	// Collect the children
	do
		{
		dirResult = NULL;
		sysErr   = readdir_r(theDir, &dirEntry, &dirResult);
		if (sysErr == kNoErr)
			{
			if (strcmp(dirEntry.d_name, ".") != 0 && strcmp(dirEntry.d_name, "..") != 0)
				{
				filePath.Format("%@/%s", thePath, dirEntry.d_name);
				theFiles.push_back(NFile(filePath));
				}
			}
		}
	while (sysErr == kNoErr && dirResult != NULL);



	// Clean up
	closedir(theDir);

	return(theFiles);
}





//============================================================================
//      NTargetFile::Delete : Delete a file.
//----------------------------------------------------------------------------
void NTargetFile::Delete(const NString &thePath)
{	int		sysErr;



	// Delete the file
	if (IsDirectory(thePath))
		sysErr = rmdir( thePath.GetUTF8());
	else
		sysErr = unlink(thePath.GetUTF8());

	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//      NTargetFile::GetDirectory : Get a directory.
//----------------------------------------------------------------------------
NFile NTargetFile::GetDirectory(NDirectoryDomain theDomain, NDirectoryLocation theLocation)
{	StAutoReleasePool		autoRelease;
	NCFString				thePath;
	NFile					theFile;



	// Get the path
	switch (theLocation) {
		case kNLocationHome:
			if (theDomain == kNDomainUser)
				thePath = NCFString(NSHomeDirectory(), false);
			else
				NN_LOG("Invalid domain - kNLocationHome requires kNDomainUser");
			break;

		case kNLocationDesktop:
			thePath = GetDirectoryForDomain(theDomain, NSDesktopDirectory);
			break;

		case kNLocationCachedData:
			thePath = GetDirectoryForDomain(theDomain, NSCachesDirectory);
			break;

		case kNLocationTemporaryData:
			if (theDomain == kNDomainUser)
				thePath = NCFString(NSTemporaryDirectory(), false);
			else
				thePath = NCFString("/tmp");
			break;

		case kNLocationApplicationSupport:
			thePath = GetDirectoryForDomain(theDomain, NSApplicationSupportDirectory);
			break;

		default:
			NN_LOG("Unknown location: %d", theLocation);
			break;
		}
	
	
	
	// Get the directory
	if (!thePath.IsEmpty())
		theFile = NFile(thePath);
	
	return(theFile);
}





//============================================================================
//      NTargetFile::CreateDirectory : Create a directory.
//----------------------------------------------------------------------------
NStatus NTargetFile::CreateDirectory(const NString &thePath)
{	int		sysErr;



	// Create the directory
	sysErr = mkdir(thePath.GetUTF8(), S_IRWXU | S_IRWXG);
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NTargetFile::ExchangeWith(const NString &srcPath, const NString &dstPath)
{	int		sysErr;



	// Swap the files
	sysErr = exchangedata(srcPath.GetUTF8(), dstPath.GetUTF8(), 0);
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::Open : Open a file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::Open(const NString &thePath, NFilePermission thePermission)
{	FILE	*theFile;
	int		theFD;



	// Create the file
	if (!Exists(thePath))
		{
		theFD = creat(thePath.GetUTF8(), S_IRWXU | S_IRWXG);
		if (theFD == 0)
			return(kNFileRefNone);
		
		close(theFD);
		}



	// Open the file
	theFile = fopen(thePath.GetUTF8(), NMacTarget::ConvertFilePermission(thePermission));
	
	return((NFileRef) theFile);
}





//============================================================================
//      NTargetFile::Close : Close a file.
//----------------------------------------------------------------------------
void NTargetFile::Close(NFileRef theFile)
{	int		sysErr;



	// Close the file
	sysErr = fclose((FILE *) theFile);
	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//      NTargetFile::GetPosition : Get the read/write position.
//----------------------------------------------------------------------------
UInt64 NTargetFile::GetPosition(NFileRef theFile)
{	off_t		thePos;



	// Validate our state
	NN_ASSERT(sizeof(off_t) >= sizeof(UInt64));



	// Get the position
	thePos = ftello((FILE *) theFile);
	
	return(thePos);
}





//============================================================================
//      NTargetFile::SetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition thePosition)
{	int		sysErr;



	// Validate our state
	NN_ASSERT(sizeof(off_t) >= sizeof(UInt64));



	// Set the position
	sysErr = fseeko((FILE *) theFile, theOffset, NMacTarget::ConvertFilePosition(thePosition));
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::Read : Read from a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Read(NFileRef theFile, UInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition thePosition, NFileFlags theFlags)
{	NStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(theSize <= kUInt32Max);
	NN_UNUSED(theFlags);



	// Adjust the position
	if (thePosition != kNPositionFromMark || theOffset != 0)
		{
		theErr = SetPosition(theFile, theOffset, thePosition);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr != kNoErr)
			return(theErr);
		}



	// Perform the read
	numRead = fread(thePtr, 1, theSize, (FILE *) theFile);
	theErr  = kNoErr;

	if (feof((FILE *) theFile))
		theErr = kNErrExhaustedSrc;
	
	return(theErr);
}





//============================================================================
//      NTargetFile::Write : Write to a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Write(NFileRef theFile, UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition thePosition, NFileFlags theFlags)
{	NStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(theSize <= kUInt32Max);
	NN_UNUSED(theFlags);



	// Adjust the position
	if (thePosition != kNPositionFromMark || theOffset != 0)
		{
		theErr = SetPosition(theFile, theOffset, thePosition);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr != kNoErr)
			return(theErr);
		}



	// Perform the write
	numWritten = fwrite(thePtr, 1, theSize, (FILE *) theFile);
	theErr     = noErr;
	
	 if (numWritten != theSize)
		theErr = kNErrDiskFull;
	
	return(theErr);
}





//============================================================================
//      NTargetFile::MapOpen : Open a memory-mapped file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::MapOpen(const NFile &theFile, NMapAccess theAccess)
{	int			theFlags;
	NString		thePath;
	NFileRef	fileRef;



	// Get the state we need
	thePath  = theFile.GetPath();
	theFlags = (theAccess == kNAccessReadWrite ? O_RDWR : O_RDONLY);



	// Open the file
	fileRef = (NFileRef) open(thePath.GetUTF8(), theFlags, 0);
	
	return(fileRef);
}





//============================================================================
//      NTargetFile::MapClose : Close a memory-mapped file.
//----------------------------------------------------------------------------
void NTargetFile::MapClose(NFileRef theFile)
{


	// Close the file
	close((int) theFile);
}





//============================================================================
//      NTargetFile::MapFetch : Fetch a page from a memory-mapped file.
//----------------------------------------------------------------------------
void *NTargetFile::MapFetch(NFileRef theFile, NMapAccess theAccess, UInt64 theOffset, UInt32 theSize, bool noCache)
{	int			pagePerm, pageFlags;
	void		*thePtr;



	// Get the state we need
	//
	// Pages can only be mapped with a compatible access mode to the underlying file;
	// a file opened as read-only can not be used to obtain read-write pages.
	pagePerm  = PROT_READ;
	pageFlags = MAP_FILE;

	if (theAccess != kNAccessRead)
		pagePerm |= PROT_WRITE;

	if (theAccess == kNAccessReadWrite)
		pageFlags |= MAP_SHARED;

	if (theAccess == kNAccessCopyOnWrite)
		pageFlags |= MAP_PRIVATE;

	if (noCache)
		pageFlags |= MAP_NOCACHE;



	// Map the page
	thePtr = mmap(NULL, theSize, pagePerm, pageFlags, (int) theFile, theOffset);
	if (thePtr == MAP_FAILED)
		thePtr = NULL;
	
	return(thePtr);
}





//============================================================================
//      NTargetFile::MapDiscard : Discard a page from a memory-mapped file.
//----------------------------------------------------------------------------
void NTargetFile::MapDiscard(NFileRef /*theFile*/, NMapAccess theAccess, const void *thePtr, UInt32 theSize)
{	int		sysErr;



	// Flush read-write pages back to disk
	if (theAccess == kNAccessReadWrite)
		{
		sysErr = msync((void *) thePtr, theSize, MS_SYNC);
		NN_ASSERT_NOERR(sysErr);
		}



	// Unmap the page
	sysErr = munmap((void *) thePtr, theSize);
	NN_ASSERT_NOERR(sysErr);
}






