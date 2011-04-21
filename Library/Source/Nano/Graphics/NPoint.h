/*	NAME:
		NPoint.h

	DESCRIPTION:
		Point object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NPOINT_HDR
#define NPOINT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NComparable.h"
#include "NEncodable.h"
#include "NVariant.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
template<class T> class NPointT;
template<class T> class NVectorT;

typedef NPointT<Float64> NPoint64;
typedef NPointT<Float32> NPoint32;
typedef NPoint32         NPoint;


// Lists
typedef std::vector<NPoint64>										NPoint64List;
typedef NPoint64List::iterator										NPoint64ListIterator;
typedef NPoint64List::const_iterator								NPoint64ListConstIterator;

typedef std::vector<NPoint32>										NPoint32List;
typedef NPoint32List::iterator										NPoint32ListIterator;
typedef NPoint32List::const_iterator								NPoint32ListConstIterator;

typedef std::vector<NPoint>											NPointList;
typedef NPointList::iterator										NPointListIterator;
typedef NPointList::const_iterator									NPointListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template<class T> class NPointT {
public:
										NPointT(const NVariant &theValue);

										NPointT(T x, T y);
										NPointT(void);


	// Clear the point
	void								Clear(void);


	// Test the point
	bool								IsZero(void) const;


	// Compare the value
	NComparison							Compare(const NPointT<T> &theValue) const;


	// Get the distance to a point
	T									GetDistance( const NPointT<T> &thePoint, bool getApprox=true) const;
	T									GetDistance2(const NPointT<T> &thePoint)                      const;


	// Manipulate the point
	void								MakeIntegral(void);
	void								Offset(   T deltaX, T deltaY);
	NPointT<T>							GetIntegral(void)             const;
	NPointT<T>							GetOffset(T deltaX, T deltaY) const;


	// Operators
	NCOMPARABLE_OPERATORS(NPointT<T>);

	const NPointT<T>&					operator +=(const NVectorT<T> &theVector);
	const NPointT<T>&					operator -=(const NVectorT<T> &theVector);

										operator NEncodable(     void) const;
										operator NFormatArgument(void) const;


public:
	T									x;
	T									y;
};





//============================================================================
//		Template files
//----------------------------------------------------------------------------
#define   NPOINT_CPP
#include "NPoint.cpp"
#undef    NPOINT_CPP





#endif // NPOINT_HDR


