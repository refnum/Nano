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
#include "NGeometryUtilities.h"
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
static const NString kNTransform64Key									= "64";
static const NString kNTransform2x3Key									= "2x3";





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NTransformX :	public NEncodable {
public:
										NENCODABLE_DECLARE(NTransformX);

										NTransformX(const NNumber &a, const NNumber &b, const NNumber &c, const NNumber &d, const NNumber &tx, const NNumber &ty);
										NTransformX(void);
	virtual							   ~NTransformX(void);


protected:
	// Encode the object
	void								EncodeSelf(NEncoder &theEncoder) const;


private:
	NNumber								mA;
	NNumber								mB;
	NNumber								mC;
	NNumber								mD;
	NNumber								mTX;
	NNumber								mTY;
};





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_NODECODE(NTransformX);





//============================================================================
//      NTransformX::NTransformX : Constructor.
//----------------------------------------------------------------------------
NTransformX::NTransformX(const NNumber &a, const NNumber &b, const NNumber &c, const NNumber &d, const NNumber &tx, const NNumber &ty)
{


	// Initialise ourselves
	mA  = a;
	mB  = b;
	mC  = c;
	mD  = d;
	mTX = tx;
	mTY = ty;
}





//============================================================================
//      NTransformX::NTransformX : Constructor.
//----------------------------------------------------------------------------
NTransformX::NTransformX()
{
}





//============================================================================
//      NTransformX::~NTransformX : Destructor.
//----------------------------------------------------------------------------
NTransformX::~NTransformX(void)
{
}





//============================================================================
//      NTransformX::EncodableGetDecoded : Get a decoded object.
//----------------------------------------------------------------------------
NVariant NTransformX::EncodableGetDecoded(const NEncoder &theEncoder)
{	NArray		theArray;
	bool		is64;



	// Decode the object
	is64 = theEncoder.DecodeBoolean(kNTransform64Key);

	if (theEncoder.DecodeObject(kNTransform2x3Key).GetValue(theArray))
		{
		if (is64)
			return(NTransform64(theArray.GetValuesFloat64()));
		else
			return(NTransform32(theArray.GetValuesFloat32()));
		}
	
	return(NVariant());
}





//============================================================================
//      NTransformX::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NTransformX::EncodeSelf(NEncoder &theEncoder) const
{	NArray		theArray;
	bool		is64;



	// Get the state we need
	is64 = ( mA.GetPrecision() == kNPrecisionFloat64 &&
			 mB.GetPrecision() == kNPrecisionFloat64 &&
			 mC.GetPrecision() == kNPrecisionFloat64 &&
			 mD.GetPrecision() == kNPrecisionFloat64 &&
			mTX.GetPrecision() == kNPrecisionFloat64 &&
			mTY.GetPrecision() == kNPrecisionFloat64);

	if (is64)
		theArray = NArray(vector(mA.GetFloat64(), mB.GetFloat64(), mC.GetFloat64(), mD.GetFloat64(), mTX.GetFloat64(), mTY.GetFloat64()));
	else
		theArray = NArray(vector(mA.GetFloat32(), mB.GetFloat32(), mC.GetFloat32(), mD.GetFloat32(), mTX.GetFloat32(), mTY.GetFloat32()));



	// Encode the object
	theEncoder.EncodeBoolean(kNTransform64Key, is64);
	theEncoder.EncodeObject( kNTransform2x3Key, theArray);
}





#else

//============================================================================
//		NTransformT::NTransformT : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
template<class T> NTransformT<T>::NTransformT(const std::vector<T> &matrix23)
{


	// Validate our parameters
	NN_ASSERT(matrix23.size() == 6);



	// Initialize ourselves
	a  = matrix23[0];		b  = matrix23[1];
	c  = matrix23[2];		d  = matrix23[3];
	tx = matrix23[4];		ty = matrix23[5];
}





//============================================================================
//		NTransformT::NTransformT : Constructor.
//----------------------------------------------------------------------------
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
	cosA = (T) cos(angle);
	sinA = (T) sin(angle);



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
	
	return(NGeometryUtilities::GetBounds(theShape.points));
}





//============================================================================
//		NTransformT::NEncodable : NEncodable operator.
//----------------------------------------------------------------------------
template<class T> NTransformT<T>::operator NEncodable(void) const
{	NTransformX		theResult(a, b, c, d, tx, ty);



	// Get the value
	return(theResult);
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




