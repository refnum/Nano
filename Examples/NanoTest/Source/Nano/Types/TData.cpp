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
static const uint8_t kBlock1[]										= { 0xAA, 0xBB, 0xCC, 0xDD };
static const uint8_t kBlock2[]										= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
static const uint8_t kBlock3[]										= { 0x1A, 0x2B, 0x3C };

static const uint8_t kBlock1_and_3[]								= { 0xAA, 0xBB, 0xCC, 0xDD, 0x1A, 0x2B, 0x3C };

static const NData kTestData1(sizeof(kBlock1), kBlock1);
static const NData kTestData2(sizeof(kBlock2), kBlock2);





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDATA(...)												TEST_NANO(TData, ##__VA_ARGS__)

FIXTURE_NANO(TData)
{
	NData		theData;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Default")
{	NData	emptyData;



	// Perform the test
	REQUIRE(!kTestData1.IsEmpty());
	REQUIRE(!kTestData2.IsEmpty());

	REQUIRE(kTestData1.GetSize() == sizeof(kBlock1));
	REQUIRE(kTestData2.GetSize() == sizeof(kBlock2));

	REQUIRE(theData.IsEmpty());
	REQUIRE(theData.GetSize() == 0);
	REQUIRE(theData.GetData() == (const uint8_t *) NULL);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("CreateCopy")
{	uint8_t		tmpBlock[NN_ARRAY_SIZE(kBlock1)];



	// Perform the test
	memcpy(tmpBlock, kBlock1, sizeof(tmpBlock));
	theData = NData(sizeof(tmpBlock), tmpBlock, true);
	memset(tmpBlock, 0x00, sizeof(tmpBlock));

	REQUIRE(memcmp(theData.GetData(), tmpBlock, sizeof(tmpBlock)) != 0);
	REQUIRE(memcmp(theData.GetData(), kBlock1,  sizeof(kBlock1))  == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("CreateNoCopy")
{	uint8_t		tmpBlock[NN_ARRAY_SIZE(kBlock1)];



	// Perform the test
	memcpy(tmpBlock, kBlock1, sizeof(tmpBlock));
	theData = NData(sizeof(tmpBlock), tmpBlock, false);
	memset(tmpBlock, 0x00, sizeof(tmpBlock));

	REQUIRE(memcmp(theData.GetData(), tmpBlock, sizeof(tmpBlock)) == 0);
	REQUIRE(memcmp(theData.GetData(), kBlock1,  sizeof(kBlock1))  != 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Clear")
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
TEST_NDATA("SizeDown")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(theData.GetSize() > 1);

	REQUIRE(theData.SetSize(1));
	REQUIRE(theData.GetSize() == 1);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("SizeUp")
{	NIndex			n, oldSize, theDelta;
	const uint8_t	*thePtr;



	// Perform the test
	theDelta = 10;
	theData  = kTestData1;
	oldSize  = theData.GetSize();
	REQUIRE(oldSize != 0);

	thePtr = theData.GetData(0);
	for (n = 0; n < oldSize; n++)
		REQUIRE(thePtr[n] != 0x00);

	REQUIRE(theData.SetSize(oldSize + theDelta));

	thePtr = theData.GetData(oldSize);
	for (n = 0; n < theDelta; n++)
		REQUIRE(thePtr[n] == 0x00);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Reserve")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(theData.GetSize() == kTestData1.GetSize());
	
	theData.Reserve(theData.GetSize() * 10);
	REQUIRE(theData.GetSize() == kTestData1.GetSize());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("GetData")
{	NData	sliceData;



	// Perform the test
	theData = kTestData2;

	sliceData = theData.GetData(NRange(0, 3));
	REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(0), 3) == 0);

	sliceData = theData.GetData(NRange(2, 3));
	REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(2), 3) == 0);

	sliceData = theData.GetData(NRange(3, 50));
	REQUIRE(sliceData.GetSize() == 3);
	REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(3), 3) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("SetDataValue")
{


	// Perform the test
	theData.SetData(sizeof(kBlock1), kBlock1);
	REQUIRE(theData.GetSize() == sizeof(kBlock1));
	REQUIRE(memcmp(theData.GetData(), kBlock1, sizeof(kBlock1)) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("SetDataZero")
{	const uint8_t	*thePtr;
	NIndex			n;



	// Perform the test
	theData.SetData(sizeof(kBlock2), NULL);
	REQUIRE(theData.GetSize() == sizeof(kBlock2));

	thePtr = theData.GetData();
	for (n = 0; n < (NIndex) sizeof(kBlock2); n++)
		REQUIRE(thePtr[n] == 0x00);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("SetDataClear")
{


	// Perform the test
	theData.SetData(sizeof(kBlock2), kBlock2);
	REQUIRE(theData.GetSize() == sizeof(kBlock2));

	theData.SetData(0, kBlock2);
	REQUIRE(theData.IsEmpty());

	theData.SetData(sizeof(kBlock2), kBlock2);
	REQUIRE(theData.GetSize() == sizeof(kBlock2));

	theData.SetData(0, NULL);
	REQUIRE(theData.IsEmpty());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("InsertDataValue")
{


	// Perform the test
	theData.InsertData(0, kTestData1);
	REQUIRE(theData == kTestData1);

	theData.InsertData(1, kTestData1);
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() * 2));

	theData.InsertData(2, sizeof(kBlock1), kBlock1);
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() * 3));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("InsertDataZero")
{	const uint8_t	*thePtr;
	NIndex			n;



	// Perform the test
	theData.InsertData(0, sizeof(kBlock1), NULL);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 1));

	theData.InsertData(1, sizeof(kBlock1), NULL);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 2));

	theData.InsertData(2, sizeof(kBlock1), NULL);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 3));

	thePtr = theData.GetData();
	for (n = 0; n < theData.GetSize(); n++)
		REQUIRE(thePtr[n] == 0x00);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Remove")
{


	// Perform the test
	theData.SetData(sizeof(kBlock1), kBlock1);
	theData.RemoveData(NRange(0, sizeof(kBlock1)));
	REQUIRE(theData.IsEmpty());

	theData.SetData(sizeof(kBlock1_and_3), kBlock1_and_3);
	theData.RemoveData(NRange(0, sizeof(kBlock1)));
	REQUIRE(theData.GetSize() == sizeof(kBlock3));
	REQUIRE(memcmp(theData.GetData(), kBlock3, sizeof(kBlock3)) == 0);

	theData.SetData(sizeof(kBlock1_and_3), kBlock1_and_3);
	theData.RemoveData(NRange(sizeof(kBlock1), sizeof(kBlock3)));
	REQUIRE(theData.GetSize() == sizeof(kBlock1));
	REQUIRE(memcmp(theData.GetData(), kBlock1, sizeof(kBlock1)) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("AppendDataValue")
{


	// Perform the test
	theData.AppendData(kTestData1);
	REQUIRE(theData == kTestData1);

	theData.AppendData(kTestData1);
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() * 2));

	theData.AppendData(sizeof(kBlock1), kBlock1);
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() * 3));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("AppendDataZero")
{	const uint8_t	*thePtr;
	NIndex			n;



	// Perform the test
	theData.AppendData(sizeof(kBlock1), NULL);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 1));

	theData.AppendData(sizeof(kBlock1), NULL);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 2));

	theData.AppendData(sizeof(kBlock1), NULL);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 3));

	thePtr = theData.GetData();
	for (n = 0; n < theData.GetSize(); n++)
		REQUIRE(thePtr[n] == 0x00);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Replace")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(theData.AppendData(sizeof(kBlock3), kBlock3)                                     != (uint8_t *) NULL);
	REQUIRE(theData.ReplaceData(NRange(0, NN_ARRAY_SIZE(kBlock1)), sizeof(kBlock3), kBlock3) != (uint8_t *) NULL);

	REQUIRE(theData.GetSize() == (2 * sizeof(kBlock3)));
	REQUIRE(memcmp(kBlock3, theData.GetData(0),               sizeof(kBlock3)) == 0);
	REQUIRE(memcmp(kBlock3, theData.GetData(sizeof(kBlock3)), sizeof(kBlock3)) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Compare")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(theData == kTestData1);
	REQUIRE(theData != kTestData2);
}





//============================================================================
//		Test cases
//----------------------------------------------------------------------------
TEST_NDATA("Concatenation1")
{


	// Perform the test
	theData  = kTestData1;
	theData += kTestData2;
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() + kTestData2.GetSize()));
	REQUIRE(memcmp(theData.GetData(0),                    kTestData1.GetData(0), kTestData1.GetSize()) == 0);
	REQUIRE(memcmp(theData.GetData(kTestData1.GetSize()), kTestData2.GetData(0), kTestData2.GetSize()) == 0);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDATA("Concatenation2")
{


	// Perform the test
	theData = kTestData1 + kTestData2;
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() + kTestData2.GetSize()));
	REQUIRE(memcmp(theData.GetData(0),                    kTestData1.GetData(0), kTestData1.GetSize()) == 0);
	REQUIRE(memcmp(theData.GetData(kTestData1.GetSize()), kTestData2.GetData(0), kTestData2.GetSize()) == 0);
}





