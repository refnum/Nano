/*	NAME:
		TByteSwap.cpp

	DESCRIPTION:
		NByteSwap tests.

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
#include "NByteSwap.h"
#include "NTestFixture.h"

// System
#include <string.h>





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
static const uint16_t kNativeUInt16                         = 0x1234;
static const uint32_t kNativeUInt32                         = 0x12345678;
static const uint64_t kNativeUInt64                         = 0x123456789ABCDEF0ULL;

static const uint16_t kSwappedUInt16                        = 0x3412;
static const uint32_t kSwappedUInt32                        = 0x78563412;
static const uint64_t kSwappedUInt64                        = 0xF0DEBC9A78563412ULL;

static const uint8_t kLittleFloat32[4]                      = {0xDB, 0x0F, 0x49, 0x40};
static const uint8_t kLittleFloat64[8]                      = {0x18, 0x2D, 0x44, 0x54, 0xFB, 0x21, 0x09, 0x40};

static const uint8_t kBigFloat32[4]                         = {0x40, 0x49, 0x0F, 0xDB};
static const uint8_t kBigFloat64[8]                         = {0x40, 0x09, 0x21, 0xFB, 0x54, 0x44, 0x2D, 0x18};





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TByteSwap)
{
	float32_t float32N, float32B, float32L;
	float64_t float64N, float64B, float64L;

	SETUP
	{
		memcpy(&float32N, NN_ENDIAN_BIG ? &kBigFloat32 : &kLittleFloat32, sizeof(float32_t));
		memcpy(&float64N, NN_ENDIAN_BIG ? &kBigFloat64 : &kLittleFloat64, sizeof(float64_t));

		memcpy(&float32B, &kBigFloat32, sizeof(float32_t));
		memcpy(&float64B, &kBigFloat64, sizeof(float64_t));

		memcpy(&float32L, &kLittleFloat32, sizeof(float32_t));
		memcpy(&float64L, &kLittleFloat64, sizeof(float64_t));
	}
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TByteSwap, "Constants")
{


	// Perform the test
	if (NN_ENDIAN_BIG)
	{
		REQUIRE(NEndian::Native == NEndian::Big);
	}
	else
	{
		REQUIRE(NEndian::Native == NEndian::Little);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TByteSwap, "Primitives")
{


	// Perform the test
	REQUIRE(NByteSwap::Swap(kNativeUInt16) == kSwappedUInt16);
	REQUIRE(NByteSwap::Swap(kNativeUInt32) == kSwappedUInt32);
	REQUIRE(NByteSwap::Swap(kNativeUInt64) == kSwappedUInt64);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TByteSwap, "NtoB")
{


	// Perform the test
	REQUIRE(NByteSwap::SwapNtoB(kNativeUInt16) == (NN_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NByteSwap::SwapNtoB(kNativeUInt32) == (NN_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NByteSwap::SwapNtoB(kNativeUInt64) == (NN_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NByteSwap::SwapNtoB(kNativeUInt16) == (NN_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NByteSwap::SwapNtoB(kNativeUInt32) == (NN_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NByteSwap::SwapNtoB(kNativeUInt64) == (NN_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	float32_t float32 = NByteSwap::SwapNtoB(float32N);
	REQUIRE(memcmp(&float32, &float32B, sizeof(float32)) == 0);

	float64_t float64 = NByteSwap::SwapNtoB(float64N);
	REQUIRE(memcmp(&float64, &float64B, sizeof(float64)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TByteSwap, "NtoL")
{


	// Perform the test
	REQUIRE(NByteSwap::SwapNtoL(kNativeUInt16) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NByteSwap::SwapNtoL(kNativeUInt32) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NByteSwap::SwapNtoL(kNativeUInt64) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NByteSwap::SwapNtoL(kNativeUInt16) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NByteSwap::SwapNtoL(kNativeUInt32) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NByteSwap::SwapNtoL(kNativeUInt64) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	float32_t float32 = NByteSwap::SwapNtoL(float32N);
	REQUIRE(memcmp(&float32, &float32L, sizeof(float32)) == 0);

	float64_t float64 = NByteSwap::SwapNtoL(float64N);
	REQUIRE(memcmp(&float64, &float64L, sizeof(float64)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TByteSwap, "BtoN")
{


	// Perform the test
	REQUIRE(NByteSwap::SwapBtoN(kNativeUInt16) == (NN_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NByteSwap::SwapBtoN(kNativeUInt32) == (NN_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NByteSwap::SwapBtoN(kNativeUInt64) == (NN_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NByteSwap::SwapBtoN(kNativeUInt16) == (NN_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NByteSwap::SwapBtoN(kNativeUInt32) == (NN_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NByteSwap::SwapBtoN(kNativeUInt64) == (NN_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	float32_t float32 = NByteSwap::SwapBtoN(float32B);
	REQUIRE(memcmp(&float32, &float32N, sizeof(float32)) == 0);

	float64_t float64 = NByteSwap::SwapBtoN(float64B);
	REQUIRE(memcmp(&float64, &float64N, sizeof(float64)) == 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TByteSwap, "LtoN")
{


	// Perform the test
	REQUIRE(NByteSwap::SwapLtoN(kNativeUInt16) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NByteSwap::SwapLtoN(kNativeUInt32) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NByteSwap::SwapLtoN(kNativeUInt64) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NByteSwap::SwapLtoN(kNativeUInt16) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NByteSwap::SwapLtoN(kNativeUInt32) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NByteSwap::SwapLtoN(kNativeUInt64) ==
			(NN_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	float32_t float32 = NByteSwap::SwapLtoN(float32L);
	REQUIRE(memcmp(&float32, &float32N, sizeof(float32)) == 0);

	float64_t float64 = NByteSwap::SwapLtoN(float64L);
	REQUIRE(memcmp(&float64, &float64N, sizeof(float64)) == 0);
}
