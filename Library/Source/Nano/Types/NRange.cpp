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
//		Public constants
//----------------------------------------------------------------------------
const NRange kNRangeNone(kNIndexNone, 0);
const NRange kNRangeAll (0, kNIndexNone);





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


	// Get the value
	return(mLocation);
}





//============================================================================
//		NRange::GetLast : Get the last item.
//----------------------------------------------------------------------------
NIndex NRange::GetLast(void) const
{


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


	// Get the value
	return(mLocation + mSize);
}





//============================================================================
//		NRange::GetNormalized : Get a normalized range.
//----------------------------------------------------------------------------
NRange NRange::GetNormalized(NIndex theSize) const
{	NRange		theResult;



	// Convert the size
	theResult = *this;
	
	if (theResult.mSize == kNIndexNone)
		theResult.mSize = theSize;



	// Check for start beyond end
	if (theResult.GetFirst() >= theSize)
		theResult = kNRangeNone;



	// Check for last beyond end
	if (theResult.GetLast() >= theSize)
		theResult.SetSize(theSize - theResult.GetLocation());

	return(theResult);
}





//============================================================================
//		NRange::GetUnion : Get the union of a range.
//----------------------------------------------------------------------------
NRange NRange::GetUnion(const NRange &theRange) const
{	NIndex		rangeFirst, rangeLast;



	// Get the union
	rangeFirst = std::min(GetFirst(), theRange.GetFirst());
	rangeLast  = std::max(GetLast(),  theRange.GetLast());
	
	return(NRange(rangeFirst, rangeLast - rangeFirst + 1));
}





//============================================================================
//		NRange::GetIntersection : Get the intersection of a range.
//----------------------------------------------------------------------------
NRange NRange::GetIntersection(const NRange &theRange) const
{	NIndex		rangeFirst, rangeLast;



	// Check our parameters
	if (GetLast() < theRange.GetLocation() || theRange.GetLast() < GetLocation())
		return(kNRangeNone);



	// Get the intersection
	rangeFirst = std::max(GetFirst(), theRange.GetFirst());
	rangeLast  = std::min(GetLast(),  theRange.GetLast());
	
	return(NRange(rangeFirst, rangeLast - rangeFirst + 1));
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
//		NRange::Overlaps : Does the range overlap another?
//----------------------------------------------------------------------------
bool NRange::Overlaps(const NRange &theRange) const
{


	// Check for overlap
    return(!GetIntersection(theRange).IsEmpty());
}





//============================================================================
//		NRange::Contains : Does the range contain another?
//----------------------------------------------------------------------------
bool NRange::Contains(NIndex theIndex) const
{


	// Check for containment
    return(theIndex >= GetFirst() && theIndex <= GetLast());
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


