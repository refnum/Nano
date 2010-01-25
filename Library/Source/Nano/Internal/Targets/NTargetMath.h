/*	NAME:
		NTargetMath.h

	DESCRIPTION:
		Math support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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


	// Get an approximate value
	static Float32						FastReciprocal(Float32 theValue);
	static Float32						FastRoot(      Float32 theValue);
	static Float32						FastInvRoot(   Float32 theValue);
};





#endif // NTARGET_MATH_HDR











