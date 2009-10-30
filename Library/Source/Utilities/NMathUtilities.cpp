/*  NAME:
        NMathUtilities.cpp

    DESCRIPTION:
        Math utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#if TARGET_CPU_PPC
    #include <ppc_intrinsics.h>
#endif

#include "NGeometryUtilities.h"
#include "NMathUtilities.h"





//============================================================================
//      NMathUtilities::AreEqual : Are two floating point numbers equal?
//----------------------------------------------------------------------------
bool NMathUtilities::AreEqual(Float64 a, Float64 b)
{   Float64   theDelta;



    // Validate our parameters
    NN_ASSERT(!isnan(a));
    NN_ASSERT(!isnan(b));



    // Check for equality
    theDelta = fabs(a - b);
    
    return(theDelta < 0.000000001);
}





//============================================================================
//      NMathUtilities::NotEqual : Are two floating point numbers not equal?
//----------------------------------------------------------------------------
bool NMathUtilities::NotEqual(Float64 a, Float64 b)
{


    // Check for inequality
    return(!AreEqual(a, b));
}





//============================================================================
//      NMathUtilities::IsZero : Is a floating point number zero?
//----------------------------------------------------------------------------
bool NMathUtilities::IsZero(Float64 theValue)
{


    // Check for zero
    return(AreEqual(theValue, 0.0));
}





//============================================================================
//      NMathUtilities::NotZero : Is a floating point number non-zero?
//----------------------------------------------------------------------------
bool NMathUtilities::NotZero(Float64 theValue)
{


    // Check for zero
    return(!AreEqual(theValue, 0.0));
}





//============================================================================
//      NMathUtilities::IsOdd : Is a number odd?
//----------------------------------------------------------------------------
bool NMathUtilities::IsOdd(SInt64 theValue)
{


    // Check the value
    return((theValue & 0x01) != 0);
}





//============================================================================
//      NMathUtilities::IsEven : Is a number even?
//----------------------------------------------------------------------------
bool NMathUtilities::IsEven(SInt64 theValue)
{


    // Check the value
    return((theValue & 0x01) == 0);
}





//============================================================================
//      NMathUtilities::IsPowerOf2 : Is a number a power of 2?
//----------------------------------------------------------------------------
bool NMathUtilities::IsPowerOf2(SInt64 theValue)
{


    // Check the value
    return((theValue & (theValue - 1)) == 0);
}





//============================================================================
//      NMathUtilities::CountBits : Count the number of set bits.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::CountBits(UInt32 theValue)
{   UInt32  numBits;



    // Count the bits
    //
    // By subtracting 1, we can obtain a mask that removes the
    // highest set bit in the value. This ensures we only need
    // to loop as many times as there are set bits.
    numBits = 0;
    
    while (theValue != 0)
        {
        theValue = theValue & (theValue - 1);
        numBits++;
        }
    
    return(numBits);
}





//============================================================================
//      NMathUtilities::RotateLeft : Left-rotate.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::RotateLeft(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
#if TARGET_CPU_PPC
    return(__rlwnm(theValue, rotateBy, 0, 31));
#else
    return((theValue << rotateBy) | (theValue >> (32 - rotateBy)));
#endif
}





//============================================================================
//      NMathUtilities::RotateRight : Right-rotate.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::RotateRight(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
#if TARGET_CPU_PPC
    return(__rlwnm(theValue, 32 - rotateBy, 0, 31));
#else
    return((theValue >> rotateBy) | (theValue << (32 - rotateBy)));
#endif
}





//============================================================================
//      NMathUtilities::GetDegrees : Convert from radians to degrees.
//----------------------------------------------------------------------------
Float64 NMathUtilities::GetDegrees(Float64 radian)
{


    // Convert to degrees
    return(radian * (180.0 / M_PI));
}





//============================================================================
//      NMathUtilities::GetRadians : Convert from degrees to radians.
//----------------------------------------------------------------------------
Float64 NMathUtilities::GetRadians(Float64 degree)
{


    // Convert to radians
    return(degree * (M_PI / 180.0));
}





//============================================================================
//      NMathUtilities::GetAbsolute : Get an absolute value.
//----------------------------------------------------------------------------
Float32 NMathUtilities::GetAbsolute(Float32 theValue)
{


    // Get the value
#if TARGET_CPU_PPC
    return((Float32) __fabs((Float64) theValue));
#else
    return(fabsf(theValue));
#endif
}





//============================================================================
//      NMathUtilities::FastReciprocal : Get an approximate reciprocal.
//----------------------------------------------------------------------------
Float32 NMathUtilities::FastReciprocal(Float32 theValue)
{


    // Get the value
#if TARGET_CPU_PPC
    return(__fres(theValue));
#else
    return(1.0f / theValue);
#endif
}





//============================================================================
//      NMathUtilities::FastRoot : Get an approximate square root.
//----------------------------------------------------------------------------
Float32 NMathUtilities::FastRoot(Float32 theValue)
{


    // Get the value
    //
    // Could use __frsqrte on PowerPC.
    return(sqrtf(theValue));
}





//============================================================================
//      NMathUtilities::FastInvRoot : Get an approximate inverse square root.
//----------------------------------------------------------------------------
Float32 NMathUtilities::FastInvRoot(Float32 theValue)
{


    // Get the value
    //
    // Could use __frsqrte on PowerPC.
    return(FastReciprocal(FastRoot(theValue)));
}





//============================================================================
//      NMathUtilities::GetPointPointDistance : Get the distance between two points.
//----------------------------------------------------------------------------
Float32 NMathUtilities::GetPointPointDistance(const CGPoint32 &point1, const CGPoint32 &point2)
{


    // Get the distance
    return(GetPointPointDistance<CGPoint32, Float32>(point1, point2));
}





//============================================================================
//      NMathUtilities::GetPointPointDistance : Get the distance between two points.
//----------------------------------------------------------------------------
Float64 NMathUtilities::GetPointPointDistance(const CGPoint64 &point1, const CGPoint64 &point2)
{


    // Get the distance
    return(GetPointPointDistance<CGPoint64, Float64>(point1, point2));
}





//============================================================================
//      NMathUtilities::GetPointPointDistance2 : Get the squared distance between two points.
//----------------------------------------------------------------------------
Float32 NMathUtilities::GetPointPointDistance2(const CGPoint32 &point1, const CGPoint32 &point2)
{


    // Get the distance squared
    return(GetPointPointDistance2<CGPoint32, Float32>(point1, point2));
}





//============================================================================
//      NMathUtilities::GetPointPointDistance2 : Get the squared distance between two points.
//----------------------------------------------------------------------------
Float64 NMathUtilities::GetPointPointDistance2(const CGPoint64 &point1, const CGPoint64 &point2)
{


    // Get the distance squared
    return(GetPointPointDistance2<CGPoint64, Float64>(point1, point2));
}





//============================================================================
//      NMathUtilities::GetPointLineDistance : Get the distance between a point and a line.
//----------------------------------------------------------------------------
Float32 NMathUtilities::GetPointLineDistance(const CGPoint32 &thePoint, const CGPoint32 &lineStart, const CGPoint32 &lineEnd)
{


    // Get the distance
    return(GetPointLineDistance<CGPoint32, Float32>(thePoint, lineStart, lineEnd));
}





//============================================================================
//      NMathUtilities::GetPointLineDistance : Get the distance between a point and a line.
//----------------------------------------------------------------------------
Float64 NMathUtilities::GetPointLineDistance(const CGPoint64 &thePoint, const CGPoint64 &lineStart, const CGPoint64 &lineEnd)
{


    // Get the distance
    return(GetPointLineDistance<CGPoint64, Float64>(thePoint, lineStart, lineEnd));
}





//============================================================================
//      NMathUtilities::GetPointLineIntersection : Get the intersection between a point and a line.
//----------------------------------------------------------------------------
CGPoint32 NMathUtilities::GetPointLineIntersection(const CGPoint32 &thePoint, const CGPoint32 &lineStart, const CGPoint32 &lineEnd, Float32 *lineU)
{


    // Get the intersection
    return(GetPointLineIntersection<CGPoint32, Float32>(thePoint, lineStart, lineEnd, lineU));
}





//============================================================================
//      NMathUtilities::GetPointLineIntersection : Get the intersection between a point and a line.
//----------------------------------------------------------------------------
CGPoint64 NMathUtilities::GetPointLineIntersection(const CGPoint64 &thePoint, const CGPoint64 &lineStart, const CGPoint64 &lineEnd, Float64 *lineU)
{


    // Get the intersection
    return(GetPointLineIntersection<CGPoint64, Float64>(thePoint, lineStart, lineEnd, lineU));
}





//============================================================================
//      NMathUtilities::GetPointPointDistance : Get the distance between two points.
//----------------------------------------------------------------------------
#pragma mark -
template <class P, class F> F NMathUtilities::GetPointPointDistance(const P &point1, const P &point2)
{


    // Get the distance
    return((F) sqrt(GetPointPointDistance2(point1, point2)));
}





//============================================================================
//      NMathUtilities::GetPointPointDistance2 : Get the squared distance between two points.
//----------------------------------------------------------------------------
template <class P, class F> F NMathUtilities::GetPointPointDistance2(const P &point1, const P &point2)
{   F     dx, dy;



    // Get the distance squared
    dx = (point2.x - point1.x);
    dy = (point2.y - point1.y);
    
    return((dx * dx) + (dy * dy));
}





//============================================================================
//      NMathUtilities::GetPointLineDistance : Get the distance between a point and a line.
//----------------------------------------------------------------------------
template <class P, class F> F NMathUtilities::GetPointLineDistance(const P &thePoint, const P &lineStart, const P &lineEnd)
{   P       pointLine;


    // Get the distance
	pointLine = GetPointLineIntersection<P, F>(thePoint, lineStart, lineEnd, NULL);
	
    return(GetPointPointDistance(thePoint, pointLine));
}





//============================================================================
//      NMathUtilities::GetPointLineIntersection : Get the intersection between a point and a line.
//----------------------------------------------------------------------------
template <class P, class F> P NMathUtilities::GetPointLineIntersection(const P &thePoint, const P &lineStart, const P &lineEnd, F *lineU)
{   F       dx, dy, u, mag;
    P       pointLine;



    // Find the closest point on the line
    if (NMathUtilities::AreEqual(lineStart.x, lineEnd.x) &&
        NMathUtilities::AreEqual(lineStart.y, lineEnd.y))
        {
        pointLine = lineStart;
		u         = 0.0f;
        }
    else
        {
        // Calculate the intersection point
        dx  = lineEnd.x - lineStart.x;
        dy  = lineEnd.y - lineStart.y;

        mag = GetPointPointDistance2(lineEnd, lineStart);
        u   = (((thePoint.x - lineStart.x) * dx) + ((thePoint.y - lineStart.y) * dy)) / mag;


        // Find the closest point
        if (u < 0.0f)
            pointLine = lineStart;

        else if (u > 1.0f)
            pointLine = lineEnd;
        
        else
            {
            pointLine.x = lineStart.x + (dx * u);
            pointLine.y = lineStart.y + (dy * u);
            }
        }



	// Get the intersection
	if (lineU != NULL)
		*lineU = u;

	return(pointLine);
}




