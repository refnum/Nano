/*	NAME:
		NRectangle.inl

	DESCRIPTION:
		Rectangle object.

		NEncodable support uses a helper class to avoid a v-table.

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
//		NRectangle::NRectangle : Constructor.
//-----------------------------------------------------------------------------
constexpr NRectangle::NRectangle(float64_t x, float64_t y, float64_t width, float64_t height)
	: origin(x, y)
	, size(width, height)
{
}





//=============================================================================
//		NRectangle::NRectangle : Constructor.
//-----------------------------------------------------------------------------
constexpr NRectangle::NRectangle(float64_t width, float64_t height)
	: origin()
	, size(width, height)
{
}





//=============================================================================
//		NRectangle::NRectangle : Constructor.
//-----------------------------------------------------------------------------
constexpr NRectangle::NRectangle(const NPoint& theOrigin, const NSize& theSize)
	: origin(theOrigin)
	, size(theSize)
{
}





//=============================================================================
//		NRectangle::NRectangle : Constructor.
//-----------------------------------------------------------------------------
constexpr NRectangle::NRectangle(const NSize& theSize)
	: origin()
	, size(theSize)
{
}





//=============================================================================
//		NRectangle::NRectangle : Constructor.
//-----------------------------------------------------------------------------
constexpr NRectangle::NRectangle()
	: origin()
	, size()
{
}





//=============================================================================
//		NRectangle::Clear : Clear the rectangle.
//-----------------------------------------------------------------------------
constexpr void NRectangle::Clear()
{


	// Clear the rectangle
	origin.Clear();
	size.Clear();
}





//=============================================================================
//		NRectangle::IsEmpty : Is the rectangle empty?
//-----------------------------------------------------------------------------
constexpr bool NRectangle::IsEmpty() const
{
	return size.IsEmpty();
}





//=============================================================================
//		NRectangle::GetPoint : Get a point.
//-----------------------------------------------------------------------------
constexpr NPoint NRectangle::GetPoint(NPosition thePosition) const
{


	// Get the point
	NPoint theResult;

	switch (thePosition)
	{
		case NPosition::Alert:
			// The alert position is used to place rectangles, so we return
			// a point consistent with the top edge of an empty rectangle.
			theResult.x = GetMidX();
			theResult.y = GetMinY() + (size.height / 3.0);
			break;

		case NPosition::Center:
			theResult.x = GetMidX();
			theResult.y = GetMidY();
			break;

		case NPosition::Left:
			theResult.x = GetMinX();
			theResult.y = GetMidY();
			break;

		case NPosition::Right:
			theResult.x = GetMaxX();
			theResult.y = GetMidY();
			break;

		case NPosition::Top:
			theResult.x = GetMidX();
			theResult.y = GetMinY();
			break;

		case NPosition::TopLeft:
			theResult.x = GetMinX();
			theResult.y = GetMinY();
			break;

		case NPosition::TopRight:
			theResult.x = GetMaxX();
			theResult.y = GetMinY();
			break;

		case NPosition::Bottom:
			theResult.x = GetMidX();
			theResult.y = GetMaxY();
			break;

		case NPosition::BottomLeft:
			theResult.x = GetMinX();
			theResult.y = GetMaxY();
			break;

		case NPosition::BottomRight:
			theResult.x = GetMaxX();
			theResult.y = GetMaxY();
			break;
	}

	return theResult;
}





//=============================================================================
//		NRectangle::GetMinX : Get the minimum X value.
//-----------------------------------------------------------------------------
constexpr float64_t NRectangle::GetMinX() const
{
	return std::min(origin.x, origin.x + size.width);
}





//=============================================================================
//		NRectangle::GetMinY : Get the minimum Y value.
//-----------------------------------------------------------------------------
constexpr float64_t NRectangle::GetMinY() const
{
	return std::min(origin.y, origin.y + size.height);
}





//=============================================================================
//		NRectangle::GetMidX : Get the middle X value.
//-----------------------------------------------------------------------------
constexpr float64_t NRectangle::GetMidX() const
{
	return GetMinX() + ((GetMaxX() - GetMinX()) / 2.0);
}





//=============================================================================
//		NRectangle::GetMidY : Get the middle Y value.
//-----------------------------------------------------------------------------
constexpr float64_t NRectangle::GetMidY() const
{
	return GetMinY() + ((GetMaxY() - GetMinY()) / 2.0);
}





//=============================================================================
//		NRectangle::GetMaxX : Get the maximum X value.
//-----------------------------------------------------------------------------
constexpr float64_t NRectangle::GetMaxX() const
{
	return std::max(origin.x, origin.x + size.width);
}





//=============================================================================
//		NRectangle::GetMaxY : Get the maximum Y value.
//-----------------------------------------------------------------------------
constexpr float64_t NRectangle::GetMaxY() const
{
	return std::max(origin.y, origin.y + size.height);
}





//=============================================================================
//		NRectangle::Contains : Does a rectangle contain a rectangle?
//-----------------------------------------------------------------------------
constexpr bool NRectangle::Contains(const NRectangle& theRect) const
{


	// Check our state
	return theRect.GetMinX() >= GetMinX() && theRect.GetMaxX() <= GetMaxX() &&
		   theRect.GetMinY() >= GetMinY() && theRect.GetMaxY() <= GetMaxY();
}





//=============================================================================
//		NRectangle::Contains : Does a rectangle contain a point?
//-----------------------------------------------------------------------------
constexpr bool NRectangle::Contains(const NPoint& thePoint) const
{


	// Check our state
	return thePoint.x >= GetMinX() && thePoint.x <= GetMaxX() && thePoint.y >= GetMinY() &&
		   thePoint.y <= GetMaxY();
}





//=============================================================================
//		NRectangle::Intersects : Does a rectangle intersect another?
//-----------------------------------------------------------------------------
constexpr bool NRectangle::Intersects(const NRectangle& theRect) const
{


	// Check our state
	return !GetIntersection(theRect).IsEmpty();
}





//=============================================================================
//		NRectangle::GetIntegral : Get an integral rectangle.
//-----------------------------------------------------------------------------
inline NRectangle NRectangle::GetIntegral() const
{


	// Get an integral rectangle
	//
	// We round the origin down and the size up, to create the smallest
	// integral rectangle that is equal to or larger than the current.
	NRectangle theResult;

	theResult.origin.x    = floor(origin.x);
	theResult.origin.y    = floor(origin.y);
	theResult.size.width  = ceil(origin.x + size.width) - theResult.origin.x;
	theResult.size.height = ceil(origin.y + size.height) - theResult.origin.y;

	return theResult;
}





//=============================================================================
//		NRectangle::MakeIntegral : Make the rectangle integral.
//-----------------------------------------------------------------------------
inline void NRectangle::MakeIntegral()
{
	*this = GetIntegral();
}





//=============================================================================
//		NRectangle::GetNormalized : Get a normalized rectangle.
//-----------------------------------------------------------------------------
constexpr NRectangle NRectangle::GetNormalized() const
{


	// Get a normalized rectangle
	//
	// A normalized rectangle has positive width and height.
	return NRectangle(origin, size.GetNormalized());
}





//=============================================================================
//		NRectangle::Normalize : Normalize the rectangle.
//-----------------------------------------------------------------------------
constexpr void NRectangle::Normalize()
{
	*this = GetNormalized();
}





//=============================================================================
//		NRectangle::GetOffset : Get an offset rectangle.
//-----------------------------------------------------------------------------
constexpr NRectangle NRectangle::GetOffset(float64_t deltaX, float64_t deltaY) const
{


	// Get an offset rectangle
	NRectangle theResult = GetNormalized();

	theResult.origin.x += deltaX;
	theResult.origin.y += deltaY;

	return theResult;
}





//=============================================================================
//		NRectangle::Offset : Offset the rectangle.
//-----------------------------------------------------------------------------
constexpr void NRectangle::Offset(float64_t deltaX, float64_t deltaY)
{
	*this = GetOffset(deltaX, deltaY);
}





//=============================================================================
//		NRectangle::GetScaled : Get a scaled rectangle.
//-----------------------------------------------------------------------------
constexpr NRectangle NRectangle::GetScaled(float64_t scaleBy) const
{


	// Get a scaled rectangle
	NRectangle theResult = GetNormalized();
	NPoint     theCenter = theResult.GetPoint(NPosition::Center);

	theResult.size.width *= scaleBy;
	theResult.size.height *= scaleBy;

	theResult.origin.x = theCenter.x - (theResult.size.width / 2.0);
	theResult.origin.y = theCenter.y - (theResult.size.height / 2.0);

	return theResult;
}





//=============================================================================
//		NRectangle::Scale : Scale the rectangle.
//-----------------------------------------------------------------------------
constexpr void NRectangle::Scale(float64_t scaleBy)
{
	*this = GetScaled(scaleBy);
}





//=============================================================================
//		NRectangle::GetInset : Get an inset rectangle.
//-----------------------------------------------------------------------------
constexpr NRectangle NRectangle::GetInset(float64_t deltaX, float64_t deltaY) const
{


	// Get an inset rectangle
	NRectangle theResult = GetNormalized();

	theResult.origin.x += deltaX;
	theResult.origin.y += deltaY;

	theResult.size.width -= (deltaX * 2.0);
	theResult.size.height -= (deltaY * 2.0);

	return theResult;
}





//=============================================================================
//		NRectangle::Inset : Inset the rectangle.
//-----------------------------------------------------------------------------
constexpr void NRectangle::Inset(float64_t deltaX, float64_t deltaY)
{
	*this = GetInset(deltaX, deltaY);
}





//=============================================================================
//		NRectangle::GetScaledToFit : Get a scaled to fit rectangle.
//-----------------------------------------------------------------------------
constexpr NRectangle NRectangle::GetScaledToFit(const NRectangle& theRect) const
{


	// Get the fitting rectangle
	//
	// We expand ourselves as much as possible, up to the size of the
	// target rectangle, then centre within the target.
	float64_t  aspectRatio = size.width / size.height;
	NRectangle theResult;

	theResult.size.width  = theRect.size.width;
	theResult.size.height = theRect.size.width / aspectRatio;

	if (theResult.size.height > theRect.size.height)
	{
		theResult.size.width  = theRect.size.height * aspectRatio;
		theResult.size.height = theRect.size.height;
	}

	theResult.SetPosition(NPosition::Center, theRect);

	return theResult;
}





//=============================================================================
//		NRectangle::ScaleToFit : Scale the rectangle to fit.
//-----------------------------------------------------------------------------
constexpr void NRectangle::ScaleToFit(const NRectangle& theRect)
{
	*this = GetScaledToFit(theRect);
}





//=============================================================================
//		NRectangle::GetPositioned : Get a positioned rectangle.
//-----------------------------------------------------------------------------
constexpr NRectangle NRectangle::GetPositioned(NPosition         thePosition,
											   const NRectangle& theRect) const
{


	// Get the state we need
	float64_t halfWidth  = (size.width / 2.0);
	float64_t halfHeight = (size.height / 2.0);



	// Position the rectangle
	NRectangle theResult(*this);

	switch (thePosition)
	{
		case NPosition::Alert:
			// The alert position is horizontally centred, with 1/3 of the
			// available vertical space above the rectangle and 2/3 below it.
			theResult.origin.x = theRect.GetMidX() - halfWidth;
			theResult.origin.y = theRect.GetMinY();

			if (theRect.size.height > theResult.size.height)
			{
				theResult.origin.y += ((theRect.size.height - theResult.size.height) / 3.0);
			}
			break;

		case NPosition::Center:
			theResult.origin.x = theRect.GetMidX() - halfWidth;
			theResult.origin.y = theRect.GetMidY() - halfHeight;
			break;

		case NPosition::Left:
			theResult.origin.x = theRect.GetMinX();
			theResult.origin.y = theRect.GetMidY() - halfHeight;
			break;

		case NPosition::Right:
			theResult.origin.x = theRect.GetMaxX() - size.width;
			theResult.origin.y = theRect.GetMidY() - halfHeight;
			break;

		case NPosition::Top:
			theResult.origin.x = theRect.GetMidX() - halfWidth;
			theResult.origin.y = theRect.GetMinY();
			break;

		case NPosition::TopLeft:
			theResult.origin.x = theRect.GetMinX();
			theResult.origin.y = theRect.GetMinY();
			break;

		case NPosition::TopRight:
			theResult.origin.x = theRect.GetMaxX() - theResult.size.width;
			theResult.origin.y = theRect.GetMinY();
			break;

		case NPosition::Bottom:
			theResult.origin.x = theRect.GetMidX() - halfWidth;
			theResult.origin.y = theRect.GetMaxY() - theResult.size.height;
			break;

		case NPosition::BottomLeft:
			theResult.origin.x = theRect.GetMinX();
			theResult.origin.y = theRect.GetMaxY() - theResult.size.height;
			break;

		case NPosition::BottomRight:
			theResult.origin.x = theRect.GetMaxX() - theResult.size.width;
			theResult.origin.y = theRect.GetMaxY() - theResult.size.height;
			break;
	}

	return theResult;
}





//=============================================================================
//		NRectangle::SetPosition : Position the rectangle.
//-----------------------------------------------------------------------------
constexpr void NRectangle::SetPosition(NPosition thePosition, const NRectangle& theRect)
{
	*this = GetPositioned(thePosition, theRect);
}





//=============================================================================
//		NRectangle::GetUnion : Get the union with a rectangle.
//-----------------------------------------------------------------------------
constexpr NRectangle NRectangle::GetUnion(const NRectangle& theRect) const
{


	// Check our state
	if (IsEmpty())
	{
		return theRect;
	}

	if (theRect.IsEmpty())
	{
		return *this;
	}



	// Get the union
	float64_t minX = std::min(GetMinX(), theRect.GetMinX());
	float64_t minY = std::min(GetMinY(), theRect.GetMinY());

	float64_t maxX = std::max(GetMaxX(), theRect.GetMaxX());
	float64_t maxY = std::max(GetMaxY(), theRect.GetMaxY());

	return NRectangle(minX, minY, maxX - minX, maxY - minY);
}





//=============================================================================
//		NRectangle::UnionWith : Union the rectangle.
//-----------------------------------------------------------------------------
constexpr void NRectangle::UnionWith(const NRectangle& theRect)
{
	*this = GetUnion(theRect);
}





//=============================================================================
//		NRectangle::GetIntersection : Get the intersection with a rectangle.
//-----------------------------------------------------------------------------
constexpr NRectangle NRectangle::GetIntersection(const NRectangle& theRect) const
{


	// Check our state
	NRectangle theResult;

	if (IsEmpty() || theRect.IsEmpty())
	{
		return theResult;
	}



	// Get the intersection
	bool notIntersect = (theRect.GetMinX() > GetMaxX() || theRect.GetMaxX() < GetMinX() ||
						 theRect.GetMinY() > GetMaxY() || theRect.GetMaxY() < GetMinY());

	if (!notIntersect)
	{
		float64_t minX = std::max(GetMinX(), theRect.GetMinX());
		float64_t minY = std::max(GetMinY(), theRect.GetMinY());

		float64_t maxX = std::min(GetMaxX(), theRect.GetMaxX());
		float64_t maxY = std::min(GetMaxY(), theRect.GetMaxY());

		theResult = NRectangle(minX, minY, maxX - minX, maxY - minY);
	}

	return theResult;
}





//=============================================================================
//		NRectangle::IntersectWith : Intersect two rectangles.
//-----------------------------------------------------------------------------
constexpr void NRectangle::IntersectWith(const NRectangle& theRect)
{
	*this = GetIntersection(theRect);
}





#pragma mark NMixinComparable
//=============================================================================
//		NRectangle::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
constexpr bool NRectangle::CompareEqual(const NRectangle& theRectangle) const
{


	// Compare the rectangle
	return origin == theRectangle.origin && size == theRectangle.size;
}





//=============================================================================
//		NRectangle::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
constexpr NComparison NRectangle::CompareOrder(const NRectangle& theRectangle) const
{


	// Order the rectangle
	//
	// A rectangle has no real ordering so we sort by size (area) and
	// then by origin.
	NComparison theResult = NCompare(size, theRectangle.size);

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(origin, theRectangle.origin);
	}

	return theResult;
}
