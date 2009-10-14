/*	NAME:
		NBitVector.cpp

	DESCRIPTION:
		Bit vector.
		
		CoreFoundation's CFBitVector class is subject to memory corruption
		bugs prior to Mac OS X 10.5 (rdar://4865533), and so bit vectors are
		implemented with std::vector<bool>.
		
		Although a bool is larger than a bit, this class is required to provide
		a specialization for this type that uses 1 bit per value.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBitVector.h"





//============================================================================
//		NBitVector::NBitVector : Constructor.
//----------------------------------------------------------------------------
NBitVector::NBitVector(void)
{
}





//============================================================================
//		NBitVector::~NBitVector : Destructor.
//----------------------------------------------------------------------------
NBitVector::~NBitVector(void)
{
}





//============================================================================
//		NBitVector::IsEmpty : Is the vector empty?
//----------------------------------------------------------------------------
bool NBitVector::IsEmpty(void) const
{


	// Check our size
	return(GetSize() == 0);
}





//============================================================================
//		NBitVector::IsNotEmpty : Is the vector not empty?
//----------------------------------------------------------------------------
bool NBitVector::IsNotEmpty(void) const
{


	// Check our size
	return(GetSize() != 0);
}





//============================================================================
//		NBitVector::GetSize : Get the vector size.
//----------------------------------------------------------------------------
UInt32 NBitVector::GetSize(void) const
{


	// Get our size
	return(mBits.size());
}





//============================================================================
//		NBitVector::SetSize : Set the vector size.
//----------------------------------------------------------------------------
void NBitVector::SetSize(UInt32 theSize)
{


	// Set our size
	mBits.resize(theSize, false);
}





//============================================================================
//		NBitVector::Clear : Clear the vector.
//----------------------------------------------------------------------------
void NBitVector::Clear(void)
{


	// Clear the vector
	SetSize(0);
}





//============================================================================
//		NBitVector::GetBit : Get a bit.
//----------------------------------------------------------------------------
bool NBitVector::GetBit(CFIndex theIndex) const
{


	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < (CFIndex) GetSize());



	// Get the bit
	return(mBits[theIndex]);
}





//============================================================================
//		NBitVector::SetBit : Set a bit.
//----------------------------------------------------------------------------
void NBitVector::SetBit(CFIndex theIndex, bool theValue)
{


	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < (CFIndex) GetSize());



	// Set the bit
	mBits[theIndex] = theValue;
}





//============================================================================
//		NBitVector::GetBits : Get the bits.
//----------------------------------------------------------------------------
NData NBitVector::GetBits(void) const
{	UInt32		n, numBits, numBytes;
	UInt8		*bytePtr;
	NData		theData;



	// Get the state we need
	numBits  = mBits.size();
	numBytes = (numBits / 8);
	
	if ((numBytes * 8) != numBits)
		numBytes++;
	
	if (!theData.SetSize(numBytes))
		return(theData);

	bytePtr = theData.GetData();



	// Get the bits
	//
	// Bits are extracted as bytes, with potentially unused top bits in the last byte.
	for (n = 0; n < numBits; n += 8)
		*bytePtr++ = GetBits(n, std::min(numBits - n, 8UL));

	return(theData);
}





//============================================================================
//      NBitVector::GetBits : Get the bits.
//----------------------------------------------------------------------------
UInt32 NBitVector::GetBits(CFIndex theIndex, UInt32 numBits) const
{	UInt32		n, theResult, theMask;



	// Validate our parameters
	NN_ASSERT(numBits >= 1 && numBits <= 32);



	// Get the bits
	theResult = 0;
	theMask   = (1 << (numBits-1));

	for (n = 0; n < numBits; n++)
		{
		if (GetBit(theIndex + n))
			theResult |= theMask;

		theMask >>= 1;
		}

	return(theResult);
}





//============================================================================
//		NBitVector::SetBits : Set the bits.
//----------------------------------------------------------------------------
void NBitVector::SetBits(const NData &theBits, UInt32 numBits)
{	const UInt8		*bytePtr;
	UInt32			n;



	// Get the state we need
	bytePtr = theBits.GetData();



	// Set the bits
	//
	// Bits are extracted as bytes, with potentially unused top bits in the last byte.
	for (n = 0; n < numBits; n += 8)
		AppendBits(*bytePtr++, std::min(numBits - n, 8UL));
}





//============================================================================
//		NBitVector::SetBits : Set the bits.
//----------------------------------------------------------------------------
void NBitVector::SetBits(bool theValue)
{	UInt32		theSize;



	// Set the bits
	theSize = GetSize();
	
	mBits.clear();
	mBits.resize(theSize, theValue);
}





//============================================================================
//      NBitVector::AppendBit : Append a bit.
//----------------------------------------------------------------------------
void NBitVector::AppendBit(bool theValue)
{


	// Add the bit
	AppendBits(theValue ? 0x01 : 0x00, 1);
}





//============================================================================
//      NBitVector::AppendBits : Append bits.
//----------------------------------------------------------------------------
void NBitVector::AppendBits(UInt32 theBits, UInt32 numBits)
{	UInt32		n, theMask;



	// Validate our parameters
	NN_ASSERT(numBits >= 1 && numBits <= 32);



	// Set the values
	theMask = (1 << (numBits-1));

	for (n = 0; n < numBits;  n++)
		{
		mBits.push_back((theBits & theMask) != 0);
		theMask >>= 1;
		}
}





//============================================================================
//		NBitVector::FlipBit : Flip a bit.
//----------------------------------------------------------------------------
void NBitVector::FlipBit(CFIndex theIndex)
{


	// Validate our parameters
	NN_ASSERT(theIndex >= 0 && theIndex < (CFIndex) GetSize());



	// Flip the bit
	mBits[theIndex] = !mBits[theIndex];
}





//============================================================================
//		NBitVector::FlipBits : Flip the bits.
//----------------------------------------------------------------------------
void NBitVector::FlipBits(const CFRange &theRange)
{	CFIndex		n, firstBit, lastBit;



	// Validate our parameters
	NN_ASSERT(theRange.location >= 0 && (theRange.location+theRange.length) < (CFIndex) GetSize());



	// Flip the bits
	firstBit = theRange.location;
	lastBit  = theRange.location + theRange.length;
	
	for (n = firstBit; n <= lastBit; n++)
		mBits[n] = !mBits[n];
}





//============================================================================
//		NBitVector::CountBits : Count the bits with a specific value.
//----------------------------------------------------------------------------
CFIndex NBitVector::CountBits(bool theValue, const CFRange &theRange) const
{	CFIndex		n, firstBit, lastBit, numFound;



	// Validate our parameters
	NN_ASSERT(theRange.location >= 0 && (theRange.location+theRange.length) < (CFIndex) GetSize());



	// Count the bits
	firstBit = theRange.location;
	lastBit  = theRange.location + theRange.length;
	numFound = 0;
	
	for (n = firstBit; n <= lastBit; n++)
		{
		if (mBits[n] == theValue)
			numFound++;
		}
	
	return(numFound);
}





//============================================================================
//		NBitVector::ContainsBit : Does a range contain a value?
//----------------------------------------------------------------------------
bool NBitVector::ContainsBit(bool theValue, const CFRange &theRange) const
{	CFIndex		n, firstBit, lastBit;



	// Validate our parameters
	NN_ASSERT(theRange.location >= 0 && (theRange.location+theRange.length) < (CFIndex) GetSize());



	// Test the bits
	firstBit = theRange.location;
	lastBit  = theRange.location + theRange.length;
	
	for (n = firstBit; n <= lastBit; n++)
		{
		if (mBits[n] == theValue)
			return(true);
		}
	
	return(false);
}





//============================================================================
//		NBitVector::FindFirstBit : Find the first bit with a specific value.
//----------------------------------------------------------------------------
CFIndex NBitVector::FindFirstBit(bool theValue, const CFRange &theRange) const
{	CFIndex		n, firstBit, lastBit;



	// Validate our parameters
	NN_ASSERT(theRange.location >= 0 && (theRange.location+theRange.length) < (CFIndex) GetSize());



	// Find the bit
	firstBit = theRange.location;
	lastBit  = theRange.location + theRange.length;
	
	for (n = firstBit; n <= lastBit; n++)
		{
		if (mBits[n] == theValue)
			return(n);
		}
	
	return(kCFNotFound);
}





//============================================================================
//		NBitVector::FindLastBit : Find the last bit with a specific value.
//----------------------------------------------------------------------------
CFIndex NBitVector::FindLastBit(bool theValue, const CFRange &theRange) const
{	CFIndex		n, firstBit, lastBit;



	// Validate our parameters
	NN_ASSERT(theRange.location >= 0 && (theRange.location+theRange.length) < (CFIndex) GetSize());



	// Find the bit
	firstBit = theRange.location;
	lastBit  = theRange.location + theRange.length;
	
	for (n = lastBit; n >= firstBit; n--)
		{
		if (n >= 0 && mBits[n] == theValue)
			return(n);
		}
	
	return(kCFNotFound);
}


