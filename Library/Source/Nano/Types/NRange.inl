/*	NAME:
		NRange.inl

	DESCRIPTION:
		Range object.

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
// Nano
#include "NDebug.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NRangeIterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type        =           size_t;
	using difference_type   =           size_t;
	using pointer           = const size_t*;
	using reference         = size_t&;


public:
	constexpr                           NRangeIterator(size_t theIndex);


	// Operators
	constexpr size_t                    operator*() const;

	constexpr NRangeIterator&           operator++();
	constexpr NRangeIterator            operator++(int);

	inline NRangeIterator&              operator--();
	inline NRangeIterator               operator--(int);

	constexpr NRangeIterator            operator+(size_t n) const;
	inline NRangeIterator               operator-(size_t n) const;

	constexpr bool                      operator==(const NRangeIterator& otherIter) const;
	constexpr bool                      operator!=(const NRangeIterator& otherIter) const;


private:
	size_t                              mIndex;
};





#pragma mark NRangeIterator
//=============================================================================
//		NRangeIterator::NRangeIterator : Constructor.
//-----------------------------------------------------------------------------
constexpr NRangeIterator::NRangeIterator(size_t theIndex)
	: mIndex(theIndex)
{
}





//=============================================================================
//		NRangeIterator::operator* : Dereference operator.
//-----------------------------------------------------------------------------
constexpr size_t NRangeIterator::operator*() const
{


	// Get the index
	return mIndex;
}





//=============================================================================
//		NRangeIterator::operator++ : Prefix increment operator.
//-----------------------------------------------------------------------------
constexpr NRangeIterator& NRangeIterator::operator++()
{


	// Decrement the iterator
	mIndex++;

	return *this;
}





//=============================================================================
//		NRangeIterator::operator++ : Postfix increment operator.
//-----------------------------------------------------------------------------
constexpr NRangeIterator NRangeIterator::operator++(int)
{


	// Increment the iterator
	NRangeIterator prevIter = *this;

	mIndex++;

	return prevIter;
}





//=============================================================================
//		NRangeIterator::operator-- : Prefix decrement operator.
//-----------------------------------------------------------------------------
inline NRangeIterator& NRangeIterator::operator--()
{


	// Validate our state
	NN_REQUIRE(mIndex != 0);



	// Decrement the iterator
	mIndex--;

	return *this;
}





//=============================================================================
//		NRangeIterator::operator-- : Postfix decrement operator.
//-----------------------------------------------------------------------------
inline NRangeIterator NRangeIterator::operator--(int)
{


	// Validate our state
	NN_REQUIRE(mIndex != 0);



	// Decrement the iterator
	NRangeIterator prevIter = *this;

	mIndex--;

	return prevIter;
}





//=============================================================================
//		NRangeIterator::operator+ : Addition operator.
//-----------------------------------------------------------------------------
constexpr NRangeIterator NRangeIterator::operator+(size_t n) const
{


	// Increment the iterator
	NRangeIterator theIter = *this;

	theIter.mIndex += n;

	return theIter;
}





//=============================================================================
//		NRangeIterator::operator- : Subtraction operator.
//-----------------------------------------------------------------------------
inline NRangeIterator NRangeIterator::operator-(size_t n) const
{


	// Validate our state
	NN_REQUIRE(mIndex >= n);



	// Decrement the iterator
	NRangeIterator theIter = *this;

	theIter.mIndex -= n;

	return theIter;
}





//=============================================================================
//		NRangeIterator::operator== : Equality operator.
//-----------------------------------------------------------------------------
constexpr bool NRangeIterator::operator==(const NRangeIterator& otherIter) const
{


	// Compare the iterator
	return mIndex == otherIter.mIndex;
}





//=============================================================================
//		NRangeIterator::operator!= : Inequality operator.
//-----------------------------------------------------------------------------
constexpr bool NRangeIterator::operator!=(const NRangeIterator& otherIter) const
{


	// Compare the iterator
	return mIndex != otherIter.mIndex;
}





#pragma mark NRange
//=============================================================================
//		NRange::NRange : Constructor.
//-----------------------------------------------------------------------------
constexpr NRange::NRange(size_t theLocation, size_t theSize)
	: mLocation(theLocation)
	, mSize(theSize)
{
}





//=============================================================================
//		NRange::NRange : Constructor.
//-----------------------------------------------------------------------------
constexpr NRange::NRange()
	: mLocation(0)
	, mSize(0)
{


	// Validate our state
	static_assert(std::is_trivially_copyable<NRange>::value, "NRange is not trivially copyable!");
	static_assert(std::is_standard_layout<NRange>::value, "NRange is not standard layout!");

	static_assert(sizeof(NRange) == 16);
	static_assert(offsetof(NRange, mLocation) == 0);
	static_assert(offsetof(NRange, mSize) == 8);
}





//=============================================================================
//		NRange::Intersects : Does the range intersect another?
//-----------------------------------------------------------------------------
inline bool NRange::Intersects(const NRange& theRange) const
{


	// Validate our state
	NN_REQUIRE(!IsMeta());



	// Check the intersection
	return !GetIntersection(theRange).IsEmpty();
}





//=============================================================================
//		NRange::Contains : Does the range contain an offset?
//-----------------------------------------------------------------------------
inline bool NRange::Contains(size_t theOffset) const
{


	// Validate our state
	NN_REQUIRE(!IsMeta());



	// Check for containment
	bool hasOffset = !IsEmpty();

	if (hasOffset)
	{
		hasOffset = (theOffset >= GetFirst() && theOffset <= GetLast());
	}

	return hasOffset;
}





//=============================================================================
//		NRange::IsMeta : Is this a meta range?
//-----------------------------------------------------------------------------
constexpr bool NRange::IsMeta() const
{


	// Check our state
	return mLocation == kNNotFound || mSize == kNNotFound;
}





//=============================================================================
//		NRange::Clear : Clear the range.
//-----------------------------------------------------------------------------
constexpr void NRange::Clear()
{


	// Clear the range
	mLocation = 0;
	mSize     = 0;
}





//=============================================================================
//		NRange::GetLocation : Get the location.
//-----------------------------------------------------------------------------
constexpr size_t NRange::GetLocation() const
{


	// Get the locatiom
	return mLocation;
}





//=============================================================================
//		NRange::SetLocation : Set the location.
//-----------------------------------------------------------------------------
constexpr void NRange::SetLocation(size_t theLocation)
{


	// Set the location
	mLocation = theLocation;
}





//=============================================================================
//		NRange::AddOffset : Add an offset to the location.
//-----------------------------------------------------------------------------
constexpr void NRange::AddOffset(size_t theOffset)
{


	// Update the location
	mLocation += theOffset;
}





//=============================================================================
//		NRange::GetSize : Get the size.
//-----------------------------------------------------------------------------
constexpr size_t NRange::GetSize() const
{


	// Get the size
	return mSize;
}





//=============================================================================
//		NRange::SetSize : Set the size.
//-----------------------------------------------------------------------------
constexpr void NRange::SetSize(size_t theSize)
{


	// Set the size
	mSize = theSize;
}





//=============================================================================
//		NRange::SetRange : Set the range.
//-----------------------------------------------------------------------------
constexpr void NRange::SetRange(size_t theLocation, size_t theSize)
{


	// Set the range
	mLocation = theLocation;
	mSize     = theSize;
}





//=============================================================================
//		NRange::GetPosition : Get the position of an element.
//-----------------------------------------------------------------------------
inline size_t NRange::GetPosition(size_t theOffset) const
{


	// Validate our parameters and state
	NN_REQUIRE(theOffset < GetSize());
	NN_REQUIRE(!IsMeta());



	// Get the element
	return mLocation + theOffset;
}





//=============================================================================
//		NRange::GetFirst : Get the first element.
//-----------------------------------------------------------------------------
inline size_t NRange::GetFirst() const
{


	// Validate our state
	NN_REQUIRE(!IsMeta());



	// Get the first element
	return mLocation;
}





//=============================================================================
//		NRange::GetLast : Get the last element.
//-----------------------------------------------------------------------------
inline size_t NRange::GetLast() const
{


	// Validate our state
	NN_REQUIRE(!IsMeta());



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
inline size_t NRange::GetNext() const
{


	// Validate our state
	NN_REQUIRE(!IsMeta());



	// Get the next element
	return mLocation + mSize;
}





//=============================================================================
//		NRange::GetOffset : Get the range relative to an offset.
//-----------------------------------------------------------------------------
inline NRange NRange::GetOffset(size_t theOffset) const
{


	// Validate our parameters and state
	NN_REQUIRE(!IsMeta());



	// Get the offset
	return {mLocation + theOffset, mSize};
}





//=============================================================================
//		NRange::GetUnion : Get the union with a range.
//-----------------------------------------------------------------------------
inline NRange NRange::GetUnion(const NRange& theRange) const
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
inline NRange NRange::GetIntersection(const NRange& theRange) const
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
constexpr NRange NRange::GetNormalized(size_t theSize) const
{


	// Normalize meta-ranges
	NRange theRange(*this);

	if (theRange.GetLocation() == kNNotFound && theRange.GetSize() == 0)
	{
		theRange.Clear();
	}

	else if (theRange.GetLocation() == 0 && theRange.GetSize() == kNNotFound)
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





//=============================================================================
//		NRange::begin : Get an iterator to the start of the range.
//-----------------------------------------------------------------------------
inline NRangeIterator NRange::begin() const
{


	// Get the iterator
	return NRangeIterator(GetFirst());
}





//=============================================================================
//		NRange::end : Get an iterator to the end of the range.
//-----------------------------------------------------------------------------
inline NRangeIterator NRange::end() const
{


	// Get the iterator
	return NRangeIterator(GetNext());
}





#pragma mark NMixinComparable
//=============================================================================
//		NRange::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
constexpr bool NRange::CompareEqual(const NRange& theRange) const
{


	// Compare the range
	return (mLocation == theRange.mLocation) && (mSize == theRange.mSize);
}





//=============================================================================
//		NRange::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
constexpr NComparison NRange::CompareOrder(const NRange& theRange) const
{


	// Compare the range
	//
	// Ranges are ordered by location first, then by size.
	NComparison theResult = NCompare(mLocation, theRange.mLocation);

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mSize, theRange.mSize);
	}

	return theResult;
}
