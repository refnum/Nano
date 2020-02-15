/*	NAME:
		TData.cpp

	DESCRIPTION:
		NData tests.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
static const uint8_t kTestArray1[]                          = {0xA1, 0xB1, 0xC1, 0xD1, 0xA2, 0xB2, 0xC2, 0xD2};
static const uint8_t kTestArray2[]                          = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30};

static const NData kTestDataSmall(sizeof(kTestArray1), kTestArray1);
static const NData kTestDataLarge(sizeof(kTestArray2), kTestArray2);





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TData)
{
	std::vector<NData> dataObjects;

	SETUP
	{
		dataObjects.push_back(kTestDataSmall);
		dataObjects.push_back(kTestDataLarge);
		dataObjects.push_back(kTestDataSmall.GetData(NRange(1, kTestDataSmall.GetSize() - 2)));
		dataObjects.push_back(kTestDataLarge.GetData(NRange(1, kTestDataLarge.GetSize() - 2)));
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Default")
{


	// Perform the test
	NData theData;

	REQUIRE(theData.IsEmpty());
	REQUIRE(theData.GetHash() == 0);
	REQUIRE(theData.GetSize() == 0);
	REQUIRE(theData.GetData() == nullptr);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Container")
{


	// Perform the test
	for (auto theData : dataObjects)
	{
		REQUIRE(!theData.IsEmpty());
		REQUIRE(theData.GetSize() != 0);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "CreateCopy")
{


	// Perform the test
	uint8_t tmpArray[std::max(sizeof(kTestArray1), sizeof(kTestArray2))];
	NData   theData;


	memcpy(tmpArray, kTestArray1, sizeof(kTestArray1));
	theData = NData(sizeof(kTestArray1), kTestArray1, NDataSource::Copy);
	memset(tmpArray, 0x00, sizeof(tmpArray));

	REQUIRE(memcmp(theData.GetData(), tmpArray, theData.GetSize()) != 0);
	REQUIRE(memcmp(theData.GetData(), kTestArray1, theData.GetSize()) == 0);


	memcpy(tmpArray, kTestArray2, sizeof(kTestArray2));
	theData = NData(sizeof(kTestArray2), kTestArray2, NDataSource::Copy);
	memset(tmpArray, 0x00, sizeof(tmpArray));

	REQUIRE(memcmp(theData.GetData(), tmpArray, theData.GetSize()) != 0);
	REQUIRE(memcmp(theData.GetData(), kTestArray2, theData.GetSize()) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "CreateNoCopy")
{


	// Perform the test
	uint8_t tmpArray[std::max(sizeof(kTestArray1), sizeof(kTestArray2))];
	NData   theData;


	memcpy(tmpArray, kTestArray1, sizeof(kTestArray1));
	theData = NData(sizeof(kTestArray1), tmpArray, NDataSource::View);
	memset(tmpArray, 0x00, sizeof(tmpArray));

	REQUIRE(memcmp(theData.GetData(), tmpArray, theData.GetSize()) == 0);
	REQUIRE(memcmp(theData.GetData(), kTestArray1, theData.GetSize()) != 0);


	memcpy(tmpArray, kTestArray2, sizeof(kTestArray2));
	theData = NData(sizeof(kTestArray2), tmpArray, NDataSource::View);
	memset(tmpArray, 0x00, sizeof(tmpArray));

	REQUIRE(memcmp(theData.GetData(), tmpArray, theData.GetSize()) == 0);
	REQUIRE(memcmp(theData.GetData(), kTestArray2, theData.GetSize()) != 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Clear")
{


	// Perform the test
	for (auto theData : dataObjects)
	{
		REQUIRE(!theData.IsEmpty());
		REQUIRE(theData.GetSize() != 0);

		theData.Clear();

		REQUIRE(theData.IsEmpty());
		REQUIRE(theData.GetSize() == 0);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SizeDown")
{


	// Perform the test
	for (auto theData : dataObjects)
	{
		REQUIRE(theData.GetSize() > 1);

		theData.SetSize(1);

		REQUIRE(theData.GetSize() == 1);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SizeUp")
{


	// Perform the test
	size_t theDelta = 100;

	for (auto theData : dataObjects)
	{
		// Save the existing content
		//
		// We save a copy of the existing content to test that resizing data
		// that is using a slice will resolve that slice to become mutable.
		size_t oldSize = theData.GetSize();
		REQUIRE(oldSize != 0);

		NVectorUInt8 oldContent(oldSize, 0);

		const uint8_t* thePtr = theData.GetData();
		for (size_t n = 0; n < oldSize; n++)
		{
			REQUIRE(thePtr[n] != 0x00);
			oldContent[n] = thePtr[n];
		}


		// Grow the data
		//
		// Existing content should be preserved, new content should be 0.
		size_t newSize = oldSize + theDelta;
		theData.SetSize(newSize);

		thePtr = theData.GetData();
		for (size_t n = 0; n < newSize; n++)
		{
			if (n < oldSize)
			{
				REQUIRE(thePtr[n] == oldContent[n]);
			}
			else
			{
				REQUIRE(thePtr[n] == 0x00);
			}
		}
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Capacity")
{


	// Perform the test
	size_t theCapacity = 100;

	for (auto theData : dataObjects)
	{
		size_t oldSize = theData.GetSize();
		REQUIRE(oldSize != 0);

		theData.SetCapacity(theCapacity);

		REQUIRE(theData.GetSize() == oldSize);
		REQUIRE(theData.GetCapacity() == theCapacity);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "GetData")
{


	// Perform the test
	for (auto theData : dataObjects)
	{
		NData sliceData = theData.GetData(NRange(0, 4));
		REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(0), 4) == 0);

		sliceData = theData.GetData(NRange(1, 3));
		REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(1), 3) == 0);

		sliceData = theData.GetData(NRange(2, 2));
		REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(2), 2) == 0);

		sliceData = theData.GetData(NRange(3, 1));
		REQUIRE(sliceData.GetSize() == 1);
		REQUIRE(memcmp(sliceData.GetData(0), theData.GetData(3), 1) == 0);

		const uint8_t* constPtr = theData.GetData();
		REQUIRE(constPtr != nullptr);

		const uint8_t* mutablePtr = theData.GetMutableData();
		REQUIRE(mutablePtr != nullptr);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SetDataValue")
{


	// Perform the test
	NData theData;


	theData.SetData(sizeof(kTestArray1), kTestArray1);

	REQUIRE(theData.GetSize() == sizeof(kTestArray1));
	REQUIRE(memcmp(theData.GetData(), kTestArray1, sizeof(kTestArray1)) == 0);


	theData.SetData(sizeof(kTestArray2), kTestArray2);

	REQUIRE(theData.GetSize() == sizeof(kTestArray2));
	REQUIRE(memcmp(theData.GetData(), kTestArray2, sizeof(kTestArray2)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SetDataZero")
{


	// Perform the test
	std::vector<size_t> theSizes{10, 100};

	for (auto theSize : theSizes)
	{
		NData theData;

		theData.SetData(theSize, nullptr, NDataSource::Zero);
		REQUIRE(theData.GetSize() == theSize);

		const uint8_t* thePtr = theData.GetData();
		for (size_t n = 0; n < theSize; n++)
		{
			REQUIRE(thePtr[n] == 0x00);
		}
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "SetDataNone")
{


	// Perform the test
	NData theData;

	theData.SetData(sizeof(kTestArray1), kTestArray1);
	REQUIRE(theData.GetSize() == sizeof(kTestArray1));

	theData.SetData(0, kTestArray1);
	REQUIRE(theData.IsEmpty());

	theData.SetData(sizeof(kTestArray1), kTestArray1);
	REQUIRE(theData.GetSize() == sizeof(kTestArray1));

	theData.SetData(0, nullptr);
	REQUIRE(theData.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "InsertDataValue")
{


	// Perform the test
	for (auto theData : dataObjects)
	{
		NData tmpData;

		tmpData.InsertData(0, theData);
		REQUIRE(theData == theData);

		tmpData.InsertData(1, theData);
		REQUIRE(tmpData.GetSize() == (theData.GetSize() * 2));

		tmpData.InsertData(2, theData.GetSize(), theData.GetData());
		REQUIRE(tmpData.GetSize() == (theData.GetSize() * 3));
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "InsertDataZero")
{


	// Perform the test
	NData theData;

	theData.InsertData(0, sizeof(kTestArray1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kTestArray1) * 1));

	theData.InsertData(1, sizeof(kTestArray1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kTestArray1) * 2));

	theData.InsertData(2, sizeof(kTestArray2), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == ((sizeof(kTestArray1) * 2) + sizeof(kTestArray2)));

	const uint8_t* thePtr = theData.GetData();
	for (size_t n = 0; n < theData.GetSize(); n++)
	{
		REQUIRE(thePtr[n] == 0x00);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "AppendDataValue")
{


	// Perform the test
	for (auto theData : dataObjects)
	{
		NData tmpData;

		tmpData.AppendData(theData);
		REQUIRE(tmpData == theData);

		tmpData.AppendData(theData);
		REQUIRE(tmpData.GetSize() == (theData.GetSize() * 2));

		tmpData.AppendData(theData.GetSize(), theData.GetData());
		REQUIRE(tmpData.GetSize() == (theData.GetSize() * 3));
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "AppendDataZero")
{


	// Perform the test
	NData          theData;
	const uint8_t* thePtr;


	theData.Clear();
	theData.AppendData(sizeof(kTestArray1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kTestArray1) * 1));

	theData.AppendData(sizeof(kTestArray1), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kTestArray1) * 2));

	thePtr = theData.GetData();
	for (size_t n = 0; n < theData.GetSize(); n++)
	{
		REQUIRE(thePtr[n] == 0x00);
	}


	theData.Clear();
	theData.AppendData(sizeof(kTestArray2), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kTestArray2) * 1));

	theData.AppendData(sizeof(kTestArray2), nullptr, NDataSource::Zero);
	REQUIRE(theData.GetSize() == (sizeof(kTestArray2) * 2));

	thePtr = theData.GetData();
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
	for (auto theData : dataObjects)
	{
		theData.RemoveData(NRange(0, theData.GetSize()));
		REQUIRE(theData.IsEmpty());

		theData.AppendData(sizeof(kTestArray1), kTestArray1);
		theData.AppendData(sizeof(kTestArray2), kTestArray2);
		theData.RemoveData(NRange(0, sizeof(kTestArray1)));
		REQUIRE(theData.GetSize() == sizeof(kTestArray2));
		REQUIRE(memcmp(theData.GetData(), kTestArray2, sizeof(kTestArray2)) == 0);

		theData.AppendData(sizeof(kTestArray2), kTestArray2);
		theData.AppendData(sizeof(kTestArray1), kTestArray1);
		theData.RemoveData(NRange(sizeof(kTestArray2), sizeof(kTestArray2)));
		theData.RemoveData(NRange(0, sizeof(kTestArray2)));
		REQUIRE(theData.GetSize() == sizeof(kTestArray1));
		REQUIRE(memcmp(theData.GetData(), kTestArray1, sizeof(kTestArray1)) == 0);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Replace")
{


	// Perform the test
	NData theData;

	REQUIRE(theData.AppendData(sizeof(kTestArray1), kTestArray1) != nullptr);
	REQUIRE(theData.ReplaceData(NRange(0, sizeof(kTestArray1)), sizeof(kTestArray2), kTestArray2) !=
			nullptr);

	REQUIRE(theData.GetSize() == sizeof(kTestArray2));
	REQUIRE(memcmp(theData.GetData(), kTestArray2, sizeof(kTestArray2)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Find")
{


	// Perform the test
	NRange theRange;
	NData  theData;

	REQUIRE(theData.AppendData(kTestDataSmall) != nullptr);
	REQUIRE(theData.AppendData(kTestDataLarge) != nullptr);

	theRange = theData.Find(kTestDataSmall);
	REQUIRE(theRange == NRange(0, kTestDataSmall.GetSize()));

	theRange = theData.Find(kTestDataLarge);
	REQUIRE(theRange == NRange(kTestDataSmall.GetSize(), kTestDataLarge.GetSize()));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "FindAll")
{


	// Get the state we need
	NData theData;

	for (auto n = 0; n < 3; n++)
	{
		theData.AppendData(kTestDataSmall);
		theData.AppendData(kTestDataLarge);
	}


	// Perform the test
	NVectorRange theResult = theData.FindAll(kTestDataSmall);
	size_t       sizeSmall = kTestDataSmall.GetSize();
	size_t       sizeLarge = kTestDataLarge.GetSize();

	REQUIRE(theResult.size() == 3);
	REQUIRE(theResult[0] == NRange(0, sizeSmall));
	REQUIRE(theResult[1] == NRange((sizeSmall + sizeLarge) * 1, sizeSmall));
	REQUIRE(theResult[2] == NRange((sizeSmall + sizeLarge) * 2, sizeSmall));


	// Perform the test
	theResult = theData.FindAll(kTestDataSmall, NRange(2, 1000));

	REQUIRE(theResult.size() == 2);
	REQUIRE(theResult[0] == NRange((sizeSmall + sizeLarge) * 1, sizeSmall));
	REQUIRE(theResult[1] == NRange((sizeSmall + sizeLarge) * 2, sizeSmall));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "StartsWith")
{


	// Perform the test
	NData smallData = kTestDataSmall + kTestDataSmall;
	NData largeData = kTestDataLarge + kTestDataLarge;

	REQUIRE(smallData.StartsWith(kTestDataSmall));
	REQUIRE(largeData.StartsWith(kTestDataLarge));

	REQUIRE(!smallData.StartsWith(kTestDataLarge));
	REQUIRE(!largeData.StartsWith(kTestDataSmall));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "EndsWith")
{


	// Perform the test
	NData smallData = kTestDataSmall + kTestDataSmall;
	NData largeData = kTestDataLarge + kTestDataLarge;

	REQUIRE(smallData.EndsWith(kTestDataSmall));
	REQUIRE(largeData.EndsWith(kTestDataLarge));

	REQUIRE(!smallData.EndsWith(kTestDataLarge));
	REQUIRE(!largeData.EndsWith(kTestDataSmall));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Contains")
{


	// Perform the test
	NData smallData = kTestDataSmall + kTestDataSmall;
	NData largeData = kTestDataLarge + kTestDataLarge;

	REQUIRE(smallData.Contains(kTestDataSmall));
	REQUIRE(largeData.Contains(kTestDataLarge));

	REQUIRE(!smallData.Contains(kTestDataLarge));
	REQUIRE(!largeData.Contains(kTestDataSmall));

	REQUIRE(!smallData.Contains(kTestDataSmall, NRange(1, 3)));
	REQUIRE(!largeData.Contains(kTestDataLarge, NRange(1, 3)));
}





//=============================================================================
//		Test cases
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Appendable")
{


	// Perform the test
	for (auto theData : dataObjects)
	{
		size_t theSize = theData.GetSize();
		theData += kTestDataSmall;

		REQUIRE(theData.GetSize() == (theSize + kTestDataSmall.GetSize()));
		REQUIRE(memcmp(theData.GetData(theSize),
					   kTestDataSmall.GetData(),
					   kTestDataSmall.GetSize()) == 0);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "CompareEqual")
{


	// Perform the test
	for (auto theData : dataObjects)
	{
		if (theData == kTestDataSmall)
		{
			REQUIRE(theData == kTestDataSmall);
		}
		else if (theData == kTestDataLarge)
		{
			REQUIRE(theData == kTestDataLarge);
		}
		else
		{
			REQUIRE((theData != kTestDataSmall && theData != kTestDataLarge));
		}
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "CompareOrder")
{


	// Perform the test
	REQUIRE(kTestDataSmall <= kTestDataSmall);
	REQUIRE(kTestDataSmall < kTestDataLarge);

	REQUIRE(kTestDataLarge >= kTestDataLarge);
	REQUIRE(kTestDataLarge > kTestDataSmall);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TData, "Hashable")
{


	// Perform the test
	for (auto theData : dataObjects)
	{
		REQUIRE(theData.GetHash() != 0);
	}
}
