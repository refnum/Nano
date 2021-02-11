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
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TMathUtils){};





//=============================================================================
//		Test Case
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
//		Test Case
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
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMathUtils, "NextPowerOf2")
{


	// Perform the test
	REQUIRE(NMathUtils::NextPowerOf2(uint8_t(0)) == 2);
	REQUIRE(NMathUtils::NextPowerOf2(uint8_t(2)) == 2);
	REQUIRE(NMathUtils::NextPowerOf2(uint8_t(4)) == 4);
	REQUIRE(NMathUtils::NextPowerOf2(uint8_t(8)) == 8);

	REQUIRE(NMathUtils::NextPowerOf2(uint16_t(1)) == 2);
	REQUIRE(NMathUtils::NextPowerOf2(uint16_t(3)) == 4);
	REQUIRE(NMathUtils::NextPowerOf2(uint16_t(5)) == 8);
	REQUIRE(NMathUtils::NextPowerOf2(uint16_t(13)) == 16);

	REQUIRE(NMathUtils::NextPowerOf2(uint32_t(27)) == 32);
	REQUIRE(NMathUtils::NextPowerOf2(uint32_t(33)) == 64);
	REQUIRE(NMathUtils::NextPowerOf2(uint32_t(100)) == 128);
	REQUIRE(NMathUtils::NextPowerOf2(uint32_t(200)) == 256);

	REQUIRE(NMathUtils::NextPowerOf2(uint64_t(501)) == 512);
	REQUIRE(NMathUtils::NextPowerOf2(uint64_t(1000)) == 1024);
	REQUIRE(NMathUtils::NextPowerOf2(uint64_t(2001)) == 2048);
	REQUIRE(NMathUtils::NextPowerOf2(uint64_t(3210)) == 4096);
}





//=============================================================================
//		Test Case
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





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMathUtils, "CountLeadingZeros")
{


	// Perform the test
	for (uint8_t n = 0; n < 8; n++)
	{
		uint8_t v = uint8_t(1 << (7 - n));
		REQUIRE(NMathUtils::CountLeadingZeros(v) == n);
	}

	for (uint16_t n = 0; n < 16; n++)
	{
		uint16_t v = uint16_t(1 << (15 - n));
		REQUIRE(NMathUtils::CountLeadingZeros(v) == n);
	}

	for (uint32_t n = 0; n < 32; n++)
	{
		uint32_t v = uint32_t(1 << (31 - n));
		REQUIRE(NMathUtils::CountLeadingZeros(v) == n);
	}

	for (uint64_t n = 0; n < 64; n++)
	{
		uint64_t v = uint64_t(1ull << (63 - n));
		REQUIRE(NMathUtils::CountLeadingZeros(v) == n);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMathUtils, "CountTrailingZeros")
{


	// Perform the test
	for (uint8_t n = 0; n < 8; n++)
	{
		uint8_t v = uint8_t(1 << n);
		REQUIRE(NMathUtils::CountTrailingZeros(v) == n);
	}

	for (uint16_t n = 0; n < 16; n++)
	{
		uint16_t v = uint16_t(1 << n);
		REQUIRE(NMathUtils::CountTrailingZeros(v) == n);
	}

	for (uint32_t n = 0; n < 32; n++)
	{
		uint32_t v = uint32_t(1 << n);
		REQUIRE(NMathUtils::CountTrailingZeros(v) == n);
	}

	for (uint64_t n = 0; n < 64; n++)
	{
		uint64_t v = uint64_t(1ull << n);
		REQUIRE(NMathUtils::CountTrailingZeros(v) == n);
	}
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMathUtils, "RotateLeft")
{


	// Perform the test
	REQUIRE(NMathUtils::RotateLeft(uint8_t(0b11001100), 1) == uint8_t(0b10011001));
	REQUIRE(NMathUtils::RotateLeft(uint8_t(0b11001100), 2) == uint8_t(0b00110011));

	REQUIRE(NMathUtils::RotateLeft(uint16_t(0b1100110011001100), 1) ==
			uint16_t(0b1001100110011001));
	REQUIRE(NMathUtils::RotateLeft(uint16_t(0b1100110011001100), 2) ==
			uint16_t(0b0011001100110011));

	REQUIRE(NMathUtils::RotateLeft(uint32_t(0b11001100110011001100110011001100), 1) ==
			uint32_t(0b10011001100110011001100110011001));
	REQUIRE(NMathUtils::RotateLeft(uint32_t(0b11001100110011001100110011001100), 2) ==
			uint32_t(0b00110011001100110011001100110011));

	REQUIRE(NMathUtils::RotateLeft(
				uint64_t(0b1100110011001100110011001100110011001100110011001100110011001100),
				1) == uint64_t(0b1001100110011001100110011001100110011001100110011001100110011001));
	REQUIRE(NMathUtils::RotateLeft(
				uint64_t(0b1100110011001100110011001100110011001100110011001100110011001100),
				2) == uint64_t(0b0011001100110011001100110011001100110011001100110011001100110011));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMathUtils, "RotateRight")
{


	// Perform the test
	REQUIRE(NMathUtils::RotateRight(uint8_t(0b11001100), 1) == uint8_t(0b01100110));
	REQUIRE(NMathUtils::RotateRight(uint8_t(0b11001100), 2) == uint8_t(0b00110011));

	REQUIRE(NMathUtils::RotateRight(uint16_t(0b1100110011001100), 1) ==
			uint16_t(0b0110011001100110));
	REQUIRE(NMathUtils::RotateRight(uint16_t(0b1100110011001100), 2) ==
			uint16_t(0b0011001100110011));

	REQUIRE(NMathUtils::RotateRight(uint32_t(0b11001100110011001100110011001100), 1) ==
			uint32_t(0b01100110011001100110011001100110));
	REQUIRE(NMathUtils::RotateRight(uint32_t(0b11001100110011001100110011001100), 2) ==
			uint32_t(0b00110011001100110011001100110011));

	REQUIRE(NMathUtils::RotateRight(
				uint64_t(0b1100110011001100110011001100110011001100110011001100110011001100),
				1) == uint64_t(0b0110011001100110011001100110011001100110011001100110011001100110));
	REQUIRE(NMathUtils::RotateRight(
				uint64_t(0b1100110011001100110011001100110011001100110011001100110011001100),
				2) == uint64_t(0b0011001100110011001100110011001100110011001100110011001100110011));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TMathUtils, "Degrees")
{


	// Perform the test
	REQUIRE(NMathUtils::ToDegrees(NMathUtils::ToRadians(90.0)) == 90.0);
}
