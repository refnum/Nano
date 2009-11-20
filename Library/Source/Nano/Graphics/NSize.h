/*	NAME:
		NSize.h

	DESCRIPTION:
		Size object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSIZE_HDR
#define NSIZE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NEncodable.h"
#include "NComparable.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NSize;

template <class T> class NSizeT;

typedef NSizeT<Float32> NSize32;
typedef NSizeT<Float64> NSize64;


// Lists
typedef std::vector<NSize>											NSizeList;
typedef NSizeList::iterator											NSizeListIterator;
typedef NSizeList::const_iterator									NSizeListConstIterator;

typedef std::vector<NSize32>										NSize32List;
typedef NSize32List::iterator										NSize32ListIterator;
typedef NSize32List::const_iterator									NSize32ListConstIterator;

typedef std::vector<NSize64>										NSize64List;
typedef NSize64List::iterator										NSize64ListIterator;
typedef NSize64List::const_iterator									NSize64ListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
template<class T> class NSizeT {
public:
										NSizeT(T width, T height);
										NSizeT(void);


	// Clear the size
	void								Clear(void);


	// Test the size
	bool								IsEmpty(void) const;


	// Compare the value
	NComparison							Compare(const NSizeT<T> &theValue) const;


	// Operators
	NCOMPARABLE_OPERATORS(NSizeT<T>);

										operator NFormatArgument(void) const;


public:
	T									width;
	T									height;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSize :	public NEncodable,
				public NSize32 {
public:
										NENCODABLE_DECLARE(NSize);

										NSize(const NSize32 &theSize);
										NSize(const NSize64 &theSize);

										NSize(Float32 width, Float32 height);
										NSize(Float64 width, Float64 height);

										NSize(void);
	virtual							   ~NSize(void);


	// Operators
										operator NSize64(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);
};





//============================================================================
//		Template files
//----------------------------------------------------------------------------
#define   NSIZE_CPP
#include "NSize.cpp"
#undef    NSIZE_CPP





#endif // NSIZE_HDR


