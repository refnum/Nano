/*  NAME:
        NMathUtilities.h

    DESCRIPTION:
        Math utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
#ifndef NMATHUTILITIES_HDR
#define NMATHUTILITIES_HDR
//============================================================================
//      Include files
//----------------------------------------------------------------------------





//============================================================================
//      Constants
//----------------------------------------------------------------------------
static const Float64 kNE												= 2.718281828459045235360;		// e
static const Float64 kNLog2E											= 1.442695040888963407360;		// log2(e)
static const Float64 kNLog10E											= 0.434294481903251827651;		// log10(e)
static const Float64 kNLn2												= 0.693147180559945309417;		// ln(2)
static const Float64 kNLn10												= 2.302585092994045684020;		// ln(10)
static const Float64 kNPi												= 3.141592653589793238460;		// pi
static const Float64 kNPi_2												= 1.570796326794896619230;		// pi/2
static const Float64 kNPi_4												= 0.785398163397448309616;		// pi/4
static const Float64 kN1_Pi												= 0.318309886183790671538;		// 1/pi
static const Float64 kN2_Pi												= 0.636619772367581343076;		// 2/pi
static const Float64 kN2_SqrtPi											= 1.128379167095512573900;		// 2/sqrt(pi)
static const Float64 kNSqrt2											= 1.414213562373095048800;		// sqrt(2)
static const Float64 kN1_Sqrt2											= 0.707106781186547524401;		// 1/sqrt(2)





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NMathUtilities {
public:
    // Floating point equality
	static bool							AreEqual(Float64 a, Float64 b);


    // Floating point tests
	static bool							IsZero(Float64 theValue);


    // Integer tests
	static bool							IsOdd(     SInt64 theValue);
	static bool							IsEven(    SInt64 theValue);
	static bool							IsPowerOf2(SInt64 theValue);


    // Count the number of set bits
	static UInt32						CountBits(UInt32 theValue);


    // Rotate an integer
	static UInt32						RotateLeft( UInt32 theValue, UInt32 rotateBy);
	static UInt32						RotateRight(UInt32 theValue, UInt32 rotateBy);


    // Convert between radians and degrees
	static NDegrees						GetDegrees(NRadians theValue);
	static NRadians						GetRadians(NDegrees theValue);


    // Fast (and approximate) reciprocal value
	static Float32						FastReciprocal(Float32 theValue);


    // Fast (and approximate) square root
	static Float32						FastRoot(   Float32 theValue);
	static Float32						FastInvRoot(Float32 theValue);
};



#endif // NMATHUTILITIES_HDR


