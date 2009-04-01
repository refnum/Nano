/*	NAME:
		NMacFile.cpp

	DESCRIPTION:
		Mac file object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFile.h"





//============================================================================
//		NFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
bool NFile::IsFile(void) const
{
	// dair, to do
	return(false);
}





//============================================================================
//		NFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NFile::IsDirectory(void) const
{
	// dair, to do
	return(false);
}





//============================================================================
//		NFile::IsWriteable : Is the file writeable?
//----------------------------------------------------------------------------
bool NFile::IsWriteable(void) const
{
	// dair, to do
	return(false);
}





//============================================================================
//		NFile::Exists : Does the file exist?
//----------------------------------------------------------------------------
bool NFile::Exists(void) const
{
	// dair, to do
	return(false);
}





//============================================================================
//		NFile::GetName : Get the file name.
//----------------------------------------------------------------------------
NString NFile::GetName(bool displayName) const
{
}





//============================================================================
//		NFile::SetName : Set the file name.
//----------------------------------------------------------------------------
NStatus NFile::SetName(const NString &theName, bool renameFile)
{
}





//============================================================================
//		NFile::GetSize : Get the file size.
//----------------------------------------------------------------------------
SInt64 NFile::GetSize(void) const
{
}





//============================================================================
//		NFile::SetSize : Set the file size.
//----------------------------------------------------------------------------
NStatus NFile::SetSize(SInt64 theSize)
{
}





//============================================================================
//		NFile::GetChild : Get the child of a file.
//----------------------------------------------------------------------------
NFile NFile::GetChild(const NString &fileName) const
{
}





//============================================================================
//		NFile::GetParent : Get the parent of a file.
//----------------------------------------------------------------------------
NFile NFile::GetParent(void) const
{
}





//============================================================================
//		NFile::Delete : Delete the file.
//----------------------------------------------------------------------------
void NFile::Delete(void)
{
}





//============================================================================
//		NFile::CreateDirectory : Create the directory.
//----------------------------------------------------------------------------
NStatus NFile::CreateDirectory(void)
{
}





//============================================================================
//		NFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NFile::ExchangeWith(const NFile &theTarget)
{
}





//============================================================================
//		NFile::Open : Open the file.
//----------------------------------------------------------------------------
NStatus NFile::Open(NFilePermission thePermissions, bool canCreate)
{
}





//============================================================================
//		NFile::Close : Close the file.
//----------------------------------------------------------------------------
void NFile::Close(void)
{
}





//============================================================================
//		NFile::GetPosition : Get the read/write position.
//----------------------------------------------------------------------------
SInt64 NFile::GetPosition(void) const
{
}





//============================================================================
//		NFile::SetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NFile::SetPosition(SInt64 theOffset, NFilePosition filePos)
{
}





//============================================================================
//		NFile::Read : Read data from the file.
//----------------------------------------------------------------------------
NStatus NFile::Read(SInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition filePos)
{
}





//============================================================================
//		NFile::Write : Write data to the file.
//----------------------------------------------------------------------------
NStatus NFile::Write(SInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition filePos)
{
}


