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
	REQUIRE(theVector.GetBit(0) == false);
	REQUIRE(theVector.GetBit(1) == true);
	REQUIRE(theVector.GetBit(2) == false);
	REQUIRE(theVector.CountBits(false) == 2);
	REQUIRE(theVector.CountBits(true)  == 1);
	REQUIRE(theVector.GetBits().GetSize() == 1);
	REQUIRE(theVector.GetBits(0, 3)       == 0x02);

	theVector.FlipBit(1);
	REQUIRE(theVector.GetBit(1) == false);

	theVector.FlipBit(1);
	REQUIRE(theVector.FindFirstBit(true)  == 1);
	REQUIRE(theVector.FindLastBit( true)  == 1);
	REQUIRE(theVector.FindFirstBit(false) == 0);
	REQUIRE(theVector.FindLastBit( false) == 2);

	theVector.SetBit(1, true);
	REQUIRE(theVector.GetBit(1) == true);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBITVECTOR("Set", "Set all bits")
{


	// Perform the test
	theVector.SetBits(false);
	REQUIRE(!theVector.ContainsBit(true));
	REQUIRE( theVector.ContainsBit(false));

	theVector.SetBits(true);
	REQUIRE( theVector.ContainsBit(true));
	REQUIRE(!theVector.ContainsBit(false));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBITVECTOR("Clear", "Clear the vector")
{


	// Perform the test
	theVector.Clear();
	REQUIRE(theVector.IsEmpty());
}
