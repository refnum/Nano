/*	NAME:
		TMathUtils.cpp

	DESCRIPTION:
		NMathUtils tests.

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
#include "NMathUtils.h"
#include "NTestFixture.h"





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TMathUtils){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TMathUtils, "Parity")
{


	// Perform the test
	REQUIRE(NMathUtils::IsOdd(1));
	REQUIRE(!NMathUtils::IsOdd(2));

	REQUIRE(!NMathUtils::IsEven(1));
	REQUIRE(NMathUtils::IsEven(2));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TMathUtils, "IsPowerOf2")
{


	// Perform the test
	REQUIRE(!NMathUtils::IsPowerOf2(0));
	REQUIRE(NMathUtils::IsPowerOf2(1));
	REQUIRE(NMathUtils::IsPowerOf2(2));
	REQUIRE(!NMathUtils::IsPowerOf2(3));
	REQUIRE(NMathUtils::IsPowerOf2(4));
	REQUIRE(!NMathUtils::IsPowerOf2(5));
	REQUIRE(!NMathUtils::IsPowerOf2(6));
	REQUIRE(!NMathUtils::IsPowerOf2(7));
	REQUIRE(NMathUtils::IsPowerOf2(8));
	REQUIRE(!NMathUtils::IsPowerOf2(9));
	REQUIRE(!NMathUtils::IsPowerOf2(10));
	REQUIRE(!NMathUtils::IsPowerOf2(11));
	REQUIRE(!NMathUtils::IsPowerOf2(12));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TMathUtils, "CountBits")
{


	// Perform the test
	for (uint8_t v = 0, n = 0; n < 8; n++)
	{
		REQUIRE(NMathUtils::CountBits(v) == n);
		v = uint8_t((v << 1) | 1);
	}

	for (uint16_t v = 0, n = 0; n < 16; n++)
	{
		REQUIRE(NMathUtils::CountBits(v) == n);
		v = uint16_t((v << 1) | 1);
	}

	for (uint32_t v = 0, n = 0; n < 32; n++)
	{
		REQUIRE(NMathUtils::CountBits(v) == n);
		v = uint32_t((v << 1) | 1);
	}

	for (uint64_t v = 0, n = 0; n < 64; n++)
	{
		REQUIRE(NMathUtils::CountBits(v) == n);
		v = uint64_t((v << 1) | 1);
	}
}
