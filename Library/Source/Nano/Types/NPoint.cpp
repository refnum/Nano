/*	NAME:
		NPoint.cpp

	DESCRIPTION:
		Point object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NEncoder.h"
#include "NString.h"
#include "NPoint.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NPoint kNPointZero;





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNPointXKey									= "x";
static const NString kNPointYKey									= "y";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
DEFINE_NENCODABLE(NPoint);





//============================================================================
//		NPoint::NPoint : Constructor.
//----------------------------------------------------------------------------
NPoint::NPoint(Float32 valX, Float32 valY)
{


	// Initialize ourselves
	x = valX;
	y = valY;
}





//============================================================================
//		NPoint::~NPoint : Destructor.
//----------------------------------------------------------------------------
NPoint::~NPoint(void)
{
}





//============================================================================
//		NPoint::Clear : Clear the point.
//----------------------------------------------------------------------------
void NPoint::Clear(void)
{


	// Clear the point
	x = 0.0f;
	y = 0.0f;
}





//============================================================================
//		NPoint::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NPoint::Compare(const NPoint &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(x, theValue.x);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(y, theValue.y);
	
	return(theResult);
}





//============================================================================
//		NPoint::IsZero : Is the point zero?
//----------------------------------------------------------------------------
bool NPoint::IsZero(void) const
{


	// Test the point
	return(NMathUtilities::IsZero(x) && NMathUtilities::IsZero(y));
}





//============================================================================
//		NPoint::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NPoint::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{x=%g, y=%g}", x, y);

	return(theResult);
}





//============================================================================
//      NPoint::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
#pragma mark -
void NPoint::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNPointXKey, x);
	theEncoder.EncodeNumber(kNPointYKey, y);
}





//============================================================================
//      NPoint::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NPoint::DecodeSelf(const NEncoder &theEncoder)
{	bool	gotValue;



	// Decode the object
	gotValue = theEncoder.DecodeNumber(kNPointXKey).GetValueFloat32(x);
	NN_ASSERT(gotValue);

	gotValue = theEncoder.DecodeNumber(kNPointYKey).GetValueFloat32(y);
	NN_ASSERT(gotValue);
}


