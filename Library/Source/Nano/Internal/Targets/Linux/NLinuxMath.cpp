/*	NAME:
		NLinuxMath.cpp

	DESCRIPTION:
		Linux math support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetMath.h"





//============================================================================
//      NTargetMath::RotateLeft : Left-rotate.
//----------------------------------------------------------------------------
UInt32 NTargetMath::RotateLeft(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
    return((theValue << rotateBy) | (theValue >> (32 - rotateBy)));
}





//============================================================================
//      NTargetMath::RotateRight : Right-rotate.
//----------------------------------------------------------------------------
UInt32 NTargetMath::RotateRight(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
    return((theValue >> rotateBy) | (theValue << (32 - rotateBy)));
}





//============================================================================
//      NTargetMath::FastReciprocal : Get an approximate reciprocal.
//----------------------------------------------------------------------------
Float32 NTargetMath::FastReciprocal(Float32 theValue)
{


    // Get the value
    return(1.0f / theValue);
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
    return(FastReciprocal(FastRoot(theValue)));
}



