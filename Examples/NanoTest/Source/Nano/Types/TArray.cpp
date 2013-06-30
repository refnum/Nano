/*	NAME:
		TArray.cpp

	DESCRIPTION:
		NArray tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NSTLUtilities.h"
#include "NTestFixture.h"
#include "NArray.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const SInt32List  kListSInt32								= nvector<SInt32>(1, 2);
static const SInt64List  kListSInt64								= nvector<SInt64>(1, 2, 3);
static const Float32List kListFloat32								= nvector<Float32>(1.0, 2.0, 3.0, 4.0);
static const Float64List kListFloat64								= nvector<Float64>(1.0, 2.0, 3.0, 4.0, 5.0);

static const NPoint kTestPoint										= NPoint(10.0f, 10.0f);
static const SInt32 kTestSInt32										=  2000;
static const SInt64 kTestSInt64										= -4000;





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NARRAY(...)											TEST_NANO(TArray, ##__VA_ARGS__)

FIXTURE_NANO(TArray)
{
	NArray	theArray, theArray2;

	static void ForEach(NIndex theIndex, const NVariant &/*theValue*/, NIndex *theCount);
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NARRAY("Default")
{


	// Perform the test
	REQUIRE(theArray.IsEmpty());
	REQUIRE(theArray2.IsEmpty());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NARRAY("Constructor")
{


	// Perform the test
	theArray = NArray(kListSInt32);
	REQUIRE(theArray.GetSize() == (NIndex) kListSInt32.size());
	REQUIRE( theArray.HasValue(2));
	REQUIRE(!theArray.HasValue(3));

	theArray = NArray(kListSInt64);
	REQUIRE(theArray.GetSize() == (NIndex) kListSInt64.size());
	REQUIRE( theArray.HasValue(3));
	REQUIRE(!theArray.HasValue(4));

	theArray = NArray(kListFloat32);
	REQUIRE(theArray.GetSize() == (NIndex) kListFloat32.size());
	REQUIRE( theArray.HasValue(2.0f));
	REQUIRE(!theArray.HasValue(2.1));

	theArray = NArray(kListFloat64);
	REQUIRE(theArray.GetSize() == (NIndex) kListFloat64.size());
	REQUIRE( theArray.HasValue(4.0f));
	REQUIRE(!theArray.HasValue(4.1));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NARRAY("Assignment")
{	Float32List		listFloat32;
	Float64List		listFloat64;
	NIndex			n;



	// Perform the test
	theArray.SetValuesSInt32(kListSInt32);
	REQUIRE( theArray.GetSize()         == (NIndex) kListSInt32.size());
	REQUIRE((theArray.GetValuesSInt32() ==          kListSInt32));

	theArray.SetValuesSInt64(kListSInt64);
	REQUIRE( theArray.GetSize()         == (NIndex) kListSInt64.size());
	REQUIRE((theArray.GetValuesSInt64() ==          kListSInt64));

	theArray.SetValuesFloat32(kListFloat32);
	listFloat32 = theArray.GetValuesFloat32();
	REQUIRE(theArray.GetSize() == (NIndex) kListFloat32.size());
	REQUIRE(listFloat32.size()  ==          kListFloat32.size());
	for (n = 0; n < theArray.GetSize(); n++)
		REQUIRE(NMathUtilities::AreEqual(listFloat32[(size_t) n], kListFloat32[(size_t) n]));

	theArray.SetValuesFloat64(kListFloat64);
	listFloat64 = theArray.GetValuesFloat64();
	REQUIRE(theArray.GetSize() == (NIndex) kListFloat64.size());
	REQUIRE(listFloat64.size()  ==          kListFloat64.size());
	for (n = 0; n < theArray.GetSize(); n++)
		REQUIRE(NMathUtilities::AreEqual(listFloat64[(size_t) n], kListFloat64[(size_t) n]));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NARRAY("Add")
{	NIndex		theCount;



	// Perform the test
	theArray.AppendValue(kTestPoint);
	theArray.AppendValue(kTestSInt32);
	theArray.AppendValue(kTestSInt64);
	REQUIRE(theArray.GetSize() == 3);
	REQUIRE(theArray.GetValuePoint(0)  == kTestPoint);
	REQUIRE(theArray.GetValueSInt32(1) == kTestSInt32);
	REQUIRE(theArray.GetValueSInt64(2) == kTestSInt64);
	
	theCount = 0;
	theArray.ForEach(BindFunction(TArray::ForEach, _1, _2, &theCount));
	REQUIRE(theCount == 3);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NARRAY("Clear")
{


	// Perform the test
	theArray = NArray(kListSInt32);
	theArray.Clear();
	REQUIRE(theArray.IsEmpty());

	theArray = NArray(kListSInt32);
	theArray.Clear();
	theArray.AppendValue(kTestSInt32);
	theArray.SetValue(0, kTestSInt64);
	REQUIRE(theArray.GetValueSInt64(0) == kTestSInt64);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NARRAY("Join")
{


	// Perform the test
	theArray  = NArray(kListSInt32);
	theArray2 = NArray(kListSInt64);
	theArray.Join(theArray2);
	REQUIRE(theArray.GetSize() == (NIndex) (kListSInt32.size() + kListSInt64.size()));
}





#pragma mark private
//============================================================================
//		TArray::ForEach : ForEach functor.
//----------------------------------------------------------------------------
void TArray::ForEach(NIndex theIndex, const NVariant &/*theValue*/, NIndex *theCount)
{


	// Compiler warning
	NN_UNUSED(theIndex);



	// Update the count
	REQUIRE(theIndex == *theCount);
	*theCount = *theCount + 1;
}
