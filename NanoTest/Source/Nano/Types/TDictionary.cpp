/*	NAME:
		TDictionary.cpp

	DESCRIPTION:
		NDictionary tests.

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
#include "NData.h"
#include "NDictionary.h"
#include "NStdAlgorithm.h"
#include "NTestFixture.h"
#include "NTime.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const uint8_t kTestBytes[]{0xA1, 0xB1, 0xC1, 0xD1, 0xA2, 0xB2, 0xC2, 0xD2};

static const NString kTestA                                 = "A";
static const NString kTestB                                 = "B";
static const NString kTestC                                 = "C";
static const NString kTest1                                 = "1";
static const NString kTest2                                 = "2";
static const NString kTest3                                 = "3";

static constexpr bool      kTestBool                        = true;
static constexpr int32_t   kTestInt32                       = 32;
static constexpr int64_t   kTestInt64                       = 65;
static constexpr float32_t kTestFloat32                     = 32.5f;
static constexpr float64_t kTestFloat64                     = 64.5;
static const NData         kTestData(sizeof(kTestBytes), kTestBytes);
static const NString       kTestString("Testing");
static constexpr NTime     kTestTime(100);





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TDictionary)
{
	NDictionary theDictionary;

	SETUP
	{
		theDictionary["Bool"]    = kTestBool;
		theDictionary["Int32"]   = kTestInt32;
		theDictionary["Int64"]   = kTestInt64;
		theDictionary["Float32"] = kTestFloat32;
		theDictionary["Float64"] = kTestFloat64;
		theDictionary["Data"]    = kTestData;
		theDictionary["String"]  = kTestString;
		theDictionary["Time"]    = kTestTime;
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDictionary, "Default")
{


	// Perform the test
	NDictionary dictionaryA;

	REQUIRE(dictionaryA.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDictionary, "Clear")
{


	// Perform the test
	REQUIRE(!theDictionary.IsEmpty());

	theDictionary.Clear();
	REQUIRE(theDictionary.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDictionary, "GetSize")
{


	// Perform the test
	REQUIRE(theDictionary.GetSize() == 8);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDictionary, "HasKey")
{


	// Perform the test
	REQUIRE(theDictionary.HasKey("Int32"));
	REQUIRE(theDictionary.HasKey("Int64"));

	REQUIRE(!theDictionary.HasKey("Not32"));
	REQUIRE(!theDictionary.HasKey("Not64"));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDictionary, "RemoveKey")
{


	// Perform the test
	REQUIRE(theDictionary.HasKey("Int32"));
	REQUIRE(theDictionary.HasKey("Int64"));
	REQUIRE(theDictionary.GetSize() == 8);

	theDictionary.RemoveKey("Int32");
	theDictionary.RemoveKey("Int64");

	REQUIRE(!theDictionary.HasKey("Int32"));
	REQUIRE(!theDictionary.HasKey("Int64"));
	REQUIRE(theDictionary.GetSize() == 6);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDictionary, "RemoveKeys")
{


	// Perform the test
	REQUIRE(theDictionary.HasKey("Int32"));
	REQUIRE(theDictionary.HasKey("Int64"));
	REQUIRE(theDictionary.GetSize() == 8);

	theDictionary.RemoveKeys({"Int32", "Int64"});

	REQUIRE(!theDictionary.HasKey("Int32"));
	REQUIRE(!theDictionary.HasKey("Int64"));
	REQUIRE(theDictionary.GetSize() == 6);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDictionary, "GetKeys")
{


	// Perform the test
	NVectorString theKeys = theDictionary.GetKeys();
	REQUIRE(theKeys.size() == theDictionary.GetSize());

	nstd::sort(theKeys);
	REQUIRE(theKeys[0] == "Bool");
	REQUIRE(theKeys[1] == "Data");
	REQUIRE(theKeys[2] == "Float32");
	REQUIRE(theKeys[3] == "Float64");
	REQUIRE(theKeys[4] == "Int32");
	REQUIRE(theKeys[5] == "Int64");
	REQUIRE(theKeys[6] == "String");
	REQUIRE(theKeys[7] == "Time");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDictionary, "Invert")
{


	// Perform the test
	theDictionary.Clear();

	theDictionary[kTestA] = kTest1;
	theDictionary[kTestB] = kTest2;
	theDictionary[kTestC] = kTest3;

	theDictionary.Invert();
	REQUIRE(theDictionary[kTest1] == kTestA);
	REQUIRE(theDictionary[kTest2] == kTestB);
	REQUIRE(theDictionary[kTest3] == kTestC);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TDictionary, "Get")
{


	// Perform the test
	theDictionary["Bool"]    = kTestBool;
	theDictionary["Int32"]   = kTestInt32;
	theDictionary["Int64"]   = kTestInt64;
	theDictionary["Float32"] = kTestFloat32;
	theDictionary["Float64"] = kTestFloat64;
	theDictionary["Data"]    = kTestData;
	theDictionary["String"]  = kTestString;
	theDictionary["Time"]    = kTestTime;


	REQUIRE(theDictionary.GetSize() == 8);
	REQUIRE(theDictionary["Bool"] == kTestBool);
	REQUIRE(theDictionary["Int32"] == kTestInt32);
	REQUIRE(theDictionary["Int64"] == kTestInt64);
	REQUIRE(theDictionary["Float32"] == kTestFloat32);
	REQUIRE(theDictionary["Float64"] == kTestFloat64);
	REQUIRE(theDictionary["Data"] == kTestData);
	REQUIRE(theDictionary["String"] == kTestString);
	REQUIRE(theDictionary["Time"] == kTestTime);


	NDictionary dictionaryA(theDictionary);
	NDictionary dictionaryB(theDictionary);

	theDictionary["DictionaryA"] = dictionaryA;
	theDictionary["DictionaryB"] = dictionaryB;

	REQUIRE(theDictionary.GetSize() == 10);
	REQUIRE(theDictionary["DictionaryA"] == dictionaryA);
	REQUIRE(theDictionary["DictionaryB"] == dictionaryB);
}
