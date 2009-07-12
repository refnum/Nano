/*	NAME:
		NMacMath.cpp

	DESCRIPTION:
		Mac math support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#if TARGET_CPU_PPC
    #include <ppc_intrinsics.h>
#endif

#include "NTargetMath.h"





//============================================================================
//      NTargetMath::RotateLeft : Left-rotate.
//----------------------------------------------------------------------------
UInt32 NTargetMath::RotateLeft(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
#if TARGET_CPU_PPC
    return(__rlwnm(theValue, rotateBy, 0, 31));
#else
    return((theValue << rotateBy) | (theValue >> (32 - rotateBy)));
#endif
}





//============================================================================
//      NTargetMath::RotateRight : Right-rotate.
//----------------------------------------------------------------------------
UInt32 NTargetMath::RotateRight(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
#if TARGET_CPU_PPC
    return(__rlwnm(theValue, 32 - rotateBy, 0, 31));
#else
    return((theValue >> rotateBy) | (theValue << (32 - rotateBy)));
#endif
}





//============================================================================
//      NTargetMath::GetAbsolute : Get an absolute value.
//----------------------------------------------------------------------------
Float32 NTargetMath::GetAbsolute(Float32 theValue)
{


    // Get the value
#if TARGET_CPU_PPC
    return((Float32) __fabs((Float64) theValue));
#else
    return(fabsf(theValue));
#endif
}





//============================================================================
//      NTargetMath::FastReciprocal : Get an approximate reciprocal.
//----------------------------------------------------------------------------
Float32 NTargetMath::FastReciprocal(Float32 theValue)
{


    // Get the value
#if TARGET_CPU_PPC
    return(__fres(theValue));
#else
    return(1.0f / theValue);
#endif
}





//============================================================================
//      NTargetMath::FastRoot : Get an approximate square root.
//----------------------------------------------------------------------------
Float32 NTargetMath::FastRoot(Float32 theValue)
{


    // Get the value
    return(sqrtf(theValue));
}





//============================================================================
//      NTargetMath::FastInvRoot : Get an approximate inverse square root.
//----------------------------------------------------------------------------
Float32 NTargetMath::FastInvRoot(Float32 theValue)
{


    // Get the value
#if TARGET_CPU_PPC
	return((Float32) __frsqrte(theValue));
#else
    return(FastReciprocal(FastRoot(theValue)));
#endif
}



