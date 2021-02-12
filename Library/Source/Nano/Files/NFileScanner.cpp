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





#pragma mark private
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
