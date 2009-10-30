/*	NAME:
		NMappedFile.cpp

	DESCRIPTION:
		Memory-mapped file.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/mman.h>

#include "NMappedFile.h"





//============================================================================
//		NMappedFile::NMappedFile : Constructor.
//----------------------------------------------------------------------------
NMappedFile::NMappedFile(void)
{


	// Initialize ourselves
	mFileRef = kInvalidForkRef;
}





//============================================================================
//		NMappedFile::~NMappedFile : Destructor.
//----------------------------------------------------------------------------
NMappedFile::~NMappedFile(void)
{


	// Clean up
	if (IsOpen())
		Close();



	// Validate our state
	NN_ASSERT(mFileRef == kInvalidForkRef);
	NN_ASSERT(mPages.empty());
}





//============================================================================
//		NMappedFile::IsOpen : Is the file open?
//----------------------------------------------------------------------------
bool NMappedFile::IsOpen(void) const
{


	// Get our state
	return(mFileRef != kInvalidForkRef);
}





//============================================================================
//		NMappedFile::Open : Open the file.
//----------------------------------------------------------------------------
OSStatus NMappedFile::Open(const NFile &theFile, MapAccess theAccess)
{	int			theFlags;
	NString		thePath;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Check our parameters
	if (!theFile.IsFile())
		return(fnfErr);



	// Update our state
	mFile       = theFile;
	mFileAccess = theAccess;
	
	mFile.ResolveAlias();



	// Open the file
	thePath  = mFile.GetPath();
	theFlags = (theAccess == kAccessReadWrite ? O_RDWR : O_RDONLY);
	mFileRef = open(thePath.GetUTF8(), theFlags, 0);

	if (mFileRef == kInvalidForkRef)
		return(permErr);

	return(noErr);
}





//============================================================================
//		NMappedFile::Close : Close the file.
//----------------------------------------------------------------------------
void NMappedFile::Close(void)
{	StLock		theLock(mLock);



	// Validate our state
	NN_ASSERT(IsOpen());



	// Clean up
	while (!mPages.empty())
		Unmap(mPages.begin()->first);

	close(mFileRef);
	mFileRef = kInvalidForkRef;
}





//============================================================================
//		NMappedFile::Map : Map a page.
//----------------------------------------------------------------------------
void *NMappedFile::Map(UInt64 theOffset, UInt32 theSize, MapAccess theAccess, bool noCache)
{	int				pagePerm, pageFlags;
	StLock			theLock(mLock);
	MapInfo			theInfo;
	void			*thePtr;



	// Validate our state
	NN_ASSERT(IsOpen());



	// Get the state we need
	if (theSize == 0)
		{
		NN_ASSERT(mFile.GetFileSize() <= ULONG_MAX);
		theSize = (UInt32) mFile.GetFileSize();
		}

	theInfo.theSize   = theSize;
	theInfo.theAccess = theAccess;



	// Map the page
	//
	// Pages can only be mapped with a compatible access mode to the underlying file;
	// a file opened as read-only can not be used to obtain read-write pages.
	pagePerm  = PROT_READ;
	pageFlags = MAP_FILE;

	if (theAccess != kAccessReadOnly)
		pagePerm |= PROT_WRITE;

	if (theAccess == kAccessReadWrite)
		pageFlags |= MAP_SHARED;

	if (theAccess == kAccessCopyOnWrite)
		pageFlags |= MAP_PRIVATE;

	if (noCache)
		pageFlags |= MAP_NOCACHE;
	
	thePtr = mmap(NULL, theSize, pagePerm, pageFlags, mFileRef, theOffset);
	if (thePtr == MAP_FAILED)
		thePtr = NULL;



	// Update our state
	if (thePtr != NULL)
		mPages[thePtr] = theInfo;
	
	return(thePtr);
}





//============================================================================
//		NMappedFile::Unamp : Unmap a page.
//----------------------------------------------------------------------------
void NMappedFile::Unmap(const void *thePtr)
{	StLock					theLock(mLock);
	MapInfoMapIterator		theIter;
	MapInfo					theInfo;
	OSStatus				sysErr;



	// Get the page info
	theIter = mPages.find(thePtr);
	NN_ASSERT(theIter != mPages.end());
	
	theInfo = theIter->second;



	// Unmap the page
	if (theInfo.theAccess == kAccessReadWrite)
		{
		sysErr = msync((void *) thePtr, theInfo.theSize, MS_SYNC);
		NN_ASSERT_NOERR(sysErr);
		}
	
	sysErr = munmap((void *) thePtr, theInfo.theSize);
	NN_ASSERT_NOERR(sysErr);



	// Update our state
	mPages.erase(theIter);
}

