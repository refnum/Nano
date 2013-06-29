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
static const UInt16 kNativeUInt16									= 0x1234;
static const UInt32 kNativeUInt32									= 0x12345678;
static const UInt64 kNativeUInt64									= 0x123456789ABCDEF0ULL;

static const UInt16 kSwappedUInt16									= 0x3412;
static const UInt32 kSwappedUInt32									= 0x78563412;
static const UInt64 kSwappedUInt64									= 0xF0DEBC9A78563412ULL;

static const UInt8 kLittleFloat32[4]								= { 0xDB, 0x0F, 0x49, 0x40 };
static const UInt8 kLittleFloat64[8]								= { 0x18, 0x2D, 0x44, 0x54, 0xFB, 0x21, 0x09, 0x40 };

static const UInt8 kBigFloat32[4]									= { 0x40, 0x49, 0x0F, 0xDB };
static const UInt8 kBigFloat64[8]									= { 0x40, 0x09, 0x21, 0xFB,	0x54, 0x44,	0x2D, 0x18 };





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NBYTESWAP(_name, _desc)								TEST_NANO(TByteSwap, _name, _desc)

FIXTURE_NANO(TByteSwap)
{
	Float32		float32N, float32B, float32L;
	Float64		float64N, float64B, float64L;
	UInt16		value16;
	UInt32		value32;
	UInt64		value64;

	SETUP
	{
		memcpy(&float32N, NN_TARGET_ENDIAN_BIG ? &kBigFloat32 : &kLittleFloat32, sizeof(Float32));
		memcpy(&float64N, NN_TARGET_ENDIAN_BIG ? &kBigFloat64 : &kLittleFloat64, sizeof(Float64));

		memcpy(&float32B, &kBigFloat32, sizeof(Float32));
		memcpy(&float64B, &kBigFloat64, sizeof(Float64));

		memcpy(&float32L, &kLittleFloat32, sizeof(Float32));
		memcpy(&float64L, &kLittleFloat64, sizeof(Float64));
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("Constants", "Constants")
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
TEST_NBYTESWAP("Primitives", "Primitives")
{


	// Perform the test
	value16 = kNativeUInt16;
	value32 = kNativeUInt32;
	value64 = kNativeUInt64;
	
	NByteSwap::SwapInt16(&value16);
	NByteSwap::SwapInt32(&value32);
	NByteSwap::SwapInt64(&value64);

	REQUIRE(value16 == kSwappedUInt16);
	REQUIRE(value32 == kSwappedUInt32);
	REQUIRE(value64 == kSwappedUInt64);
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("NtoB", "Native to Big")
{


	// Perform the test
	REQUIRE(NSwapUInt16_NtoB(kNativeUInt16) == (UInt16) (NN_TARGET_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapUInt32_NtoB(kNativeUInt32) == (UInt32) (NN_TARGET_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapUInt64_NtoB(kNativeUInt64) == (UInt64) (NN_TARGET_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NSwapSInt16_NtoB(kNativeUInt16) == (SInt16) (NN_TARGET_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapSInt32_NtoB(kNativeUInt32) == (SInt32) (NN_TARGET_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapSInt64_NtoB(kNativeUInt64) == (SInt64) (NN_TARGET_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NMathUtilities::AreEqual(NSwapFloat32_NtoB(float32N), float32B));
	REQUIRE(NMathUtilities::AreEqual(NSwapFloat64_NtoB(float64N), float64B));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("NtoL", "Native to Little")
{


	// Perform the test
	REQUIRE(NSwapUInt16_NtoL(kNativeUInt16) == (UInt16) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapUInt32_NtoL(kNativeUInt32) == (UInt32) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapUInt64_NtoL(kNativeUInt64) == (UInt64) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NSwapSInt16_NtoL(kNativeUInt16) == (SInt16) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapSInt32_NtoL(kNativeUInt32) == (SInt32) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapSInt64_NtoL(kNativeUInt64) == (SInt64) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NMathUtilities::AreEqual(NSwapFloat32_NtoL(float32N), float32L));
	REQUIRE(NMathUtilities::AreEqual(NSwapFloat64_NtoL(float64N), float64L));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("BtoN", "Big to Native")
{


	// Perform the test
	REQUIRE(NSwapUInt16_BtoN(kNativeUInt16) == (UInt16) (NN_TARGET_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapUInt32_BtoN(kNativeUInt32) == (UInt32) (NN_TARGET_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapUInt64_BtoN(kNativeUInt64) == (UInt64) (NN_TARGET_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NSwapSInt16_BtoN(kNativeUInt16) == (SInt16) (NN_TARGET_ENDIAN_BIG ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapSInt32_BtoN(kNativeUInt32) == (SInt32) (NN_TARGET_ENDIAN_BIG ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapSInt64_BtoN(kNativeUInt64) == (SInt64) (NN_TARGET_ENDIAN_BIG ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NMathUtilities::AreEqual(NSwapFloat32_BtoN(float32B), float32N));
	REQUIRE(NMathUtilities::AreEqual(NSwapFloat64_BtoN(float64B), float64N));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NBYTESWAP("LtoN", "Little to Native")
{


	// Perform the test
	REQUIRE(NSwapUInt16_LtoN(kNativeUInt16) == (UInt16) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapUInt32_LtoN(kNativeUInt32) == (UInt32) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapUInt64_LtoN(kNativeUInt64) == (UInt64) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NSwapSInt16_LtoN(kNativeUInt16) == (SInt16) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt16 : kSwappedUInt16));
	REQUIRE(NSwapSInt32_LtoN(kNativeUInt32) == (SInt32) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt32 : kSwappedUInt32));
	REQUIRE(NSwapSInt64_LtoN(kNativeUInt64) == (SInt64) (NN_TARGET_ENDIAN_LITTLE ? kNativeUInt64 : kSwappedUInt64));

	REQUIRE(NMathUtilities::AreEqual(NSwapFloat32_LtoN(float32L), float32N));
	REQUIRE(NMathUtilities::AreEqual(NSwapFloat64_LtoN(float64L), float64N));
}






