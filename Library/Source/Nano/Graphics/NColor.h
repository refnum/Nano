/*	NAME:
		NColor.h

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
#ifndef NCOLOR_H
#define NCOLOR_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NMixinComparable.h"
#include "NanoMacros.h"
#include "NanoTypes.h"





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
// Color formats
//
// A color format defines the order of components within a representation,
// not their order in memory.
//
// For example, a uint32_t containing NColorFormat::ARGB would be stored as:
//
//		NN_ENDIAN_BIG			[AA] [RR] [GG] [BB]
//		NN_ENDIAN_LITTLE		[BB] [GG] [RR] [AA]
//
// In both layouts the alpha byte could be accessed by a right shift of 24.
enum class NColorFormat
{
	ARGB,
	RGBA
};





//=============================================================================
//		Types
//-----------------------------------------------------------------------------
// Forward declaration
class NString;





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NN_EMPTY_BASE NColor final : public NMixinComparable<NColor>
{
public:
	constexpr explicit                  NColor(float32_t r, float32_t g, float32_t b, float32_t a = 1.0f);
	explicit                            NColor(uint32_t theColor,       NColorFormat theFormat = NColorFormat::RGBA);
										NColor(const NString& hexColor, NColorFormat theFormat = NColorFormat::RGBA);

	constexpr                           NColor();


	// Test the alpha
	constexpr bool                      IsOpaque()      const;
	constexpr bool                      IsTransparent() const;


	// Get/set the color
	constexpr void                      GetColor(float32_t& r, float32_t& g, float32_t& b, float32_t& a) const;
	constexpr uint32_t                  GetColor(NColorFormat theFormat = NColorFormat::RGBA)            const;

	void                                SetColor(float32_t r, float32_t g, float32_t b, float32_t a = 1.0f);
	void                                SetColor(uint32_t theColor, NColorFormat theFormat = NColorFormat::RGBA);


	// Get/set the components
	constexpr float32_t                 GetRed()   const;
	constexpr float32_t                 GetGreen() const;
	constexpr float32_t                 GetBlue()  const;
	constexpr float32_t                 GetAlpha() const;

	void                                SetRed(  float32_t theValue);
	void                                SetGreen(float32_t theValue);
	void                                SetBlue( float32_t theValue);
	void                                SetAlpha(float32_t theValue);


	// Interpolate between two colors
	static NColor                       Interpolate(const NColor& startColor, const NColor& endColor, float theFraction);


public:
	// NMixinComparable
	constexpr bool                      CompareEqual(const NColor& theColor) const;
	constexpr NComparison               CompareOrder(const NColor& theColor) const;


private:
	float32_t                           mRed;
	float32_t                           mGreen;
	float32_t                           mBlue;
	float32_t                           mAlpha;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NColor.inl"



// Colors
inline constexpr NColor kNColorBlack     = NColor(0.00f, 0.00f, 0.00f, 1.0f);
inline constexpr NColor kNColorBlue      = NColor(0.00f, 0.00f, 1.00f, 1.0f);
inline constexpr NColor kNColorBrown     = NColor(0.60f, 0.40f, 0.20f, 1.0f);
inline constexpr NColor kNColorClear     = NColor(       1.00f, 1.00f, 1.00f, 0.0f);
inline constexpr NColor kNColorCyan      = NColor(0.00f, 1.00f, 1.00f,               1.0f);
inline constexpr NColor kNColorGray      = NColor(0.50f, 0.50f, 0.50f,               1.0f);
inline constexpr NColor kNColorGrayDark  = NColor(0.33f, 0.33f, 0.33f,               1.0f);
inline constexpr NColor kNColorGrayLight = NColor(0.66f, 0.66f, 0.66f,               1.0f);
inline constexpr NColor kNColorGreen     = NColor(0.00f, 1.00f, 0.00f,               1.0f);
inline constexpr NColor kNColorMagenta   = NColor(       1.00f,        0.00f, 1.00f, 1.0f);
inline constexpr NColor kNColorOrange    = NColor(       1.00f, 0.50f, 0.00f,        1.0f);
inline constexpr NColor kNColorPurple    = NColor(0.50f, 0.00f, 0.50f, 1.0f);
inline constexpr NColor kNColorRed       = NColor(1.00f, 0.00f, 0.00f, 1.0f);
inline constexpr NColor kNColorWhite     = NColor(1.00f, 1.00f, 1.00f, 1.0f);
inline constexpr NColor kNColorYellow    = NColor(1.00f, 1.00f, 0.00f, 1.0f);


#endif // NCOLOR_H
