/*	NAME:
		NFile.cpp

	DESCRIPTION:
		Nano file object.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NFileUtilities.h"
#include "NCFObject.h"
#include "NCFURL.h"
#include "NFile.h"





//============================================================================
//		NFile::NFile : Constructor.
//----------------------------------------------------------------------------
NFile::NFile(const FSSpec &theFSSpec)
{	OSStatus	theErr;



	// Initialize ourselves
	InitializeSelf();



	// Set the file
	theErr = FSpMakeFSRef(&theFSSpec, &mFSRef);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NFile::NFile : Constructor.
//----------------------------------------------------------------------------
NFile::NFile(const FSRef &theFSRef)
{


	// Initialize ourselves
	InitializeSelf();



	// Set the file
	mFSRef = theFSRef;
}





//============================================================================
//		NFile::NFile : Constructor.
//----------------------------------------------------------------------------
NFile::NFile(const NString &thePath, CFURLPathStyle pathStyle)
{


	// Initialize ourselves
	InitializeSelf();



	// Set the file
	SetPath(thePath, pathStyle);
}





//============================================================================
//		NFile::NFile : Constructor.
//----------------------------------------------------------------------------
NFile::NFile(void)
{


	// Initialize ourselves
	InitializeSelf();
}





//============================================================================
//		NFile::~NFile : Destructor.
//----------------------------------------------------------------------------
NFile::~NFile(void)
{


	// Clean up
	if (IsOpen())
		CloseFile();
}





//============================================================================
//		NFile::IsValid : Is the file valid?
//----------------------------------------------------------------------------
bool NFile::IsValid(void) const
{


	// Check our state
	//
	// A file is considered valid if it references something that exists
	// or could exist (i.e., IsValid can be used to test if a file object
	// is a 'null' file which has never been initialized).
	return(Exists() || mNameIfParent.IsNotEmpty());
}





//============================================================================
//		NFile::Exists : Does the file/directory exist?
//----------------------------------------------------------------------------
bool NFile::Exists(void) const
{	FSCatalogInfo	theInfo;
	OSStatus		theErr;



	// Check our state
	theErr = GetCatalogInfo(kFSCatInfoNodeFlags, theInfo);
	
	return(theErr == noErr);
}





//============================================================================
//		NFile::NotExist : Does the file/directory not exist?
//----------------------------------------------------------------------------
bool NFile::NotExist(void) const
{


	// Check our state
	return(!Exists());
}





//============================================================================
//		NFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
bool NFile::IsFile(void) const
{	FSCatalogInfo	theInfo;
	OSStatus		theErr;
	bool			isFile;



	// Check our state
	theErr = GetCatalogInfo(kFSCatInfoNodeFlags, theInfo);
	isFile = (theErr == noErr && (theInfo.nodeFlags & kFSNodeIsDirectoryMask) == 0);

	return(isFile);
}





//============================================================================
//		NFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NFile::IsDirectory(void) const
{	FSCatalogInfo	theInfo;
	OSStatus		theErr;
	bool			isDir;



	// Check our state
	theErr = GetCatalogInfo(kFSCatInfoNodeFlags, theInfo);
	isDir  = (theErr == noErr && (theInfo.nodeFlags & kFSNodeIsDirectoryMask) != 0);

	return(isDir);
}





//============================================================================
//		NFile::IsAlias : Is this an alias?
//----------------------------------------------------------------------------
bool NFile::IsAlias(void) const
{	FileInfo	theInfo;
	bool		isAlias;



	// Check the flags
	theInfo = GetFileInfo();
	isAlias = ((theInfo.finderFlags & kIsAlias) != 0);
	
	return(isAlias);
}





//============================================================================
//		NFile::GetFSRef : Get the FSRef.
//----------------------------------------------------------------------------
FSRef NFile::GetFSRef(void) const
{	


	// Resolve our file
	if (!ResolveFile())
		return(kFSRefZero);



	// Get the FSRef
	return(mFSRef);
}





//============================================================================
//		NFile::SetFSRef : Set the FSRef.
//----------------------------------------------------------------------------
void NFile::SetFSRef(const FSRef &theFSRef, const NString &nameIfParent)
{


	// Set the FSref
	mFSRef        = theFSRef;
	mNameIfParent = nameIfParent;
}





//============================================================================
//		NFile::GetFileName : Get the file name.
//----------------------------------------------------------------------------
NString NFile::GetFileName(void) const
{	NString			theResult;
	HFSUniStr255	fileName;
	OSStatus		theErr;



	// Handle files that don't exist
	if (mNameIfParent.IsNotEmpty())
		theResult = mNameIfParent;



	// Handle files that do exist
	else
		{
		theErr = FSGetCatalogInfo(&mFSRef, kFSCatInfoNone, NULL, &fileName, NULL, NULL);
		NN_ASSERT_NOERR(theErr);
	
		if (theErr == noErr)
			theResult = NString(fileName);
		}

	return(theResult);
}





//============================================================================
//		NFile::SetFileName : Set the file name.
//----------------------------------------------------------------------------
OSStatus NFile::SetFileName(const NString &theName, bool renameFile)
{	FSRef		theFSRef;
	OSStatus	theErr;



	// Resolve our file
	if (!ResolveFile())
		return(fnfErr);



	// Rename the file if necessary
	if (renameFile)
		{
		theFSRef = GetFSRef();
		theErr   = FSRenameUnicode(&theFSRef, theName.GetSize(), theName.GetUTF16(), kUnicode16BitFormat, NULL);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr != noErr)
			return(theErr);
		}



	// Set the file name
	mFSRef        = GetParent().GetFSRef();
	mNameIfParent = theName;
	
	return(noErr);
}





//============================================================================
//		NFile::GetExtension : Get the file extension.
//----------------------------------------------------------------------------
NString NFile::GetExtension(void) const
{	NString		theResult;
	CFRange		dotPos;



	// Get the extension
	theResult = GetFileName();
	dotPos    = theResult.Find(".", kCFCompareBackwards);
	
	if (dotPos.length == 0)
		theResult.Clear();
	else
		theResult = theResult.GetString(dotPos.location + 1);
	
	return(theResult);
}





//============================================================================
//		NFile::SetExtension : Set the file extension.
//----------------------------------------------------------------------------
OSStatus NFile::SetExtension(const NString &theExtension, bool renameFile)
{	NString		theName, oldExtension;



	// Construct the new name
	oldExtension = GetExtension();
	theName      = GetFileName();

	theName  = theName.GetLeft(theName.GetSize() - oldExtension.GetSize());
	theName += theExtension;



	// Set the file name
	return(SetFileName(theName, renameFile));
}





//============================================================================
//		NFile::IsExtensionVisible : Is the file extension visible?
//----------------------------------------------------------------------------
bool NFile::IsExtensionVisible(void) const
{	bool					isVisible;
	FSRef					theFSRef;
	LSItemInfoRecord		theInfo;
	OSStatus				theErr;



	// Get the state we need
	isVisible = true;
	theFSRef  = GetFSRef();
	
	theErr = LSCopyItemInfoForRef(&theFSRef, kLSRequestExtensionFlagsOnly, &theInfo);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		isVisible = ((theInfo.flags & kLSItemInfoExtensionIsHidden) == 0);
	
	return(isVisible);
}





//============================================================================
//		NFile::SetExtensionVisible : Set the file extension visibility.
//----------------------------------------------------------------------------
void NFile::SetExtensionVisible(bool isVisible)
{	FSRef		theFSRef;
	OSStatus	theErr;



	// Set the visibility
	theFSRef = GetFSRef();
	theErr   = LSSetExtensionHiddenForRef(&theFSRef, !isVisible);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NFile::GetPath : Get the path.
//----------------------------------------------------------------------------
NString NFile::GetPath(CFURLPathStyle pathStyle) const
{	NString		theResult;
	NCFObject	cfURL;



	// Resolve our file
	//
	// Even if the file can't be resolved, we can still construct a path.
	(void) ResolveFile();



	// Get the full path
	if (cfURL.Set(CFURLCreateFromFSRef(kCFAllocatorNano, &mFSRef)))
		theResult.Set(CFURLCopyFileSystemPath(cfURL, pathStyle));



	// Append the file name
	//
	// If the file doesn't exist, our FSRef is to the parent directory.
	if (mNameIfParent.IsNotEmpty())
		{
		theResult += NFileUtilities::GetPathSeparator(pathStyle);
		theResult += mNameIfParent;
		}

	return(theResult);
}





//============================================================================
//		NFile::SetPath : Set the path.
//----------------------------------------------------------------------------
void NFile::SetPath(const NString &thePath, CFURLPathStyle pathStyle)
{	NCFObject	cfURL, cfParent;



	// Reset our state
	memset(&mFSRef, 0x00, sizeof(mFSRef));
	mNameIfParent.Clear();



	// Convert the path
	if (cfURL.Set(CFURLCreateWithFileSystemPath(kCFAllocatorNano, thePath, pathStyle, false)))
		{
		// Get the FSRef
		if (!CFURLGetFSRef(cfURL, &mFSRef))
			{
			// Fall back to parent+name
			if (cfParent.Set(CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorNano, cfURL)))
				{
				if (CFURLGetFSRef(cfParent, &mFSRef))
					mNameIfParent.Set(CFURLCopyLastPathComponent(cfURL));
				else
					mFSRef = kFSRefZero;
				}
			}
		}
}





//============================================================================
//		NFile::GetURL : Get the URL.
//----------------------------------------------------------------------------
NCFURL NFile::GetURL(void) const
{	NCFURL		theURL;



	// Get the URL
	theURL = NCFURL(*this);

	return(theURL);
}





//============================================================================
//		NFile::SetURL : Set the URL.
//----------------------------------------------------------------------------
void NFile::SetURL(const NCFURL &theURL)
{


	// Set the URL
	*this = theURL.GetFile();
}





//============================================================================
//		NFile::GetFSSpec : Get the FSSpec.
//----------------------------------------------------------------------------
FSSpec NFile::GetFSSpec(void) const
{	FSSpec		theFSSpec;
	OSStatus	theErr;



	// Resolve our file
	if (!ResolveFile())
		return(kFSSpecZero);



	// Get the FSSpec
	theErr = FSGetCatalogInfo(&mFSRef, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
	NN_ASSERT_NOERR(theErr);

	if (theErr != noErr)
		theFSSpec = kFSSpecZero;
	
	return(theFSSpec);
}





//============================================================================
//		NFile::GetDisplayName : Get the file display name.
//----------------------------------------------------------------------------
NString NFile::GetDisplayName(void) const
{	NString			theResult;
	CFStringRef		cfString;
	OSStatus		theErr;



	// Resolve our file
	if (!ResolveFile())
		return(theResult);



	// Get the display name
	//
	// We fetch the display name for ourselves as a URL, to allow us to return an
	// appropriate name (e.g., with extension hidden) even if our file does not exist.
	theErr = LSCopyDisplayNameForURL(GetURL(), &cfString);
	if (theErr == noErr)
		theResult.Set(cfString);

	return(theResult);
}





//============================================================================
//		NFile::GetUTI : Get the file UTI.
//----------------------------------------------------------------------------
NUTI NFile::GetUTI(void) const
{	NString			theExtension;
	OSType			theType;
	CFStringRef		cfValue;
	NUTI			theUTI;
	OSStatus		theErr;



	// Use Launch Services if available
	//
	// Using LS to fetch the UTI is the preferred path, since this allows the UTI
	// to be determined using whatever meta-data the system thinks is appropriate.
	//
	// LS can only determine the UTI if the file exists; for files that don't exist,
	// we will need to fall through to obtaining the UTI from our extension.
	if (ResolveFile() && LSCopyItemAttribute != NULL)
		{
		theErr = LSCopyItemAttribute(&mFSRef, kLSRolesAll, kLSItemContentType, (CFTypeRef *) &cfValue);
		if (theErr == noErr)
			{
			theUTI = cfValue;
			CFSafeRelease(cfValue);
			}
		}



	// Fall back to the file type
	if (!theUTI.IsValid() && Exists())
		{
		theType = GetFileType();
		if (theType != kFileTypeNone)
			theUTI = NUTI(kUTTagClassOSType, NUTI::GetStringFromOSType(theType));
		}
	
	
	
	// Fall back to the file extension
	if (!theUTI.IsValid())
		{
		theExtension = GetExtension();
		if (theExtension.IsNotEmpty())
			theUTI = NUTI(kUTTagClassFilenameExtension, theExtension);
		}



	// Fall back to the root
	if (!theUTI.IsValid())
		theUTI = kUTNTypeItem;
	
	return(theUTI);
}





//============================================================================
//		NFile::GetFileCreator : Get the file creator.
//----------------------------------------------------------------------------
OSType NFile::GetFileCreator(void) const
{	OSType		fileCreator, fileType;



	// Get the file creator
	GetFileCreatorAndType(fileCreator, fileType);
	
	return(fileCreator);
}





//============================================================================
//		NFile::GetFileType : Get the file type.
//----------------------------------------------------------------------------
OSType NFile::GetFileType(void) const
{	OSType		fileCreator, fileType;



	// Get the file type
	GetFileCreatorAndType(fileCreator, fileType);
	
	return(fileType);
}





//============================================================================
//		NFile::GetFileCreatorAndType : Get the file creator and type.
//----------------------------------------------------------------------------
void NFile::GetFileCreatorAndType(OSType &fileCreator, OSType &fileType) const
{	FileInfo	theInfo;



	// Get the file creator and type
	theInfo = GetFileInfo();
	
	fileCreator = theInfo.fileCreator;
	fileType    = theInfo.fileType;
}





//============================================================================
//		NFile::SetFileCreator : Set the file creator.
//----------------------------------------------------------------------------
void NFile::SetFileCreator(OSType fileCreator)
{


	// Set the file creator
	SetFileCreatorAndType(fileCreator, GetFileType());
}





//============================================================================
//		NFile::SetFileType : Set the file type.
//----------------------------------------------------------------------------
void NFile::SetFileType(OSType fileType)
{


	// Set the file type
	SetFileCreatorAndType(GetFileCreator(), fileType);
}





//============================================================================
//		NFile::SetFileCreatorAndType : Set the file creator and type.
//----------------------------------------------------------------------------
void NFile::SetFileCreatorAndType(OSType fileCreator, OSType fileType)
{	FileInfo	theInfo;



	// Set the file creator and type
	theInfo = GetFileInfo();
	
	theInfo.fileCreator = fileCreator;
	theInfo.fileType    = fileType;
	
	SetFileInfo(theInfo);
}





//============================================================================
//		NFile::GetPermissions : Get the permissions.
//----------------------------------------------------------------------------
mode_t NFile::GetPermissions(void) const
{	FSPermissionInfo	*permInfo;
	FSCatalogInfo		theInfo;
	OSStatus			theErr;



	// Get the state we need
	theErr = GetCatalogInfo(kFSCatInfoPermissions, theInfo);
	if (theErr != noErr)
		return(0);



	// Get the file permissions
	//
	// Note that the FSFileSecurityRef is returned as a copy, even
	// though this is a getter API - and so we must release it.
	permInfo = (FSPermissionInfo *) &theInfo.permissions;
	CFSafeRelease(permInfo->fileSec);
	
	return(permInfo->mode);
}





//============================================================================
//		NFile::SetPermissions : Set the permissions.
//----------------------------------------------------------------------------
void NFile::SetPermissions(mode_t theMode)
{	FSPermissionInfo	*permInfo;
	FSCatalogInfo		theInfo;
	OSStatus			theErr;



	// Get the state we need
	permInfo       = (FSPermissionInfo *) &theInfo.permissions;
	permInfo->mode = theMode;



	// Set the file permissions
	theErr = SetCatalogInfo(kFSCatInfoPermissions, theInfo);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NFile::GetFileInfo : Get the Finder info.
//----------------------------------------------------------------------------
FileInfo NFile::GetFileInfo(void) const
{	FileInfo		fileInfo;
	FSCatalogInfo	theInfo;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(Exists());
	NN_ASSERT(sizeof(theInfo.finderInfo) == sizeof(fileInfo));



	// Get the Finder info
	theErr = GetCatalogInfo(kFSCatInfoFinderInfo, theInfo);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr == noErr)
		memcpy(&fileInfo, &theInfo.finderInfo, sizeof(fileInfo));
	else
		memset(&fileInfo, 0x00,                sizeof(fileInfo));

	return(fileInfo);
}





//============================================================================
//		NFile::SetFileInfo : Set the Finder info.
//----------------------------------------------------------------------------
void NFile::SetFileInfo(const FileInfo &fileInfo)
{	FSCatalogInfo	theInfo;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(Exists());
	NN_ASSERT(sizeof(theInfo.finderInfo) == sizeof(fileInfo));



	// Set the Finder info
	memcpy(&theInfo.finderInfo, &fileInfo, sizeof(fileInfo));

	theErr = SetCatalogInfo(kFSCatInfoFinderInfo, theInfo);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NFile::GetCatalogInfo : Get the catalog info.
//----------------------------------------------------------------------------
OSStatus NFile::GetCatalogInfo(FSCatalogInfoBitmap whichInfo, FSCatalogInfo &theInfo) const
{	OSStatus	theErr;



	// Resolve our file
	if (!ResolveFile())
		return(fnfErr);



	// Get the catalog info
	theErr = FSGetCatalogInfo(&mFSRef, whichInfo, &theInfo, NULL, NULL, NULL);

	return(theErr);
}





//============================================================================
//		NFile::SetCatalogInfo : Set the catalog info.
//----------------------------------------------------------------------------
OSStatus NFile::SetCatalogInfo(FSCatalogInfoBitmap whichInfo, const FSCatalogInfo &theInfo)
{	OSStatus	theErr;



	// Resolve our file
	if (!ResolveFile())
		return(fnfErr);



	// Set the catalog info
	theErr = FSSetCatalogInfo(&mFSRef, whichInfo, &theInfo);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NFile::GetFileSize : Get the file size.
//----------------------------------------------------------------------------
UInt64 NFile::GetFileSize(void) const
{	FSCatalogInfo	theInfo;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(IsFile());
	


	// Get the state we need
	theErr = GetCatalogInfo(kFSCatInfoDataSizes, theInfo);
	NN_ASSERT_NOERR(theErr);
	
	if (theErr != noErr)
		theInfo.dataLogicalSize = 0;
	
	return(theInfo.dataLogicalSize);
}





//============================================================================
//		NFile::GetChild : Get the child of a directory.
//----------------------------------------------------------------------------
NFile NFile::GetChild(const NString &fileName) const
{	NFile	theChild;



	// Validate our state
	NN_ASSERT(IsDirectory());



	// Get the child
	theChild.SetFSRef(GetFSRef(), fileName);
	
	return(theChild);
}





//============================================================================
//		NFile::GetParent : Get the parent of a file.
//----------------------------------------------------------------------------
NFile NFile::GetParent(void) const
{	FSRef		theFSRef, parentFSRef;
	NFile		theParent;
	OSStatus	theErr;



	// Handle files that don't exist
	if (mNameIfParent.IsNotEmpty())
		theParent = mFSRef;
	
	
	
	// Handle files that do exist
	else
		{
		// Resolve our file
		if (!ResolveFile())
			return(kFSSpecZero);


		// Get the parent
		theFSRef = GetFSRef();
		theErr   = FSGetCatalogInfo(&theFSRef, kFSCatInfoNone, NULL, NULL, NULL, &parentFSRef);
		NN_ASSERT_NOERR(theErr);
	
		if (theErr == noErr)
			theParent.SetFSRef(parentFSRef);
		}

	return(theParent);
}





//============================================================================
//		NFile::Delete : Delete the file.
//----------------------------------------------------------------------------
void NFile::Delete(void)
{	NFile			theParent;
	FSRef			theFSRef;
	NString			theName;
	OSStatus		theErr;



	// Validate our state
	NN_ASSERT(Exists());



	// Get the state we need
	theParent = GetParent();
	theName   = GetFileName();



	// Delete the file
	//
	// If it's an open file, we can still unlink it.
	theFSRef = GetFSRef();
	theErr   = FSDeleteObject(&theFSRef);

	if (theErr == fBsyErr && IsFile())
		theErr = (OSStatus) unlink(GetPath().GetUTF8());

	NN_ASSERT_NOERR(theErr);



	// Update our state
	//
	// Since the file no longer exists, we need to revert back to FSRef+name.
	*this = theParent.GetChild(theName);
}





//============================================================================
//      NFile::CreateDirectory : Create a directory.
//----------------------------------------------------------------------------
OSStatus NFile::CreateDirectory(void)
{	FSRef		theFSRef;
	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(!Exists());
	NN_ASSERT(mNameIfParent.IsNotEmpty());



	// Create the directory
	theErr = FSCreateDirectoryUnicode(&mFSRef, mNameIfParent.GetSize(), mNameIfParent.GetUTF16(), kFSCatInfoNone, NULL, &theFSRef, NULL, NULL);
	NN_ASSERT_NOERR(theErr);

	if (theErr == noErr)
		SetFSRef(theFSRef);
	
	return(theErr);
}





//============================================================================
//      NFile::ResolveAlias : Resolve an alias file.
//----------------------------------------------------------------------------
bool NFile::ResolveAlias(void)
{	Boolean		wasFolder, wasAlias;
	OSStatus	theErr;



	// Resolve our file
	if (!ResolveFile())
		return(false);



	// Resolve an alias
	//
	// If the file was not an alias, this has no effect.
	wasAlias = false;
	theErr   = FSResolveAliasFile(&mFSRef, true, &wasFolder, &wasAlias);
	NN_ASSERT_NOERR(theErr);
	
	return(wasAlias);
}





//============================================================================
//		NFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
OSStatus NFile::ExchangeWith(const NFile &theTarget)
{	FSRef			thisRef, targetRef;
	OSStatus		theErr;



	// Get the state we need
	thisRef   = GetFSRef();
	targetRef = theTarget.GetFSRef();



	// Exchange the files
	theErr = FSExchangeObjects(&thisRef, &targetRef);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NFile::IsWriteable : Is a file/directory writeable?
//----------------------------------------------------------------------------
bool NFile::IsWriteable(void) const
{	bool				canWrite;
	FSCatalogInfo		theInfo;
	NFile				tmpFile;
	OSStatus			theErr;



	// Validate our parameters
	NN_ASSERT(Exists());



	// Test a file
	if (IsFile())
		{
		tmpFile  = *this;
		theErr   = tmpFile.OpenFile(fsWrPerm);
		canWrite = (theErr == noErr);
		
		if (canWrite)
			tmpFile.CloseFile();
		}
	
	
	// Test a directory
	else
		{
		theErr   = GetCatalogInfo(kFSCatInfoUserPrivs, theInfo);
		canWrite = (theErr == noErr);
		
		if (canWrite)
			canWrite = ((theInfo.userPrivileges & kioACUserNoMakeChangesMask) == 0);
		}
	
	return(canWrite);
}





//============================================================================
//		NFile::IsOpen : Is the file open?
//----------------------------------------------------------------------------
bool NFile::IsOpen(void) const
{


	// Check our state
	return(mForkRef != kInvalidForkRef);
}





//============================================================================
//		NFile::OpenFile : Open the file.
//----------------------------------------------------------------------------
OSStatus NFile::OpenFile(SInt8 thePermissions, bool canCreate)
{	HFSUniStr255		forkName;
	FSRef				theFSRef;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(!IsDirectory());
	NN_ASSERT(!IsOpen());



	// Get the state we need
	theErr = FSGetDataForkName(&forkName);
	NN_ASSERT_NOERR(theErr);



	// Create the file
	if (canCreate && !Exists())
		{
		theErr = FSCreateFileUnicode(&mFSRef, mNameIfParent.GetSize(), mNameIfParent.GetUTF16(), kFSCatInfoNone, NULL, &theFSRef, NULL);
		if (theErr != noErr)
			return(theErr);
		
		SetFSRef(theFSRef);
		}



	// Open the file
	theErr = FSOpenFork(&mFSRef, forkName.length, forkName.unicode, thePermissions, &mForkRef);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NFile::CloseFile : Close the file.
//----------------------------------------------------------------------------
void NFile::CloseFile(void)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(IsFile());
	NN_ASSERT(IsOpen());



	// Close the file
	theErr = FSCloseFork(mForkRef);
	NN_ASSERT_NOERR(theErr);



	// Reset our state
	mForkRef = kInvalidForkRef;
}





//============================================================================
//		NFile::GetForkRef : Get the data fork reference.
//----------------------------------------------------------------------------
SInt16 NFile::GetForkRef(void) const
{


	// Get the data fork reference
	return(mForkRef);
}





//============================================================================
//      NFile::GetForkPosition : Get the data fork position.
//----------------------------------------------------------------------------
SInt64 NFile::GetForkPosition(void) const
{	OSStatus	theErr;
	SInt64		thePos;



    // Validate our state
    NN_ASSERT(IsFile());
    NN_ASSERT(IsOpen());



	// Get the data fork position
	thePos = 0;
	theErr = FSGetForkPosition(mForkRef, &thePos);
	NN_ASSERT_NOERR(theErr);

	return(thePos);
}





//============================================================================
//      NFile::SetForkPosition : Set the data fork position.
//----------------------------------------------------------------------------
OSStatus NFile::SetForkPosition(SInt64 theOffset, UInt16 posMode)
{	OSStatus	theErr;



    // Validate our state
    NN_ASSERT(IsFile());
    NN_ASSERT(IsOpen());



	// Set the data fork position
	theErr = FSSetForkPosition(mForkRef, posMode, theOffset);
	NN_ASSERT_NOERR(theErr);

	return(theErr);
}





//============================================================================
//      NFile::GetForkSize : Get the data fork size.
//----------------------------------------------------------------------------
SInt64 NFile::GetForkSize(void) const
{	SInt64		theSize;
	OSStatus	theErr;



    // Validate our state
    NN_ASSERT(IsFile());
    NN_ASSERT(IsOpen());



	// Get the data fork size
	theSize = 0;
	theErr  = FSGetForkSize(mForkRef, &theSize);
	NN_ASSERT_NOERR(theErr);

	return(theSize);
}





//============================================================================
//      NFile::SetForkSize : Set the data fork size.
//----------------------------------------------------------------------------
OSStatus NFile::SetForkSize(SInt64 theOffset, UInt16 posMode)
{	OSStatus	theErr;



    // Validate our state
    NN_ASSERT(IsFile());
    NN_ASSERT(IsOpen());



	// Set the data fork size
	theErr = FSSetForkSize(mForkRef, posMode, theOffset);
	NN_ASSERT_NOERR(theErr);

	return(theErr);
}





//============================================================================
//		NFile::ReadFile : Read from the file.
//----------------------------------------------------------------------------
OSStatus NFile::ReadFile(SInt64 dataSize, void *dataPtr, UInt64 &numRead, SInt64 theOffset, SInt16 posMode)
{	ByteCount		theSize;
	OSStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(dataSize <= ULONG_MAX);
	NN_ASSERT(IsFile());
	NN_ASSERT(IsOpen());



	// Read from the file
	theSize = 0;
	theErr = FSReadFork(mForkRef, posMode, theOffset, (ByteCount) dataSize, dataPtr, &theSize);
    NN_ASSERT(theErr == noErr || theErr == eofErr);

    numRead = theSize;
	
	return(theErr);
}





//============================================================================
//		NFile::ReadFile : Read from the file.
//----------------------------------------------------------------------------
OSStatus NFile::WriteFile(SInt64 dataSize, const void *dataPtr, UInt64 &numWritten, SInt64 theOffset, SInt16 posMode)
{	ByteCount		theSize;
	OSStatus		theErr;



	// Validate our parameters and state
	NN_ASSERT(dataSize <= ULONG_MAX);
	NN_ASSERT(IsFile());
	NN_ASSERT(IsOpen());



	// Write to the file
	theErr = FSWriteFork(mForkRef, posMode, theOffset, (ByteCount) dataSize, dataPtr, &theSize);
	NN_ASSERT_NOERR(theErr);

	numWritten = (theErr == noErr) ? theSize : 0;
	
	return(theErr);
}





//============================================================================
//		NFile::= : Assignment operator.
//----------------------------------------------------------------------------
const NFile& NFile::operator = (const NFile &theFile)
{


	// Assign the file
	mFSRef        = theFile.mFSRef;
	mNameIfParent = theFile.mNameIfParent;
	
	return(*this);
}





//============================================================================
//		NFile::Compare : Compare two objects.
//----------------------------------------------------------------------------
#pragma mark -
CFComparisonResult NFile::Compare(const NComparable &theObject) const
{	const NFile		*theFile = dynamic_cast<const NFile*>(&theObject);
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theFile != NULL);



	// Resolve the files
	if (!ResolveFile() || !theFile->ResolveFile())
		return(NComparable::Compare(theObject));



	// Compare the files
	theErr = FSCompareFSRefs(&mFSRef, &theFile->mFSRef);

	if (theErr == noErr)
		return(kCFCompareEqualTo);
	else
		return(GET_CF_COMPARE(GetPath(), theFile->GetPath()));
}





//============================================================================
//		NFile::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NFile::InitializeSelf(void)
{


	// Initialize ourselves
	mForkRef = kInvalidForkRef;
	mFSRef   = kFSRefZero;
}





//============================================================================
//		NFile::ResolveFile : Resolve the file.
//----------------------------------------------------------------------------
bool NFile::ResolveFile(void) const
{	FSRef		theFSRef;
	OSStatus	theErr;



	// Resolve the file
	//
	// Since FSRefs can only reference files that exist, we represent non-existent
	// files by storing an FSRef to the parent directory and a file name.
	if (mNameIfParent.IsNotEmpty())
		{
		theErr = FSMakeFSRefUnicode(&mFSRef, mNameIfParent.GetSize(), mNameIfParent.GetUTF16(), kUnicodeUTF16Format, &theFSRef);
		if (theErr != noErr)
			return(false);
		
		mFSRef = theFSRef;
		mNameIfParent.Clear();
		}
	
	return(true);
}


