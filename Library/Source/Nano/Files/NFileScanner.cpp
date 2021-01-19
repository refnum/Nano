/*	NAME:
		NFileScanner.cpp

	DESCRIPTION:
		File scanner.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFileScanner.h"

// Nano
#include "NFile.h"
#include "NStdAlgorithm.h"





//=============================================================================
//		NFileScannerIterator::NFileScannerIterator : Constructor.
//-----------------------------------------------------------------------------
NFileScannerIterator::NFileScannerIterator(NFileScanner& theScanner, const NFile& theFile)
	: mScanner(theScanner)
	, mFile(theFile)
{
}





//=============================================================================
//		NFileScannerIterator::operator* : Dereference operator.
//-----------------------------------------------------------------------------
const NFile& NFileScannerIterator::operator*() const
{


	// Get the file
	return mFile;
}





//=============================================================================
//		NFileScannerIterator::operator++ : Prefix increment operator.
//-----------------------------------------------------------------------------
NFileScannerIterator& NFileScannerIterator::operator++()
{


	// Advance the iterator
	mFile = mScanner.GetNext();

	return *this;
}





//=============================================================================
//		NFileScannerIterator::operator++ : Postfix increment operator.
//-----------------------------------------------------------------------------
NFileScannerIterator NFileScannerIterator::operator++(int)
{


	// Advance the iterator
	NFileScannerIterator prevIter = *this;

	mFile = mScanner.GetNext();

	return prevIter;
}





//=============================================================================
//		NFileScannerIterator::operator+ : Addition operator.
//-----------------------------------------------------------------------------
NFileScannerIterator NFileScannerIterator::operator+(size_t n) const
{


	// Advance the iterator
	NFileScannerIterator theIter = *this;

	for (size_t x = 0; x < n; x++)
	{
		++theIter;
	}

	return theIter;
}





//=============================================================================
//		NFileScannerIterator::operator== : Equality operator.
//-----------------------------------------------------------------------------
bool NFileScannerIterator::operator==(const NFileScannerIterator& otherIter) const
{


	// Validate our parameters
	NN_REQUIRE(&mScanner == &otherIter.mScanner);


	// Compare the iterator
	if (mFile.IsValid() && otherIter.mFile.IsValid())
	{
		return mFile == otherIter.mFile;
	}

	else if (!mFile.IsValid() && !otherIter.mFile.IsValid())
	{
		return true;
	}
	else
	{
		return false;
	}
}





//=============================================================================
//		NFileScannerIterator::operator!= : Inequality operator.
//-----------------------------------------------------------------------------
bool NFileScannerIterator::operator!=(const NFileScannerIterator& otherIter) const
{


	// Validate our parameters
	NN_REQUIRE(&mScanner == &otherIter.mScanner);


	// Compare the iterator
	if (mFile.IsValid() && otherIter.mFile.IsValid())
	{
		return mFile != otherIter.mFile;
	}
	else if (!mFile.IsValid() && !otherIter.mFile.IsValid())
	{
		return false;
	}
	else
	{
		return true;
	}
}





#pragma mark NFileScanner
//=============================================================================
//		NFileScanner::NFileScanner : Constructor.
//-----------------------------------------------------------------------------
NFileScanner::NFileScanner(const NFile& theFile)
	: mRecurseDepth(kNSizeMax)
	, mFilterPath()
	, mFilterName()
	, mFilterItem(nullptr)
	, mScanRoot{}
	, mScanRoots{}
	, mScanStop(false)
	, mScanDepth(0)
	, mScanResults{}

{


	// Initialise ourselves
	Scan(theFile);
}





//=============================================================================
//		NFileScanner::NFileScanner : Constructor.
//-----------------------------------------------------------------------------
NFileScanner::NFileScanner()
	: mRecurseDepth(kNSizeMax)
	, mFilterPath()
	, mFilterName()
	, mFilterItem(nullptr)
	, mScanRoot{}
	, mScanRoots{}
	, mScanStop(false)
	, mScanDepth(0)
	, mScanResults{}
{
}





//=============================================================================
//		NFileScanner::GetRecurseDepth : Get the recursion depth.
//-----------------------------------------------------------------------------
size_t NFileScanner::GetRecurseDepth() const
{


	// Get the depth
	return mRecurseDepth;
}





//=============================================================================
//		NFileScanner::SetRecurseDepth : Set the recursion depth.
//-----------------------------------------------------------------------------
void NFileScanner::SetRecurseDepth(size_t theDepth)
{


	// Set the depth
	mRecurseDepth = theDepth;
}





//=============================================================================
//		NFileScanner::GetFilterPath : Get the path filter.
//-----------------------------------------------------------------------------
NString NFileScanner::GetFilterPath() const
{


	// Get the filter
	return mFilterPath;
}





//=============================================================================
//		NFileScanner::SetFilterPath : Set the path filter.
//-----------------------------------------------------------------------------
void NFileScanner::SetFilterPath(const NString& thePattern)
{


	// Set the filter
	mFilterPath = thePattern;
}





//=============================================================================
//		NFileScanner::GetFilterName : Get the name filter.
//-----------------------------------------------------------------------------
NString NFileScanner::GetFilterName() const
{


	// Get the filter
	return mFilterName;
}





//=============================================================================
//		NFileScanner::SetFilterName : Set the name filter.
//-----------------------------------------------------------------------------
void NFileScanner::SetFilterName(const NString& thePattern)
{


	// Set the filter
	mFilterName = thePattern;
}





//=============================================================================
//		NFileScanner::GetFilterItem : Get the item filter.
//-----------------------------------------------------------------------------
NFileScannerFilter NFileScanner::GetFilterItem() const
{


	// Get the filter
	return mFilterItem;
}





//=============================================================================
//		NFileScanner::SetFilterItem : Set the item filter.
//-----------------------------------------------------------------------------
void NFileScanner::SetFilterItem(const NFileScannerFilter& theFilter)
{


	// Set the filter
	mFilterItem = theFilter;
}





//=============================================================================
//		NFileScanner::Scan : Scan the files.
//-----------------------------------------------------------------------------
NStatus NFileScanner::Scan(const NFile& theRoot)
{


	// Validate our parameters
	NN_REQUIRE(theRoot.IsDirectory());



	// Start the scan
	mScanRoot    = theRoot;
	mScanRoots   = {{mScanRoot, 0}};
	mScanStop    = false;
	mScanDepth   = 0;
	mScanResults = {};

	return NStatus::OK;
}





//=============================================================================
//		NFileScanner::GetNext: Get the next file.
//-----------------------------------------------------------------------------
NFile NFileScanner::GetNext()
{


	// Validate our state
	NN_REQUIRE(IsValid());


	// Collect more results
	if (mScanResults.empty() && !mScanStop)
	{
		ContinueScan();
	}


	// Get the next file
	NFile theFile;

	if (!mScanResults.empty())
	{
		theFile = nstd::extract_front(mScanResults);
	}

	return theFile;
}





//=============================================================================
//		NFileScanner::GetFiles : Get the files.
//-----------------------------------------------------------------------------
NVectorFile NFileScanner::GetFiles()
{


	// Validate our state
	NN_REQUIRE(IsValid());

	// Get the files
	NVectorFile theFiles;

	for (const auto& theFile : *this)
	{
		theFiles.emplace_back(theFile);
	}

	return theFiles;
}





//=============================================================================
//		NFileScanner::begin : Return the initial iterator.
//-----------------------------------------------------------------------------
NFileScannerIterator NFileScanner::begin()
{


	// Validate our state
	NN_REQUIRE(IsValid());



	// Get the iterator
	//
	// Scanning from the start implicitly restarts the scan.
	Scan(mScanRoot);

	return {*this, GetNext()};
}





//=============================================================================
//		NFileScanner::end : Return the end iterator.
//-----------------------------------------------------------------------------
NFileScannerIterator NFileScanner::end()
{


	// Validate our state
	NN_REQUIRE(IsValid());

	// Get the iterator
	return {*this, NFile()};
}





#pragma mark private
//=============================================================================
//		NFileScanner::IsValid : Is the scanner in a valid state to scan?
//-----------------------------------------------------------------------------
bool NFileScanner::IsValid()
{


	// Check our state
	return mScanRoot.IsDirectory();
}





//=============================================================================
//		NFileScanner::ContinueScan : Continue the scan.
//-----------------------------------------------------------------------------
void NFileScanner::ContinueScan()
{


	// Validate our state
	NN_REQUIRE(mScanResults.empty());



	// Get the state we need
	bool matchPath = !mFilterPath.IsEmpty();
	bool matchName = !mFilterName.IsEmpty();


	// Continue the scan
	//
	// We can stop once we've collected some results, or exhausted the start points.
	while (mScanResults.empty() && !mScanRoots.empty() && !mScanStop)
	{
		// Process the files
		NFileScannerRoot theRoot = nstd::extract_front(mScanRoots);

		for (const auto& theFile : theRoot.theFile.GetChildren())
		{
			// Collect the matches
			bool isMatch = true;

			if (isMatch && matchPath)
			{
				isMatch = theFile.GetPath().GetPath().Contains(mFilterPath, kNStringPatternNoCase);
			}

			if (isMatch && matchName)
			{
				isMatch = theFile.GetName().Contains(mFilterName, kNStringPatternNoCase);
			}

			if (isMatch && mFilterItem != nullptr)
			{
				NN_REQUIRE(!mScanStop);
				isMatch = mFilterItem(theFile, mScanStop);
			}

			if (isMatch)
			{
				mScanResults.emplace_back(theFile);
			}



			// Collect directories as new roots
			if (mScanDepth < mRecurseDepth && theFile.IsDirectory())
			{
				mScanRoots.push_back({theFile, mScanDepth + 1});
			}
		}
	}
}
