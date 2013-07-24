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
#include "NTestFixture.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const uint64_t kTestRangeSmallMin							= 100;
static const uint64_t kTestRangeSmallMax							= 101;

static const uint64_t kTestRangeLargeMin							= 100;
static const uint64_t kTestRangeLargeMax							= 120;





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NMATHUTILITIES(...)									TEST_NANO(TMathUtilities, ##__VA_ARGS__)

FIXTURE_NANO(TMathUtilities)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NMATHUTILITIES("Float")
{


	// Perform the test
	REQUIRE(NMathUtilities::AreEqual(1.2345f, 1.2345f));
	REQUIRE(NMathUtilities::AreEqual(1.0 + 1.0, 3.0 - 1.0));

	REQUIRE( NMathUtilities::IsZero(0.0f));
	REQUIRE( NMathUtilities::IsZero(0.0));
	REQUIRE(!NMathUtilities::IsZero(1.0f));
	REQUIRE(!NMathUtilities::IsZero(1.0));

	REQUIRE(!NMathUtilities::IsNan(kNPi));
	REQUIRE(!NMathUtilities::IsInf(kNPi));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NMATHUTILITIES("Integer")
{


	// Perform the test
	REQUIRE( NMathUtilities::IsOdd(1));
	REQUIRE(!NMathUtilities::IsOdd(2));

	REQUIRE(!NMathUtilities::IsEven(1));
	REQUIRE( NMathUtilities::IsEven(2));

	REQUIRE(!NMathUtilities::IsPowerOf2(0));
	REQUIRE( NMathUtilities::IsPowerOf2(1));
	REQUIRE( NMathUtilities::IsPowerOf2(2));
	REQUIRE(!NMathUtilities::IsPowerOf2(3));
	REQUIRE( NMathUtilities::IsPowerOf2(4));
	REQUIRE(!NMathUtilities::IsPowerOf2(5));
	REQUIRE(!NMathUtilities::IsPowerOf2(6));
	REQUIRE(!NMathUtilities::IsPowerOf2(7));
	REQUIRE( NMathUtilities::IsPowerOf2(8));
	REQUIRE(!NMathUtilities::IsPowerOf2(9));
	REQUIRE(!NMathUtilities::IsPowerOf2(10));
	REQUIRE(!NMathUtilities::IsPowerOf2(11));
	REQUIRE(!NMathUtilities::IsPowerOf2(12));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NMATHUTILITIES("Bits")
{


	// Perform the test
	REQUIRE(NMathUtilities::CountBits(0) == 0);
	REQUIRE(NMathUtilities::CountBits(1) == 1);
	REQUIRE(NMathUtilities::CountBits(2) == 1);
	REQUIRE(NMathUtilities::CountBits(3) == 2);
	REQUIRE(NMathUtilities::CountBits(4) == 1);
	REQUIRE(NMathUtilities::CountBits(5) == 2);
	REQUIRE(NMathUtilities::CountBits(6) == 2);
	REQUIRE(NMathUtilities::CountBits(7) == 3);

	REQUIRE(NMathUtilities::RotateLeft( NN_B32(11001100, 11001100, 11001100, 11001100), 1) == NN_B32(10011001, 10011001, 10011001, 10011001));
	REQUIRE(NMathUtilities::RotateLeft( NN_B32(11001100, 11001100, 11001100, 11001100), 2) == NN_B32(00110011, 00110011, 00110011, 00110011));

	REQUIRE(NMathUtilities::RotateRight(NN_B32(11001100, 11001100, 11001100, 11001100), 1) == NN_B32(01100110, 01100110, 01100110, 01100110));
	REQUIRE(NMathUtilities::RotateRight(NN_B32(11001100, 11001100, 11001100, 11001100), 2) == NN_B32(00110011, 00110011, 00110011, 00110011));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NMATHUTILITIES("Angles")
{


	// Perform the test
	REQUIRE(NMathUtilities::AreEqual(90.0, NMathUtilities::GetDegrees(NMathUtilities::GetRadians(90.0))));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NMATHUTILITIES("Trig")
{


	// Perform the test
	REQUIRE(NMathUtilities::AreEqual(NMathUtilities::GetSign( kNPi),  1.0));
	REQUIRE(NMathUtilities::AreEqual(NMathUtilities::GetSign(-kNPi), -1.0));

	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Sec(  1.0), 1.850816f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Csc(  1.0), 1.188395f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Ctan( 1.0), 0.642093f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Asin( 1.0), 1.570796f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Acos( 1.0), 0.000000f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Asec( 1.0), 0.000000f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Acsc( 1.0), 1.570796f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Acot( 1.0), 0.785398f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Sinh( 1.0), 1.175201f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Cosh( 1.0), 1.543081f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Tanh( 1.0), 0.761594f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Sech( 1.0), 0.648054f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Csch( 1.0), 0.850918f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Coth( 1.0), 1.313035f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Asinh(1.0), 0.881374f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Acosh(1.0), 0.000000f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Atanh(0.5), 0.549306f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Asech(1.0), 0.000000f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Acsch(1.0), 0.881374f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Acoth(8.0), 0.125657f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Rint(1.0), 1.0f));
	REQUIRE(NMathUtilities::AreEqual((float32_t) NMathUtilities::Rint(1.0), 1.0f));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NMATHUTILITIES("Random")
{	uint64_t		valueUInt64;
	uint32_t		valueUInt32;
	uint16_t		valueUInt16;
	uint8_t			valueUInt8;
	bool			valueBool;
	NIndex			n;



	// Perform the test
	REQUIRE(NMathUtilities::SetRandomSeed(0) != 0);
	
	for (n = 0; n < 1000; n++)
		{
		// Open range
		valueBool   = NMathUtilities::GetRandomBoolean();
		valueUInt8  = NMathUtilities::GetRandomUInt8();
		valueUInt16 = NMathUtilities::GetRandomUInt16();
		valueUInt32 = NMathUtilities::GetRandomUInt32();
		valueUInt64 = NMathUtilities::GetRandomUInt64();

		REQUIRE((valueBool == true || valueBool == false));
		REQUIRE((valueUInt8  >= kUInt8Min  && valueUInt8  <= kUInt8Max));
		REQUIRE((valueUInt16 >= kUInt16Min && valueUInt16 <= kUInt16Max));
		REQUIRE((valueUInt32 >= kUInt32Min && valueUInt32 <= kUInt32Max));
		REQUIRE((valueUInt64 >= kUInt64Min && valueUInt64 <= kUInt64Max));


		// Small range
		valueUInt8  = NMathUtilities::GetRandomUInt8(kTestRangeSmallMin, kTestRangeSmallMax);
		valueUInt16 = NMathUtilities::GetRandomUInt8(kTestRangeSmallMin, kTestRangeSmallMax);
		valueUInt32 = NMathUtilities::GetRandomUInt8(kTestRangeSmallMin, kTestRangeSmallMax);
		valueUInt64 = NMathUtilities::GetRandomUInt8(kTestRangeSmallMin, kTestRangeSmallMax);
		
		REQUIRE((valueUInt8  >= kTestRangeSmallMin && valueUInt8  <= kTestRangeSmallMax));
		REQUIRE((valueUInt16 >= kTestRangeSmallMin && valueUInt16 <= kTestRangeSmallMax));
		REQUIRE((valueUInt32 >= kTestRangeSmallMin && valueUInt32 <= kTestRangeSmallMax));
		REQUIRE((valueUInt64 >= kTestRangeSmallMin && valueUInt64 <= kTestRangeSmallMax));


		// Large range
		valueUInt8  = NMathUtilities::GetRandomUInt8(kTestRangeLargeMin, kTestRangeLargeMax);
		valueUInt16 = NMathUtilities::GetRandomUInt8(kTestRangeLargeMin, kTestRangeLargeMax);
		valueUInt32 = NMathUtilities::GetRandomUInt8(kTestRangeLargeMin, kTestRangeLargeMax);
		valueUInt64 = NMathUtilities::GetRandomUInt8(kTestRangeLargeMin, kTestRangeLargeMax);
		
		REQUIRE((valueUInt8  >= kTestRangeLargeMin && valueUInt8  <= kTestRangeLargeMax));
		REQUIRE((valueUInt16 >= kTestRangeLargeMin && valueUInt16 <= kTestRangeLargeMax));
		REQUIRE((valueUInt32 >= kTestRangeLargeMin && valueUInt32 <= kTestRangeLargeMax));
		REQUIRE((valueUInt64 >= kTestRangeLargeMin && valueUInt64 <= kTestRangeLargeMax));
		}
}




