/*	NAME:
		NAlias.cpp

	DESCRIPTION:
		File alias.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAlias.h"





//============================================================================
//		NAlias::NAlias : Constructor.
//----------------------------------------------------------------------------
NAlias::NAlias(const NFile &theFile)
{


	// Initialize ourselves
	mAlias = NULL;
	
	SetFile(theFile);
}





//============================================================================
//		NAlias::NAlias : Constructor.
//----------------------------------------------------------------------------
NAlias::NAlias(const NAlias &theAlias)
{


	// Initialize ourselves
	mAlias = NULL;
	
	SetFile(theAlias.GetFile());
}





//============================================================================
//		NAlias::NAlias : Constructor.
//----------------------------------------------------------------------------
NAlias::NAlias(void)
{


	// Initialize ourselves
	mAlias = NULL;
}





//============================================================================
//		NAlias::~NAlias : Destructor.
//----------------------------------------------------------------------------
NAlias::~NAlias(void)
{


	// Clean up
	DisposeAlias();
}





//============================================================================
//		NAlias::IsValid : Is the alias valid?
//----------------------------------------------------------------------------
bool NAlias::IsValid(void) const
{


	// Get our state
	return(mAlias != NULL);
}





//============================================================================
//		NAlias::GetFile : Get the file.
//----------------------------------------------------------------------------
NFile NAlias::GetFile(void) const
{	NAlias		*thisPtr = (NAlias *) this;
	Boolean		wasChanged;
	FSRef		theFSRef;
	OSStatus	theErr;



	// Build the alias
	//
	// If we were assigned a file that does not exist, we want to create an
	// alias to track that file whenever the file is created. To do this, we
	// just create the alias on-demand whenever the file is retrieved.
	//
	// This does mean casting away our const, but since we also need mFile
	// to be mutable this is no great loss.
	//
	// Note that we assign the file's FSRef to ourselves, since SetFile will
	// reset mFile prior to assigning the new file.
	if (mAlias == NULL && mFile.Exists())
		thisPtr->SetFile(mFile.GetFSRef());



	// Resolve the alias
	if (mAlias != NULL)
		{
		theErr = FSResolveAlias(NULL, mAlias, &theFSRef, &wasChanged);
		NN_ASSERT_NOERR(theErr);
		
		if (theErr == noErr)
			mFile.SetFSRef(theFSRef);
		}



	// Get the file
	return(mFile);
}





//============================================================================
//		NAlias::SetFile : Set the file.
//----------------------------------------------------------------------------
void NAlias::SetFile(const NFile &theFile)
{	Boolean		wasChanged;
	FSRef		theFSRef;
	OSStatus	theErr;



	// Reset our state
	DisposeAlias();



	// Set the file
	mFile = theFile;



	// Create/update the alias
	//
	// If our file exists, we can also create an alias to track it.
	if (mFile.Exists())
		{
		theFSRef = theFile.GetFSRef();

		if (mAlias == NULL)
			theErr = FSNewAliasMinimal(&theFSRef, &mAlias);
		else
			theErr = FSUpdateAlias(NULL, &theFSRef, mAlias, &wasChanged);

		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NAlias::GetData : Get the alias data.
//----------------------------------------------------------------------------
NData NAlias::GetData(void) const
{	NData	theData;



	// Validate our state
	NN_ASSERT(IsValid());



	// Get the data
	theData.AppendData(GetHandleSize((Handle) mAlias), *mAlias);
	
	return(theData);
}





//============================================================================
//		NAlias::SetData : Set the alias data.
//----------------------------------------------------------------------------
void NAlias::SetData(const NData &theData)
{


	// Reset our state
	DisposeAlias();



	// Create the alias
	//
	// After unflattening the alias, we resolve it to bring it up to date.
	mAlias = (AliasHandle) NewHandle(theData.GetSize());
	if (mAlias != NULL)
		{
		memcpy(*mAlias, theData.GetData(), theData.GetSize());
		(void) GetFile();
		}
}





//============================================================================
//		NAlias::= : Assignment operator.
//----------------------------------------------------------------------------
const NAlias& NAlias::operator = (const NAlias &theAlias)
{


	// Assign the alias
	SetFile(theAlias.GetFile());
	
	return(*this);
}





//============================================================================
//		NAlias::operator AliasHandle : AliasHandle-cast operator.
//----------------------------------------------------------------------------
NAlias::operator AliasHandle(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());



	// Get the alias
	return(mAlias);
}





//============================================================================
//		NAlias::DisposeAlias : Dispose of the alias.
//----------------------------------------------------------------------------
#pragma mark -
void NAlias::DisposeAlias(void)
{


	// Dispose of the alias
	if (mAlias != NULL)
		DisposeHandle((Handle) mAlias);



	// Reset our state
	mFile  = kFSRefZero;
	mAlias = NULL;
}


