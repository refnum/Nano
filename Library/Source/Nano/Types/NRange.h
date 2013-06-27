/*	NAME:
		NRange.h

	DESCRIPTION:
		Range object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NRANGE_HDR
#define NRANGE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NComparable.h"
#include "NEncodable.h"
#include "NContainer.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Meta-ranges
//
// Declared as #defines to allow their use during static initialisation.
#define kNRangeNone													NRange(kNIndexNone, 0)
#define kNRangeAll													NRange(0, kNIndexNone)





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef std::vector<NRange>											NRangeList;
typedef NRangeList::iterator										NRangeListIterator;
typedef NRangeList::const_iterator									NRangeListConstIterator;
typedef NRangeList::reverse_iterator								NRangeListReverseIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NRange : public NEncodable {
public:
										NENCODABLE_DECLARE(NRange);

										NRange(NIndex theLocation=0, NIndex theSize=0);
	virtual							   ~NRange(void);


	// Compare the value
	NComparison							Compare(const NRange &theValue) const;


	// Get/set the location
	NIndex								GetLocation(void) const;
	void								SetLocation(NIndex theValue);


	// Get/set the size
	NIndex								GetSize(void) const;
	void								SetSize(NIndex theValue);


	// Get the limits
	//
	// A range has a location and a size: the first and last items are items
	// which lie in the range, while the "next item" is the item immediately
	// following the range.
	//
	// The last item in an empty range is the same as the first: the range has
	// no content, but still has a location.
	//
	// May only be called on a normalized range.
	NIndex								GetFirst(void) const;
	NIndex								GetLast( void) const;
	NIndex								GetNext( void) const;


	// Get an offset
	//
	// May only be called on a normalized range.
	NIndex								GetOffset(NIndex theOffset) const;
	

	// Get a normalized range
	//
	// Normalizing a range returns a range without meta-values:
	//
	//		kNRangeNone			0..0
	//		kNRangeAll			0..theSize
	//		N..kNIndexNone		N..end
	//
	// A range whose size is kNIndexNone is treated as a range that extends
	// to the end of the implied 0..theSize range.
	//
	// If the range is outside the implied 0..theSize range, the returned
	// range has its original location and a size of 0.
	//
	// A range that exceeds the implied 0..theSize range is clamped to the
	// maximum available size.
	NRange								GetNormalized(NIndex theSize) const;


	// Get a union
	//
	// May only be called on a normalized range.
	NRange								GetUnion(       const NRange &theRange) const;
	NRange								GetIntersection(const NRange &theRange) const;


	// Test the range
	bool								IsEmpty(void) const;
	bool								IsMeta( void) const;


	// Does a range overlap another?
	//
	// May only be called on a normalized range, or kNRangeNone
	bool								Overlaps(const NRange &theRange)  const;


	// Does a range contain another?
	//
	// May only be called on a normalized range, or kNRangeNone
	bool								Contains(NIndex theOffset) const;


	// Operators
	NCOMPARABLE_OPERATORS(NRange)

										operator NFormatArgument(void) const;


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NIndex								mLocation;
	NIndex								mSize;
};






#endif // NRANGE_HDR


