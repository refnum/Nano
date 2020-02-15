/*	NAME:
		NRange.cpp

	DESCRIPTION:
		Range object.

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
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NRange.h"

// Nano
#include "NDebug.h"

// System
#include <algorithm>





//=============================================================================
//		NRange::GetOffset : Get the range relative to an offset.
//-----------------------------------------------------------------------------
NRange NRange::GetOffset(size_t theOffset) const
{


	// Validate our parameters and state
	NN_REQUIRE(!IsMeta());



	// Get the offset
	return {mLocation + theOffset, mSize};
}





//=============================================================================
//		NRange::GetUnion : Get the union with a range.
//-----------------------------------------------------------------------------
NRange NRange::GetUnion(const NRange& theRange) const
{


	// Validate our state
	NN_REQUIRE(!IsMeta());


	// Check for empty
	if (IsEmpty())
	{
		return theRange;
	}

	if (theRange.IsEmpty())
	{
		return *this;
	}



	// Get the union
	size_t rangeFirst = std::min(GetFirst(), theRange.GetFirst());
	size_t rangeLast  = std::max(GetLast(), theRange.GetLast());

	return NRange(rangeFirst, rangeLast - rangeFirst + 1);
}





//=============================================================================
//		NRange::GetIntersection : Get the intersection of a range.
//-----------------------------------------------------------------------------
NRange NRange::GetIntersection(const NRange& theRange) const
{


	// Validate our state
	NN_REQUIRE(!IsMeta());


	// Check for empty ranges
	if (IsEmpty() || theRange.IsEmpty())
	{
		return NRange();
	}



	// Check for empty intersection
	if (theRange.GetFirst() > GetLast() || theRange.GetLast() < GetFirst())
	{
		return NRange();
	}



	// Get the intersection
	size_t rangeFirst = std::max(GetFirst(), theRange.GetFirst());
	size_t rangeLast  = std::min(GetLast(), theRange.GetLast());

	return NRange(rangeFirst, rangeLast - rangeFirst + 1);
}





//=============================================================================
//		NRange::GetNormalized : Get a normalized range.
//-----------------------------------------------------------------------------
NRange NRange::GetNormalized(size_t theSize) const
{


	// Normalize meta-ranges
	NRange theRange(*this);

	if (theRange == kNRangeNone)
	{
		theRange.Clear();
	}

	else if (theRange == kNRangeAll)
	{
		theRange.SetRange(0, theSize);
	}



	// Normalize normal ranges
	else if (theRange.GetLocation() >= theSize)
	{
		theRange.SetSize(0);
	}

	else if (theRange.GetNext() >= theSize)
	{
		theRange.SetSize(theSize - theRange.GetLocation());
	}

	return theRange;
}





#pragma mark NMixinComparable
//=============================================================================
//		NRange::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
bool NRange::CompareEqual(const NRange& theRange) const
{


	// Compare the range
	return (mLocation == theRange.mLocation) && (mSize == theRange.mSize);
}





//=============================================================================
//		NRange::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
NComparison NRange::CompareOrder(const NRange& theRange) const
{


	// Compare the range
	//
	// Ranges have no intrinsic ordering so we order by location first,
	// then order by size.
	NComparison theResult = NCompare(mLocation, theRange.mLocation);

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mSize, theRange.mSize);
	}

	return theResult;
}
