/*	NAME:
		NFileMap.cpp

	DESCRIPTION:
		Memory-mapped file.

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
#include "NFileMap.h"





//============================================================================
//		NFileMap::NFileMap : Constructor.
//----------------------------------------------------------------------------
NFileMap::NFileMap(void)
{


	// Initialize ourselves
	mFileRef    = kNFileRefNone;
	mFileAccess = kNAccessRead;
}





//============================================================================
//		NFileMap::~NFileMap : Destructor.
//----------------------------------------------------------------------------
NFileMap::~NFileMap(void)
{


	// Clean up
	if (IsOpen())
		Close();



	// Validate our state
	NN_ASSERT(mFileRef == kNFileRefNone);
	NN_ASSERT(mPages.empty());
}





//============================================================================
//		NFileMap::IsOpen : Is the file open?
//----------------------------------------------------------------------------
bool NFileMap::IsOpen(void) const
{


	// Get our state
	return(mFileRef != kNFileRefNone);
}





//============================================================================
//		NFileMap::Open : Open the file.
//----------------------------------------------------------------------------
NStatus NFileMap::Open(const NFile &theFile, NMapAccess theAccess)
{	NStatus		theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Check our parameters
	if (!theFile.IsFile())
		return(kNErrNotFound);



	// Open the file
	mFile       = theFile;
	mFileAccess = theAccess;

	mFileRef = NTargetFile::MapOpen(theFile, theAccess);
	theErr   = IsOpen() ? kNoErr : kNErrPermission;
	
	return(theErr);
}





//============================================================================
//		NFileMap::Close : Close the file.
//----------------------------------------------------------------------------
void NFileMap::Close(void)
{	StLock		acquireLock(mLock);



	// Validate our state
	NN_ASSERT(IsOpen());



	// Unmap any pages
	while (!mPages.empty())
		Unmap(mPages.begin()->first);



	// Close the file
	NTargetFile::MapClose(mFileRef);
	
	mFileRef = kNFileRefNone;
}





//============================================================================
//		NFileMap::Map : Map a page.
//----------------------------------------------------------------------------
void *NFileMap::Map(UInt64 theOffset, UInt32 theSize, NMapAccess theAccess, bool noCache)
{	StLock			acquireLock(mLock);
	NMapInfo		theInfo;
	void			*thePtr;



	// Validate our state
	NN_ASSERT(IsOpen());



	// Get the state we need
	if (theSize == 0)
		{
		NN_ASSERT(mFile.GetSize() <= ULONG_MAX);
		theSize = (UInt32) mFile.GetSize();
		}

	theInfo.theSize   = theSize;
	theInfo.theAccess = theAccess;



	// Map the page
	thePtr = NTargetFile::MapFetch(mFileRef, theAccess, theOffset, theSize, noCache);
	if (thePtr != NULL)
		{
		NN_ASSERT(mPages.find(thePtr) == mPages.end());
		mPages[thePtr] = theInfo;
		}

	return(thePtr);
}





//============================================================================
//		NFileMap::Unmap : Unmap a page.
//----------------------------------------------------------------------------
void NFileMap::Unmap(const void *thePtr)
{	StLock					acquireLock(mLock);
	NMapInfoMapIterator		theIter;
	NMapInfo				theInfo;



	// Get the state we need
	theIter = mPages.find(thePtr);
	NN_ASSERT(theIter != mPages.end());
	
	if (theIter == mPages.end())
		return;

	theInfo = theIter->second;



	// Unmap the page
	NTargetFile::MapDiscard(mFileRef, theInfo.theAccess, thePtr, theInfo.theSize);
	mPages.erase(theIter);
}

