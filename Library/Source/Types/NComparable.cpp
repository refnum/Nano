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
//		Linker stub
//----------------------------------------------------------------------------
#ifndef NCOMPARABLE_CPP

void SuppressNoCodeLinkerWarning_NComparable(void);
void SuppressNoCodeLinkerWarning_NComparable(void)
{
}

#else





//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"





//============================================================================
//		NComparable::NComparable : Constructor.
//----------------------------------------------------------------------------
template<typename T> NComparable<T>::NComparable(void)
{
}





//============================================================================
//		NComparable::~NComparable : Destructor.
//----------------------------------------------------------------------------
template<typename T> NComparable<T>::~NComparable(void)
{
}





//============================================================================
//		NComparable::== : Equality operator
//----------------------------------------------------------------------------
template<typename T> bool NComparable<T>::operator == (const T &theValue) const
{


	// Compare the objects
	return(Compare(theValue) == kNCompareEqualTo);
}





//============================================================================
//		NComparable::!= : Inequality operator.
//----------------------------------------------------------------------------
template<typename T> bool NComparable<T>::operator != (const T &theValue) const
{


	// Compare the objects
	return(Compare(theValue) != kNCompareEqualTo);
}





//============================================================================
//		NComparable::<= : Comparison operator.
//----------------------------------------------------------------------------
template<typename T> bool NComparable<T>::operator <= (const T &theValue) const
{


	// Compare the objects
	return(Compare(theValue) != kNCompareGreaterThan);
}





//============================================================================
//		NComparable::< : Comparison operator.
//----------------------------------------------------------------------------
template<typename T> bool NComparable<T>::operator < (const T &theValue) const
{


	// Compare the objects
	return(Compare(theValue) == kNCompareLessThan);
}





//============================================================================
//		NComparable::>= : Comparison operator.
//----------------------------------------------------------------------------
template<typename T> bool NComparable<T>::operator >= (const T &theValue) const
{


	// Compare the objects
	return(Compare(theValue) != kNCompareLessThan);
}





//============================================================================
//		NComparable::> : Comparison operator.
//----------------------------------------------------------------------------
template<typename T> bool NComparable<T>::operator > (const T &theValue) const
{


	// Compare the objects
	return(Compare(theValue) == kNCompareGreaterThan);
}





//============================================================================
//		NComparable::CompareData : Compare two blocks of data.
//----------------------------------------------------------------------------
template<typename T> NComparison NComparable<T>::CompareData(NIndex theSize1, const void *thePtr1,
															 NIndex theSize2, const void *thePtr2) const
{	NComparison		theResult;



	// Compare the data
	//
	// Since differently-sized bits of data can't be ordered, we compare
	// by size first and only examine content if we have equal sizes.
	if (theSize1 != theSize2)
		theResult = GetComparison(theSize1, theSize2);
	else
		{
		if (thePtr1 == thePtr2)
			theResult = kNCompareEqualTo;
		else
			theResult = GetComparison(memcmp(thePtr1, thePtr2, theSize1));
		}
	
	return(theResult);
}






#endif // NCOMPARABLE_CPP

