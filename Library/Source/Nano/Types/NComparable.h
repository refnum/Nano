/*	NAME:
		NComparable.h

	DESCRIPTION:
		Mix-in class for comparable objects.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCOMPARABLE_HDR
#define NCOMPARABLE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
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
// NComparable operators
//
// Most comparable objects should simply derive from NComparable to obtain the
// full range of comparable operators automatically.
//
// Objects that wish to avoid a v-table for space/performance reasons can use
// this macro to obtain NComparable behaviour directly.
#define NCOMPARABLE_OPERATORS(_type)																				\
	inline bool operator == (const _type &theValue) const { return(Compare(theValue) == kNCompareEqualTo);     };	\
	inline bool operator != (const _type &theValue) const { return(Compare(theValue) != kNCompareEqualTo);     };	\
	inline bool operator <= (const _type &theValue) const { return(Compare(theValue) != kNCompareGreaterThan); };	\
	inline bool operator <	(const _type &theValue) const { return(Compare(theValue) == kNCompareLessThan);    };	\
	inline bool operator >= (const _type &theValue) const { return(Compare(theValue) != kNCompareLessThan);    };	\
	inline bool operator >	(const _type &theValue) const { return(Compare(theValue) == kNCompareGreaterThan); }





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template <class T> class NComparable {
public:
										NComparable(void);
	virtual							   ~NComparable(void);


	// Compare the value
	virtual NComparison					Compare(const T &theValue) const = 0;


	// Operators
	NCOMPARABLE_OPERATORS(T);


protected:
	// Compare two blocks of data
	inline NComparison					CompareData(NIndex theSize1, const void *thePtr1,
													NIndex theSize2, const void *thePtr2) const;
};





//============================================================================
//		Inline functions
//----------------------------------------------------------------------------
// Get an NComparison from two values using < and >
//
// Overrides are provided for float-float comparisons for a better equality test.
template <class A, class B> inline NComparison GetComparison(const A &a, const B &b)
{
	return(a < b ? kNCompareLessThan : (a > b ? kNCompareGreaterThan : kNCompareEqualTo));
}

inline NComparison GetComparison(const Float32 &a, const Float32 &b)
{
	if (NMathUtilities::AreEqual(a, b))
		return(kNCompareEqualTo);
	else
		return(a < b ? kNCompareLessThan : kNCompareGreaterThan);
}

inline NComparison GetComparison(const Float64 &a, const Float64 &b)
{
	if (NMathUtilities::AreEqual(a, b))
		return(kNCompareEqualTo);
	else
		return(a < b ? kNCompareLessThan : kNCompareGreaterThan);
}


// Get an NComparison from a memcmp/strcmp-style result
inline NComparison GetComparison(SInt32 x)
{
	return(GetComparison(x, 0L));
}





//============================================================================
//		Class definition
//----------------------------------------------------------------------------
#define   NCOMPARABLE_CPP
#include "NComparable.cpp"
#undef    NCOMPARABLE_CPP





#endif // NCOMPARABLE_HDR
