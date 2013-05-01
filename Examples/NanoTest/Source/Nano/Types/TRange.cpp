/*	NAME:
		TRange.cpp

	DESCRIPTION:
		NRange tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
static const NRange kTestRange1										= NRange(0, 5);
static const NRange kTestRange2										= NRange(3, 7);





//============================================================================
//		TRange::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TRange::Execute(void)
{


	// Execute the tests
	NN_ASSERT(kNRangeNone.GetLocation() == kNIndexNone);
	NN_ASSERT(kNRangeNone.GetSize()     == 0);

	NN_ASSERT(kNRangeAll.GetLocation() == 0);
	NN_ASSERT(kNRangeAll.GetSize()     == kNIndexNone);

	NN_ASSERT(kNRangeNone.IsEmpty());
	NN_ASSERT(!kNRangeAll.IsEmpty());

	NN_ASSERT(kTestRange1.GetLocation() == 0);
	NN_ASSERT(kTestRange1.GetSize()     == 5);
	NN_ASSERT(kTestRange1.GetFirst()    == 0);
	NN_ASSERT(kTestRange1.GetLast()     == 4);
	NN_ASSERT(kTestRange1.GetNext()     == 5);

	NN_ASSERT(kTestRange2.GetLocation() == 3);
	NN_ASSERT(kTestRange2.GetSize()     == 7);
	NN_ASSERT(kTestRange2.GetFirst()    == 3);
	NN_ASSERT(kTestRange2.GetLast()     == 9);
	NN_ASSERT(kTestRange2.GetNext()     == 10);

	NN_ASSERT(kTestRange1.GetUnion(       kTestRange2) == NRange(0, 10));
	NN_ASSERT(kTestRange1.GetIntersection(kTestRange2) == NRange(3, 2));
	NN_ASSERT(kTestRange1.Overlaps(kTestRange2));

	NN_ASSERT( kTestRange1.Contains(0));
	NN_ASSERT( kTestRange1.Contains(4));
	NN_ASSERT(!kTestRange1.Contains(5));

	NN_ASSERT( kTestRange2.Contains(3));
	NN_ASSERT( kTestRange2.Contains(9));
	NN_ASSERT(!kTestRange2.Contains(2));
	NN_ASSERT(!kTestRange2.Contains(10));

	NN_ASSERT(kTestRange1.GetIntersection(NRange(8, 3)) == kNRangeNone);
	NN_ASSERT(kTestRange1.GetUnion(       NRange(0, 3)) != kNRangeNone);
}


