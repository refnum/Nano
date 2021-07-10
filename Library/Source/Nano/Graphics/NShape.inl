/*	NAME:
		NShape.inl

	DESCRIPTION:
		Shape object.

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
#include "NGeomUtils.h"





//=============================================================================
//		NShape::NShape : Constructor.
//-----------------------------------------------------------------------------
inline NShape::NShape(const NVectorPoint& thePoints, const NVectorSize& theLoops)
	: points(thePoints)
	, loops(theLoops)
{
}





//=============================================================================
//		NShape::Clear : Clear the shape.
//-----------------------------------------------------------------------------
inline void NShape::Clear()
{


	// Clear the shape
	points.clear();
	loops.clear();
}





//=============================================================================
//		NShape::IsEmpty : Is the shape empty?
//-----------------------------------------------------------------------------
inline bool NShape::IsEmpty() const
{


	// Test the shape
	return points.empty();
}





//=============================================================================
//		NShape::GetBounds : Get the bounds.
//-----------------------------------------------------------------------------
inline NRectangle NShape::GetBounds() const
{


	// Get the bounds
	return NGeomUtils::GetBounds(points);
}





#pragma mark NMixinComparable
//=============================================================================
//		NShape::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
inline bool NShape::CompareEqual(const NShape& theShape) const
{


	// Compare the shape
	return points == theShape.points && loops == theShape.loops;
}





//=============================================================================
//		NShape::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
inline NComparison NShape::CompareOrder(const NShape& theShape) const
{


	// Order the shape
	//
	// Sort by size of content; sorting by area might be more appropriate.
	NComparison theResult = NCompare(points.size(), theShape.points.size());

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(loops.size(), theShape.loops.size());
	}

	return theResult;
}
