/*	NAME:
		NWindowsFile.cpp

	DESCRIPTION:
		Windows file support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <shellapi.h>
#include <winioctl.h>
#include <objidl.h>
#include <shlobj.h>

#include "NWindows.h"
#include "NRegistry.h"
#include "NSpinLock.h"
#include "NWinTarget.h"
#include "NPropertyList.h"
#include "NTargetFile.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
static const NIndex kUnmountLockAttempts						= 20;
static const DWORD  kUnmountLockTimeout							= 10000;





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// Mapped pages
typedef std::map<void*, void*>									PagePtrMap;
typedef PagePtrMap::iterator									PagePtrMapIterator;
typedef PagePtrMap::const_iterator								PagePtrMapConstIterator;


// Mapped files
typedef struct {
	HANDLE			theFile;
	HANDLE			memFile;
	NSpinLock		theLock;
	PagePtrMap		pageTable;
} FileMapInfo;





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//      GetDirectoryForDomain : Get a well-known directory.
//----------------------------------------------------------------------------
static NString GetDirectoryForDomain(NDirectoryDomain theDomain, int theID)
{	TCHAR		theBuffer[MAX_PATH];
	NString		thePath;



	// Validate our parameters
	NN_ASSERT(theDomain == kNDomainUser);
	NN_UNUSED(theDomain);



	// Get the directory
	if (SUCCEEDED(SHGetFolderPath(NULL, theID | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, theBuffer)))
		thePath = ToNN(theBuffer);
	
	return(thePath);
}





//============================================================================
//      DeviceControl : Send a device control code.
//----------------------------------------------------------------------------
static NStatus DeviceControl(HANDLE hVolume, DWORD ioCode, NIndex inSize, const void *inData)
{	DWORD		returnedSize;
	NStatus		theErr;
	BOOL		wasOK;



	// Send the control code
	wasOK  = DeviceIoControl(hVolume, ioCode, (LPVOID) inData, inSize, NULL, 0, &returnedSize, NULL);
	theErr = wasOK ? kNoErr : kNErrPermission;

	return(theErr);
}





#pragma mark NTargetFile
//============================================================================
//      NTargetFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
bool NTargetFile::IsFile(const NString &thePath)
{	DWORD		fileInfo;
	bool		isFile;



	// Check the path
	fileInfo = GetFileAttributes(ToWN(thePath));
	isFile   = ((fileInfo & FILE_ATTRIBUTE_DIRECTORY) == 0);
	
	return(isFile);
}





//============================================================================
//      NTargetFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NTargetFile::IsDirectory(const NString &thePath)
{	DWORD		fileInfo;
	bool		isDir;



	// Check the path
	fileInfo = GetFileAttributes(ToWN(thePath));
	isDir    = ((fileInfo & FILE_ATTRIBUTE_DIRECTORY) != 0);
	
	return(isDir);
}





//============================================================================
//      NTargetFile::IsLink : Is this a link?
//----------------------------------------------------------------------------
bool NTargetFile::IsLink(const NString &thePath)
{	SHFILEINFO		shellInfo;
	DWORD			fileInfo;
	bool			isLink;



	// Check for soft links
	fileInfo = GetFileAttributes(ToWN(thePath));
	isLink   = ((fileInfo & FILE_ATTRIBUTE_REPARSE_POINT) != 0);



	// Check for user links
	//
	// SHGetFileInfo will only return SFGAO_LINK if the file has a .lnk extension,
	// even if IPersistFile would have been able to load the data.
	//
	// Since shortcuts are only usable in Explorer if they have a .lnk extension
	// we assume that, like ResolveTarget, we can ignore 'raw' shortcuts.
	if (!isLink)
		{
	    if ((SHGetFileInfo(ToWN(thePath), 0, &shellInfo, sizeof(shellInfo), SHGFI_ATTRIBUTES) != 0))
			isLink = ((shellInfo.dwAttributes & SFGAO_LINK) == SFGAO_LINK);
		}

	return(isLink);
}





//============================================================================
//      NTargetFile::IsWriteable : Is a file writeable?
//----------------------------------------------------------------------------
bool NTargetFile::IsWriteable(const NString &thePath)
{	bool		isWriteable;
	DWORD		fileInfo;



	// Check the path
	fileInfo    = GetFileAttributes(ToWN(thePath));
	isWriteable = ((fileInfo & FILE_ATTRIBUTE_READONLY) == 0);
	
	return(isWriteable);
}





//============================================================================
//      NTargetFile::Exists : Does a file exist?
//----------------------------------------------------------------------------
bool NTargetFile::Exists(const NString &thePath)
{	bool		doesExist;
	DWORD		fileInfo;



	// Check the path
	fileInfo  = GetFileAttributes(ToWN(thePath));
	doesExist = (fileInfo != INVALID_FILE_ATTRIBUTES);

	return(doesExist);
}





//============================================================================
//      NTargetFile::GetName : Get a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::GetName(const NString &thePath, bool displayName)
{	NRange			slashPos, dotPos;
	NRegistry		theRegistry;
	NString			theName;
	NStatus			theErr;



	// Get the state we need
	theName = thePath;



	// Extract the file name
	slashPos = theName.Find("\\", kNStringBackwards);
	if (!slashPos.IsEmpty())
		theName = theName.GetRight(theName.GetSize() - slashPos.GetNext());



	// Extract the display name
	if (displayName)
		{
		theErr = theRegistry.Open(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced");
		if (theErr == kNoErr)
			{
			if (theRegistry.GetValueBoolean("HideFileExt"))
				{
				dotPos = theName.Find(".", kNStringBackwards);
				if (!dotPos.IsEmpty())
					theName = theName.GetLeft(dotPos.GetLocation());
				}
			}
		}

	return(theName);
}





//============================================================================
//      NTargetFile::SetName : Set a file's name.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetName(const NString &thePath, const NString &fileName, bool renameFile, bool isPath, NString &newPath)
{	NStatus		theErr;



	// Get the new path
	theErr = kNoErr;

	if (isPath)
		newPath = fileName;
	else
		newPath = GetChild(GetParent(thePath), fileName);



	// Rename the file
	if (renameFile)
		{
		if (!MoveFileEx(ToWN(thePath), ToWN(newPath), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH))
			theErr = NWinTarget::GetLastError();
		}

	return(theErr);
}





//============================================================================
//      NTargetFile::GetSize : Get a file's size.
//----------------------------------------------------------------------------
UInt64 NTargetFile::GetSize(const NString &thePath)
{	WIN32_FILE_ATTRIBUTE_DATA		fileInfo;
	UInt64							theSize;
	
	
	
	// Get the size
	theSize = 0;

	if (GetFileAttributesEx(ToWN(thePath), GetFileExInfoStandard, &fileInfo))
		theSize = ToNN(fileInfo.nFileSizeHigh, fileInfo.nFileSizeLow);

	return(theSize);
}





//============================================================================
//      NTargetFile::SetSize : Set a file's size.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetSize(const NString &thePath, NFileRef theFile, UInt64 theSize)
{	NStatus		theErr;



	// Check the size
	//
	// SetEndOfFile returns ERROR_ACCESS_DENIED when trying to set
	// the size of an empty file to zero.
	if (GetSize(thePath) == theSize)
		return(kNoErr);



	// Set the size
	theErr = FileSetPosition(theFile, theSize, kNPositionFromStart);
	if (theErr == kNoErr)
		{
		if (!SetEndOfFile((HANDLE) theFile))
			theErr = NWinTarget::GetLastError();
		}
	
	return(theErr);
}





//============================================================================
//      NTargetFile::GetCreationTime : Get the creation time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetCreationTime(const NString &thePath)
{	WIN32_FILE_ATTRIBUTE_DATA		fileInfo;
	NDate							theDate;



	// Get the time
	if (GetFileAttributesEx(ToWN(thePath), GetFileExInfoStandard, &fileInfo))
		theDate = NWinTarget::ConvertFILETIME(fileInfo.ftCreationTime);

	return(theDate);
}





//============================================================================
//      NTargetFile::GetAccessTime : Get the access time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetAccessTime(const NString &thePath)
{	WIN32_FILE_ATTRIBUTE_DATA		fileInfo;
	NDate							theDate;



	// Get the time
	if (GetFileAttributesEx(ToWN(thePath), GetFileExInfoStandard, &fileInfo))
		theDate = NWinTarget::ConvertFILETIME(fileInfo.ftLastAccessTime);

	return(theDate);
}





//============================================================================
//      NTargetFile::GetModificationTime : Get the modification time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetModificationTime(const NString &thePath)
{	WIN32_FILE_ATTRIBUTE_DATA		fileInfo;
	NDate							theDate;



	// Get the time
	if (GetFileAttributesEx(ToWN(thePath), GetFileExInfoStandard, &fileInfo))
		theDate = NWinTarget::ConvertFILETIME(fileInfo.ftLastWriteTime);

	return(theDate);
}





//============================================================================
//      NTargetFile::GetChild : Get the child of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetChild(const NString &thePath, const NString &fileName)
{	NString		theChild;



	// Get the child
	theChild = thePath;
	theChild.TrimRight("\\");

	theChild += "\\";
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
	slashPos = thePath.Find("\\", kNStringBackwards);

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
{	TCHAR			theBuffer[MAX_PATH];
	IPersistFile	*persistFile;
	IShellLink		*shellLink;
	SHFILEINFO		shellInfo;
	NString			theTarget;



	// Get the state we need
	theTarget = thePath;

	shellLink   = NULL;
	persistFile = NULL;

    if ((SHGetFileInfo(ToWN(thePath), 0, &shellInfo, sizeof(shellInfo), SHGFI_ATTRIBUTES) == 0))
		return(theTarget);

    if (!(shellInfo.dwAttributes & SFGAO_LINK))
		return(theTarget);

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &shellLink)))
		shellLink->QueryInterface(IID_IPersistFile, (LPVOID *) &persistFile);



	// Resolve the target
	if (persistFile != NULL)
		{
        if (SUCCEEDED(persistFile->Load(ToWN(thePath), STGM_READ)))
			{
            if (SUCCEEDED(shellLink->Resolve(0, SLR_NO_UI)))
				{
                if (SUCCEEDED(shellLink->GetPath(theBuffer, sizeof(theBuffer), NULL, 0)))
					theTarget = ToNN(theBuffer);
				}
			}
		}



	// Clean up
	WNSafeRelease(persistFile);
	WNSafeRelease(shellLink);

	CoUninitialize();

	return(theTarget);
}





//============================================================================
//      NTargetFile::GetChildren : Get the children of a path.
//----------------------------------------------------------------------------
NFileList NTargetFile::GetChildren(const NString &thePath)
{	NString				filePath, fileName;
	WIN32_FIND_DATA		dirEntry;
	NFileList			theFiles;
	HANDLE				theDir;



	// Get the state we need
	filePath.Format("%@\\*", thePath);
	theDir = FindFirstFile(ToWN(filePath), &dirEntry);

	if (theDir == INVALID_HANDLE_VALUE)
		return(theFiles);



	// Collect the children
	do
		{
		fileName = ToNN(dirEntry.cFileName);
		if (fileName != "." && fileName != "..")
			{
			filePath.Format("%@\\%@", thePath, fileName);
			theFiles.push_back(NFile(filePath));
			}
		}
	while (FindNextFile(theDir, &dirEntry));



	// Clean up
	FindClose(theDir);

	return(theFiles);
}





//============================================================================
//      NTargetFile::Delete : Delete a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Delete(const NString &thePath, bool moveToTrash)
{	NData				pathList;
	SHFILEOPSTRUCT		fileOp;
	NStatus				theErr;
	BOOL				wasOK;



	// Move to the trash
	if (moveToTrash)
		{
		pathList = thePath.GetData(kNStringEncodingUTF16);
		pathList.SetSize(pathList.GetSize() + (sizeof(TCHAR) * 2));

		memset(&fileOp, 0x00, sizeof(fileOp));
		fileOp.wFunc  = FO_DELETE; 
		fileOp.pFrom  = (LPCTSTR) pathList.GetData();
		fileOp.fFlags = FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;

		wasOK = SHFileOperation(&fileOp);
		}



	// Or delete
	else
		{
		if (IsDirectory(thePath))
			wasOK = RemoveDirectory(ToWN(thePath));
		else
			wasOK = DeleteFile(ToWN(thePath));
		}



	// Get the result
	theErr = wasOK ? kNoErr : NWinTarget::GetLastError();

	return(theErr);
}





//============================================================================
//      NTargetFile::GetDirectory : Get a directory.
//----------------------------------------------------------------------------
NFile NTargetFile::GetDirectory(NDirectoryDomain theDomain, NDirectoryLocation theLocation)
{	TCHAR		theBuffer[MAX_PATH];
	NFile		theFile;
	NString		thePath;
	bool		isUser;



	// Get the state we need
	isUser = (theDomain == kNDomainUser);
	
	switch (theLocation) {
		case kNLocationHome:
			NN_ASSERT(isUser);
			thePath = GetDirectoryForDomain(theDomain, CSIDL_PROFILE);
			break;
		
		case kNLocationDesktop:
			thePath = GetDirectoryForDomain(theDomain, isUser ? CSIDL_DESKTOPDIRECTORY : CSIDL_COMMON_DESKTOPDIRECTORY);
			break;
		
		case kNLocationDocuments:
			thePath = GetDirectoryForDomain(theDomain, isUser ? CSIDL_MYDOCUMENTS : CSIDL_COMMON_DOCUMENTS);
			break;

		case kNLocationLogs:
			thePath = GetDirectoryForDomain(theDomain, CSIDL_COMMON_APPDATA);
			break;

		case kNLocationCache:
			thePath = GetDirectoryForDomain(theDomain, CSIDL_INTERNET_CACHE);
			break;
		
		case kNLocationTemporary:
			if (SUCCEEDED(GetTempPath(NN_ARRAY_SIZE(theBuffer), theBuffer)))
				thePath = ToNN(theBuffer);
			break;
		
		case kNLocationApplicationSupport:
			thePath = GetDirectoryForDomain(theDomain, isUser ? CSIDL_LOCAL_APPDATA : CSIDL_COMMON_APPDATA);
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
{	NStatus		theErr;



	// Create the directory
	theErr = kNoErr;
	
	if (!::CreateDirectory(ToWN(thePath), NULL))
		theErr = NWinTarget::GetLastError();

	return(theErr);
}





//============================================================================
//      NTargetFile::CreateLink : Create a link.
//----------------------------------------------------------------------------
NStatus NTargetFile::CreateLink(const NString &thePath, const NString &targetPath, NFileLink /*theType*/)
{	IPersistFile	*persistFile;
	IShellLink		*shellLink;
	HRESULT			winErr;



	// Get the state we need
	winErr = ERROR_INVALID_HANDLE;

	shellLink   = NULL;
	persistFile = NULL;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &shellLink)))
		shellLink->QueryInterface(IID_IPersistFile, (LPVOID *) &persistFile);



	// Create the link
	//
	// For now we always create shortcuts.
	if (persistFile != NULL)
		{
		winErr = shellLink->SetPath(ToWN(targetPath));

		if (SUCCEEDED(winErr))
			winErr = persistFile->Save(ToWN(thePath), TRUE);
		}



	// Clean up
	WNSafeRelease(persistFile);
	WNSafeRelease(shellLink);

	CoUninitialize();

	return(NWinTarget::ConvertHRESULT(winErr));
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
//		Note : http://support.microsoft.com/default.aspx?scid=kb;en-us;165721
//----------------------------------------------------------------------------
NStatus NTargetFile::UnmountVolume(const NString &thePath)
{	NString						driveLetter, pathRoot, pathVolume;
	DWORD						accessFlags;
	UINT						driveType;
	HANDLE						hVolume;
	PREVENT_MEDIA_REMOVAL		pmrInfo;
	NStatus						theErr;
	NIndex						n;



	// Check our parameters
	if (thePath.IsEmpty())
		return(kNErrNotFound);



	// Get the state we need
	driveLetter = thePath.GetLeft(1);

	pathRoot.Format("%@:\\",        driveLetter);
	pathVolume.Format("\\\\.\\%@:", driveLetter);

	pmrInfo.PreventMediaRemoval = FALSE;



	// Get the drive type
	driveType = GetDriveType(ToWN(pathRoot));

	switch(driveType) {
		case DRIVE_REMOVABLE:
			accessFlags = GENERIC_READ | GENERIC_WRITE;
			break;
		
		case DRIVE_CDROM:
			accessFlags = GENERIC_READ;
			break;
		
		default:
			NN_LOG("Unknown drive type (%d) for (%@)", driveType, thePath);
			return(kNErrNotSupported);
			break;
		}



	// Open the volume
	hVolume = CreateFile(ToWN(pathVolume), accessFlags, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	theErr  = (hVolume == INVALID_HANDLE_VALUE) ? kNErrPermission : kNoErr;



	// Lock the volume
	if (theErr == kNoErr)
		{
		for (n = 0; n < kUnmountLockAttempts; n++)
			{
			theErr = DeviceControl(hVolume, FSCTL_LOCK_VOLUME, 0, NULL);
			if (theErr == kNoErr)
				break;

			Sleep(kUnmountLockTimeout / kUnmountLockAttempts);
			}
		}



	// Unmount the volume
	if (theErr == kNoErr)
		theErr = DeviceControl(hVolume, FSCTL_DISMOUNT_VOLUME, 0, NULL);

	if (theErr == kNoErr)
		theErr = DeviceControl(hVolume, IOCTL_STORAGE_MEDIA_REMOVAL, sizeof(pmrInfo), &pmrInfo);

	if (theErr == kNoErr)
		theErr = DeviceControl(hVolume, IOCTL_STORAGE_EJECT_MEDIA, 0, NULL);



	// Close the volume
	WNSafeCloseHandle(hVolume);

	return(theErr);
}





//============================================================================
//      NTargetFile::FileOpen : Open a file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::FileOpen(const NString &thePath, NFilePermission thePermission)
{	HANDLE		theFile;
	NStatus		theErr;



	// Open the file
	theFile = CreateFile(ToWN(thePath),
						 NWinTarget::ConvertFilePermission(thePermission),
						 FILE_SHARE_READ | FILE_SHARE_WRITE,
						 NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (theFile == INVALID_HANDLE_VALUE)
		theFile = kNFileRefNone;



	// Adjust the position
	//
	// Files opened for writing always append to the file.
	if (thePermission == kNPermissionWrite)
		{
		theErr = FileSetPosition((NFileRef) theFile, 0, kNPositionFromEnd);
		NN_ASSERT_NOERR(theErr);

		if (theErr != kNoErr)
			{
			FileClose((NFileRef) theFile);
			theFile = kNFileRefNone;
			}
		}

	return((NFileRef) theFile);
}





//============================================================================
//      NTargetFile::FileClose : Close a file.
//----------------------------------------------------------------------------
void NTargetFile::FileClose(NFileRef theFile)
{	BOOL	wasOK;



	// Close the file
	wasOK = CloseHandle((HANDLE) theFile);
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetFile::FileGetPosition : Get the read/write position.
//----------------------------------------------------------------------------
UInt64 NTargetFile::FileGetPosition(NFileRef theFile)
{	SInt64				theOffset;
	LARGE_INTEGER		thePos;



	// Validate our state
	NN_ASSERT(sizeof(LARGE_INTEGER) <= sizeof(UInt64));



	// Set the position
	theOffset = 0;
	
	if (SetFilePointerEx((HANDLE) theFile, ToWN(0LL), &thePos, FILE_CURRENT))
		theOffset = ToNN(thePos);
	
	return(theOffset);
}





//============================================================================
//      NTargetFile::FileSetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NTargetFile::FileSetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition thePosition)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(sizeof(LARGE_INTEGER) <= sizeof(UInt64));



	// Set the position
	theErr = kNoErr;
	
	if (!SetFilePointerEx((HANDLE) theFile, ToWN(theOffset), NULL, NWinTarget::ConvertFilePosition(thePosition)))
		theErr = NWinTarget::GetLastError();
	
	return(theErr);
}





//============================================================================
//      NTargetFile::FileRead : Read from a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::FileRead(NFileRef theFile, UInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition thePosition, NFileFlags /*theFlags*/)
{	DWORD		bytesRead;
	NStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theSize <= kSInt32Max);



	// Adjust the position
	if (thePosition != kNPositionFromMark || theOffset != 0)
		{
		theErr = FileSetPosition(theFile, theOffset, thePosition);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr != kNoErr)
			return(theErr);
		}



	// Perform the read
	numRead = 0;
	theErr  = kNoErr;

	if (!ReadFile((HANDLE) theFile,  thePtr, (DWORD) theSize, &bytesRead, NULL))
		theErr = NWinTarget::GetLastError();

	if (theErr == kNoErr && bytesRead == 0)
		theErr = kNErrExhaustedSrc;

	if (theErr == kNoErr)
		numRead = bytesRead;
	
	return(theErr);
}





