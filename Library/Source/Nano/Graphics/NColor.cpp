/*	NAME:
		NColor.cpp

	DESCRIPTION:
		Color object.
	
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
#include "NEncoder.h"
#include "NColor.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NColor kNColorBlack									= NColor(0.00f, 0.00f, 0.00f, 1.0f);
const NColor kNColorBlue									= NColor(0.00f, 0.00f, 1.00f, 1.0f);
const NColor kNColorBrown									= NColor(0.60f, 0.40f, 0.20f, 1.0f);
const NColor kNColorClear									= NColor(1.00f, 1.00f, 1.00f, 0.0f);
const NColor kNColorCyan									= NColor(0.00f, 1.00f, 1.00f, 1.0f);
const NColor kNColorGray									= NColor(0.50f, 0.50f, 0.50f, 1.0f);
const NColor kNColorGrayDark								= NColor(0.33f, 0.33f, 0.33f, 1.0f);
const NColor kNColorGrayLight								= NColor(0.66f, 0.66f, 0.66f, 1.0f);
const NColor kNColorGreen									= NColor(0.00f, 1.00f, 0.00f, 1.0f);
const NColor kNColorMagenta									= NColor(1.00f, 0.00f, 1.00f, 1.0f);
const NColor kNColorOrange									= NColor(1.00f, 0.50f, 0.00f, 1.0f);
const NColor kNColorPurple									= NColor(0.50f, 0.00f, 0.50f, 1.0f);
const NColor kNColorRed										= NColor(1.00f, 0.00f, 0.00f, 1.0f);
const NColor kNColorWhite									= NColor(1.00f, 1.00f, 1.00f, 1.0f);
const NColor kNColorYellow									= NColor(1.00f, 1.00f, 0.00f, 1.0f);





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Keys
//
// These values are considered to be fixed, and will never change.
static const NString kNColorRedKey							= "r";
static const NString kNColorGreenKey						= "g";
static const NString kNColorBlueKey							= "b";
static const NString kNColorAlphaKey						= "a";


// Misc
static const float kOneOver255								= 1.0f /   255.0f;
static const float kOneOver65535							= 1.0f / 65535.0f;





//============================================================================
//		Implementation
//----------------------------------------------------------------------------
NENCODABLE_DEFINE(NColor);





//============================================================================
//		NColor::NColor : Constructor.
//----------------------------------------------------------------------------
NColor::NColor(Float32 r, Float32 g, Float32 b, Float32 a)
{


	// Initialize ourselves
	SetColor(r, g, b, a);
}





//============================================================================
//		NColor::NColor : Constructor.
//----------------------------------------------------------------------------
NColor::NColor(UInt32 theColor, NColorFormat theFormat)
{


	// Initialize ourselves
	SetColor(theColor, theFormat);
}





//============================================================================
//		NColor::NColor : Constructor.
//----------------------------------------------------------------------------
NColor::NColor(const NString &hexColor, NColorFormat theFormat)
{	unsigned int	valueR, valueG, valueB, valueA;
	Float32			r, g, b, a;



	// Break down the color
	r = g = b = a = 0.0f;

	switch (theFormat) {
		case kNColorARGB:
			if (sscanf(hexColor.GetUTF8(), "%2x%2x%2x%2x", &valueA, &valueR, &valueG, &valueB) == 4)
				{
				r = ((Float32) valueR) * kOneOver255;
				g = ((Float32) valueG) * kOneOver255;
				b = ((Float32) valueB) * kOneOver255;
				a = ((Float32) valueA) * kOneOver255;
				}
			break;

		case kNColorRGBA:
			if (sscanf(hexColor.GetUTF8(), "%2x%2x%2x%2x", &valueR, &valueG, &valueB, &valueA) == 4)
				{
				r = ((Float32) valueR) * kOneOver255;
				g = ((Float32) valueG) * kOneOver255;
				b = ((Float32) valueB) * kOneOver255;
				a = ((Float32) valueA) * kOneOver255;
				}
			break;

		default:
			NN_LOG("Unknown color format: %d", theFormat);
			break;
		}



	// Initialize ourselves
	SetColor(r, g, b, a);
}





//============================================================================
//		NColor::NColor : Constructor.
//----------------------------------------------------------------------------
NColor::NColor(void)
{


	// Initialize ourselves
	SetColor(1.0f, 0.0f, 0.0f, 1.0f);
}





//============================================================================
//		NColor::~NColor : Destructor.
//----------------------------------------------------------------------------
NColor::~NColor(void)
{
}





//============================================================================
//		NColor::IsOpaque : Is the color opaque?
//----------------------------------------------------------------------------
bool NColor::IsOpaque(void) const
{


	// Test the alpha
	return(mAlpha >= 1.0f);
}





//============================================================================
//		NColor::IsTransparent : Is the color transparent?
//----------------------------------------------------------------------------
bool NColor::IsTransparent(void) const
{


	// Test the alpha
	return(mAlpha <= 0.0f);
}





//============================================================================
//		NColor::Compare : Compare the value.
//----------------------------------------------------------------------------
NComparison NColor::Compare(const NColor &theValue) const
{	NComparison		theResult;



	// Compare the value
	//
	// We have no natural order, so the only real comparison is equality.
	theResult = GetComparison(mRed, theValue.mRed);
		
	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(mGreen, theValue.mGreen);

	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(mBlue, theValue.mBlue);

	if (theResult == kNCompareEqualTo)
		theResult = GetComparison(mAlpha, theValue.mAlpha);

	return(theResult);
}





//============================================================================
//		NColor::GetColor : Get the color.
//----------------------------------------------------------------------------
void NColor::GetColor(Float32 &r, Float32 &g, Float32 &b, Float32 &a) const
{


	// Get the color
	r = mRed;
	g = mGreen;
	b = mBlue;
	a = mAlpha;
}





//============================================================================
//		NColor::GetColor : Get the color.
//----------------------------------------------------------------------------
UInt32 NColor::GetColor(NColorFormat theFormat) const
{	UInt32	theColor, r, g, b, a;



	// Get the color
	r = (UInt8) (mRed   * 255.0f);
	g = (UInt8) (mGreen * 255.0f);
	b = (UInt8) (mBlue  * 255.0f);
	a = (UInt8) (mAlpha * 255.0f);



	// Encode the color
	switch (theFormat) {
		case kNColorARGB:
			theColor = (a << 24) | (r << 16) | (g << 8) | (b << 0);
			break;

		case kNColorRGBA:
			theColor = (r << 24) | (g << 16) | (b << 8) | (a << 0);
			break;

		default:
			theColor = 0x000000FF;
			NN_LOG("Unknown color format: %d", theFormat);
			break;
		}
	
	return(theColor);
}





//============================================================================
//		NColor::SetColor : Set the color.
//----------------------------------------------------------------------------
void NColor::SetColor(Float32 r, Float32 g, Float32 b, Float32 a)
{


	// Set the color
	SetRed(  r);
	SetGreen(g);
	SetBlue( b);
	SetAlpha(a);
}





//============================================================================
//		NColor::SetColor : Set the color.
//----------------------------------------------------------------------------
void NColor::SetColor(UInt32 theColor, NColorFormat theFormat)
{	Float32		r, g, b, a;



	// Break down the color
	switch (theFormat) {
		case kNColorARGB:
			a = ((Float32) ((theColor >> 24) & 0xFF)) * kOneOver255;
			r = ((Float32) ((theColor >> 16) & 0xFF)) * kOneOver255;
			g = ((Float32) ((theColor >>  8) & 0xFF)) * kOneOver255;
			b = ((Float32) ((theColor >>  0) & 0xFF)) * kOneOver255;
			break;

		case kNColorRGBA:
			r = ((Float32) ((theColor >> 24) & 0xFF)) * kOneOver255;
			g = ((Float32) ((theColor >> 16) & 0xFF)) * kOneOver255;
			b = ((Float32) ((theColor >>  8) & 0xFF)) * kOneOver255;
			a = ((Float32) ((theColor >>  0) & 0xFF)) * kOneOver255;
			break;

		default:
			r = g = b = a = 0.0f;
			NN_LOG("Unknown color format: %d", theFormat);
			break;
		}



	// Set the color
	SetColor(r, g, b, a);
}





//============================================================================
//		NColor::GetRed : Get the red value.
//----------------------------------------------------------------------------
Float32 NColor::GetRed(void) const
{


	// Get the value
	return(mRed);
}





//============================================================================
//		NColor::GetGreen : Get the green value.
//----------------------------------------------------------------------------
Float32 NColor::GetGreen(void) const
{


	// Get the value
	return(mGreen);
}





//============================================================================
//		NColor::GetBlue : Get the blue value.
//----------------------------------------------------------------------------
Float32 NColor::GetBlue(void) const
{


	// Get the value
	return(mBlue);
}





//============================================================================
//		NColor::GetAlpha : Get the alpha value.
//----------------------------------------------------------------------------
Float32 NColor::GetAlpha(void) const
{


	// Get the value
	return(mAlpha);
}





//============================================================================
//		NColor::SetRed : Set the red value.
//----------------------------------------------------------------------------
void NColor::SetRed(Float32 theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mRed = theValue;
}





//============================================================================
//		NColor::SetGreen : Set the green value.
//----------------------------------------------------------------------------
void NColor::SetGreen(Float32 theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mGreen = theValue;
}





//============================================================================
//		NColor::SetBlue : Set the blue value.
//----------------------------------------------------------------------------
void NColor::SetBlue(Float32 theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mBlue = theValue;
}





//============================================================================
//		NColor::SetAlpha : Set the alpha value.
//----------------------------------------------------------------------------
void NColor::SetAlpha(Float32 theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mAlpha = theValue;
}





//============================================================================
//		NColor::NFormatArgument : NFormatArgument operator.
//----------------------------------------------------------------------------
NColor::operator NFormatArgument(void) const
{	NString		theResult;



	// Get the value
	theResult.Format("{r=%g, g=%g, b=%g, a=%g}", mRed, mGreen, mBlue, mAlpha);

	return(theResult);
}





//============================================================================
//      NColor::Interpolate : Interpolate between two colors.
//----------------------------------------------------------------------------
NColor NColor::Interpolate(const NColor &startColor, const NColor &endColor, float theProgress)
{	NColor		theResult;



	// Validate our parameters
	NN_ASSERT(theProgress >= 0.0f && theProgress <= 1.0f);



	// Interpolate the colors
	theResult.mRed   = NN_LERP_VALUE(startColor.mRed,   endColor.mRed,   theProgress);
	theResult.mGreen = NN_LERP_VALUE(startColor.mGreen, endColor.mGreen, theProgress);
	theResult.mBlue  = NN_LERP_VALUE(startColor.mBlue,  endColor.mBlue,  theProgress);
	theResult.mAlpha = NN_LERP_VALUE(startColor.mAlpha, endColor.mAlpha, theProgress);

	return(theResult);
}





//============================================================================
//      NColor::EncodeSelf : Encode the object.
//----------------------------------------------------------------------------
#pragma mark -
void NColor::EncodeSelf(NEncoder &theEncoder) const
{


	// Encode the object
	theEncoder.EncodeNumber(kNColorRedKey,   mRed);
	theEncoder.EncodeNumber(kNColorGreenKey, mGreen);
	theEncoder.EncodeNumber(kNColorBlueKey,  mBlue);
	theEncoder.EncodeNumber(kNColorAlphaKey, mAlpha);
}





//============================================================================
//      NColor::DecodeSelf : Decode the object.
//----------------------------------------------------------------------------
void NColor::DecodeSelf(const NEncoder &theEncoder)
{


	// Encode the object
	mRed   = theEncoder.DecodeNumber(kNColorRedKey  ).GetFloat32();
	mGreen = theEncoder.DecodeNumber(kNColorGreenKey).GetFloat32();
	mBlue  = theEncoder.DecodeNumber(kNColorBlueKey ).GetFloat32();
	mAlpha = theEncoder.DecodeNumber(kNColorAlphaKey).GetFloat32();
}



