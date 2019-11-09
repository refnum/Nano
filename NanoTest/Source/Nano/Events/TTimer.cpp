/*	NAME:
		TTimer.cpp

	DESCRIPTION:
		NTimer tests.

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
#include "NTimer.h"

#include "CTestUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NTime kTestDelay										= 0.0005;
static const NTime kTestRepeat										= 0.0006;





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NTIMER(...)											TEST_NANO(TTimer, ##__VA_ARGS__)

FIXTURE_NANO(TTimer)
{
	NTimer	theTimer;
};





//============================================================================
//		IncrementValue : Increment a value.
//----------------------------------------------------------------------------
static void IncrementValue(uint32_t *theValue, uint32_t maxValue)
{


	// Update the value
	*theValue = std::min(maxValue, *theValue + 1);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTIMER("Default")
{


	// Perform the test
	REQUIRE(!theTimer.HasTimer());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTIMER("Active")
{	uint32_t	theValue;
	NTimerID	theID;



	// Perform the test
	theValue = 0;
	theID    = theTimer.AddTimer(BindFunction(IncrementValue, &theValue, 1), kTestDelay, kTestRepeat);

	REQUIRE(theID != kNTimerNone);
	REQUIRE(theTimer.HasTimer());
	REQUIRE(theTimer.HasTimer(theID));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTIMER("Reset")
{	uint32_t	theValue;
	NTimerID	theID;
	NIndex		n;



	// Perform the test
	theValue = 0;
	theID    = theTimer.AddTimer(BindFunction(IncrementValue, &theValue, 3), kTestDelay, kTestRepeat);

	for (n = 0; n < 100 && theValue != 3; n++)
		CTestUtilities::ExecuteRunloop(kTestRepeat);

	REQUIRE(theValue == 3);


	theValue = 0;
	theTimer.ResetTimer(kTestRepeat, theID);
	
	for (n = 0; n < 100 && theValue != 3; n++)
		CTestUtilities::ExecuteRunloop(kTestRepeat);

	REQUIRE(theValue == 3);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTIMER("Removal")
{	uint32_t	theValue;
	NTimerID	theID;



	// Perform the test
	theValue = 0;
	theID    = theTimer.AddTimer(BindFunction(IncrementValue, &theValue, 1), kTestDelay, kTestRepeat);

	REQUIRE(theTimer.HasTimer());
	REQUIRE(theTimer.HasTimer(theID));

	theTimer.RemoveTimer(theID);

	REQUIRE(!theTimer.HasTimer());
	REQUIRE(!theTimer.HasTimer(theID));
}


