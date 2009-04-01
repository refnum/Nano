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
		: NComparable()
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
//		NFile::IsOpen : Is the file open?
//----------------------------------------------------------------------------
bool NFile::IsOpen(void) const
{


	// Check our state
	return(mFileRef != kNFileRefNone);
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
//		NFile::Compare : Compare two objects.
//----------------------------------------------------------------------------
#pragma mark -
NComparison NFile::Compare(const NComparable &theObject) const
{	const NFile		*theValue = dynamic_cast<const NFile*>(&theObject);



	// Validate our parameters
	NN_ASSERT(theValue != NULL);



	// Compare the values
	return(GET_COMPARISON(mPath, theValue->mPath));
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


