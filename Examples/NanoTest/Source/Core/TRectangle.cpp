/*	NAME:
		TRectangle.cpp

	DESCRIPTION:
		NRectangle tests.

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
#include "NRectangle.h"

#include "TRectangle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NRectangle kNRectangleTest1(1.0f, 2.0f, 10.0f, 20.0f);





//============================================================================
//		TRectangle::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TRectangle::Execute(void)
{	NRectangle		testRect;



	// Execute the tests
	NN_ASSERT(kNRectangleZero.origin == kNPointZero);
	NN_ASSERT(kNRectangleZero.size   == kNSizeZero);

	NN_ASSERT(kNRectangleZero.IsEmpty());
	NN_ASSERT(kNRectangleTest1.IsNotEmpty());

	testRect = kNRectangleTest1;
	NN_ASSERT(testRect.IsNotEmpty());

	testRect.Clear();
	NN_ASSERT(testRect.IsEmpty());

	NN_ASSERT(testRect == kNRectangleZero);
	NN_ASSERT(testRect != kNRectangleTest1);

	NN_ASSERT(NMathUtilities::AreEqual(kNRectangleTest1.origin.x,     1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kNRectangleTest1.origin.y,     2.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kNRectangleTest1.size.width,  10.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kNRectangleTest1.size.height, 20.0f));
}


