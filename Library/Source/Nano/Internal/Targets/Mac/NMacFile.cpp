/*	NAME:
		NMacFile.cpp

	DESCRIPTION:
		Mac file support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
#include "NCoreFoundation.h"
#include "NSTLUtilities.h"
#include "NCFString.h"
#include "NCocoa.h"
#include "NMacTarget.h"
#include "NTargetFile.h"





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// Mapped pages
typedef std::map<void*, void*>									PagePtrMap;
typedef PagePtrMap::iterator									PagePtrMapIterator;
typedef PagePtrMap::const_iterator								PagePtrMapConstIterator;


// Mapped files
typedef struct {
	int				theFile;
	NSpinLock		theLock;
	PagePtrMap		pageTable;
} FileMapInfo;





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//      GetDirectoryForDomain : Get a well-known directory.
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
			return("");
		}



	// Get the directory
	nsPaths = NSSearchPathForDirectoriesInDomains(theDirectory, nsDomain, YES);
	nsPath  = [nsPaths objectAtIndex:0];
	thePath = NCFString(nsPath, false);
	
	return(thePath);
}





//============================================================================
//      GetCFBundle : Get a CFBundleRef.
//----------------------------------------------------------------------------
static NCFObject GetCFBundle(const NFile &theBundle)
{	NCFObject		cfURL, cfBundle;



	// Check our parameters
	if (!theBundle.IsDirectory())
		return(cfBundle);



	// Get the bundle
	if (!cfURL.SetObject(CFURLCreateWithFileSystemPath(kCFAllocatorNano, ToCF(theBundle.GetPath()), kCFURLPOSIXPathStyle, true)))
		return(cfBundle);

	cfBundle.SetObject(CFBundleCreate(kCFAllocatorNano, cfURL));
	NN_ASSERT(cfBundle.IsValid());

	return(cfBundle);
}





//============================================================================
//      NTargetFile::CreateAlias : Create an alias.
//----------------------------------------------------------------------------
//		Note :	Based on PD code by Daniel Reese.
//
//				http://www.danandcheryl.com/2009/08/how-create-alias-programmatically
//----------------------------------------------------------------------------
static NStatus CreateAlias(const NString &thePath, const NString &targetPath)
{
#if NN_TARGET_MAC
	FSRef				parentRef, aliasRef, targetRef;
	NString				theParent, theName;
	FSCatalogInfo		catalogInfo;
	FileInfo			*finderInfo;
	ResFileRefNum		resForkRef;
    HFSUniStr255		aliasName;
    AliasHandle			aliasHnd;
	NStatus				theErr;



	// Get the state we need
	theParent = NTargetFile::GetParent(thePath);
	theName   = NTargetFile::GetName(  thePath, false);
	aliasHnd  = NULL;



	// Create the alias file
	theErr = FSPathMakeRef((const UInt8 *) theParent.GetUTF8(), &parentRef, NULL);
	NN_ASSERT_NOERR(theErr);

	if (theErr == kNoErr)
		{
	    theErr = FSGetHFSUniStrFromString(ToCF(theName), &aliasName);
		NN_ASSERT_NOERR(theErr);
		}

	if (theErr == kNoErr)
	    FSCreateResFile(&parentRef, aliasName.length, aliasName.unicode, 0, NULL, &aliasRef, NULL);

	if (theErr != kNoErr)
		return(theErr);



	// Add the alias resource
	theErr = FSPathMakeRef((const UInt8 *) targetPath.GetUTF8(), &targetRef, NULL);
	NN_ASSERT_NOERR(theErr);

	if (theErr == kNoErr)
		{
	    theErr = FSNewAlias(NULL, &targetRef, &aliasHnd);
		NN_ASSERT_NOERR(theErr);
		NN_ASSERT(aliasHnd != NULL);
		}

	if (theErr == kNoErr)
		{
	    resForkRef = FSOpenResFile(&aliasRef, fsRdWrPerm);
	    UseResFile(resForkRef);

	    AddResource((Handle) aliasHnd, rAliasType, 0, NULL);
	    CloseResFile(resForkRef);
		}



	// Update the Finder info
	//
	// Clearing the Finder's 'inited' bit forces it to refresh.
    theErr = FSGetCatalogInfo(&aliasRef, kFSCatInfoFinderInfo, &catalogInfo, NULL, NULL, NULL);
	NN_ASSERT_NOERR(theErr);

	if (theErr == kNoErr)
		{
		finderInfo = (FileInfo *) &catalogInfo.finderInfo;
	    finderInfo->finderFlags |= kIsAlias;
	    finderInfo->finderFlags &= ~kHasBeenInited;
	    finderInfo->fileType     = kContainerFolderAliasType;

	    theErr = FSSetCatalogInfo(&aliasRef, kFSCatInfoFinderInfo, &catalogInfo);
		NN_ASSERT_NOERR(theErr);
		}

	return(theErr);

#else
	return(kNErrNotSupported);
#endif
}





//============================================================================
//      Public functions
//----------------------------------------------------------------------------
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
#endif

	struct stat		fileInfo;
	int				sysErr;
	bool			isLink;



	// Check for soft links
	sysErr = lstat(thePath.GetUTF8(), &fileInfo);
	isLink = (sysErr == kNoErr && S_ISLNK(fileInfo.st_mode));



	// Check for user links
#if NN_TARGET_MAC
	if (!isLink)
		{
		theErr = FSPathMakeRef((const UInt8 *) thePath.GetUTF8(), &theFSRef, NULL);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr == kNoErr)
			{
			isAlias = false;
			theErr  = FSIsAliasFile(&theFSRef, &isAlias, &isFolder);
			NN_ASSERT_NOERR(theErr);

			if (theErr == kNoErr)
				isLink = (bool) isAlias;
			}
		}
#endif

	return(isLink);
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
		if (cfURL.SetObject(CFURLCreateWithFileSystemPath(NULL, ToCF(thePath), kCFURLPOSIXPathStyle, IsDirectory(thePath))))
			{
			theErr = LSCopyDisplayNameForURL(cfURL, &cfString);
			if (theErr == kNoErr)
				theName = NCFString(cfString, true);
			}
		}


#elif NN_TARGET_IOS
	NN_UNUSED(displayName);
	NN_UNUSED(cfString);
	NN_UNUSED(theErr);
#endif



	// Get the file name
	if (theName.IsEmpty())
		{
		slashPos = thePath.Find("/", kNStringBackwards);
		if (slashPos.IsEmpty())
			theName = thePath;
		else
			theName = thePath.GetRight(thePath.GetSize() - slashPos.GetNext());
		}

	return(theName);
}





//============================================================================
//      NTargetFile::SetName : Set a file's name.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetName(const NString &thePath, const NString &fileName, bool renameFile, bool isPath, NString &newPath)
{	NStatus		theErr;
	int			sysErr;



	// Get the new path
	theErr = kNoErr;

	if (isPath)
		newPath = fileName;
	else
		newPath = GetChild(GetParent(thePath), fileName);



	// Rename the file
	if (renameFile)
		{
		sysErr = rename(thePath.GetUTF8(), newPath.GetUTF8());
		if (sysErr == -1)
			{
			sysErr = errno;
			theErr = NMacTarget::ConvertSysErr(sysErr);
			}
		}

	return(theErr);
}





//============================================================================
//      NTargetFile::GetSize : Get a file's size.
//----------------------------------------------------------------------------
UInt64 NTargetFile::GetSize(const NString &thePath)
{	struct stat		fileInfo;
	UInt64			theSize;
	int				sysErr;



	// Validate our state
	NN_ASSERT(sizeof(fileInfo.st_size) <= sizeof(UInt64));



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
NStatus NTargetFile::SetSize(const NString &thePath, NFileRef /*theFile*/, UInt64 theSize)
{	int		sysErr;



	// Set the file size
	sysErr = truncate(thePath.GetUTF8(), theSize);
	NN_ASSERT_NOERR(sysErr);

	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::GetCreationTime : Get the creation time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetCreationTime(const NString &thePath)
{	struct stat		fileInfo;
	NDate			theDate;
	int				sysErr;



	// Get the time
	sysErr = stat(thePath.GetUTF8(), &fileInfo);
	if (sysErr == kNoErr)
		theDate.SetTime((fileInfo.st_birthtimespec.tv_sec  - kNEpochTimeSince1970) +
						(fileInfo.st_birthtimespec.tv_nsec * kNTimeNanosecond));

	return(theDate);
}





//============================================================================
//      NTargetFile::GetAccessTime : Get the access time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetAccessTime(const NString &thePath)
{	struct stat		fileInfo;
	NDate			theDate;
	int				sysErr;



	// Get the time
	sysErr = stat(thePath.GetUTF8(), &fileInfo);
	if (sysErr == kNoErr)
		theDate.SetTime((fileInfo.st_atimespec.tv_sec  - kNEpochTimeSince1970) +
						(fileInfo.st_atimespec.tv_nsec * kNTimeNanosecond));

	return(theDate);
}





//============================================================================
//      NTargetFile::GetModificationTime : Get the modification time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetModificationTime(const NString &thePath)
{	struct stat		fileInfo;
	NDate			theDate;
	int				sysErr;



	// Get the time
	sysErr = stat(thePath.GetUTF8(), &fileInfo);
	if (sysErr == kNoErr)
		theDate.SetTime((fileInfo.st_mtimespec.tv_sec  - kNEpochTimeSince1970) +
						(fileInfo.st_mtimespec.tv_nsec * kNTimeNanosecond));

	return(theDate);
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
	NCFString		cfString;
	FSRef			theFSRef;
	OSStatus		theErr;
	NCFObject		cfURL;
#endif

	NString			theResult;
	char			*realPath;



	// Get the state we need
	theResult = thePath;



	// Resolve soft links
	realPath = realpath(thePath.GetUTF8(), NULL);
	if (realPath != NULL)
		{
		theResult = NString(realPath, kNStringLength);
		free(realPath);
		}



	// Resolve user links
#if NN_TARGET_MAC
	theErr = FSPathMakeRef((const UInt8 *) theResult.GetUTF8(), &theFSRef, NULL);
	if (theErr == kNoErr)
		{
		theErr = FSResolveAliasFile(&theFSRef, true, &wasFolder, &wasAlias);
		NN_ASSERT_NOERR(theErr);

		if (theErr == kNoErr && wasAlias)
			{
			if (cfURL.SetObject(CFURLCreateFromFSRef(kCFAllocatorDefault, &theFSRef)))
				{
				cfString.SetObject(CFURLCopyFileSystemPath(cfURL, kCFURLPOSIXPathStyle));
				theResult = cfString;
				}
			}
		}
#endif

	return(theResult);
}





//============================================================================
//      NTargetFile::GetChildren : Get the children of a path.
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
		sysErr    = readdir_r(theDir, &dirEntry, &dirResult);
		if (sysErr == kNoErr && dirResult != NULL)
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
NStatus NTargetFile::Delete(const NString &thePath, bool moveToTrash)
{	int		sysErr;



	// Move to the trash
	if (moveToTrash)
		{
#if NN_TARGET_MAC
		sysErr = FSPathMoveObjectToTrashSync(thePath.GetUTF8(), NULL, kFSFileOperationDefaultOptions);
#else
		NN_LOG("iOS does not support a trash");
		sysErr = -1;
#endif
		}



	// Or delete
	else
		{
		if (IsDirectory(thePath))
			sysErr = rmdir( thePath.GetUTF8());
		else
			sysErr = unlink(thePath.GetUTF8());
		}

	return(sysErr == 0 ? kNoErr : kNErrPermission);
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

		case kNLocationDocuments:
			thePath = GetDirectoryForDomain(theDomain, NSDocumentDirectory);
			break;

		case kNLocationLogs:
			if (theDomain == kNDomainUser)
				thePath = NCFString(NSHomeDirectory(), false);

			thePath += "/Library/Logs";
			break;

		case kNLocationCache:
			thePath = GetDirectoryForDomain(theDomain, NSCachesDirectory);
			break;

		case kNLocationTemporary:
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
//      NTargetFile::CreateLink : Create a link.
//----------------------------------------------------------------------------
NStatus NTargetFile::CreateLink(const NString &thePath, const NString &targetPath, NFileLink theType)
{	NStatus		theErr;



	// Create the link
	theErr = kNoErr;

	switch (theType) {
		case kNLinkSoft:
			if (symlink(targetPath.GetUTF8(), thePath.GetUTF8()) != 0)
				theErr = NMacTarget::ConvertSysErr(errno);
			break;

		case kNLinkHard:
			if (link(targetPath.GetUTF8(), thePath.GetUTF8()) != 0)
				theErr = NMacTarget::ConvertSysErr(errno);
			break;

		case kNLinkUser:
			theErr = CreateAlias(thePath, targetPath);
			break;

		default:
			NN_LOG("Unknown link type: %d", theType);
			theErr = kNErrNotSupported;
			break;
		}
	
	return(theErr);
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
//      NTargetFile::UnmountVolume : Unmount a volume.
//----------------------------------------------------------------------------
NStatus NTargetFile::UnmountVolume(const NString &thePath)
{
#if NN_TARGET_MAC
	BOOL	wasOK;



	// Unmount the volume
	wasOK = [[NSWorkspace sharedWorkspace] unmountAndEjectDeviceAtPath:ToNS(thePath)];
	NN_ASSERT(wasOK);

	return(wasOK ? kNoErr : kNErrPermission);


#elif NN_TARGET_IOS
	NN_UNUSED(thePath);

	return(kNErrNotSupported);
#endif
}





//============================================================================
//      NTargetFile::FileOpen : Open a file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::FileOpen(const NString &thePath, NFilePermission thePermission)
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
//      NTargetFile::FileClose : Close a file.
//----------------------------------------------------------------------------
void NTargetFile::FileClose(NFileRef theFile)
{	int		sysErr;



	// Close the file
	sysErr = fclose((FILE *) theFile);
	NN_ASSERT_NOERR(sysErr);
}





//============================================================================
//      NTargetFile::FileGetPosition : Get the read/write position.
//----------------------------------------------------------------------------
UInt64 NTargetFile::FileGetPosition(NFileRef theFile)
{	off_t		thePos;



	// Validate our state
	NN_ASSERT(sizeof(off_t) <= sizeof(UInt64));



	// Get the position
	thePos = ftello((FILE *) theFile);
	
	return(thePos);
}





//============================================================================
//      NTargetFile::FileSetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NTargetFile::FileSetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition thePosition)
{	int		sysErr;



	// Validate our state
	NN_ASSERT(sizeof(off_t) >= sizeof(UInt64));



	// Set the position
	sysErr = fseeko((FILE *) theFile, theOffset, NMacTarget::ConvertFilePosition(thePosition));
	NN_ASSERT_NOERR(sysErr);
	
	return(NMacTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::FileRead : Read from a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::FileRead(NFileRef theFile, UInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition thePosition, NFileFlags /*theFlags*/)
{	NStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(theSize <= kUInt32Max);



	// Adjust the position
	if (thePosition != kNPositionFromMark || theOffset != 0)
		{
		theErr = FileSetPosition(theFile, theOffset, thePosition);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr != kNoErr)
			return(theErr);
		}



	// Perform the read
	numRead = fread(thePtr, 1, (size_t) theSize, (FILE *) theFile);
	theErr  = kNoErr;

	if (feof((FILE *) theFile))
		theErr = kNErrExhaustedSrc;
	
	return(theErr);
}





//============================================================================
//      NTargetFile::FileWrite : Write to a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::FileWrite(NFileRef theFile, UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition thePosition, NFileFlags /*theFlags*/)
{	NStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(theSize <= kUInt32Max);



	// Adjust the position
	if (thePosition != kNPositionFromMark || theOffset != 0)
		{
		theErr = FileSetPosition(theFile, theOffset, thePosition);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr != kNoErr)
			return(theErr);
		}



	// Perform the write
	numWritten = fwrite(thePtr, 1, (size_t) theSize, (FILE *) theFile);
	theErr     = kNoErr;
	
	 if (numWritten != theSize)
		theErr = kNErrDiskFull;
	
	return(theErr);
}





//============================================================================
//      NTargetFile::MapOpen : Open a memory-mapped file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::MapOpen(const NFile &theFile, NMapAccess theAccess)
{	FileMapInfo		*theInfo;
	int				theFlags;
	NString			thePath;



	// Get the state we need
	thePath  = theFile.GetPath();
	theFlags = (theAccess == kNAccessReadWrite ? O_RDWR : O_RDONLY);



	// Create the file map state
	theInfo = new FileMapInfo;
	if (theInfo == NULL)
		return(kNFileRefNone);



	// Open the file
	theInfo->theFile = open(thePath.GetUTF8(), theFlags, 0);
	if (theInfo->theFile == -1)
		{
		MapClose((NFileRef) theInfo);
		theInfo = NULL;
		}
	
	return((NFileRef) theInfo);
}





//============================================================================
//      NTargetFile::MapClose : Close a memory-mapped file.
//----------------------------------------------------------------------------
void NTargetFile::MapClose(NFileRef theFile)
{	FileMapInfo		*theInfo = (FileMapInfo *) theFile;



	// Close the file
	if (theInfo->theFile != -1)
		close(theInfo->theFile);
	
	delete theInfo;
}





//============================================================================
//      NTargetFile::MapFetch : Fetch a page from a memory-mapped file.
//----------------------------------------------------------------------------
void *NTargetFile::MapFetch(NFileRef theFile, NMapAccess theAccess, UInt64 theOffset, UInt32 theSize, bool noCache)
{	FileMapInfo		*theInfo = (FileMapInfo *) theFile;
	StLock			acquireLock(theInfo->theLock);

	off_t			pageSize, mapOffset, mapDelta;
	int				pagePerm, pageFlags;
	void			*pagePtr, *thePtr;
	size_t			mapSize;



	// Get the state we need
	//
	// Pages can only be mapped with a compatible access mode to the underlying
	// file; a file opened as read-only can not be used to obtain read-write pages.
	//
	// Mac OS X 10.6 introduces two behaviour changes in 64-bit builds:
	//
	//		- The mapped offset must be pagesize()-aligned, and so we may need
	//		  to adjust the offset/size to compensate.
	//
	//		- The page flags must include either MAP_SHARED or MAP_PRIVATE, so
	//		  we default to MAP_SHARED and enable MAP_PRIVATE if required.
	//
	// For each file we maintain a table of mapped pointers to their originating
	// pages, allowing us to unmap the correct page when the pointer is discarded.
	pagePerm  = PROT_READ;
	pageFlags = MAP_FILE | MAP_SHARED;
	pageSize  = getpagesize();

	if (theAccess != kNAccessRead)
		pagePerm |= PROT_WRITE;

	if (theAccess == kNAccessCopyOnWrite)
		{
		pageFlags &= ~MAP_SHARED;
		pageFlags |=  MAP_PRIVATE;
		}

	if (noCache)
		pageFlags |= MAP_NOCACHE;

	mapOffset = theOffset - (theOffset % pageSize);
	mapDelta  = theOffset - mapOffset;
	mapSize   = theSize   + (size_t) mapDelta;



	// Fetch the page
	pagePtr = mmap(NULL, mapSize, pagePerm, pageFlags, theInfo->theFile, mapOffset);
	thePtr  = NULL;
	
	if (pagePtr != MAP_FAILED)
		{
		thePtr = (void *) (((UInt8 *) pagePtr) + mapDelta);
		
		NN_ASSERT(theInfo->pageTable.find(thePtr) == theInfo->pageTable.end());
		theInfo->pageTable[thePtr] = pagePtr;
		}

	return(thePtr);
}





//============================================================================
//      NTargetFile::MapDiscard : Discard a page from a memory-mapped file.
//----------------------------------------------------------------------------
void NTargetFile::MapDiscard(NFileRef theFile, NMapAccess theAccess, const void *thePtr, UInt32 theSize)
{	FileMapInfo		*theInfo = (FileMapInfo *) theFile;
	StLock			acquireLock(theInfo->theLock);

	void					*pagePtr;
	off_t					mapDelta;
	size_t					mapSize;
	PagePtrMapIterator		theIter;
	int						sysErr;



	// Get the state we need
	theIter = theInfo->pageTable.find(const_cast<void *>(thePtr));
	NN_ASSERT(theIter != theInfo->pageTable.end());

	pagePtr  = theIter->second;
	mapDelta = (((UInt8 *) thePtr) - ((UInt8 *) pagePtr));
	
	NN_ASSERT(mapDelta >= 0);
	mapSize = theSize + (size_t) mapDelta;



	// Flush read-write pages back to disk
	if (theAccess == kNAccessReadWrite)
		{
		sysErr = msync(pagePtr, mapSize, MS_SYNC);
		NN_ASSERT_NOERR(sysErr);
		}



	// Discard the page
	sysErr = munmap(pagePtr, mapSize);
	NN_ASSERT_NOERR(sysErr);

	theInfo->pageTable.erase(theIter);
}





//============================================================================
//      NTargetFile::BundleGetInfo : Get a bundle's Info.plist.
//----------------------------------------------------------------------------
NDictionary NTargetFile::BundleGetInfo(const NFile &theBundle)
{	NCFDictionary	theResult;
	NCFObject		cfBundle;



	// Get the state we need
	cfBundle = GetCFBundle(theBundle);
	NN_ASSERT(cfBundle.IsValid());
	
	if (!cfBundle.IsValid())
		return(theResult);
	
	
	
	// Get the info
	theResult.SetObject(CFBundleGetInfoDictionary(cfBundle), false);
	NN_ASSERT(!theResult.IsEmpty());

	return(theResult);
}





//============================================================================
//      NTargetFile::BundleGetExecutable : Get a bundle executable.
//----------------------------------------------------------------------------
NFile NTargetFile::BundleGetExecutable(const NFile &theBundle, const NString &theName)
{	NCFObject		cfBundle, cfURL;
	NFile			theResult;
	NCFString		thePath;



	// Get the state we need
	cfBundle = GetCFBundle(theBundle);
	NN_ASSERT(cfBundle.IsValid());
	
	if (!cfBundle.IsValid())
		return(theResult);



	// Get the executable
	if (theName.IsEmpty())
		cfURL.SetObject(CFBundleCopyExecutableURL(cfBundle));
	else
		cfURL.SetObject(CFBundleCopyAuxiliaryExecutableURL(cfBundle, ToCF(theName)));



	// Get the absolute path
	if (cfURL.IsValid())
		cfURL.SetObject(CFURLCopyAbsoluteURL(cfURL));
		
	if (cfURL.IsValid())
		{
		if (thePath.SetObject(CFURLCopyFileSystemPath(cfURL, kCFURLPOSIXPathStyle)))
			theResult = thePath;
		}
			
	return(theResult);
}





//============================================================================
//      NTargetFile::BundleGetResources : Get a bundle's Resources directory.
//----------------------------------------------------------------------------
NFile NTargetFile::BundleGetResources(const NFile &theBundle)
{	NCFObject		cfBundle, cfURL;
	NFile			theResult;
	NCFString		cfString;



	// Get the state we need
	cfBundle = GetCFBundle(theBundle);
	NN_ASSERT(cfBundle.IsValid());
	
	if (!cfBundle.IsValid())
		return(theResult);



	// Get the resources
	if (cfURL.SetObject(CFBundleCopyResourcesDirectoryURL(cfBundle)))
		{
		if (cfURL.SetObject(CFURLCopyAbsoluteURL(cfURL)))
			{
			if (cfString.SetObject(CFURLCopyFileSystemPath(cfURL, kCFURLPOSIXPathStyle)))
				theResult = NFile(cfString);
			}
		}
	
	return(theResult);
}


