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
#include "NTargetFile.h"





//============================================================================
//      NTargetFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetFile::IsFile(const NString &thePath)
{	DWORD		fileInfo;
	bool		isFile;



	// Check the path
	fileInfo = GetFileAttributes(thePath.GetUTF16());
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
	fileInfo = GetFileAttributes(thePath.GetUTF16());
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
	fileInfo = GetFileAttributes(thePath.GetUTF16());
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
	fileInfo    = GetFileAttributes(thePath.GetUTF16());
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
	fileInfo  = GetFileAttributes(thePath.GetUTF16());
	doesExist = (fileInfo != INVALID_FILE_ATTRIBUTES);

	return(doesExist);
}





//============================================================================
//      NTargetFile::GetName : Get a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::GetName(const NString &thePath, bool displayName)
{
	// dair, to do
	return("TODO");
}





//============================================================================
//      NTargetFile::SetName : Set a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::SetName(const NString &thePath, const NString &fileName, bool renameFile)
{
	// dair, to do
	return("TODO");
}





//============================================================================
//      NTargetFile::GetSize : Get a file's size.
//----------------------------------------------------------------------------
UInt64 NTargetFile::GetSize(const NString &thePath)
{	WIN32_FILE_ATTRIBUTE_DATA		fileInfo;
	UInt64							theSize;
	
	
	
	// Get the size
	theSize = 0;

	if (GetFileAttributesEx(thePath.GetUTF16(), GetFileExInfoStandard, &fileInfo))
		theSize = ((UInt64) fileInfo.nFileSizeLow) + (((UInt64) fileInfo.nFileSizeHigh) << 32);

	return(theSize);
}





//============================================================================
//      NTargetFile::SetSize : Set a file's size.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetSize(const NString &thePath, UInt64 theSize)
{
	// dair, to do
	return(kNErrParam);
}





//============================================================================
//      NTargetFile::GetChild : Get the child of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetChild(const NString &thePath, const NString &fileName)
{
	// dair, to do
	return("TODO");
}





//============================================================================
//      NTargetFile::GetParent : Get the parent of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetParent(const NString &thePath)
{
	// dair, to do
	return("TODO");
}





//============================================================================
//      NTargetFile::GetTarget : Get the target of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetTarget(const NString &thePath)
{
	// dair, to do
	return("TODO");
}





//============================================================================
//      NTargetFile::GetChildren : Get the chilren of a path.
//----------------------------------------------------------------------------
NFileList NTargetFile::GetChildren(const NString &thePath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::Delete : Delete a file.
//----------------------------------------------------------------------------
void NTargetFile::Delete(const NString &thePath)
{	BOOL	wasOK;



	// Delete the file
	wasOK = DeleteFile(thePath.GetUTF16());
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetFile::GetDirectory : Get a directory.
//----------------------------------------------------------------------------
NFile NTargetFile::GetDirectory(NDirectoryDomain theDomain, NDirectoryLocation theLocation)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::CreateDirectory : Create a directory.
//----------------------------------------------------------------------------
NStatus NTargetFile::CreateDirectory(const NString &thePath)
{
	// dair, to do
	return(kNErrParam);
}





//============================================================================
//      NTargetFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NTargetFile::ExchangeWith(const NString &srcPath, const NString &dstPath)
{
	// dair, to do
	return(kNErrParam);
}





//============================================================================
//      NTargetFile::Open : Open a file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::Open(const NString &thePath, NFilePermission thePermission)
{
	// dair, to do
	return(kNFileRefNone);
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
UInt64 NTargetFile::GetPosition(NFileRef theFile)
{
	// dair, to do
	return(0);
}





//============================================================================
//      NTargetFile::SetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition thePosition)
{
	// dair, to do
	return(kNErrParam);
}





//============================================================================
//      NTargetFile::Read : Read from a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Read(NFileRef theFile, UInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition thePosition, NFileFlags theFlags)
{
	// dair, to do
	return(kNErrParam);
}





//============================================================================
//      NTargetFile::Write : Write to a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Write(NFileRef theFile, UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition thePosition, NFileFlags theFlags)
{
	// dair, to do
	return(kNErrParam);
}





//============================================================================
//      NTargetFile::MapOpen : Open a memory-mapped file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::MapOpen(const NFile &theFile, NMapAccess theAccess)
{
	// dair, to do
	return(kNFileRefNone);
}





//============================================================================
//      NTargetFile::MapClose : Close a memory-mapped file.
//----------------------------------------------------------------------------
void NTargetFile::MapClose(NFileRef theFile)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::MapFetch : Fetch a page from a memory-mapped file.
//----------------------------------------------------------------------------
void *NTargetFile::MapFetch(NFileRef theFile, NMapAccess theAccess, UInt64 theOffset, UInt32 theSize, bool noCache)
{
	// dair, to do
	return(NULL);
}





//============================================================================
//      NTargetFile::MapDiscard : Discard a page from a memory-mapped file.
//----------------------------------------------------------------------------
void NTargetFile::MapDiscard(NFileRef theFile, NMapAccess theAccess, const void *thePtr, UInt32 theSize)
{
	// dair, to do
}




