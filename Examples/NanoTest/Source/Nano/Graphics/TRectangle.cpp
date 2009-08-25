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
static const NRectangle kValueRectangle								= NRectangle(1.0f, 2.0f, 10.0f, 20.0f);





//============================================================================
//		TRectangle::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TRectangle::Execute(void)
{	NRectangle		testRect;



	// Execute the tests
	NN_ASSERT(testRect.IsEmpty());
	NN_ASSERT(testRect.origin.IsZero());
	NN_ASSERT(testRect.size.IsEmpty());

	NN_ASSERT(!kValueRectangle.IsEmpty());
	NN_ASSERT(!kValueRectangle.origin.IsZero());
	NN_ASSERT(!kValueRectangle.size.IsEmpty());

	testRect = kValueRectangle;
	NN_ASSERT(!testRect.IsEmpty());

	testRect.Clear();
	NN_ASSERT(testRect.IsEmpty());
	NN_ASSERT(testRect != kValueRectangle);

	NN_ASSERT(NMathUtilities::AreEqual(kValueRectangle.origin.x,     1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kValueRectangle.origin.y,     2.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kValueRectangle.size.width,  10.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kValueRectangle.size.height, 20.0f));
}


