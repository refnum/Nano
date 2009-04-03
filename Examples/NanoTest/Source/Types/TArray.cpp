/*	NAME:
		TArray.cpp

	DESCRIPTION:
		NArray tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NArray.h"

#include "TArray.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const SInt32List  kTestListSInt32			= vector((SInt32) 1, (SInt32) 2);
static const SInt64List  kTestListSInt64			= vector((SInt64) 1, (SInt64) 2, (SInt64) 3);
static const Float32List kTestListFloat32			= vector(1.0f, 2.0f, 3.0f, 4.0f);
static const Float64List kTestListFloat64			= vector(1.0, 2.0, 3.0, 4.0, 5.0);





//============================================================================
//		TArray::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TArray::Execute(void)
{	NArray		testArray, testArray2;
	NPoint		valuePoint(10, 10);
	SInt32		valueSInt32 =  2000;
	SInt64		valueSInt64 = -4000;



	// Execute the tests
	testArray.AppendValue(valuePoint);
	testArray.AppendValue(valueSInt32);
	testArray.AppendValue(valueSInt64);
	NN_ASSERT(testArray.GetSize() == 3);
	NN_ASSERT(testArray.GetValuePoint(0)  == valuePoint);
	NN_ASSERT(testArray.GetValueSInt32(1) == valueSInt32);
	NN_ASSERT(testArray.GetValueSInt64(2) == valueSInt64);

	testArray = NArray(kTestListSInt32);
	NN_ASSERT(testArray.GetSize() == kTestListSInt32.size());

	testArray = NArray(kTestListSInt64);
	NN_ASSERT(testArray.GetSize() == kTestListSInt64.size());

	testArray = NArray(kTestListFloat32);
	NN_ASSERT(testArray.GetSize() == kTestListFloat32.size());

	testArray = NArray(kTestListFloat64);
	NN_ASSERT(testArray.GetSize() == kTestListFloat64.size());

	testArray.SetValuesSInt32(kTestListSInt32);
	NN_ASSERT(testArray.GetSize()         == kTestListSInt32.size());
	NN_ASSERT(testArray.GetValuesSInt32() == kTestListSInt32);

	testArray.SetValuesSInt64(kTestListSInt64);
	NN_ASSERT(testArray.GetSize()         == kTestListSInt64.size());
	NN_ASSERT(testArray.GetValuesSInt64() == kTestListSInt64);

	testArray.SetValuesFloat32(kTestListFloat32);
	NN_ASSERT(testArray.GetSize()          == kTestListFloat32.size());
	NN_ASSERT(testArray.GetValuesFloat32() == kTestListFloat32);

	testArray.SetValuesFloat64(kTestListFloat64);
	NN_ASSERT(testArray.GetSize()          == kTestListFloat64.size());
	NN_ASSERT(testArray.GetValuesFloat64() == kTestListFloat64);

	testArray  = NArray(kTestListSInt32);
	testArray2 = NArray(kTestListSInt64);
	testArray.Join(testArray2);
	NN_ASSERT(testArray.GetSize() == kTestListSInt32.size() + kTestListSInt64.size());

	testArray = NArray(kTestListFloat32);
	NN_ASSERT( testArray.HasValue(2.0f));
	NN_ASSERT(!testArray.HasValue(2.0));
	
	testArray = NArray(kTestListSInt32);
	testArray.Clear();
	NN_ASSERT(testArray.IsEmpty());

	testArray.Clear();
	testArray.AppendValue(valueSInt32);
	testArray.SetValue(0, valueSInt64);
	NN_ASSERT(testArray.GetValue(0) == valueSInt64);
}
