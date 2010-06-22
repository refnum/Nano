/*	NAME:
		TData.cpp

	DESCRIPTION:
		NData tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NData.h"

#include "TData.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8 kBlock1[]						= { 0xAA, 0xBB, 0xCC, 0xDD };
static const UInt8 kBlock2[]						= { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
static const UInt8 kBlock3[]						= { 0x1A, 0x2B, 0x3C };

static const UInt8 kBlock1_and_3[]					= { 0xAA, 0xBB, 0xCC, 0xDD, 0x1A, 0x2B, 0x3C };

static const NData kNDataTest1(NN_ARRAY_SIZE(kBlock1), kBlock1);
static const NData kNDataTest2(NN_ARRAY_SIZE(kBlock2), kBlock2);





//============================================================================
//		TData::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TData::Execute(void)
{	UInt32			n, theSize;
	const NData		*testConst;
	NData			testData;



	// Execute the tests
	NN_ASSERT(!kNDataTest1.IsEmpty());
	NN_ASSERT(!kNDataTest2.IsEmpty());

	NN_ASSERT(kNDataTest1.GetSize() == NN_ARRAY_SIZE(kBlock1));
	NN_ASSERT(kNDataTest2.GetSize() == NN_ARRAY_SIZE(kBlock2));

	NN_ASSERT(testData.IsEmpty());
	NN_ASSERT(testData.GetSize() == 0);
	NN_ASSERT(testData.GetData() == NULL);

	testData = kNDataTest1;
	NN_ASSERT(!testData.IsEmpty());

	testData.Clear();
	NN_ASSERT(testData.IsEmpty());
	NN_ASSERT(testData.GetSize() == 0);

	testData = kNDataTest1;
	testData.SetData(NN_ARRAY_SIZE(kBlock1_and_3), kBlock1_and_3);

	theSize   = testData.GetSize();
	testConst = &testData;

	for (n = 0; n < theSize; n++)
		NN_ASSERT(*(testConst->GetData(n)) == kBlock1_and_3[n]);

	*(testData.GetData(2)) = *(testData.GetData(2)) + 1;
	testConst = &testData;
	NN_ASSERT(*(testConst->GetData(2)) != kBlock1_and_3[2]);

	testData = kNDataTest1;
	testData.SetData(kNDataTest2.GetSize(), kNDataTest2.GetData());
	NN_ASSERT(memcmp(kBlock2, testData.GetData(), testData.GetSize()) == 0);

	testData = kNDataTest1;
	NN_ASSERT(testData.AppendData(NN_ARRAY_SIZE(kBlock3), kBlock3) != NULL);
	NN_ASSERT(testData.GetSize() == (kNDataTest1.GetSize() + NN_ARRAY_SIZE(kBlock3)));
	NN_ASSERT(memcmp(kBlock1_and_3, testData.GetData(), testData.GetSize()) == 0);

	testData = kNDataTest1;
	NN_ASSERT(testData.AppendData(NN_ARRAY_SIZE(kBlock3), kBlock3) != NULL);
	NN_ASSERT(testData.ReplaceData(NRange(0, NN_ARRAY_SIZE(kBlock1)), NN_ARRAY_SIZE(kBlock3), kBlock3));
	NN_ASSERT(testData.GetSize() == (2 * NN_ARRAY_SIZE(kBlock3)));
	NN_ASSERT(memcmp(kBlock3, testData.GetData(0),                       NN_ARRAY_SIZE(kBlock3)) == 0);
	NN_ASSERT(memcmp(kBlock3, testData.GetData(NN_ARRAY_SIZE(kBlock3)), NN_ARRAY_SIZE(kBlock3)) == 0);
	testData.RemoveData(NRange(0, NN_ARRAY_SIZE(kBlock3)));
	NN_ASSERT(testData.GetSize() == (1 * NN_ARRAY_SIZE(kBlock3)));
	NN_ASSERT(memcmp(kBlock3, testData.GetData(), NN_ARRAY_SIZE(kBlock3)) == 0);

	testData.Clear();
	NN_ASSERT(testData.AppendData(NN_ARRAY_SIZE(kBlock3), kBlock3) != NULL);
	NN_ASSERT(testData.AppendData(NN_ARRAY_SIZE(kBlock3), kBlock3) != NULL);
	NN_ASSERT(testData.ReplaceData(NRange(0, NN_ARRAY_SIZE(kBlock3)), NN_ARRAY_SIZE(kBlock1), kBlock1));
	NN_ASSERT(testData.GetSize() == NN_ARRAY_SIZE(kBlock1_and_3));
	NN_ASSERT(memcmp(kBlock1_and_3, testData.GetData(), testData.GetSize()) == 0);

	testData = kNDataTest1;
	testData.RemoveData(NN_ARRAY_SIZE(kBlock1), true);
	NN_ASSERT(testData.IsEmpty());

	testData = kNDataTest1;
	testData.RemoveData(NN_ARRAY_SIZE(kBlock1), false);
	NN_ASSERT(testData.IsEmpty());
}


