/*	NAME:
		TMathUtilities.cpp

	DESCRIPTION:
		NMathUtilities tests.

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

#include "TMathUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt64 kTestRangeSmallMin								= 100;
static const UInt64 kTestRangeSmallMax								= 101;

static const UInt64 kTestRangeLargeMin								= 100;
static const UInt64 kTestRangeLargeMax								= 120;





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

	NN_ASSERT(NMathUtilities::RotateLeft(NN_B32(11001100, 11001100, 11001100, 11001100), 1) == NN_B32(10011001, 10011001, 10011001, 10011001));
	NN_ASSERT(NMathUtilities::RotateLeft(NN_B32(11001100, 11001100, 11001100, 11001100), 2) == NN_B32(00110011, 00110011, 00110011, 00110011));

	NN_ASSERT(NMathUtilities::RotateRight(NN_B32(11001100, 11001100, 11001100, 11001100), 1) == NN_B32(01100110, 01100110, 01100110, 01100110));
	NN_ASSERT(NMathUtilities::RotateRight(NN_B32(11001100, 11001100, 11001100, 11001100), 2) == NN_B32(00110011, 00110011, 00110011, 00110011));



	// Angles
	NN_ASSERT(NMathUtilities::AreEqual(90.0, NMathUtilities::GetDegrees(NMathUtilities::GetRadians(90.0))));



	// Maths
	NN_ASSERT(NMathUtilities::AreEqual(NMathUtilities::GetSign( kNPi),  1.0));
	NN_ASSERT(NMathUtilities::AreEqual(NMathUtilities::GetSign(-kNPi), -1.0));

	NN_ASSERT(NMathUtilities::AreEqual(NMathUtilities::FastRoot(   4.0f),        2.0f));
	NN_ASSERT(NMathUtilities::AreEqual(NMathUtilities::FastInvRoot(4.0f), 1.0f / 2.0f));

	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Sec(  1.0), 1.850816f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Csc(  1.0), 1.188395f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Ctan( 1.0), 0.642093f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Asin( 1.0), 1.570796f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Acos( 1.0), 0.000000f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Asec( 1.0), 0.000000f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Acsc( 1.0), 1.570796f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Acot( 1.0), 0.785398f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Sinh( 1.0), 1.175201f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Cosh( 1.0), 1.543081f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Tanh( 1.0), 0.761594f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Sech( 1.0), 0.648054f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Csch( 1.0), 0.850918f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Coth( 1.0), 1.313035f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Asinh(1.0), 0.881374f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Acosh(1.0), 0.000000f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Atanh(0.5), 0.549306f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Asech(1.0), 0.000000f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Acsch(1.0), 0.881374f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Acoth(8.0), 0.125657f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Rint(1.0), 1.0f));
	NN_ASSERT(NMathUtilities::AreEqual((Float32) NMathUtilities::Rint(1.0), 1.0f));



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

		NN_ASSERT(valueBool == true || valueBool == false);
		NN_ASSERT(valueUInt8  >= kUInt8Min  && valueUInt8  <= kUInt8Max);
                NN_ASSERT(valueUInt16 >= kUInt16Min && valueUInt16 <= kUInt16Max);
                NN_ASSERT(valueUInt32 >= kUInt32Min && valueUInt32 <= kUInt32Max);
                NN_ASSERT(valueUInt64 >= kUInt64Min && valueUInt64 <= kUInt64Max);


		// Small range
		valueUInt8  = NMathUtilities::GetRandomUInt8(kTestRangeSmallMin, kTestRangeSmallMax);
		valueUInt16 = NMathUtilities::GetRandomUInt8(kTestRangeSmallMin, kTestRangeSmallMax);
		valueUInt32 = NMathUtilities::GetRandomUInt8(kTestRangeSmallMin, kTestRangeSmallMax);
		valueUInt64 = NMathUtilities::GetRandomUInt8(kTestRangeSmallMin, kTestRangeSmallMax);
		
		NN_ASSERT(valueUInt8  >= kTestRangeSmallMin && valueUInt8  <= kTestRangeSmallMax);
		NN_ASSERT(valueUInt16 >= kTestRangeSmallMin && valueUInt16 <= kTestRangeSmallMax);
		NN_ASSERT(valueUInt32 >= kTestRangeSmallMin && valueUInt32 <= kTestRangeSmallMax);
		NN_ASSERT(valueUInt64 >= kTestRangeSmallMin && valueUInt64 <= kTestRangeSmallMax);


		// Large range
		valueUInt8  = NMathUtilities::GetRandomUInt8(kTestRangeLargeMin, kTestRangeLargeMax);
		valueUInt16 = NMathUtilities::GetRandomUInt8(kTestRangeLargeMin, kTestRangeLargeMax);
		valueUInt32 = NMathUtilities::GetRandomUInt8(kTestRangeLargeMin, kTestRangeLargeMax);
		valueUInt64 = NMathUtilities::GetRandomUInt8(kTestRangeLargeMin, kTestRangeLargeMax);
		
		NN_ASSERT(valueUInt8  >= kTestRangeLargeMin && valueUInt8  <= kTestRangeLargeMax);
		NN_ASSERT(valueUInt16 >= kTestRangeLargeMin && valueUInt16 <= kTestRangeLargeMax);
		NN_ASSERT(valueUInt32 >= kTestRangeLargeMin && valueUInt32 <= kTestRangeLargeMax);
		NN_ASSERT(valueUInt64 >= kTestRangeLargeMin && valueUInt64 <= kTestRangeLargeMax);
		}
}




