/*  NAME:
        NMathUtilities.cpp

    DESCRIPTION:
        Math utilities.

		Non-POSIX math functions derived from:
		
			http://msdn.microsoft.com/en-us/library/w3t84e33(VS.71).aspx
    
    COPYRIGHT:
        Copyright (c) 2006-2013, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NTargetMath.h"
#include "NTargetPOSIX.h"
#include "NTargetTime.h"
#include "NMathUtilities.h"





//============================================================================
//      Global variables
//----------------------------------------------------------------------------
UInt32 gNMathUtilitiesRandomSeed = NMathUtilities::SetRandomSeed();





//============================================================================
//      NMathUtilities::AreEqual : Are two floating point numbers equal?
//----------------------------------------------------------------------------
bool NMathUtilities::AreEqual(Float32 a, Float32 b, NIndex maxUlps)
{	SInt32		intA, intB, theDelta;
	bool		areEqual;



	// Validate our parameters
    NN_ASSERT(!NTargetPOSIX::is_nan(a) && !NTargetPOSIX::is_inf(a));
    NN_ASSERT(!NTargetPOSIX::is_nan(b) && !NTargetPOSIX::is_inf(b));
	NN_ASSERT(maxUlps > 0 && maxUlps < (4 * 1024 * 1024));



	// Compare as two's complement integers
	//
	// This uses the 'almost equal' technique of comparing IEEE floats by the
	// number of possible representations between the two values:
	//
	// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
	//
	// As noted there are situations where this is not perfect, but numbers that
	// are a few 'steps' (ulps) away from each other are essentially equal.
	intA = *(SInt32 *) &a;
	intB = *(SInt32 *) &b;

    if (intA < 0)
        intA = 0x80000000L - intA;

    if (intB < 0)
        intB = 0x80000000L - intB;

	theDelta = (SInt32) labs(intA - intB);
	areEqual = (theDelta <= maxUlps);

	return(areEqual);
}





//============================================================================
//      NMathUtilities::AreEqual : Are two floating point numbers equal?
//----------------------------------------------------------------------------
bool NMathUtilities::AreEqual(Float64 a, Float64 b, NIndex maxUlps)
{	SInt64		intA, intB, theDelta;
	bool		areEqual;



	// Validate our parameters
    NN_ASSERT(!NTargetPOSIX::is_nan(a) && !NTargetPOSIX::is_inf(a));
    NN_ASSERT(!NTargetPOSIX::is_nan(b) && !NTargetPOSIX::is_inf(b));
	NN_ASSERT(maxUlps > 0 && maxUlps < (4 * 1024 * 1024));



	// Compare as two's complement integers
	//
	// This uses the 'almost equal' technique of comparing IEEE floats by the
	// number of possible representations between the two values:
	//
	// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
	//
	// As noted there are situations where this is not perfect, but numbers that
	// are a few 'steps' (ulps) away from each other are essentially equal.
	intA = *(SInt64 *) &a;
	intB = *(SInt64 *) &b;

    if (intA < 0)
        intA = 0x8000000000000000LL - intA;

    if (intB < 0)
        intB = 0x8000000000000000LL - intB;
	
	theDelta = NTargetPOSIX::llabs(intA - intB);
	areEqual = (theDelta <= maxUlps);
	
	return(areEqual);
}





//============================================================================
//      NMathUtilities::IsZero : Is a floating point number zero?
//----------------------------------------------------------------------------
bool NMathUtilities::IsZero(Float32 theValue)
{


    // Test the value
    return(AreEqual(theValue, 0.0f));
}





//============================================================================
//      NMathUtilities::IsZero : Is a floating point number zero?
//----------------------------------------------------------------------------
bool NMathUtilities::IsZero(Float64 theValue)
{


    // Test the value
    return(AreEqual(theValue, 0.0));
}





//============================================================================
//      NMathUtilities::IsNan : Is a floating point number NaN?
//----------------------------------------------------------------------------
bool NMathUtilities::IsNan(Float64 theValue)
{


    // Test the value
    return(NTargetPOSIX::is_nan(theValue));
}





//============================================================================
//      NMathUtilities::IsInf : Is a floating point number infinite?
//----------------------------------------------------------------------------
bool NMathUtilities::IsInf(Float64 theValue)
{


    // Test the value
    return(NTargetPOSIX::is_inf(theValue));
}





//============================================================================
//      NMathUtilities::IsOdd : Is a number odd?
//----------------------------------------------------------------------------
bool NMathUtilities::IsOdd(SInt64 theValue)
{


    // Test the value
    return((theValue & 0x01) != 0);
}





//============================================================================
//      NMathUtilities::IsEven : Is a number even?
//----------------------------------------------------------------------------
bool NMathUtilities::IsEven(SInt64 theValue)
{


    // Test the value
    return((theValue & 0x01) == 0);
}





//============================================================================
//      NMathUtilities::IsPowerOf2 : Is a number a power of 2?
//----------------------------------------------------------------------------
bool NMathUtilities::IsPowerOf2(SInt64 theValue)
{


	// Test the value
	return(theValue != 0 && (theValue & (theValue - 1)) == 0);
}





//============================================================================
//      NMathUtilities::CountBits : Count the number of set bits.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::CountBits(UInt32 theValue)
{   UInt32  numBits;



    // Count the bits
    //
    // By subtracting 1, we can obtain a mask that removes the highest set bit
	// in the value. This ensures we only need to loop as many times as there
	// are set bits.
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
	return(NTargetMath::RotateLeft(theValue, rotateBy));
}





//============================================================================
//      NMathUtilities::RotateRight : Right-rotate.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::RotateRight(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
	return(NTargetMath::RotateRight(theValue, rotateBy));
}





//============================================================================
//      NMathUtilities::GetDegrees : Convert from radians to degrees.
//----------------------------------------------------------------------------
NDegrees NMathUtilities::GetDegrees(NRadians theValue)
{


    // Convert to degrees
    return(theValue * (180.0 / kNPi));
}





//============================================================================
//      NMathUtilities::GetRadians : Convert from degrees to radians.
//----------------------------------------------------------------------------
NRadians NMathUtilities::GetRadians(NDegrees theValue)
{


    // Convert to radians
    return(theValue * (kNPi / 180.0));
}





//============================================================================
//      NMathUtilities::GetSign : Get the sign.
//----------------------------------------------------------------------------
Float64 NMathUtilities::GetSign(Float64 theValue)
{


	// Get the sign
	return(theValue < 0.0 ? -1.0 : 1.0);
}





//============================================================================
//      NMathUtilities::Sec : Calculate the secant.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Sec(Float64 theValue)
{


    // Get the value
	return(1.0 / cos(theValue));
}





//============================================================================
//      NMathUtilities::Csc : Calculate the cosecant.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Csc(Float64 theValue)
{


    // Get the value
	return(1.0 / sin(theValue));
}





//============================================================================
//      NMathUtilities::Ctan : Calculate the cotangent.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Ctan(Float64 theValue)
{


    // Get the value
	return(1.0 / tan(theValue));
}





//============================================================================
//      NMathUtilities::Asin : Calculate the inverse sine.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Asin(Float64 theValue)
{


    // Get the value
	return(NTargetPOSIX::asin(theValue));
}





//============================================================================
//      NMathUtilities::Acos : Calculate the inverse cosine.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Acos(Float64 theValue)
{


    // Get the value
	return(NTargetPOSIX::acos(theValue));
}





//============================================================================
//      NMathUtilities::Asec : Calculate the inverse secant.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Asec(Float64 theValue)
{


    // Get the value
	return(2.0 * atan(1.0) - atan(GetSign(theValue) / sqrt(theValue * theValue - 1.0)));
}





//============================================================================
//      NMathUtilities::Acsc : Calculate the inverse cosecant.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Acsc(Float64 theValue)
{


    // Get the value
	return(atan(GetSign(theValue) / sqrt(theValue * theValue - 1.0)));
}





//============================================================================
//      NMathUtilities::Acot : Calculate the inverse cotangent.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Acot(Float64 theValue)
{


    // Get the value
	return(2.0 * atan(1.0) - atan(theValue));
}





//============================================================================
//      NMathUtilities::Sinh : Calculate the hyperbolic sine.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Sinh(Float64 theValue)
{


    // Get the value
	return(NTargetPOSIX::sinh(theValue));
}





//============================================================================
//      NMathUtilities::Cosh : Calculate the hyperbolic cosine.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Cosh(Float64 theValue)
{


    // Get the value
	return(NTargetPOSIX::cosh(theValue));
}





//============================================================================
//      NMathUtilities::Tanh : Calculate the hyperbolic tangent.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Tanh(Float64 theValue)
{


    // Get the value
	return(NTargetPOSIX::tanh(theValue));
}





//============================================================================
//      NMathUtilities::Sech : Calculate the hyperbolic secant.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Sech(Float64 theValue)
{


    // Get the value
	return(2.0 / (exp(theValue) + exp(-theValue)));
}





//============================================================================
//      NMathUtilities::Csch : Calculate the hyperbolic cosecant.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Csch(Float64 theValue)
{


    // Get the value
	return(2.0 / (exp(theValue) - exp(-theValue)));
}





//============================================================================
//      NMathUtilities::Coth : Calculate the hyperbolic cotangent.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Coth(Float64 theValue)
{


    // Get the value
	return((exp(theValue) + exp(-theValue)) / (exp(theValue) - exp(-theValue)));
}





//============================================================================
//      NMathUtilities::Asinh : Calculate the inverse hyperbolic sine.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Asinh(Float64 theValue)
{


    // Get the value
	return(NTargetPOSIX::asinh(theValue));
}





//============================================================================
//      NMathUtilities::Acosh : Calculate the inverse hyperbolic cosine.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Acosh(Float64 theValue)
{


    // Get the value
	return(NTargetPOSIX::acosh(theValue));
}





//============================================================================
//      NMathUtilities::Atanh : Calculate the inverse hyperbolic tangent.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Atanh(Float64 theValue)
{


    // Get the value
	return(NTargetPOSIX::atanh(theValue));
}





//============================================================================
//      NMathUtilities::Asech : Calculate the inverse hyperbolic secant.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Asech(Float64 theValue)
{


    // Get the value
	return(log((sqrt(-theValue * theValue + 1.0) + 1.0) / theValue));
}





//============================================================================
//      NMathUtilities::Acsch : Calculate the inverse hyperbolic cosecant.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Acsch(Float64 theValue)
{


    // Get the value
	return(log((GetSign(theValue) * sqrt(theValue * theValue + 1.0) + 1.0) / theValue));
}





//============================================================================
//      NMathUtilities::Acoth : Calculate the inverse hyperbolic cotangent.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Acoth(Float64 theValue)
{


    // Get the value
	return(log((theValue + 1.0) / (theValue - 1.0)) / 2.0);
}





//============================================================================
//      NMathUtilities::Rint : Round to the nearest integer.
//----------------------------------------------------------------------------
Float64 NMathUtilities::Rint(Float64 theValue)
{


    // Get the value
	return(NTargetPOSIX::rint(theValue));
}





//============================================================================
//		NMathUtilities::SetRandomSeed : Set the random seed.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::SetRandomSeed(UInt32 theSeed)
{	UInt32		pidSeed, timeSeed;



	// Generate a seed
	if (theSeed == 0)
		{
		pidSeed  = (UInt16) (NTargetPOSIX::getpid()                                  & 0xFFFF);
		timeSeed = (UInt16) (((UInt64) (NTargetTime::GetTime() / kNTimeMillisecond)) & 0xFFFF);
		theSeed  = (pidSeed << 16) | timeSeed;
		}



	// Set the seed
	NTargetPOSIX::srandom(theSeed);

	return(theSeed);
}





//============================================================================
//		NMathUtilities::GetRandomBoolean : Get a random bool.
//----------------------------------------------------------------------------
bool NMathUtilities::GetRandomBoolean(void)
{


	// Get the value
	return(GetRandomValue(0, 1) != 0);
}





//============================================================================
//		NMathUtilities::GetRandomUInt8 : Get a random UInt8.
//----------------------------------------------------------------------------
UInt8 NMathUtilities::GetRandomUInt8(UInt8 valueMin, UInt8 valueMax)
{


	// Get the value
	return((UInt8) GetRandomValue(valueMin, valueMax));
}





//============================================================================
//		NMathUtilities::GetRandomUInt16 : Get a random UInt16.
//----------------------------------------------------------------------------
UInt16 NMathUtilities::GetRandomUInt16(UInt16 valueMin, UInt16 valueMax)
{


	// Get the value
	return((UInt16) GetRandomValue(valueMin, valueMax));
}





//============================================================================
//		NMathUtilities::GetRandomUInt32 : Get a random UInt32.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::GetRandomUInt32(UInt32 valueMin, UInt32 valueMax)
{


	// Get the value
	return((UInt32) GetRandomValue(valueMin, valueMax));
}





//============================================================================
//		NMathUtilities::GetRandomUInt64 : Get a random UInt64.
//----------------------------------------------------------------------------
UInt64 NMathUtilities::GetRandomUInt64(UInt64 valueMin, UInt64 valueMax)
{	UInt64		theValue, theRange;



	// Validate our parameters
	NN_ASSERT(valueMax > valueMin);



	// Get the value
	theRange = valueMax - valueMin;
	theValue = (((UInt64) GetRandomUInt32()) << 32) |
				((UInt64) GetRandomUInt32());

	theValue = valueMin + (theValue % theRange);
	
	return(theValue);
}





#pragma mark private
//============================================================================
//		NMathUtilities::GetRandomValue : Get a random value.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::GetRandomValue(UInt32 valueMin, UInt32 valueMax)
{	UInt32		theValue, theRange;



	// Validate our parameters
	NN_ASSERT(valueMax > valueMin);



	// Get the value
	theRange = valueMax - valueMin;
	theValue = valueMin + (NTargetPOSIX::random() % theRange);



	// Check our state
	NN_ASSERT(theValue >= valueMin && theValue <= valueMax);

	return(theValue);
}



