/*	NAME:
		TRange.cpp

	DESCRIPTION:
		NRange tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NRange.h"

#include "TRange.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NRange kValueRange1									= NRange(0, 5);
static const NRange kValueRange2									= NRange(3, 7);





//============================================================================
//		TRange::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TRange::Execute(void)
{


	// Execute the tests
	NN_ASSERT(kNRangeNone.GetLocation() == 0);
	NN_ASSERT(kNRangeNone.GetSize()     == 0);

	NN_ASSERT(kNRangeAll.GetLocation() == 0);
	NN_ASSERT(kNRangeAll.GetSize()     == kNIndexNone);

	NN_ASSERT(kNRangeNone.IsEmpty());
	NN_ASSERT(!kNRangeAll.IsEmpty());

	NN_ASSERT(kValueRange1.GetLocation() == 0);
	NN_ASSERT(kValueRange1.GetSize()     == 5);
	NN_ASSERT(kValueRange1.GetFirst()    == 0);
	NN_ASSERT(kValueRange1.GetLast()     == 4);
	NN_ASSERT(kValueRange1.GetNext()     == 5);

	NN_ASSERT(kValueRange2.GetLocation() == 3);
	NN_ASSERT(kValueRange2.GetSize()     == 7);
	NN_ASSERT(kValueRange2.GetFirst()    == 3);
	NN_ASSERT(kValueRange2.GetLast()     == 9);
	NN_ASSERT(kValueRange2.GetNext()     == 10);

	NN_ASSERT(kValueRange1.GetUnion(       kValueRange2) == NRange(0, 10));
	NN_ASSERT(kValueRange1.GetIntersection(kValueRange2) == NRange(3, 2));
	NN_ASSERT(kValueRange1.Overlaps(kValueRange2));

	NN_ASSERT( kValueRange1.Contains(0));
	NN_ASSERT( kValueRange1.Contains(4));
	NN_ASSERT(!kValueRange1.Contains(5));

	NN_ASSERT( kValueRange2.Contains(3));
	NN_ASSERT( kValueRange2.Contains(9));
	NN_ASSERT(!kValueRange2.Contains(2));
	NN_ASSERT(!kValueRange2.Contains(10));

	NN_ASSERT(kValueRange1.GetIntersection(NRange(8, 3)) == kNRangeNone);
	NN_ASSERT(kValueRange1.GetUnion(       NRange(0, 3)) != kNRangeNone);
}



