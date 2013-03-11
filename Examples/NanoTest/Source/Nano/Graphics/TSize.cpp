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
static const NSize kTestSize1										= NSize(1.0f, -1.0f);
static const NSize kTestSize1_Normalized							= NSize(1.0f,  1.0f);

static const NSize kTestSize2										= NSize(1.20f, 7.80f);
static const NSize kTestSize2_Integral								= NSize(2.00f, 8.00f);
static const NSize kTestSize2_Resized								= NSize(2.40f, 4.40f);
static const NSize kTestSize2_Scaled								= NSize(12.0f, 78.0f);

static const NSize kTestSize3										= NSize(1.20f, 7.80f);

static const NSize kTestSize4										= NSize(1.0f, 1.0f);
static const NSize kTestSize4_Width									= NSize(0.0f, 1.0f);
static const NSize kTestSize4_Height								= NSize(1.0f, 0.0f);





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
	NN_ASSERT(!kTestSize1.IsEmpty());

	testSize = kTestSize1;
	NN_ASSERT(!testSize.IsEmpty());

	testSize.Clear();
	NN_ASSERT(testSize.IsEmpty());
	NN_ASSERT(testSize != kTestSize1);

	NN_ASSERT(!kTestSize4.IsEmpty());
	NN_ASSERT( kTestSize4_Width.IsEmpty());
	NN_ASSERT( kTestSize4_Height.IsEmpty());



	// Comparisons
	NN_ASSERT(NMathUtilities::AreEqual(kTestSize1.width,   1.0f));
	NN_ASSERT(NMathUtilities::AreEqual(kTestSize1.height, -1.0f));

	NN_ASSERT(kTestSize2 != kTestSize1);
	NN_ASSERT(kTestSize2 == kTestSize3);



	// Manipulations
	testSize = kTestSize1;     testSize.Normalize();
	NN_ASSERT(testSize == kTestSize1.GetNormalized());
	NN_ASSERT(testSize == kTestSize1_Normalized);

	testSize = kTestSize2; testSize.MakeIntegral();
	NN_ASSERT(testSize == kTestSize2.GetIntegral());
	NN_ASSERT(testSize == kTestSize2_Integral);

	testSize = kTestSize2;     testSize.Resize( 1.2f, -3.4f);
	NN_ASSERT(testSize == kTestSize2.GetResized(1.2f, -3.4f));
	NN_ASSERT(testSize == kTestSize2_Resized);

	testSize = kTestSize2;     testSize.Scale( 10.0f);
	NN_ASSERT(testSize == kTestSize2.GetScaled(10.0f));
	NN_ASSERT(testSize == kTestSize2_Scaled);
}

