/*	NAME:
		NComparable.h

	DESCRIPTION:
		Mix-in class for comparable objects.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOMPARABLE_HDR
#define NCOMPARABLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
typedef enum {
	kNCompareLessThan		= -1,
	kNCompareEqualTo		=  0,
	kNCompareGreaterThan	=  1
} NComparison;





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Compare two values
//
// Returns an NComparisonResult for two types that define the < and > operators.
#define GET_COMPARISON(_a, _b)						((_a) < (_b) ? kNCompareLessThan : ((_a) > (_b) ? kNCompareGreaterThan : kNCompareEqualTo))





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template <class T> class NComparable {
public:
										 NComparable(void);
	virtual								~NComparable(void);


	// Compare the value
	virtual NComparison					Compare(const T &theValue) const = 0;


	// Operators
	inline bool							operator == (const T &theValue) const;
	inline bool							operator != (const T &theValue) const;
	inline bool							operator <= (const T &theValue) const;
	inline bool							operator <	(const T &theValue) const;
	inline bool							operator >= (const T &theValue) const;
	inline bool							operator >	(const T &theValue) const;


protected:
	// Compare two blocks of data
	inline NComparison					CompareData(NIndex theSize1, const void *thePtr1,
													NIndex theSize2, const void *thePtr2) const;


private:


};





//============================================================================
//		Class definition
//----------------------------------------------------------------------------
#define   NCOMPARABLE_CPP
#include "NComparable.cpp"
#undef    NCOMPARABLE_CPP





#endif // NCOMPARABLE_HDR
