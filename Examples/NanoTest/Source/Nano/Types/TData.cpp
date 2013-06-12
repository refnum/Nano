/*	NAME:
		TData.cpp

	DESCRIPTION:
		NData tests.

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
#include "NData.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8 kBlock1[]										= { 0xAA, 0xBB, 0xCC, 0xDD };
static const UInt8 kBlock2[]										= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
static const UInt8 kBlock3[]										= { 0x1A, 0x2B, 0x3C };

static const UInt8 kBlock1_and_3[]									= { 0xAA, 0xBB, 0xCC, 0xDD, 0x1A, 0x2B, 0x3C };

static const NData kTestData1(NN_ARRAY_SIZE(kBlock1), kBlock1);
static const NData kTestData2(NN_ARRAY_SIZE(kBlock2), kBlock2);





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDATA(_name, _desc)									NANO_TEST(TData, _name, _desc)

NANO_FIXTURE(TData)
{
	NData		theData;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Default", "Default state")
{	NData	emptyData;



	// Perform the test
	REQUIRE(!kTestData1.IsEmpty());
	REQUIRE(!kTestData2.IsEmpty());

	REQUIRE(kTestData1.GetSize() == NN_ARRAY_SIZE(kBlock1));
	REQUIRE(kTestData2.GetSize() == NN_ARRAY_SIZE(kBlock2));

	REQUIRE(theData.IsEmpty());
	REQUIRE(theData.GetSize() == 0);
	REQUIRE(theData.GetData() == NULL);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Clear", "Clear the value")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(!theData.IsEmpty());

	theData.Clear();
	REQUIRE(theData.IsEmpty());
	REQUIRE(theData.GetSize() == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Assign", "Assign data")
{	const NData		*constData;
	NIndex			n, theSize;



	// Perform the test
	theData = kTestData1;
	theData.SetData(NN_ARRAY_SIZE(kBlock1_and_3), kBlock1_and_3);

	theSize   = theData.GetSize();
	constData = &theData;

	for (n = 0; n < theSize; n++)
		REQUIRE(*(constData->GetData(n)) == kBlock1_and_3[n]);

	*(theData.GetData(2)) = *(theData.GetData(2)) + 1;
	constData = &theData;
	REQUIRE(*(constData->GetData(2)) != kBlock1_and_3[2]);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Append", "Append data")
{


	// Perform the test
	theData = kTestData1;
	theData.SetData(kTestData2.GetSize(), kTestData2.GetData());
	REQUIRE(memcmp(kBlock2, theData.GetData(), (size_t) theData.GetSize()) == 0);

	theData = kTestData1;
	REQUIRE(theData.AppendData(NN_ARRAY_SIZE(kBlock3), kBlock3) != NULL);
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() + NN_ARRAY_SIZE(kBlock3)));
	REQUIRE(memcmp(kBlock1_and_3, theData.GetData(), (size_t) theData.GetSize()) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Replace", "Replace data")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(theData.AppendData(NN_ARRAY_SIZE(kBlock3), kBlock3) != NULL);
	REQUIRE(theData.ReplaceData(NRange(0, NN_ARRAY_SIZE(kBlock1)), NN_ARRAY_SIZE(kBlock3), kBlock3) != NULL);
	REQUIRE(theData.GetSize() == (2 * NN_ARRAY_SIZE(kBlock3)));
	REQUIRE(memcmp(kBlock3, theData.GetData(0),                       NN_ARRAY_SIZE(kBlock3)) == 0);
	REQUIRE(memcmp(kBlock3, theData.GetData(NN_ARRAY_SIZE(kBlock3)), NN_ARRAY_SIZE(kBlock3)) == 0);

	theData.RemoveData(NRange(0, NN_ARRAY_SIZE(kBlock3)));
	REQUIRE(theData.GetSize() == (1 * NN_ARRAY_SIZE(kBlock3)));
	REQUIRE(memcmp(kBlock3, theData.GetData(), (size_t) NN_ARRAY_SIZE(kBlock3)) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Append+Replace", "Append and replace")
{


	// Perform the test
	REQUIRE(theData.AppendData(NN_ARRAY_SIZE(kBlock3), kBlock3) != NULL);
	REQUIRE(theData.AppendData(NN_ARRAY_SIZE(kBlock3), kBlock3) != NULL);
	REQUIRE(theData.ReplaceData(NRange(0, NN_ARRAY_SIZE(kBlock3)), NN_ARRAY_SIZE(kBlock1), kBlock1) != NULL);

	REQUIRE(theData.GetSize() == NN_ARRAY_SIZE(kBlock1_and_3));
	REQUIRE(memcmp(kBlock1_and_3, theData.GetData(), (size_t) theData.GetSize()) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Remove", "Remove data")
{


	// Perform the test
	theData = kTestData1;
	theData.RemoveData(NN_ARRAY_SIZE(kBlock1), true);
	REQUIRE(theData.IsEmpty());

	theData = kTestData1;
	theData.RemoveData(NN_ARRAY_SIZE(kBlock1), false);
	REQUIRE(theData.IsEmpty());
}


