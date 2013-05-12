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
#include "NTimer.h"

#include "CTestUtilities.h"
#include "TTimer.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NTime kTestDelay										= 0.0005;
static const NTime kTestRepeat										= 0.0006;





//============================================================================
//		TTimer::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TTimer::Execute(void)
{	NTimer		theTimer;
	UInt32		theValue;
	NTimerID	theID;



	// Empty
	NN_ASSERT(!theTimer.HasTimer());



	// Active
	theValue = 0;
	theID    = theTimer.AddTimer(BindFunction(TTimer::IncrementValue, &theValue), kTestDelay, kTestRepeat);

	NN_ASSERT(theID != kNTimerNone);
	NN_ASSERT(theTimer.HasTimer());
	NN_ASSERT(theTimer.HasTimer(theID));



	// Running
	while (theValue < 3)
		CTestUtilities::ExecuteRunloop(kTestRepeat);

	theTimer.ResetTimer(kTestRepeat, theID);

	while (theValue < 5)
		CTestUtilities::ExecuteRunloop(kTestRepeat);



	// Removal
	theTimer.RemoveTimer(theID);

	NN_ASSERT(!theTimer.HasTimer());
	NN_ASSERT(!theTimer.HasTimer(theID));
}





//============================================================================
//		TTimer::IncrementValue : Increment a value.
//----------------------------------------------------------------------------
#pragma mark -
void TTimer::IncrementValue(UInt32 *theValue)
{


	// Update the value
	*theValue = *theValue + 1;
}


