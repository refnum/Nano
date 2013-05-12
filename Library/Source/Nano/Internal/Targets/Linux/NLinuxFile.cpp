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
#include "NTargetFile.h"





//============================================================================
//      NTargetFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetFile::IsFile(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::IsFile not implemented!");
	return(false);
}





//============================================================================
//      NTargetFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NTargetFile::IsDirectory(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::IsDirectory not implemented!");
	return(false);
}





//============================================================================
//      NTargetFile::IsLink : Is this a link?
//----------------------------------------------------------------------------
bool NTargetFile::IsLink(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::IsLink not implemented!");
	return(false);
}





//============================================================================
//      NTargetFile::IsWriteable : Is a file writeable?
//----------------------------------------------------------------------------
bool NTargetFile::IsWriteable(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::IsWriteable not implemented!");
	return(false);
}





//============================================================================
//      NTargetFile::Exists : Does a file exist?
//----------------------------------------------------------------------------
bool NTargetFile::Exists(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::Exists not implemented!");
	return(false);
}





//============================================================================
//      NTargetFile::GetName : Get a file's name.
//----------------------------------------------------------------------------
NString NTargetFile::GetName(const NString &thePath, bool displayName)
{


	// dair, to do
	NN_LOG("NTargetFile::GetName not implemented!");
	return("");
}





//============================================================================
//      NTargetFile::SetName : Set a file's name.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetName(const NString &thePath, const NString &fileName, bool renameFile, bool isPath, NString &newPath)
{


	// dair, to do
	NN_LOG("NTargetFile::SetName not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetFile::GetSize : Get a file's size.
//----------------------------------------------------------------------------
UInt64 NTargetFile::GetSize(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::GetSize not implemented!");
	return(0);
}





//============================================================================
//      NTargetFile::SetSize : Set a file's size.
//----------------------------------------------------------------------------
NStatus NTargetFile::SetSize(const NString &thePath, NFileRef /*theFile*/, UInt64 theSize)
{


	// dair, to do
	NN_LOG("NTargetFile::SetSize not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetFile::GetCreationTime : Get the creation time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetCreationTime(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::GetCreationTime not implemented!");
	return(0.0);
}





//============================================================================
//      NTargetFile::GetAccessTime : Get the access time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetAccessTime(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::GetAccessTime not implemented!");
	return(0.0);
}





//============================================================================
//      NTargetFile::GetModificationTime : Get the modification time.
//----------------------------------------------------------------------------
NDate NTargetFile::GetModificationTime(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::GetModificationTime not implemented!");
	return(0.0);
}





//============================================================================
//      NTargetFile::GetChild : Get the child of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetChild(const NString &thePath, const NString &fileName)
{


	// dair, to do
	NN_LOG("NTargetFile::GetChild not implemented!");
	return("");
}





//============================================================================
//      NTargetFile::GetParent : Get the parent of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetParent(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::GetParent not implemented!");
	return("");
}





//============================================================================
//      NTargetFile::GetTarget : Get the target of a path.
//----------------------------------------------------------------------------
NString NTargetFile::GetTarget(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::GetTarget not implemented!");
	return("");
}





//============================================================================
//      NTargetFile::GetChildren : Get the children of a path.
//----------------------------------------------------------------------------
NFileList NTargetFile::GetChildren(const NString &thePath)
{	NFileList	theFiles;



	// dair, to do
	NN_LOG("NTargetFile::GetChildren not implemented!");
	return(theFiles);
}





