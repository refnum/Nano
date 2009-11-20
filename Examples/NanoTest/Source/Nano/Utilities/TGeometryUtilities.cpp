/*	NAME:
		TGeometryUtilities.cpp

	DESCRIPTION:
		NGeometryUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NGeometryUtilities.h"
#include "NSTLUtilities.h"

#include "TGeometryUtilities.h"





//============================================================================
//		TGeometryUtilities::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TGeometryUtilities::Execute(void)
{	NRectangle64		theBounds64;
	NRectangle32		theBounds32;
	NRectangle			theBounds;
	NPoint64List		thePoints64;
	NPoint32List		thePoints32;
	NPointList			thePoints;



	// Prepare the data
	thePoints64 = vector(NPoint64(1.0f, 1.1f), NPoint64(2.0f, 2.2f), NPoint64(-3.0f, -3.3f));
	thePoints32 = vector(NPoint32(1.0f, 1.1f), NPoint32(2.0f, 2.2f), NPoint32(-3.0f, -3.3f));
	thePoints   = vector(NPoint  (1.0f, 1.1f), NPoint  (2.0f, 2.2f), NPoint  (-3.0f, -3.3f));

	theBounds64 = NGeometryUtilities::GetBounds(thePoints64);
	theBounds32 = NGeometryUtilities::GetBounds(thePoints32);
	theBounds   = NGeometryUtilities::GetBounds(thePoints);

	NN_ASSERT(NMathUtilities::AreEqual(theBounds64.origin.x,    theBounds32.origin.x));
	NN_ASSERT(NMathUtilities::AreEqual(theBounds64.origin.y,    theBounds32.origin.y));
	NN_ASSERT(NMathUtilities::AreEqual(theBounds64.size.width,  theBounds32.size.width));
	NN_ASSERT(NMathUtilities::AreEqual(theBounds64.size.height, theBounds32.size.height));
	NN_ASSERT(theBounds == theBounds32);
}


