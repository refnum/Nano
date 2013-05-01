/*	NAME:
		TRectangle.cpp

	DESCRIPTION:
		NRectangle tests.

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
#include "NRectangle.h"

#include "TRectangle.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NRectangle kTestRectangle1								= NRectangle(1.0f,   2.0f, 10.0f, -20.0f);
static const NRectangle kTestRectangle1_Normalized					= NRectangle(1.0f, -18.0f, 10.0f,  20.0f);

static const NRectangle kTestRectangle2								= NRectangle(3.4f, 4.8f, 1.2f, 7.8f);
static const NRectangle kTestRectangle2_Integral					= NRectangle(3.0f, 4.0f, 2.0f, 9.0f);

static const NRectangle kTestRectangle3								= NRectangle(3.7f, 5.2f, 0.1f, 1.1f);





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

	NN_ASSERT(!kTestRectangle1.IsEmpty());
	NN_ASSERT(!kTestRectangle1.origin.IsZero());
	NN_ASSERT(!kTestRectangle1.size.IsEmpty());

	testRect = kTestRectangle1;
	NN_ASSERT(!testRect.IsEmpty());

	testRect.Clear();
	NN_ASSERT(testRect.IsEmpty());
	NN_ASSERT(testRect != kTestRectangle1);



	// Comparisons
	NN_ASSERT(NMathUtilities::AreEqual(kTestRectangle1.origin.x,      1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kTestRectangle1.origin.y,      2.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kTestRectangle1.size.width,   10.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kTestRectangle1.size.height, -20.0f));

	testRect = kTestRectangle3.GetScaled(1.0f);
	NN_ASSERT(kTestRectangle3 != kTestRectangle2);
	NN_ASSERT(kTestRectangle3 == testRect);



	// Manipulations
	testRect = kTestRectangle1;     testRect.Normalize();
	NN_ASSERT(testRect == kTestRectangle1.GetNormalized());
	NN_ASSERT(testRect == kTestRectangle1_Normalized);

	testRect = kTestRectangle2; testRect.MakeIntegral();
	NN_ASSERT(testRect == kTestRectangle2.GetIntegral());
	NN_ASSERT(testRect == kTestRectangle2_Integral);

	NN_ASSERT(kTestRectangle2.GetScaled(1.0f) == kTestRectangle2);
	NN_ASSERT(kTestRectangle2.GetScaled(0.0f).IsEmpty());



	// Geometry
	NN_ASSERT(kTestRectangle2.Contains(kTestRectangle3.origin));
	NN_ASSERT(kTestRectangle2.Contains(kTestRectangle3));

	NN_ASSERT(!kTestRectangle2.Contains(kTestRectangle1.origin));
	NN_ASSERT(!kTestRectangle2.Contains(kTestRectangle1));

	NN_ASSERT(kTestRectangle2.Intersects(kTestRectangle3));

	NN_ASSERT(kTestRectangle2.GetUnion(kTestRectangle3) == kTestRectangle2);
}