//============================================================================
//      NTargetFile::FileWrite : Write to a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::FileWrite(NFileRef theFile, UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition thePosition, NFileFlags /*theFlags*/)
{	DWORD		bytesWritten;
	NStatus		theErr;



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
	numWritten = 0;
	theErr     = kNoErr;
	
	if (!WriteFile((HANDLE) theFile, thePtr, (DWORD) theSize, &bytesWritten, NULL))
		theErr = NWinTarget::GetLastError();

	if (theErr == kNoErr)
		numWritten = bytesWritten;

	 if (numWritten != theSize)
		theErr = kNErrDiskFull;
	
	return(theErr);
}





//============================================================================
//      NTargetFile::MapOpen : Open a memory-mapped file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::MapOpen(const NFile &theFile, NMapAccess theAccess)
{	FileMapInfo		*theInfo;
	DWORD			theFlags;
	NString			thePath;



	// Get the state we need
	thePath  = theFile.GetPath();
	theFlags = GENERIC_READ;
	
	 if (theAccess == kNAccessReadWrite)
		theFlags |= GENERIC_WRITE;



	// Create the file map state
	theInfo = new FileMapInfo;
	if (theInfo == NULL)
		return(kNFileRefNone);



	// Open the file
	theInfo->theFile = CreateFile(ToWN(thePath), theFlags, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	theInfo->memFile = INVALID_HANDLE_VALUE;

	if (theInfo->theFile != INVALID_HANDLE_VALUE)
		theInfo->memFile = CreateFileMapping(theInfo->theFile, NULL, NWinTarget::ConvertFileMapAccess(theAccess, false), 0, 0, NULL);



	// Handle failure
	if (theInfo->theFile == INVALID_HANDLE_VALUE || theInfo->memFile == INVALID_HANDLE_VALUE)
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
	WNSafeCloseHandle(theInfo->theFile);
	WNSafeCloseHandle(theInfo->memFile);
	
	delete theInfo;
}





//============================================================================
//      NTargetFile::MapFetch : Fetch a page from a memory-mapped file.
//----------------------------------------------------------------------------
void *NTargetFile::MapFetch(NFileRef theFile, NMapAccess theAccess, UInt64 theOffset, UInt32 theSize, bool /*noCache*/)
{	FileMapInfo		*theInfo = (FileMapInfo *) theFile;
	StLock			acquireLock(theInfo->theLock);

	SInt64			pageSize, mapOffset, mapDelta;
	DWORD			offsetHigh, offsetLow;
	void			*pagePtr, *thePtr;
	SYSTEM_INFO		sysInfo;



	// Get the state we need
	//
	// The mapped offset must be dwAllocationGranularity aligned, so we may need
	// to adjust the offset/size to compensate.
	//
	// For each file we maintain a table of mapped pointers to their originating
	// pages, allowing us to unmap the correct page when the pointer is discarded.
	GetSystemInfo(&sysInfo);
	pageSize = sysInfo.dwAllocationGranularity;

	mapOffset = theOffset - (theOffset % pageSize);
	mapDelta  = theOffset - mapOffset;
	theSize   = (UInt32) (theSize + mapDelta);

	ToWN(mapOffset, offsetHigh, offsetLow);



	// Fetch the page
	pagePtr = MapViewOfFile(theInfo->memFile, NWinTarget::ConvertFileMapAccess(theAccess, true), offsetHigh, offsetLow, theSize);
	thePtr  = NULL;
	
	if (pagePtr != NULL)
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
	SInt64					mapDelta;
	PagePtrMapIterator		theIter;
	BOOL					wasOK;



	// Get the state we need
	theIter = theInfo->pageTable.find((void *) thePtr);
	NN_ASSERT(theIter != theInfo->pageTable.end());

	pagePtr  = theIter->second;
	mapDelta = (((UInt8 *) thePtr) - ((UInt8 *) pagePtr));
	
	NN_ASSERT(mapDelta >= 0);
	theSize = (UInt32) (theSize + mapDelta);



	// Flush read-write pages back to disk
	if (theAccess == kNAccessReadWrite)
		{
		wasOK = FlushViewOfFile(pagePtr, theSize);
		NN_ASSERT(wasOK);
		}



	// Discard the page
	wasOK = UnmapViewOfFile(pagePtr);
	NN_ASSERT(wasOK);

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
	fileInfo = theBundle.GetChild("Contents\\Info.plist");
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
	theResult = theBundle.GetChild("Contents\\Windows").GetChild(theName);

	return(theResult);
}





//============================================================================
//      NTargetFile::BundleGetResources : Get a bundle's Resources directory.
//----------------------------------------------------------------------------
NFile NTargetFile::BundleGetResources(const NFile &theBundle)
{	NFile		theResult;



	// Get the resources
	theResult = theBundle.GetChild("Contents\\Resources");
	
	return(theResult);
}





