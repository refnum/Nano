/*	NAME:
		NWinMath.cpp

	DESCRIPTION:
		Windows math support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <math.h>
#include <float.h>

#include "NTargetMath.h"





//============================================================================
//      NTargetMath::RotateLeft : Left-rotate.
//----------------------------------------------------------------------------
UInt32 NTargetMath::RotateLeft(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
    return(RotateLeft32(theValue, rotateBy));
}





//============================================================================
//      NTargetMath::RotateRight : Right-rotate.
//----------------------------------------------------------------------------
UInt32 NTargetMath::RotateRight(UInt32 theValue, UInt32 rotateBy)
{


    // Rotate the value
    return(RotateRight32(theValue, rotateBy));
}


