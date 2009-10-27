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
// Convert to CoreGraphics
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



// Convert from CoreGraphics
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





#endif // NCOREGRAPHICS_HDR





