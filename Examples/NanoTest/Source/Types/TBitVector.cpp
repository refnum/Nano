/*	NAME:
		TBitVector.cpp

	DESCRIPTION:
		NBitVector tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NBitVector.h"

#include "TBitVector.h"





//============================================================================
//		TBitVector::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TBitVector::Execute(void)
{	NBitVector		testVector;



	// Execute the tests
	testVector.AppendBit(true);
	testVector.AppendBit(false);
	testVector.AppendBit(true);
	NN_ASSERT(testVector.GetSize() == 3);

	testVector.FlipBits();
	NN_ASSERT(testVector.GetBit(0) == false);
	NN_ASSERT(testVector.GetBit(1) == true);
	NN_ASSERT(testVector.GetBit(2) == false);
	NN_ASSERT(testVector.CountBits(true) == 2);
	NN_ASSERT(testVector.CountBits(true) == 1);
	NN_ASSERT(testVector.GetBits().GetSize() == 1);
	NN_ASSERT(testVector.GetBits(0, 3)       == 0x02);

	testVector.FlipBit(1);
	NN_ASSERT(testVector.GetBit(1) == false);

	testVector.FlipBit(1);
	NN_ASSERT(testVector.FindFirstBit(true)  == 1);
	NN_ASSERT(testVector.FindLastBit( true)  == 1);
	NN_ASSERT(testVector.FindFirstBit(false) == 0);
	NN_ASSERT(testVector.FindLastBit( false) == 2);

	testVector.SetBit(1, true);
	NN_ASSERT(testVector.GetBit(1) == true);

	testVector.SetBits(true);
	NN_ASSERT( testVector.ContainsBit(true));
	NN_ASSERT(!testVector.ContainsBit(false));

	testVector.SetBits(false);
	NN_ASSERT(!testVector.ContainsBit(true));
	NN_ASSERT( testVector.ContainsBit(false));

	testVector.Clear();
	NN_ASSERT(testVector.IsEmpty());
}