//============================================================================
//      NTargetFile::Delete : Delete a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::Delete(const NString &thePath, bool moveToTrash)
{


	// dair, to do
	NN_LOG("NTargetFile::Delete not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetFile::GetDirectory : Get a directory.
//----------------------------------------------------------------------------
NFile NTargetFile::GetDirectory(NDirectoryDomain theDomain, NDirectoryLocation theLocation)
{


	// dair, to do
	NN_LOG("NTargetFile::GetDirectory not implemented!");
	return(NFile());
}





//============================================================================
//      NTargetFile::CreateDirectory : Create a directory.
//----------------------------------------------------------------------------
NStatus NTargetFile::CreateDirectory(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::CreateDirectory not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NTargetFile::ExchangeWith(const NString &srcPath, const NString &dstPath)
{


	// dair, to do
	NN_LOG("NTargetFile::ExchangeWith not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetFile::UnmountVolume : Unmount a volume.
//----------------------------------------------------------------------------
NStatus NTargetFile::UnmountVolume(const NString &thePath)
{


	// dair, to do
	NN_LOG("NTargetFile::UnmountVolume not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetFile::FileOpen : Open a file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::FileOpen(const NString &thePath, NFilePermission thePermission)
{


	// dair, to do
	NN_LOG("NTargetFile::FileOpen not implemented!");
	return(0);
}





//============================================================================
//      NTargetFile::FileClose : Close a file.
//----------------------------------------------------------------------------
void NTargetFile::FileClose(NFileRef theFile)
{


	// dair, to do
	NN_LOG("NTargetFile::FileClose not implemented!");
}





//============================================================================
//      NTargetFile::FileGetPosition : Get the read/write position.
//----------------------------------------------------------------------------
UInt64 NTargetFile::FileGetPosition(NFileRef theFile)
{


	// dair, to do
	NN_LOG("NTargetFile::FileGetPosition not implemented!");
	return(0);
}





//============================================================================
//      NTargetFile::FileSetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NTargetFile::FileSetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition thePosition)
{


	// dair, to do
	NN_LOG("NTargetFile::FileSetPosition not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetFile::FileRead : Read from a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::FileRead(NFileRef theFile, UInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition thePosition, NFileFlags /*theFlags*/)
{


	// dair, to do
	NN_LOG("NTargetFile::FileRead not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetFile::FileWrite : Write to a file.
//----------------------------------------------------------------------------
NStatus NTargetFile::FileWrite(NFileRef theFile, UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition thePosition, NFileFlags /*theFlags*/)
{


	// dair, to do
	NN_LOG("NTargetFile::FileWrite not implemented!");
	return(kNErrPermission);
}





//============================================================================
//      NTargetFile::MapOpen : Open a memory-mapped file.
//----------------------------------------------------------------------------
NFileRef NTargetFile::MapOpen(const NFile &theFile, NMapAccess theAccess)
{


	// dair, to do
	NN_LOG("NTargetFile::MapOpen not implemented!");
	return(0);
}





//============================================================================
//      NTargetFile::MapClose : Close a memory-mapped file.
//----------------------------------------------------------------------------
void NTargetFile::MapClose(NFileRef theFile)
{


	// dair, to do
	NN_LOG("NTargetFile::MapClose not implemented!");
}





//============================================================================
//      NTargetFile::MapFetch : Fetch a page from a memory-mapped file.
//----------------------------------------------------------------------------
void *NTargetFile::MapFetch(NFileRef theFile, NMapAccess theAccess, UInt64 theOffset, UInt32 theSize, bool noCache)
{


	// dair, to do
	NN_LOG("NTargetFile::MapFetch not implemented!");
	return(NULL);
}





//============================================================================
//      NTargetFile::MapDiscard : Discard a page from a memory-mapped file.
//----------------------------------------------------------------------------
void NTargetFile::MapDiscard(NFileRef theFile, NMapAccess theAccess, const void *thePtr, UInt32 theSize)
{


	// dair, to do
	NN_LOG("NTargetFile::MapDiscard not implemented!");
}





//============================================================================
//      NTargetFile::BundleGetInfo : Get a bundle's Info.plist.
//----------------------------------------------------------------------------
NDictionary NTargetFile::BundleGetInfo(const NFile &theBundle)
{


	// dair, to do
	NN_LOG("NTargetFile::BundleGetInfo not implemented!");
	return(NDictionary());
}





//============================================================================
//      NTargetFile::BundleGetExecutable : Get a bundle executable.
//----------------------------------------------------------------------------
NFile NTargetFile::BundleGetExecutable(const NFile &theBundle, const NString &theName)
{


	// dair, to do
	NN_LOG("NTargetFile::BundleGetExecutable not implemented!");
	return(NFile());
}





//============================================================================
//      NTargetFile::BundleGetResources : Get a bundle's Resources directory.
//----------------------------------------------------------------------------
NFile NTargetFile::BundleGetResources(const NFile &theBundle)
{


	// dair, to do
	NN_LOG("NTargetFile::BundleGetResources not implemented!");
	return(NFile());
}


