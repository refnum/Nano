/*	NAME:
		NComparable.cpp

	DESCRIPTION:
		Mix-in class for comparable objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"





//============================================================================
//		NComparable::NComparable : Constructor.
//----------------------------------------------------------------------------
NComparable::NComparable(void)
{
}





//============================================================================
//		NComparable::~NComparable : Destructor.
//----------------------------------------------------------------------------
NComparable::~NComparable(void)
{
}





//============================================================================
//		NComparable::== : Equality operator
//----------------------------------------------------------------------------
bool NComparable::operator == (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) == kNCompareEqualTo);
}





//============================================================================
//		NComparable::!= : Inequality operator.
//----------------------------------------------------------------------------
bool NComparable::operator != (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) != kNCompareEqualTo);
}





//============================================================================
//		NComparable::<= : Comparison operator.
//----------------------------------------------------------------------------
bool NComparable::operator <= (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) != kNCompareGreaterThan);
}





//============================================================================
//		NComparable::< : Comparison operator.
//----------------------------------------------------------------------------
bool NComparable::operator < (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) == kNCompareLessThan);
}





//============================================================================
//		NComparable::>= : Comparison operator.
//----------------------------------------------------------------------------
bool NComparable::operator >= (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) != kNCompareLessThan);
}





//============================================================================
//		NComparable::> : Comparison operator.
//----------------------------------------------------------------------------
bool NComparable::operator > (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) == kNCompareGreaterThan);
}





//============================================================================
//		NComparable::Compare : Compare two objects.
//----------------------------------------------------------------------------
#pragma mark -
NComparison NComparable::Compare(const NComparable &theObject) const
{


	// Compare the objects
	return(GET_COMPARISON(this, &theObject));
}

