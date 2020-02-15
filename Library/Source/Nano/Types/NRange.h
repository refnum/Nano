/*	NAME:
		NRange.h

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
#ifndef NRANGE_H
#define NRANGE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinComparable.h"
#include "NMixinContainer.h"
#include "NanoConstants.h"
#include "NanoMacros.h"

// System
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declarations
class NRange;


// Containers
using NVectorRange = std::vector<NRange>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NRange final
	: public NMixinContainer<NRange>
	, public NMixinComparable<NRange>
{
public:
	constexpr                           NRange(size_t theLocation, size_t theSize);
	constexpr                           NRange();


	// Does a range intersect another?
	bool                                Intersects(const NRange& theRange) const;


	// Does the range contain an offset?
	bool                                Contains(size_t theOffset) const;


	// Is this a meta-range?
	bool                                IsMeta() const;


	// Clear the range
	void                                Clear();


	// Get/set the location
	size_t                              GetLocation() const;
	void                                SetLocation(size_t theLocation);


	// Get/set the size
	size_t                              GetSize() const;
	void                                SetSize(size_t theSize);


	// Set the range
	void                                SetRange(size_t theLocation, size_t theSize);


	// Get the contents
	//
	// The position of the elements within the range are relative
	// to the range location/
	//
	// The first element in the range has an offset of 0.
	//
	// The first and last element are both contained within the range.
	//
	// The "next element" is the element immediately after the range.
	size_t                              GetPosition(size_t theOffset) const;

	size_t                              GetFirst() const;
	size_t                              GetLast()  const;
	size_t                              GetNext()  const;


	// Get the union with a range
	NRange                              GetUnion(const NRange& theRange) const;


	// Get the intersection with a range
	NRange                              GetIntersection(const NRange& theRange) const;


	// Get a normalized range
	//
	// Normalizing a meta-range evaluates it against an implied 0..theSize range:
	//
	//		kNRangeNone			0..0
	//		kNRangeAll			0..theSize
	//
	// A range whose size exceeds the implied 0..theSize range is clamped to
	// the maximum available size.
	//
	// A range whose location falls ouside the implied 0..theSize range keeps
	// its original location and receives a size of 0.
	NRange                              GetNormalized(size_t theSize) const;


public:
	// NMixinComparable
	bool                                CompareEqual(const NRange& theRange) const;
	NComparison                         CompareOrder(const NRange& theRange) const;


private:
	size_t                              mLocation;
	size_t                              mSize;
};





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static const NRange kNRangeNone(  kNNotFound, 0);
static const NRange kNRangeAll(0, kNNotFound);





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NRange.inl"



#endif // NRANGE_H
