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





//============================================================================
//		NPoint::== : Equality operator
//----------------------------------------------------------------------------
bool NPoint::operator == (const NPoint &thePoint) const
{


	// Compare the values
	return(NMathUtilities::AreEqual(x, thePoint.x) && NMathUtilities::AreEqual(y, thePoint.y));
}





//============================================================================
//		NPoint::!= : Inequality operator.
//----------------------------------------------------------------------------
bool NPoint::operator != (const NPoint &thePoint) const
{


	// Compare the values
	return(NMathUtilities::NotEqual(x, thePoint.x) || NMathUtilities::NotEqual(y, thePoint.y));
}

