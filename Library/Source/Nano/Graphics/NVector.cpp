/*	NAME:
		NVector.cpp

	DESCRIPTION:
		Vector object.

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





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NVector kNVectorZero;
const NVector kNVectorNorth									= NVector( 0.0f,  1.0f);
const NVector kNVectorSouth									= NVector( 0.0f, -1.0f);
const NVector kNVectorEast									= NVector( 1.0f,  0.0f);
const NVector kNVectorWest									= NVector(-1.0f,  0.0f);





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNVectorXKey									= "x";
static const NString kNVectorYKey									= "y";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NVector);





//============================================================================
//		NVector::NVector : Constructor.
//----------------------------------------------------------------------------
NVector::NVector(const NPoint &point1, const NPoint &point2)
{


	// Initialize ourselves
	x = point2.x - point1.x;
	y = point2.y - point1.y;
}





//============================================================================
//		NVector::NVector : Constructor.
//----------------------------------------------------------------------------
NVector::NVector(Float32 valX, Float32 valY)
{


	// Initialize ourselves
	x = valX;
	y = valY;
}





//============================================================================
//		NVector::~NVector : Destructor.
//----------------------------------------------------------------------------
NVector::~NVector(void)
{
}





//============================================================================
//		NVector::Clear : Clear the Vector.
//----------------------------------------------------------------------------
void NVector::Clear(void)
{


	// Clear the vector
	x = 0.0f;
	y = 0.0f;
}





//============================================================================
//		NVector::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NVector::Compare(const NVector &theValue) const
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
//		NVector::IsZero : Is the vector zero?
//----------------------------------------------------------------------------
bool NVector::IsZero(void) const
{


	// Test the vector
	return(NMathUtilities::IsZero(x) && NMathUtilities::IsZero(y));
}





//============================================================================
//		NVector::IsNormalized : Is the vector normalized?
//----------------------------------------------------------------------------
bool NVector::IsNormalized(void) const
{


	// Test the vector
	return(NMathUtilities::AreEqual(GetLength(), 1.0f));
}





//============================================================================
//		NVector::Add : Add a vector.
//----------------------------------------------------------------------------
void NVector::Add(const NVector &theVector)
{


	// Add the vector
	x += theVector.x;
	y += theVector.y;
}





//============================================================================
//		NVector::Subtract : Subtract a vector.
//----------------------------------------------------------------------------
void NVector::Subtract(const NVector &theVector)
{


	// Subtract the vector
	x -= theVector.x;
	y -= theVector.y;
}





//============================================================================
//		NVector::Scale : Scale the vector.
//----------------------------------------------------------------------------
void NVector::Scale(Float32 s)
{


	// Scale the vector
	x *= s;
	y *= s;
}





//============================================================================
//		NVector::Negate : Negate the vector.
//----------------------------------------------------------------------------
void NVector::Negate(void)
{


	// Negate the vector
	x = -x;
	y = -y;
}





//============================================================================
//		NVector::Normalize : Normalize the vector.
//----------------------------------------------------------------------------
void NVector::Normalize(void)
{	Float32		invLength;



	// Normalize the vector
	invLength = 1.0f / GetLength();

	x *= invLength;
	y *= invLength;
}





//============================================================================
//		NVector::GetLength : Get the length.
//----------------------------------------------------------------------------
Float32 NVector::GetLength(void) const
{


	// Get the length
	return(NMathUtilities::FastRoot(GetLength2()));
}





//============================================================================
//		NVector::GetLength2 : Get the length^2.
//----------------------------------------------------------------------------
Float32 NVector::GetLength2(void) const
{


	// Get the length^2
	return((x * x) + (y * y));
}





//============================================================================
//		NVector::GetDot : Get the dot product.
//----------------------------------------------------------------------------
Float32 NVector::GetDot(const NVector &theVector) const
{


	// Get the dot product
	return((x * theVector.x) + (y * theVector.y));
}





//============================================================================
//		NVector::GetCross : Get the cross product.
//----------------------------------------------------------------------------
Float32 NVector::GetCross(const NVector &theVector) const
{


	// Get the dot product
	return((x * theVector.y) - (y * theVector.x));
}





//============================================================================
//		NVector::GetAngle : Get the angle between two vectors.
//----------------------------------------------------------------------------
NDegrees NVector::GetAngle(const NVector &theVector) const
{	Float32			dotProduct, crossProduct, angleRad;
	NVector			normal1, normal2;



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
//		NVector::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NVector::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{x=%g, y=%g}", x, y);

	return(theResult);
}





//============================================================================
//      NVector::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
#pragma mark -
void NVector::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNVectorXKey, x);
	theEncoder.EncodeNumber(kNVectorYKey, y);
}





//============================================================================
//      NVector::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NVector::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	x = theEncoder.DecodeNumber(kNVectorXKey).GetFloat32();
	y = theEncoder.DecodeNumber(kNVectorYKey).GetFloat32();
}


