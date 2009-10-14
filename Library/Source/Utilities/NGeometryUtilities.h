/*	NAME:
		NGeometryUtilities.h

	DESCRIPTION:
		Geometry utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NGEOMETRYUTILITIES_HDR
#define NGEOMETRYUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NString.h"
#include "NMathUtilities.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Zero items
static const HISize  kHISizeZero							= { 0.0f, 0.0f };
static const HIPoint kHIPointZero							= { 0.0f, 0.0f };
static const HIRect  kHIRectZero							= { kHIPointZero, kHISizeZero };

static const Point kQDPointZero								= { 0, 0 };
static const Rect  kQDRectZero								= { 0, 0, 0, 0 };


// Positions
typedef enum {
	kPositionAlert,
	kPositionCenter,
	kPositionTopLeft,
	kPositionTopRight,
	kPositionBottomLeft,
	kPositionBottomRight,
} NPosition;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Geometric types
typedef Point		QDPoint;
typedef Rect		QDRect;

typedef CGPoint		CGPoint32;
typedef CGSize		CGSize32;
typedef CGRect		CGRect32;

typedef struct {
	Float64			x;
	Float64			y;
} CGPoint64;

typedef struct {
	Float64			width;
	Float64			height;
} CGSize64;

typedef struct {
	CGPoint64		origin;
	CGSize64		size;
} CGRect64;


// Lists
typedef std::vector<CGPoint>								CGPointList;
typedef CGPointList::iterator								CGPointListIterator;
typedef CGPointList::const_iterator							CGPointListConstIterator;

typedef std::vector<CGSize>									CGSizeList;
typedef CGSizeList::iterator								CGSizeListIterator;
typedef CGSizeList::const_iterator							CGSizeListConstIterator;

typedef std::vector<CGRect>									CGRectList;
typedef CGRectList::iterator								CGRectListIterator;
typedef CGRectList::const_iterator							CGRectListConstIterator;


typedef std::vector<CGPoint32>								CGPoint32List;
typedef CGPoint32List::iterator								CGPoint32ListIterator;
typedef CGPoint32List::const_iterator						CGPoint32ListConstIterator;

typedef std::vector<CGSize32>								CGSize32List;
typedef CGSize32List::iterator								CGSize32ListIterator;
typedef CGSize32List::const_iterator						CGSize32ListConstIterator;

typedef std::vector<CGRect32>								CGRect32List;
typedef CGRect32List::iterator								CGRect32ListIterator;
typedef CGRect32List::const_iterator						CGRect32ListConstIterator;


typedef std::vector<CGPoint64>								CGPoint64List;
typedef CGPoint64List::iterator								CGPoint64ListIterator;
typedef CGPoint64List::const_iterator						CGPoint64ListConstIterator;

typedef std::vector<CGSize64>								CGSize64List;
typedef CGSize64List::iterator								CGSize64ListIterator;
typedef CGSize64List::const_iterator						CGSize64ListConstIterator;

typedef std::vector<CGRect64>								CGRect64List;
typedef CGRect64List::iterator								CGRect64ListIterator;
typedef CGRect64List::const_iterator						CGRect64ListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NGeometryUtilities {
public:
	// Convert points
	static void							ConvertPoint(const CGPoint &inPoint, QDPoint &outPoint);
	static void							ConvertPoint(const QDPoint &inPoint, CGPoint &outPoint);


	// Convert rectangles
	static void							ConvertRect(const CGRect &inRect, QDRect &outRect);
	static void							ConvertRect(const QDRect &inRect, CGRect &outRect);
	static void							ConvertRect(const PMRect &inRect, CGRect &outRect);


	// Scale rectangles
	static void							ScaleRect( CGRect &theRect, float scaleBy);
	static void							ScaleToFit(CGRect &theRect, const CGRect &fitRect);


	// Union a rectangle
	static CGRect						UnionRect(const CGRect32 &rect1, const CGRect &rect2);


	// Get the center point
	static CGPoint						GetCenter(const CGRect &theRect);
	static CGPoint						GetCenter(const CGSize &theSize);


	// Position a rectangle
	static void							PositionRect(HIRect &theRect, const HIRect &placeIn, NPosition thePosition);


	// Transform geometry
	static HIPoint						Transform(const HIPoint			&inPoint,
													HICoordinateSpace	 inSpace   = kHICoordSpace72DPIGlobal,
													void				*inObject  = NULL,
													HICoordinateSpace	 outSpace  = kHICoordSpaceScreenPixel,
													void				*outObject = NULL);

	static HISize						Transform(const HISize			&inSize,
													HICoordinateSpace	 inSpace   = kHICoordSpace72DPIGlobal,
													void				*inObject  = NULL,
													HICoordinateSpace	 outSpace  = kHICoordSpaceScreenPixel,
													void				*outObject = NULL);

	static HIRect						Transform(const HIRect			&inRect,
													HICoordinateSpace	 inSpace   = kHICoordSpace72DPIGlobal,
													void				*inObject  = NULL,
													HICoordinateSpace	 outSpace  = kHICoordSpaceScreenPixel,
													void				*outObject = NULL);


	// Get a bounding box
	static CGRect32						GetBounds(const CGPoint32List &thePoints);
	static CGRect64						GetBounds(const CGPoint64List &thePoints);


	// Get the length of a line
	static Float32						GetLength(const CGPoint32List &theLine);
	static Float64						GetLength(const CGPoint64List &theLine);


	// Clip a polygon to a rectangle
	static CGPoint32List				ClipPolygon(const CGPoint32List &thePoly, const CGRect32 &theRect);
	static CGPoint64List				ClipPolygon(const CGPoint64List &thePoly, const CGRect64 &theRect);


	// Test a point against a polygon
	static bool							PointInPolygon(const CGPoint32 &thePoint, const CGPoint32List &thePoly);
	static bool							PointInPolygon(const CGPoint64 &thePoint, const CGPoint64List &thePoly);

	static bool							PointInPolygon(const CGPoint32 &thePoint, UInt32 numPoints, const CGPoint32 *thePoints);
	static bool							PointInPolygon(const CGPoint64 &thePoint, UInt32 numPoints, const CGPoint64 *thePoints);


	// Test a point against a rectangle
	static bool							PointInRect(const CGPoint32 &thePoint, const CGRect32 &theRect);
	static bool							PointInRect(const CGPoint64 &thePoint, const CGRect64 &theRect);


private:
	template <class P, class R> static R	GetBounds(  const P &thePoints);
	template <class P, class F> static F	GetLength(  const P &theLine);

	template <class P, class R>  static P		ClipPolygon(const P &thePoly, const R &theRect);
	template <class P, class PP> static void	ClipToEdge(     const PP &edgeStart, const PP &edgeEnd, const P &thePoly, P &theResult);
	template <class PP>          static bool	IsInside(       const PP &edgeStart, const PP &edgeEnd, const PP &thePoint);
	template <class PP>          static PP		GetIntersection(const PP &edgeStart, const PP &edgeEnd, const PP &lineStart, const PP &lineEnd);

    template <class P, class F> static bool     PointInPolygon(const P &thePoint, UInt32 numPoints, const P *thePoints);
	template <class P, class R> static bool		PointInRect(   const P &thePoint, const R &theRect);
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
inline bool operator ==(const HIPoint &value1, const HIPoint &value2)
{
	return(NMathUtilities::AreEqual(value1.x, value2.x) &&
		   NMathUtilities::AreEqual(value1.y, value2.y));
}

inline bool operator !=(const HIPoint &value1, const HIPoint &value2)
{
	return(NMathUtilities::NotEqual(value1.x, value2.x) ||
		   NMathUtilities::NotEqual(value1.y, value2.y));
}

inline bool operator ==(const HISize &value1, const HISize &value2)
{
	return(NMathUtilities::AreEqual(value1.width,  value2.width) &&
		   NMathUtilities::AreEqual(value1.height, value2.height));
}

inline bool operator !=(const HISize &value1, const HISize &value2)
{
	return(NMathUtilities::NotEqual(value1.width,  value2.width) ||
		   NMathUtilities::NotEqual(value1.height, value2.height));
}

inline bool operator ==(const HIRect &value1, const HIRect &value2)
{
	return(value1.origin == value2.origin && value1.size == value2.size);
}

inline bool operator !=(const HIRect &value1, const HIRect &value2)
{
	return(value1.origin != value2.origin || value1.size != value2.size);
}





#endif // NGEOMETRYUTILITIES_HDR


