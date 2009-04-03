/*	NAME:
		NFile.cpp

	DESCRIPTION:
		File object.

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
//		NFile::NFile : Constructor.
//----------------------------------------------------------------------------
NFile::NFile(const NString &thePath)
{


	// Initialize ourselves
	InitializeSelf(thePath);
}





//============================================================================
//		NFile::NFile : Constructor.
//----------------------------------------------------------------------------
NFile::NFile(const NFile &theFile)
		: NComparable<NFile>()
{


	// Initialise ourselves
	InitializeSelf();
	CloneFile(theFile);
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
		Close();
}





//============================================================================
//		NFile::IsValid : Is the file valid?
//----------------------------------------------------------------------------
bool NFile::IsValid(void) const
{


	// Check our state
	return(mPath.IsNotEmpty());
}





//============================================================================
//        NFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
bool NFile::IsFile(void) const
{
    // dair, to do
    return(false);
}





//============================================================================
//        NFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NFile::IsDirectory(void) const
{
    // dair, to do
    return(false);
}





//============================================================================
//        NFile::IsWriteable : Is the file writeable?
//----------------------------------------------------------------------------
bool NFile::IsWriteable(void) const
{
    // dair, to do
    return(false);
}





//============================================================================
//		NFile::IsOpen : Is the file open?
//----------------------------------------------------------------------------
bool NFile::IsOpen(void) const
{


	// Check our state
	return(mFileRef != kNFileRefNone);
}





//============================================================================
//        NFile::Exists : Does the file exist?
//----------------------------------------------------------------------------
bool NFile::Exists(void) const
{
    // dair, to do
    return(false);
}





//============================================================================
//		NFile::NotExists : Does the file not exist?
//----------------------------------------------------------------------------
bool NFile::NotExists(void) const
{


	// Check the file
	return(!Exists());
}





//============================================================================
//		NFile::Compare : Compare the value.
//----------------------------------------------------------------------------
#pragma mark -
NComparison NFile::Compare(const NFile &theValue) const
{


	// Compare the value
	return(mPath.Compare(theValue.mPath));
}





//============================================================================
//		NFile::GetPath : Get the path.
//----------------------------------------------------------------------------
NString NFile::GetPath(void) const
{


	// Get the path
	return(mPath);
}





//============================================================================
//		NFile::SetPath : Set the path.
//----------------------------------------------------------------------------
void NFile::SetPath(const NString &thePath)
{


	// Set the path
	mPath = thePath;
}





//============================================================================
//        NFile::GetName : Get the file name.
//----------------------------------------------------------------------------
NString NFile::GetName(bool displayName) const
{
}





//============================================================================
//        NFile::SetName : Set the file name.
//----------------------------------------------------------------------------
NStatus NFile::SetName(const NString &theName, bool renameFile)
{
}





//============================================================================
//		NFile::GetExtension : Set the file extension.
//----------------------------------------------------------------------------
NString NFile::GetExtension(void) const
{
}





//============================================================================
//		NFile::SetExtension : Set the file extension.
//----------------------------------------------------------------------------
NStatus NFile::SetExtension(const NString &theExtension, bool renameFile)
{
}





//============================================================================
//        NFile::GetSize : Get the file size.
//----------------------------------------------------------------------------
SInt64 NFile::GetSize(void) const
{
}





//============================================================================
//        NFile::SetSize : Set the file size.
//----------------------------------------------------------------------------
NStatus NFile::SetSize(SInt64 theSize)
{
}





//============================================================================
//        NFile::GetChild : Get the child of a file.
//----------------------------------------------------------------------------
NFile NFile::GetChild(const NString &fileName) const
{
}





//============================================================================
//        NFile::GetParent : Get the parent of a file.
//----------------------------------------------------------------------------
NFile NFile::GetParent(void) const
{
}





//============================================================================
//        NFile::Delete : Delete the file.
//----------------------------------------------------------------------------
void NFile::Delete(void)
{
}





//============================================================================
//        NFile::CreateDirectory : Create the directory.
//----------------------------------------------------------------------------
NStatus NFile::CreateDirectory(void)
{
}





//============================================================================
//        NFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NFile::ExchangeWith(const NFile &theTarget)
{
}





//============================================================================
//        NFile::Open : Open the file.
//----------------------------------------------------------------------------
NStatus NFile::Open(NFilePermission thePermissions, bool canCreate)
{
}





//============================================================================
//        NFile::Close : Close the file.
//----------------------------------------------------------------------------
void NFile::Close(void)
{
}





//============================================================================
//        NFile::GetPosition : Get the read/write position.
//----------------------------------------------------------------------------
SInt64 NFile::GetPosition(void) const
{
}





//============================================================================
//        NFile::SetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NFile::SetPosition(SInt64 theOffset, NFilePosition filePos)
{
}





//============================================================================
//        NFile::Read : Read data from the file.
//----------------------------------------------------------------------------
NStatus NFile::Read(SInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NFilePosition filePos)
{
}





//============================================================================
//        NFile::Write : Write data to the file.
//----------------------------------------------------------------------------
NStatus NFile::Write(SInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition filePos)
{
}





//============================================================================
//		NFile::= : Assignment operator.
//----------------------------------------------------------------------------
const NFile& NFile::operator = (const NFile &theFile)
{


	// Assign the file
	if (this != &theFile)
		CloneFile(theFile);
	
	return(*this);
}





//============================================================================
//		NFile::InitializeSelf : Initialize the file.
//----------------------------------------------------------------------------
#pragma mark -
void NFile::InitializeSelf(const NString &thePath)
{


	// Initialize ourselves
	mPath    = thePath;
	mFileRef = kNFileRefNone;
}





//============================================================================
//		NFile::CloneFile : Clone a file.
//----------------------------------------------------------------------------
void NFile::CloneFile(const NFile &theFile)
{


	// Reset our state
	if (IsOpen())
		Close();



	// Clone the file
	//
	// File references are not copied between files; a file reference
	// is owned by the file object which opened it.
	mPath = theFile.mPath;
}
















