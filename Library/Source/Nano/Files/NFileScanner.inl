/*	NAME:
		NFileScanner.inl

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





#pragma mark NFileScannerIterator
//=============================================================================
//		NFileScannerIterator::NFileScannerIterator : Constructor.
//-----------------------------------------------------------------------------
inline NFileScannerIterator::NFileScannerIterator(NFileScanner& theScanner, const NFile& theFile)
	: mScanner(theScanner)
	, mFile(theFile)
{
}





//=============================================================================
//		NFileScannerIterator::operator* : Dereference operator.
//-----------------------------------------------------------------------------
inline const NFile& NFileScannerIterator::operator*() const
{


	// Get the file
	return mFile;
}





//=============================================================================
//		NFileScannerIterator::operator++ : Prefix increment operator.
//-----------------------------------------------------------------------------
inline NFileScannerIterator& NFileScannerIterator::operator++()
{


	// Increment the iterator
	mFile = mScanner.GetNext();

	return *this;
}





//=============================================================================
//		NFileScannerIterator::operator++ : Postfix increment operator.
//-----------------------------------------------------------------------------
inline NFileScannerIterator NFileScannerIterator::operator++(int)
{


	// Increment the iterator
	NFileScannerIterator prevIter = *this;

	mFile = mScanner.GetNext();

	return prevIter;
}





//=============================================================================
//		NFileScannerIterator::operator+ : Addition operator.
//-----------------------------------------------------------------------------
inline NFileScannerIterator NFileScannerIterator::operator+(size_t n) const
{


	// Increment the iterator
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
inline bool NFileScannerIterator::operator==(const NFileScannerIterator& otherIter) const
{


	// Validate our parameters
	NN_REQUIRE(&mScanner == &otherIter.mScanner);



	// Compare the iterator
	return mFile == otherIter.mFile;
}





//=============================================================================
//		NFileScannerIterator::operator!= : Inequality operator.
//-----------------------------------------------------------------------------
inline bool NFileScannerIterator::operator!=(const NFileScannerIterator& otherIter) const
{


	// Validate our parameters
	NN_REQUIRE(&mScanner == &otherIter.mScanner);



	// Compare the iterator
	return mFile != otherIter.mFile;
}





#pragma mark NFileScanner
//=============================================================================
//		NFileScanner::GetRecurseDepth : Get the recursion depth.
//-----------------------------------------------------------------------------
inline size_t NFileScanner::GetRecurseDepth() const
{


	// Get the depth
	return mRecurseDepth;
}





//=============================================================================
//		NFileScanner::SetRecurseDepth : Set the recursion depth.
//-----------------------------------------------------------------------------
inline void NFileScanner::SetRecurseDepth(size_t theDepth)
{


	// Set the depth
	mRecurseDepth = theDepth;
}





//=============================================================================
//		NFileScanner::GetFilterPath : Get the path filter.
//-----------------------------------------------------------------------------
inline NString NFileScanner::GetFilterPath() const
{


	// Get the filter
	return mFilterPath;
}





//=============================================================================
//		NFileScanner::SetFilterPath : Set the path filter.
//-----------------------------------------------------------------------------
inline void NFileScanner::SetFilterPath(const NString& thePattern)
{


	// Set the filter
	mFilterPath = thePattern;
}





//=============================================================================
//		NFileScanner::GetFilterName : Get the name filter.
//-----------------------------------------------------------------------------
inline NString NFileScanner::GetFilterName() const
{


	// Get the filter
	return mFilterName;
}





//=============================================================================
//		NFileScanner::SetFilterName : Set the name filter.
//-----------------------------------------------------------------------------
inline void NFileScanner::SetFilterName(const NString& thePattern)
{


	// Set the filter
	mFilterName = thePattern;
}





//=============================================================================
//		NFileScanner::GetFilterItem : Get the item filter.
//-----------------------------------------------------------------------------
inline NFileScannerFilter NFileScanner::GetFilterItem() const
{


	// Get the filter
	return mFilterItem;
}





//=============================================================================
//		NFileScanner::SetFilterItem : Set the item filter.
//-----------------------------------------------------------------------------
inline void NFileScanner::SetFilterItem(const NFileScannerFilter& theFilter)
{


	// Set the filter
	mFilterItem = theFilter;
}





#pragma mark private
//=============================================================================
//		NFileScanner::IsValid : Is the scanner in a valid state to scan?
//-----------------------------------------------------------------------------
inline bool NFileScanner::IsValid()
{


	// Check our state
	return mScanRoot.IsDirectory();
}
