/*	NAME:
		NFileIterator.cpp

	DESCRIPTION:
		File iterator.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NFileIterator.h"





//============================================================================
//		NFileIterator::NFileIterator : Constructor.
//----------------------------------------------------------------------------
NFileIterator::NFileIterator(void)
{


	// Initialize ourselves
	mRecurseLimit = 0;

	mIsOpen    = false;
	mStopScan  = false;
	mScanDepth = 0;
}





//============================================================================
//		NFileIterator::~NFileIterator : Destructor.
//----------------------------------------------------------------------------
NFileIterator::~NFileIterator(void)
{


	// Clean up
	if (IsOpen())
		Close();
}





//============================================================================
//		NFileIterator::GetRecurseLimit : Get the recurse limit.
//----------------------------------------------------------------------------
NIndex NFileIterator::GetRecurseLimit(void) const
{


	// Get the limit
	return(mRecurseLimit);
}





//============================================================================
//		NFileIterator::SetRecurseLimit : Set the recurse limit.
//----------------------------------------------------------------------------
void NFileIterator::SetRecurseLimit(NIndex theLimit)
{


	// Validate our parameters
	NN_ASSERT(theLimit >= 0);



	// Set the limit
	mRecurseLimit = theLimit;
}





//============================================================================
//		NFileIterator::GetRecurseFilter : Get the recurse filter.
//----------------------------------------------------------------------------
NFilterFunctor NFileIterator::GetRecurseFilter(void) const
{


	// Get the filter
	return(mRecurseFilter);
}





//============================================================================
//		NFileIterator::SetRecurseFilter : Set the recurse filter.
//----------------------------------------------------------------------------
void NFileIterator::SetRecurseFilter(const NFilterFunctor &theFilter)
{


	// Set the filter
	mRecurseFilter = theFilter;
}





//============================================================================
//		NFileIterator::GetItemFilter : Get the item filter.
//----------------------------------------------------------------------------
NFilterFunctor NFileIterator::GetItemFilter(void) const
{


	// Get the filter
	return(mItemFilter);
}





//============================================================================
//		NFileIterator::SetItemFilter : Set the item filter.
//----------------------------------------------------------------------------
void NFileIterator::SetItemFilter(const NFilterFunctor &theFilter)
{


	// Set the filter
	mItemFilter = theFilter;
}





//============================================================================
//		NFileIterator::GetPatternName : Get the file name pattern.
//----------------------------------------------------------------------------
NString NFileIterator::GetPatternName(void) const
{


	// Get the pattern
	return(mItemPatternName);
}





//============================================================================
//		NFileIterator::SetPatternName : Set the file name pattern.
//----------------------------------------------------------------------------
void NFileIterator::SetPatternName(const NString &thePattern)
{


	// Set the pattern
	mItemPatternName = thePattern;
}





//============================================================================
//		NFileIterator::GetPatternPath : Get the file path pattern.
//----------------------------------------------------------------------------
NString NFileIterator::GetPatternPath(void) const
{


	// Get the pattern
	return(mItemPatternPath);
}





//============================================================================
//		NFileIterator::SetPatternPath : Set the file path pattern.
//----------------------------------------------------------------------------
void NFileIterator::SetPatternPath(const NString &thePattern)
{


	// Set the pattern
	mItemPatternPath = thePattern;
}





//============================================================================
//		NFileIterator::IsOpen : Is the iterator open?
//----------------------------------------------------------------------------
bool NFileIterator::IsOpen(void) const
{


	// Get our state
	return(mIsOpen);
}





//============================================================================
//		NFileIterator::Open : Open the iterator.
//----------------------------------------------------------------------------
NStatus NFileIterator::Open(const NFile &theRoot)
{	IteratorStart		theStart;



	// Validate our state
	NN_ASSERT(!IsOpen());
	NN_ASSERT(mScanDepth == 0);
	NN_ASSERT(mScanResult.empty());
	NN_ASSERT(mScanRemaining.empty());



	// Check our parameters
	if (!theRoot.IsDirectory())
		return(kNErrParam);



	// Update our state
	theStart.theFile  = theRoot;
	theStart.theDepth = 0;

	mIsOpen = true;
	mScanRemaining.push_back(theStart);

	return(kNoErr);
}





//============================================================================
//		NFileIterator::Close : Close the iterator.
//----------------------------------------------------------------------------
void NFileIterator::Close(void)
{


	// Validate our state
	NN_ASSERT(IsOpen());



	// Reset our state
	mIsOpen   = false;
	mStopScan = false;
	
	mScanDepth = 0;
	mScanResult.clear();
	mScanRemaining.clear();
}





//============================================================================
//		NFileIterator::GetNext: Get the next file.
//----------------------------------------------------------------------------
bool NFileIterator::GetNext(NFile &theFile)
{	bool	gotFile;



	// Reset our state
	theFile.Clear();



	// Collect some more results
	if (mScanResult.empty())
		ContinueScan();

	if (mStopScan)
		return(false);



	// Get the next file
	gotFile = !mScanResult.empty();
	if (gotFile)
		theFile = extract_front(mScanResult);

	return(gotFile);
}





//============================================================================
//		NFileIterator::GetFiles : Get the files.
//----------------------------------------------------------------------------
NFileList NFileIterator::GetFiles(const NFile &startDir)
{	NFileList		theFiles;
	NFile			theFile;
	NStatus			theErr;



	// Validate our state
	NN_ASSERT(!IsOpen());



	// Get the files
	theErr = Open(startDir);
	if (theErr == kNoErr)
		{
		while (GetNext(theFile))
			theFiles.push_back(theFile);

		Close();
		}

	return(theFiles);
}





//============================================================================
//		NFileIterator::ContinueScan : Continue the scan.
//----------------------------------------------------------------------------
#pragma mark -
void NFileIterator::ContinueScan(void)
{	bool						isMatch, canRecurse, matchFile, matchPath;
	IteratorStart				theStart;
	NFileList					theFiles;
	NString						theName;
	NFileListConstIterator		theIter;
	NFile						theFile;



	// Validate our state
	NN_ASSERT(mScanResult.empty());



	// Get the state we need
	matchFile = !mItemPatternName.IsEmpty();
	matchPath = !mItemPatternPath.IsEmpty();



	// Continue the scan
	//
	// We can stop once we've collected some results, or exhausted the start points.
	while (mScanResult.empty() && !mScanRemaining.empty() && !mStopScan)
		{
		// Get the state we need
		theStart = extract_front(mScanRemaining);
		theFiles = theStart.theFile.GetChildren();
	
	
	
		// Process the files
		for (theIter = theFiles.begin(); theIter != theFiles.end(); theIter++)
			{
			// Get the state we need
			theFile    = *theIter;
			isMatch    = true;
			canRecurse = true;

			if (matchFile || matchPath)
				theName = theFile.GetName();



			// Collect the matches
			if (isMatch && matchFile)
				isMatch = theName.Contains(mItemPatternName, kNStringPatternNoCase);

			if (isMatch && matchPath)
				isMatch = theName.Contains(mItemPatternPath, kNStringPatternNoCase);
		
			if (isMatch && mItemFilter != NULL)
				isMatch = mItemFilter(theFile, &mStopScan);

			if (isMatch)
				mScanResult.push_back(theFile);



			// Collect directories for later
			if (theFile.IsDirectory())
				{
				theStart.theFile  = theFile;
				theStart.theDepth = mScanDepth + 1;
			
				if (canRecurse && mRecurseLimit != 0)
					canRecurse = (theStart.theDepth >= mRecurseLimit);
			
				if (canRecurse && mRecurseFilter != NULL)
					canRecurse = mRecurseFilter(theFile, &mStopScan);

				if (canRecurse)
					mScanRemaining.push_back(theStart);
				}
			}
		}
}






