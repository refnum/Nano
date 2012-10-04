/*  NAME:
        NGeometryUtilities.h

    DESCRIPTION:
        Geometry utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2010, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
#ifndef NGEOMETRYUTILITIES_HDR
#define NGEOMETRYUTILITIES_HDR
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NRectangle.h"
#include "NShape.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
typedef enum {
	kNGeometryInside,
	kNGeometryOutside,
	kNGeometryIntersects,
} NGeometryComparison;





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NGeometryUtilities {
public:
	// Get the length of a line
	template<class T>
	static T							GetLength(const std::vector< NPointT<T> > &theLine, bool getApprox=true);


	// Get a bounding box
	template<class T>
	static NRectangleT<T>				GetBounds(const std::vector< NPointT<T> > &thePoints);


	// Get a point on a line
	//
	// Returns the point at the specified offset within the line, where 0.0
	// is the first point in the line and 1.0 the last point in the line.
	template<class T>
	static NPointT<T>					GetPointOnLine(const std::vector< NPointT<T> > &thePoints, T theOffset);


	// Get the closest point to a line
	template<class T>
	static NPointT<T>					GetClosestPointOnLine(const NPointT<T> &thePoint, const NPointT<T> &lineStart, const NPointT<T> &lineEnd, T *lineU=NULL);


	// Compare a rectangle to a shape
	template<class T>
	static NGeometryComparison			CompareRectangleToShape(const NRectangleT<T> &theRectangle, const NShapeT<T> &theShape);


	// Compare a rectangle to a polygon
	template<class T>
	static NGeometryComparison			CompareRectangleToPolygon(const NRectangleT<T> &theRect, NIndex numPoints, const NPointT<T> *thePoints);


	// Compare a point to a polygon
	template<class T>
	static NGeometryComparison			ComparePointToPolygon(const NPointT<T> &thePoint, NIndex numPoints, const NPointT<T> *thePoints);


	// Clip a line
	template<class T>
	static std::vector< NPointT<T> >	ClipLine(const std::vector< NPointT<T> > &theLine, const NRectangleT<T> &theRectangle);


private:
	template<class T>
	static NGeometryComparison			CompareLineToRectangle( const NRectangleT<T> &theRect, const NPointT<T> &p0, const NPointT<T> &p1);

	template<class T>
	static NBitfield					GetClipCode(const NRectangleT<T> &theRect, const NPointT<T> &thePoint);

	template<class T>
	static void							ClipToEdge(const NPointT<T> &edgeStart, const NPointT<T> &edgeEnd, const std::vector< NPointT<T> > &theLine, NPointT<T> &theResult);

	template<class T>
	static bool							IsInside(const NPointT<T> &edgeStart, const NPointT<T> &edgeEnd, const NPointT<T> &thePoint);
	
	template<class T>
	static NPointT<T>					GetIntersection(const NPointT<T> &edgeStart, const NPointT<T> &edgeEnd, const NPointT<T> &lineStart, const NPointT<T> &lineEnd);
};





//============================================================================
//		Template files
//----------------------------------------------------------------------------
#define   NGEOMETRYUTILITIES_CPP
#include "NGeometryUtilities.cpp"
#undef    NGEOMETRYUTILITIES_CPP




#endif // NGEOMETRYUTILITIES_HDR


