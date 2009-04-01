/*	NAME:
		TColor.cpp

	DESCRIPTION:
		NColor tests.

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
#include "NColor.h"

#include "TColor.h"





//============================================================================
//		TColor::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TColor::Execute(void)
{	Float32		r, g, b, a;
	NColor		testColor;



	// Execute the tests
	testColor = NColor(1.0f, 0.0f, 0.0f, 1.0f);
	NN_ASSERT(testColor == kColorRed);

	testColor = NColor(0.0f, 1.0f, 0.0f);
	NN_ASSERT(testColor == kColorGreen);

	testColor = NColor(0xFF00FFFF);
	NN_ASSERT(testColor == kColorMagenta);

	testColor = NColor("0000FFFF");
	NN_ASSERT(testColor == kColorBlue);

	kColorCyan.GetColor(r, g, b, a);
	NN_ASSERT(NMathUtilities::AreEqual(r, 0.0f));
	NN_ASSERT(NMathUtilities::AreEqual(g, 1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(b, 1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(a, 1.0f));

	NN_ASSERT(kColorYellow.GetColor() == 0xFFFF00FF);
}
