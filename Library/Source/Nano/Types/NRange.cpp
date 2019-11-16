/*	NAME:
		NRange.cpp

	DESCRIPTION:
		Range object.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
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





//=============================================================================
//		NRange::NRange : Constructor.
//-----------------------------------------------------------------------------
NRange::NRange(size_t theLocation, size_t theSize)
	: mLocation(theLocation)
	, mSize(theSize)
{
}





//=============================================================================
//		NRange::IsEmpty : Is the range empty?
//-----------------------------------------------------------------------------
bool NRange::IsEmpty() const
{
	return mSize == 0;
}





//=============================================================================
//		NRange::Overlaps : Does the range overlap another?
//-----------------------------------------------------------------------------
bool NRange::Overlaps(const NRange& theRange) const
{


	// Check for overlap
	bool hasOverlap = (!IsEmpty() && !theRange.IsEmpty());

	if (hasOverlap)
	{
		hasOverlap = !GetIntersection(theRange).IsEmpty();
	}

	return hasOverlap;
}





//=============================================================================
//		NRange::Contains : Does the range contain an offset?
//-----------------------------------------------------------------------------
bool NRange::Contains(size_t theOffset) const
{


	// Check for containment
	bool hasOffset = !IsEmpty();

	if (hasOffset)
	{
		hasOffset = (theOffset >= GetFirst() && theOffset <= GetLast());
	}

	return hasOffset;
}





//=============================================================================
//		NRange::GetLocation : Get the location.
//-----------------------------------------------------------------------------
size_t NRange::GetLocation() const
{


	// Get the locatiom
	return mLocation;
}





//=============================================================================
//		NRange::SetLocation : Set the location.
//-----------------------------------------------------------------------------
void NRange::SetLocation(size_t theValue)
{


	// Set the location
	mLocation = theValue;
}





//=============================================================================
//		NRange::GetSize : Get the size.
//-----------------------------------------------------------------------------
size_t NRange::GetSize() const
{


	// Get the size
	return mSize;
}





//=============================================================================
//		NRange::SetSize : Set the size.
//-----------------------------------------------------------------------------
void NRange::SetSize(size_t theValue)
{


	// Set the size
	mSize = theValue;
}





//=============================================================================
//		NRange::GetFirst : Get the first element.
//-----------------------------------------------------------------------------
size_t NRange::GetFirst() const
{


	// Get the first element
	return mLocation;
}





//=============================================================================
//		NRange::GetLast : Get the last element.
//-----------------------------------------------------------------------------
size_t NRange::GetLast() const
{


	// Get the last element
	if (mSize == 0)
	{
		return mLocation;
	}
	else
	{
		return mLocation + mSize - 1;
	}
}





//=============================================================================
//		NRange::GetNext : Get the subsequent element.
//-----------------------------------------------------------------------------
size_t NRange::GetNext() const
{


	// Get the next element
	return mLocation + mSize;
}





//=============================================================================
//		NRange::GetOffset : Get an offset.
//-----------------------------------------------------------------------------
size_t NRange::GetOffset(size_t theOffset) const
{


	// Get the offset
	return mLocation + theOffset;
}





//=============================================================================
//		NRange::GetUnion : Get the union with a range.
//-----------------------------------------------------------------------------
NRange NRange::GetUnion(const NRange& theRange) const
{


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





#pragma mark protected
//=============================================================================
//		NRange::Compare : Compare a range.
//-----------------------------------------------------------------------------
NComparison NRange::Compare(const NRange& theRange) const
{


	// Compare the range
	//
	// Ranges can only be compared for equality / inequality.
	NComparison theResult = NCompare(mSize, theRange.mSize);

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mLocation, theRange.mLocation);
	}

	return theResult;
}
