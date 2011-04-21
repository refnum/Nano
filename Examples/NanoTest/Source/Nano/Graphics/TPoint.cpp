/*	NAME:
		TPoint.cpp

	DESCRIPTION:
		NPoint tests.

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
#include "NPoint.h"

#include "TPoint.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NPoint kValuePoint1									= NPoint(1.0f, -1.0f);

static const NPoint kValuePoint2									= NPoint(1.3f, 1.9f);
static const NPoint kValuePoint2_Integral							= NPoint(1.0f, 2.0f);
static const NPoint kValuePoint2_Offset								= NPoint(5.3f, 5.9f);





//============================================================================
//		TPoint::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TPoint::Execute(void)
{	NPoint		testPoint;



	// Sizes
	NN_ASSERT(sizeof(NPoint64) == 16);
	NN_ASSERT(sizeof(NPoint32) == 8);
	NN_ASSERT(sizeof(NPoint)   == 8);



	// Contents
	NN_ASSERT(testPoint.IsZero());
	NN_ASSERT(!kValuePoint1.IsZero());

	testPoint = kValuePoint1;
	NN_ASSERT(!testPoint.IsZero());

	testPoint.Clear();
	NN_ASSERT(testPoint.IsZero());
	NN_ASSERT(testPoint != kValuePoint1);



	// Comparisons
	NN_ASSERT(NMathUtilities::AreEqual(kValuePoint1.x,  1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kValuePoint1.y, -1.0f));



	// Manipulations
	testPoint = kValuePoint2; testPoint.MakeIntegral();
	NN_ASSERT(testPoint == kValuePoint2.GetIntegral());
	NN_ASSERT(testPoint == kValuePoint2_Integral);

	testPoint = kValuePoint2;    testPoint.Offset(4.0f, 4.0f);
	NN_ASSERT(testPoint == kValuePoint2.GetOffset(4.0f, 4.0f));
	NN_ASSERT(testPoint == kValuePoint2_Offset);
}




