/*	NAME:
		NFileScanner.h

	DESCRIPTION:
		File scanner.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#ifndef NFILE_SCANNER_H
#define NFILE_SCANNER_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NFile.h"
#include "NanoConstants.h"


// System
#include <functional>
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NFileScanner;

// Roots
struct NFileScannerRoot
{
	NFile  theFile;
	size_t theDepth;
};


// Filters
//
// The item filter should return true/false as the current item
// should be included in the results.
//
// The stopScan flag should be set to true if the scan should
// stop after the current item.
typedef std::function<bool(const NFile& theFile, bool& stopScan)> NFileScannerFilter;


// Containers
using NVectorFileScannerRoot = std::vector<NFileScannerRoot>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileScannerIterator
{
public:
										NFileScannerIterator(NFileScanner& theScanner, const NFile& theFile);


	// Operators
	const NFile&                        operator*() const;

	NFileScannerIterator&               operator++();
	NFileScannerIterator                operator++(int);

	NFileScannerIterator                operator+(size_t n) const;

	bool                                operator==(const NFileScannerIterator& otherIter) const;
	bool                                operator!=(const NFileScannerIterator& otherIter) const;


private:
	NFileScanner&                       mScanner;
	NFile                               mFile;
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NFileScanner final
{
public:
										NFileScanner();
										NFileScanner(const NFile& theFile);


	// Get/set the recursion depth
	//
	// The default recursion depth is kNSizeMax.
	size_t                              GetRecurseDepth() const;
	void                                SetRecurseDepth(size_t theDepth);


	// Get/set the pattern filters
	//
	// The pattern filters are applied to the name, then path, of each item
	// within the recursion depth.
	//
	// If a pattern is set then only matching items are passed to the item
	// filter.
	//
	// Patterns are case-insensitive.
	NString                             GetFilterPath() const;
	void                                SetFilterPath(  const NString& thePattern);

	NString                             GetFilterName() const;
	void                                SetFilterName(  const NString& thePattern);


	// Get/set the item filter
	//
	// The item filter is invoked on each item that passes the pattern filters
	// and which exists within the recursion depth.
	//
	// If a filter is set then only items that return true will be passed to
	// the output.
	NFileScannerFilter                  GetFilterItem() const;
	void                                SetFilterItem(  const NFileScannerFilter& theFilter);


	// Scan the files
	//
	// Start scanning a directory.
	NStatus                             Scan(const NFile& theRoot);


	// Get the next file
	//
	// May only be called when a scan is underway.
	//
	// Returns an invalid file when the scan is complete.
	NFile                               GetNext();


	// Get the files
	//
	// May only be called when a scan is underway.
	//
	// Returns the files found by the scan.
	NVectorFile                         GetFiles();


	// Get NFileScanner iterators
	//
	// May only be called when a scan is underway.
	//
	// A call to begin() implicitly restarts the scan.
	//
	// Returns iterators suitable for a range-based for.
	NFileScannerIterator                begin();
	NFileScannerIterator                end();


private:
	bool                                IsValid();
	void                                ContinueScan();


private:
	size_t                              mRecurseDepth;
	NString                             mFilterPath;
	NString                             mFilterName;
	NFileScannerFilter                  mFilterItem;

	NFile                               mScanRoot;
	NVectorFileScannerRoot              mScanRoots;
	bool                                mScanStop;
	size_t                              mScanDepth;
	NVectorFile                         mScanResults;
};



#endif // NFILE_SCANNER_H
