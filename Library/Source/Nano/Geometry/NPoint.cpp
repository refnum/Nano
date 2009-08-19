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
#include "NVector.h"
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
NENCODABLE_DEFINE(NPoint);





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
//		NPoint::Add : Add a vector to the point.
//----------------------------------------------------------------------------
void NPoint::Add(const NVector &theVector)
{


	// Add the vector
	x += theVector.x;
	y += theVector.y;
}





//============================================================================
//		NPoint::Subtract : Subtract a vector from the point.
//----------------------------------------------------------------------------
void NPoint::Subtract(const NVector &theVector)
{


	// Subtract the vector
	x -= theVector.x;
	y -= theVector.y;
}





//============================================================================
//		NPoint::GetDistance : Get the distance to a point.
//----------------------------------------------------------------------------
Float32 NPoint::GetDistance(const NPoint &thePoint) const
{


	// Get the distance
	return(NMathUtilities::FastRoot(GetDistance2(thePoint)));
}





//============================================================================
//		NPoint::GetDistance2 : Get the distance^2 to a point.
//----------------------------------------------------------------------------
Float32 NPoint::GetDistance2(const NPoint &thePoint) const
{	Float32		deltaX, deltaY;



	// Get the distance^2
	deltaX = x - thePoint.x;
	deltaY = y - thePoint.y;
	
	return((deltaX * deltaX) + (deltaY * deltaY));
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
{


	// Decode the object
	x = theEncoder.DecodeNumber(kNPointXKey).GetFloat32();
	y = theEncoder.DecodeNumber(kNPointYKey).GetFloat32();
}


