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
#include "NEncoder.h"
#include "NString.h"
#include "NSize.h"





//============================================================================
//		Public onstants
//----------------------------------------------------------------------------
const NSize kNSizeZero;





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kNSizeWidthKey									= "width";
static const NString kNSizeHeightKey								= "height";





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
DEFINE_NENCODABLE(NSize);





//============================================================================
//		NSize::NSize : Constructor.
//----------------------------------------------------------------------------
NSize::NSize(Float32 valWidth, Float32 valHeight)
{


	// Initialize ourselves
	width  = valWidth;
	height = valHeight;
}





//============================================================================
//		NSize::~NSize : Destructor.
//----------------------------------------------------------------------------
NSize::~NSize(void)
{
}





//============================================================================
//		NSize::Clear : Clear the size.
//----------------------------------------------------------------------------
void NSize::Clear(void)
{


	// Clear the size
	width  = 0.0f;
	height = 0.0f;
}





//============================================================================
//		NSize::IsEmpty : Is the size empty?
//----------------------------------------------------------------------------
bool NSize::IsEmpty(void) const
{


	// Test the size
	return(NMathUtilities::IsZero(width) && NMathUtilities::IsZero(height));
}





//============================================================================
//		NSize::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NSize::Compare(const NSize &theValue) const
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
//		NSize::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NSize::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{w=%g, h=%g}", width, height);

	return(theResult);
}





//============================================================================
//      NSize::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
#pragma mark -
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



