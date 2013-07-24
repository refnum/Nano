/*	NAME:
		NTargetMath.h

	DESCRIPTION:
		Math support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_MATH_HDR
#define NTARGET_MATH_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetMath {
public:
	// Rotate an integer
	static uint32_t						RotateLeft( uint32_t theValue, uint32_t rotateBy);
	static uint32_t						RotateRight(uint32_t theValue, uint32_t rotateBy);
};





#endif // NTARGET_MATH_HDR











