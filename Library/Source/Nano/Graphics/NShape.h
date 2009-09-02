/*	NAME:
		NShape.h

	DESCRIPTION:
		Shape object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
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
#include "NRectangle.h"
#include "NPoint.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NShape;

template <class T> class NShapeT;

typedef NShapeT<Float32> NShape32;
typedef NShapeT<Float64> NShape64;


// Lists
typedef std::vector<NShape>											NShapeList;
typedef NShapeList::iterator										NShapeListIterator;
typedef NShapeList::const_iterator									NShapeListConstIterator;

typedef std::vector<NShape32>										NShape32List;
typedef NShape32List::iterator										NShape32ListIterator;
typedef NShape32List::const_iterator								NShape32ListConstIterator;

typedef std::vector<NShape64>										NShape64List;
typedef NShape64List::iterator										NShape64ListIterator;
typedef NShape64List::const_iterator								NShape64ListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template<class T> class NShapeT : public NComparable< NShapeT<T> > {
public:
										NShapeT(const std::vector< NPointT<T> > &thePoints, const NIndexList &theLoops=NIndexList());

										NShapeT(void);
	virtual							   ~NShapeT(void);


	// Clear the shape
	void								Clear(void);


	// Test the shape
	bool								IsEmpty(void) const;


	// Compare the value
	NComparison							Compare(const NShapeT<T> &theValue) const;


	// Get the bounds
	NRectangleT<T>						GetBounds(void) const;
	NRectangleT<T>						GetBounds(const std::vector< NPointT<T> > &thePoints) const;


	// Operators
										operator NFormatArgument(void) const;


public:
	std::vector< NPointT<T> >			points;
	NIndexList							loops;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NShape :	public NEncodable,
				public NShape32 {
public:
										NENCODABLE_DECLARE(NShape);

										NShape(const NShape32 &theShape);
										NShape(const NShape64 &theShape);

										NShape(const NPoint32List &thePoints, const NIndexList &theLoops=NIndexList());
										NShape(const NPoint64List &thePoints, const NIndexList &theLoops=NIndexList());

										NShape(void);
	virtual							   ~NShape(void);


	// Operators
										operator NShape64(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NPoint64List						Convert32To64(const NPoint32List &points32) const;
	NPoint32List						Convert64To32(const NPoint64List &points64) const;
};





//============================================================================
//		Template files
//----------------------------------------------------------------------------
#define   NSHAPE_CPP
#include "NShape.cpp"
#undef    NSHAPE_CPP





#endif // NSIZE_HDR


