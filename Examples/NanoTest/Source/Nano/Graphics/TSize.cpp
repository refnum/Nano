/*	NAME:
		TSize.cpp

	DESCRIPTION:
		NSize tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
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
static const NSize kValueSize1										= NSize(1.0f, -1.0f);
static const NSize kValueSize1_Normalized							= NSize(1.0f,  1.0f);

static const NSize kValueSize2										= NSize(1.20f, 7.80f);
static const NSize kValueSize2_Integral								= NSize(2.00f, 8.00f);
static const NSize kValueSize2_Resized								= NSize(2.40f, 4.40f);
static const NSize kValueSize2_Scaled								= NSize(12.0f, 78.0f);





//============================================================================
//		TSize::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TSize::Execute(void)
{	NSize	testSize;



	// Sizes
	NN_ASSERT(sizeof(NSize64) == 16);
	NN_ASSERT(sizeof(NSize32) == 8);
	NN_ASSERT(sizeof(NSize)   == 8);



	// Contents
	NN_ASSERT(testSize.IsEmpty());
	NN_ASSERT(!kValueSize1.IsEmpty());

	testSize = kValueSize1;
	NN_ASSERT(!testSize.IsEmpty());

	testSize.Clear();
	NN_ASSERT(testSize.IsEmpty());
	NN_ASSERT(testSize != kValueSize1);



	// Comparisons
	NN_ASSERT(NMathUtilities::AreEqual(kValueSize1.width,   1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kValueSize1.height, -1.0f));



	// Manipulations
	testSize = kValueSize1;     testSize.Normalize();
	NN_ASSERT(testSize == kValueSize1.GetNormalized());
	NN_ASSERT(testSize == kValueSize1_Normalized);

	testSize = kValueSize2; testSize.MakeIntegral();
	NN_ASSERT(testSize == kValueSize2.GetIntegral());
	NN_ASSERT(testSize == kValueSize2_Integral);

	testSize = kValueSize2;     testSize.Resize( 1.2f, -3.4f);
	NN_ASSERT(testSize == kValueSize2.GetResized(1.2f, -3.4f));
	NN_ASSERT(testSize == kValueSize2_Resized);

	testSize = kValueSize2;     testSize.Scale( 10.0f);
	NN_ASSERT(testSize == kValueSize2.GetScaled(10.0f));
	NN_ASSERT(testSize == kValueSize2_Scaled);
}


