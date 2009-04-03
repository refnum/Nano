/*	NAME:
		NPoint.cpp

	DESCRIPTION:
		Point object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NPoint.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
const NPoint kNPointZero;





//============================================================================
//		NPoint::NPoint : Constructor.
//----------------------------------------------------------------------------
NPoint::NPoint(Float32 valX, Float32 valY)
{


	// Initialize ourselves
	x = valX;
	y = valY;
}





//============================================================================
//		NPoint::~NPoint : Destructor.
//----------------------------------------------------------------------------
NPoint::~NPoint(void)
{
}





//============================================================================
//		NPoint::Clear : Clear the point.
//----------------------------------------------------------------------------
void NPoint::Clear(void)
{


	// Clear the point
	x = 0.0f;
	y = 0.0f;
}





//============================================================================
//		NPoint::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NPoint::Compare(const NPoint &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GET_COMPARISON(x, theValue.x);
		
	if (theResult == kNCompareEqualTo)
		theResult = GET_COMPARISON(y, theValue.y);
	
	return(theResult);
}





//============================================================================
//		NPoint::IsZero : Is the point zero?
//----------------------------------------------------------------------------
bool NPoint::IsZero(void) const
{


	// Test the point
	return(NMathUtilities::IsZero(x) && NMathUtilities::IsZero(y));
}





//============================================================================
//		NPoint::IsNotZero : Is the point non-zero?
//----------------------------------------------------------------------------
bool NPoint::IsNotZero(void) const
{


	// Test the point
	return(NMathUtilities::NotZero(x) || NMathUtilities::NotZero(y));
}

