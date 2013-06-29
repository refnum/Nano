/*	NAME:
		TColor.cpp

	DESCRIPTION:
		NColor tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NTestFixture.h"
#include "NColor.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NCOLOR(_name, _desc)									TEST_NANO(TColor, _name, _desc)

FIXTURE_NANO(TColor)
{
	NColor		theColor;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCOLOR("Opacity", "Opacity")
{


	// Perform the test
	REQUIRE( kNColorBlack.IsOpaque());
	REQUIRE(!kNColorBlack.IsTransparent());

	REQUIRE(!kNColorClear.IsOpaque());
	REQUIRE( kNColorClear.IsTransparent());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCOLOR("Components", "Components")
{


	// Perform the test
	theColor = NColor(1.0f, 0.0f, 0.0f, 1.0f);
	REQUIRE(theColor == kNColorRed);

	theColor = NColor(0.0f, 1.0f, 0.0f);
	REQUIRE(theColor == kNColorGreen);

	theColor = NColor(0xFF00FFFF);
	REQUIRE(theColor == kNColorMagenta);

	theColor = NColor("0000FFFF");
	REQUIRE(theColor == kNColorBlue);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NCOLOR("Getters", "Getters")
{	Float32		r, g, b, a;



	// Perform the test
	kNColorCyan.GetColor(r, g, b, a);
	REQUIRE(NMathUtilities::AreEqual(r, 0.0f));
	REQUIRE(NMathUtilities::AreEqual(g, 1.0f));
	REQUIRE(NMathUtilities::AreEqual(b, 1.0f));
	REQUIRE(NMathUtilities::AreEqual(a, 1.0f));

	REQUIRE(kNColorYellow.GetColor()            == 0xFFFF00FF);
	REQUIRE(kNColorYellow.GetColor(kNColorRGBA) == 0xFFFF00FF);
	REQUIRE(kNColorYellow.GetColor(kNColorARGB) == 0xFFFFFF00);
}

