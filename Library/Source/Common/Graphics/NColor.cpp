/*	NAME:
		NColor.cpp

	DESCRIPTION:
		Color object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NColor.h"





//============================================================================
//		Public constants
//----------------------------------------------------------------------------
const NColor kColorBlack									= NColor(0.00f, 0.00f, 0.00f, 1.0f);
const NColor kColorBlue										= NColor(0.00f, 0.00f, 1.00f, 1.0f);
const NColor kColorBrown									= NColor(0.60f, 0.40f, 0.20f, 1.0f);
const NColor kColorClear									= NColor(1.00f, 1.00f, 1.00f, 0.0f);
const NColor kColorCyan										= NColor(0.00f, 1.00f, 1.00f, 1.0f);
const NColor kColorGray										= NColor(0.50f, 0.50f, 0.50f, 1.0f);
const NColor kColorGrayDark									= NColor(0.33f, 0.33f, 0.33f, 1.0f);
const NColor kColorGrayLight								= NColor(0.66f, 0.66f, 0.66f, 1.0f);
const NColor kColorGreen									= NColor(0.00f, 1.00f, 0.00f, 1.0f);
const NColor kColorMagenta									= NColor(1.00f, 0.00f, 1.00f, 1.0f);
const NColor kColorOrange									= NColor(1.00f, 0.50f, 0.00f, 1.0f);
const NColor kColorPurple									= NColor(0.50f, 0.00f, 0.50f, 1.0f);
const NColor kColorRed										= NColor(1.00f, 0.00f, 0.00f, 1.0f);
const NColor kColorWhite									= NColor(1.00f, 1.00f, 1.00f, 1.0f);
const NColor kColorYellow									= NColor(1.00f, 1.00f, 0.00f, 1.0f);





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const float kOneOver255								= 1.0f /   255.0f;
static const float kOneOver65535							= 1.0f / 65535.0f;





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
NColor::NColor(UInt32 theColor, ColorFormat theFormat)
{


	// Initialize ourselves
	SetColor(theColor, theFormat);
}





//============================================================================
//		NColor::NColor : Constructor.
//----------------------------------------------------------------------------
NColor::NColor(const NString &hexColor, ColorFormat theFormat)
{	UInt32		byteR, byteG, byteB, byteA;		// dair, Windows sscanf uses 4 bytes for hhx, corrupts stack
	Float32		r, g, b, a;



	// Break down the color
	r = g = b = a = 0.0f;

	switch (theFormat) {
		case kColorRGBA:
			if (sscanf(hexColor.GetUTF8(), "%2hhx%2hhx%2hhx%2hhx", &byteR, &byteG, &byteB, &byteA) == 4)
				{
				r = ((Float32) byteR) * kOneOver255;
				g = ((Float32) byteG) * kOneOver255;
				b = ((Float32) byteB) * kOneOver255;
				a = ((Float32) byteA) * kOneOver255;
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
UInt32 NColor::GetColor(ColorFormat theFormat) const
{	UInt32	theColor, r, g, b, a;



	// Get the color
	r = (UInt8) (mRed   * 255.0f);
	g = (UInt8) (mGreen * 255.0f);
	b = (UInt8) (mBlue  * 255.0f);
	a = (UInt8) (mAlpha * 255.0f);



	// Encode the color
	switch (theFormat) {
		case kColorRGBA:
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
void NColor::SetColor(UInt32 theColor, ColorFormat theFormat)
{	Float32		r, g, b, a;



	// Break down the color
	switch (theFormat) {
		case kColorRGBA:
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
//		NColor::Compare : Compare two objects.
//----------------------------------------------------------------------------
#pragma mark -
NComparison NColor::Compare(const NComparable &theObject) const
{	const NColor	*theColor = dynamic_cast<const NColor*>(&theObject);



	// Validate our parameters
	NN_ASSERT(theColor != NULL);



	// Compare the colors
	//
	// Although colors have equality, they do not have any real ordering.
	if (NMathUtilities::AreEqual(mRed,   theColor->mRed)   &&
		NMathUtilities::AreEqual(mGreen, theColor->mGreen) && 
		NMathUtilities::AreEqual(mBlue,  theColor->mBlue)  && 
		NMathUtilities::AreEqual(mAlpha, theColor->mAlpha))
		return(kNCompareEqualTo);

	if (mRed   < theColor->mRed   &&
		mGreen < theColor->mGreen &&
		mBlue  < theColor->mBlue  &&
		mAlpha < theColor->mAlpha)
		return(kNCompareEqualTo);
	
	return(kNCompareGreaterThan);
}

