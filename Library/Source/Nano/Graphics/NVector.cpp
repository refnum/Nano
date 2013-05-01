/*	NAME:
		NVector.cpp

	DESCRIPTION:
		Vector object.
		
		NEncodable support uses a helper class to avoid a v-table.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NString.h"

#ifndef NVECTOR_CPP
	#include "NEncoder.h"
	#include "NVector.h"
#endif

#ifdef NVECTOR_CPP





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NVectorX : public NEncodable {
public:
										NENCODABLE_DECLARE(NVectorX);

										NVectorX(const NNumber &x, const NNumber &y);
										NVectorX(void);
	virtual							   ~NVectorX(void);


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NNumber								mX;
	NNumber								mY;
};





//============================================================================
//		NVectorT::NVectorT : Constructor.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::NVectorT(const NVariant &theValue)
{	NVector64	vector64;
	NVector32	vector32;
	NVector		vector;



	// Initialize ourselves
	NVectorX::EncodableRegister();
	
	if (theValue.GetValue(vector64))
		{
		x = (T) vector64.x;
		y = (T) vector64.y;
		}

	else if (theValue.GetValue(vector32))
		{
		x = (T) vector32.x;
		y = (T) vector32.y;
		}

	else if (theValue.GetValue(vector))
		{
		x = (T) vector.x;
		y = (T) vector.y;
		}

	else
		NN_LOG("Unknown type!");
}





//============================================================================
//		NVectorT::NVectorT : Constructor.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::NVectorT(T valX, T valY)
{


	// Initialize ourselves
	NVectorX::EncodableRegister();
	
	x = valX;
	y = valY;
}





//============================================================================
//		NVectorT::NVectorT : Constructor.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::NVectorT(const NPointT<T> &point1, const NPointT<T> &point2)
{


	// Initialize ourselves
	NVectorX::EncodableRegister();
	
	x = point2.x - point1.x;
	y = point2.y - point1.y;
}





//============================================================================
//		NVectorT::NVectorT : Constructor.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::NVectorT(void)
{


	// Initialize ourselves
	NVectorX::EncodableRegister();
	
	x = 0;
	y = 0;
}





//============================================================================
//		NVectorT::Clear : Clear the vector.
//----------------------------------------------------------------------------
template<class T> void NVectorT<T>::Clear(void)
{


	// Clear the vector
	x = 0;
	y = 0;
}





//============================================================================
//		NVectorT::IsZero : Is the vector zero?
//----------------------------------------------------------------------------
template<class T> bool NVectorT<T>::IsZero(void) const
{


	// Test the vector
	return(NMathUtilities::IsZero(x) && NMathUtilities::IsZero(y));
}





//============================================================================
//		NVectorT::IsNormalized : Is the vector normalized?
//----------------------------------------------------------------------------
template<class T> bool NVectorT<T>::IsNormalized(void) const
{


	// Test the vector
	return(NMathUtilities::AreEqual(GetLength(), 1.0));
}





//============================================================================
//		NVectorT::Compare : Compare the value.
//----------------------------------------------------------------------------
template<class T> NComparison NVectorT<T>::Compare(const NVectorT<T> &theValue) const
{	T				length1, length2;
	NComparison		theResult;



	// Compare the value
	//
	// Equality is detected with direct comparisons, but once inequality has
	// been established we compare by (relative) length to provide a magnitude
	// comparison.
	//
	// The magnitude comparison requires multiplication, which will reduce
	// precision slightly, so can't be our only test.
	theResult = GetComparison(x, theValue.x);

	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(y, theValue.y);

	if (theResult != kNCompareEqualTo)
		{
		length1   =          GetLength2();
		length2   = theValue.GetLength2();
		theResult = GetComparison(length1, length2);
		}

	return(theResult);
}





//============================================================================
//		NVectorT::Normalize : Normalize the vector.
//----------------------------------------------------------------------------
template<class T> void NVectorT<T>::Normalize(void)
{


	// Update our state
	*this = GetNormalized();
}





//============================================================================
//		NVectorT::Negate : Negate the vector.
//----------------------------------------------------------------------------
template<class T> void NVectorT<T>::Negate(void)
{


	// Update our state
	*this = GetNegated();
}





//============================================================================
//		NVectorT::Scale : Scale the vector.
//----------------------------------------------------------------------------
template<class T> void NVectorT<T>::Scale(T scaleBy)
{


	// Update our state
	*this = GetScaled(scaleBy);
}





//============================================================================
//		NVectorT::GetNormalized : Get the normalized vector.
//----------------------------------------------------------------------------
template<class T> NVectorT<T> NVectorT<T>::GetNormalized(void) const
{	NVectorT<T>		theResult;
	T				invLength;



	// Normalize the vector
	invLength = (T) (1.0 / GetLength());

	theResult.x = x * invLength;
	theResult.y = y * invLength;

	return(theResult);
}





//============================================================================
//		NVectorT::GetNegated : Get the negated vector.
//----------------------------------------------------------------------------
template<class T> NVectorT<T> NVectorT<T>::GetNegated(void) const
{	NVectorT<T>		theResult;



	// Negate the vector
	theResult.x = -x;
	theResult.y = -y;
	
	return(theResult);
}





//============================================================================
//		NVectorT::GetScaled : Get the scaled vector.
//----------------------------------------------------------------------------
template<class T> NVectorT<T> NVectorT<T>::GetScaled(T scaleBy) const
{	NVectorT<T>		theResult;



	// Scale the vector
	theResult.x = x * scaleBy;
	theResult.y = y * scaleBy;
	
	return(theResult);
}





//============================================================================
//		NVectorT::GetLength : Get the length.
//----------------------------------------------------------------------------
template<class T> T NVectorT<T>::GetLength(bool getApprox) const
{	T	theLength;



	// Get the length
	theLength = GetLength2();
	
	if (getApprox)
		theLength = (T) NMathUtilities::FastRoot((Float32) theLength);
	else
		theLength = (T) sqrt(theLength);
	
	return(theLength);
}





//============================================================================
//		NVectorT::GetLength2 : Get the length^2.
//----------------------------------------------------------------------------
template<class T> T NVectorT<T>::GetLength2(void) const
{


	// Get the length^2
	return((x * x) + (y * y));
}





//============================================================================
//		NVectorT::GetDot : Get the dot product.
//----------------------------------------------------------------------------
template<class T> T NVectorT<T>::GetDot(const NVectorT<T> &theVector) const
{


	// Get the dot product
	return((x * theVector.x) + (y * theVector.y));
}





//============================================================================
//		NVectorT::GetCross : Get the cross product.
//----------------------------------------------------------------------------
template<class T> T NVectorT<T>::GetCross(const NVectorT<T> &theVector) const
{


	// Get the dot product
	return((x * theVector.y) - (y * theVector.x));
}





//============================================================================
//		NVectorT::GetAngle : Get the angle between two vectors.
//----------------------------------------------------------------------------
template<class T> NDegrees NVectorT<T>::GetAngle(const NVectorT<T> &theVector) const
{	T				dotProduct, crossProduct, angleRad;
	NVectorT<T>		normal1, normal2;



	// Get the state we need
	normal1 = *this;
	normal1.Normalize();
	
	normal2 = theVector;
	normal2.Normalize();



	// Calculate the angle
	dotProduct   = normal1.GetDot(  normal2);
	crossProduct = normal1.GetCross(normal2);
	angleRad     = atan2(crossProduct, dotProduct);
	
	return(NMathUtilities::GetDegrees(angleRad));
}





//============================================================================
//		NVectorT::+= : Addition operator.
//----------------------------------------------------------------------------
template<class T> const NVectorT<T>& NVectorT<T>::operator += (const NVectorT<T> &theVector)
{


	// Add the vector
	x += theVector.x;
	y += theVector.y;

	return(*this);
}





//============================================================================
//		NVectorT::-= : Subtraction operator.
//----------------------------------------------------------------------------
template<class T> const NVectorT<T>& NVectorT<T>::operator -= (const NVectorT<T> &theVector)
{


	// Subtract the vector
	x -= theVector.x;
	y -= theVector.y;

	return(*this);
}





//============================================================================
//		NVectorT::NEncodable : NEncodable operator.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::operator NEncodable(void) const
{	NVectorX	theResult(x, y);



	// Get the value
	return(theResult);
}





//============================================================================
//		NVectorT::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
template<class T> NVectorT<T>::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{x=%g, y=%g}", x, y);

	return(theResult);
}





#else // NVECTOR_CPP
#pragma mark -
//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NVector kNVectorNorth											= NVector( 0.0f,  1.0f);
const NVector kNVectorSouth											= NVector( 0.0f, -1.0f);
const NVector kNVectorEast											= NVector( 1.0f,  0.0f);
const NVector kNVectorWest											= NVector(-1.0f,  0.0f);





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNVectorXKey									= "x";
static const NString kNVectorYKey									= "y";





//============================================================================
//		NEncodable
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_REGISTER(NVectorX, NVector);

bool NVectorX::EncodableCanEncode(const NVariant &theValue)
{	bool	canEncode;

	canEncode  = theValue.IsType(typeid(NVector64));
	canEncode |= theValue.IsType(typeid(NVector32));
	canEncode |= theValue.IsType(typeid(NVector));

	return(canEncode);
}

void NVectorX::EncodableEncodeObject(NEncoder &theEncoder, const NVariant &theValue)
{	NVectorX	theObject;
	NVector64	vector64;
	NVector32	vector32;
	NVector		vector;

	if (theValue.GetValue(vector64))
		theObject = NVectorX(vector64.x, vector64.y);

	else if (theValue.GetValue(vector32))
		theObject = NVectorX(vector32.x, vector32.y);

	else if (theValue.GetValue(vector))
		theObject = NVectorX(vector.x, vector.y);

	else
		NN_LOG("Unknown type!");

	theObject.EncodeSelf(theEncoder);
}

NVariant NVectorX::EncodableDecodeObject(const NEncoder &theEncoder)
{	NVectorX	theObject;

	theObject.DecodeSelf(theEncoder);

	if (theObject.mX.GetPrecision() == kNPrecisionFloat64 || theObject.mY.GetPrecision() == kNPrecisionFloat64)
		return(NVector64(theObject.mX.GetFloat64(), theObject.mY.GetFloat64()));
	else
		return(NVector32(theObject.mX.GetFloat32(), theObject.mY.GetFloat32()));
}





//============================================================================
//      NVectorX::NVectorX : Constructor.
//----------------------------------------------------------------------------
NVectorX::NVectorX(const NNumber &x, const NNumber &y)
{


	// Initialise ourselves
	mX = x;
	mY = y;
}





//============================================================================
//      NVectorX::NVectorX : Constructor.
//----------------------------------------------------------------------------
NVectorX::NVectorX()
{
}





//============================================================================
//      NVectorX::~NVectorX : Destructor.
//----------------------------------------------------------------------------
NVectorX::~NVectorX(void)
{
}





//============================================================================
//      NVectorX::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NVectorX::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNVectorXKey, mX);
	theEncoder.EncodeNumber(kNVectorYKey, mY);
}





//============================================================================
//      NVectorX::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NVectorX::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	mX = theEncoder.DecodeNumber(kNVectorXKey);
	mY = theEncoder.DecodeNumber(kNVectorYKey);
}



#endif // NVECTOR_CPP


