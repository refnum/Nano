/*	NAME:
		NSkia.h

	DESCRIPTION:
		Skia support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSKIA_HDR
#define NSKIA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "SkPoint.h"
#include "SkSize.h"
#include "SkRect.h"
#include "SkColor.h"

#include "NPoint.h"
#include "NSize.h"
#include "NRectangle.h"
#include "NColor.h"





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

	theResult.set(theSize.width, theSize.height);
	return(theResult);
}

inline SkRect ToSk(const NRectangle &theRect)
{	SkRect		theResult;

	theResult.set(theRect.GetMinX(), theRect.GetMinY(), theRect.GetMaxX(), theRect.GetMaxY());
	return(theResult);
}

inline SkColor ToSk(const NColor &theColor)
{
	return(theColor.GetColor(kNColorARGB));
}



// Convert from Skia
inline NPoint ToNN(const SkPoint &thePoint)
{
	return(NPoint(thePoint.fX, thePoint.fY));
}

inline NSize ToNN(const SkSize &theSize)
{
	return(NSize(theSize.fWidth, theSize.fHeight));
}

inline NRectangle ToNN(const SkRect &theRect)
{
	return(NRectangle(theRect.fLeft, theRect.fTop, theRect.width(), theRect.height()));
}

inline NColor ToNN(SkColor theColor)
{
	return(NColor(theColor, kNColorARGB));
}





#endif // NSKIA_HDR





