/*	NAME:
		NTransform.cpp

	DESCRIPTION:
		Transform object.

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
#include "NSTLUtilities.h"
#include "NShape.h"
#include "NString.h"

#ifndef NTRANSFORM_CPP

#include "NEncoder.h"
#include "NTransform.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNTransform2X3Key									= "2x3";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NTransform);





//============================================================================
//		NTransform::NTransform : Constructor.
//----------------------------------------------------------------------------
NTransform::NTransform(const NTransform32 &theTransform)
		: NTransform32(theTransform.a, theTransform.b, theTransform.c, theTransform.d, theTransform.tx, theTransform.ty)
{
}





//============================================================================
//		NTransform::NTransform : Constructor.
//----------------------------------------------------------------------------
NTransform::NTransform(const NTransform64 &theTransform)
		: NTransform32(theTransform.a, theTransform.b, theTransform.c, theTransform.d, theTransform.tx, theTransform.ty)
{
}





//============================================================================
//		NTransform::NTransform : Constructor.
//----------------------------------------------------------------------------
NTransform::NTransform(Float32 a, Float32 b, Float32 c, Float32 d, Float32 tx, Float32 ty)
		: NTransform32(a, b, c, d, tx, ty)
{
}





//============================================================================
//		NTransform::NTransform : Constructor.
//----------------------------------------------------------------------------
NTransform::NTransform(Float64 a, Float64 b, Float64 c, Float64 d, Float64 tx, Float64 ty)
		: NTransform32(a, b, c, d, tx, ty)
{
}





//============================================================================
//		NTransform::NTransform : Constructor.
//----------------------------------------------------------------------------
NTransform::NTransform(void)
{
}





//============================================================================
//		NTransform::~NTransform : Destructor.
//----------------------------------------------------------------------------
NTransform::~NTransform(void)
{
}





//============================================================================
//		NTransform::NTransform64 : NTransform64 operator.
//----------------------------------------------------------------------------
NTransform::operator NTransform64(void) const
{	NTransform64		theResult;



	// Get the value
	theResult.a  = a;		theResult.b  = b;
	theResult.c  = c;		theResult.d  = d;
	theResult.tx = tx;		theResult.ty = ty;
	
	return(theResult);
}





//============================================================================
//      NTransform::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NTransform::EncodeSelf(NEncoder &theEncoder) const
{	NArray		theArray;



	// Encode the object
	theArray.SetValuesFloat32(vector(a, b, c, d, tx, ty));
	
	theEncoder.EncodeObject(kNTransform2X3Key, theArray);
}





//============================================================================
//      NTransform::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NTransform::DecodeSelf(const NEncoder &theEncoder)
{	Float32List		theMatrix;
	NArray			theArray;



	// Decode the object
	if (theEncoder.DecodeObject(kNTransform2X3Key).GetValue(theArray))
		{
		theMatrix = theArray.GetValuesFloat32();
		NN_ASSERT(theMatrix.size() == 6);
		
		a  = theMatrix[0];		b  = theMatrix[1];
		c  = theMatrix[2];		d  = theMatrix[3];
		tx = theMatrix[4];		ty = theMatrix[5];
		}
}





#else

//============================================================================
//		NTransformT::NTransformT : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
template<class T> NTransformT<T>::NTransformT(T valA, T valB, T valC, T valD, T valTX, T valTY)
{


	// Initialize ourselves
	a  = valA;		b  = valB;
	c  = valC;		d  = valD;
	tx = valTX;		ty = valTY;
}





//============================================================================
//		NTransformT::NTransformT : Constructor.
//----------------------------------------------------------------------------
template<class T> NTransformT<T>::NTransformT(void)
{


	// Initialize ourselves
	Clear();
}





//============================================================================
//		NTransformT::~NTransformT : Destructor.
//----------------------------------------------------------------------------
template<class T> NTransformT<T>::~NTransformT(void)
{
}





//============================================================================
//		NTransformT::Clear : Clear the transform.
//----------------------------------------------------------------------------
template<class T> void NTransformT<T>::Clear(void)
{


	// Clear the transform
	a  = 1.0;	b  = 0.0;
	c  = 0.0;	d  = 1.0;
	tx = 0.0;	ty = 0.0;
}





//============================================================================
//		NTransformT::IsIdentity : Is this the identity transform?
//----------------------------------------------------------------------------
template<class T> bool NTransformT<T>::IsIdentity(void) const
{	bool	isIdentity;



	// Check our state
	isIdentity = NMathUtilities::AreEqual( a, 1.0) && NMathUtilities::AreEqual( b, 0.0) &&
				 NMathUtilities::AreEqual( c, 0.0) && NMathUtilities::AreEqual( d, 1.0) &&
				 NMathUtilities::AreEqual(tx, 0.0) && NMathUtilities::AreEqual(ty, 0.0);

	return(isIdentity);
}





//============================================================================
//		NTransformT::Compare : Compare the value.
//----------------------------------------------------------------------------
template<class T> NComparison NTransformT<T>::Compare(const NTransformT<T> &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(a, theValue.a);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(b,  theValue.b);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(c,  theValue.c);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(d,  theValue.d);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(tx, theValue.tx);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(ty, theValue.ty);
	
	return(theResult);
}





//============================================================================
//		NTransformT::SetTranslate : Set a translation.
//----------------------------------------------------------------------------
template<class T> void NTransformT<T>::SetTranslate(T x, T y)
{


	// Set the transform
	a  = 1.0;	b  = 0.0;
	c  = 0.0;	d  = 1.0;
	tx = x;		ty = y;
}





//============================================================================
//		NTransformT::SetScale : Set a scale.
//----------------------------------------------------------------------------
template<class T> void NTransformT<T>::SetScale(T x, T y)
{


	// Set the transform
	a  = x;		b  = 0.0;
	c  = 0.0;	d  = y;
	tx = 0.0;	ty = 0.0;
}





//============================================================================
//		NTransformT::SetRotate : Set a rotation.
//----------------------------------------------------------------------------
template<class T> void NTransformT<T>::SetRotate(NRadians angle)
{	T		cosA, sinA;



	// Get the state we need
	cosA = cos(angle);
	sinA = sin(angle);



	// Set the transform
	a  =  cosA;		b  = sinA;
	c  = -sinA;		d  = cosA;
	tx =  0.0;		ty = 0.0;
}





//============================================================================
//		NTransformT::Apply : Apply the transform.
//----------------------------------------------------------------------------
template<class T> NPointT<T> NTransformT<T>::Apply(const NPointT<T> &thePoint)
{	NPointT<T>		theResult;



	// Apply the transform
	theResult.x = (a * thePoint.x) + (c * thePoint.y) + tx;
	theResult.y = (b * thePoint.x) + (d * thePoint.y) + ty;
	
	return(theResult);
}





//============================================================================
//		NTransformT::Apply : Apply the transform.
//----------------------------------------------------------------------------
template<class T> NSizeT<T> NTransformT<T>::Apply(const NSizeT<T> &theSize)
{	NSizeT<T>		theResult;



	// Apply the transform
	theResult.x = (a * theSize.width) + (c * theSize.height);
	theResult.y = (b * theSize.width) + (d * theSize.height);
	
	return(theResult);
}





//============================================================================
//		NTransformT::Apply : Apply the transform.
//----------------------------------------------------------------------------
template<class T> NRectangleT<T> NTransformT<T>::Apply(const NRectangleT<T> &theRectangle)
{	NPointT<T>			pTL, pTR, pBL, pBR;
	NShapeT<T>			theShape;



	// Apply the transform
	pTL = Apply(theRectangle.GetCorner(kNPositionTopLeft));
	pTR = Apply(theRectangle.GetCorner(kNPositionTopRight));
	pBL = Apply(theRectangle.GetCorner(kNPositionBottomLeft));
	pBR = Apply(theRectangle.GetCorner(kNPositionBottomRight));

	theShape = NShapeT<T>(vector(pTL, pTR, pBL, pBR));
	
	return(theShape.GetBounds());
}





//============================================================================
//		NTransformT::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
template<class T> NTransformT<T>::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{[%g, %g] [%g, %g] [%g, %g]}", a, b, c, d, tx, ty);

	return(theResult);
}



#endif // NTRANSFORM_CPP




