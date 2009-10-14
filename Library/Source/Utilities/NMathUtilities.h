/*	NAME:
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
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMathUtilities {
public:
	// Floating point equality
	static bool							AreEqual(Float64 a, Float64 b);
	static bool							NotEqual(Float64 a, Float64 b);


	// Floating point comparisons
	static bool							IsZero( Float64 theValue);
	static bool							NotZero(Float64 theValue);


	// Integer comparisons
	static bool							IsOdd(     SInt64 theValue);
	static bool							IsEven(    SInt64 theValue);
    static bool							IsPowerOf2(SInt64 theValue);


	// Count the number of set bits
	static UInt32						CountBits(UInt32 theValue);


	// Rotate an integer
	static UInt32						RotateLeft( UInt32 theValue, UInt32 rotateBy);
	static UInt32						RotateRight(UInt32 theValue, UInt32 rotateBy);


	// Convert between radians and degrees
	static Float64						GetDegrees(Float64 radian);
	static Float64						GetRadians(Float64 degrees);


	// Get an absolute value
	static Float32						GetAbsolute(Float32 theValue);


	// Fast (and approximate) reciprocal value
	static Float32						FastReciprocal(Float32 theValue);


	// Fast (and approximate) square root
	static Float32						FastRoot(	Float32 theValue);
	static Float32						FastInvRoot(Float32 theValue);


	// Get the distance between two points
	static Float32						GetPointPointDistance( const CGPoint &point1, const CGPoint &point2);
	static Float32						GetPointPointDistance2(const CGPoint &point1, const CGPoint &point2);


	// Get the distance between a point and a line
	static Float32						GetPointLineDistance(const CGPoint &thePoint, const CGPoint &lineStart, const CGPoint &lineEnd);
};

	
	


#endif // NMATHUTILITIES_HDR


