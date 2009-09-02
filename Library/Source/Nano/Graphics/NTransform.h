/*	NAME:
		NTransform.h

	DESCRIPTION:
		Transform object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTRANSFORM_HDR
#define NTRANSFORM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NEncodable.h"
#include "NComparable.h"
#include "NRectangle.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NTransform;

template <class T> class NTransformT;

typedef NTransformT<Float32> NTransform32;
typedef NTransformT<Float64> NTransform64;


// Lists
typedef std::vector<NTransform>										NTransformList;
typedef NTransformList::iterator									NTransformListIterator;
typedef NTransformList::const_iterator								NTransformListConstIterator;

typedef std::vector<NTransform32>									NTransform32List;
typedef NTransform32List::iterator									NTransform32ListIterator;
typedef NTransform32List::const_iterator							NTransform32ListConstIterator;

typedef std::vector<NTransform64>									NTransform64List;
typedef NTransform64List::iterator									NTransform64ListIterator;
typedef NTransform64List::const_iterator							NTransform64ListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template<class T> class NTransformT : public NComparable< NTransformT<T> > {
public:
										NTransformT(T a, T b, T c, T d, T tx, T ty);

										NTransformT(void);
	virtual							   ~NTransformT(void);


	// Clear to the identity transform
	void								Clear(void);


	// Is the transform the identity transform?
	bool								IsIdentity(void) const;


	// Compare the value
	NComparison							Compare(const NTransformT<T> &theValue) const;


	// Set the transform
	void								SetTranslate(T x, T y);
	void								SetScale(    T x, T y);
	void								SetRotate(   NRadians angle);


	// Apply the transform
	//
	// Transforming a rectangle returns the the bounding box of
	// the four transformed corners.
	NPointT<T>							Apply(const NPointT<T>     &thePoint);
	NSizeT<T>							Apply(const NSizeT<T>      &theSize);
	NRectangleT<T>						Apply(const NRectangleT<T> &theRectangle);


	// Operators
										operator NFormatArgument(void) const;


public:
	T									a;
	T									b;
	T									c;
	T									d;
	T									tx;
	T									ty;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTransform :	public NEncodable,
					public NTransform32 {
public:
										NENCODABLE_DECLARE(NTransform);

										NTransform(const NTransform32 &theTransform);
										NTransform(const NTransform64 &theTransform);

										NTransform(Float32 a, Float32 b, Float32 c, Float32 d, Float32 tx, Float32 ty);
										NTransform(Float64 a, Float64 b, Float64 c, Float64 d, Float64 tx, Float64 ty);

										NTransform(void);
	virtual							   ~NTransform(void);


	// Operators
										operator NTransform64(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);
};





//============================================================================
//		Template files
//----------------------------------------------------------------------------
#define   NTRANSFORM_CPP
#include "NTransform.cpp"
#undef    NTRANSFORM_CPP









#endif // NTRANSFORM_HDR


