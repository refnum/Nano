/*	NAME:
		TByteSwap.cpp

	DESCRIPTION:
		NByteSwap tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NByteSwap.h"

#include "TByteSwap.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const UInt16 kValue16									= 0x1234;
static const UInt32 kValue32									= 0x12345678;
static const UInt64 kValue64									= 0x123456789ABCDEF0ULL;

static const UInt16 kSwapped16									= 0x3412;
static const UInt32 kSwapped32									= 0x78563412;
static const UInt64 kSwapped64									= 0xF0DEBC9A78563412ULL;

static const UInt8 kFloat32L[4]									= { 0xDB, 0x0F, 0x49, 0x40 };
static const UInt8 kFloat64L[8]									= { 0x18, 0x2D, 0x44, 0x54, 0xFB, 0x21, 0x09, 0x40 };

static const UInt8 kFloat32B[4]									= { 0x40, 0x49, 0x0F, 0xDB };
static const UInt8 kFloat64B[8]									= { 0x40, 0x09, 0x21, 0xFB,	0x54, 0x44,	0x2D, 0x18 };





//============================================================================
//		TByteSwap::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TByteSwap::Execute(void)
{	Float32		float32N, float32B, float32L;
	Float64		float64N, float64B, float64L;
	UInt16		value16;
	UInt32		value32;
	UInt64		value64;



	// Get the state we need
	memcpy(&float32N, NN_TARGET_ENDIAN_BIG ? &kFloat32B : &kFloat32L, sizeof(Float32));
	memcpy(&float64N, NN_TARGET_ENDIAN_BIG ? &kFloat64B : &kFloat64L, sizeof(Float64));

	memcpy(&float32B, &kFloat32B, sizeof(Float32));
	memcpy(&float64B, &kFloat64B, sizeof(Float64));

	memcpy(&float32L, &kFloat32L, sizeof(Float32));
	memcpy(&float64L, &kFloat64L, sizeof(Float64));



	// Constants
	if (NN_TARGET_ENDIAN_BIG)
		NN_ASSERT(kNEndianNative == kNEndianBig);
	else
		NN_ASSERT(kNEndianNative == kNEndianLittle);



	// Primitives
	value16 = kValue16;
	value32 = kValue32;
	value64 = kValue64;
	
	NByteSwap::SwapInt16(&value16);
	NByteSwap::SwapInt32(&value32);
	NByteSwap::SwapInt64(&value64);

	NN_ASSERT(value16 == kSwapped16);
	NN_ASSERT(value32 == kSwapped32);
	NN_ASSERT(value64 == kSwapped64);



	// Native to big
	NN_ASSERT(NSwapUInt16_NtoB(kValue16) == (UInt16) (NN_TARGET_ENDIAN_BIG ? kValue16 : kSwapped16));
	NN_ASSERT(NSwapUInt32_NtoB(kValue32) == (UInt32) (NN_TARGET_ENDIAN_BIG ? kValue32 : kSwapped32));
	NN_ASSERT(NSwapUInt64_NtoB(kValue64) == (UInt64) (NN_TARGET_ENDIAN_BIG ? kValue64 : kSwapped64));

	NN_ASSERT(NSwapSInt16_NtoB(kValue16) == (SInt16) (NN_TARGET_ENDIAN_BIG ? kValue16 : kSwapped16));
	NN_ASSERT(NSwapSInt32_NtoB(kValue32) == (SInt32) (NN_TARGET_ENDIAN_BIG ? kValue32 : kSwapped32));
	NN_ASSERT(NSwapSInt64_NtoB(kValue64) == (SInt64) (NN_TARGET_ENDIAN_BIG ? kValue64 : kSwapped64));

	NN_ASSERT(NMathUtilities::AreEqual(NSwapFloat32_NtoB(float32N), float32B));
	NN_ASSERT(NMathUtilities::AreEqual(NSwapFloat64_NtoB(float64N), float64B));



	// Native to little
	NN_ASSERT(NSwapUInt16_NtoL(kValue16) == (UInt16) (NN_TARGET_ENDIAN_LITTLE ? kValue16 : kSwapped16));
	NN_ASSERT(NSwapUInt32_NtoL(kValue32) == (UInt32) (NN_TARGET_ENDIAN_LITTLE ? kValue32 : kSwapped32));
	NN_ASSERT(NSwapUInt64_NtoL(kValue64) == (UInt64) (NN_TARGET_ENDIAN_LITTLE ? kValue64 : kSwapped64));

	NN_ASSERT(NSwapSInt16_NtoL(kValue16) == (SInt16) (NN_TARGET_ENDIAN_LITTLE ? kValue16 : kSwapped16));
	NN_ASSERT(NSwapSInt32_NtoL(kValue32) == (SInt32) (NN_TARGET_ENDIAN_LITTLE ? kValue32 : kSwapped32));
	NN_ASSERT(NSwapSInt64_NtoL(kValue64) == (SInt64) (NN_TARGET_ENDIAN_LITTLE ? kValue64 : kSwapped64));

	NN_ASSERT(NMathUtilities::AreEqual(NSwapFloat32_NtoL(float32N), float32L));
	NN_ASSERT(NMathUtilities::AreEqual(NSwapFloat64_NtoL(float64N), float64L));



	// Big to native
	NN_ASSERT(NSwapUInt16_BtoN(kValue16) == (UInt16) (NN_TARGET_ENDIAN_BIG ? kValue16 : kSwapped16));
	NN_ASSERT(NSwapUInt32_BtoN(kValue32) == (UInt32) (NN_TARGET_ENDIAN_BIG ? kValue32 : kSwapped32));
	NN_ASSERT(NSwapUInt64_BtoN(kValue64) == (UInt64) (NN_TARGET_ENDIAN_BIG ? kValue64 : kSwapped64));

	NN_ASSERT(NSwapSInt16_BtoN(kValue16) == (SInt16) (NN_TARGET_ENDIAN_BIG ? kValue16 : kSwapped16));
	NN_ASSERT(NSwapSInt32_BtoN(kValue32) == (SInt32) (NN_TARGET_ENDIAN_BIG ? kValue32 : kSwapped32));
	NN_ASSERT(NSwapSInt64_BtoN(kValue64) == (SInt64) (NN_TARGET_ENDIAN_BIG ? kValue64 : kSwapped64));

	NN_ASSERT(NMathUtilities::AreEqual(NSwapFloat32_BtoN(float32B), float32N));
	NN_ASSERT(NMathUtilities::AreEqual(NSwapFloat64_BtoN(float64B), float64N));



	// Little to native
	NN_ASSERT(NSwapUInt16_LtoN(kValue16) == (UInt16) (NN_TARGET_ENDIAN_LITTLE ? kValue16 : kSwapped16));
	NN_ASSERT(NSwapUInt32_LtoN(kValue32) == (UInt32) (NN_TARGET_ENDIAN_LITTLE ? kValue32 : kSwapped32));
	NN_ASSERT(NSwapUInt64_LtoN(kValue64) == (UInt64) (NN_TARGET_ENDIAN_LITTLE ? kValue64 : kSwapped64));

	NN_ASSERT(NSwapSInt16_LtoN(kValue16) == (SInt16) (NN_TARGET_ENDIAN_LITTLE ? kValue16 : kSwapped16));
	NN_ASSERT(NSwapSInt32_LtoN(kValue32) == (SInt32) (NN_TARGET_ENDIAN_LITTLE ? kValue32 : kSwapped32));
	NN_ASSERT(NSwapSInt64_LtoN(kValue64) == (SInt64) (NN_TARGET_ENDIAN_LITTLE ? kValue64 : kSwapped64));

	NN_ASSERT(NMathUtilities::AreEqual(NSwapFloat32_LtoN(float32L), float32N));
	NN_ASSERT(NMathUtilities::AreEqual(NSwapFloat64_LtoN(float64L), float64N));
}




