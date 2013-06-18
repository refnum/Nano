/*	NAME:
		NRange.cpp

	DESCRIPTION:
		Range object.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NEncoder.h"
#include "NString.h"
#include "NRange.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNRangeLocationKey								= "location";
static const NString kNRangeSizeKey									= "size";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NRange);





//============================================================================
//		NRange::NRange : Constructor.
//----------------------------------------------------------------------------
NRange::NRange(NIndex theLocation, NIndex theSize)
{


	// Initialise ourselves
	mLocation = theLocation;
	mSize     = theSize;
}





//============================================================================
//		NRange::~NRange : Destructor.
//----------------------------------------------------------------------------
NRange::~NRange(void)
{
}





//============================================================================
//		NRange::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NRange::Compare(const NRange &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(mSize, theValue.mSize);

	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(mLocation, theValue.mLocation);

	return(theResult);
}





//============================================================================
//		NRange::GetLocation : Get the location.
//----------------------------------------------------------------------------
NIndex NRange::GetLocation(void) const
{


	// Get the value
	return(mLocation);
}





//============================================================================
//		NRange::SetLocation : Set the location.
//----------------------------------------------------------------------------
void NRange::SetLocation(NIndex theValue)
{


	// Set the value
	mLocation = theValue;
}





//============================================================================
//		NRange::GetSize : Get the size.
//----------------------------------------------------------------------------
NIndex NRange::GetSize(void) const
{


	// Get the value
	return(mSize);
}





//============================================================================
//		NRange::SetSize : Set the size.
//----------------------------------------------------------------------------
void NRange::SetSize(NIndex theValue)
{


	// Set the value
	mSize = theValue;
}





//============================================================================
//		NRange::GetFirst : Get the first item.
//----------------------------------------------------------------------------
NIndex NRange::GetFirst(void) const
{


	// Validate our state
	NN_ASSERT(!IsMeta());



	// Get the value
	return(mLocation);
}





//============================================================================
//		NRange::GetLast : Get the last item.
//----------------------------------------------------------------------------
NIndex NRange::GetLast(void) const
{


	// Validate our state
	NN_ASSERT(!IsMeta());



	// Get the value
	if (mSize == 0)
		return(mLocation);
	else
		return(mLocation + mSize - 1);
}





//============================================================================
//		NRange::GetNext : Get the subsequent item.
//----------------------------------------------------------------------------
NIndex NRange::GetNext(void) const
{


	// Validate our state
	NN_ASSERT(!IsMeta());



	// Get the value
	return(mLocation + mSize);
}





//============================================================================
//		NRange::GetOffset : Get an offset.
//----------------------------------------------------------------------------
NIndex NRange::GetOffset(NIndex theOffset) const
{


	// Validate our state
	NN_ASSERT(!IsMeta());



	// Get the offset
	return(mLocation + theOffset);
}





//============================================================================
//		NRange::GetNormalized : Get a normalized range.
//----------------------------------------------------------------------------
NRange NRange::GetNormalized(NIndex theSize) const
{	NRange		theResult;



	// Get the state we need
	theResult = *this;



	// Normalize meta-values
	if (theResult == kNRangeNone)
		theResult = NRange(0, 0);

	else if (theResult == kNRangeAll)
		theResult = NRange(0, theSize);



	// Normalize the range
	//
	// Non-meta ranges are normalised relative to the implicit 0..theSize range.
	//
	// A range outside that range is returned with its current position, but with
	// a size of 0 (since it is essentially empty).
	//
	// A range within that range with a meta-length is expanded to the available size.
	//
	// A range within that range which exceeds the size is clamped to the available size.
	else if (mLocation >= theSize || theSize == 0)
		theResult.SetSize(0);
	
	else if (theResult.GetSize() == kNIndexNone || theResult.GetLast() >= theSize)
		theResult.SetSize(theSize - theResult.mLocation);

	return(theResult);
}





//============================================================================
//		NRange::GetUnion : Get the union of a range.
//----------------------------------------------------------------------------
NRange NRange::GetUnion(const NRange &theRange) const
{	NIndex		rangeFirst, rangeLast;
	NRange		theResult;



	// Validate our parameters and state
	NN_ASSERT(!theRange.IsMeta());
	NN_ASSERT(!IsMeta());



	// Check for empty
	if (IsEmpty())
		return(theRange);
	
	if (theRange.IsEmpty())
		return(*this);



	// Get the union
	rangeFirst = std::min(GetFirst(), theRange.GetFirst());
	rangeLast  = std::max(GetLast(),  theRange.GetLast());
	theResult  = NRange(rangeFirst, rangeLast - rangeFirst + 1);
	
	return(theResult);
}





//============================================================================
//		NRange::GetIntersection : Get the intersection of a range.
//----------------------------------------------------------------------------
NRange NRange::GetIntersection(const NRange &theRange) const
{	NIndex		rangeFirst, rangeLast;
	NRange		theResult;



	// Validate our parameters and state
	NN_ASSERT(!theRange.IsMeta());
	NN_ASSERT(!IsMeta());



	// Check for empty
	if (IsEmpty() || theRange.IsEmpty())
		return(theResult);



	// Check for no intersection
	if (theRange.GetFirst() > GetLast() || theRange.GetLast() < GetFirst())
		return(theResult);
	
	
	
	// Get the intersection
	rangeFirst = std::max(GetFirst(), theRange.GetFirst());
	rangeLast  = std::min(GetLast(),  theRange.GetLast());
	theResult  = NRange(rangeFirst, rangeLast - rangeFirst + 1);
	
	return(theResult);
}





//============================================================================
//		NRange::IsEmpty : Is the range empty?
//----------------------------------------------------------------------------
bool NRange::IsEmpty(void) const
{


	// Check our state
	return(mSize == 0);
}





//============================================================================
//		NRange::IsMeta : Is the range a meta-range?
//----------------------------------------------------------------------------
bool NRange::IsMeta(void) const
{


	// Check our state
	return(mLocation == kNIndexNone || mSize == kNIndexNone);
}





//============================================================================
//		NRange::Overlaps : Does the range overlap another?
//----------------------------------------------------------------------------
bool NRange::Overlaps(const NRange &theRange) const
{	bool	hasOverlap;



	// Validate our parameters and state
	//
	// kNRangeNone is the only supported meta range, as it is empty.
	NN_ASSERT(!theRange.IsMeta());
	NN_ASSERT(*this == kNRangeNone || !IsMeta());



	// Check for overlap
	hasOverlap = (!IsEmpty() && !theRange.IsEmpty());
	
	if (hasOverlap)
		hasOverlap = !GetIntersection(theRange).IsEmpty();
	
	return(hasOverlap);
}





//============================================================================
//		NRange::Contains : Does the range contain an offset?
//----------------------------------------------------------------------------
bool NRange::Contains(NIndex theOffset) const
{	bool	hasOffset;



	// Validate our state
	//
	// kNRangeNone is the only supported meta range, as it is empty.
	NN_ASSERT(*this == kNRangeNone || !IsMeta());



	// Check for containment
	hasOffset = !IsEmpty();

	if (hasOffset)
		hasOffset = (theOffset >= GetFirst() && theOffset <= GetLast());
	
	return(hasOffset);
}





//============================================================================
//		NRange::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NRange::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{l=%ld, s=%ld}", mLocation, mSize);

	return(theResult);
}





#pragma mark protected
//============================================================================
//      NRange::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NRange::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNRangeLocationKey, (SInt64) mLocation);
	theEncoder.EncodeNumber(kNRangeSizeKey,     (SInt64) mSize);
}





//============================================================================
//      NRange::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NRange::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	mLocation = theEncoder.DecodeNumber(kNRangeLocationKey).GetSInt32();
	mSize     = theEncoder.DecodeNumber(kNRangeSizeKey).GetSInt32();
}


