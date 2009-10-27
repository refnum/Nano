/*	NAME:
		NSkia.h

	DESCRIPTION:
		Skia support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSKIA_HDR
#define NSKIA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Convert to Skia
inline SkPoint ToSk(const NPoint &thePoint)
{	SkPoint		theResult;

	theResult.set(thePoint.x, thePoint.y);
	return(theResult);
}

inline SkSize ToSk(const NSize &theSize)
{	SkSize		theResult;

	theResult.set(thePoint.width, thePoint.height);
	return(theResult);
}

inline SkRect ToSk(const NRectangle &theRect)
{	SkRect		theResult;

	theResult.set(theRect.GetMinX(), theRect.GetMinY(), theRect.GetMaxX(), theRect.GetMaxY());
	return(theResult);
}



// Convert from Skia
inline NPoint ToNN(const SkPoint &thePoint)
{
	return(NPoint(thePoint.x, thePoint.y));
}

inline NSize ToNN(const SkSize &theSize)
{
	return(NSize(theSize.width, theSize.height));
}

inline NRectangle ToNN(const SkRect &theRect)
{
	return(NRectangle(theRect.fLeft, theRect.fTop, theRect.width(), theRect.height()));
}





#endif // NSKIA_HDR





