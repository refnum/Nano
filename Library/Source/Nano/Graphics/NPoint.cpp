/*	NAME:
		NPoint.cpp

	DESCRIPTION:
		Point object.

		NEncodable support uses a helper class to avoid a v-table.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
#endif

#ifdef NPOINT_CPP





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
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NNumber								mX;
	NNumber								mY;
};





//============================================================================
//		NPointT::NPointT : Constructor.
//----------------------------------------------------------------------------
template<class T> NPointT<T>::NPointT(const NVariant &theValue)
{	NPoint64	point64;
	NPoint32	point32;
	NPoint		point;



	// Initialize ourselves
	NPointX::EncodableRegister();
	
	if (theValue.GetValue(point64))
		{
		x = (T) point64.x;
		y = (T) point64.y;
		}

	else if (theValue.GetValue(point32))
		{
		x = (T) point32.x;
		y = (T) point32.y;
		}

	else if (theValue.GetValue(point))
		{
		x = (T) point.x;
		y = (T) point.y;
		}
	
	else
		NN_LOG("Unknown type!");
}





//============================================================================
//		NPointT::NPointT : Constructor.
//----------------------------------------------------------------------------
template<class T> NPointT<T>::NPointT(T valX, T valY)
{


	// Initialize ourselves
	NPointX::EncodableRegister();
	
	x = valX;
	y = valY;
}





//============================================================================
//		NPointT::NPointT : Constructor.
//----------------------------------------------------------------------------
template<class T> NPointT<T>::NPointT(void)
{


	// Initialize ourselves
	NPointX::EncodableRegister();
	
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
//		NPointT::MakeIntegral : Make the point integral.
//----------------------------------------------------------------------------
template<class T> void NPointT<T>::MakeIntegral(void)
{


	// Update our state
	*this = GetIntegral();
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
//		NPointT::GetIntegral : Make the point integral.
//----------------------------------------------------------------------------
template<class T> NPointT<T> NPointT<T>::GetIntegral(void) const
{	NPointT<T>		theResult;



	// Get the integral point
	//
	// An integral point has its position rounded to the nearest integer.
	theResult.x = (T) NMathUtilities::Rint(x);
	theResult.y = (T) NMathUtilities::Rint(y);

	return(theResult);
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





#else // NPOINT_CPP
#pragma mark -
//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNPointXKey									= "x";
static const NString kNPointYKey									= "y";





//============================================================================
//		NEncodable
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_REGISTER(NPointX, NPoint);

bool NPointX::EncodableCanEncode(const NVariant &theValue)
{	bool	canEncode;

	canEncode  = theValue.IsType(typeid(NPoint64));
	canEncode |= theValue.IsType(typeid(NPoint32));
	canEncode |= theValue.IsType(typeid(NPoint));

	return(canEncode);
}

void NPointX::EncodableEncodeObject(NEncoder &theEncoder, const NVariant &theValue)
{	NPointX		theObject;
	NPoint64	point64;
	NPoint32	point32;
	NPoint		point;

	if (theValue.GetValue(point64))
		theObject = NPointX(point64.x, point64.y);

	else if (theValue.GetValue(point32))
		theObject = NPointX(point32.x, point32.y);

	else if (theValue.GetValue(point))
		theObject = NPointX(point.x, point.y);

	else
		NN_LOG("Unknown type!");

	theObject.EncodeSelf(theEncoder);
}

NVariant NPointX::EncodableDecodeObject(const NEncoder &theEncoder)
{	NPointX		theObject;

	theObject.DecodeSelf(theEncoder);

	if (theObject.mX.GetPrecision() == kNPrecisionFloat64 || theObject.mY.GetPrecision() == kNPrecisionFloat64)
		return(NPoint64(theObject.mX.GetFloat64(), theObject.mY.GetFloat64()));
	else
		return(NPoint32(theObject.mX.GetFloat32(), theObject.mY.GetFloat32()));
}





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
NPointX::NPointX(void)
{
}





//============================================================================
//      NPointX::~NPointX : Destructor.
//----------------------------------------------------------------------------
NPointX::~NPointX(void)
{
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





//============================================================================
//      NPointX::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NPointX::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	mX = theEncoder.DecodeNumber(kNPointXKey);
	mY = theEncoder.DecodeNumber(kNPointYKey);
}



#endif // NPOINT_CPP



