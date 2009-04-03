/*	NAME:
		NRange.h

	DESCRIPTION:
		Range object.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NRANGE_HDR
#define NRANGE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NComparable.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
class NRange;

extern const NRange kNRangeNone;
extern const NRange kNRangeAll;





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
class NRange : public NComparable<NRange> {
public:
										 NRange(NIndex theLocation=0, NIndex theSize=0);
	virtual								~NRange(void);


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
	NIndex								GetFirst(void) const;
	NIndex								GetLast( void) const;
	NIndex								GetNext( void) const;


	// Get a normalized range
	//
	// A normalized range converts meta-range into a range for a specific size.
	NRange								GetNormalized(NIndex theSize) const;
	

	// Get a modified range
	NRange								GetUnion(       const NRange &theRange) const;
	NRange								GetIntersection(const NRange &theRange) const;


	// Test the range
	bool								IsEmpty(   void)                 const;
	bool								IsNotEmpty(void)                 const;

	bool								Overlaps(const NRange &theRange) const;
	bool								Contains(NIndex        theIndex) const;


private:
	NIndex								mLocation;
	NIndex								mSize;
};




#endif // NRANGE_HDR


