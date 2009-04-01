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
static const NRange kNRangeTest1(0, 5);
static const NRange kNRangeTest2(3, 7);





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
	NN_ASSERT(kNRangeAll.IsNotEmpty());

	NN_ASSERT(kNRangeTest1.GetLocation() == 0);
	NN_ASSERT(kNRangeTest1.GetSize()     == 5);
	NN_ASSERT(kNRangeTest1.GetFirst()    == 0);
	NN_ASSERT(kNRangeTest1.GetLast()     == 4);
	NN_ASSERT(kNRangeTest1.GetNext()     == 5);

	NN_ASSERT(kNRangeTest2.GetLocation() == 3);
	NN_ASSERT(kNRangeTest2.GetSize()     == 7);
	NN_ASSERT(kNRangeTest2.GetFirst()    == 3);
	NN_ASSERT(kNRangeTest2.GetLast()     == 9);
	NN_ASSERT(kNRangeTest2.GetNext()     == 10);

	NN_ASSERT(kNRangeTest1.GetUnion(       kNRangeTest2) == NRange(0, 10));
	NN_ASSERT(kNRangeTest1.GetIntersection(kNRangeTest2) == NRange(3, 2));
	NN_ASSERT(kNRangeTest1.Overlaps(kNRangeTest2));

	NN_ASSERT( kNRangeTest1.Contains(0));
	NN_ASSERT( kNRangeTest1.Contains(4));
	NN_ASSERT(!kNRangeTest1.Contains(5));

	NN_ASSERT( kNRangeTest2.Contains(3));
	NN_ASSERT( kNRangeTest2.Contains(9));
	NN_ASSERT(!kNRangeTest2.Contains(2));
	NN_ASSERT(!kNRangeTest2.Contains(10));

	NN_ASSERT(kNRangeTest1.GetIntersection(NRange(8, 3)) == kNRangeNone);
	NN_ASSERT(kNRangeTest1.GetUnion(       NRange(0, 3)) != kNRangeNone);
}



