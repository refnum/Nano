/*	NAME:
		NBitVector.cpp

	DESCRIPTION:
		Bit vector.
		
		CoreFoundation's CFBitVector class is subject to memory corruption
		bugs prior to Mac OS X 10.5 (rdar://4865533), and so we provide our
		own implementation to support 10.4.
		
		Although std::vector<bool> provides a specialization that uses 1 bit
		per value, that class was not used since it does not provide an
		efficient way to get or set the contents as a block of memory (and
		std::bitset requires a fixed size on creation).
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NBitVector.h"





//============================================================================
//		NBitVector::NBitVector : Constructor.
//----------------------------------------------------------------------------
NBitVector::NBitVector(void)
{


	// Initialize ourselves
	mIsMutable = true;
	
	mSize  = 0;
	mBytes = NULL;
}





//============================================================================
//		NBitVector::~NBitVector : Destructor.
//----------------------------------------------------------------------------
NBitVector::~NBitVector(void)
{
}





//============================================================================
//		NBitVector::IsMutable : Is the vector mutable?
//----------------------------------------------------------------------------
bool NBitVector::IsMutable(void) const
{


	// Get our state
	return(mIsMutable);
}





//============================================================================
//		NBitVector::GetSize : Get the size.
//----------------------------------------------------------------------------
UInt32 NBitVector::GetSize(void) const
{


	// Get the size
	return(mSize);
}





//============================================================================
//		NBitVector::SetSize : Set the vector size.
//----------------------------------------------------------------------------
void NBitVector::SetSize(UInt32 theSize, UInt32 extraCapacity)
{	UInt32	numBytes;
	bool	wasOK;



	// Validate our state
	NN_ASSERT(IsMutable());



	// Set the size
	numBytes = GetBytesForBits(theSize) + GetBytesForBits(extraCapacity);
	wasOK    = mData.SetSize(numBytes);
	NN_ASSERT(wasOK);

	if (wasOK)
		UpdateData(theSize, mData);
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
//		NBitVector::GetBits : Get the bits.
//----------------------------------------------------------------------------
NData NBitVector::GetBits(void) const
{


	// Copy the data
	//
	// If we're an immutable vector created from external data, we create a
	// mutable copy of the data on demand for callers.
	if (!IsMutable() && mData.IsEmpty())
		mData.SetData(GetBytesForBits(GetSize()), mBytes);



	// Get the bis
	//
	// We pre-grow the buffer when appending to minimise resize costs, so the
	// buffer is often larger than the used portion.
	//
	// To ensure our caller only gets the data they need, we shrink on demand.
	mData.SetSize(GetBytesForBits(GetSize()));

	return(mData);
}





//============================================================================
//		NBitVector::SetBits : Set the bits.
//----------------------------------------------------------------------------
void NBitVector::SetBits(bool theValue)
{	UInt32		n, numBits;



	// Set the bits
	numBits = GetSize();
	
	for (n = 0; n < numBits; n++)
		SetBit(n, theValue);
}





//============================================================================
//		NBitVector::SetBits : Set the bits.
//----------------------------------------------------------------------------
void NBitVector::SetBits(const NData &theData, bool copyData)
{	const NData		*dataPtr;



	// Set the bits
	mIsMutable = copyData;
	dataPtr	   = copyData ? &mData : &theData;
	
	if (mIsMutable)
		mData = theData;
	else
		mData.Clear();

	UpdateData(dataPtr->GetSize() * 8, *dataPtr);
}





//============================================================================
//		NBitVector::AppendBit : Append a bit.
//----------------------------------------------------------------------------
void NBitVector::AppendBit(bool theValue)
{


	// Add the bit
	AppendBits(theValue ? 0x01 : 0x00, 1);
}





//============================================================================
//		NBitVector::AppendBits : Append bits.
//----------------------------------------------------------------------------
void NBitVector::AppendBits(UInt32 theValue, UInt32 numBits)
{	UInt32		n, theSize, theMask;



	// Validate our parameters
	NN_ASSERT(numBits >= 1 && numBits <= 32);



	// Get the state we need
	theSize = GetSize();
	theMask = (1 << (numBits-1));



	// Grow the buffer
	//
	// To reduce the cost of repeatedly growing the buffer, we pre-allocate
	// space to ensure most appends do not need to grow.
	SetSize(theSize + numBits, kKilobyte * 8);



	// Append the bits
	for (n = 0; n < numBits;  n++)
		{
		SetBit(theSize + n, (theValue & theMask) != 0);
		theMask >>= 1;
		}
}





//============================================================================
//		NBitVector::FlipBit : Flip a bit.
//----------------------------------------------------------------------------
void NBitVector::FlipBit(CFIndex theIndex)
{


	// Flip the bit
	SetBit(theIndex, !GetBit(theIndex));
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
	lastBit	 = theRange.location + theRange.length;
	
	for (n = firstBit; n <= lastBit; n++)
		SetBit(n, !GetBit(n));
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
	lastBit	 = theRange.location + theRange.length;
	numFound = 0;
	
	for (n = firstBit; n <= lastBit; n++)
		{
		if (GetBit(n) == theValue)
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
	lastBit	 = theRange.location + theRange.length;
	
	for (n = firstBit; n <= lastBit; n++)
		{
		if (GetBit(n) == theValue)
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
	lastBit	 = theRange.location + theRange.length;
	
	for (n = firstBit; n <= lastBit; n++)
		{
		if (GetBit(n) == theValue)
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
	lastBit	 = theRange.location + theRange.length;
	
	for (n = lastBit; n >= firstBit; n--)
		{
		if (n >= 0 && GetBit(n) == theValue)
			return(n);
		}
	
	return(kCFNotFound);
}





//============================================================================
//		NBitVector::UpdateData : Update the data state.
//----------------------------------------------------------------------------
#pragma mark -
void NBitVector::UpdateData(UInt32 numBits, const NData &theData)
{


	// Update our state
	mSize  = numBits;
	mBytes = theData.GetData();
}





//============================================================================
//		NBitVector::GetBytesForBits : Get the number of bytes we need.
//----------------------------------------------------------------------------
UInt32 NBitVector::GetBytesForBits(UInt32 numBits) const
{	UInt32	numBytes;



	// Count the number of bytes
	numBytes = numBits / 8;

	if ((numBytes * 8) < numBits)
		numBytes++;
	
	return(numBytes);
}




