/*	NAME:
		NRange.h

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
#ifndef NRANGE_H
#define NRANGE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NMixinComparable.h"
#include "NMixinContainer.h"

// System
#include <vector>





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
class NRange;

using NVectorRange = std::vector<NRange>;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NRange final
	: public NMixinContainer<NRange>
	, public NMixinComparable<NRange>
{
public:
										NRange(size_t theLocation = 0, size_t theSize = 0);


	// Does the range overlap another?
	bool                                Overlaps(const NRange& theRange) const;


	// Does the range contain an offset?
	bool                                Contains(size_t theOffset) const;


	// Get/set the location
	size_t                              GetLocation() const;
	void                                SetLocation(size_t theLocation);


	// Get/set the size
	size_t                              GetSize() const;
	void                                SetSize(size_t theSize);


	// Set the range
	void                                SetRange(size_t theLocation, size_t theSize);


	// Get the limits
	//
	// The first and last element are contained within the range.
	//
	// The "next element" is the element immediately after the range.
	size_t                              GetFirst() const;
	size_t                              GetLast()  const;
	size_t                              GetNext()  const;


	// Get an offset
	//
	// Return an offset relative to our location.
	size_t                              GetOffset(size_t theOffset) const;


	// Get the union with a range
	NRange                              GetUnion(const NRange& theRange) const;


	// Get the intersection with a range
	NRange                              GetIntersection(const NRange& theRange) const;


	// Compare a range
	NComparison                         Compare(const NRange& theRange) const;


private:
	size_t                              mLocation;
	size_t                              mSize;
};



#endif // NRANGE_H
