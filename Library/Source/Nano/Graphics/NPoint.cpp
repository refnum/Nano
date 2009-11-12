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
#include "NString.h"
#include "NVector.h"

#ifndef NPOINT_CPP

#include "NEncoder.h"
#include "NPoint.h"





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
//      NPoint::NPoint : Constructor.
//----------------------------------------------------------------------------
NPoint::NPoint(const NPoint32 &thePoint)
		: NPoint32(thePoint.x, thePoint.y)
{
}





//============================================================================
//      NPoint::NPoint : Constructor.
//----------------------------------------------------------------------------
NPoint::NPoint(const NPoint64 &thePoint)
		: NPoint32(thePoint.x, thePoint.y)
{
}





//============================================================================
//      NPoint::NPoint : Constructor.
//----------------------------------------------------------------------------
NPoint::NPoint(Float32 x, Float32 y)
		: NPoint32(x, y)
{
}





//============================================================================
//      NPoint::NPoint : Constructor.
//----------------------------------------------------------------------------
NPoint::NPoint(Float64 x, Float64 y)
		: NPoint32(x, y)
{
}





//============================================================================
//      NPoint::NPoint : Constructor.
//----------------------------------------------------------------------------
NPoint::NPoint(void)
{
}





//============================================================================
//      NPoint::~NPoint : Destructor.
//----------------------------------------------------------------------------
NPoint::~NPoint(void)
{
}





//============================================================================
//		NPoint::NPoint64 : NPoint64 operator.
//----------------------------------------------------------------------------
NPoint::operator NPoint64(void) const
{	NPoint64		theResult;



	// Get the value
	theResult.x = x;
	theResult.y = y;

	return(theResult);
}





//============================================================================
//      NPoint::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
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





#else

//============================================================================
//		NPointT::NPointT : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
template<class T> NPointT<T>::NPointT(T valX, T valY)
{


	// Initialize ourselves
	x = valX;
	y = valY;
}





//============================================================================
//		NPointT::NPointT : Constructor.
//----------------------------------------------------------------------------
template<class T> NPointT<T>::NPointT(void)
{


	// Initialize ourselves
	x = 0;
	y = 0;
}





//============================================================================
//		NPointT::~NPointT : Destructor.
//----------------------------------------------------------------------------
template<class T> NPointT<T>::~NPointT(void)
{
}





//============================================================================
//		NPointT::Clear : Clear the point.
//----------------------------------------------------------------------------
template<class T> void NPointT<T>::Clear(void)
{


	// Clear the point
	x = 0;
	y = 0;
}





//============================================================================
//		NPointT::IsZero : Is the point zero?
//----------------------------------------------------------------------------
template<class T> bool NPointT<T>::IsZero(void) const
{


	// Test the point
	return(NMathUtilities::IsZero(x) && NMathUtilities::IsZero(y));
}





//============================================================================
//		NPointT::Compare : Compare the value.
//----------------------------------------------------------------------------
template<class T> NComparison NPointT<T>::Compare(const NPointT<T> &theValue) const
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
//		NPointT::GetDistance : Get the distance to a point.
//----------------------------------------------------------------------------
template<class T> T NPointT<T>::GetDistance(const NPointT<T> &thePoint, bool getApprox) const
{	T		theDistance;



	// Get the distance
	theDistance = GetDistance2(thePoint);
	
	if (getApprox)
		theDistance = NMathUtilities::FastRoot(theDistance);
	else
		theDistance = sqrt(theDistance);
	
	return(theDistance);
}





//============================================================================
//		NPointT::GetDistance2 : Get the distance^2 to a point.
//----------------------------------------------------------------------------
template<class T> T NPointT<T>::GetDistance2(const NPointT<T> &thePoint) const
{	T	deltaX, deltaY;



	// Get the distance^2
	deltaX = x - thePoint.x;
	deltaY = y - thePoint.y;
	
	return((deltaX * deltaX) + (deltaY * deltaY));
}





//============================================================================
//		NPointT::Offset : Offset the point.
//----------------------------------------------------------------------------
template<class T> void NPointT<T>::Offset(T deltaX, T deltaY)
{


	// Update our state
	*this = GetOffset(deltaX, deltaY);
}





//============================================================================
//		NPointT::GetOffset : Offset the point.
//----------------------------------------------------------------------------
template<class T> NPointT<T> NPointT<T>::GetOffset(T deltaX, T deltaY) const
{	NPointT<T>		theResult;



	// Offset the point
	theResult.x = x + deltaX;
	theResult.y = y + deltaY;
	
	return(theResult);
}





//============================================================================
//		NPointT::+= : Addition operator.
//----------------------------------------------------------------------------
template<class T> const NPointT<T>& NPointT<T>::operator += (const NVectorT<T> &theVector)
{


	// Add the vector
	x += theVector.x;
	y += theVector.y;

	return(*this);
}





//============================================================================
//		NPointT::-= : Subtraction operator.
//----------------------------------------------------------------------------
template<class T> const NPointT<T>& NPointT<T>::operator -= (const NVectorT<T> &theVector)
{


	// Subtract the vector
	x -= theVector.x;
	y -= theVector.y;

	return(*this);
}





//============================================================================
//		NPointT::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
template<class T> NPointT<T>::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{x=%g, y=%g}", x, y);

	return(theResult);
}





#endif // NPOINT_CPP

