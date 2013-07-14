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
static void IncrementValue(UInt32 *theValue)
{


	// Update the value
	*theValue = *theValue + 1;
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
{	UInt32		theValue;
	NTimerID	theID;



	// Perform the test
	theValue = 0;
	theID    = theTimer.AddTimer(BindFunction(IncrementValue, &theValue), kTestDelay, kTestRepeat);

	REQUIRE(theID != kNTimerNone);
	REQUIRE(theTimer.HasTimer());
	REQUIRE(theTimer.HasTimer(theID));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTIMER("Reset")
{	UInt32		theValue;
	NTimerID	theID;
	NIndex		n;



	// Perform the test
	theValue = 0;
	theID    = theTimer.AddTimer(BindFunction(IncrementValue, &theValue), kTestDelay, kTestRepeat);

	for (n = 0; n < 100; n++)
		{
		CTestUtilities::ExecuteRunloop(kTestRepeat);
		if (theValue == 3)
			break;
		}

	REQUIRE(theValue == 3);


	theTimer.ResetTimer(kTestRepeat, theID);
	
	for (n = 0; n < 100; n++)
		{
		CTestUtilities::ExecuteRunloop(kTestRepeat);
		if (theValue == 5)
			break;
		}

	REQUIRE(theValue == 5);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTIMER("Removal")
{	UInt32		theValue;
	NTimerID	theID;



	// Perform the test
	theValue = 0;
	theID    = theTimer.AddTimer(BindFunction(IncrementValue, &theValue), kTestDelay, kTestRepeat);

	REQUIRE(theTimer.HasTimer());
	REQUIRE(theTimer.HasTimer(theID));

	theTimer.RemoveTimer(theID);

	REQUIRE(!theTimer.HasTimer());
	REQUIRE(!theTimer.HasTimer(theID));
}


