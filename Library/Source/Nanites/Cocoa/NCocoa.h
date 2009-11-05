/*	NAME:
		NCocoa.h

	DESCRIPTION:
		Cocoa support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOCOA_HDR
#define NCOCOA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"
#include "NRange.h"





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Nano to Cocoa
#if NN_TARGET_MAC
inline NSPoint ToNS(const NPoint &thePoint)
{
	return(NSMakePoint(thePoint.x, thePoint.y));
}

inline NSSize ToNS(const NSize &theSize)
{
	return(NSMakeSize(theSize.width, theSize.height));
}

inline NSRect ToNS(const NRectangle &theRect)
{
	return(NSMakeRect(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}
#endif // NN_TARGET_MAC

inline NSRange ToNS(const NRange &theRange)
{
	return(NSMakeRange(theRange.GetLocation(), theRange.GetSize()));
}



// Cocoa to Nano
#if NN_TARGET_MAC
inline NPoint ToNN(const NSPoint &thePoint)
{
	return(NPoint(thePoint.x, thePoint.y));
}

inline NSize ToNN(const NSSize &theSize)
{
	return(NSize(theSize.width, theSize.height));
}

inline NRectangle ToNN(const NSRect &theRect)
{
	return(NRectangle(theRect.origin.x, theRect.origin.y, theRect.size.width, theRect.size.height));
}
#endif // NN_TARGET_MAC

inline NRange ToNN(const NSRange &theRange)
{
	return(NRange(theRange.location, theRange.length));
}



// Equality operators
inline bool operator==(const NSPoint &value1, const NSPoint &value2)
{
	return(	NMathUtilities::AreEqual(value1.x, value2.x) &&
			NMathUtilities::AreEqual(value1.y, value2.y));
}

inline bool operator==(const NSSize &value1, const NSSize &value2)
{
	return(	NMathUtilities::AreEqual(value1.width,  value2.width) &&
			NMathUtilities::AreEqual(value1.height, value2.height));
}

inline bool operator==(const NSRect &value1, const NSRect &value2)
{
	return(value1.origin == value2.origin && value1.size == value2.size);
}



// Inequality operators
inline bool operator!=(const NSPoint &value1, const NSPoint &value2)
{
	return(	!NMathUtilities::AreEqual(value1.x, value2.x) ||
			!NMathUtilities::AreEqual(value1.y, value2.y));
}

inline bool operator!=(const NSSize &value1, const NSSize &value2)
{
	return(	!NMathUtilities::AreEqual(value1.width,  value2.width) ||
			!NMathUtilities::AreEqual(value1.height, value2.height));
}

inline bool operator!=(const NSRect &value1, const NSRect &value2)
{
	return(value1.origin != value2.origin || value1.size != value2.size);
}





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// NSBundle
#define NSBundleString(_name)								NSLocalizedString(@_name, @"")
#define NSBundleImagePNG(_name)								[[NSImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@_name ofType:@"png"]]
#define NSBundleImageJPG(_name)								[[NSImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@_name ofType:@"jpg"]]





#endif // NCOCOA_HDR





