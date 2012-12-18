/*	NAME:
		TMathUtilities.cpp

	DESCRIPTION:
		NMathUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"

#include "TMathUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt64 kValueRangeSmallMin							= 100;
static const UInt64 kValueRangeSmallMax							= 101;

static const UInt64 kValueRangeLargeMin							= 100;
static const UInt64 kValueRangeLargeMax							= 120;





//============================================================================
//		TMathUtilities::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TMathUtilities::Execute(void)
{	UInt64		valueUInt64;
	UInt32		valueUInt32;
	UInt16		valueUInt16;
	UInt8		valueUInt8;
	bool		valueBool;
	NIndex		n;



	// Floating point
	NN_ASSERT(NMathUtilities::AreEqual(1.2345f, 1.2345f));
	NN_ASSERT(NMathUtilities::AreEqual(1.0 + 1.0, 3.0 - 1.0));

	NN_ASSERT( NMathUtilities::IsZero(0.0f));
	NN_ASSERT( NMathUtilities::IsZero(0.0));
	NN_ASSERT(!NMathUtilities::IsZero(1.0f));
	NN_ASSERT(!NMathUtilities::IsZero(1.0));

	NN_ASSERT(!NMathUtilities::IsNan(kNPi));
	NN_ASSERT(!NMathUtilities::IsInf(kNPi));



	// Integer
	NN_ASSERT( NMathUtilities::IsOdd(1));
	NN_ASSERT(!NMathUtilities::IsOdd(2));

	NN_ASSERT(!NMathUtilities::IsEven(1));
	NN_ASSERT( NMathUtilities::IsEven(2));

	NN_ASSERT(!NMathUtilities::IsPowerOf2(0));
	NN_ASSERT( NMathUtilities::IsPowerOf2(1));
	NN_ASSERT( NMathUtilities::IsPowerOf2(2));
	NN_ASSERT(!NMathUtilities::IsPowerOf2(3));
	NN_ASSERT( NMathUtilities::IsPowerOf2(4));
	NN_ASSERT(!NMathUtilities::IsPowerOf2(5));
	NN_ASSERT(!NMathUtilities::IsPowerOf2(6));
	NN_ASSERT(!NMathUtilities::IsPowerOf2(7));
	NN_ASSERT( NMathUtilities::IsPowerOf2(8));
	NN_ASSERT(!NMathUtilities::IsPowerOf2(9));
	NN_ASSERT(!NMathUtilities::IsPowerOf2(10));
	NN_ASSERT(!NMathUtilities::IsPowerOf2(11));
	NN_ASSERT(!NMathUtilities::IsPowerOf2(12));



	// Bits
	NN_ASSERT(NMathUtilities::CountBits(0) == 0);
	NN_ASSERT(NMathUtilities::CountBits(1) == 1);
	NN_ASSERT(NMathUtilities::CountBits(2) == 1);
	NN_ASSERT(NMathUtilities::CountBits(3) == 2);
	NN_ASSERT(NMathUtilities::CountBits(4) == 1);
	NN_ASSERT(NMathUtilities::CountBits(5) == 2);
	NN_ASSERT(NMathUtilities::CountBits(6) == 2);
	NN_ASSERT(NMathUtilities::CountBits(7) == 3);

	NN_ASSERT(NMathUtilities::RotateLeft(1024, 1) == 2048);
	NN_ASSERT(NMathUtilities::RotateLeft(1024, 2) == 4096);

	NN_ASSERT(NMathUtilities::RotateRight(4096, 1) == 2048);
	NN_ASSERT(NMathUtilities::RotateRight(4096, 2) == 1024);



	// Angles
	NN_ASSERT(NMathUtilities::AreEqual(90.0, NMathUtilities::GetDegrees(NMathUtilities::GetRadians(90.0))));



	// Maths
	NN_ASSERT(NMathUtilities::AreEqual(NMathUtilities::GetSign( kNPi),  1.0));
	NN_ASSERT(NMathUtilities::AreEqual(NMathUtilities::GetSign(-kNPi), -1.0));

	NN_ASSERT(NMathUtilities::AreEqual(NMathUtilities::FastRoot(   4.0f),        2.0f));
	NN_ASSERT(NMathUtilities::AreEqual(NMathUtilities::FastInvRoot(4.0f), 1.0f / 2.0f));



	// Random
	NN_ASSERT(NMathUtilities::SetRandomSeed(0) != 0);
	
	for (n = 0; n < 1000; n++)
		{
		// Open range
		valueBool   = NMathUtilities::GetRandomBoolean();
		valueUInt8  = NMathUtilities::GetRandomUInt8();
		valueUInt16 = NMathUtilities::GetRandomUInt16();
		valueUInt32 = NMathUtilities::GetRandomUInt32();
		valueUInt64 = NMathUtilities::GetRandomUInt64();


		// Small range
		valueUInt8  = NMathUtilities::GetRandomUInt8(kValueRangeSmallMin, kValueRangeSmallMax);
		valueUInt16 = NMathUtilities::GetRandomUInt8(kValueRangeSmallMin, kValueRangeSmallMax);
		valueUInt32 = NMathUtilities::GetRandomUInt8(kValueRangeSmallMin, kValueRangeSmallMax);
		valueUInt64 = NMathUtilities::GetRandomUInt8(kValueRangeSmallMin, kValueRangeSmallMax);
		
		NN_ASSERT(valueUInt8  >= kValueRangeSmallMin && valueUInt8  <= kValueRangeSmallMax);
		NN_ASSERT(valueUInt16 >= kValueRangeSmallMin && valueUInt16 <= kValueRangeSmallMax);
		NN_ASSERT(valueUInt32 >= kValueRangeSmallMin && valueUInt32 <= kValueRangeSmallMax);
		NN_ASSERT(valueUInt64 >= kValueRangeSmallMin && valueUInt64 <= kValueRangeSmallMax);


		// Large range
		valueUInt8  = NMathUtilities::GetRandomUInt8(kValueRangeLargeMin, kValueRangeLargeMax);
		valueUInt16 = NMathUtilities::GetRandomUInt8(kValueRangeLargeMin, kValueRangeLargeMax);
		valueUInt32 = NMathUtilities::GetRandomUInt8(kValueRangeLargeMin, kValueRangeLargeMax);
		valueUInt64 = NMathUtilities::GetRandomUInt8(kValueRangeLargeMin, kValueRangeLargeMax);
		
		NN_ASSERT(valueUInt8  >= kValueRangeLargeMin && valueUInt8  <= kValueRangeLargeMax);
		NN_ASSERT(valueUInt16 >= kValueRangeLargeMin && valueUInt16 <= kValueRangeLargeMax);
		NN_ASSERT(valueUInt32 >= kValueRangeLargeMin && valueUInt32 <= kValueRangeLargeMax);
		NN_ASSERT(valueUInt64 >= kValueRangeLargeMin && valueUInt64 <= kValueRangeLargeMax);
		}
}




