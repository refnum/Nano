/*	NAME:
		TColor.cpp

	DESCRIPTION:
		NColor tests.

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
#include "NColor.h"
#include "NFormat.h"
#include "NTestFixture.h"





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TColor)
{
	NColor theColor;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "Default")
{


	// Perform the test
	REQUIRE(theColor.GetRed() == 0.0f);
	REQUIRE(theColor.GetGreen() == 0.0f);
	REQUIRE(theColor.GetBlue() == 0.0f);
	REQUIRE(theColor.GetAlpha() == 1.0f);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "Constructor")
{


	// Perform the test
	theColor = NColor(1.0f, 0.5f, 1.0f, 0.5f);
	REQUIRE(theColor.GetRed() == 1.0f);
	REQUIRE(theColor.GetGreen() == 0.5f);
	REQUIRE(theColor.GetBlue() == 1.0f);
	REQUIRE(theColor.GetAlpha() == 0.5f);

	theColor = NColor(0.5f, 0.5f, 0.5f);
	REQUIRE(theColor.GetRed() == 0.5f);
	REQUIRE(theColor.GetGreen() == 0.5f);
	REQUIRE(theColor.GetBlue() == 0.5f);
	REQUIRE(theColor.GetAlpha() == 1.0f);

	theColor = NColor(0.5f, 0.5f, 0.5f);
	REQUIRE(theColor.GetRed() == 0.5f);
	REQUIRE(theColor.GetGreen() == 0.5f);
	REQUIRE(theColor.GetBlue() == 0.5f);
	REQUIRE(theColor.GetAlpha() == 1.0f);


	theColor = NColor(0xFF00FF00);
	REQUIRE(theColor.GetRed() == 1.0f);
	REQUIRE(theColor.GetGreen() == 0.0f);
	REQUIRE(theColor.GetBlue() == 1.0f);
	REQUIRE(theColor.GetAlpha() == 0.0f);

	theColor = NColor(0xFF00FF00, NColorFormat::RGBA);
	REQUIRE(theColor.GetRed() == 1.0f);
	REQUIRE(theColor.GetGreen() == 0.0f);
	REQUIRE(theColor.GetBlue() == 1.0f);
	REQUIRE(theColor.GetAlpha() == 0.0f);

	theColor = NColor(0xFF00FF00, NColorFormat::ARGB);
	REQUIRE(theColor.GetAlpha() == 1.0f);
	REQUIRE(theColor.GetRed() == 0.0f);
	REQUIRE(theColor.GetGreen() == 1.0f);
	REQUIRE(theColor.GetBlue() == 0.0f);

	theColor = NColor("FF00FF00", NColorFormat::RGBA);
	REQUIRE(theColor.GetRed() == 1.0f);
	REQUIRE(theColor.GetGreen() == 0.0f);
	REQUIRE(theColor.GetBlue() == 1.0f);
	REQUIRE(theColor.GetAlpha() == 0.0f);

	theColor = NColor("FF00FF00", NColorFormat::ARGB);
	REQUIRE(theColor.GetAlpha() == 1.0f);
	REQUIRE(theColor.GetRed() == 0.0f);
	REQUIRE(theColor.GetGreen() == 1.0f);
	REQUIRE(theColor.GetBlue() == 0.0f);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "IsOpaque")
{


	// Perform the test
	theColor.SetAlpha(1.0f);
	REQUIRE(theColor.IsOpaque());

	theColor.SetAlpha(0.99f);
	REQUIRE(!theColor.IsOpaque());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "IsTransparent")
{


	// Perform the test
	theColor.SetAlpha(0.0f);
	REQUIRE(theColor.IsTransparent());

	theColor.SetAlpha(0.01f);
	REQUIRE(!theColor.IsTransparent());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "GetColor")
{


	// Perform the test
	float32_t r, g, b, a;
	theColor = NColor(1.0f, 0.8f, 0.6f, 0.4f);

	theColor.GetColor(r, g, b, a);
	REQUIRE(r == 1.0f);
	REQUIRE(g == 0.8f);
	REQUIRE(b == 0.6f);
	REQUIRE(a == 0.4f);

	REQUIRE(theColor.GetColor(NColorFormat::RGBA) == 0xFFCC9966);
	REQUIRE(theColor.GetColor(NColorFormat::ARGB) == 0x66FFCC99);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "SetColor")
{


	// Perform the test
	float32_t r, g, b, a;
	theColor.SetColor(1.0f, 0.8f, 0.6f, 0.4f);

	theColor.GetColor(r, g, b, a);
	REQUIRE(r == 1.0f);
	REQUIRE(g == 0.8f);
	REQUIRE(b == 0.6f);
	REQUIRE(a == 0.4f);

	theColor.SetColor(0xFFCC9966, NColorFormat::RGBA);
	REQUIRE(theColor.GetColor(NColorFormat::RGBA) == 0xFFCC9966);

	theColor.SetColor(0x66FFCC99, NColorFormat::ARGB);
	REQUIRE(theColor.GetColor(NColorFormat::ARGB) == 0x66FFCC99);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "GetComponents")
{


	// Perform the test
	theColor = NColor(1.0f, 0.8f, 0.6f, 0.4f);

	float32_t r = theColor.GetRed();
	REQUIRE(r == 1.0f);

	float32_t g = theColor.GetGreen();
	REQUIRE(g == 0.8f);

	float32_t b = theColor.GetBlue();
	REQUIRE(b == 0.6f);

	float32_t a = theColor.GetAlpha();
	REQUIRE(a == 0.4f);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "SetComponents")
{


	// Perform the test
	theColor.SetRed(1.0f);
	theColor.SetGreen(0.8f);
	theColor.SetBlue(0.6f);
	theColor.SetAlpha(0.4f);
	REQUIRE(theColor == NColor(1.0f, 0.8f, 0.6f, 0.4f));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "Interpolate")
{


	// Perform the test
	NColor colourA(0.1f, 0.2f, 0.3f, 0.4f);
	NColor colourB(0.5f, 0.5f, 0.5f, 0.5f);
	NColor colourC(0.9f, 0.8f, 0.7f, 0.6f);

	REQUIRE(NColor::Interpolate(colourA, colourC, 0.0f) == colourA);
	REQUIRE(NColor::Interpolate(colourA, colourC, 0.5f) == colourB);
	REQUIRE(NColor::Interpolate(colourA, colourC, 1.0f) == colourC);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "CompareEqual")
{


	// Perform the test
	NColor colourA(1.0f, 0.0f, 0.0f, 1.0f);
	NColor colourB(0.0f, 1.0f, 0.0f, 1.0f);
	NColor colourC(1.0f, 0.0f, 0.0f, 1.0f);

	REQUIRE(colourA != colourB);
	REQUIRE(colourB != colourA);

	REQUIRE(colourA == colourC);
	REQUIRE(colourC == colourA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "CompareOrder")
{


	// Perform the test
	NColor colourA(1.0f, 0.0f, 0.0f, 1.0f);
	NColor colourB(1.0f, 0.0f, 0.0f, 0.5f);
	NColor colourC(1.0f, 1.0f, 0.0f, 1.0f);

	REQUIRE(colourB < colourA);
	REQUIRE(colourA < colourC);

	REQUIRE(colourC > colourA);
	REQUIRE(colourA > colourB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TColor, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", kNColorBlue) == "{r = 0, g = 0, b = 1, a = 1}");
	REQUIRE(NFormat("{}", kNColorGray) == "{r = 0.5, g = 0.5, b = 0.5, a = 1}");
	REQUIRE(NFormat("{}", kNColorCyan) == "{r = 0, g = 1, b = 1, a = 1}");
}
