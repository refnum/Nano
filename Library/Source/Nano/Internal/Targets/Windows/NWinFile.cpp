/*	NAME:
		NWindowsFile.cpp

	DESCRIPTION:
		Windows file support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <shellapi.h>
#include <objidl.h>
#include <shlobj.h>

#include "NWindows.h"
#include "NWinTarget.h"
#include "NTargetFile.h"





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// File map info
typedef struct {
	HANDLE			theFile;
	HANDLE			memFile;
} FileMapInfo;





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//      GetDirectoryForDomain : Get a well-known directory.
//----------------------------------------------------------------------------
static NString GetDirectoryForDomain(NDirectoryDomain theDomain, REFKNOWNFOLDERID theID)
{	TCHAR		*pathPtr;
	NString		thePath;



	// Validate our parameters
	NN_ASSERT(theDomain == kNDomainUser);
	NN_UNUSED(theDomain);



	// Get the directory
	if (SUCCEEDED(SHGetKnownFolderPath(theID, KF_FLAG_CREATE, NULL, &pathPtr)))
		{
		thePath = ToNN(pathPtr);
		CoTaskMemFree( pathPtr);
		}
	
	return(thePath);
}





//============================================================================
//      NTargetFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
#pragma mark -
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
{	DWORD		fileInfo;
	bool		isLink;



	// Check the path
	fileInfo = GetFileAttributes(ToWN(thePath));
	isLink   = ((fileInfo & FILE_ATTRIBUTE_REPARSE_POINT) != 0);
	
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
NString NTargetFile::GetName(const NString &thePath, bool /*displayName*/)
{	NRange			slashPos;
	NString			theName;



	// Get the file name
	slashPos = thePath.Find("\\", kNStringBackwards);
	if (!slashPos.IsEmpty())
		theName = thePath.GetRight(thePath.GetSize() - slashPos.GetNext());

	return(theName);
}





//============================================================================
//      NTargetFile::SetName : Set a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::SetName(const NString &thePath, const NString &fileName, bool renameFile)
{	NString		newPath;
	BOOL		wasOK;



	// Get the new path
	newPath = GetChild(GetParent(thePath), fileName);



	// Rename the file
	if (renameFile)
		{
		wasOK = MoveFileEx(ToWN(thePath), ToWN(newPath), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH);
		NN_ASSERT(wasOK);
		}
	
	return(newPath);
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
		theSize = ((UInt64) fileInfo.nFileSizeLow) + (((UInt64) fileInfo.nFileSizeHigh) << 32);

	return(theSize);
}





//============================================================================
//      NTargetFile::SetSize : Set a file's size.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetSize(const NString &thePath, UInt64 theSize)
{	NFileRef	theFile;
	NStatus		theErr;
	BOOL		wasOK;



	// Open the file
	theFile = Open(thePath, kNPermissionWrite);
	if (theFile == kNFileRefNone)
		return(kNErrPermission);



	// Set the size
	theErr = SetPosition(theFile, theSize, kNPositionFromStart);
	if (theErr == kNoErr)
		{
		wasOK = SetEndOfFile((HANDLE) theFile);
		NN_ASSERT(wasOK);
		}



	// Clean up
	Close(theFile);
	
	return(theErr);
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
	NString			theTarget;
	SHFILEINFO		theInfo;



	// Get the state we need
	theTarget = thePath;

	shellLink   = NULL;
	persistFile = NULL;

    if ((SHGetFileInfo(ToWN(thePath), 0, &theInfo, sizeof(theInfo), SHGFI_ATTRIBUTES) == 0))
		return(theTarget);

    if (!(theInfo.dwAttributes & SFGAO_LINK))
		return(theTarget);

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

	return(theTarget);
}





//============================================================================
//      NTargetFile::GetChildren : Get the children of a path.
//----------------------------------------------------------------------------
NFileList NTargetFile::GetChildren(const NString &thePath)
{	WIN32_FIND_DATA		dirEntry;
	NString				filePath;
	NFileList			theFiles;
	HANDLE				theDir;



	// Get the state we need
	theDir = FindFirstFile(ToWN(thePath), &dirEntry);
	if (theDir == INVALID_HANDLE_VALUE)
		return(theFiles);



	// Collect the children
	do
		{
		filePath.Format("%@/%@", thePath, ToNN(dirEntry.cFileName));
		theFiles.push_back(NFile(filePath));
		}
	while (FindNextFile(theDir, &dirEntry));



	// Clean up
	FindClose(theDir);

	return(theFiles);
}





