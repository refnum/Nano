/*  NAME:
        NMathUtilities.cpp

    DESCRIPTION:
        Math utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include "NTargetMath.h"
#include "NTargetPOSIX.h"
#include "NMathUtilities.h"





//============================================================================
//      NMathUtilities::AreEqual : Are two floating point numbers equal?
//----------------------------------------------------------------------------
bool NMathUtilities::AreEqual(Float64 a, Float64 b)
{   Float64   theDelta;



    // Validate our parameters
    NN_ASSERT(!NTargetPOSIX::is_nan(a));
    NN_ASSERT(!NTargetPOSIX::is_nan(b));



    // Check for equality
    theDelta = fabs(a - b);
    
    return(theDelta < 0.000000001);
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
Float64 NMathUtilities::GetDegrees(Float64 radian)
{


    // Convert to degrees
    return(radian * (180.0 / kPi));
}





//============================================================================
//      NMathUtilities::GetRadians : Convert from degrees to radians.
//----------------------------------------------------------------------------
Float64 NMathUtilities::GetRadians(Float64 degree)
{


    // Convert to radians
    return(degree * (kPi / 180.0));
}





//============================================================================
//      NMathUtilities::GetAbsolute : Get an absolute value.
//----------------------------------------------------------------------------
Float32 NMathUtilities::GetAbsolute(Float32 theValue)
{


    // Get the value
	return(NTargetMath::GetAbsolute(theValue));
}





//============================================================================
//      NMathUtilities::FastReciprocal : Get an approximate reciprocal.
//----------------------------------------------------------------------------
Float32 NMathUtilities::FastReciprocal(Float32 theValue)
{


    // Get the value
	return(NTargetMath::FastReciprocal(theValue));
}





//============================================================================
//      NMathUtilities::FastRoot : Get an approximate square root.
//----------------------------------------------------------------------------
Float32 NMathUtilities::FastRoot(Float32 theValue)
{


    // Get the value
	return(NTargetMath::FastRoot(theValue));
}





//============================================================================
//      NMathUtilities::FastInvRoot : Get an approximate inverse square root.
//----------------------------------------------------------------------------
Float32 NMathUtilities::FastInvRoot(Float32 theValue)
{


    // Get the value
	return(NTargetMath::FastInvRoot(theValue));
}




