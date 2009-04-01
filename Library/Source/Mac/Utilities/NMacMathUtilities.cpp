/*  NAME:
        NMacMathUtilities.cpp

    DESCRIPTION:
        Mac math utilities.
    
    COPYRIGHT:
        Copyright (c) 2006-2009, refNum Software
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

#include "NMathUtilities.h"





//============================================================================
//      NMathUtilities::IsNan : Is a value NAN?
//----------------------------------------------------------------------------
bool NMathUtilities::IsNan(Float64 theValue)
{


	// Check the value
	return(isnan(theValue));
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



