/*	NAME:
		TBitVector.cpp

	DESCRIPTION:
		NBitVector tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NBitVector.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NBITVECTOR(_name, _desc)								NANO_TEST(TBitVector, _name, _desc)

NANO_FIXTURE(TBitVector)
{
	NBitVector		theVector;
	
	SETUP
	{
		theVector.AppendBit(true);
		theVector.AppendBit(false);
		theVector.AppendBit(true);
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBITVECTOR("Manipulate", "Manipulate individual bits")
{


	// Perform the test
	REQUIRE(theVector.GetSize() == 3);

	theVector.FlipBits();
	NN_ASSERT(theVector.GetBit(0) == false);
	NN_ASSERT(theVector.GetBit(1) == true);
	NN_ASSERT(theVector.GetBit(2) == false);
	NN_ASSERT(theVector.CountBits(false) == 2);
	NN_ASSERT(theVector.CountBits(true)  == 1);
	NN_ASSERT(theVector.GetBits().GetSize() == 1);
	NN_ASSERT(theVector.GetBits(0, 3)       == 0x02);

	theVector.FlipBit(1);
	NN_ASSERT(theVector.GetBit(1) == false);

	theVector.FlipBit(1);
	NN_ASSERT(theVector.FindFirstBit(true)  == 1);
	NN_ASSERT(theVector.FindLastBit( true)  == 1);
	NN_ASSERT(theVector.FindFirstBit(false) == 0);
	NN_ASSERT(theVector.FindLastBit( false) == 2);

	theVector.SetBit(1, true);
	NN_ASSERT(theVector.GetBit(1) == true);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBITVECTOR("Set", "Set all bits")
{


	// Perform the test
	theVector.SetBits(false);
	NN_ASSERT(!theVector.ContainsBit(true));
	NN_ASSERT( theVector.ContainsBit(false));

	theVector.SetBits(true);
	NN_ASSERT( theVector.ContainsBit(true));
	NN_ASSERT(!theVector.ContainsBit(false));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBITVECTOR("Clear", "Clear the vector")
{


	// Perform the test
	theVector.Clear();
	NN_ASSERT(theVector.IsEmpty());
}
