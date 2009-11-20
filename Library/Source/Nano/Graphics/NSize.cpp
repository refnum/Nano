/*	NAME:
		NSize.cpp

	DESCRIPTION:
		Size object.

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

#ifndef NSIZE_CPP

#include "NEncoder.h"
#include "NSize.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNSizeWidthKey									= "width";
static const NString kNSizeHeightKey								= "height";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NSize);





//============================================================================
//      NSize::NSize : Constructor.
//----------------------------------------------------------------------------
NSize::NSize(const NSize32 &theSize)
		: NSize32(theSize.width, theSize.height)
{
}





//============================================================================
//      NSize::NSize : Constructor.
//----------------------------------------------------------------------------
NSize::NSize(const NSize64 &theSize)
		: NSize32(theSize.width, theSize.height)
{
}





//============================================================================
//		NSize::NSize : Constructor.
//----------------------------------------------------------------------------
NSize::NSize(Float32 width, Float32 height)
	: NSize32(width, height)
{
}





//============================================================================
//		NSize::NSize : Constructor.
//----------------------------------------------------------------------------
NSize::NSize(Float64 width, Float64 height)
	: NSize32(width, height)
{
}





//============================================================================
//		NSize::NSize : Constructor.
//----------------------------------------------------------------------------
NSize::NSize(void)
{
}





//============================================================================
//		NSize::~NSize : Destructor.
//----------------------------------------------------------------------------
NSize::~NSize(void)
{
}





//============================================================================
//		NSize::NSize64 : NSize64 operator.
//----------------------------------------------------------------------------
NSize::operator NSize64(void) const
{	NSize64		theResult;



	// Get the value
	theResult.width  = width;
	theResult.height = height;
	
	return(theResult);
}





//============================================================================
//      NSize::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NSize::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNSizeWidthKey,  width);
	theEncoder.EncodeNumber(kNSizeHeightKey, height);
}





//============================================================================
//      NSize::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NSize::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	width  = theEncoder.DecodeNumber(kNSizeWidthKey).GetFloat32();
	height = theEncoder.DecodeNumber(kNSizeHeightKey).GetFloat32();
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
//		NSizeT::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
template<class T> NSizeT<T>::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{w=%g, h=%g}", width, height);

	return(theResult);
}







#endif // NSIZE_CPP

