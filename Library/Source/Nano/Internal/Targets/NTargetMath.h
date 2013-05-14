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
	static UInt32						RotateLeft( UInt32 theValue, UInt32 rotateBy);
	static UInt32						RotateRight(UInt32 theValue, UInt32 rotateBy);
};





#endif // NTARGET_MATH_HDR











