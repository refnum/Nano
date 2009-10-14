/*	NAME:
		NMathUtilities.h

	DESCRIPTION:
		Math utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMATHUTILITIES_HDR
#define NMATHUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMathUtilities {
public:
	// Floating point equality
	static bool							AreEqual(float a, float b);
	static bool							NotEqual(float a, float b);


	// Floating point comparisons
	static bool							IsZero( float a);
	static bool							NotZero(float a);


	// Check for a power of 2
	static bool							IsPowerOf2(UInt32 n);
};

	
	


#endif // NMATHUTILITIES_HDR


