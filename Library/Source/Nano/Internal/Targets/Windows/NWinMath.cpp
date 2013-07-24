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
uint32_t NTargetMath::RotateLeft(uint32_t theValue, uint32_t rotateBy)
{


    // Rotate the value
    return(RotateLeft32(theValue, rotateBy));
}





//============================================================================
//      NTargetMath::RotateRight : Right-rotate.
//----------------------------------------------------------------------------
uint32_t NTargetMath::RotateRight(uint32_t theValue, uint32_t rotateBy)
{


    // Rotate the value
    return(RotateRight32(theValue, rotateBy));
}


