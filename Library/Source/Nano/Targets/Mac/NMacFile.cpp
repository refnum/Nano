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
#include <dirent.h>

#include "NCFString.h"

#include "NMacTarget.h"
#include "NTargetFile.h"





//============================================================================
//      NTargetFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
bool NTargetFile::IsFile(const NString &thePath)
{	struct stat64		fileInfo;
	NStatus				sysErr;
	bool				isFile;



	// Check the path
	sysErr = stat64(thePath.GetUTF8(), &fileInfo);
	isFile = (sysErr == kNoErr && S_ISREG(fileInfo.st_mode));
	
	return(isFile);
}





//============================================================================
//      NTargetFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NTargetFile::IsDirectory(const NString &thePath)
{	struct stat64		fileInfo;
	NStatus				sysErr;
	bool				isFile;



	// Check the path
	sysErr = stat64(thePath.GetUTF8(), &fileInfo);
	isFile = (sysErr == kNoErr && S_ISDIR(fileInfo.st_mode));
	
	return(isFile);
}





//============================================================================
//      NTargetFile::IsWriteable : Is a file writeable?
//----------------------------------------------------------------------------
bool NTargetFile::IsWriteable(const NString &thePath)
{	bool				isWriteable;
	struct stat64		fileInfo;
	FILE				*tmpFile;
	DIR					*tmpDir;
	NStatus				sysErr;



	// Check the path
	isWriteable = false;
	sysErr      = stat64(thePath.GetUTF8(), &fileInfo);

	if (sysErr != kNoErr)
		return(isWriteable);



	// Check a file
	if (S_ISREG(fileInfo.st_mode))
		{
		tmpFile = fopen(thePath.GetUTF8(), "wb");
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
{	struct stat64		fileInfo;
	bool				doesExist;
	NStatus				sysErr;



	// Check the path
	sysErr    = stat64(thePath.GetUTF8(), &fileInfo);
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
	if (displayName)
		{
		if (cfURL.Set(CFURLCreateWithFileSystemPath(NULL, NCFString(thePath), kCFURLPOSIXPathStyle, IsDirectory(thePath))))
			{
			theErr = LSCopyDisplayNameForURL(cfURL, &cfString);
			if (theErr == noErr)
				theName = NCFString(cfString, true);
			}
		}
	
	
	// Get the file name
	else
		{
		slashPos = thePath.Find("/", kNStringBackwards);
		if (slashPos.IsNotEmpty())
			theName = thePath.GetRight(thePath.GetSize() - slashPos.GetNext());
		}

	return(theName);
}





//============================================================================
//      NTargetFile::SetName : Set a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::SetName(const NString &thePath, const NString &fileName, bool renameFile)
{	NString		newPath;
	NStatus		sysErr;



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
SInt64 NTargetFile::GetSize(const NString &thePath)
{	struct stat64		fileInfo;
	SInt64				theSize;
	NStatus				sysErr;



	// Get the file size
	theSize = 0;
	sysErr  = stat64(thePath.GetUTF8(), &fileInfo);
	NN_ASSERT_NOERR(sysErr);

	if (sysErr == kNoErr)
		theSize = fileInfo.st_size;

	return(theSize);
}





//============================================================================
//      NTargetFile::SetSize : Set a file's size.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetSize(NFileRef theFile, SInt64 theSize)
{	OSStatus	theErr;



	// Set the size
	theErr = FSSetForkSize(NMacTarget::GetFileRefNum(theFile), fsFromStart, theSize);
	NN_ASSERT_NOERR(theErr);

	return(NMacTarget::GetStatus(theErr));
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

	if (slashPos.IsNotEmpty() && slashPos.GetLocation() >= 1)
		theParent = thePath.GetLeft(slashPos.GetLocation() - 1);
	else
		theParent = thePath;
	
	return(theParent);
}





//============================================================================
//      NTargetFile::Delete : Delete a file.
//----------------------------------------------------------------------------
void NTargetFile::Delete(const NString &thePath)
{


	// Delete the file
	unlink(thePath.GetUTF8());
}





//============================================================================
//      NTargetFile::CreateDirectory : Create a directory.
//----------------------------------------------------------------------------
NStatus NTargetFile::CreateDirectory(const NString &thePath)
{	NStatus		sysErr;



	// Create the directory
	sysErr = mkdir(thePath.GetUTF8(), S_IRWXU | S_IRWXG);
	NN_ASSERT_NOERR(sysErr);
	
	return(sysErr);
}





//============================================================================
//      NTargetFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NTargetFile::ExchangeWith(const NString &srcPath, const NString &dstPath)
{	FSRef		srcFSRef, dstFSRef;
	NStatus		theErr;



	// Get the state we need
	theErr  = NMacTarget::GetFSRef(srcPath, srcFSRef);
	theErr |= NMacTarget::GetFSRef(dstPath, dstFSRef);
	
	NN_ASSERT_NOERR(theErr);



	// Exchange the files
	if (theErr == noErr)
		{
		theErr = NMacTarget::GetStatus(FSExchangeObjects(&srcFSRef, &dstFSRef));
		NN_ASSERT_NOERR(theErr);
		}
	
	return(theErr);
}





//============================================================================
//      NTargetFile::Open : Open a file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::Open(const NString &thePath, NFilePermission thePermission)
{	HFSUniStr255		forkName;
	FSRef				theFSRef;
	FSIORefNum			fileRef;
	OSStatus			theErr;



	// Validate our state
	NN_ASSERT(sizeof(FSIORefNum) <= sizeof(NFileRef));



	// Get the state we need
	fileRef = 0;
	theErr  = FSGetDataForkName(&forkName);
	NN_ASSERT_NOERR(theErr);



	// Create the file
	if (!Exists(thePath))
		{
		theErr = creat(thePath.GetUTF8(), S_IRWXU | S_IRWXG);
		NN_ASSERT_NOERR(theErr);
		}



	// Open the file
	theErr  = NMacTarget::GetFSRef(thePath, theFSRef);
	theErr |= FSOpenFork(&theFSRef, forkName.length, forkName.unicode,
							NMacTarget::GetFilePermission(thePermission), &fileRef);

	NN_ASSERT_NOERR(theErr);

	return((NFileRef) fileRef);
}





//============================================================================
//      NTargetFile::Close : Close a file.
//----------------------------------------------------------------------------
void NTargetFile::Close(NFileRef theFile)
{	OSStatus	theErr;



	// Close the file
	theErr = FSCloseFork(NMacTarget::GetFileRefNum(theFile));
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//      NTargetFile::GetPosition : Get the read/write position.
//----------------------------------------------------------------------------
SInt64 NTargetFile::GetPosition(NFileRef theFile)
{	SInt64		thePos;
	OSStatus	theErr;



	// Get the position
	thePos = 0;
	theErr = FSGetForkPosition(NMacTarget::GetFileRefNum(theFile), &thePos);
	NN_ASSERT_NOERR(theErr);
	
	return(thePos);
}





//============================================================================
//      NTargetFile::SetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition thePosition)
{	OSStatus	theErr;



	// Set the position
	theErr = FSSetForkPosition(NMacTarget::GetFileRefNum(theFile), NMacTarget::GetFilePosition(thePosition), theOffset);
	NN_ASSERT_NOERR(theErr);

	return(NMacTarget::GetStatus(theErr));
}





//============================================================================
//      NTargetFile::Read : Read from a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Read(NFileRef theFile, SInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition thePosition)
{	ByteCount		actualSize;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theSize <= kUInt32Max);



	// Read from the file
	actualSize = 0;
	theErr     = FSReadFork(NMacTarget::GetFileRefNum(theFile),
							NMacTarget::GetFilePosition(thePosition),
							theOffset, (ByteCount) theSize, thePtr, &actualSize);

    NN_ASSERT(theErr == noErr || theErr == eofErr);
    numRead = actualSize;

	return(NMacTarget::GetStatus(theErr));
}





//============================================================================
//      NTargetFile::Write : Write to a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Write(NFileRef theFile, SInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition thePosition)
{	ByteCount		actualSize;
	OSStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theSize <= kUInt32Max);



	// Write to the file
	actualSize = 0;
	theErr     = FSWriteFork(NMacTarget::GetFileRefNum(theFile),
							 NMacTarget::GetFilePosition(thePosition),
							 theOffset, (ByteCount) theSize, thePtr, &actualSize);

    NN_ASSERT_NOERR(theErr);
	numWritten = actualSize;

	return(NMacTarget::GetStatus(theErr));
}





