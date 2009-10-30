/*	NAME:
		NGeometryUtilities.cpp

	DESCRIPTION:
		Geometry utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NGeometryUtilities.h"





//============================================================================
//		NGeometryUtilities::ConvertPoint : Convert a point.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertPoint(const CGPoint &inPoint, QDPoint &outPoint)
{


	// Validate our parameters
	NN_ASSERT(inPoint.x > SHRT_MIN && inPoint.x <= SHRT_MAX);
	NN_ASSERT(inPoint.y > SHRT_MIN && inPoint.y <= SHRT_MAX);



	// Convert the point
	outPoint.h = (SInt16) inPoint.x;
	outPoint.v = (SInt16) inPoint.y;
}





//============================================================================
//		NGeometryUtilities::ConvertPoint : Convert a point.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertPoint(const QDPoint &inPoint, CGPoint &outPoint)
{


	// Convert the point
	outPoint.x = (float) inPoint.h;
	outPoint.y = (float) inPoint.v;
}





//============================================================================
//		NGeometryUtilities::ConvertRect : Convert a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertRect(const CGRect &inRect, QDRect &outRect)
{


	// Validate our parameters
	NN_ASSERT(inRect.origin.x > SHRT_MIN && (inRect.origin.x + inRect.size.width)  <= SHRT_MAX);
	NN_ASSERT(inRect.origin.y > SHRT_MIN && (inRect.origin.y + inRect.size.height) <= SHRT_MAX);



	// Convert the rectangle
	outRect.left   = (SInt16)  inRect.origin.x;
	outRect.top    = (SInt16)  inRect.origin.y;
	outRect.right  = (SInt16) (inRect.origin.x + inRect.size.width);
	outRect.bottom = (SInt16) (inRect.origin.y + inRect.size.height);
}





//============================================================================
//		NGeometryUtilities::ConvertRect : Convert a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertRect(const QDRect &inRect, CGRect &outRect)
{


	// Convert the rectangle
	outRect.origin.x    = (float)  inRect.left;
	outRect.origin.y    = (float)  inRect.top;
	outRect.size.width  = (float) (inRect.right  - inRect.left);
	outRect.size.height = (float) (inRect.bottom - inRect.top);
}





//============================================================================
//		NGeometryUtilities::ConvertRect : Convert a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::ConvertRect(const PMRect &inRect, CGRect &outRect)
{


	// Convert the rectangle
	outRect.origin.x    = (float) inRect.left;
	outRect.origin.y    = (float) inRect.top;
	outRect.size.width  = (float) (inRect.right  - inRect.left);
	outRect.size.height = (float) (inRect.bottom - inRect.top);
}





//============================================================================
//		NGeometryUtilities::ScaleRect : Scale a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::ScaleRect(CGRect &theRect, float scaleBy)
{	HIPoint		theCentre;



	// Scale the rectangle
	theCentre = GetCenter(theRect);
	
	theRect.size.width  *= scaleBy;
	theRect.size.height *= scaleBy;

	theRect.origin.x = theCentre.x - (theRect.size.width  / 2.0f);
	theRect.origin.y = theCentre.y - (theRect.size.height / 2.0f);
}





//============================================================================
//		NGeometryUtilities::ScaleToFit : Scale a rectangle to fit.
//----------------------------------------------------------------------------
void NGeometryUtilities::ScaleToFit(CGRect &theRect, const CGRect &fitRect)
{	float		aspectRatio;
	CGSize		theSize;



	// Calculate the new size
	//
	// We expand the rectangle as much as possible, up to the size of fitRect.
	aspectRatio = theRect.size.width / theRect.size.height;

	theSize.width  = fitRect.size.width;
	theSize.height = fitRect.size.width / aspectRatio;

	if (theSize.height > fitRect.size.height)
		{
		theSize.width  = fitRect.size.height * aspectRatio;
		theSize.height = fitRect.size.height;
		}



	// Position the rectangle
	theRect.size     = theSize;
	theRect.origin.x = fitRect.origin.x + ((fitRect.size.width  - theSize.width)  / 2.0f);
	theRect.origin.y = fitRect.origin.y + ((fitRect.size.height - theSize.height) / 2.0f);
}





//============================================================================
//		NGeometryUtilities::UnionRect : Union two rectangles.
//----------------------------------------------------------------------------
CGRect NGeometryUtilities::UnionRect(const CGRect &rect1, const CGRect &rect2)
{


	// Calculate the union
	//
	// If CGRectUnion is passed an empty rectangle, it calculates the union between
	// the origin of the empty rectangle and the bounds of the non-empty rectangle.
	//
	// This method ignores empty rectangles, allowing it to be used to union a list
	// of rectangles with an initially empty rectangle to find the overall bounds.
	if (CGRectIsEmpty(rect1))
		return(rect2);

	if (CGRectIsEmpty(rect2))
		return(rect1);

	return(CGRectUnion(rect1, rect2));
}





//============================================================================
//		NGeometryUtilities::GetCenter : Get the center point.
//----------------------------------------------------------------------------
CGPoint NGeometryUtilities::GetCenter(const CGRect &theRect)
{	CGPoint		thePoint;



	// Get the center point
	thePoint.x = theRect.origin.x + (theRect.size.width  / 2.0f);
	thePoint.y = theRect.origin.y + (theRect.size.height / 2.0f);

	return(thePoint);
}





//============================================================================
//		NGeometryUtilities::GetCenter : Get the center point.
//----------------------------------------------------------------------------
CGPoint NGeometryUtilities::GetCenter(const CGSize &theSize)
{


	// Get the center point
	return(GetCenter(CGRectMake(0.0f, 0.0f, theSize.width, theSize.height)));
}





//============================================================================
//		NGeometryUtilities::PositionRect : Position a rectangle.
//----------------------------------------------------------------------------
void NGeometryUtilities::PositionRect(HIRect &theRect, const HIRect &placeIn, NPosition thePosition)
{	float		minX, midX, maxX, minY, midY, maxY;
	float		halfWidth, halfHeight;



	// Get the state we need
	minX = placeIn.origin.x;
	maxX = placeIn.origin.x + placeIn.size.width;
	midX = minX + ((maxX - minX) / 2.0f);

	minY = placeIn.origin.y;
	maxY = placeIn.origin.y + placeIn.size.height;
	midY = minY + ((maxY - minY) / 2.0f);

	halfWidth  = theRect.size.width  / 2.0f;
	halfHeight = theRect.size.height / 2.0f;



	// Position the rectangle
	//
	// The alert position was defined by the original HIG as being a gap of
	// "20% of the available height" above the rectangle, subject to space.
	switch (thePosition) {
		case kPositionAlert:
			theRect.origin.x = midX - halfWidth;
			theRect.origin.y = minY;

			if (theRect.size.height < placeIn.size.height)
				theRect.origin.y +=  (placeIn.size.height * 0.2f);
			break;

		case kPositionCenter:
			theRect.origin.x = midX - halfWidth;
			theRect.origin.y = midY - halfHeight;
			break;

		case kPositionTopLeft:
			theRect.origin.x = minX;
			theRect.origin.y = minY;
			break;

		case kPositionTopRight:
			theRect.origin.x = maxX - theRect.size.width;
			theRect.origin.y = minY;
			break;

		case kPositionBottomLeft:
			theRect.origin.x = minX;
			theRect.origin.y = maxY - theRect.size.height;
			break;

		case kPositionBottomRight:
			theRect.origin.x = maxX - theRect.size.width;
			theRect.origin.y = maxY - theRect.size.height;
			break;

		default:
			NN_LOG("Unknown position: %d", thePosition);
			break;
		}
}





//============================================================================
//		NGeometryUtilities::Transform : Transform a point.
//----------------------------------------------------------------------------
HIPoint NGeometryUtilities::Transform(const HIPoint		  &inPoint,
										HICoordinateSpace  inSpace, void  *inObject,
										HICoordinateSpace outSpace, void *outObject)
{	HIPoint		thePoint;



	// Validate our parameters
	//
	// For now we just assume transforms are needed for scaling, but we should
	// support general conversions between views and windows on older systems.
	if (HIPointConvert == NULL)
		{
		NN_ASSERT( inSpace == kHICoordSpace72DPIGlobal ||  inSpace == kHICoordSpaceScreenPixel);
		NN_ASSERT(outSpace == kHICoordSpace72DPIGlobal || outSpace == kHICoordSpaceScreenPixel);
		}



	// Transform the geometry
	//
	// Resolution-independence is only available from Mac OS X 10.4 onwards,
	// so for older systems we can assume that the scaling is always 1:1.
	thePoint = inPoint;
	
	if (HIPointConvert != NULL)
		HIPointConvert(&thePoint, inSpace, inObject, outSpace, outObject);
	
	return(thePoint);
}





//============================================================================
//		NGeometryUtilities::Transform : Transform a size.
//----------------------------------------------------------------------------
HISize NGeometryUtilities::Transform(const HISize		  &inSize,
										HICoordinateSpace  inSpace, void  *inObject,
										HICoordinateSpace outSpace, void *outObject)
{	HISize		theSize;



	// Validate our parameters
	//
	// For now we just assume transforms are needed for scaling, but we should
	// support general conversions between views and windows on older systems.
	if (HISizeConvert == NULL)
		{
		NN_ASSERT( inSpace == kHICoordSpace72DPIGlobal ||  inSpace == kHICoordSpaceScreenPixel);
		NN_ASSERT(outSpace == kHICoordSpace72DPIGlobal || outSpace == kHICoordSpaceScreenPixel);
		}



	// Transform the geometry
	//
	// Resolution-independence is only available from Mac OS X 10.4 onwards,
	// so for older systems we can assume that the scaling is always 1:1.
	theSize = inSize;
	
	if (HISizeConvert != NULL)
		HISizeConvert(&theSize, inSpace, inObject, outSpace, outObject);
	
	return(theSize);
}





//============================================================================
//		NGeometryUtilities::Transform : Transform a rectangle.
//----------------------------------------------------------------------------
HIRect NGeometryUtilities::Transform(const HIRect		  &inRect,
										HICoordinateSpace  inSpace, void  *inObject,
										HICoordinateSpace outSpace, void *outObject)
{	HIRect		theRect;



	// Validate our parameters
	//
	// For now we just assume transforms are needed for scaling, but we should
	// support general conversions between views and windows on older systems.
	if (HIRectConvert == NULL)
		{
		NN_ASSERT( inSpace == kHICoordSpace72DPIGlobal ||  inSpace == kHICoordSpaceScreenPixel);
		NN_ASSERT(outSpace == kHICoordSpace72DPIGlobal || outSpace == kHICoordSpaceScreenPixel);
		}



	// Transform the geometry
	//
	// Resolution-independence is only available from Mac OS X 10.4 onwards,
	// so for older systems we can assume that the scaling is always 1:1.
	theRect = inRect;
	
	if (HIRectConvert != NULL)
		HIRectConvert(&theRect, inSpace, inObject, outSpace, outObject);
	
	return(theRect);
}





//============================================================================
//		NGeometryUtilities::GetBounds : Get a bounding box.
//----------------------------------------------------------------------------
CGRect32 NGeometryUtilities::GetBounds(const CGPoint32List &thePoints)
{


	// Get the bounding box
	return(GetBounds<CGPoint32List, CGRect32, Float32>(thePoints));
}





//============================================================================
//		NGeometryUtilities::GetBounds : Get a bounding box.
//----------------------------------------------------------------------------
CGRect64 NGeometryUtilities::GetBounds(const CGPoint64List &thePoints)
{


	// Get the bounding box
	return(GetBounds<CGPoint64List, CGRect64, Float64>(thePoints));
}





//============================================================================
//		NGeometryUtilities::GetLength : Get the length of a line.
//----------------------------------------------------------------------------
Float32 NGeometryUtilities::GetLength(const CGPoint32List &theLine)
{


	// Get the length
	return(GetLength<CGPoint32List, Float32>(theLine));
}





//============================================================================
//		NGeometryUtilities::GetLength : Get the length of a line.
//----------------------------------------------------------------------------
Float64 NGeometryUtilities::GetLength(const CGPoint64List &theLine)
{


	// Get the length
	return(GetLength<CGPoint64List, Float64>(theLine));
}





//============================================================================
//		NGeometryUtilities::ClipPolygon : Clip a polygon to a rectangle.
//----------------------------------------------------------------------------
CGPoint32List NGeometryUtilities::ClipPolygon(const CGPoint32List &thePoly, const CGRect32 &theRect)
{


	// Clip the polygon
	return(ClipPolygon<CGPoint32List, CGRect32>(thePoly, theRect));
}





//============================================================================
//		NGeometryUtilities::ClipPolygon : Clip a polygon to a rectangle.
//----------------------------------------------------------------------------
CGPoint64List NGeometryUtilities::ClipPolygon(const CGPoint64List &thePoly, const CGRect64 &theRect)
{


	// Get the length
	return(ClipPolygon<CGPoint64List, CGRect64>(thePoly, theRect));
}





//============================================================================
//		NGeometryUtilities::PointInPolygon : Test a point against a polygon.
//----------------------------------------------------------------------------
bool NGeometryUtilities::PointInPolygon(const CGPoint32 &thePoint, const CGPoint32List &thePoly)
{


	// Test the polygon
	return(PointInPolygon(thePoint, thePoly.size(), &thePoly[0]));
}





//============================================================================
//		NGeometryUtilities::PointInPolygon : Test a point against a polygon.
//----------------------------------------------------------------------------
bool NGeometryUtilities::PointInPolygon(const CGPoint64 &thePoint, const CGPoint64List &thePoly)
{


	// Test the polygon
	return(PointInPolygon(thePoint, thePoly.size(), &thePoly[0]));
}





//============================================================================
//		NGeometryUtilities::PointInPolygon : Test a point against a polygon.
//----------------------------------------------------------------------------
bool NGeometryUtilities::PointInPolygon(const CGPoint32 &thePoint, UInt32 numPoints, const CGPoint32 *thePoints)
{


	// Test the polygon
    return(PointInPolygon<CGPoint32, Float32>(thePoint, numPoints, thePoints));
}





//============================================================================
//		NGeometryUtilities::PointInPolygon : Test a point against a polygon.
//----------------------------------------------------------------------------
bool NGeometryUtilities::PointInPolygon(const CGPoint64 &thePoint, UInt32 numPoints, const CGPoint64 *thePoints)
{


	// Test the polygon
    return(PointInPolygon<CGPoint64, Float64>(thePoint, numPoints, thePoints));
}





//============================================================================
//		NGeometryUtilities::PointInRect : Test a point against a rectangle.
//----------------------------------------------------------------------------
bool NGeometryUtilities::PointInRect(const CGPoint32 &thePoint, const CGRect32 &theRect)
{


	// Test the rectangle
	return(PointInRect<CGPoint32, CGRect32>(thePoint, theRect));
}





//============================================================================
//		NGeometryUtilities::PointInRect : Test a point against a rectangle.
//----------------------------------------------------------------------------
bool NGeometryUtilities::PointInRect(const CGPoint64 &thePoint, const CGRect64 &theRect)
{


	// Test the rectangle
	return(PointInRect<CGPoint64, CGRect64>(thePoint, theRect));
}





//============================================================================
//		NGeometryUtilities::GetBounds : Get a bounding box.
//----------------------------------------------------------------------------
#pragma mark -
template <class P, class R, class F> R NGeometryUtilities::GetBounds(const P &thePoints)
{	Float64		minX, minY, maxX, maxY;
	UInt32		n, numPoints;
	R			theRect;



	// Find the min/max extents
	numPoints = thePoints.size();

	minX = maxX = (numPoints != 0) ? thePoints[0].x : 0.0;
	minY = maxY = (numPoints != 0) ? thePoints[0].y : 0.0;

	for (n = 0; n < numPoints; n++)
		{
		minX = std::min(minX, (Float64) thePoints[n].x);
		minY = std::min(minY, (Float64) thePoints[n].y);

		maxX = std::max(maxX, (Float64) thePoints[n].x);
		maxY = std::max(maxY, (Float64) thePoints[n].y);
		}



	// Get the bounding box
	theRect.origin.x    = (F)  minX;
	theRect.origin.y    = (F)  minY;
	theRect.size.width  = (F) (maxX - minX);
	theRect.size.height = (F) (maxY - minY);
	
	return(theRect);
}





//============================================================================
//		NGeometryUtilities::GetLength : Get the length of a line.
//----------------------------------------------------------------------------
template <class P, class F> F NGeometryUtilities::GetLength(const P &theLine)
{	F			theLength, dx, dy;
	UInt32		n, numPoints;



	// Measure the line
	theLength = (F) 0.0;
	numPoints = theLine.size();

	if (numPoints >= 2)
		{
		for (n = 0; n < (numPoints-1); n++)
			{
			dx = (theLine[n+1].x - theLine[n].x);
			dy = (theLine[n+1].y - theLine[n].y);
			
			theLength += (F) sqrt((dx * dx) + (dy * dy));
			}
		}
	
	return(theLength);
}





//============================================================================
//		NGeometryUtilities::ClipPolygon : Clip a polygon to a rectangle.
//----------------------------------------------------------------------------
template <class P, class R> P NGeometryUtilities::ClipPolygon(const P &thePoly, const R &theRect)
{	typedef		typename P::value_type	PPoint;

	PPoint		rectTL, rectTR, rectBL, rectBR;
	P			tmpPoly1, tmpPoly2;



	// Find the corners
	rectTL.x = theRect.origin.x;
	rectTL.y = theRect.origin.y;

	rectTR.x = theRect.origin.x + theRect.size.width;
	rectTR.y = theRect.origin.y;

	rectBL.x = theRect.origin.x;
	rectBL.y = theRect.origin.y + theRect.size.height;

	rectBR.x = theRect.origin.x + theRect.size.width;
	rectBR.y = theRect.origin.y + theRect.size.height;



	// Clip the polygon against each edge
	ClipToEdge<P, PPoint>(rectTL, rectBL, thePoly,  tmpPoly1);
	ClipToEdge<P, PPoint>(rectBL, rectBR, tmpPoly1, tmpPoly2);
	ClipToEdge<P, PPoint>(rectBR, rectTR, tmpPoly2, tmpPoly1);
	ClipToEdge<P, PPoint>(rectTR, rectTL, tmpPoly1, tmpPoly2);
	
	return(tmpPoly2);
}





//============================================================================
//		NGeometryUtilities::ClipToEdge : Clip a polygon to an edge.
//----------------------------------------------------------------------------
template <class P, class PP> void NGeometryUtilities::ClipToEdge(const PP &edgeStart, const PP &edgeEnd, const P &thePoly, P &theResult)
{	UInt32		n, numPoints;
	PP			s, p, i;



	// Get the state we need
	numPoints = thePoly.size();
	if (numPoints == 0)
		return;



	// Cip the polygon
	//
	// Each segment is compared to the clipping edge, which gives four cases:
	//
	//	1) Both endpoints (s and p) are inside the clipping edge, so p is added.
	//	2) Both endpoints (s and p) are outside the clipping edge, so nothing is added.
	//	3) Vertex s is inside and p is outside, so the intersection (i) is added.
	//	4) Vertex p is inside and s is outside, so the intersection (i) and p are added.
	//
	// Since most polygons end up about the same size, space is reserved prior to adding.
	theResult.clear();
	theResult.reserve(numPoints);

	s = thePoly[numPoints - 1];
	for (n = 0; n < numPoints; n++)
		{
		p = thePoly[n];


		// Cases 1 or 4
		if (IsInside<PP>(edgeStart, edgeEnd, p))
			{
			// Case 1
			if (IsInside<PP>(edgeStart, edgeEnd, s))
				theResult.push_back(p);
			
			// Case 4
			else
				{
				i = GetIntersection<PP>(edgeStart, edgeEnd, s, p);
				
				theResult.push_back(i);
				theResult.push_back(p);
				}
			}


		// Cases 2 or 3
		else
			{
			// Case 3
			if (IsInside<PP>(edgeStart, edgeEnd, s))
				{
				i = GetIntersection<PP>(edgeStart, edgeEnd, s, p);
				theResult.push_back(i);
				}
			
			
			// Case 2
			else
				;
			}
		
		s = p;
		}
}





//============================================================================
//		NGeometryUtilities::IsInside : Is a point inside an edge?
//----------------------------------------------------------------------------
template <class PP> bool NGeometryUtilities::IsInside(const PP &edgeStart, const PP &edgeEnd, const PP &thePoint)
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
template <class PP> PP NGeometryUtilities::GetIntersection(const PP &edgeStart, const PP &edgeEnd, const PP &lineStart, const PP &lineEnd)
{	PP	thePoint;



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





//============================================================================
//		NGeometryUtilities::PointInPolygon : Test a point against a polygon.
//----------------------------------------------------------------------------
template <class P, class F> bool NGeometryUtilities::PointInPolygon(const P &thePoint, UInt32 numPoints, const P *thePoints)
{	bool			p1Above, p2Above;
	SInt32			crossNum;
	P				p1, p2;
 	F				c, w;
    UInt32			n;



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
	
    return(NMathUtilities::IsOdd(crossNum));
}





//============================================================================
//		NGeometryUtilities::PointInRect : Test a point against a rectangle.
//----------------------------------------------------------------------------
template <class P, class R> bool NGeometryUtilities::PointInRect(const P &thePoint, const R &theRect)
{


	// Test the point
	if (thePoint.x < theRect.origin.x || thePoint.x > (theRect.origin.x + theRect.size.width) ||
		thePoint.y < theRect.origin.y || thePoint.y > (theRect.origin.y + theRect.size.height))
		return(false);
	
	return(true);
}




