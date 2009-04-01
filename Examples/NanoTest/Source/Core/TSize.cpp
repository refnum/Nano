/*	NAME:
		TSize.cpp

	DESCRIPTION:
		NSize tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NSize.h"

#include "TSize.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NSize kNSizeTest1(1.0f, -1.0f);





//============================================================================
//		TSize::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TSize::Execute(void)
{	NSize	testSize;



	// Execute the tests
	NN_ASSERT(kNSizeZero.IsEmpty());
	NN_ASSERT(kNSizeTest1.IsNotEmpty());

	testSize = kNSizeTest1;
	NN_ASSERT(testSize.IsNotEmpty());

	testSize.Clear();
	NN_ASSERT(testSize.IsEmpty());

	NN_ASSERT(testSize == kNSizeZero);
	NN_ASSERT(testSize != kNSizeTest1);

	NN_ASSERT(NMathUtilities::AreEqual(kNSizeTest1.width,   1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kNSizeTest1.height, -1.0f));
}


