/*	NAME:
		NTransform.cpp

	DESCRIPTION:
		Transform object.

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
#include "NGeometryUtilities.h"
#include "NMathUtilities.h"
#include "NSTLUtilities.h"
#include "NShape.h"
#include "NString.h"

#ifndef NTRANSFORM_CPP
	#include "NEncoder.h"
	#include "NTransform.h"
#endif

#ifdef NTRANSFORM_CPP





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
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NNumber								mA;
	NNumber								mB;
	NNumber								mC;
	NNumber								mD;
	NNumber								mTX;
	NNumber								mTY;
};





//============================================================================
//		NTransformT::NTransformT : Constructor.
//----------------------------------------------------------------------------
template<class T> NTransformT<T>::NTransformT(const NVariant &theValue)
{	NTransform64	transform64;
	NTransform32	transform32;
	NTransform		transform;



	// Initialize ourselves
	NTransformX::EncodableRegister();
	
	if (theValue.GetValue(transform64))
		{
		a  = (T) transform64.a;		b  = (T) transform64.b;
		c  = (T) transform64.c;		d  = (T) transform64.d;
		tx = (T) transform64.tx;	ty = (T) transform64.ty;
		}

	else if (theValue.GetValue(transform32))
		{
		a  = (T) transform32.a;		b  = (T) transform32.b;
		c  = (T) transform32.c;		d  = (T) transform32.d;
		tx = (T) transform32.tx;	ty = (T) transform32.ty;
		}

	else if (theValue.GetValue(transform))
		{
		a  = (T) transform.a;		b  = (T) transform.b;
		c  = (T) transform.c;		d  = (T) transform.d;
		tx = (T) transform.tx;		ty = (T) transform.ty;
		}

	else
		NN_LOG("Unknown type!");
}





//============================================================================
//		NTransformT::NTransformT : Constructor.
//----------------------------------------------------------------------------
template<class T> NTransformT<T>::NTransformT(const std::vector<T> &matrix23)
{


	// Validate our parameters
	NN_ASSERT(matrix23.size() == 6);



	// Initialize ourselves
	NTransformX::EncodableRegister();
	
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
	NTransformX::EncodableRegister();
	
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
	NTransformX::EncodableRegister();
	
	Clear();
}





//============================================================================
//		NTransformT::Clear : Clear the transform.
//----------------------------------------------------------------------------
template<class T> void NTransformT<T>::Clear(void)
{


	// Clear the transform
	a  = (T) 1.0;	b  = (T) 0.0;
	c  = (T) 0.0;	d  = (T) 1.0;
	tx = (T) 0.0;	ty = (T) 0.0;
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
	a  = (T) 1.0;	b  = (T) 0.0;
	c  = (T) 0.0;	d  = (T) 1.0;
	tx = (T) x;		ty = (T) y;
}





//============================================================================
//		NTransformT::SetScale : Set a scale.
//----------------------------------------------------------------------------
template<class T> void NTransformT<T>::SetScale(T x, T y)
{


	// Set the transform
	a  = (T) x;		b  = (T) 0.0;
	c  = (T) 0.0;	d  = (T) y;
	tx = (T) 0.0;	ty = (T) 0.0;
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
	tx = (T) 0.0;	ty = (T) 0.0;
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





#else // NTRANSFORM_CPP
#pragma mark -
//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNTransform64Key									= "64";
static const NString kNTransform2x3Key									= "2x3";





//============================================================================
//		NEncodable
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_REGISTER(NTransformX, NTransform);

bool NTransformX::EncodableCanEncode(const NVariant &theValue)
{	bool	canEncode;

	canEncode  = theValue.IsType(typeid(NTransform64));
	canEncode |= theValue.IsType(typeid(NTransform32));
	canEncode |= theValue.IsType(typeid(NTransform));

	return(canEncode);
}

void NTransformX::EncodableEncodeObject(NEncoder &theEncoder, const NVariant &theValue)
{	NTransform64	transform64;
	NTransform32	transform32;
	NTransform		transform;
	NTransformX		theObject;

	if (theValue.GetValue(transform64))
		theObject = NTransformX(transform64.a, transform64.b, transform64.c, transform64.d, transform64.tx, transform64.ty);

	else if (theValue.GetValue(transform32))
		theObject = NTransformX(transform32.a, transform32.b, transform32.c, transform32.d, transform32.tx, transform32.ty);

	else if (theValue.GetValue(transform))
		theObject = NTransformX(transform.a, transform.b, transform.c, transform.d, transform.tx, transform.ty);

	else
		NN_LOG("Unknown type!");

	theObject.EncodeSelf(theEncoder);
}

NVariant NTransformX::EncodableDecodeObject(const NEncoder &theEncoder)
{	NTransformX		theObject;

	theObject.DecodeSelf(theEncoder);

	if (theObject.mA.GetPrecision()  == kNPrecisionFloat64 ||
		theObject.mB.GetPrecision()  == kNPrecisionFloat64 ||
		theObject.mC.GetPrecision()  == kNPrecisionFloat64 ||
		theObject.mD.GetPrecision()  == kNPrecisionFloat64 ||
		theObject.mTX.GetPrecision() == kNPrecisionFloat64 ||
		theObject.mTY.GetPrecision() == kNPrecisionFloat64)
		return(NTransform64(theObject.mA.GetFloat64(),  theObject.mB.GetFloat64(),
							theObject.mC.GetFloat64(),  theObject.mD.GetFloat64(),
							theObject.mTX.GetFloat64(), theObject.mTY.GetFloat64()));
	else
		return(NTransform32(theObject.mA.GetFloat32(),  theObject.mB.GetFloat32(),
							theObject.mC.GetFloat32(),  theObject.mD.GetFloat32(),
							theObject.mTX.GetFloat32(), theObject.mTY.GetFloat32()));
}





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





//============================================================================
//      NTransformX::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NTransformX::DecodeSelf(const NEncoder &theEncoder)
{	NArray			theArray;
	Float64List		array64;
	Float32List		array32;
	bool			is64;



	// Decode the object
	is64 = theEncoder.DecodeBoolean(kNTransform64Key);

	if (theEncoder.DecodeObject(kNTransform2x3Key).GetValue(theArray))
		{
		if (is64)
			{
			array64 = theArray.GetValuesFloat64();
			NN_ASSERT(array64.size() == 6);
			
			mA  = array64[0];		mB  = array64[1];
			mC  = array64[2];		mD  = array64[3];
			mTX = array64[4];		mTX = array64[5];
			}
		else
			{
			array32 = theArray.GetValuesFloat32();
			NN_ASSERT(array32.size() == 6);
			
			mA  = array32[0];		mB  = array32[1];
			mC  = array32[2];		mD  = array32[3];
			mTX = array32[4];		mTX = array32[5];
			}
		}
}



#endif // NTRANSFORM_CPP




