/*	NAME:
		NComparable.cpp

	DESCRIPTION:
		Base class for comparable objects.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
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
	return(Compare(theObject) == kCFCompareEqualTo);
}





//============================================================================
//		NComparable::!= : Inequality operator.
//----------------------------------------------------------------------------
bool NComparable::operator != (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) != kCFCompareEqualTo);
}





//============================================================================
//		NComparable::<= : Comparison operator.
//----------------------------------------------------------------------------
bool NComparable::operator <= (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) != kCFCompareGreaterThan);
}





//============================================================================
//		NComparable::< : Comparison operator.
//----------------------------------------------------------------------------
bool NComparable::operator < (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) == kCFCompareLessThan);
}





//============================================================================
//		NComparable::>= : Comparison operator.
//----------------------------------------------------------------------------
bool NComparable::operator >= (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) != kCFCompareLessThan);
}





//============================================================================
//		NComparable::> : Comparison operator.
//----------------------------------------------------------------------------
bool NComparable::operator > (const NComparable &theObject) const
{


	// Compare the objects
	return(Compare(theObject) == kCFCompareGreaterThan);
}





//============================================================================
//		NComparable::Compare : Compare two objects.
//----------------------------------------------------------------------------
#pragma mark -
CFComparisonResult NComparable::Compare(const NComparable &theObject) const
{


	// Compare the objects
	return(GET_CF_COMPARE(this, &theObject));
}

