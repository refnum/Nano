/*	NAME:
		NRectangle.cpp

	DESCRIPTION:
		Rectangle object.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NString.h"
#include "NEncoder.h"
#include "NRectangle.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NRectangle kNRectangleZero;





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNRectangleOriginXKey							= "x";
static const NString kNRectangleOriginYKey							= "y";
static const NString kNRectangleSizeWidthKey						= "width";
static const NString kNRectangleSizeHeightKey						= "height";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NRectangle);





//============================================================================
//		NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const Float32 x, Float32 y, Float32 width, Float32 height)
{


	// Initialize ourselves
	origin = NPoint(x, y);
	size   = NSize(width, height);
}





//============================================================================
//		NRectangle::NRectangle : Constructor.
//----------------------------------------------------------------------------
NRectangle::NRectangle(const NPoint &valOrigin, const NSize &valSize)
{


	// Initialize ourselves
	origin = valOrigin;
	size   = valSize;
}





//============================================================================
//		NRectangle::~NRectangle : Destructor.
//----------------------------------------------------------------------------
NRectangle::~NRectangle(void)
{
}





//============================================================================
//		NRectangle::Clear : Clear the Rectangle.
//----------------------------------------------------------------------------
void NRectangle::Clear(void)
{


	// Clear the Rectangle
	origin.Clear();
	size.Clear();
}





//============================================================================
//		NRectangle::IsEmpty : Is the rectangle empty?
//----------------------------------------------------------------------------
bool NRectangle::IsEmpty(void) const
{


	// Test the rectangle
	return(size.IsEmpty());
}





//============================================================================
//		NRectangle::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NRectangle::Compare(const NRectangle &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = size.Compare(theValue.size);
		
	if (theResult == kNCompareEqualTo)
		theResult = origin.Compare(theValue.origin);

	return(theResult);
}





//============================================================================
//		NRectangle::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NRectangle::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{x=%g, y=%g, w=%g, h=%g}", origin.x, origin.y, size.width, size.height);

	return(theResult);
}





//============================================================================
//      NRectangle::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
#pragma mark -
void NRectangle::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNRectangleOriginXKey,    origin.x);
	theEncoder.EncodeNumber(kNRectangleOriginYKey,    origin.y);
	theEncoder.EncodeNumber(kNRectangleSizeWidthKey,  size.width);
	theEncoder.EncodeNumber(kNRectangleSizeHeightKey, size.height);
}





//============================================================================
//      NRectangle::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NRectangle::DecodeSelf(const NEncoder &theEncoder)
{


	// Decode the object
	origin.x    = theEncoder.DecodeNumber(kNRectangleOriginXKey).GetFloat32();
	origin.y    = theEncoder.DecodeNumber(kNRectangleOriginYKey).GetFloat32();
	size.width  = theEncoder.DecodeNumber(kNRectangleSizeWidthKey).GetFloat32();
	size.height = theEncoder.DecodeNumber(kNRectangleSizeHeightKey).GetFloat32();
}


