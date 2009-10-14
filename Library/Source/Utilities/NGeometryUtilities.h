/*	NAME:
		NGeometryUtilities.h

	DESCRIPTION:
		Geometry utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
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
	kPositionCenter,
	kPositionAlert,
} NPosition;





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef std::vector<CGPoint>								CGPointList;
typedef CGPointList::iterator								CGPointConstIterator;
typedef CGPointList::const_iterator							CGPointListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NGeometryUtilities {
public:
	// Convert points
	static void							ConvertPoint(const HIPoint &inPoint,   Point &outPoint);
	static void							ConvertPoint(const   Point &inPoint, HIPoint &outPoint);


	// Convert rectangles
	static void							ConvertRect(const HIRect &inRect,   Rect &outRect);
	static void							ConvertRect(const   Rect &inRect, HIRect &outRect);
	static void							ConvertRect(const PMRect &inRect, HIRect &outRect);


	// Scale rectangles
	static void							ScaleRect( HIRect &theRect, float scaleBy);
	static void							ScaleToFit(HIRect &theRect, const HIRect &fitRect);


	// Get the center point
	static HIPoint						GetCenter(const HIRect &theRect);
	static HIPoint						GetCenter(const HISize &theSize);


	// Position a rectangle
	static void							PositionRect(HIRect &theRect, const HIRect &posRect, NPosition thePosition);


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


