/*	NAME:
		NCoreGraphics.h

	DESCRIPTION:
		CoreGraphics support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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



// Cocoa to CoreGraphics
#if defined(__OBJC__)
inline CGPoint ToCG(const NSPoint &thePoint)
{
	return(CGPointMake(thePoint.x, thePoint.y));
}

inline CGSize ToCG(const NSSize &theSize)
{
	return(CGSizeMake(theSize.width, theSize.height));
}

inline CGRect ToCG(const NSRect &theRect)
{
	return(CGRectMake(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}
#endif



// CoreGraphics to Nano
inline NPoint ToNN(const CGPoint &thePoint)
{
	return(NPoint(thePoint.x, thePoint.y));
}

inline NSize ToNN(const CGSize &theSize)
{
	return(NSize(theSize.width, theSize.height));
}

inline NRectangle ToNN(const CGRect &theRect)
{
	return(NRectangle(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}



// CoreGraphics to Cocoa
#if defined(__OBJC__)
inline NSPoint ToNS(const CGPoint &thePoint)
{
	return(NSMakePoint(thePoint.x, thePoint.y));
}

inline NSSize ToNS(const CGSize &theSize)
{
	return(NSMakeSize(theSize.width, theSize.height));
}

inline NSRect ToNS(const CGRect &theRect)
{
	return(NSMakeRect(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}
#endif



// Equality operators
inline bool operator==(const CGPoint &value1, const CGPoint &value2)
{
	return(	NMathUtilities::AreEqual(value1.x, value2.x) &&
			NMathUtilities::AreEqual(value1.y, value2.y));
}

inline bool operator==(const CGSize &value1, const CGSize &value2)
{
	return(	NMathUtilities::AreEqual(value1.width,  value2.width) &&
			NMathUtilities::AreEqual(value1.height, value2.height));
}

inline bool operator==(const CGRect &value1, const CGRect &value2)
{
	return(value1.origin == value2.origin && value1.size == value2.size);
}



// Inequality operators
inline bool operator!=(const CGPoint &value1, const CGPoint &value2)
{
	return(	!NMathUtilities::AreEqual(value1.x, value2.x) ||
			!NMathUtilities::AreEqual(value1.y, value2.y));
}

inline bool operator!=(const CGSize &value1, const CGSize &value2)
{
	return(	!NMathUtilities::AreEqual(value1.width,  value2.width) ||
			!NMathUtilities::AreEqual(value1.height, value2.height));
}

inline bool operator!=(const CGRect &value1, const CGRect &value2)
{
	return(value1.origin != value2.origin || value1.size != value2.size);
}





#endif // NCOREGRAPHICS_HDR





