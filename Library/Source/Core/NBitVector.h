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
#include "NContainer.h"
#include "NData.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NBitVector : public NContainer {
public:
										NBitVector(void);
	virtual								~NBitVector(void);


	// Is the vector mutable?
	bool								IsMutable(void) const;
	

	// Get/set the vector size
	//
	// Increasing the size will append 0'd bits to the vector.
	//
	// To amortize the cost of repeatedly growing a bit vector, extra capacity can be requested
	// that may reserve additional storage for at least that many extra bits (although these
	// bits can not be accessed until a subsequent SetSize increases the vector size).
	UInt32								GetSize(void) const;
	void								SetSize(UInt32 theSize, UInt32 extraCapacity=0);


	// Clear the vector
	void								Clear(void);


	// Get/set a bit
	inline bool							GetBit(CFIndex theIndex) const;
	inline void							SetBit(CFIndex theIndex, bool theValue);


	// Get/set the bits
	//
	// Bits are returned in the least-most-significant numBits bits of the result.
	//
	// When data is assigned, it can be copied into the vector or used as-is. If
	// the data is not copied, the vector forms an immutable view of those bits.
	NData								GetBits(void)							  const;
	inline UInt32						GetBits(CFIndex theIndex, UInt32 numBits) const;

	void								SetBits(bool		 theValue);
	void								SetBits(const NData &theData, bool copyData=true);


	// Append bits
	//
	// Bits are read from the least-most-significant numBits of the value.
	void								AppendBit( bool	  theValue);
	void								AppendBits(UInt32 theValue, UInt32 numBits);


	// Flip the bits
	void								FlipBit(CFIndex theIndex);
	void								FlipBits(const CFRange &theRange=kCFRangeAll);


	// Test the bits
	CFIndex								CountBits(	bool theValue, const CFRange &theRange=kCFRangeAll) const;
	bool								ContainsBit(bool theValue, const CFRange &theRange=kCFRangeAll) const;


	// Find bits
	CFIndex								FindFirstBit(bool theValue, const CFRange &theRange=kCFRangeAll) const;
	CFIndex								FindLastBit( bool theValue, const CFRange &theRange=kCFRangeAll) const;


private:
	void								UpdateData(     UInt32 numBits, const NData &theData);

	UInt32								GetBytesForBits(UInt32 numBits)                     const;
	inline const UInt8					*GetByteForBit(CFIndex theIndex, CFIndex &bitIndex) const;


private:
	bool								mIsMutable;
	mutable NData						mData;

	UInt32								mSize;
	const UInt8							*mBytes;
};





//============================================================================
//		NBitVector::GetByteForBit : Get the byte that contains a bit.
//----------------------------------------------------------------------------
//		Note : Inlined for performance.
//----------------------------------------------------------------------------
inline const UInt8 *NBitVector::GetByteForBit(CFIndex theIndex, CFIndex &bitIndex) const
{	UInt32		byteIndex;



	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < (CFIndex) GetSize());



	// Get the byte
	//
	// The bit index as the index within the byte, not the vector.
	byteIndex = theIndex / 8;
	bitIndex  = 7 - (theIndex - (byteIndex * 8));
	
	return(&mBytes[byteIndex]);
}





//============================================================================
//		NBitVector::GetBit : Get a bit.
//----------------------------------------------------------------------------
//		Note : Inlined for performance.
//----------------------------------------------------------------------------
inline bool NBitVector::GetBit(CFIndex theIndex) const
{	CFIndex		bitIndex;
	UInt8		theByte;



	// Get the bit
	theByte = *GetByteForBit(theIndex, bitIndex);
	
	return((theByte >> bitIndex) & 0x01);
}





//============================================================================
//		NBitVector::GetBits : Get the bits.
//----------------------------------------------------------------------------
//		Note : Inlined for performance.
//----------------------------------------------------------------------------
inline UInt32 NBitVector::GetBits(CFIndex theIndex, UInt32 numBits) const
{	UInt32		n, theResult, theMask;



	// Validate our parameters
	NN_ASSERT((theIndex + numBits) <= GetSize());
	NN_ASSERT(numBits >= 1 && numBits <= 32);



	// Get the bits
	theResult = 0;
	theMask	  = (1 << (numBits-1));

	for (n = 0; n < numBits; n++)
		{
		if (GetBit(theIndex + n))
			theResult |= theMask;

		theMask >>= 1;
		}

	return(theResult);
}





//============================================================================
//		NBitVector::SetBit : Set a bit.
//----------------------------------------------------------------------------
//		Note : Inlined for performance.
//----------------------------------------------------------------------------
inline void NBitVector::SetBit(CFIndex theIndex, bool theValue)
{	CFIndex		bitIndex;
	UInt8		*bytePtr;



	// Validate our state
	NN_ASSERT(IsMutable());
	


	// Set the bit
	bytePtr	  = (UInt8 *) GetByteForBit(theIndex, bitIndex);
	*bytePtr &= ~(1 << bitIndex);

	if (theValue)
		*bytePtr |= (1 << bitIndex);
}





#endif // NBITVECTOR_HDR


