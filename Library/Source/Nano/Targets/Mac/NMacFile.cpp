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
NString NTargetFile::SetName(const NString &thePath, const NString &theName, bool renameFile)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::GetSize : Get a file's size.
//----------------------------------------------------------------------------
SInt64 NTargetFile::GetSize(const NString &thePath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::SetSize : Set a file's size.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetSize(const NString &thePath, SInt64 theSize)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::GetChild : Get the child of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetChild(const NString &thePath, const NString &theChild)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::GetParent : Get the parent of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetParent(const NString &thePath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::Delete : Delete a file.
//----------------------------------------------------------------------------
void NTargetFile::Delete(const NString &thePath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::CreateDirectory : Create a directory.
//----------------------------------------------------------------------------
NStatus NTargetFile::CreateDirectory(const NString &thePath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NTargetFile::ExchangeWith(const NString &srcPath, const NString &targetPath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::Open : Open a file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::Open(const NString &thePath, NFilePermission thePermissions)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::Close : Close a file.
//----------------------------------------------------------------------------
void NTargetFile::Close(NFileRef theFile)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::GetPosition : Get the read/write position.
//----------------------------------------------------------------------------
SInt64 NTargetFile::GetPosition(NFileRef theFile)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::SetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition filePos)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::Read : Read from a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Read(NFileRef theFile, SInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition filePos)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::Write : Write to a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Write(NFileRef theFile, SInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition filePos)
{
	// dair, to do
}




