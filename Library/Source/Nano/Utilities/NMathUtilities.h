/*  NAME:
        NMathUtilities.h

    DESCRIPTION:
        Math utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
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
static const float64_t kNE												= 2.718281828459045235360;		// e
static const float64_t kNLog2E											= 1.442695040888963407360;		// log2(e)
static const float64_t kNLog10E											= 0.434294481903251827651;		// log10(e)
static const float64_t kNLn2											= 0.693147180559945309417;		// ln(2)
static const float64_t kNLn10											= 2.302585092994045684020;		// ln(10)
static const float64_t kNPi												= 3.141592653589793238460;		// pi
static const float64_t kNPi_2											= 1.570796326794896619230;		// pi/2
static const float64_t kNPi_4											= 0.785398163397448309616;		// pi/4
static const float64_t kN1_Pi											= 0.318309886183790671538;		// 1/pi
static const float64_t kN2_Pi											= 0.636619772367581343076;		// 2/pi
static const float64_t kN2_SqrtPi										= 1.128379167095512573900;		// 2/sqrt(pi)
static const float64_t kNSqrt2											= 1.414213562373095048800;		// sqrt(2)
static const float64_t kN1_Sqrt2										= 0.707106781186547524401;		// 1/sqrt(2)





//============================================================================
//      Class declaration
//----------------------------------------------------------------------------
class NMathUtilities {
public:
    // Floating point equality
	static bool							AreEqual(float32_t a, float32_t b, NIndex maxUlps=15);
	static bool							AreEqual(float64_t a, float64_t b, NIndex maxUlps=15);


    // Floating point tests
	static bool							IsZero(float32_t theValue);
	static bool							IsZero(float64_t theValue);

	static bool							IsNan(float64_t theValue);
	static bool							IsInf(float64_t theValue);


    // Integer tests
	static bool							IsOdd(     int64_t theValue);
	static bool							IsEven(    int64_t theValue);
	static bool							IsPowerOf2(int64_t theValue);


    // Count the number of set bits
	static NIndex						CountBits(uint32_t theValue);


    // Rotate an integer
	static uint32_t						RotateLeft( uint32_t theValue, uint32_t rotateBy);
	static uint32_t						RotateRight(uint32_t theValue, uint32_t rotateBy);


    // Convert between radians and degrees
	static NDegrees						GetDegrees(NRadians theValue);
	static NRadians						GetRadians(NDegrees theValue);


	// Get the sign
	static float64_t					GetSign(float64_t theValue);


	// Math operations
	static float64_t					Sec(  float64_t theValue);
	static float64_t					Csc(  float64_t theValue);
	static float64_t					Ctan( float64_t theValue);
	static float64_t					Asin( float64_t theValue);
	static float64_t					Acos( float64_t theValue);
	static float64_t					Asec( float64_t theValue);
	static float64_t					Acsc( float64_t theValue);
	static float64_t					Acot( float64_t theValue);
	static float64_t					Sinh( float64_t theValue);
	static float64_t					Cosh( float64_t theValue);
	static float64_t					Tanh( float64_t theValue);
	static float64_t					Sech( float64_t theValue);
	static float64_t					Csch( float64_t theValue);
	static float64_t					Coth( float64_t theValue);
	static float64_t					Asinh(float64_t theValue);
	static float64_t					Acosh(float64_t theValue);
	static float64_t					Atanh(float64_t theValue);
	static float64_t					Asech(float64_t theValue);
	static float64_t					Acsch(float64_t theValue);
	static float64_t					Acoth(float64_t theValue);
	static float64_t					Rint( float64_t theValue);


	// Random numbers
	//
	// Assigning a zero value for the seed will generate a random seed.
	static uint32_t						SetRandomSeed(uint32_t theSeed=0);
	
	static bool							GetRandomBoolean(void);
	static uint8_t						GetRandomUInt8( uint8_t  valueMin=kUInt8Min,  uint8_t  valueMax=kUInt8Max);
	static uint16_t						GetRandomUInt16(uint16_t valueMin=kUInt16Min, uint16_t valueMax=kUInt16Max);
	static uint32_t						GetRandomUInt32(uint32_t valueMin=kUInt32Min, uint32_t valueMax=kUInt32Max);
	static uint64_t						GetRandomUInt64(uint64_t valueMin=kUInt64Min, uint64_t valueMax=kUInt64Max);


private:
	static uint32_t						GetRandomValue(uint32_t valueMin, uint32_t valueMax);
};



#endif // NMATHUTILITIES_HDR


