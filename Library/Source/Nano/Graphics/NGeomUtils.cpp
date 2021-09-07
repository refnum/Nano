/*	NAME:
		NGeomUtils.cpp

	DESCRIPTION:
		Geometry utilities.

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

// Nano
#include "NMathUtils.h"
#include "NRectangle.h"
#include "NShape.h"





//=============================================================================
//		NGeomUtils::GetLength : Get the length of a line.
//-----------------------------------------------------------------------------
float64_t NGeomUtils::GetLength(const NVectorPoint& thePoints)
{


	// Measure the line
	size_t    numPoints = thePoints.size();
	float64_t theLength = 0.0;

	if (numPoints >= 2)
	{
		for (size_t n = 0; n < (numPoints - 1); n++)
		{
			theLength += thePoints[n].GetDistance(thePoints[n + 1]);
		}
	}

	return theLength;
}





//=============================================================================
//		NGeomUtils::GetBounds : Get the bounds.
//-----------------------------------------------------------------------------
NRectangle NGeomUtils::GetBounds(const NVectorPoint& thePoints)
{


	// Get the state we need
	float64_t minX = 0.0;
	float64_t maxX = 0.0;

	float64_t minY = 0.0;
	float64_t maxY = 0.0;

	size_t numPoints = thePoints.size();
	if (numPoints != 0)
	{
		minX = maxX = thePoints[0].x;
		minY = maxY = thePoints[0].y;
	}



	// Get the bounds
	for (const auto& thePoint : thePoints)
	{
		minX = std::min(minX, thePoint.x);
		maxX = std::max(maxX, thePoint.x);

		minY = std::min(minY, thePoint.y);
		maxY = std::max(maxY, thePoint.y);
	}

	return NRectangle(minX, minY, maxX - minX, maxY - minY);
}





//=============================================================================
//		NGeomUtils::GetPointOnLine : Get a point on a line.
//-----------------------------------------------------------------------------
NPoint NGeomUtils::GetPointOnLine(const NVectorPoint& thePoints, float64_t theOffset)
{


	// Validate our parameters
	NN_REQUIRE(thePoints.size() >= 2);
	NN_REQUIRE(theOffset >= 0.0 && theOffset <= 1.0);



	// Find the intersection point
	//
	// To calculate the intersection we need to calculate the intersection
	// as if the line was entirely straight (intersectAt), and then re-walk
	// the line to determine which segment (and where in that segment) the
	// intersection point occurs.
	float64_t intersectAt  = GetLength(thePoints) * theOffset;
	float64_t segmentStart = 0.0;
	float64_t segmentEnd   = 0.0;
	size_t    numPoints    = thePoints.size();

	for (size_t n = 0; n < (numPoints - 1); n++)
	{
		// Identify the bounds of this segment
		float64_t dx        = (thePoints[n + 1].x - thePoints[n].x);
		float64_t dy        = (thePoints[n + 1].y - thePoints[n].y);
		float64_t theLength = sqrt((dx * dx) + (dy * dy));

		segmentStart = segmentEnd;
		segmentEnd += theLength;


		// Stop once we find the intersecting segment
		//
		// By calculating the %ge offset of the intersection within this
		// segment, we can determine how much of the segment delta to use
		// in order to find the location of the intersection.
		if (intersectAt <= segmentEnd)
		{
			NPoint    theResult;
			float64_t theDelta = (intersectAt - segmentStart) / (segmentEnd - segmentStart);
			theResult.x        = (thePoints[n].x + (dx * theDelta));
			theResult.y        = (thePoints[n].y + (dy * theDelta));

			return theResult;
		}
	}



	// Handle failure
	NN_LOG_WARNING("Unable to find intersection at offset {}", theOffset);

	return {};
}





//=============================================================================
//		NGeomUtils::GetClosestPointOnLine : Get the closest point on a line.
//-----------------------------------------------------------------------------
NPoint NGeomUtils::GetClosestPointOnLine(const NPoint& thePoint,
										 const NPoint& lineStart,
										 const NPoint& lineEnd,
										 float64_t*    lineU)
{


	// Get the state we need
	NPoint    theResult;
	float64_t u = 0.0;



	// Get the closest point on the line
	if (lineStart == lineEnd)
	{
		theResult = lineStart;
		u         = 0.0;
	}
	else
	{
		// Calculate the intersection point
		float64_t dx  = lineEnd.x - lineStart.x;
		float64_t dy  = lineEnd.y - lineStart.y;
		float64_t mag = lineStart.GetDistance2(lineEnd);

		u = (((thePoint.x - lineStart.x) * dx) + ((thePoint.y - lineStart.y) * dy)) / mag;


		// Get the closest point
		if (u < 0.0)
		{
			theResult = lineStart;
		}
		else if (u > 1.0)
		{
			theResult = lineEnd;
		}
		else
		{
			theResult.x = lineStart.x + (dx * u);
			theResult.y = lineStart.y + (dy * u);
		}
	}



	// Get the intersection
	if (lineU != nullptr)
	{
		*lineU = u;
	}

	return theResult;
}





//=============================================================================
//		NGeomUtils::CompareRectangleToShape : Compare a rectangle to a shape.
//-----------------------------------------------------------------------------
NPlacement NGeomUtils::CompareRectangleToShape(const NRectangle& theRectangle,
											   const NShape&     theShape)
{


	// Validate our parameters
	NN_REQUIRE(theShape.points.size() >= 3);
	NN_REQUIRE(theShape.loops.size() >= 1);



	// Test the bounding box
	if (!theRectangle.Intersects(theShape.GetBounds()))
	{
		return NPlacement::Outside;
	}



	// Test the polygon
	size_t numPoints = theShape.points.size();
	size_t numLoops  = theShape.loops.size();

	for (size_t n = 0; n < numLoops; n++)
	{
		// Get the state we need
		size_t firstPoint = theShape.loops[n];
		size_t loopPoints = 0;

		if (n == (numLoops - 1))
		{
			loopPoints = numPoints - firstPoint;
		}
		else
		{
			loopPoints = theShape.loops[n + 1] - firstPoint;
		}



		// Test the polygon/hole
		//
		// The first loop is the polygon, and subsequent loops are holes.
		//
		// If the rectangle falls inside the polygon, we must continue testing until we
		// examine all the holes (or find one that contains or intersects the rectangle).
		NPlacement theResult =
			CompareRectangleToPolygon(theRectangle, loopPoints, &theShape.points[firstPoint]);

		if (n == 0)
		{
			// Rectangle wasn't entirely inside the polygon
			if (theResult != NPlacement::Inside)
			{
				return theResult;
			}
		}

		else
		{
			// Rectangle fell through a hole
			if (theResult == NPlacement::Inside)
			{
				return NPlacement::Outside;
			}


			// Or intersects the hole boundary
			else if (theResult == NPlacement::Intersects)
			{
				return NPlacement::Intersects;
			}
		}
	}

	return NPlacement::Inside;
}





//=============================================================================
//		NGeomUtils::CompareRectangleToPoly : Compare a rectangle to a polygon.
//-----------------------------------------------------------------------------
NPlacement NGeomUtils::CompareRectangleToPolygon(const NRectangle& theRect,
												 size_t            numPoints,
												 const NPoint*     thePoints)
{


	// Validate our parameters
	NN_REQUIRE(numPoints >= 3);



	// Test the final segment
	//
	// To simplify the loop, we test the final segment first.
	NVectorPoint theSegment;

	NPoint     p0         = thePoints[numPoints - 1];
	NPlacement prevResult = ClipLineToRectangle(theRect, {p0, thePoints[0]}, theSegment);

	if (prevResult == NPlacement::Intersects)
	{
		return NPlacement::Intersects;
	}



	// Test the polygon
	NPlacement theResult = prevResult;

	for (size_t n = 0; n < numPoints; n++)
	{
		// Check the segment
		//
		// Any change in type implies an intersection, so we can stop.
		NPoint p1 = thePoints[n];
		theResult = ClipLineToRectangle(theRect, {p0, p1}, theSegment);

		if (theResult != prevResult)
		{
			return NPlacement::Intersects;
		}



		// Update our state
		p0         = p1;
		prevResult = theResult;
	}

	NN_REQUIRE(theResult == NPlacement::Inside || theResult == NPlacement::Outside);



	// Handle containment
	//
	// If all lines are within the rectangle, the polygon is entirely contained
	// by the rectangle. We treat this as an intersection, since the rectangle
	// overlaps the same area as the polygon.
	if (theResult == NPlacement::Inside)
	{
		return NPlacement::Intersects;
	}



	// Classify the result
	//
	// If all lines are outside the rectangle, the rectangle may be outside the
	// polygon or may be entirely contained by the polygon.
	//
	// Testing a rectangle corner against the polgyon lets us see which we have.
	return ComparePointToPolygon(theRect.origin, numPoints, thePoints);
}





//=============================================================================
//		NGeomUtils::ComparePointToPolygon : Compare a point to a polygon.
//-----------------------------------------------------------------------------
NPlacement NGeomUtils::ComparePointToPolygon(const NPoint& thePoint,
											 size_t        numPoints,
											 const NPoint* thePoints)
{


	// Validate our parameters
	NN_REQUIRE(numPoints >= 3);



	// Test the polygon
	int32_t crossNum = 0;
	NPoint  p1       = thePoints[numPoints - 1];
	bool    p1Above  = (p1.y > thePoint.y);

	for (size_t n = 0; n < numPoints; n++)
	{
		// Look for a crossing in y
		NPoint p2      = thePoints[n];
		bool   p2Above = (p2.y > thePoint.y);

		if (p1Above != p2Above)
		{
			// If the segment is entirely to the left in x, it can't cross
			if (std::max(p1.x, p2.x) < thePoint.x)
			{
				// Can't cross
			}


			// If the segment is entirely to the right in x, it must cross
			else if (std::min(p1.x, p2.x) > thePoint.x)
			{
				crossNum++;
			}


			// Otherwise we need to check the intersection
			else
			{
				float64_t w = (thePoint.y - p1.y) / (p2.y - p1.y);
				float64_t c = p1.x + (w * (p2.x - p1.x));

				if (thePoint.x < c)
				{
					crossNum++;
				}
			}
		}

		p1      = p2;
		p1Above = p2Above;
	}



	// Process the result
	if (NMathUtils::IsOdd(crossNum))
	{
		return NPlacement::Inside;
	}
	else
	{
		return NPlacement::Outside;
	}
}





//=============================================================================
//		NGeomUtils::ClipPolygonToRectangle : Clip a polygon to a rectangle.
//-----------------------------------------------------------------------------
NVectorPoint NGeomUtils::ClipPolygonToRectangle(const NVectorPoint& thePolygon,
												const NRectangle&   theRectangle)
{


	// Get the state we need
	NPoint pTL = theRectangle.GetPoint(NPosition::TopLeft);
	NPoint pTR = theRectangle.GetPoint(NPosition::TopRight);
	NPoint pBL = theRectangle.GetPoint(NPosition::BottomLeft);
	NPoint pBR = theRectangle.GetPoint(NPosition::BottomRight);



	// Clip using Sutherland-Hodgman
	NVectorPoint tmpPolygon1, tmpPolygon2;

	ClipPolygonToEdge(pTL, pBL, thePolygon, tmpPolygon1);
	ClipPolygonToEdge(pBL, pBR, tmpPolygon1, tmpPolygon2);
	ClipPolygonToEdge(pBR, pTR, tmpPolygon2, tmpPolygon1);
	ClipPolygonToEdge(pTR, pTL, tmpPolygon1, tmpPolygon2);

	return tmpPolygon2;
}





//=============================================================================
//		NGeomUtils::ClipLineToRectangle : Clip a line to a rectangle.
//-----------------------------------------------------------------------------
//		Note : Clips using Cohen-Sutherland.
//-----------------------------------------------------------------------------
NPlacement NGeomUtils::ClipLineToRectangle(const NRectangle&   theRect,
										   const NVectorPoint& theInput,
										   NVectorPoint&       theOutput)
{


	// Get the state we need
	NPoint p0 = theInput[0];
	NPoint p1 = theInput[1];

	NClipFlags code0 = GetClipCode(theRect, p0);
	NClipFlags code1 = GetClipCode(theRect, p1);


	// Both points are outside
	if (code0 & code1)
	{
		theOutput.clear();
		return NPlacement::Outside;
	}



	// Both points are inside
	else if (!(code0 | code1))
	{
		theOutput = theInput;
		return NPlacement::Inside;
	}



	// Intersection
	//
	// We pick an exterior point, then clip the segment to an edge.
	NPoint     pointOut;
	NClipFlags codeOut;

	if (code0)
	{
		codeOut  = code0;
		pointOut = p0;
	}
	else
	{
		codeOut  = code1;
		pointOut = p1;
	}

	float64_t dx = p1.x - p0.x;
	float64_t dy = p1.y - p0.y;

	float64_t minX = theRect.GetMinX();
	float64_t maxX = theRect.GetMaxX();

	float64_t minY = theRect.GetMinY();
	float64_t maxY = theRect.GetMaxY();



	// Split line at top of rectangle
	if (codeOut & NClip::Top)
	{
		pointOut.x = p0.x + (dx * ((maxY - p0.y) / dy));
		pointOut.y = maxY;
	}

	// Split line at bottom of rectangle
	else if (codeOut & NClip::Bottom)
	{
		pointOut.x = p0.x + (dx * ((minY - p0.y) / dy));
		pointOut.y = minY;
	}

	// Split line at right edge of rectangle
	else if (codeOut & NClip::Right)
	{
		pointOut.x = maxX;
		pointOut.y = p0.y + (dy * ((maxX - p0.x) / dx));
	}

	// Split line at left edge of rectangle
	else if (codeOut & NClip::Left)
	{
		pointOut.x = minX;
		pointOut.y = p0.y + (dy * ((minX - p0.x) / dx));
	}

	else
	{
		NN_LOG_ERROR("Invalid codeOut!");
	}



	// Clip the new segment
	NPlacement theResult = NPlacement::Inside;

	if (codeOut == code0)
	{
		theResult = ClipLineToRectangle(theRect, {pointOut, p1}, theOutput);
	}
	else
	{
		theResult = ClipLineToRectangle(theRect, {p0, pointOut}, theOutput);
	}



	// Update the result
	//
	// Since we know we had an intersection with one of the rectangle edges,
	// a result of inside is actually an intersection.
	//
	// A result of outside indicates the line intersected an edge but didn't
	// encroach on the rectangle itself, so that result remains.
	if (theResult == NPlacement::Inside)
	{
		theResult = NPlacement::Intersects;
	}

	return theResult;
}





//=============================================================================
//		NGeomUtils::GetClipCode : Get a Sutherland-Cohen clip code.
//-----------------------------------------------------------------------------
NClipFlags NGeomUtils::GetClipCode(const NRectangle& theRect, const NPoint& thePoint)
{


	// Get the state we need
	NClipFlags theCode;



	// Test top/bottom edges
	if (thePoint.y > theRect.GetMaxY())
	{
		theCode |= NClip::Top;
	}

	else if (thePoint.y < theRect.GetMinY())
	{
		theCode |= NClip::Bottom;
	}



	// Test left/right edges
	if (thePoint.x > theRect.GetMaxX())
	{
		theCode |= NClip::Right;
	}

	else if (thePoint.x < theRect.GetMinX())
	{
		theCode |= NClip::Left;
	}

	return theCode;
}





#pragma mark private
//=============================================================================
//		NGeomUtils::ClipPolygonToEdge : Clip a polygon to an edge.
//-----------------------------------------------------------------------------
void NGeomUtils::ClipPolygonToEdge(const NPoint&       edgeStart,
								   const NPoint&       edgeEnd,
								   const NVectorPoint& theInput,
								   NVectorPoint&       theOutput)
{


	// Get the state we need
	//
	// Reserve space up front as most polygons produce a similar size of output.
	theOutput.clear();

	if (theInput.empty())
	{
		return;
	}

	theOutput.reserve(theInput.size());



	// Perform the clip
	//
	// Each segment is compared to the clipping edge, which gives four cases:
	//
	//	1) Both endpoints (s and p) are inside the clipping edge, so p is added.
	//	2) Both endpoints (s and p) are outside the clipping edge, so nothing is added.
	//	3) Vertex s is inside and p is outside, so the intersection (i) is added.
	//	4) Vertex p is inside and s is outside, so the intersection (i) and p are added.
	//
	NPoint s = theInput.back();

	for (const auto& p : theInput)
	{
		// Cases 1 or 4
		if (IsInside(edgeStart, edgeEnd, p))
		{
			// Case 1
			if (IsInside(edgeStart, edgeEnd, s))
			{
				theOutput.push_back(p);
			}

			// Case 4
			else
			{
				NPoint i = GetIntersection(edgeStart, edgeEnd, s, p);

				theOutput.push_back(i);
				theOutput.push_back(p);
			}
		}


		// Cases 2 or 3
		else
		{
			// Case 3
			if (IsInside(edgeStart, edgeEnd, s))
			{
				NPoint i = GetIntersection(edgeStart, edgeEnd, s, p);
				theOutput.push_back(i);
			}


			// Case 2
			else
			{
			}
		}

		s = p;
	}
}





//=============================================================================
//		NGeomUtils::IsInside : Is a point inside an edge?
//-----------------------------------------------------------------------------
bool NGeomUtils::IsInside(const NPoint& edgeStart, const NPoint& edgeEnd, const NPoint& thePoint)
{


	// Bottom edge
	if (edgeEnd.x > edgeStart.x)
	{
		if (thePoint.y <= edgeStart.y)
		{
			return true;
		}
	}


	// Top edge
	if (edgeStart.x < edgeEnd.x)
	{
		if (thePoint.y >= edgeStart.y)
		{
			return true;
		}
	}


	// Right edge
	if (edgeEnd.y < edgeStart.y)
	{
		if (thePoint.x <= edgeEnd.x)
		{
			return true;
		}
	}


	// Left edge
	if (edgeEnd.y > edgeStart.y)
	{
		if (thePoint.x >= edgeEnd.x)
		{
			return true;
		}
	}

	return false;
}





//=============================================================================
//		NGeomUtils::GetIntersection : Get the intersection with an edge.
//-----------------------------------------------------------------------------
NPoint NGeomUtils::GetIntersection(const NPoint& edgeStart,
								   const NPoint& edgeEnd,
								   const NPoint& lineStart,
								   const NPoint& lineEnd)
{


	// Calculate the intersectin point
	NPoint thePoint;

	if (edgeStart.y == edgeEnd.y)
	{
		thePoint.y = edgeStart.y;
		thePoint.x = lineStart.x + ((edgeStart.y - lineStart.y) *
									((lineEnd.x - lineStart.x) / (lineEnd.y - lineStart.y)));
	}
	else
	{
		thePoint.x = edgeStart.x;
		thePoint.y = lineStart.y + ((edgeStart.x - lineStart.x) *
									((lineEnd.y - lineStart.y) / (lineEnd.x - lineStart.x)));
	}

	return thePoint;
}
