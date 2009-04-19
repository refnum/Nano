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
bool NTargetFile::IsFile(const NString &thePath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NTargetFile::IsDirectory(const NString &thePath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::IsWriteable : Is a file writeable?
//----------------------------------------------------------------------------
bool NTargetFile::IsWriteable(const NString &thePath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::Exists : Does a file exist?
//----------------------------------------------------------------------------
bool NTargetFile::Exists(const NString &thePath)
{
	// dair, to do
}





//============================================================================
//      NTargetFile::GetName : Get a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::GetName(const NString &thePath, bool displayName)
{
	// dair, to do
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




