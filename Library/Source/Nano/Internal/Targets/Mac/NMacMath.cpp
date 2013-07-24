/*	NAME:
		NMacMath.cpp

	DESCRIPTION:
		Mac math support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
uint32_t NTargetMath::RotateLeft(uint32_t theValue, uint32_t rotateBy)
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
uint32_t NTargetMath::RotateRight(uint32_t theValue, uint32_t rotateBy)
{


    // Rotate the value
#if TARGET_CPU_PPC
    return(__rlwnm(theValue, 32 - rotateBy, 0, 31));
#else
    return((theValue >> rotateBy) | (theValue << (32 - rotateBy)));
#endif
}



