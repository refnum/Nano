/*	NAME:
		TArray.cpp

	DESCRIPTION:
		NArray tests.

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
#include "NSTLUtilities.h"
#include "NArray.h"

#include "TArray.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const SInt32List  kListSInt32								= vector((SInt32) 1, (SInt32) 2);
static const SInt64List  kListSInt64								= vector((SInt64) 1, (SInt64) 2, (SInt64) 3);
static const Float32List kListFloat32								= vector(1.0f, 2.0f, 3.0f, 4.0f);
static const Float64List kListFloat64								= vector(1.0, 2.0, 3.0, 4.0, 5.0);

static const NPoint kTestPoint										= NPoint(10.0f, 10.0f);
static const SInt32 kTestSInt32										=  2000;
static const SInt64 kTestSInt64										= -4000;





//============================================================================
//		TArray::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TArray::Execute(void)
{	NArray			testArray, testArray2;
	Float32List		listFloat32;
	Float64List		listFloat64;
	NIndex			n, theCount;



	// Execute the tests
	testArray.AppendValue(kTestPoint);
	testArray.AppendValue(kTestSInt32);
	testArray.AppendValue(kTestSInt64);
	NN_ASSERT(testArray.GetSize() == 3);
	NN_ASSERT(testArray.GetValuePoint(0)  == kTestPoint);
	NN_ASSERT(testArray.GetValueSInt32(1) == kTestSInt32);
	NN_ASSERT(testArray.GetValueSInt64(2) == kTestSInt64);
	
	theCount = 0;
	testArray.ForEach(BindFunction(TArray::ForEach, _1, _2, &theCount));
	NN_ASSERT(theCount == 3);

	testArray = NArray(kListSInt32);
	NN_ASSERT(testArray.GetSize() == (NIndex) kListSInt32.size());

	testArray = NArray(kListSInt64);
	NN_ASSERT(testArray.GetSize() == (NIndex) kListSInt64.size());

	testArray = NArray(kListFloat32);
	NN_ASSERT(testArray.GetSize() == (NIndex) kListFloat32.size());

	testArray = NArray(kListFloat64);
	NN_ASSERT(testArray.GetSize() == (NIndex) kListFloat64.size());

	testArray.SetValuesSInt32(kListSInt32);
	NN_ASSERT(testArray.GetSize()         == (NIndex) kListSInt32.size());
	NN_ASSERT(testArray.GetValuesSInt32() ==          kListSInt32);

	testArray.SetValuesSInt64(kListSInt64);
	NN_ASSERT(testArray.GetSize()         == (NIndex) kListSInt64.size());
	NN_ASSERT(testArray.GetValuesSInt64() ==          kListSInt64);

	testArray.SetValuesFloat32(kListFloat32);
	listFloat32 = testArray.GetValuesFloat32();
	NN_ASSERT(testArray.GetSize() == (NIndex) kListFloat32.size());
	NN_ASSERT(listFloat32.size()  ==          kListFloat32.size());
	for (n = 0; n < testArray.GetSize(); n++)
		NN_ASSERT(NMathUtilities::AreEqual(listFloat32[n], kListFloat32[n]));

	testArray.SetValuesFloat64(kListFloat64);
	listFloat64 = testArray.GetValuesFloat64();
	NN_ASSERT(testArray.GetSize() == (NIndex) kListFloat64.size());
	NN_ASSERT(listFloat64.size()  ==          kListFloat64.size());
	for (n = 0; n < testArray.GetSize(); n++)
		NN_ASSERT(NMathUtilities::AreEqual(listFloat64[n], kListFloat64[n]));

	testArray  = NArray(kListSInt32);
	testArray2 = NArray(kListSInt64);
	testArray.Join(testArray2);
	NN_ASSERT(testArray.GetSize() == (NIndex) (kListSInt32.size() + kListSInt64.size()));

	testArray = NArray(kListFloat32);
	NN_ASSERT( testArray.HasValue(2.0f));
	NN_ASSERT(!testArray.HasValue(2.1));
	
	testArray = NArray(kListSInt32);
	testArray.Clear();
	NN_ASSERT(testArray.IsEmpty());

	testArray = NArray(kListSInt32);
	testArray.Clear();
	testArray.AppendValue(kTestSInt32);
	testArray.SetValue(0, kTestSInt64);
	NN_ASSERT(testArray.GetValueSInt64(0) == kTestSInt64);
}





//============================================================================
//		TArray::ForEach : ForEach functor.
//----------------------------------------------------------------------------
#pragma mark -
void TArray::ForEach(NIndex theIndex, const NVariant &/*theValue*/, NIndex *theCount)
{


	// Compiler warning
	NN_UNUSED(theIndex);



	// Update the count
	NN_ASSERT(theIndex == *theCount);
	*theCount = *theCount + 1;
}


