/*	NAME:
		NCoreGraphics.h

	DESCRIPTION:
		CoreGraphics support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOREGRAPHICS_HDR
#define NCOREGRAPHICS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef std::vector<CGFloat>										CGFloatList;
typedef CGFloatList::iterator										CGFloatListIterator;
typedef CGFloatList::const_iterator									CGFloatListConstIterator;

typedef std::vector<CGPoint>										CGPointList;
typedef CGPointList::iterator										CGPointListIterator;
typedef CGPointList::const_iterator									CGPointListConstIterator;





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Nano to CoreGraphics
inline CGPoint ToCG(const NPoint &thePoint)
{
	return(CGPointMake(thePoint.x, thePoint.y));
}

inline CGSize ToCG(const NSize &theSize)
{
	return(CGSizeMake(theSize.width, theSize.height));
}

inline CGRect ToCG(const NRectangle &theRect)
{
	return(CGRectMake(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}



// CoreGraphics to Nano
inline NPoint ToNN(const CGPoint &thePoint)
{
	return(NPoint(	(Float32) thePoint.x,
					(Float32) thePoint.y));
}

inline NSize ToNN(const CGSize &theSize)
{
	return(NSize(	(Float32) theSize.width,
					(Float32) theSize.height));
}

inline NRectangle ToNN(const CGRect &theRect)
{
	return(NRectangle(	(Float32) theRect.origin.x,
						(Float32) theRect.origin.y,
						(Float32) theRect.size.width,
						(Float32) theRect.size.height));
}



// Equality operators
inline bool operator==(const CGPoint &value1, const CGPoint &value2)
{
	return(CGPointEqualToPoint(value1, value2));
}

inline bool operator==(const CGSize &value1, const CGSize &value2)
{
	return(CGSizeEqualToSize(value1, value2));
}

inline bool operator==(const CGRect &value1, const CGRect &value2)
{
	return(CGRectEqualToRect(value1, value2));
}



// Inequality operators
inline bool operator!=(const CGPoint &value1, const CGPoint &value2)
{
	return(!CGPointEqualToPoint(value1, value2));
}

inline bool operator!=(const CGSize &value1, const CGSize &value2)
{
	return(!CGSizeEqualToSize(value1, value2));
}

inline bool operator!=(const CGRect &value1, const CGRect &value2)
{
	return(!CGRectEqualToRect(value1, value2));
}



// CoreGraphics constructors
inline CGRect CGRectMake(const CGSize &theSize)
{
	return(CGRectMake(0.0, 0.0, theSize.width, theSize.height));
}

inline CGRect CGRectMake(CGFloat theWidth, CGFloat theHeight)
{
	return(CGRectMake(0.0, 0.0, theWidth, theHeight));
}







#endif // NCOREGRAPHICS_HDR





