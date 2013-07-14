/*	NAME:
		NLinuxFile.cpp

	DESCRIPTION:
		Linux file support.
	
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
#include <sys/mount.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <pwd.h>

#include "NSTLUtilities.h"
#include "NSpinLock.h"
#include "NLinuxTarget.h"
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


typedef std::map<NString, NString>								DirectoryMap;



//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
static DirectoryMap gUserDirs;




//============================================================================
//      NTargetFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
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
{	struct stat		fileInfo;
	int				sysErr;
	bool			isLink;



	// Check for soft links
	sysErr = lstat(thePath.GetUTF8(), &fileInfo);
	isLink = (sysErr == kNoErr && S_ISLNK(fileInfo.st_mode));

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
NString NTargetFile::GetName(const NString &thePath, bool /*displayName*/)
{	NRange		slashPos;
	NString		theName;



	// Get the file name
	slashPos = thePath.Find("/", kNStringBackwards);
	if (slashPos.IsEmpty())
		theName = thePath;
	else
		theName = thePath.GetRight(thePath.GetSize() - slashPos.GetNext());

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
			theErr = NLinuxTarget::ConvertSysErr(sysErr);
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

	return(NLinuxTarget::ConvertSysErr(sysErr));
}





//============================================================================
//      NTargetFile::GetCreationTime : Get the creation time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetCreationTime(const NString &thePath)
{	struct stat		fileInfo;
	NDate			theDate;
	int				sysErr;



	// Get the time
	//
	// Not actually the creation time, but as good as we can do.
	sysErr = stat(thePath.GetUTF8(), &fileInfo);
	if (sysErr == kNoErr)
		theDate.SetTime((fileInfo.st_ctime        - kNEpochTimeSince1970) +
						(fileInfo.st_ctim.tv_nsec * kNTimeNanosecond));

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
		theDate.SetTime((fileInfo.st_atime        - kNEpochTimeSince1970) +
						(fileInfo.st_atim.tv_nsec * kNTimeNanosecond));

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
		theDate.SetTime((fileInfo.st_mtime        - kNEpochTimeSince1970) +
						(fileInfo.st_mtim.tv_nsec * kNTimeNanosecond));

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
{	NString			theResult;
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
		NN_LOG("Linux does not support a trash");
		sysErr = -1;
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
//      UserDirPopulate : Populate the user dirs
//----------------------------------------------------------------------------
static void UserDirPopulate()
{
	NFile theFile = NTargetFile::GetDirectory(kNDomainUser, kNLocationPreferences).GetChild("user-dirs.dirs");

	if (theFile.IsFile())
	{
		// Need some kind of ReadLine functionality here
		NString fullText = NFileUtilities::GetFileText(theFile);
		NStringList lines = fullText.Split("\n");

		for (NStringList::iterator it = lines.begin(); it != lines.end(); ++it)
		{
			// Trim all left and right whitespace
			it->Trim();

			NIndex stringSize = it->GetSize();

			// Must be at least three characters x=y
			if (stringSize < 3)
				continue;

			NRange equalsSearch = it->Find("=");

			// There must be an equals
			if (equalsSearch.GetSize() == 0)
				continue;

			NIndex equalsPos = equalsSearch.GetFirst();

			// We must have at least one character on the right and leftof the equals
			if (equalsPos > stringSize - 2 || equalsPos == 0)
				continue;

			NString value = *it;//it->SubString(stringSize-1);
			NString key = *it;//it->SubString(stringSize+1);

			// Already trimmed value left whitespace and key right whitespace
			value.TrimRight();
			key.TrimLeft();

			// Trim any quotes on the value
			value.TrimLeft("\"");
			value.TrimRight("\"");

			// We might have trimmed down to nothing
			if (value.GetSize() == 0 || key.GetSize() == 0)
				continue;

			value.Replace("$HOME", NTargetFile::GetDirectory(kNDomainUser, kNLocationPreferences).GetChild(value).GetPath());

			gUserDirs[key]=value;
		}
	}
}






//============================================================================
//      NTargetFile::GetDirectory : Get a directory.
//----------------------------------------------------------------------------
NFile NTargetFile::GetDirectory(NDirectoryDomain theDomain, NDirectoryLocation theLocation)
{	const char		*pathPtr;
	struct passwd	*pwInfo;
	NFile			theFile;
	bool			isUser;



	// Get the state we need
	isUser = (theDomain == kNDomainUser);
	pathPtr = NULL;

	//http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html
	//http://freedesktop.org/wiki/Software/xdg-user-dirs/
	switch (theLocation) {
		case kNLocationHome:
			NN_ASSERT(isUser);
			pathPtr = getenv("HOME");
			if (pathPtr == NULL)
				{
				pwInfo = getpwuid(getuid());
				if (pwInfo != NULL)
					pathPtr = pwInfo->pw_dir;
				}
			break;

		case kNLocationLogs:
			if (isUser)
			{
				theFile = GetDirectory(theDomain, kNLocationApplicationSupport); // No spec for this but .local/share makes more sense
			} else {
				theFile = NFile("/var/log");
			}
			break;

		case kNLocationPreferences:
			if (isUser)
			{
				pathPtr = getenv("XDG_CONFIG_HOME");
				if (pathPtr == NULL)
					theFile = GetDirectory(theDomain, kNLocationHome).GetChild(".config"); // No env so default to .config
				else
					theFile = NFile(NString(pathPtr));
			} else {
				pathPtr = getenv("XDG_CONFIG_DIRS");
				NStringList split = NString(pathPtr).Split(":");
				if (split.size() == 0 || split[0].GetSize() == 0)
					theFile = NFile("/etc/xdg"); // No env so use manual default of /etc/xdg
				else
 					// For now just return the first and ignore the rest, though we need a way to return multiple config dirs.
					theFile = NFile(split[0]);
			}
			break;

		case kNLocationCache:
			if (isUser)
			{
				pathPtr = getenv("XDG_CACHE_HOME");
				if (pathPtr == NULL) // No env so use manual default
					theFile = GetDirectory(theDomain, kNLocationHome).GetChild(".cache");
				else
					theFile = NFile(NString(pathPtr));
			} else {
				theFile = NFile("/var/cache");
			}
			break;
		
		case kNLocationTemporary:
			// Maybe this should use XDG_RUNTIME_DIR for user?
			pathPtr = getenv("TMP");
			if (pathPtr == NULL)
				pathPtr = getenv("TMPDIR");
			if (pathPtr == NULL)
				pathPtr = getenv("TEMP");
			if (pathPtr == NULL)
				pathPtr = "/tmp";
			break;
		
		case kNLocationApplicationSupport:
			if (isUser)
			{
				pathPtr = getenv("XDG_DATA_HOME");
				if (pathPtr == NULL)
					theFile = GetDirectory(theDomain, kNLocationHome).GetChild(".local").GetChild("share"); // No env so default to .local/share
				else
					theFile = NFile(NString(pathPtr));
			} else {
				pathPtr = getenv("XDG_DATA_DIRS");
				NStringList split = NString(pathPtr).Split(":");
				if (split.size() == 0 || split[0].GetSize() == 0) 
					theFile = NFile("/usr/local/share"); // No env so use manual default of /usr/local/share:/usr/share
				else
					// For now just return the first and ignore the rest, though we need a way to return multiple data dirs.
					theFile = NFile(split[0]);
			}	
			break;
		
		case kNLocationDesktop:
			{
				if (gUserDirs.size() == 0)
					UserDirPopulate();
				NString found  = gUserDirs["XDG_DESKTOP_DIR"];
				if (!found.IsEmpty())
					theFile = NFile(found);
				else
					theFile = GetDirectory(theDomain, kNLocationHome).GetChild("Desktop");
			break;
			}
		case kNLocationDocuments:
			{
				if (gUserDirs.size() == 0)
					UserDirPopulate();
				NString found  = gUserDirs["XDG_DOCUMENTS_DIR"];
				if (!found.IsEmpty())
					theFile = NFile(found);
				else
					theFile = GetDirectory(theDomain, kNLocationHome);
			break;
			}
		default:
			NN_LOG("Unknown location: %d", theLocation);
			break;
		}



	// Get the directory
	if (pathPtr != NULL)
		theFile = NFile(NString(pathPtr, kNStringLength));

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
	
	return(NLinuxTarget::ConvertSysErr(sysErr));
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
				theErr = NLinuxTarget::ConvertSysErr(errno);
			break;

		case kNLinkHard:
			if (link(targetPath.GetUTF8(), thePath.GetUTF8()) != 0)
				theErr = NLinuxTarget::ConvertSysErr(errno);
			break;

		case kNLinkUser:
			NN_LOG("Unsupported link type: %d", theType);
			theErr = kNErrNotSupported;
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
{	NString		dstTmp, newPath;
	NStatus		theErr;



	// Get the state we need
	dstTmp = GetChild(GetParent(srcPath), GetName(srcPath, false) + "_exg");
	NN_ASSERT(!Exists(dstTmp));



	// Exchange the files
	//
	// This is not atomic, and needs to handle failure.
	NN_LOG("NTargetFile::ExchangeWith is not atomic!");

	theErr = SetName(dstPath, dstTmp,  true, true, newPath);
	NN_ASSERT_NOERR(theErr);
	
	theErr = SetName(srcPath, dstPath, true, true, newPath);
	NN_ASSERT_NOERR(theErr);
	
	theErr = SetName(dstTmp,  srcPath, true, true, newPath);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//      NTargetFile::UnmountVolume : Unmount a volume.
//----------------------------------------------------------------------------
NStatus NTargetFile::UnmountVolume(const NString &thePath)
{
	int	status;

	// Unmount, we can also use umount2 with flags
	// However documentation specifies umount2 might not be
	// on all systems
	status = umount(thePath.GetUTF8());

	return NLinuxTarget::ConvertSysErr(status);
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
	theFile = fopen(thePath.GetUTF8(), NLinuxTarget::ConvertFilePermission(thePermission));
	
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



	// Validate our parameters
	NN_ASSERT(sizeof(off_t) == sizeof(UInt32));
	NN_ASSERT(theOffset <= (SInt64) kUInt32Max);
	


	// Set the position
	sysErr = fseeko((FILE *) theFile, theOffset, NLinuxTarget::ConvertFilePosition(thePosition));
	NN_ASSERT_NOERR(sysErr);
	
	return(NLinuxTarget::ConvertSysErr(sysErr));
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
void *NTargetFile::MapFetch(NFileRef theFile, NMapAccess theAccess, UInt64 theOffset, UInt32 theSize, bool /*noCache*/)
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
	// Some systems have additional requirements:
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
{	NPropertyList		propertyList;
	NDictionary			theResult;
	NFile				fileInfo;



	// Get the info
	fileInfo = theBundle.GetChild("Contents/Info.plist");
	if (fileInfo.IsFile())
		theResult = propertyList.Load(fileInfo);
	
	return(theResult);
}





//============================================================================
//      NTargetFile::BundleGetExecutable : Get a bundle executable.
//----------------------------------------------------------------------------
NFile NTargetFile::BundleGetExecutable(const NFile &theBundle, const NString &theName)
{	NFile		theResult;



	// Get the executable
	theResult = theBundle.GetChild("Contents/Linux").GetChild(theName);

	return(theResult);
}





//============================================================================
//      NTargetFile::BundleGetResources : Get a bundle's Resources directory.
//----------------------------------------------------------------------------
NFile NTargetFile::BundleGetResources(const NFile &theBundle)
{	NFile		theResult;



	// Get the resources
	theResult = theBundle.GetChild("Contents/Resources");
	
	return(theResult);
}


