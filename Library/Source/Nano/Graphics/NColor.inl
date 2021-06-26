/*	NAME:
		NColor.inl

	DESCRIPTION:
		Color object.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NDebug.h"

// Nano
#include "NStdMath.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
// Conversion
static constexpr float kNOneOver255                         = 1.0f / 255.0f;





//=============================================================================
//		NColor::NColor : Constructor.
//-----------------------------------------------------------------------------
constexpr NColor::NColor(float32_t r, float32_t g, float32_t b, float32_t a)
	: mRed(r)
	, mGreen(g)
	, mBlue(b)
	, mAlpha(a)
{
}





//=============================================================================
//		NColor::NColor : Constructor.
//-----------------------------------------------------------------------------
inline NColor::NColor(uint32_t theColor, NColorFormat theFormat)
	: mRed(0.0f)
	, mGreen(0.0f)
	, mBlue(0.0f)
	, mAlpha(0.0f)
{


	// Initialize ourselves
	SetColor(theColor, theFormat);
}





//=============================================================================
//		NColor::NColor : Constructor.
//-----------------------------------------------------------------------------
constexpr NColor::NColor()
	: mRed(0.0f)
	, mGreen(0.0f)
	, mBlue(0.0f)
	, mAlpha(1.0f)
{
}





//=============================================================================
//		NColor::IsOpaque : Is the color opaque?
//-----------------------------------------------------------------------------
constexpr bool NColor::IsOpaque() const
{


	// Test the alpha
	return mAlpha == 1.0f;
}





//=============================================================================
//		NColor::IsTransparent : Is the color transparent?
//-----------------------------------------------------------------------------
constexpr bool NColor::IsTransparent() const
{


	// Test the alpha
	return mAlpha == 0.0f;
}





//=============================================================================
//		NColor::GetColor : Get the color.
//-----------------------------------------------------------------------------
constexpr void NColor::GetColor(float32_t& r, float32_t& g, float32_t& b, float32_t& a) const
{


	// Get the color
	r = mRed;
	g = mGreen;
	b = mBlue;
	a = mAlpha;
}





//=============================================================================
//		NColor::GetColor : Get the color.
//-----------------------------------------------------------------------------
constexpr uint32_t NColor::GetColor(NColorFormat theFormat) const
{


	// Get the color
	uint32_t r        = uint8_t(mRed * 255.0f);
	uint32_t g        = uint8_t(mGreen * 255.0f);
	uint32_t b        = uint8_t(mBlue * 255.0f);
	uint32_t a        = uint8_t(mAlpha * 255.0f);
	uint32_t theColor = 0;

	switch (theFormat)
	{
		case NColorFormat::ARGB:
			theColor = (a << 24) | (r << 16) | (g << 8) | (b << 0);
			break;

		case NColorFormat::RGBA:
			theColor = (r << 24) | (g << 16) | (b << 8) | (a << 0);
			break;
	}

	return theColor;
}





//=============================================================================
//		NColor::SetColor : Set the color.
//-----------------------------------------------------------------------------
inline void NColor::SetColor(float32_t r, float32_t g, float32_t b, float32_t a)
{


	// Set the color
	SetRed(r);
	SetGreen(g);
	SetBlue(b);
	SetAlpha(a);
}





//=============================================================================
//		NColor::SetColor : Set the color.
//-----------------------------------------------------------------------------
inline void NColor::SetColor(uint32_t theColor, NColorFormat theFormat)
{


	// Break down the color
	float32_t r = 0.0f;
	float32_t g = 0.0f;
	float32_t b = 0.0f;
	float32_t a = 0.0f;

	switch (theFormat)
	{
		case NColorFormat::ARGB:
			a = float32_t((theColor >> 24) & 0xFF) * kNOneOver255;
			r = float32_t((theColor >> 16) & 0xFF) * kNOneOver255;
			g = float32_t((theColor >> 8) & 0xFF) * kNOneOver255;
			b = float32_t((theColor >> 0) & 0xFF) * kNOneOver255;
			break;

		case NColorFormat::RGBA:
			r = float32_t((theColor >> 24) & 0xFF) * kNOneOver255;
			g = float32_t((theColor >> 16) & 0xFF) * kNOneOver255;
			b = float32_t((theColor >> 8) & 0xFF) * kNOneOver255;
			a = float32_t((theColor >> 0) & 0xFF) * kNOneOver255;
			break;
	}



	// Set the color
	SetColor(r, g, b, a);
}





//=============================================================================
//		NColor::GetRed : Get the red value.
//-----------------------------------------------------------------------------
constexpr float32_t NColor::GetRed() const
{


	// Get the value
	return mRed;
}





//=============================================================================
//		NColor::GetGreen : Get the green value.
//-----------------------------------------------------------------------------
constexpr float32_t NColor::GetGreen() const
{


	// Get the value
	return mGreen;
}





//=============================================================================
//		NColor::GetBlue : Get the blue value.
//-----------------------------------------------------------------------------
constexpr float32_t NColor::GetBlue() const
{


	// Get the value
	return mBlue;
}





//=============================================================================
//		NColor::GetAlpha : Get the alpha value.
//-----------------------------------------------------------------------------
constexpr float32_t NColor::GetAlpha() const
{


	// Get the value
	return mAlpha;
}





//=============================================================================
//		NColor::SetRed : Set the red value.
//-----------------------------------------------------------------------------
inline void NColor::SetRed(float32_t theValue)
{


	// Validate our parameters
	NN_REQUIRE(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mRed = theValue;
}





//=============================================================================
//		NColor::SetGreen : Set the green value.
//-----------------------------------------------------------------------------
inline void NColor::SetGreen(float32_t theValue)
{


	// Validate our parameters
	NN_REQUIRE(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mGreen = theValue;
}





//=============================================================================
//		NColor::SetBlue : Set the blue value.
//-----------------------------------------------------------------------------
inline void NColor::SetBlue(float32_t theValue)
{


	// Validate our parameters
	NN_REQUIRE(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mBlue = theValue;
}





//=============================================================================
//		NColor::SetAlpha : Set the alpha value.
//-----------------------------------------------------------------------------
inline void NColor::SetAlpha(float32_t theValue)
{


	// Validate our parameters
	NN_REQUIRE(theValue >= 0.0f && theValue <= 1.0f);



	// Set the value
	mAlpha = theValue;
}





//=============================================================================
//		NColor::Interpolate : Interpolate between two colors.
//-----------------------------------------------------------------------------
inline NColor NColor::Interpolate(const NColor& startColor,
								  const NColor& endColor,
								  float32_t     theFraction)
{


	// Validate our parameters
	NN_REQUIRE(theFraction >= 0.0f && theFraction <= 1.0f);



	// Interpolate the colors
	NColor theResult;

	theResult.mRed   = nstd::lerp(startColor.mRed, endColor.mRed, theFraction);
	theResult.mGreen = nstd::lerp(startColor.mGreen, endColor.mGreen, theFraction);
	theResult.mBlue  = nstd::lerp(startColor.mBlue, endColor.mBlue, theFraction);
	theResult.mAlpha = nstd::lerp(startColor.mAlpha, endColor.mAlpha, theFraction);

	return theResult;
}





#pragma mark NMixinComparable
//=============================================================================
//		NColor::CompareEqual : Perform an equality comparison.
//-----------------------------------------------------------------------------
constexpr bool NColor::CompareEqual(const NColor& theColor) const
{


	// Compare the color
	return (mRed == theColor.mRed) && (mGreen == theColor.mGreen) && (mBlue == theColor.mBlue) &&
		   (mAlpha == theColor.mAlpha);
}





//=============================================================================
//		NColor::CompareOrder : Perform a three-way comparison.
//-----------------------------------------------------------------------------
constexpr NComparison NColor::CompareOrder(const NColor& theColor) const
{


	// Order by component
	NComparison theResult = NCompare(mAlpha, theColor.mAlpha);

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mRed, theColor.mRed);
	}

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mGreen, theColor.mGreen);
	}

	if (theResult == NComparison::EqualTo)
	{
		theResult = NCompare(mBlue, theColor.mBlue);
	}

	return theResult;
}
