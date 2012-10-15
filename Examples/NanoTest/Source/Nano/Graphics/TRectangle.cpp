/*	NAME:
		TRectangle.cpp

	DESCRIPTION:
		NRectangle tests.

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
#include "NRectangle.h"

#include "TRectangle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NRectangle kValueRectangle1							= NRectangle(1.0f,   2.0f, 10.0f, -20.0f);
static const NRectangle kValueRectangle1_Normalized					= NRectangle(1.0f, -18.0f, 10.0f,  20.0f);

static const NRectangle kValueRectangle2							= NRectangle(3.4f, 4.8f, 1.2f, 7.8f);
static const NRectangle kValueRectangle2_Integral					= NRectangle(3.0f, 4.0f, 2.0f, 9.0f);

static const NRectangle kValueRectangle3							= NRectangle(3.7f, 5.2f, 0.1f, 1.1f);





//============================================================================
//		TRectangle::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TRectangle::Execute(void)
{	NRectangle		testRect;



	// Sizes
	NN_ASSERT(sizeof(NRectangle64) == 32);
	NN_ASSERT(sizeof(NRectangle32) == 16);
	NN_ASSERT(sizeof(NRectangle)   == 16);



	// Contents
	NN_ASSERT(testRect.IsEmpty());
	NN_ASSERT(testRect.origin.IsZero());
	NN_ASSERT(testRect.size.IsEmpty());

	NN_ASSERT(!kValueRectangle1.IsEmpty());
	NN_ASSERT(!kValueRectangle1.origin.IsZero());
	NN_ASSERT(!kValueRectangle1.size.IsEmpty());

	testRect = kValueRectangle1;
	NN_ASSERT(!testRect.IsEmpty());

	testRect.Clear();
	NN_ASSERT(testRect.IsEmpty());
	NN_ASSERT(testRect != kValueRectangle1);



	// Comparisons
	NN_ASSERT(NMathUtilities::AreEqual(kValueRectangle1.origin.x,      1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kValueRectangle1.origin.y,      2.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kValueRectangle1.size.width,   10.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kValueRectangle1.size.height, -20.0f));

	testRect = kValueRectangle3.GetScaled(1.0f);
	NN_ASSERT(kValueRectangle3 != kValueRectangle2);
	NN_ASSERT(kValueRectangle3 == testRect);



	// Manipulations
	testRect = kValueRectangle1;     testRect.Normalize();
	NN_ASSERT(testRect == kValueRectangle1.GetNormalized());
	NN_ASSERT(testRect == kValueRectangle1_Normalized);

	testRect = kValueRectangle2; testRect.MakeIntegral();
	NN_ASSERT(testRect == kValueRectangle2.GetIntegral());
	NN_ASSERT(testRect == kValueRectangle2_Integral);

	NN_ASSERT(kValueRectangle2.GetScaled(1.0f) == kValueRectangle2);
	NN_ASSERT(kValueRectangle2.GetScaled(0.0f).IsEmpty());



	// Geometry
	NN_ASSERT(kValueRectangle2.Contains(kValueRectangle3.origin));
	NN_ASSERT(kValueRectangle2.Contains(kValueRectangle3));

	NN_ASSERT(!kValueRectangle2.Contains(kValueRectangle1.origin));
	NN_ASSERT(!kValueRectangle2.Contains(kValueRectangle1));

	NN_ASSERT(kValueRectangle2.Intersects(kValueRectangle3));

	NN_ASSERT(kValueRectangle2.GetUnion(kValueRectangle3) == kValueRectangle2);
}




