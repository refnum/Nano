/*  NAME:
        NMathUtilities.h

    DESCRIPTION:
        Math utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2007, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
#ifndef NMATHUTILITIES_HDR
#define NMATHUTILITIES_HDR
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NMathUtilities {
public:
    // Floating point equality
    static bool                         AreEqual(Float64 a, Float64 b);
    static bool                         NotEqual(Float64 a, Float64 b);


    // Floating point comparisons
    static bool                         IsZero( Float64 theValue);
    static bool                         NotZero(Float64 theValue);


    // Integer comparisons
    static bool                         IsOdd(     SInt64 theValue);
    static bool                         IsEven(    SInt64 theValue);
    static bool                         IsPowerOf2(SInt64 theValue);


    // Count the number of set bits
    static UInt32                       CountBits(UInt32 theValue);


    // Rotate an integer
    static UInt32                       RotateLeft( UInt32 theValue, UInt32 rotateBy);
    static UInt32                       RotateRight(UInt32 theValue, UInt32 rotateBy);


    // Convert between radians and degrees
    static Float64                      GetDegrees(Float64 radian);
    static Float64                      GetRadians(Float64 degrees);


    // Get an absolute value
    static Float32                      GetAbsolute(Float32 theValue);


    // Fast (and approximate) reciprocal value
    static Float32                      FastReciprocal(Float32 theValue);


    // Fast (and approximate) square root
    static Float32                      FastRoot(   Float32 theValue);
    static Float32                      FastInvRoot(Float32 theValue);


    // Get the distance between two points
    static Float32                      GetPointPointDistance(const CGPoint32 &point1, const CGPoint32 &point2);
    static Float64                      GetPointPointDistance(const CGPoint64 &point1, const CGPoint64 &point2);


    // Get the distance squared between two points
    static Float32                      GetPointPointDistance2(const CGPoint32 &point1, const CGPoint32 &point2);
    static Float64                      GetPointPointDistance2(const CGPoint64 &point1, const CGPoint64 &point2);


    // Get the distance between a point and a line
    static Float32                      GetPointLineDistance(const CGPoint32 &thePoint, const CGPoint32 &lineStart, const CGPoint32 &lineEnd);
    static Float64                      GetPointLineDistance(const CGPoint64 &thePoint, const CGPoint64 &lineStart, const CGPoint64 &lineEnd);


    // Get the intersection between a point and a line
    static CGPoint32                    GetPointLineIntersection(const CGPoint32 &thePoint, const CGPoint32 &lineStart, const CGPoint32 &lineEnd, Float32 *lineU=NULL);
    static CGPoint64                    GetPointLineIntersection(const CGPoint64 &thePoint, const CGPoint64 &lineStart, const CGPoint64 &lineEnd, Float64 *lineU=NULL);


private:
    template <class P, class F> static F GetPointPointDistance( const P &point1, const P &point2);
    template <class P, class F> static F GetPointPointDistance2(const P &point1, const P &point2);
    template <class P, class F> static F GetPointLineDistance(    const P &thePoint, const P &lineStart, const P &lineEnd);
    template <class P, class F> static P GetPointLineIntersection(const P &thePoint, const P &lineStart, const P &lineEnd, F *lineU);
};

    
    


//============================================================================
//      Inline functions
//----------------------------------------------------------------------------
inline bool operator ==(const HIPoint &value1, const HIPoint &value2)
{
    return(NMathUtilities::AreEqual(value1.x, value2.x) &&
           NMathUtilities::AreEqual(value1.y, value2.y));
}

inline bool operator !=(const HIPoint &value1, const HIPoint &value2)
{
    return(NMathUtilities::NotEqual(value1.x, value2.x) ||
           NMathUtilities::NotEqual(value1.y, value2.y));
}

inline bool operator ==(const HISize &value1, const HISize &value2)
{
    return(NMathUtilities::AreEqual(value1.width,  value2.width) &&
           NMathUtilities::AreEqual(value1.height, value2.height));
}

inline bool operator !=(const HISize &value1, const HISize &value2)
{
    return(NMathUtilities::NotEqual(value1.width,  value2.width) ||
           NMathUtilities::NotEqual(value1.height, value2.height));
}

inline bool operator ==(const HIRect &value1, const HIRect &value2)
{
    return(value1.origin == value2.origin && value1.size == value2.size);
}

inline bool operator !=(const HIRect &value1, const HIRect &value2)
{
    return(value1.origin != value2.origin || value1.size != value2.size);
}





#endif // NMATHUTILITIES_HDR


