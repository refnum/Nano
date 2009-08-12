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
#include "NTargetFile.h"
#include "NEncoder.h"
#include "NFile.h"





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
DEFINE_NENCODABLE(NFile);





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
		: NEncodable(), NComparable<NFile>()
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
	return(!mPath.IsEmpty());
}





//============================================================================
//        NFile::IsFile : Is this a file?
//----------------------------------------------------------------------------
bool NFile::IsFile(void) const
{


	// Check the path
	return(Exists() && NTargetFile::IsFile(mPath));
}





//============================================================================
//        NFile::IsDirectory : Is this a directory?
//----------------------------------------------------------------------------
bool NFile::IsDirectory(void) const
{


	// Check the path
	return(Exists() && NTargetFile::IsDirectory(mPath));
}





//============================================================================
//        NFile::IsWriteable : Is the file writeable?
//----------------------------------------------------------------------------
bool NFile::IsWriteable(void) const
{


	// Check the path
	return(NTargetFile::IsWriteable(mPath));
}





//============================================================================
//		NFile::IsOpen : Is the file open?
//----------------------------------------------------------------------------
bool NFile::IsOpen(void) const
{


	// Check our state
	return(mFile != kNFileRefNone);
}





//============================================================================
//        NFile::Exists : Does the file exist?
//----------------------------------------------------------------------------
bool NFile::Exists(void) const
{


	// Check the path
	return(NTargetFile::Exists(mPath));
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


	// Update our state
	if (IsOpen())
		Close();



	// Set the path
	mPath = thePath;
}





//============================================================================
//        NFile::GetName : Get the file name.
//----------------------------------------------------------------------------
NString NFile::GetName(bool displayName) const
{


	// Get the name
	return(NTargetFile::GetName(mPath, displayName));
}





//============================================================================
//        NFile::SetName : Set the file name.
//----------------------------------------------------------------------------
NStatus NFile::SetName(const NString &theName, bool renameFile)
{	NString		newPath;
	NStatus		theErr;



	// Set the name
	newPath = NTargetFile::SetName(mPath, theName, renameFile);
	theErr  = newPath.IsEmpty() ? kNErrPermission : kNoErr;
	
	if (theErr == kNoErr)
		mPath = newPath;
	
	return(theErr);
}





//============================================================================
//		NFile::GetExtension : Set the file extension.
//----------------------------------------------------------------------------
NString NFile::GetExtension(void) const
{	NString			theResult;
	NRange			theDot;



	// Get the state we need
	theResult = GetName();
	theDot    = theResult.Find(".", kNStringBackwards);



	// Extract the extension
	if (theDot.IsEmpty())
		theResult.Clear();
	else
		theResult = theResult.GetString(theDot.GetNext());

	return(theResult);
}





//============================================================================
//		NFile::SetExtension : Set the file extension.
//----------------------------------------------------------------------------
NStatus NFile::SetExtension(const NString &theExtension, bool renameFile)
{	NString		theName, oldExtension;



	// Construct the new name
	theName      = GetName();
	oldExtension = GetExtension();
	
	if (!oldExtension.IsEmpty())
		theName = theName.GetLeft(theName.GetSize() - (oldExtension.GetSize() + 1));
	
	theName += ".";
	theName += theExtension;



	// Set the file name
	return(SetName(theName, renameFile));
}





//============================================================================
//        NFile::GetSize : Get the file size.
//----------------------------------------------------------------------------
UInt64 NFile::GetSize(void) const
{


	// Check our state
	if (!Exists() || !IsFile())
		return(0);



	// Get the size
	return(NTargetFile::GetSize(mPath));
}





//============================================================================
//        NFile::SetSize : Set the file size.
//----------------------------------------------------------------------------
NStatus NFile::SetSize(UInt64 theSize)
{	NStatus		theErr;



	// Create the file if necessary
	if (!Exists())
		CreateFile();



	// Set the size
	theErr = NTargetFile::SetSize(mPath, theSize);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//        NFile::GetChild : Get the child of a directory.
//----------------------------------------------------------------------------
NFile NFile::GetChild(const NString &fileName) const
{


	// Get the child
	return(NTargetFile::GetChild(mPath, fileName));
}





//============================================================================
//        NFile::GetParent : Get the parent of a file/directory.
//----------------------------------------------------------------------------
NFile NFile::GetParent(void) const
{


	// Get the parent
	return(NTargetFile::GetParent(mPath));
}





//============================================================================
//        NFile::Delete : Delete the file.
//----------------------------------------------------------------------------
void NFile::Delete(void)
{


	// Validate our state
	NN_ASSERT(Exists());



	// Delete the file
	NTargetFile::Delete(mPath);
}





//============================================================================
//        NFile::CreateFile : Create a file.
//----------------------------------------------------------------------------
NStatus NFile::CreateFile(void)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(!Exists());



	// Create a file
	theErr = Open(kNPermissionWrite, true);
	NN_ASSERT_NOERR(theErr);

	if (theErr == kNoErr)
		Close();
	
	return(theErr);
}
		
		



