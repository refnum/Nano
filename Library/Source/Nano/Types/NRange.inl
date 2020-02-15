/*	NAME:
		NRange.inl

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
inline bool NRange::IsMeta() const
{


	// Check our state
	return mLocation == kNNotFound || mSize == kNNotFound;
}





//=============================================================================
//		NRange::Clear : Clear the range.
//-----------------------------------------------------------------------------
inline void NRange::Clear()
{


	// Clear the range
	mLocation = 0;
	mSize     = 0;
}





//=============================================================================
//		NRange::GetLocation : Get the location.
//-----------------------------------------------------------------------------
inline size_t NRange::GetLocation() const
{


	// Get the locatiom
	return mLocation;
}





//=============================================================================
//		NRange::SetLocation : Set the location.
//-----------------------------------------------------------------------------
inline void NRange::SetLocation(size_t theValue)
{


	// Set the location
	mLocation = theValue;
}





//=============================================================================
//		NRange::GetSize : Get the size.
//-----------------------------------------------------------------------------
inline size_t NRange::GetSize() const
{


	// Get the size
	return mSize;
}





//=============================================================================
//		NRange::SetSize : Set the size.
//-----------------------------------------------------------------------------
inline void NRange::SetSize(size_t theValue)
{


	// Set the size
	mSize = theValue;
}





//=============================================================================
//		NRange::SetRange : Set the range.
//-----------------------------------------------------------------------------
inline void NRange::SetRange(size_t theLocation, size_t theSize)
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
