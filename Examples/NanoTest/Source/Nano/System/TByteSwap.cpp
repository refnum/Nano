/*	NAME:
		TByteSwap.cpp

	DESCRIPTION:
		NByteSwap tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NTestFixture.h"
#include "NByteSwap.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const uint16_t kNativeUInt16									= 0x1234;
static const uint32_t kNativeUInt32									= 0x12345678;
static const uint64_t kNativeUInt64									= 0x123456789ABCDEF0ULL;

static const uint16_t kSwappedUInt16								= 0x3412;
static const uint32_t kSwappedUInt32								= 0x78563412;
static const uint64_t kSwappedUInt64								= 0xF0DEBC9A78563412ULL;

static const uint8_t kLittleFloat32[4]								= { 0xDB, 0x0F, 0x49, 0x40 };
static const uint8_t kLittleFloat64[8]								= { 0x18, 0x2D, 0x44, 0x54, 0xFB, 0x21, 0x09, 0x40 };

static const uint8_t kBigFloat32[4]									= { 0x40, 0x49, 0x0F, 0xDB };
static const uint8_t kBigFloat64[8]									= { 0x40, 0x09, 0x21, 0xFB,	0x54, 0x44,	0x2D, 0x18 };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NBYTESWAP(...)											TEST_NANO(TByteSwap, ##__VA_ARGS__)

FIXTURE_NANO(TByteSwap)
{
	float32_t		float32N, float32B, float32L;
	float64_t		float64N, float64B, float64L;
	uint16_t		value16;
	uint32_t		value32;
	uint64_t		value64;

	SETUP
	{
		memcpy(&float32N, NN_TARGET_ENDIAN_BIG ? &kBigFloat32 : &kLittleFloat32, sizeof(float32_t));
		memcpy(&float64N, NN_TARGET_ENDIAN_BIG ? &kBigFloat64 : &kLittleFloat64, sizeof(float64_t));

		memcpy(&float32B, &kBigFloat32, sizeof(float32_t));
		memcpy(&float64B, &kBigFloat64, sizeof(float64_t));

		memcpy(&float32L, &kLittleFloat32, sizeof(float32_t));
		memcpy(&float64L, &kLittleFloat64, sizeof(float64_t));
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("Constants")
{


	// Perform the test
	if (NN_TARGET_ENDIAN_BIG)
		REQUIRE(kNEndianNative == kNEndianBig);
	else
		REQUIRE(kNEndianNative == kNEndianLittle);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("Primitives")
{


	// Perform the test
	value16 = kNativeUInt16;
	value32 = kNativeUInt32;
	value64 = kNativeUInt64;
	
	NByteSwap::Swap16(&value16);
	NByteSwap::Swap32(&value32);
	NByteSwap::Swap64(&value64);

	REQUIRE(value16 == kSwappedUInt16);
	REQUIRE(value32 == kSwappedUInt32);
	REQUIRE(value64 == kSwappedUInt64);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("NtoB")
{


	// Perform the test
	REQUIRE(NSwapUInt16_NtoB(kNativeUInt16) == (uint16_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapUInt32_NtoB(kNativeUInt32) == (uint32_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapUInt64_NtoB(kNativeUInt64) == (uint64_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NSwapInt16_NtoB(kNativeUInt16) == (int16_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapInt32_NtoB(kNativeUInt32) == (int32_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapInt64_NtoB(kNativeUInt64) == (int64_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NMathUtilities::AreEqual(NSwapFloat32_NtoB(float32N), float32B));
	REQUIRE(NMathUtilities::AreEqual(NSwapFloat64_NtoB(float64N), float64B));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("NtoL")
{


	// Perform the test
	REQUIRE(NSwapUInt16_NtoL(kNativeUInt16) == (uint16_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapUInt32_NtoL(kNativeUInt32) == (uint32_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapUInt64_NtoL(kNativeUInt64) == (uint64_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NSwapInt16_NtoL(kNativeUInt16) == (int16_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapInt32_NtoL(kNativeUInt32) == (int32_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapInt64_NtoL(kNativeUInt64) == (int64_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NMathUtilities::AreEqual(NSwapFloat32_NtoL(float32N), float32L));
	REQUIRE(NMathUtilities::AreEqual(NSwapFloat64_NtoL(float64N), float64L));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("BtoN")
{


	// Perform the test
	REQUIRE(NSwapUInt16_BtoN(kNativeUInt16) == (uint16_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapUInt32_BtoN(kNativeUInt32) == (uint32_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapUInt64_BtoN(kNativeUInt64) == (uint64_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NSwapInt16_BtoN(kNativeUInt16) == (int16_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapInt32_BtoN(kNativeUInt32) == (int32_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapInt64_BtoN(kNativeUInt64) == (int64_t) (NN_TARGET_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NMathUtilities::AreEqual(NSwapFloat32_BtoN(float32B), float32N));
	REQUIRE(NMathUtilities::AreEqual(NSwapFloat64_BtoN(float64B), float64N));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("LtoN")
{


	// Perform the test
	REQUIRE(NSwapUInt16_LtoN(kNativeUInt16) == (uint16_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapUInt32_LtoN(kNativeUInt32) == (uint32_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapUInt64_LtoN(kNativeUInt64) == (uint64_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NSwapInt16_LtoN(kNativeUInt16) == (int16_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapInt32_LtoN(kNativeUInt32) == (int32_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapInt64_LtoN(kNativeUInt64) == (int64_t) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NMathUtilities::AreEqual(NSwapFloat32_LtoN(float32L), float32N));
	REQUIRE(NMathUtilities::AreEqual(NSwapFloat64_LtoN(float64L), float64N));
}






