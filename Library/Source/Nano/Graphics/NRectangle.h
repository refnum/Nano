/*	NAME:
		NRectangle.h

	DESCRIPTION:
		Rectangle object.

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
#ifndef NRECTANGLE_H
#define NRECTANGLE_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.h"
#include "NPoint.h"
#include "NSize.h"
#include "NanoMacros.h"
#include "NanoTypes.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Positions
enum class NPosition
{
	Alert,
	Center,
	Left,
	Right,
	Top,
	TopLeft,
	TopRight,
	Bottom,
	BottomLeft,
	BottomRight
};





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NRectangle final : public NMixinComparable<NRectangle>
{
public:
	constexpr                           NRectangle(float64_t x, float64_t y, float64_t width, float64_t height);
	constexpr                           NRectangle(float64_t width, float64_t height);

	constexpr                           NRectangle(const NPoint& theOrigin, const NSize& theSize);
	constexpr                           NRectangle(const NSize& theSize);

	constexpr                           NRectangle();


	// Clear the rectangle
	constexpr void                      Clear();


	// Is the rectangle empty?
	constexpr bool                      IsEmpty() const;


	// Get a point within the rectangle
	constexpr NPoint                    GetPoint(NPosition thePosition) const;


	// Get the min / mix / max coordinate
	constexpr float64_t                 GetMinX() const;
	constexpr float64_t                 GetMidX() const;
	constexpr float64_t                 GetMaxX() const;

	constexpr float64_t                 GetMinY() const;
	constexpr float64_t                 GetMidY() const;
	constexpr float64_t                 GetMaxY() const;


	// Does the rectangle contain an object?
	constexpr bool                      Contains(const NRectangle& theRect) const;
	constexpr bool                      Contains(const NPoint& thePoint)    const;


	// Does the rectangle intersect another?
	constexpr bool                      Intersects(const NRectangle& theRect) const;


	// Get an integral rectangle
	//
	// An integral rectangle has its origin rounded down and its size rounded
	// up, creating the smallest integral rectangle that is equal to or larger
	// than the current rectangle.
	NRectangle                          GetIntegral() const;
	void                                MakeIntegral();


	// Get a normalized rectangle
	//
	// A normalised rectangle has positive dimensions.
	constexpr NRectangle                GetNormalized() const;
	constexpr void                      Normalize();


	// Get an offset rectangle
	constexpr NRectangle                GetOffset(float64_t deltaX, float64_t deltaY) const;
	constexpr void                      Offset(   float64_t deltaX, float64_t deltaY);


	// Get a scaled rectangle
	constexpr NRectangle                GetScaled(float64_t scaleBy) const;
	constexpr void                      Scale(    float64_t scaleBy);


	// Get an inset rectangle
	constexpr NRectangle                GetInset(float64_t deltaX, float64_t deltaY) const;
	constexpr void                      Inset(   float64_t deltaX, float64_t deltaY);


	// Get a fitted rectangle
	//
	// The rectangle is scaled to fit the target (whilst
	// maintaining its current aspect ratio), then centered.
	constexpr NRectangle                GetScaledToFit(const NRectangle& theRect) const;
	constexpr void                      ScaleToFit(    const NRectangle& theRect);


	// Get a positioned rectangle
	//
	// A positioned rectangle is placed within the target.
	constexpr NRectangle                GetPositioned(NPosition thePosition, const NRectangle& theRect) const;
	constexpr void                      SetPosition(  NPosition thePosition, const NRectangle& theRect);


	// Get a union with a rectangle
	constexpr NRectangle                GetUnion( const NRectangle& theRect) const;
	constexpr void                      UnionWith(const NRectangle& theRect);


	// Get an intersection with a rectangle
	constexpr NRectangle                GetIntersection(const NRectangle& theRect) const;
	constexpr void                      IntersectWith(  const NRectangle& theRect);



public:
	// NMixinComparable
	constexpr bool                      CompareEqual(const NRectangle& theRectangle) const;
	constexpr NComparison               CompareOrder(const NRectangle& theRectangle) const;


public:
	NPoint                              origin;
	NSize                               size;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NRectangle.inl"



#endif // NRECTANGLE_H
