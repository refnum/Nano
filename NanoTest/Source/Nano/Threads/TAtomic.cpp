/*	NAME:
		TAtomic.cpp

	DESCRIPTION:
		NAtomic tests.

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
// Nano
#include "NAtomic.h"
#include "NString.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint32_t kTestUInt32                       = 112233;
static const NString      kTestString                       = "Hello";

static constexpr uint32_t kOtherUInt32                      = 445566;
static const NString      kOtherString                      = "Goodbye";





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TAtomic){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "Constructor")
{


	// Perform the test
	NAtomic<uint32_t> valueA1;
	NAtomic<uint32_t> valueA2(kTestUInt32);

	NAtomic<NString> valueM1;
	NAtomic<NString> valueM2(kTestString);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "Assignment")
{


	// Perform the test
	NAtomic<uint32_t> valueA = kTestUInt32;
	NAtomic<NString>  valueM = kTestString;

	NN_UNUSED(valueA);
	NN_UNUSED(valueM);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "is_always_lock_free")
{


	// Perform the test
	REQUIRE(NAtomic<uint32_t>::is_always_lock_free);
	REQUIRE(!NAtomic<NString>::is_always_lock_free);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "is_locK_free")
{


	// Perform the test
	NAtomic<uint32_t> valueA;
	NAtomic<NString>  valueM;

	REQUIRE(valueA.is_lock_free());
	REQUIRE(!valueM.is_lock_free());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "store")
{


	// Perform the test
	NAtomic<uint32_t> valueA;
	NAtomic<NString>  valueM;

	valueA.store(kTestUInt32);
	valueM.store(kTestString);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "load")
{


	// Perform the test
	NAtomic<uint32_t> valueA;
	NAtomic<NString>  valueM;

	valueA.store(kTestUInt32);
	valueM.store(kTestString);

	REQUIRE(valueA.load() == kTestUInt32);
	REQUIRE(valueM.load() == kTestString);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "Cast")
{


	// Perform the test
	NAtomic<uint32_t> valueA(kTestUInt32);
	NAtomic<NString>  valueM(kTestString);

	uint32_t resultA = valueA;
	NString  resultM = valueM;

	REQUIRE(resultA == kTestUInt32);
	REQUIRE(resultM == kTestString);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "exchange")
{


	// Perform the test
	NAtomic<uint32_t> valueA(kTestUInt32);
	NAtomic<NString>  valueM(kTestString);

	uint32_t oldA = valueA.exchange(kOtherUInt32);
	NString  oldM = valueM.exchange(kOtherString);

	REQUIRE(oldA == kTestUInt32);
	REQUIRE(oldM == kTestString);

	REQUIRE(valueA == kOtherUInt32);
	REQUIRE(valueM == kOtherString);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "compare_exchange")
{


	// Perform the test
	NAtomic<uint32_t> valueA(kTestUInt32);
	NAtomic<NString>  valueM(kTestString);

	uint32_t expectedA(kTestUInt32);
	NString  expectedM(kTestString);

	bool changedA = valueA.compare_exchange_strong(expectedA, kOtherUInt32);
	bool changedM = valueM.compare_exchange_strong(expectedM, kOtherString);
	REQUIRE(changedA);
	REQUIRE(changedM);
	REQUIRE(expectedA == kTestUInt32);
	REQUIRE(expectedM == kTestString);
	REQUIRE(valueA == kOtherUInt32);
	REQUIRE(valueM == kOtherString);

	changedA = valueA.compare_exchange_strong(expectedA, kTestUInt32);
	changedM = valueM.compare_exchange_strong(expectedM, kTestString);
	REQUIRE(!changedA);
	REQUIRE(!changedM);
	REQUIRE(expectedA == kOtherUInt32);
	REQUIRE(expectedM == kOtherString);
	REQUIRE(valueA == kOtherUInt32);
	REQUIRE(valueM == kOtherString);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "CompareEqual")
{


	// Perform the test
	NAtomic<uint32_t> valueA1(kTestUInt32);
	NAtomic<uint32_t> valueA2(kOtherUInt32);

	NAtomic<NString> valueM1(kTestString);
	NAtomic<NString> valueM2(kOtherString);

	REQUIRE(valueA1 == valueA1);
	REQUIRE(valueA2 == valueA2);
	REQUIRE(valueA1 != valueA2);

	REQUIRE(valueM1 == valueM1);
	REQUIRE(valueM2 == valueM2);
	REQUIRE(valueM1 != valueM2);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TAtomic, "CompareOrder")
{


	// Perform the test
	NAtomic<uint32_t> valueA1(kTestUInt32);
	NAtomic<uint32_t> valueA2(kOtherUInt32);

	NAtomic<NString> valueM1(kTestString);
	NAtomic<NString> valueM2(kOtherString);

	REQUIRE(valueA1 <= valueA2);
	REQUIRE(valueA1 < valueA2);
	REQUIRE(valueA2 >= valueA1);
	REQUIRE(valueA2 > valueA1);

	REQUIRE(valueM2 <= valueM1);
	REQUIRE(valueM2 < valueM1);
	REQUIRE(valueM1 >= valueM2);
	REQUIRE(valueM1 > valueM2);
}
