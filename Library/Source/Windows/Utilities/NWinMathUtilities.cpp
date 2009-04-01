/*  NAME:
        NWinMathUtilities.cpp

    DESCRIPTION:
        Windows math utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
        <http://www.refnum.com/>

        All rights reserved. Released under the terms of licence.html.
    __________________________________________________________________________
*/
//============================================================================
//      Include files
//----------------------------------------------------------------------------
#include <math.h>
#include <float.h>

#include "NMathUtilities.h"





//============================================================================
//      NMathUtilities::IsNan : Is a value NAN?
//----------------------------------------------------------------------------
bool NMathUtilities::IsNan(Float64 theValue)
{


	// Check the value
	return(_isnan(theValue) != 0);
}





//============================================================================
//      NMathUtilities::RotateLeft : Left-rotate.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::RotateLeft(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
    return((theValue << rotateBy) | (theValue >> (32 - rotateBy)));
}





//============================================================================
//      NMathUtilities::RotateRight : Right-rotate.
//----------------------------------------------------------------------------
UInt32 NMathUtilities::RotateRight(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
    return((theValue >> rotateBy) | (theValue << (32 - rotateBy)));
}





//============================================================================
//      NMathUtilities::GetAbsolute : Get an absolute value.
//----------------------------------------------------------------------------
Float32 NMathUtilities::GetAbsolute(Float32 theValue)
{


    // Get the value
    return(fabsf(theValue));
}





//============================================================================
//      NMathUtilities::FastReciprocal : Get an approximate reciprocal.
//----------------------------------------------------------------------------
Float32 NMathUtilities::FastReciprocal(Float32 theValue)
{


    // Get the value
    return(1.0f / theValue);
}





//============================================================================
//      NMathUtilities::FastRoot : Get an approximate square root.
//----------------------------------------------------------------------------
Float32 NMathUtilities::FastRoot(Float32 theValue)
{


    // Get the value
    return(sqrtf(theValue));
}





//============================================================================
//      NMathUtilities::FastInvRoot : Get an approximate inverse square root.
//----------------------------------------------------------------------------
Float32 NMathUtilities::FastInvRoot(Float32 theValue)
{


    // Get the value
    return(FastReciprocal(FastRoot(theValue)));
}