//============================================================================
//        NFile::CreateDirectory : Create a directory.
//----------------------------------------------------------------------------
NStatus NFile::CreateDirectory(void)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(!Exists());



	// Create a directory
	theErr = NTargetFile::CreateDirectory(mPath);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//        NFile::ExchangeWith : Exchange two files.
//----------------------------------------------------------------------------
NStatus NFile::ExchangeWith(const NFile &theTarget)
{	NStatus		theErr;



	// Exchange two files
	theErr = NTargetFile::ExchangeWith(mPath, theTarget.mPath);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//        NFile::Open : Open the file.
//----------------------------------------------------------------------------
NStatus NFile::Open(NPermission thePermission, bool canCreate)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Check our state
	if (!canCreate && !IsFile())
		return(kNErrNotFound);



	// Open the file
	mFile  = NTargetFile::Open(mPath, thePermission);
	theErr = (mFile == kNFileRefNone) ? kNErrPermission : kNoErr;
	
	return(theErr);
}





//============================================================================
//        NFile::Close : Close the file.
//----------------------------------------------------------------------------
void NFile::Close(void)
{


	// Validate our state
	NN_ASSERT(Exists() ? IsFile() : true);
	NN_ASSERT(IsOpen());



	// Close the file
	NTargetFile::Close(mFile);
	mFile = kNFileRefNone;
}





//============================================================================
//        NFile::GetPosition : Get the read/write position.
//----------------------------------------------------------------------------
UInt64 NFile::GetPosition(void) const
{	UInt64		thePosition;



	// Validate our state
	NN_ASSERT(IsFile());
	NN_ASSERT(IsOpen());



	// Get the position
	thePosition = NTargetFile::GetPosition(mFile);
	
	return(thePosition);
}





//============================================================================
//        NFile::SetPosition : Set the read/write position.
//----------------------------------------------------------------------------
NStatus NFile::SetPosition(SInt64 theOffset, NPosition thePosition)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(IsFile());
	NN_ASSERT(IsOpen());



	// Set the position
	theErr = NTargetFile::SetPosition(mFile, theOffset, thePosition);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//        NFile::Read : Read data from the file.
//----------------------------------------------------------------------------
NStatus NFile::Read(UInt64 theSize, void *thePtr, UInt64 &numRead, SInt64 theOffset, NPosition thePosition)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(IsFile());
	NN_ASSERT(IsOpen());



	// Read the file
	theErr = NTargetFile::Read(mFile, theSize, thePtr, numRead, theOffset, thePosition);
	NN_ASSERT(theErr == kNoErr || theErr == kNErrExhaustedSrc);
	
	return(theErr);
}





//============================================================================
//        NFile::Write : Write data to the file.
//----------------------------------------------------------------------------
NStatus NFile::Write(UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NPosition thePosition)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(IsFile());
	NN_ASSERT(IsOpen());



	// Write the file
	theErr = NTargetFile::Write(mFile, theSize, thePtr, numWritten, theOffset, thePosition);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
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
//		NFile::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NFile::operator NFormatArgument(void) const
{


	// Get the value
	return(GetPath());
}





//============================================================================
//      NFile::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
#pragma mark -
void NFile::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeString(kNEncoderValueKey, GetPath());
}





//============================================================================
//      NFile::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NFile::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	SetPath(theEncoder.DecodeString(kNEncoderValueKey));
}





//============================================================================
//		NFile::InitializeSelf : Initialize the file.
//----------------------------------------------------------------------------
#pragma mark -
void NFile::InitializeSelf(const NString &thePath)
{


	// Initialize ourselves
	mPath = thePath;
	mFile = kNFileRefNone;
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




