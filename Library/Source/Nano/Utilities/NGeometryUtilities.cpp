/*  NAME:
        NGeometryUtilities.cpp

    DESCRIPTION:
        Geometry utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2010, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
//============================================================================
//		Linker
//----------------------------------------------------------------------------
#ifndef NGEOMETRYUTILITIES_CPP

#include "NGeometryUtilities.h"

void NGeometryUtilities_SuppressNoCodeLinkerWarning(void);
void NGeometryUtilities_SuppressNoCodeLinkerWarning(void)
{
}

#else





//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
static const NBitfield kNGeometryClipNone							= 0;
static const NBitfield kNGeometryClipLeft							= (1 << 0);
static const NBitfield kNGeometryClipRight							= (1 << 1);
static const NBitfield kNGeometryClipTop							= (1 << 2);
static const NBitfield kNGeometryClipBottom							= (1 << 3);





//============================================================================
//		NGeometryUtilities::GetLength : Get the length of a line.
//----------------------------------------------------------------------------
template<class T> T NGeometryUtilities::GetLength(const std::vector< NPointT<T> > &theLine, bool getApprox)
{	NIndex		n, numPoints;
	T			theLength;



	// Measure the line
	theLength = 0.0;
	numPoints = theLine.size();

	if (numPoints >= 2)
		{
		for (n = 0; n < (numPoints-1); n++)
			theLength += theLine[n].GetDistance(theLine[n+1], getApprox);
		}
	
	return(theLength);
}





//============================================================================
//		NGeometryUtilities::GetBounds : Get the bounds.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NGeometryUtilities::GetBounds(const std::vector< NPointT<T> > &thePoints)
{	T					minX, maxX, minY, maxY;
	NIndex				n, numPoints;
	NRectangleT<T>		theResult;
	NPointT<T>			thePoint;



	// Get the state we need
	numPoints = thePoints.size();

	minX = maxX = (numPoints != 0) ? thePoints[0].x : ((T) 0.0);
	minY = maxY = (numPoints != 0) ? thePoints[0].y : ((T) 0.0);



	// Get the bounds
	for (n = 0; n < numPoints; n++)
		{
		thePoint = thePoints[n];
		
		minX = std::min(minX, thePoint.x);
		maxX = std::max(maxX, thePoint.x);

		minY = std::min(minY, thePoint.y);
		maxY = std::max(maxY, thePoint.y);
		}
	
	
	
	// Get the bounds
	theResult = NRectangleT<T>(minX, minY, maxX - minX, maxY - minY);
	
	return(theResult);
}





//============================================================================
//      NGeometryUtilities::GetPointOnLine : Get a point on a line.
//----------------------------------------------------------------------------
template<class T> NPointT<T> NGeometryUtilities::GetPointOnLine(const std::vector< NPointT<T> > &thePoints, T theOffset)
{	T				theLength, intersectAt, segmentStart, segmentEnd;
	T				dx, dy, theDelta;
	NIndex			n, numPoints;
	NPointT<T>		theResult;



	// Validate our parameters
	NN_ASSERT(thePoints.size() >= 2);
	NN_ASSERT(theOffset        >= 0.0 && theOffset <= 1.0);



	// Get the state we need
	numPoints   = thePoints.size();
	theLength   = GetLength(thePoints);
	intersectAt = theLength * theOffset;



	// Find the intersection point
	//
	// To calculate the intersection we need to calculate the intersection
	// as if the line was entirely straight (intersectAt), and then re-walk
	// the line to determine which segment (and where in that segment) the
	// intersection point occurs.
	segmentStart = 0.0;
	segmentEnd   = 0.0;

	for (n = 0; n < (numPoints-1); n++)
		{
		// Identify the bounds of this segment
		dx = (thePoints[n+1].x - thePoints[n].x);
		dy = (thePoints[n+1].y - thePoints[n].y);
		
		theLength    = sqrt((dx * dx) + (dy * dy));
		segmentStart = segmentEnd;
		segmentEnd  += theLength;
		
		
		// Stop once we find the intersecting segment
		//
		// By calculating the %ge offset of the intersection within this
		// segment, we can determine how much of the segment delta to use
		// in order to find the location of the intersection.
		if (intersectAt <= segmentEnd)
			{
			theDelta    = (intersectAt - segmentStart) / (segmentEnd - segmentStart);
			theResult.x = (thePoints[n].x + (dx * theDelta));
			theResult.y = (thePoints[n].y + (dy * theDelta));
			
			return(theResult);
			}
		}



	// Handle failure
	NN_LOG("Unable to find intersection at offset %g", theOffset);
	return(theResult);
}





//============================================================================
//		NGeometryUtilities::GetClosestPointOnLine : Get the closest point on a line.
//----------------------------------------------------------------------------
template<class T> NPointT<T> NGeometryUtilities::GetClosestPointOnLine(const NPointT<T> &thePoint, const NPointT<T> &lineStart, const NPointT<T> &lineEnd, T *lineU)
{	T				dx, dy, u, mag;
	NPointT<T>		theResult;



	// Get the closest point on the line
    if (NMathUtilities::AreEqual(lineStart.x, lineEnd.x) &&
        NMathUtilities::AreEqual(lineStart.y, lineEnd.y))
        {
        theResult = lineStart;
		u         = 0.0;
        }
    else
        {
        // Calculate the intersection point
        dx  = lineEnd.x - lineStart.x;
        dy  = lineEnd.y - lineStart.y;

        mag = lineStart.GetDistance2(lineEnd);
        u   = (((thePoint.x - lineStart.x) * dx) + ((thePoint.y - lineStart.y) * dy)) / mag;


        // Get the closest point
        if (u < 0.0)
            theResult = lineStart;
        else if (u > 1.0)
            theResult = lineEnd;
        else
            {
            theResult.x = lineStart.x + (dx * u);
            theResult.y = lineStart.y + (dy * u);
            }
        }



	// Get the intersection
	if (lineU != NULL)
		*lineU = u;

	return(theResult);
}





//============================================================================
//		NGeometryUtilities::CompareRectangleToShape : Compare a rectangle to a shape.
//----------------------------------------------------------------------------
template<class T> NGeometryComparison NGeometryUtilities::CompareRectangleToShape(const NRectangleT<T> &theRectangle, const NShapeT<T> &theShape)
{	NIndex						n, firstPoint, numPoints, numLoops, loopPoints;
	const NPointT<T>			*thePoints;
	const NIndex				*theLoops;
	NGeometryComparison			theResult;
	std::vector< NPointT<T> >	tmpPoly;



	// Validate our parameters
	NN_ASSERT(theShape.points.size() >= 3);
	NN_ASSERT(theShape.loops.size()  >= 1);



	// Test the bounding box
	if (!theRectangle.Intersects(theShape.GetBounds()))
		return(kNGeometryOutside);



	// Get the state we need
	numPoints =  theShape.points.size();
	thePoints = &theShape.points[0];

	numLoops  =  theShape.loops.size();
	theLoops  = &theShape.loops[0];



	// Test the polygon
	for (n = 0; n < numLoops; n++)
		{
		// Get the state we need
		firstPoint = theLoops[n];

		if (n == (numLoops - 1))
			loopPoints = numPoints       - firstPoint;
		else
			loopPoints = theLoops[n + 1] - firstPoint;



		// Test the polygon/hole
		//
		// The first loop is the polygon, and subsequent loops are holes.
		//
		// If the rectangle falls inside the polygon, we must continue testing until we
		// examine all the holes (or find one that contains or intersects the rectangle).
		theResult = CompareRectangleToPolygon(theRectangle, loopPoints, &thePoints[firstPoint]);

		if (n == 0)
			{
			// Rectangle wasn't entirely inside the polygon
			if (theResult != kNGeometryInside)
				return(theResult);
			}

		else
			{
			// Rectangle fell through a hole
			if (theResult == kNGeometryInside)
				return(kNGeometryOutside);
			
			
			// Or intersects the hole boundary
			else if (theResult == kNGeometryIntersects)
				return(kNGeometryIntersects);
			}
		}
	
	return(kNGeometryInside);
}





//=============================================================================
//		NGeometryUtilities::CompareRectangleToPoly : Compare a rectangle to a polygon.
//-----------------------------------------------------------------------------
template<class T> NGeometryComparison NGeometryUtilities::CompareRectangleToPolygon(const NRectangleT<T> &theRect, NIndex numPoints, const NPointT<T> *thePoints)
{	NGeometryComparison		theResult, prevResult;
	NPointT<T>				p0, p1;
	NIndex					n;
	


	// Validate our parameters
	NN_ASSERT(numPoints >= 3);



	// Test the final segment
	//
	// To simplify the loop, we test the final segment first.
	p0         = thePoints[numPoints - 1];
	prevResult = CompareLineToRectangle(theRect, p0, thePoints[0]);

	if (prevResult == kNGeometryIntersects)
		return(kNGeometryIntersects);



	// Test the polygon
	theResult = prevResult;

	for (n = 0; n < numPoints; n++)
		{
		// Check the segment
		//
		// Any change in type, or an intersection, and we can stop.
		p1        = thePoints[n];
		theResult = CompareLineToRectangle(theRect, p0, p1);
		
		if (theResult != prevResult)
			return(kNGeometryIntersects);



		// Update our state
		p0         = p1;
		prevResult = theResult;
		}
	
	NN_ASSERT(theResult == kNGeometryInside || theResult == kNGeometryOutside);



	// Handle containment
	//
	// If all lines are within the rectangle, the polygon is entirely contained
	// by the rectangle. We treat this as an intersection, since the rectangle
	// overlaps the same area as the polygon.
	if (theResult == kNGeometryInside)
		return(kNGeometryIntersects);



	// Classify the result
	//
	// If all lines are outside the rectangle, the rectangle may be outside the
	// polygon or may be entirely contained by the polygon.
	//
	// Testing a rectangle corner against the polgyon lets us see which we have.
	return(ComparePointToPolygon(theRect.origin, numPoints, thePoints));
}





//=============================================================================
//		NGeometryUtilities::ComparePointToPolygon : Compare a point to a polygon.
//-----------------------------------------------------------------------------
template<class T> NGeometryComparison NGeometryUtilities::ComparePointToPolygon(const NPointT<T> &thePoint, NIndex numPoints, const NPointT<T> *thePoints)
{	bool			p1Above, p2Above;
	SInt32			crossNum;
	NPointT<T>		p1, p2;
 	T				c, w;
    NIndex			n;



	// Validate our parameters
	NN_ASSERT(numPoints >= 3);



	// Test the polygon
	crossNum = 0;
	p1       = thePoints[numPoints-1];
	p1Above  = (p1.y > thePoint.y);
	
	for (n = 0; n < numPoints; n++)
		{
		// Look for a crossing in y
		p2      = thePoints[n];
		p2Above = (p2.y > thePoint.y);

		if (p1Above != p2Above)
			{
			// If the segment is entirely to the left in x, it can't cross
			if (std::max(p1.x, p2.x) < thePoint.x)
				;
			
			
			// If the segment is entirely to the right in x, it must cross
			else if (std::min(p1.x, p2.x) > thePoint.x)
				crossNum++;
			
			
			// Otherwise we need to check the intersection
			else
				{
				w = (thePoint.y - p1.y) / (p2.y - p1.y);
				c = p1.x + (w * (p2.x - p1.x));
				
				if (thePoint.x < c)
					crossNum++;
				}
			}
		
		p1      = p2;
		p1Above = p2Above;
		}



	// Process the result
    if (NMathUtilities::IsOdd(crossNum))
		return(kNGeometryInside);
	else
		return(kNGeometryOutside);
}





//=============================================================================
//		NGeometryUtilities::ClipLine : Clip a line to a rectangle.
//-----------------------------------------------------------------------------
template<class T> std::vector< NPointT<T> >	NGeometryUtilities::ClipLine(const std::vector< NPointT<T> > &theLine, const NRectangleT<T> &theRectangle)
{	NPointT<T>						pTL, pTR, pBL, pBR;
	std::vector< NPointT<T> >		tmpLine1, tmpLine2;



	// Get the state we need
	pTL = theRectangle.GetPoint(kNPositionTopLeft);
	pTR = theRectangle.GetPoint(kNPositionTopRight);
	pBL = theRectangle.GetPoint(kNPositionBottomLeft);
	pBR = theRectangle.GetPoint(kNPositionBottomRight);



	// Clip the line against each edge
	ClipToEdge(pTL, pBL, theLine,  tmpLine1);
	ClipToEdge(pBL, pBR, tmpLine1, tmpLine2);
	ClipToEdge(pBR, pTR, tmpLine2, tmpLine1);
	ClipToEdge(pTR, pTL, tmpLine1, tmpLine2);
	
	return(tmpLine2);
}





//=============================================================================
//		NGeometryUtilities::CompareLineToRectangle : Compare a line to a rectangle.
//-----------------------------------------------------------------------------
#pragma mark -
template<class T> NGeometryComparison NGeometryUtilities::CompareLineToRectangle(const NRectangleT<T> &theRect, const NPointT<T> &p0, const NPointT<T> &p1)
{	T				dx, dy, minX, maxX, minY, maxY;
	NBitfield		code0, code1, codeOut;
	NPointT<T>		pointOut;



	// Get the state we need
	code0 = GetClipCode(theRect, p0);
	code1 = GetClipCode(theRect, p1);



	// Both points are outside
	if ((code0 & code1) != 0)
		return(kNGeometryOutside);
	
	
	// Both points are inside
	else if ((code0 | code1) == 0)
		return(kNGeometryInside);


	// One point is outside and one is inside
	else if (code0 == 0 || code1 == 0)
		return(kNGeometryIntersects);


	// Possible intersection
	else
		{
		// Pick an exterior point
		if (code0 != 0)
			{
			codeOut  = code0;
			pointOut = p0;
			}
		else
			{
			codeOut  = code1;
			pointOut = p1;
			}
		
		
		
		// Get the state we need
		dx = p1.x - p0.x;
		dy = p1.y - p0.y;
		
		minX = theRect.origin.x;
		maxX = theRect.origin.x + theRect.size.width;

		minY = theRect.origin.y;
		maxY = theRect.origin.y + theRect.size.height;



		// Clip the segment to the rectangle edge
		if (codeOut & kNGeometryClipTop)
			{
			// Split line at top of rectangle
			pointOut.x = p0.x + (dx * ((maxY - p0.y) / dy));
			pointOut.y = maxY;
			}
			
		else if (codeOut & kNGeometryClipBottom)
			{
			// Split line at bottom of rectangle
			pointOut.x = p0.x + (dx * ((minY - p0.y) / dy));
			pointOut.y = maxY;
			}
			
			
		if (codeOut & kNGeometryClipRight)
			{
			// Split line at right edge of rectangle
			pointOut.y = p0.y + (dy * ((maxX - p0.x) / dx));
			pointOut.x = maxX;
			}
			
		else if (codeOut & kNGeometryClipLeft)
			{
			// Split line at left edge of rectangle
			pointOut.y = p0.y + (dy * ((minX - p0.x) / dx));
			pointOut.x = minX;
			}



		// Test the new segment
		if (codeOut == code0)
			return(CompareLineToRectangle(theRect, pointOut, p1));
		else
			return(CompareLineToRectangle(theRect, p0, pointOut));
		}



	// Never reached
	NN_ASSERT(false);

	return(kNGeometryIntersects);
}





//=============================================================================
//		NGeometryUtilities::GetClipCode : Get a Sutherland-Cohen clip code.
//-----------------------------------------------------------------------------
template<class T> NBitfield NGeometryUtilities::GetClipCode(const NRectangleT<T> &theRect, const NPointT<T> &thePoint)
{	NBitfield	theCode;



	// Calculate the clp code
	theCode = kNGeometryClipNone;

	if (thePoint.y > (theRect.origin.y + theRect.size.height))
		theCode |= kNGeometryClipTop;

	else if (thePoint.y < theRect.origin.y)
		theCode |= kNGeometryClipBottom;

	if (thePoint.x > (theRect.origin.x + theRect.size.width))
		theCode |= kNGeometryClipRight;

	else if (thePoint.x < theRect.origin.x)
		theCode |= kNGeometryClipLeft;
	
	return(theCode);
}





//============================================================================
//		NGeometryUtilities::ClipToEdge : Clip a line to an edge.
//----------------------------------------------------------------------------
template<class T> void NGeometryUtilities::ClipToEdge(const NPointT<T> &edgeStart, const NPointT<T> &edgeEnd, const std::vector< NPointT<T> > &theLine, NPointT<T> &theResult)
{	NIndex			n, numPoints;
	NPointT<T>		s, p, i;



	// Get the state we need
	numPoints = theLine.size();
	if (numPoints == 0)
		return;



	// Cip the line
	//
	// Each segment is compared to the clipping edge, which gives four cases:
	//
	//	1) Both endpoints (s and p) are inside the clipping edge, so p is added.
	//	2) Both endpoints (s and p) are outside the clipping edge, so nothing is added.
	//	3) Vertex s is inside and p is outside, so the intersection (i) is added.
	//	4) Vertex p is inside and s is outside, so the intersection (i) and p are added.
	//
	// Since most lines end up about the same size, space is reserved prior to adding.
	theResult.clear();
	theResult.reserve(numPoints);

	s = theLine[numPoints - 1];
	for (n = 0; n < numPoints; n++)
		{
		p = theLine[n];


		// Cases 1 or 4
		if (IsInside(edgeStart, edgeEnd, p))
			{
			// Case 1
			if (IsInside(edgeStart, edgeEnd, s))
				theResult.push_back(p);
			
			// Case 4
			else
				{
				i = GetIntersection(edgeStart, edgeEnd, s, p);
				
				theResult.push_back(i);
				theResult.push_back(p);
				}
			}


		// Cases 2 or 3
		else
			{
			// Case 3
			if (IsInside(edgeStart, edgeEnd, s))
				{
				i = GetIntersection(edgeStart, edgeEnd, s, p);
				theResult.push_back(i);
				}
			
			
			// Case 2
			else
				{ }
			}
		
		s = p;
		}
}





//============================================================================
//		NGeometryUtilities::IsInside : Is a point inside an edge?
//----------------------------------------------------------------------------
template<class T> bool NGeometryUtilities::IsInside(const NPointT<T> &edgeStart, const NPointT<T> &edgeEnd, const NPointT<T> &thePoint)
{


	// Bottom edge
	if (edgeEnd.x > edgeStart.x)
		{
		if (thePoint.y >= edgeStart.y)
			return(true);
		}


	// Top edge
	if (edgeEnd.x < edgeEnd.x)
		{
		if (thePoint.y <= edgeStart.y)
			return(true);
		}


	// Right edge
	if (edgeEnd.y > edgeStart.y)
		{
		if (thePoint.x <= edgeEnd.x)
			return(true);
		}


	// Left edge
	if (edgeEnd.y < edgeStart.y)
		{
		if (thePoint.x >= edgeEnd.x)
			return(true);
		}

	return(false);
}





//============================================================================
//		NGeometryUtilities::GetIntersection : Get the intersection with an edge.
//----------------------------------------------------------------------------
template<class T> NPointT<T> NGeometryUtilities::GetIntersection(const NPointT<T> &edgeStart, const NPointT<T> &edgeEnd, const NPointT<T> &lineStart, const NPointT<T> &lineEnd)
{	NPointT<T>	thePoint;



	// Calculate the intersectin point
	if (NMathUtilities::AreEqual(edgeStart.y, edgeEnd.y))
		{
		thePoint.y = edgeStart.y;
		thePoint.x = lineStart.x + ((edgeStart.y-lineStart.y) * ((lineEnd.x-lineStart.x) / (lineEnd.y-lineStart.y)));
		}
	else
		{
		thePoint.x = edgeStart.x;
		thePoint.y = lineStart.y + ((edgeStart.x-lineStart.x) * ((lineEnd.y-lineStart.y) / (lineEnd.x-lineStart.x)));
		}
	
	return(thePoint);
}





#endif // NGEOMETRYUTILITIES_CPP



