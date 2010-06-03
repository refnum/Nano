/*	NAME:
		NSize.cpp

	DESCRIPTION:
		Size object.
		
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

#ifndef NSIZE_CPP
	#include "NEncoder.h"
	#include "NSize.h"
#endif

#ifdef NSIZE_CPP





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NSizeX : public NEncodable {
public:
										NENCODABLE_DECLARE(NSizeX);

										NSizeX(const NNumber &width, const NNumber &height);
										NSizeX(void);
	virtual							   ~NSizeX(void);


protected:
	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	NNumber								mWidth;
	NNumber								mHeight;
};





//============================================================================
//		NSizeT::NSizeT : Constructor.
//----------------------------------------------------------------------------
template<class T> NSizeT<T>::NSizeT(const NVariant &theValue)
{	NSize64		size64;
	NSize32		size32;
	NSize		size;



	// Initialize ourselves
	NSizeX::EncodableRegister();
	
	if (theValue.GetValue(size64))
		{
		width  = (T) size64.width;
		height = (T) size64.height;
		}

	else if (theValue.GetValue(size32))
		{
		width  = (T) size32.width;
		height = (T) size32.height;
		}

	else if (theValue.GetValue(size))
		{
		width  = (T) size.width;
		height = (T) size.height;
		}

	else
		NN_LOG("Unknown type!");
}





//============================================================================
//		NSizeT::NSizeT : Constructor.
//----------------------------------------------------------------------------
template<class T> NSizeT<T>::NSizeT(T valWidth, T valHeight)
{


	// Initialize ourselves
	NSizeX::EncodableRegister();
	
	width  = valWidth;
	height = valHeight;
}





//============================================================================
//		NSizeT::NSizeT : Constructor.
//----------------------------------------------------------------------------
template<class T> NSizeT<T>::NSizeT(void)
{


	// Initialize ourselves
	NSizeX::EncodableRegister();
	
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
//		NSizeT::Normalize : Normalize the size.
//----------------------------------------------------------------------------
template<class T> void NSizeT<T>::Normalize(void)
{


	// Update our state
	*this = GetNormalized();
}





//============================================================================
//		NSizeT::MakeIntegral : Make the size integral.
//----------------------------------------------------------------------------
template<class T> void NSizeT<T>::MakeIntegral(void)
{


	// Update our state
	*this = GetIntegral();
}





//============================================================================
//		NSizeT::Resize : Resize the size.
//----------------------------------------------------------------------------
template<class T> void NSizeT<T>::Resize(T deltaX, T deltaY)
{


	// Update our state
	*this = GetResized(deltaX, deltaY);
}





//============================================================================
//		NSizeT::Scale : Scale the size.
//----------------------------------------------------------------------------
template<class T> void NSizeT<T>::Scale(T scaleBy)
{


	// Update our state
	*this = GetScaled(scaleBy);
}





//============================================================================
//		NSizeT::GetNormalized : Get the normalized size.
//----------------------------------------------------------------------------
template<class T> NSizeT<T> NSizeT<T>::GetNormalized(void) const
{	NSizeT<T>	theResult;



	// Noramlize the size
	//
	// A normalized size has positive width and height.
	theResult.width  = (width  < (T) 0) ? -width  : width;
	theResult.height = (height < (T) 0) ? -height : height;
	
	return(theResult);
}





//============================================================================
//		NSizeT::GetIntegral : Get the integral size.
//----------------------------------------------------------------------------
template<class T> NSizeT<T> NSizeT<T>::GetIntegral(void) const
{	NSizeT<T>		theResult;



	// Get the integral size
	//
	// An integral size has its size rounded up, to create a (potentially
	// larger) size with integer coordinates.
	theResult.width  = ceil(width);
	theResult.height = ceil(height);

	return(theResult);
}





//============================================================================
//		NSizeT::GetResized : Resize the size.
//----------------------------------------------------------------------------
template<class T> NSizeT<T> NSizeT<T>::GetResized(T deltaX, T deltaY) const
{	NSizeT<T>	theResult;



	// Resize the size
	theResult.width  = width  + deltaX;
	theResult.height = height + deltaY;
	
	return(theResult);
}





//============================================================================
//		NSizeT::GetScaled : Scale the size.
//----------------------------------------------------------------------------
template<class T> NSizeT<T> NSizeT<T>::GetScaled(T scaleBy) const
{	NSizeT<T>		theResult;



	// Scale the size
	theResult.width  = width  * scaleBy;
	theResult.height = height * scaleBy;
	
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





#else // NSIZE_CPP
#pragma mark -
//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNSizeWidthKey									= "width";
static const NString kNSizeHeightKey								= "height";





//============================================================================
//		NEncodable
//----------------------------------------------------------------------------
NENCODABLE_DEFINE_REGISTER(NSizeX, NSize);

bool NSizeX::EncodableCanEncode(const NVariant &theValue)
{	bool	canEncode;

	canEncode  = theValue.IsType(typeid(NSize64));
	canEncode |= theValue.IsType(typeid(NSize32));
	canEncode |= theValue.IsType(typeid(NSize));

	return(canEncode);
}

void NSizeX::EncodableEncodeObject(NEncoder &theEncoder, const NVariant &theValue)
{	NSizeX		theObject;
	NSize64		size64;
	NSize32		size32;
	NSize		size;

	if (theValue.GetValue(size64))
		theObject = NSizeX(size64.width, size64.height);

	else if (theValue.GetValue(size32))
		theObject = NSizeX(size32.width, size32.height);

	else if (theValue.GetValue(size))
		theObject = NSizeX(size.width, size.height);

	else
		NN_LOG("Unknown type!");

	theObject.EncodeSelf(theEncoder);
}

NVariant NSizeX::EncodableDecodeObject(const NEncoder &theEncoder)
{	NSizeX		theObject;

	theObject.DecodeSelf(theEncoder);

	if (theObject.mWidth.GetPrecision() == kNPrecisionFloat64 || theObject.mHeight.GetPrecision() == kNPrecisionFloat64)
		return(NSize64(theObject.mWidth.GetFloat64(), theObject.mHeight.GetFloat64()));
	else
		return(NSize32(theObject.mWidth.GetFloat32(), theObject.mHeight.GetFloat32()));
}





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
//      NSizeX::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
void NSizeX::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNSizeWidthKey,  mWidth);
	theEncoder.EncodeNumber(kNSizeHeightKey, mHeight);
}





//============================================================================
//      NSizeX::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NSizeX::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	mWidth  = theEncoder.DecodeNumber(kNSizeWidthKey);
	mHeight = theEncoder.DecodeNumber(kNSizeHeightKey);
}



#endif // NSIZE_CPP



