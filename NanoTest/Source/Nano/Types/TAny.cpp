/*	NAME:
		TAny.cpp

	DESCRIPTION:
		NAny tests.

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
#include "NAny.h"
#include "NArray.h"
#include "NData.h"
#include "NDictionary.h"
#include "NNumber.h"
#include "NStdContainer.h"
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





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TAny)
{
	NAny theValue;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Default")
{


	// Perform the test
	REQUIRE(theValue.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Constructor")
{


	// Perform the test
	NAny     valueA(123);
	std::any valueB(123);

	NAny valueX(valueA);
	NAny valueY(valueB);
	NAny valueZ(123);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Assignment")
{


	// Perform the test
	NAny     valueA(123);
	std::any valueB(123);

	NAny valueX = valueA;
	NAny valueY = valueB;
	NAny valueZ = 123;
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "IsEmpty")
{


	// Perform the test
	REQUIRE(theValue.IsEmpty());

	theValue = 123;
	REQUIRE(!theValue.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Clear")
{


	// Perform the test
	theValue = 123;
	REQUIRE(!theValue.IsEmpty());

	theValue.Clear();
	REQUIRE(theValue.IsEmpty());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "GetType")
{


	// Perform the test
	theValue = kTestUInt32;
	REQUIRE(theValue.GetType() == typeid(kTestUInt32));
	REQUIRE(theValue.GetType() != typeid(kTestInt64));
	REQUIRE(theValue.GetType() != typeid(kTestFloat32));

	theValue = kTestInt64;
	REQUIRE(theValue.GetType() != typeid(kTestUInt32));
	REQUIRE(theValue.GetType() == typeid(kTestInt64));
	REQUIRE(theValue.GetType() != typeid(kTestFloat32));

	theValue = kTestFloat32;
	REQUIRE(theValue.GetType() != typeid(kTestUInt32));
	REQUIRE(theValue.GetType() != typeid(kTestInt64));
	REQUIRE(theValue.GetType() == typeid(kTestFloat32));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Is")
{


	// Perform the test
	theValue = kTestBool;
	REQUIRE(theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestUInt32;
	REQUIRE(!theValue.IsBool());
	REQUIRE(theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestUInt64;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestInt32;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestInt64;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestFloat32;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestFloat64;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestArray;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestData;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestDictionary;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestNumber;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestString;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestTime;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());
	REQUIRE(!theValue.IsArray());
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsDictionary());
	REQUIRE(!theValue.IsNumber());
	REQUIRE(!theValue.IsString());
	REQUIRE(theValue.IsTime());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Get/Value")
{


	// Perform the test
	theValue = kTestBool;
	REQUIRE(theValue.GetBool() == kTestBool);

	theValue = kTestUInt32;
	REQUIRE(theValue.GetUInt32() == kTestUInt32);

	theValue = kTestUInt64;
	REQUIRE(theValue.GetUInt64() == kTestUInt64);

	theValue = kTestInt32;
	REQUIRE(theValue.GetInt32() == kTestInt32);

	theValue = kTestInt64;
	REQUIRE(theValue.GetInt64() == kTestInt64);

	theValue = kTestFloat32;
	REQUIRE(theValue.GetFloat32() == kTestFloat32);

	theValue = kTestFloat64;
	REQUIRE(theValue.GetFloat64() == kTestFloat64);

	theValue = kTestArray;
	REQUIRE(theValue.GetArray() == kTestArray);

	theValue = kTestData;
	REQUIRE(theValue.GetData() == kTestData);

	theValue = kTestDictionary;
	REQUIRE(theValue.GetDictionary() == kTestDictionary);

	theValue = kTestNumber;
	REQUIRE(theValue.GetNumber() == kTestNumber);

	theValue = kTestString;
	REQUIRE(theValue.GetString() == kTestString);

	theValue = kTestTime;
	REQUIRE(theValue.GetTime() == kTestTime);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Get/Empty")
{


	// Perform the test
	REQUIRE(theValue.GetBool() == false);
	REQUIRE(theValue.GetUInt32() == 0);
	REQUIRE(theValue.GetUInt64() == 0);
	REQUIRE(theValue.GetInt32() == 0);
	REQUIRE(theValue.GetInt64() == 0);
	REQUIRE(theValue.GetFloat32() == 0.0f);
	REQUIRE(theValue.GetFloat64() == 0.0);
	REQUIRE(theValue.GetArray() == NArray());
	REQUIRE(theValue.GetData() == NData());
	REQUIRE(theValue.GetDictionary() == NDictionary());
	REQUIRE(theValue.GetNumber() == NNumber());
	REQUIRE(theValue.GetString() == NString());
	REQUIRE(theValue.GetTime() == NTime());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Compare")
{


	// Perform the test
	NAny valueA = kTestUInt32;
	NAny valueB = kTestFloat32;
	REQUIRE(valueB < valueA);
	REQUIRE(valueA > valueB);

	theValue = valueA;
	REQUIRE(theValue == valueA);
	REQUIRE(theValue != valueB);

	theValue = valueB;
	REQUIRE(theValue != valueA);
	REQUIRE(theValue == valueB);
}
