/*	NAME:
		TAtomicInt.cpp

	DESCRIPTION:
		NAtomicInt tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAtomicInt.h"

#include "TAtomicInt.h"





//============================================================================
//		TAtomicInt::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TAtomicInt::Execute(void)
{	NAtomicInt		testInt;



	// Execute the tests
	testInt = 1;
	NN_ASSERT(testInt == 1);
	NN_ASSERT(testInt != 0);

	testInt++;
	NN_ASSERT(testInt == 2);

	testInt--;
	NN_ASSERT(testInt == 1);

	testInt += 3;
	NN_ASSERT(testInt == 4);

	testInt -= 4;
	NN_ASSERT(testInt == 0);
}




