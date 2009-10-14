/*	NAME:
		NColor.cpp

	DESCRIPTION:
		Color object.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCoreGraphicsUtilities.h"
#include "NMathUtilities.h"
#include "NColor.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const float kOneOver255								= 1.0f /   255.0f;
static const float kOneOver65535							= 1.0f / 65535.0f;





//============================================================================
//		NColor::NColor : Constructor.
//----------------------------------------------------------------------------
NColor::NColor(float r, float g, float b, float a)
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
{	UInt8	byteR, byteG, byteB, byteA;
	float	r, g, b, a;



	// Break down the color
	r = g = b = a = 0.0f;

	switch (theFormat) {
		case kFormatRGBA:
			if (sscanf(hexColor.GetUTF8(), "%2hhx%2hhx%2hhx%2hhx", &byteR, &byteG, &byteB, &byteA) == 4)
				{
				r = ((float) byteR) * kOneOver255;
				g = ((float) byteG) * kOneOver255;
				b = ((float) byteB) * kOneOver255;
				a = ((float) byteA) * kOneOver255;
				}
			break;

		default:
			NN_ASSERT_MSG("Unknown color format: %d", theFormat);
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
void NColor::GetColor(float &r, float &g, float &b, float &a) const
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
		case kFormatRGBA:
			theColor = (r << 24) | (g << 16) | (b << 8) | (a << 0);
			break;

		default:
			theColor = 0x000000FF;
			NN_ASSERT_MSG("Unknown color format: %d", theFormat);
			break;
		}
	
	return(theColor);
}





//============================================================================
//		NColor::GetColor : Get the color.
//----------------------------------------------------------------------------
void NColor::GetColor(RGBColor &theColor) const
{


	// Get the color
	theColor.red   = (UInt16) (mRed   * 65535.0f);
	theColor.green = (UInt16) (mGreen * 65535.0f);
	theColor.blue  = (UInt16) (mBlue  * 65535.0f);
}





//============================================================================
//		NColor::CopyColor : Get the color.
//----------------------------------------------------------------------------
CGColorRef NColor::CopyColor(void) const
{	float			theComponents[4];
	CGColorRef		cgColor;



	// Create the color
	theComponents[0] = mRed;
	theComponents[1] = mGreen;
	theComponents[2] = mBlue;
	theComponents[3] = mAlpha;

	cgColor = CGColorCreate(NCoreGraphicsUtilities::GetColorSpaceRGB(), theComponents);
	NN_ASSERT(cgColor != NULL);
	
	return(cgColor);
}





//============================================================================
//		NColor::SetColor : Set the color.
//----------------------------------------------------------------------------
void NColor::SetColor(float r, float g, float b, float a)
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
{	float	r, g, b, a;



	// Break down the color
	switch (theFormat) {
		case kFormatRGBA:
			r = ((float) ((theColor >> 24) & 0xFF)) * kOneOver255;
			g = ((float) ((theColor >> 16) & 0xFF)) * kOneOver255;
			b = ((float) ((theColor >>  8) & 0xFF)) * kOneOver255;
			a = ((float) ((theColor >>  0) & 0xFF)) * kOneOver255;
			break;

		default:
			r = g = b = a = 0.0f;
			NN_ASSERT_MSG("Unknown color format: %d", theFormat);
			break;
		}



	// Set the color
	SetColor(r, g, b, a);
}





//============================================================================
//		NColor::SetColor : Set the color.
//----------------------------------------------------------------------------
void NColor::SetColor(const RGBColor &theColor)
{


	// Set the color
	//
	// Since an RGBColor ignores alpha, we leave our existing value unchanged.
	mRed   = ((float) theColor.red)   * kOneOver65535;
	mGreen = ((float) theColor.green) * kOneOver65535;
	mBlue  = ((float) theColor.blue)  * kOneOver65535;
}





//============================================================================
//		NColor::SetColor : Set the color.
//----------------------------------------------------------------------------
void NColor::SetColor(CGColorRef cgColor)
{	const float		*theComponents;



	// Validate our parameters
	NN_ASSERT(cgColor != NULL);
	NN_ASSERT(CGColorGetNumberOfComponents(cgColor) == 4);



	// Set the color
	theComponents = CGColorGetComponents(cgColor);

	mRed   = theComponents[0];
	mGreen = theComponents[1];
	mBlue  = theComponents[2];
	mAlpha = theComponents[3];
}





//============================================================================
//		NColor::GetRed : Get the red value.
//----------------------------------------------------------------------------
float NColor::GetRed(void) const
{


	// Get the value
	return(mRed);
}





//============================================================================
//		NColor::GetGreen : Get the green value.
//----------------------------------------------------------------------------
float NColor::GetGreen(void) const
{


	// Get the value
	return(mGreen);
}





//============================================================================
//		NColor::GetBlue : Get the blue value.
//----------------------------------------------------------------------------
float NColor::GetBlue(void) const
{


	// Get the value
	return(mBlue);
}





//============================================================================
//		NColor::GetAlpha : Get the alpha value.
//----------------------------------------------------------------------------
float NColor::GetAlpha(void) const
{


	// Get the value
	return(mAlpha);
}





//============================================================================
//		NColor::SetRed : Set the red value.
//----------------------------------------------------------------------------
void NColor::SetRed(float theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mRed = theValue;
}





//============================================================================
//		NColor::SetGreen : Set the green value.
//----------------------------------------------------------------------------
void NColor::SetGreen(float theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mGreen = theValue;
}





//============================================================================
//		NColor::SetBlue : Set the blue value.
//----------------------------------------------------------------------------
void NColor::SetBlue(float theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mBlue = theValue;
}





//============================================================================
//		NColor::SetAlpha : Set the alpha value.
//----------------------------------------------------------------------------
void NColor::SetAlpha(float theValue)
{


	// Validate our parameters
	NN_ASSERT(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mAlpha = theValue;
}





//============================================================================
//		NColor::== : Are two colors equal?
//----------------------------------------------------------------------------
const bool NColor::operator == (const NColor &theColor) const
{


	// Compare the colors
	return(NMathUtilities::AreEqual(mRed,   theColor.mRed)   &&
		   NMathUtilities::AreEqual(mGreen, theColor.mGreen) && 
		   NMathUtilities::AreEqual(mBlue,  theColor.mBlue)  && 
		   NMathUtilities::AreEqual(mAlpha, theColor.mAlpha));
}





//============================================================================
//		NColor::!= : Are two colors not equal?
//----------------------------------------------------------------------------
const bool NColor::operator != (const NColor &theColor) const
{


	// Compare the colors
	return(NMathUtilities::NotEqual(mRed,   theColor.mRed)   ||
		   NMathUtilities::NotEqual(mGreen, theColor.mGreen) || 
		   NMathUtilities::NotEqual(mBlue,  theColor.mBlue)  || 
		   NMathUtilities::NotEqual(mAlpha, theColor.mAlpha));
}

