/*	NAME:
		NTransform.h

	DESCRIPTION:
		Transform object.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
#include "NComparable.h"
#include "NEncodable.h"
#include "NVariant.h"
#include "NRectangle.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
template <class T> class NTransformT;

typedef NTransformT<Float64> NTransform64;
typedef NTransformT<Float32> NTransform32;
typedef NTransform32         NTransform;


// Lists
typedef std::vector<NTransform64>									NTransform64List;
typedef NTransform64List::iterator									NTransform64ListIterator;
typedef NTransform64List::const_iterator							NTransform64ListConstIterator;

typedef std::vector<NTransform32>									NTransform32List;
typedef NTransform32List::iterator									NTransform32ListIterator;
typedef NTransform32List::const_iterator							NTransform32ListConstIterator;

typedef std::vector<NTransform>										NTransformList;
typedef NTransformList::iterator									NTransformListIterator;
typedef NTransformList::const_iterator								NTransformListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template<class T> class NTransformT {
public:
										NTransformT(const NVariant &theValue);

										NTransformT(const std::vector<T> &matrix23);
										NTransformT(T a, T b, T c, T d, T tx, T ty);

										NTransformT(void);


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
	NCOMPARABLE_OPERATORS(NPointT<T>);

										operator NEncodable(     void) const;
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
//		Inline functions
//----------------------------------------------------------------------------
inline NTransform NTransformTranslate(Float32 x, Float32 y)
{	NTransform	theTransform;
	
	theTransform.SetTranslate(x, y);
	return(theTransform);
}

inline NTransform NTransformScale(Float32 x, Float32 y)
{	NTransform	theTransform;
	
	theTransform.SetScale(x, y);
	return(theTransform);
}

inline NTransform NTransformRotate(NRadians angle)
{	NTransform	theTransform;
	
	theTransform.SetRotate(angle);
	return(theTransform);
}





//============================================================================
//		Template files
//----------------------------------------------------------------------------
#define   NTRANSFORM_CPP
#include "NTransform.cpp"
#undef    NTRANSFORM_CPP









#endif // NTRANSFORM_HDR


