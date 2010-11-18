/*	NAME:
		NCoreGraphics.h

	DESCRIPTION:
		CoreGraphics support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
#if NN_TARGET_MAC && defined(__OBJC__)
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
#endif // NN_TARGET_MAC



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
#if NN_TARGET_MAC && defined(__OBJC__)
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
#endif // NN_TARGET_MAC



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





#endif // NCOREGRAPHICS_HDR





