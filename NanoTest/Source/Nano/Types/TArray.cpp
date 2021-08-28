/*	NAME:
		TArray.cpp

	DESCRIPTION:
		NArray tests.

	COPYRIGHT:
		Copyright (c) 2006-2021, refNum Software
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
#include "NArray.h"
#include "NData.h"
#include "NDictionary.h"
#include "NNumber.h"
#include "NStdAlgorithm.h"
#include "NStdContainer.h"
#include "NString.h"
#include "NTestFixture.h"
#include "NTime.h"





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		GetTestDictionary : Get a test dictionary.
//-----------------------------------------------------------------------------
static NDictionary GetTestDictionary()
{


	// Get the dictionary
	NDictionary theResult;

	theResult["one"]   = 1;
	theResult["two"]   = "two";
	theResult["three"] = NNumber(3);

	return theResult;
}





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const uint8_t kTestUInt8s[]{0xA1, 0xB1, 0xC1, 0xD1, 0xA2, 0xB2, 0xC2, 0xD2};

static constexpr bool      kTestBool                        = false;
static constexpr uint32_t  kTestUInt32                      = 123;
static constexpr uint64_t  kTestUInt64                      = 456;
static constexpr int32_t   kTestInt32                       = -123;
static constexpr int64_t   kTestInt64                       = -456;
static constexpr float32_t kTestFloat32                     = 5.5f;
static constexpr float64_t kTestFloat64                     = 6.5;

static const NArray      kTestArray(nstd::vector(kTestUInt8s));
static const NData       kTestData(sizeof(kTestUInt8s), kTestUInt8s);
static const NDictionary kTestDictionary(GetTestDictionary());
static const NNumber     kTestNumber(100);
static const NString     kTestString("Testing");
static constexpr NTime   kTestTime(100);

static const NVectorInt32   kTestVectorInt32{1, 2};
static const NVectorInt64   kTestVectorInt64{1, 2, 3};
static const NVectorFloat32 kTestVectorFloat32{1.0, 2.0, 3.0, 4.0};
static const NVectorFloat64 kTestVectorFloat64{1.0, 2.0, 3.0, 4.0, 5.0};





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TArray)
{
	NArray theArray;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TArray, "Default")
{


	// Perform the test
	REQUIRE(theArray.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TArray, "Constructor")
{


	// Perform the test
	theArray = NArray(kTestVectorInt32);
	REQUIRE(theArray.GetSize() == kTestVectorInt32.size());

	theArray = NArray(kTestVectorInt64);
	REQUIRE(theArray.GetSize() == kTestVectorInt64.size());

	theArray = NArray(kTestVectorFloat32);
	REQUIRE(theArray.GetSize() == kTestVectorFloat32.size());

	theArray = NArray(kTestVectorFloat64);
	REQUIRE(theArray.GetSize() == kTestVectorFloat64.size());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TArray, "Clear")
{


	// Perform the test
	theArray = NArray(kTestVectorInt32);
	REQUIRE(!theArray.IsEmpty());

	theArray.Clear();
	REQUIRE(theArray.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TArray, "GetSize")
{


	// Perform the test
	theArray = NArray(kTestVectorInt32);
	REQUIRE(theArray.GetSize() == kTestVectorInt32.size());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TArray, "HasValue")
{


	// Perform the test
	theArray = NArray(kTestVectorInt32);
	REQUIRE(theArray.HasValue(2));
	REQUIRE(!theArray.HasValue(3));

	theArray = NArray(kTestVectorInt64);
	REQUIRE(theArray.HasValue(3));
	REQUIRE(!theArray.HasValue(4));

	theArray = NArray(kTestVectorFloat32);
	REQUIRE(theArray.HasValue(2.0f));
	REQUIRE(!theArray.HasValue(2.1));

	theArray = NArray(kTestVectorFloat64);
	REQUIRE(theArray.HasValue(4.0f));
	REQUIRE(!theArray.HasValue(4.1));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TArray, "RemoveValue")
{


	// Perform the test
	theArray = std::vector{1, 2, 3, 4, 5};

	theArray.RemoveValue(1);
	REQUIRE(theArray == NArray(std::vector{1, 3, 4, 5}));

	theArray.RemoveValue(3);
	REQUIRE(theArray == NArray(std::vector{1, 3, 4}));

	theArray.RemoveValue(0);
	REQUIRE(theArray == NArray(std::vector{3, 4}));

	theArray.RemoveValue(1);
	REQUIRE(theArray == NArray(std::vector{3}));

	theArray.RemoveValue(0);
	REQUIRE(theArray.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TArray, "RemoveValues")
{


	// Perform the test
	theArray = std::vector{1, 2, 3, 4, 5};

	theArray.RemoveValues(NRange(1, 2));
	REQUIRE(theArray == NArray({1, 4, 5}));

	theArray.RemoveValues(NRange(0, 0));
	REQUIRE(theArray == NArray({1, 4, 5}));

	theArray.RemoveValues(NRange(0, 1));
	REQUIRE(theArray == NArray({4, 5}));

	theArray.RemoveValues(NRange(0, 2));
	REQUIRE(theArray.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TArray, "Get")
{


	// Perform the test
	theArray.push_back(kTestBool);
	theArray.push_back(kTestUInt32);
	theArray.push_back(kTestUInt64);
	theArray.push_back(kTestInt32);
	theArray.push_back(kTestInt64);
	theArray.push_back(kTestFloat32);
	theArray.push_back(kTestFloat64);
	theArray.push_back(kTestArray);
	theArray.push_back(kTestData);
	theArray.push_back(kTestDictionary);
	theArray.push_back(kTestNumber);
	theArray.push_back(kTestString);
	theArray.push_back(kTestTime);

	REQUIRE(theArray.GetSize() == 13);
	REQUIRE(theArray.GetBool(0) == kTestBool);
	REQUIRE(theArray.GetUInt32(1) == kTestUInt32);
	REQUIRE(theArray.GetUInt64(2) == kTestUInt64);
	REQUIRE(theArray.GetInt32(3) == kTestInt32);
	REQUIRE(theArray.GetInt64(4) == kTestInt64);
	REQUIRE(theArray.GetFloat32(5) == kTestFloat32);
	REQUIRE(theArray.GetFloat64(6) == kTestFloat64);
	REQUIRE(theArray.GetArray(7) == kTestArray);
	REQUIRE(theArray.GetData(8) == kTestData);
	REQUIRE(theArray.GetDictionary(9) == kTestDictionary);
	REQUIRE(theArray.GetNumber(10) == kTestNumber);
	REQUIRE(theArray.GetString(11) == kTestString);
	REQUIRE(theArray.GetTime(12) == kTestTime);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TArray, "GetVector")
{


	// Perform the test
	std::vector<uint8_t> theVector = theArray.GetVector<uint8_t>();
	theArray.SetVector(theVector);

	std::vector<uint8_t> theResult = theArray.GetVector<uint8_t>();
	REQUIRE(theResult.size() == theVector.size());

	for (size_t n = 0; n < theVector.size(); n++)
	{
		REQUIRE(theVector[n] == theResult[n]);
	}
}
