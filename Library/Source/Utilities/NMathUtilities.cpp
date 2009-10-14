/*	NAME:
		NMathUtilities.cpp

	DESCRIPTION:
		Math utilities.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include Maths
//----------------------------------------------------------------------------
#include "NMathUtilities.h"





//============================================================================
//		NMathUtilities::AreEqual : Are two floating point numbers equal?
//----------------------------------------------------------------------------
bool NMathUtilities::AreEqual(float a, float b)
{	float	theDelta;



	// Validate our parameters
	NN_ASSERT(!isnan(a));
	NN_ASSERT(!isnan(b));



	// Check for equality
	theDelta = fabs(a - b);
	
	return(theDelta < 0.00001f);
}





//============================================================================
//		NMathUtilities::NotEqual : Are two floating point numbers not equal?
//----------------------------------------------------------------------------
bool NMathUtilities::NotEqual(float a, float b)
{


	// Check for inequality
	return(!AreEqual(a, b));
}





//============================================================================
//		NMathUtilities::IsZero : Is a floating point number zero?
//----------------------------------------------------------------------------
bool NMathUtilities::IsZero(float a)
{


	// Check for zero
	return(AreEqual(a, 0.0f));
}





//============================================================================
//		NMathUtilities::NotZero : Is a floating point number non-zero?
//----------------------------------------------------------------------------
bool NMathUtilities::NotZero(float a)
{


	// Check for zero
	return(!AreEqual(a, 0.0f));
}





//============================================================================
//		NMathUtilities::IsPowerOf2 : Is a number a power of 2?
//----------------------------------------------------------------------------
bool NMathUtilities::IsPowerOf2(UInt32 n)
{


	// Check the number
	return((n & (n - 1)) == 0);
}
