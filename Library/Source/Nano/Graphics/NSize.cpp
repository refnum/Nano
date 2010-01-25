/*	NAME:
		NSize.cpp

	DESCRIPTION:
		Size object.

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

#ifndef NSIZE_CPP

#include "NEncoder.h"
#include "NSize.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNSizeWidthKey									= "width";
static const NString kNSizeHeightKey								= "height";





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NSizeX :	public NEncodable {
public:
										NENCODABLE_DECLARE(NSizeX);

										NSizeX(const NNumber &width, const NNumber &height);
										NSizeX(void);
	virtual							   ~NSizeX(void);


protected:
	// Encode the object
	void								EncodeSelf(NEncoder &theEncoder) const;


private:
	NNumber								mWidth;
	NNumber								mHeight;
};





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_NODECODE(NSizeX);





//============================================================================
//      NSizeX::NSizeX : Constructor.
//----------------------------------------------------------------------------
NSizeX::NSizeX(const NNumber &width, const NNumber &height)
{


	// Initialise ourselves
	mWidth  = width;
	mHeight = height;
}





//============================================================================
//      NSizeX::NSizeX : Constructor.
//----------------------------------------------------------------------------
NSizeX::NSizeX()
{
}





//============================================================================
//      NSizeX::~NSizeX : Destructor.
//----------------------------------------------------------------------------
NSizeX::~NSizeX(void)
{
}





//============================================================================
//      NSizeX::EncodableGetDecoded : Get a decoded object.
//----------------------------------------------------------------------------
NVariant NSizeX::EncodableGetDecoded(const NEncoder &theEncoder)
{


	// Decode the object
	mWidth = theEncoder.DecodeNumber(kNSizeWidthKey);
	mHeight = theEncoder.DecodeNumber(kNSizeHeightKey);
	
	if (mWidth.GetPrecision() == kNPrecisionFloat64 || mHeight.GetPrecision() == kNPrecisionFloat64)
		return(NSize64(mWidth.GetFloat64(), mHeight.GetFloat64()));
	else
		return(NSize32(mWidth.GetFloat32(), mHeight.GetFloat32()));
}





//============================================================================
//      NSizeX::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NSizeX::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNSizeWidthKey,  mWidth);
	theEncoder.EncodeNumber(kNSizeHeightKey, mHeight);
}





#else

//============================================================================
//		NSizeT::NSizeT : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
template<class T> NSizeT<T>::NSizeT(T valWidth, T valHeight)
{


	// Initialize ourselves
	width  = valWidth;
	height = valHeight;
}





//============================================================================
//		NSizeT::NSizeT : Constructor.
//----------------------------------------------------------------------------
template<class T> NSizeT<T>::NSizeT(void)
{


	// Initialize ourselves
	width  = 0;
	height = 0;
}





//============================================================================
//		NSizeT::Clear : Clear the size.
//----------------------------------------------------------------------------
template<class T> void NSizeT<T>::Clear(void)
{


	// Clear the size
	width  = 0;
	height = 0;
}





//============================================================================
//		NSizeT::IsEmpty : Is the size empty?
//----------------------------------------------------------------------------
template<class T> bool NSizeT<T>::IsEmpty(void) const
{


	// Test the size
	return(NMathUtilities::IsZero(width) && NMathUtilities::IsZero(height));
}





//============================================================================
//		NSizeT::Compare : Compare the value.
//----------------------------------------------------------------------------
template<class T> NComparison NSizeT<T>::Compare(const NSizeT &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(width, theValue.width);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(height, theValue.height);
	
	return(theResult);
}





//============================================================================
//		NSizeT::NEncodable : NEncodable operator.
//----------------------------------------------------------------------------
template<class T> NSizeT<T>::operator NEncodable(void) const
{	NSizeX		theResult(width, height);



	// Get the value
	return(theResult);
}





//============================================================================
//		NSizeT::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
template<class T> NSizeT<T>::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{w=%g, h=%g}", width, height);

	return(theResult);
}







#endif // NSIZE_CPP

