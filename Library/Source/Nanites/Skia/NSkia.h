/*	NAME:
		NSkia.h

	DESCRIPTION:
		Skia support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSKIA_HDR
#define NSKIA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#if NN_TARGET_MACOS
	#pragma GCC diagnostic ignored "-Wshadow"
#endif

#include "SkBitmap.h"
#include "SkBlurDrawLooper.h"
#include "SkCanvas.h"
#include "SkColor.h"
#include "SkColorPriv.h"
#include "SkDashPathEffect.h"
#include "SkGeometry.h"
#include "SkGradientShader.h"
#include "SkImageDecoder.h"
#include "SkImageEncoder.h"
#include "SkLineClipper.h"
#include "SkMatrix.h"
#include "SkPaint.h"
#include "SkPath.h"
#include "SkPoint.h"
#include "SkRect.h"
#include "SkSize.h"
#include "SkStream.h"
#include "SkStroke.h"
#include "SkTypeface.h"

#if NN_TARGET_MACOS
	#pragma GCC diagnostic warning "-Wshadow"
#endif

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

inline SkIPoint ToSkI(const NPoint &thePoint)
{	SkIPoint		theResult;

	theResult.set((int) thePoint.x, (int) thePoint.y);
	return(theResult);
}

inline SkISize ToSkI(const NSize &theSize)
{	SkISize		theResult;

	theResult.set((int) theSize.width, (int) theSize.height);
	return(theResult);
}

inline SkIRect ToSkI(const NRectangle &theRect)
{	SkIRect		theResult;

	theResult.set((int) theRect.GetMinX(), (int) theRect.GetMinY(), (int) theRect.GetMaxX(), (int) theRect.GetMaxY());
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

inline NPoint ToNN(const SkIPoint &thePoint)
{
	return(NPoint((float32_t) thePoint.fX, (float32_t) thePoint.fY));
}

inline NSize ToNN(const SkISize &theSize)
{
	return(NSize((float32_t) theSize.fWidth, (float32_t) theSize.fHeight));
}

inline NRectangle ToNN(const SkIRect &theRect)
{
	return(NRectangle((float32_t) theRect.fLeft, (float32_t) theRect.fTop, (float32_t) theRect.width(), (float32_t) theRect.height()));
}

inline NColor ToNN(SkColor theColor)
{
	return(NColor(theColor, kNColorARGB));
}





#endif // NSKIA_HDR





