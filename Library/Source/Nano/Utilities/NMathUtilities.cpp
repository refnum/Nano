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
uint32_t gNMathUtilitiesRandomSeed = NMathUtilities::SetRandomSeed();





//============================================================================
//      NMathUtilities::AreEqual : Are two floating point numbers equal?
//----------------------------------------------------------------------------
bool NMathUtilities::AreEqual(float32_t a, float32_t b, NIndex maxUlps)
{	int32_t		intA, intB, theDelta;
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
	intA = *(int32_t *) &a;
	intB = *(int32_t *) &b;

    if (intA < 0)
        intA = 0x80000000L - intA;

    if (intB < 0)
        intB = 0x80000000L - intB;

	theDelta = (int32_t) labs(intA - intB);
	areEqual = (theDelta <= maxUlps);

	return(areEqual);
}





//============================================================================
//      NMathUtilities::AreEqual : Are two floating point numbers equal?
//----------------------------------------------------------------------------
bool NMathUtilities::AreEqual(float64_t a, float64_t b, NIndex maxUlps)
{	int64_t		intA, intB, theDelta;
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
	intA = *(int64_t *) &a;
	intB = *(int64_t *) &b;

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
bool NMathUtilities::IsZero(float32_t theValue)
{


    // Test the value
    return(AreEqual(theValue, 0.0f));
}





//============================================================================
//      NMathUtilities::IsZero : Is a floating point number zero?
//----------------------------------------------------------------------------
bool NMathUtilities::IsZero(float64_t theValue)
{


    // Test the value
    return(AreEqual(theValue, 0.0));
}





//============================================================================
//      NMathUtilities::IsNan : Is a floating point number NaN?
//----------------------------------------------------------------------------
bool NMathUtilities::IsNan(float64_t theValue)
{


    // Test the value
    return(NTargetPOSIX::is_nan(theValue));
}





//============================================================================
//      NMathUtilities::IsInf : Is a floating point number infinite?
//----------------------------------------------------------------------------
bool NMathUtilities::IsInf(float64_t theValue)
{


    // Test the value
    return(NTargetPOSIX::is_inf(theValue));
}





//============================================================================
//      NMathUtilities::IsOdd : Is a number odd?
//----------------------------------------------------------------------------
bool NMathUtilities::IsOdd(int64_t theValue)
{


    // Test the value
    return((theValue & 0x01) != 0);
}





//============================================================================
//      NMathUtilities::IsEven : Is a number even?
//----------------------------------------------------------------------------
bool NMathUtilities::IsEven(int64_t theValue)
{


    // Test the value
    return((theValue & 0x01) == 0);
}





//============================================================================
//      NMathUtilities::IsPowerOf2 : Is a number a power of 2?
//----------------------------------------------------------------------------
bool NMathUtilities::IsPowerOf2(int64_t theValue)
{


	// Test the value
	return(theValue != 0 && (theValue & (theValue - 1)) == 0);
}





//============================================================================
//      NMathUtilities::CountBits : Count the number of set bits.
//----------------------------------------------------------------------------
NIndex NMathUtilities::CountBits(uint32_t theValue)
{   NIndex	  numBits;



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
uint32_t NMathUtilities::RotateLeft(uint32_t theValue, uint32_t rotateBy)
{


    // Rotate the value
	return(NTargetMath::RotateLeft(theValue, rotateBy));
}





//============================================================================
//      NMathUtilities::RotateRight : Right-rotate.
//----------------------------------------------------------------------------
uint32_t NMathUtilities::RotateRight(uint32_t theValue, uint32_t rotateBy)
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
float64_t NMathUtilities::GetSign(float64_t theValue)
{


	// Get the sign
	return(theValue < 0.0 ? -1.0 : 1.0);
}





//============================================================================
//      NMathUtilities::Sec : Calculate the secant.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Sec(float64_t theValue)
{


    // Get the value
	return(1.0 / cos(theValue));
}





//============================================================================
//      NMathUtilities::Csc : Calculate the cosecant.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Csc(float64_t theValue)
{


    // Get the value
	return(1.0 / sin(theValue));
}





//============================================================================
//      NMathUtilities::Ctan : Calculate the cotangent.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Ctan(float64_t theValue)
{


    // Get the value
	return(1.0 / tan(theValue));
}





//============================================================================
//      NMathUtilities::Asin : Calculate the inverse sine.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Asin(float64_t theValue)
{


    // Get the value
	return(NTargetPOSIX::asin(theValue));
}





//============================================================================
//      NMathUtilities::Acos : Calculate the inverse cosine.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Acos(float64_t theValue)
{


    // Get the value
	return(NTargetPOSIX::acos(theValue));
}





//============================================================================
//      NMathUtilities::Asec : Calculate the inverse secant.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Asec(float64_t theValue)
{


    // Get the value
	return(2.0 * atan(1.0) - atan(GetSign(theValue) / sqrt(theValue * theValue - 1.0)));
}





//============================================================================
//      NMathUtilities::Acsc : Calculate the inverse cosecant.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Acsc(float64_t theValue)
{


    // Get the value
	return(atan(GetSign(theValue) / sqrt(theValue * theValue - 1.0)));
}





//============================================================================
//      NMathUtilities::Acot : Calculate the inverse cotangent.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Acot(float64_t theValue)
{


    // Get the value
	return(2.0 * atan(1.0) - atan(theValue));
}





//============================================================================
//      NMathUtilities::Sinh : Calculate the hyperbolic sine.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Sinh(float64_t theValue)
{


    // Get the value
	return(NTargetPOSIX::sinh(theValue));
}





//============================================================================
//      NMathUtilities::Cosh : Calculate the hyperbolic cosine.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Cosh(float64_t theValue)
{


    // Get the value
	return(NTargetPOSIX::cosh(theValue));
}





//============================================================================
//      NMathUtilities::Tanh : Calculate the hyperbolic tangent.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Tanh(float64_t theValue)
{


    // Get the value
	return(NTargetPOSIX::tanh(theValue));
}





//============================================================================
//      NMathUtilities::Sech : Calculate the hyperbolic secant.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Sech(float64_t theValue)
{


    // Get the value
	return(2.0 / (exp(theValue) + exp(-theValue)));
}





//============================================================================
//      NMathUtilities::Csch : Calculate the hyperbolic cosecant.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Csch(float64_t theValue)
{


    // Get the value
	return(2.0 / (exp(theValue) - exp(-theValue)));
}





//============================================================================
//      NMathUtilities::Coth : Calculate the hyperbolic cotangent.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Coth(float64_t theValue)
{


    // Get the value
	return((exp(theValue) + exp(-theValue)) / (exp(theValue) - exp(-theValue)));
}





//============================================================================
//      NMathUtilities::Asinh : Calculate the inverse hyperbolic sine.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Asinh(float64_t theValue)
{


    // Get the value
	return(NTargetPOSIX::asinh(theValue));
}





//============================================================================
//      NMathUtilities::Acosh : Calculate the inverse hyperbolic cosine.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Acosh(float64_t theValue)
{


    // Get the value
	return(NTargetPOSIX::acosh(theValue));
}





//============================================================================
//      NMathUtilities::Atanh : Calculate the inverse hyperbolic tangent.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Atanh(float64_t theValue)
{


    // Get the value
	return(NTargetPOSIX::atanh(theValue));
}





//============================================================================
//      NMathUtilities::Asech : Calculate the inverse hyperbolic secant.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Asech(float64_t theValue)
{


    // Get the value
	return(log((sqrt(-theValue * theValue + 1.0) + 1.0) / theValue));
}





//============================================================================
//      NMathUtilities::Acsch : Calculate the inverse hyperbolic cosecant.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Acsch(float64_t theValue)
{


    // Get the value
	return(log((GetSign(theValue) * sqrt(theValue * theValue + 1.0) + 1.0) / theValue));
}





//============================================================================
//      NMathUtilities::Acoth : Calculate the inverse hyperbolic cotangent.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Acoth(float64_t theValue)
{


    // Get the value
	return(log((theValue + 1.0) / (theValue - 1.0)) / 2.0);
}





//============================================================================
//      NMathUtilities::Rint : Round to the nearest integer.
//----------------------------------------------------------------------------
float64_t NMathUtilities::Rint(float64_t theValue)
{


    // Get the value
	return(NTargetPOSIX::rint(theValue));
}





//============================================================================
//		NMathUtilities::SetRandomSeed : Set the random seed.
//----------------------------------------------------------------------------
uint32_t NMathUtilities::SetRandomSeed(uint32_t theSeed)
{	uint32_t		pidSeed, timeSeed;



	// Generate a seed
	if (theSeed == 0)
		{
		pidSeed  = (uint16_t) (NTargetPOSIX::getpid()                                    & 0xFFFF);
		timeSeed = (uint16_t) (((uint64_t) (NTargetTime::GetTime() / kNTimeMillisecond)) & 0xFFFF);
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
//		NMathUtilities::GetRandomUInt8 : Get a random uint8_t.
//----------------------------------------------------------------------------
uint8_t NMathUtilities::GetRandomUInt8(uint8_t valueMin, uint8_t valueMax)
{


	// Get the value
	return((uint8_t) GetRandomValue(valueMin, valueMax));
}





//============================================================================
//		NMathUtilities::GetRandomUInt16 : Get a random uint16_t.
//----------------------------------------------------------------------------
uint16_t NMathUtilities::GetRandomUInt16(uint16_t valueMin, uint16_t valueMax)
{


	// Get the value
	return((uint16_t) GetRandomValue(valueMin, valueMax));
}





//============================================================================
//		NMathUtilities::GetRandomUInt32 : Get a random uint32_t.
//----------------------------------------------------------------------------
uint32_t NMathUtilities::GetRandomUInt32(uint32_t valueMin, uint32_t valueMax)
{


	// Get the value
	return((uint32_t) GetRandomValue(valueMin, valueMax));
}





//============================================================================
//		NMathUtilities::GetRandomUInt64 : Get a random uint64_t.
//----------------------------------------------------------------------------
uint64_t NMathUtilities::GetRandomUInt64(uint64_t valueMin, uint64_t valueMax)
{	uint64_t		theValue, theRange;



	// Validate our parameters
	NN_ASSERT(valueMax > valueMin);



	// Get the value
	theRange = valueMax - valueMin;
	theValue = (((uint64_t) GetRandomUInt32()) << 32) |
				((uint64_t) GetRandomUInt32());

	theValue = valueMin + (theValue % theRange);
	
	return(theValue);
}





#pragma mark private
//============================================================================
//		NMathUtilities::GetRandomValue : Get a random value.
//----------------------------------------------------------------------------
uint32_t NMathUtilities::GetRandomValue(uint32_t valueMin, uint32_t valueMax)
{	uint32_t		theValue, theRange;



	// Validate our parameters
	NN_ASSERT(valueMax > valueMin);



	// Get the value
	theRange = valueMax - valueMin;
	theValue = valueMin + (NTargetPOSIX::random() % theRange);



	// Check our state
	NN_ASSERT(theValue >= valueMin && theValue <= valueMax);

	return(theValue);
}



