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
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const uint8_t kTestBytes[]{0xA1, 0xB1, 0xC1, 0xD1, 0xA2, 0xB2, 0xC2, 0xD2};

static constexpr uint32_t  kTestUInt32                      = 123;
static constexpr int64_t   kTestInt64                       = -123;
static constexpr float32_t kTestFloat32                     = 123.5;
static const NData         kTestData(sizeof(kTestBytes), kTestBytes);
static const NString       kTestString("Testing");
static constexpr NTime     kTestTime(100);





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
	theValue = kTestUInt32;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt8());
	REQUIRE(!theValue.IsUInt16());
	REQUIRE(theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt8());
	REQUIRE(!theValue.IsInt16());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());

	theValue = kTestInt64;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt8());
	REQUIRE(!theValue.IsUInt16());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt8());
	REQUIRE(!theValue.IsInt16());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(theValue.IsInt64());
	REQUIRE(!theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());

	theValue = kTestFloat32;
	REQUIRE(!theValue.IsBool());
	REQUIRE(!theValue.IsUInt8());
	REQUIRE(!theValue.IsUInt16());
	REQUIRE(!theValue.IsUInt32());
	REQUIRE(!theValue.IsUInt64());
	REQUIRE(!theValue.IsInt8());
	REQUIRE(!theValue.IsInt16());
	REQUIRE(!theValue.IsInt32());
	REQUIRE(!theValue.IsInt64());
	REQUIRE(theValue.IsFloat32());
	REQUIRE(!theValue.IsFloat64());

	theValue = kTestData;
	REQUIRE(theValue.IsData());
	REQUIRE(!theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestString;
	REQUIRE(!theValue.IsData());
	REQUIRE(theValue.IsString());
	REQUIRE(!theValue.IsTime());

	theValue = kTestTime;
	REQUIRE(!theValue.IsData());
	REQUIRE(!theValue.IsString());
	REQUIRE(theValue.IsTime());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Get")
{


	// Perform the test
	theValue = kTestUInt32;
	REQUIRE(theValue.GetUInt32() == kTestUInt32);

	theValue = kTestInt64;
	REQUIRE(theValue.GetInt64() == kTestInt64);

	theValue = kTestFloat32;
	REQUIRE(theValue.GetFloat32() == kTestFloat32);

	theValue = kTestData;
	REQUIRE(theValue.GetData() == kTestData);

	theValue = kTestString;
	REQUIRE(theValue.GetString() == kTestString);

	theValue = kTestTime;
	REQUIRE(theValue.GetTime() == kTestTime);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Compare")
{


	// Perform the test
	NAny valueA = kTestUInt32;
	NAny valueB = kTestFloat32;
	REQUIRE(valueA < valueB);
	REQUIRE(valueB > valueA);

	theValue = valueA;
	REQUIRE(theValue == valueA);
	REQUIRE(theValue != valueB);

	theValue = valueB;
	REQUIRE(theValue != valueA);
	REQUIRE(theValue == valueB);
}
