/*	NAME:
		TPoint.cpp

	DESCRIPTION:
		NPoint tests.

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
#include "NPoint.h"

#include "TPoint.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NPoint kValuePoint										= NPoint(1.0f, -1.0f);





//============================================================================
//		TPoint::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TPoint::Execute(void)
{	NPoint		testPoint;



	// Execute the tests
	NN_ASSERT(sizeof(NPoint64) == 16);
	NN_ASSERT(sizeof(NPoint32) == 8);
	NN_ASSERT(sizeof(NPoint)   == 8);
	
	NN_ASSERT(testPoint.IsZero());
	NN_ASSERT(!kValuePoint.IsZero());

	testPoint = kValuePoint;
	NN_ASSERT(!testPoint.IsZero());

	testPoint.Clear();
	NN_ASSERT(testPoint.IsZero());
	NN_ASSERT(testPoint != kValuePoint);

	NN_ASSERT(NMathUtilities::AreEqual(kValuePoint.x,  1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kValuePoint.y, -1.0f));
}



