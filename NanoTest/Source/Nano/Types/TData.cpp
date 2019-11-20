/*	NAME:
		TData.cpp

	DESCRIPTION:
		NData tests.

	COPYRIGHT:
		Copyright (c) 2006-2019, refNum Software
		All rights reserved.

		Redistribution and use in source and binary forms, with or without
		modification, are permitted provided that the following conditions
		are met:
		
		1. Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.
		
		2. Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.
		
		3. Neither the name of the copyright holder nor the names of its
		contributors may be used to endorse or promote products derived from
		this software without specific prior written permission.
		
		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
		"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
		LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
		A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
		HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
		DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
		THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
		(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	___________________________________________________________________________
*/
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NData.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const uint8_t kBlock1[]                              = {0xAA, 0xBB, 0xCC, 0xDD};
static const uint8_t kBlock2[]                              = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
static const uint8_t kBlock3[]                              = {0x1A, 0x2B, 0x3C};

static const uint8_t kBlock1_and_3[]                        = {0xAA, 0xBB, 0xCC, 0xDD, 0x1A, 0x2B, 0x3C};

static const NData kTestData1(sizeof(kBlock1), kBlock1);
static const NData kTestData2(sizeof(kBlock2), kBlock2);





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TData)
{
	NData theData;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Default")
{


	// Perform the test
	REQUIRE(!kTestData1.IsEmpty());
	REQUIRE(!kTestData2.IsEmpty());

	REQUIRE(kTestData1.GetSize() == sizeof(kBlock1));
	REQUIRE(kTestData2.GetSize() == sizeof(kBlock2));

	REQUIRE(theData.IsEmpty());
	REQUIRE(theData.GetSize() == 0);
	REQUIRE(theData.GetData() == nullptr);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "CreateCopy")
{


	// Perform the test
	uint8_t tmpBlock[std::size(kBlock1)];
	memcpy(tmpBlock, kBlock1, sizeof(tmpBlock));

	theData = NData(sizeof(tmpBlock), tmpBlock, NDataSource::Copy);
	memset(tmpBlock, 0x00, sizeof(tmpBlock));

	REQUIRE(memcmp(theData.GetData(), tmpBlock, sizeof(tmpBlock)) != 0);
	REQUIRE(memcmp(theData.GetData(), kBlock1, sizeof(kBlock1)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "CreateNoCopy")
{


	// Perform the test
	uint8_t tmpBlock[std::size(kBlock1)];
	memcpy(tmpBlock, kBlock1, sizeof(tmpBlock));

	theData = NData(sizeof(tmpBlock), tmpBlock, NDataSource::View);
	memset(tmpBlock, 0x00, sizeof(tmpBlock));

	REQUIRE(memcmp(theData.GetData(), tmpBlock, sizeof(tmpBlock)) == 0);
	REQUIRE(memcmp(theData.GetData(), kBlock1, sizeof(kBlock1)) != 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Clear")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(!theData.IsEmpty());

	theData.Clear();
	REQUIRE(theData.IsEmpty());
	REQUIRE(theData.GetSize() == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SizeDown")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(theData.GetSize() > 1);

	theData.SetSize(1);
	REQUIRE(theData.GetSize() == 1);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SizeUp")
{


	// Perform the test
	size_t theDelta = 10;
	theData         = kTestData1;
	size_t oldSize  = theData.GetSize();
	REQUIRE(oldSize != 0);

	const uint8_t* thePtr = theData.GetData(0);
	for (size_t n = 0; n < oldSize; n++)
	{
		REQUIRE(thePtr[n] != 0x00);
	}

	theData.SetSize(oldSize + theDelta);

	thePtr = theData.GetData(oldSize);
	for (size_t n = 0; n < theDelta; n++)
	{
		REQUIRE(thePtr[n] == 0x00);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Reserve")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(theData.GetSize() == kTestData1.GetSize());

	theData.SetCapacity(theData.GetSize() * 10);
	REQUIRE(theData.GetSize() == kTestData1.GetSize());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "GetData")
{


	// Perform the test
	theData = kTestData2;

	NData sliceData = theData.GetData(NRange(0, 3));
	REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(0), 3) == 0);

	sliceData = theData.GetData(NRange(2, 3));
	REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(2), 3) == 0);

	sliceData = theData.GetData(NRange(3, 50));
	REQUIRE(sliceData.GetSize() == 3);
	REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(3), 3) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SetDataValue")
{


	// Perform the test
	theData.SetData(sizeof(kBlock1), kBlock1);
	REQUIRE(theData.GetSize() == sizeof(kBlock1));
	REQUIRE(memcmp(theData.GetData(), kBlock1, sizeof(kBlock1)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SetDataZero")
{


	// Perform the test
	theData.SetData(sizeof(kBlock2), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == sizeof(kBlock2));

	const uint8_t* thePtr = theData.GetData();
	for (size_t n = 0; n < sizeof(kBlock2); n++)
	{
		REQUIRE(thePtr[n] == 0x00);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SetDataClear")
{


	// Perform the test
	theData.SetData(sizeof(kBlock2), kBlock2);
	REQUIRE(theData.GetSize() == sizeof(kBlock2));

	theData.SetData(0, kBlock2);
	REQUIRE(theData.IsEmpty());

	theData.SetData(sizeof(kBlock2), kBlock2);
	REQUIRE(theData.GetSize() == sizeof(kBlock2));

	theData.SetData(0, nullptr);
	REQUIRE(theData.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "InsertDataValue")
{


	// Perform the test
	theData.InsertData(0, kTestData1);
	REQUIRE(theData == kTestData1);

	theData.InsertData(1, kTestData1);
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() * 2));

	theData.InsertData(2, sizeof(kBlock1), kBlock1);
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() * 3));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "InsertDataZero")
{


	// Perform the test
	theData.InsertData(0, sizeof(kBlock1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 1));

	theData.InsertData(1, sizeof(kBlock1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 2));

	theData.InsertData(2, sizeof(kBlock1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 3));

	const uint8_t* thePtr = theData.GetData();
	for (size_t n = 0; n < theData.GetSize(); n++)
	{
		REQUIRE(thePtr[n] == 0x00);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Remove")
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





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "AppendDataValue")
{


	// Perform the test
	theData.AppendData(kTestData1);
	REQUIRE(theData == kTestData1);

	theData.AppendData(kTestData1);
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() * 2));

	theData.AppendData(sizeof(kBlock1), kBlock1);
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() * 3));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "AppendDataZero")
{


	// Perform the test
	theData.AppendData(sizeof(kBlock1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 1));

	theData.AppendData(sizeof(kBlock1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 2));

	theData.AppendData(sizeof(kBlock1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kBlock1) * 3));

	const uint8_t* thePtr = theData.GetData();
	for (size_t n = 0; n < theData.GetSize(); n++)
	{
		REQUIRE(thePtr[n] == 0x00);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Replace")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(theData.AppendData(sizeof(kBlock3), kBlock3) != nullptr);
	REQUIRE(theData.ReplaceData(NRange(0, std::size(kBlock1)), sizeof(kBlock3), kBlock3) !=
			nullptr);

	REQUIRE(theData.GetSize() == (2 * sizeof(kBlock3)));
	REQUIRE(memcmp(kBlock3, theData.GetData(0), sizeof(kBlock3)) == 0);
	REQUIRE(memcmp(kBlock3, theData.GetData(sizeof(kBlock3)), sizeof(kBlock3)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Compare")
{


	// Perform the test
	theData = kTestData1;
	REQUIRE(theData == kTestData1);
	REQUIRE(theData != kTestData2);
}





//=============================================================================
//		Test cases
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Concatenation1")
{


	// Perform the test
	theData = kTestData1;
	theData += kTestData2;
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() + kTestData2.GetSize()));
	REQUIRE(memcmp(theData.GetData(0), kTestData1.GetData(0), kTestData1.GetSize()) == 0);
	REQUIRE(memcmp(theData.GetData(kTestData1.GetSize()),
				   kTestData2.GetData(0),
				   kTestData2.GetSize()) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Concatenation2")
{


	// Perform the test
	theData = kTestData1 + kTestData2;
	REQUIRE(theData.GetSize() == (kTestData1.GetSize() + kTestData2.GetSize()));
	REQUIRE(memcmp(theData.GetData(0), kTestData1.GetData(0), kTestData1.GetSize()) == 0);
	REQUIRE(memcmp(theData.GetData(kTestData1.GetSize()),
				   kTestData2.GetData(0),
				   kTestData2.GetSize()) == 0);
}
