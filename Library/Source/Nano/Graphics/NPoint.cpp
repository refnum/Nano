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
//		Internal class declaration
//----------------------------------------------------------------------------
class NPointX :	public NEncodable {
public:
										NENCODABLE_DECLARE(NPointX);

										NPointX(const NNumber &x, const NNumber &y);
										NPointX(void);
	virtual							   ~NPointX(void);


protected:
	// Encode the object
	void								EncodeSelf(NEncoder &theEncoder) const;


private:
	NNumber								mX;
	NNumber								mY;
};





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_NODECODE(NPointX);





//============================================================================
//      NPointX::NPointX : Constructor.
//----------------------------------------------------------------------------
NPointX::NPointX(const NNumber &x, const NNumber &y)
{


	// Initialise ourselves
	mX = x;
	mY = y;
}





//============================================================================
//      NPointX::NPointX : Constructor.
//----------------------------------------------------------------------------
NPointX::NPointX()
{
}





//============================================================================
//      NPointX::~NPointX : Destructor.
//----------------------------------------------------------------------------
NPointX::~NPointX(void)
{
}





//============================================================================
//      NPointX::EncodableGetDecoded : Get a decoded object.
//----------------------------------------------------------------------------
NVariant NPointX::EncodableGetDecoded(const NEncoder &theEncoder)
{


	// Decode the object
	mX = theEncoder.DecodeNumber(kNPointXKey);
	mY = theEncoder.DecodeNumber(kNPointYKey);
	
	if (mX.GetPrecision() == kNPrecisionFloat64 || mY.GetPrecision() == kNPrecisionFloat64)
		return(NPoint64(mX.GetFloat64(), mY.GetFloat64()));
	else
		return(NPoint32(mX.GetFloat32(), mY.GetFloat32()));
}





//============================================================================
//      NPointX::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NPointX::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNPointXKey, mX);
	theEncoder.EncodeNumber(kNPointYKey, mY);
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
		theDistance = (T) NMathUtilities::FastRoot((Float32) theDistance);
	else
		theDistance = (T) sqrt(theDistance);
	
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
//		NPointT::NEncodable : NEncodable operator.
//----------------------------------------------------------------------------
template<class T> NPointT<T>::operator NEncodable(void) const
{	NPointX		theResult(x, y);



	// Get the value
	return(theResult);
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