//============================================================================
//      NTargetFile::Delete : Delete a file.
//----------------------------------------------------------------------------
void NTargetFile::Delete(const NString &thePath)
{	BOOL	wasOK;



	// Delete the file
	wasOK = DeleteFile(ToWN(thePath));
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetFile::GetDirectory : Get a directory.
//----------------------------------------------------------------------------
NFile NTargetFile::GetDirectory(NDirectoryDomain theDomain, NDirectoryLocation theLocation)
{	TCHAR		theBuffer[MAX_PATH];
	NFile		theFile;
	NString		thePath;



	// Get the state we need
	switch (theLocation) {
		case kNLocationHome:
			thePath = GetDirectoryForDomain(theDomain, FOLDERID_Profile);
			break;
		
		case kNLocationDesktop:
			thePath = GetDirectoryForDomain(theDomain, FOLDERID_Desktop);
			break;
		
		case kNLocationCachedData:
			thePath = GetDirectoryForDomain(theDomain, FOLDERID_InternetCache);
			break;
		
		case kNLocationTemporaryData:
			if (SUCCEEDED(GetTempPath(NN_ARRAY_SIZE(theBuffer), theBuffer)))
				thePath = ToNN(theBuffer);
			break;
		
		case kNLocationApplicationSupport:
			thePath = GetDirectoryForDomain(theDomain, FOLDERID_ProgramData);
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
//      NTargetFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NTargetFile::ExchangeWith(const NString &srcPath, const NString &dstPath)
{	NString		dstTmp;



	// Get the state we need
	dstTmp = GetChild(GetParent(srcPath), GetName(srcPath, false) + "_exg");
	NN_ASSERT(!Exists(dstTmp));



	// Exchange the files
	//
	// This is not atomic, and needs to handle failure.
	NN_LOG("NTargetFile::ExchangeWith is not atomic!");

	SetName(dstPath, dstTmp,  true);
	SetName(srcPath, dstPath, true);
	SetName(dstTmp,  srcPath, true);
	
	return(kNoErr);
}





//============================================================================
//      NTargetFile::Open : Open a file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::Open(const NString &thePath, NFilePermission thePermission)
{	HANDLE		theFile;



	// Open the file
	theFile = CreateFile(ToWN(thePath), NWinTarget::ConvertFilePermission(thePermission),
							0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	return((NFileRef) theFile);
}





//============================================================================
//      NTargetFile::Close : Close a file.
//----------------------------------------------------------------------------
void NTargetFile::Close(NFileRef theFile)
{	BOOL	wasOK;



	// Close the file
	wasOK = CloseHandle((HANDLE) theFile);
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetFile::GetPosition : Get the read/write position.
//----------------------------------------------------------------------------
UInt64 NTargetFile::GetPosition(NFileRef theFile)
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
//      NTargetFile::SetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition thePosition)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(sizeof(LARGE_INTEGER) >= sizeof(UInt64));



	// Set the position
	theErr = kNoErr;
	
	if (!SetFilePointerEx((HANDLE) theFile, ToWN(theOffset), NULL, NWinTarget::ConvertFilePosition(thePosition)))
		theErr = NWinTarget::GetLastError();
	
	return(theErr);
}





//============================================================================
//      NTargetFile::Read : Read from a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Read(NFileRef theFile, UInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition thePosition, NFileFlags theFlags)
{	DWORD		bytesRead;
	NStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theSize <= kSInt32Max);
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
	numRead = 0;
	theErr  = kNoErr;

	if (!ReadFile((HANDLE) theFile,  thePtr, (DWORD) theSize, &bytesRead, NULL))
		theErr = NWinTarget::GetLastError();

	if (theErr == kNoErr)
		numRead = bytesRead;
	
	return(theErr);
}





//============================================================================
//      NTargetFile::Write : Write to a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Write(NFileRef theFile, UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition thePosition, NFileFlags theFlags)
{	DWORD		bytesWritten;
	NStatus		theErr;



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
	theInfo->theFile = CreateFile(ToWN(thePath), theFlags, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (theInfo->theFile != INVALID_HANDLE_VALUE)
		theInfo->memFile = CreateFileMapping(theInfo->theFile, NULL, NWinTarget::ConvertFileMapAccess(theAccess, true), 0, 0, NULL);



	// Handle failure
	if (theInfo->theFile == INVALID_HANDLE_VALUE || theInfo->memFile == INVALID_HANDLE_VALUE)
		{
		MapClose((NFileRef) theInfo);
		delete theInfo;
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
}





//============================================================================
//      NTargetFile::MapFetch : Fetch a page from a memory-mapped file.
//----------------------------------------------------------------------------
void *NTargetFile::MapFetch(NFileRef theFile, NMapAccess theAccess, UInt64 theOffset, UInt32 theSize, bool noCache)
{	FileMapInfo		*theInfo = (FileMapInfo *) theFile;
	DWORD			offsetHigh, offsetLow;
	void			*thePtr;



	// Compiler warnings
	NN_UNUSED(noCache);



	// Get the state we need
	ToWN(theOffset, offsetHigh, offsetLow);



	// Fetch the page
	thePtr = MapViewOfFile(theInfo->memFile, NWinTarget::ConvertFileMapAccess(theAccess, false), offsetHigh, offsetLow, theSize);
	
	return(thePtr);
}





//============================================================================
//      NTargetFile::MapDiscard : Discard a page from a memory-mapped file.
//----------------------------------------------------------------------------
void NTargetFile::MapDiscard(NFileRef theFile, NMapAccess theAccess, const void *thePtr, UInt32 theSize)
{	FileMapInfo		*theInfo = (FileMapInfo *) theFile;
	BOOL			wasOK;



	// Compiler warnings
	NN_UNUSED(theAccess);
	NN_UNUSED(theFile);



	// Flush read-write pages back to disk
	if (theAccess == kNAccessReadWrite)
		{
		wasOK = FlushViewOfFile(thePtr, 0);
		NN_ASSERT(wasOK);
		}



	// Discard the page
	wasOK = UnmapViewOfFile(thePtr);
	NN_ASSERT(wasOK);
}





