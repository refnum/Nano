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
static constexpr uint32_t  kTestUInt32                      = 123;
static constexpr int64_t   kTestInt64                       = -123;
static constexpr float32_t kTestFloat32                     = 123.5;





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TAny)
{
	NAny theValue;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Default")
{


	// Perform the test
	REQUIRE(theValue.IsEmpty());
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Construction")
{


	// Perform the test
	NAny     valueA(123);
	std::any valueB(123);

	NAny valueX(valueA);
	NAny valueY(valueB);
	NAny valueZ(123);
}





//=============================================================================
//		Test case
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
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "IsEmpty")
{


	// Perform the test
	REQUIRE(theValue.IsEmpty());

	theValue = 123;
	REQUIRE(!theValue.IsEmpty());
}





//=============================================================================
//		Test case
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
//		Test case
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
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Has")
{


	// Perform the test
	theValue = kTestUInt32;
	REQUIRE(!theValue.HasBool());
	REQUIRE(!theValue.HasUInt8());
	REQUIRE(!theValue.HasUInt16());
	REQUIRE(theValue.HasUInt32());
	REQUIRE(!theValue.HasUInt64());
	REQUIRE(!theValue.HasInt8());
	REQUIRE(!theValue.HasInt16());
	REQUIRE(!theValue.HasInt32());
	REQUIRE(!theValue.HasInt64());
	REQUIRE(!theValue.HasFloat32());
	REQUIRE(!theValue.HasFloat64());

	theValue = kTestInt64;
	REQUIRE(!theValue.HasBool());
	REQUIRE(!theValue.HasUInt8());
	REQUIRE(!theValue.HasUInt16());
	REQUIRE(!theValue.HasUInt32());
	REQUIRE(!theValue.HasUInt64());
	REQUIRE(!theValue.HasInt8());
	REQUIRE(!theValue.HasInt16());
	REQUIRE(!theValue.HasInt32());
	REQUIRE(theValue.HasInt64());
	REQUIRE(!theValue.HasFloat32());
	REQUIRE(!theValue.HasFloat64());

	theValue = kTestFloat32;
	REQUIRE(!theValue.HasBool());
	REQUIRE(!theValue.HasUInt8());
	REQUIRE(!theValue.HasUInt16());
	REQUIRE(!theValue.HasUInt32());
	REQUIRE(!theValue.HasUInt64());
	REQUIRE(!theValue.HasInt8());
	REQUIRE(!theValue.HasInt16());
	REQUIRE(!theValue.HasInt32());
	REQUIRE(!theValue.HasInt64());
	REQUIRE(theValue.HasFloat32());
	REQUIRE(!theValue.HasFloat64());
}





//=============================================================================
//		Test case
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
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TAny, "Compare")
{


	// Perform the test
	NAny valueA = kTestUInt32;
	NAny valueB = kTestFloat32;

	theValue = valueA;
	REQUIRE(theValue == valueA);
	REQUIRE(theValue != valueB);

	theValue = valueB;
	REQUIRE(theValue != valueA);
	REQUIRE(theValue == valueB);
}
