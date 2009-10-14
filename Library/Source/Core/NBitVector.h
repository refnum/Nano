/*	NAME:
		NBitVector.h

	DESCRIPTION:
		Bit vector.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NBITVECTOR_HDR
#define NBITVECTOR_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NData.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef std::vector<bool>									BoolList;
typedef BoolList::iterator									BoolListIterator;
typedef BoolList::const_iterator							BoolListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NBitVector {
public:
										NBitVector(void);
	virtual								~NBitVector(void);


	// Is the vector empty?
	bool								IsEmpty(   void) const;
	bool								IsNotEmpty(void) const;


	// Get/set the vector size
	//
	// Increasing the size will append 0'd bits to the vector.
	UInt32								GetSize(void) const;
	void								SetSize(UInt32 theSize);


	// Clear the vector
	void								Clear(void);


	// Get/set a bit
	bool								GetBit(CFIndex theIndex) const;
	void								SetBit(CFIndex theIndex, bool theValue);


	// Get/set the bits
	//
	// Bits are returned in the least-most-significant bits of the result, from most to least significant.
	NData								GetBits(void)                             const;
	UInt32								GetBits(CFIndex theIndex, UInt32 numBits) const;

	void								SetBits(const NData &theBits, UInt32 numBits);
	void								SetBits(bool theValue);


	// Append bits
	//
	// Bits are read from the least-most-significant bits of the value, from most to least significant.
	void								AppendBit(bool theValue);
	void								AppendBits(UInt32 theBits, UInt32 numBits);


	// Flip the bits
	void								FlipBit(CFIndex theIndex);
	void								FlipBits(const CFRange &theRange=kCFRangeAll);


	// Test the bits
	CFIndex								CountBits(  bool theValue, const CFRange &theRange=kCFRangeAll) const;
	bool								ContainsBit(bool theValue, const CFRange &theRange=kCFRangeAll) const;


	// Find bits
	CFIndex								FindFirstBit(bool theValue, const CFRange &theRange=kCFRangeAll) const;
	CFIndex								FindLastBit( bool theValue, const CFRange &theRange=kCFRangeAll) const;


private:
	BoolList							mBits;
};






#endif // NBITVECTOR_HDR


