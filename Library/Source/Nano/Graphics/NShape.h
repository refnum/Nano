/*	NAME:
		NShape.h

	DESCRIPTION:
		Shape object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSHAPE_HDR
#define NSHAPE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NComparable.h"
#include "NEncodable.h"
#include "NVariant.h"
#include "NRectangle.h"
#include "NPoint.h"
#include "NArray.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
template <class T> class NShapeT;

typedef NShapeT<Float64> NShape64;
typedef NShapeT<Float32> NShape32;
typedef NShape32         NShape;


// Lists
typedef std::vector<NShape64>										NShape64List;
typedef NShape64List::iterator										NShape64ListIterator;
typedef NShape64List::const_iterator								NShape64ListConstIterator;

typedef std::vector<NShape32>										NShape32List;
typedef NShape32List::iterator										NShape32ListIterator;
typedef NShape32List::const_iterator								NShape32ListConstIterator;

typedef std::vector<NShape>											NShapeList;
typedef NShapeList::iterator										NShapeListIterator;
typedef NShapeList::const_iterator									NShapeListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template<class T> class NShapeT {
public:
										NShapeT(const NVariant &theValue);

										NShapeT(const std::vector< NPointT<T> > &thePoints, const NIndexList &theLoops=NIndexList());
										NShapeT(void);


	// Clear the shape
	void								Clear(void);


	// Test the shape
	bool								IsEmpty(void) const;


	// Compare the value
	NComparison							Compare(const NShapeT<T> &theValue) const;


	// Operators
	NCOMPARABLE_OPERATORS(NPointT<T>);

										operator NEncodable(     void) const;
										operator NFormatArgument(void) const;


public:
	std::vector< NPointT<T> >			points;
	NIndexList							loops;
};





//============================================================================
//		Template files
//----------------------------------------------------------------------------
#define   NSHAPE_CPP
#include "NShape.cpp"
#undef    NSHAPE_CPP





#endif // NSIZE_HDR


