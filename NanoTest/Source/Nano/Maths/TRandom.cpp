/*	NAME:
		TRandom.cpp

	DESCRIPTION:
		NRandom tests.

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
#include "NDataDigest.h"
#include "NRandom.h"
#include "NTestFixture.h"

// System
#include <math.h>





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TRandom)
{
	NRandom theStream;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "Default")
{


	// Perform the test
	REQUIRE(theStream.GetSeed() != 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "Constructor")
{


	// Perform the test
	NRandom streamA(1);
	NRandom streamB(2);
	NRandom streamC(3);

	REQUIRE(streamA.GetSeed() == 1);
	REQUIRE(streamB.GetSeed() == 2);
	REQUIRE(streamC.GetSeed() == 3);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "SetSeed")
{


	// Perform the test
	theStream.SetSeed(1);
	REQUIRE(theStream.GetSeed() == 1);

	theStream.SetSeed(2);
	REQUIRE(theStream.GetSeed() == 2);

	theStream.SetSeed(3);
	REQUIRE(theStream.GetSeed() == 3);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "NextValue")
{


	// Perform the test
	uint8_t theBuffer[137]{};

	uint16_t  prevUInt16  = 0;
	uint32_t  prevUInt32  = 0;
	uint64_t  prevUInt64  = 0;
	float32_t prevFloat32 = 0.0f;
	float64_t prevFloat64 = 0.0;
	size_t    prevBuffer  = 0;

	for (size_t n = 0; n < 1000; n++)
	{
		(void) theStream.NextBool();
		(void) theStream.NextUInt8();

		uint16_t valueUInt16 = theStream.NextUInt16();
		REQUIRE(valueUInt16 != prevUInt16);

		uint32_t valueUInt32 = theStream.NextUInt32();
		REQUIRE(valueUInt32 != prevUInt32);

		uint64_t valueUInt64 = theStream.NextUInt64();
		REQUIRE(valueUInt64 != prevUInt64);

		float32_t valueFloat32 = theStream.NextFloat32();
		REQUIRE(!isnan(valueFloat32));
		REQUIRE(!isinf(valueFloat32));
		REQUIRE(valueFloat32 != prevFloat32);

		float64_t valueFloat64 = theStream.NextFloat64();
		REQUIRE(!isnan(valueFloat64));
		REQUIRE(!isinf(valueFloat64));
		REQUIRE(valueFloat64 != prevFloat64);

		theStream.NextData(sizeof(theBuffer), &theBuffer);
		size_t valueBuffer = NDataDigest::GetRuntime(sizeof(theBuffer), &theBuffer);
		REQUIRE(valueBuffer != prevBuffer);

		prevUInt16  = valueUInt16;
		prevUInt32  = valueUInt32;
		prevUInt64  = valueUInt64;
		prevFloat32 = valueFloat32;
		prevBuffer  = valueBuffer;
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "NextValueRange")
{


	// Perform the test
	for (size_t n = 0; n < 1000; n++)
	{
		uint8_t valueUInt8 = theStream.NextUInt8(100, 120);
		REQUIRE(valueUInt8 >= 100);
		REQUIRE(valueUInt8 <= 120);

		uint16_t valueUInt16 = theStream.NextUInt16(1000, 1020);
		REQUIRE(valueUInt16 >= 1000);
		REQUIRE(valueUInt16 <= 1020);

		uint32_t valueUInt32 = theStream.NextUInt32(10000, 10020);
		REQUIRE(valueUInt32 >= 10000);
		REQUIRE(valueUInt32 <= 10020);

		uint64_t valueUInt64 = theStream.NextUInt64(100000, 100020);
		REQUIRE(valueUInt64 >= 100000);
		REQUIRE(valueUInt64 <= 100020);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "GetValue")
{


	// Perform the test
	uint8_t theBuffer[137]{};

	uint32_t  prevUInt16  = 0;
	uint32_t  prevUInt32  = 0;
	uint64_t  prevUInt64  = 0;
	float32_t prevFloat32 = 0.0f;
	float64_t prevFloat64 = 0.0;
	size_t    prevBuffer  = 0;

	for (size_t n = 0; n < 1000; n++)
	{
		(void) NRandom::GetBool();
		(void) NRandom::GetUInt8();

		uint16_t valueUInt16 = NRandom::GetUInt16();
		REQUIRE(valueUInt16 != prevUInt16);

		uint32_t valueUInt32 = NRandom::GetUInt32();
		REQUIRE(valueUInt32 != prevUInt32);

		uint64_t valueUInt64 = NRandom::GetUInt64();
		REQUIRE(valueUInt64 != prevUInt64);

		float32_t valueFloat32 = NRandom::GetFloat32();
		REQUIRE(!isnan(valueFloat32));
		REQUIRE(!isinf(valueFloat32));
		REQUIRE(valueFloat32 != prevFloat32);

		float64_t valueFloat64 = NRandom::GetFloat64();
		REQUIRE(!isnan(valueFloat64));
		REQUIRE(!isinf(valueFloat64));
		REQUIRE(valueFloat64 != prevFloat64);

		NRandom::GetData(sizeof(theBuffer), &theBuffer);
		size_t valueBuffer = NDataDigest::GetRuntime(sizeof(theBuffer), &theBuffer);
		REQUIRE(valueBuffer != prevBuffer);

		prevUInt16  = valueUInt16;
		prevUInt32  = valueUInt32;
		prevUInt64  = valueUInt64;
		prevFloat32 = valueFloat32;
		prevBuffer  = valueBuffer;
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "GetValueRange")
{


	// Perform the test
	for (size_t n = 0; n < 1000; n++)
	{
		uint8_t valueUInt8 = NRandom::GetUInt8(100, 120);
		REQUIRE(valueUInt8 >= 100);
		REQUIRE(valueUInt8 <= 120);

		uint16_t valueUInt16 = NRandom::GetUInt16(1000, 1020);
		REQUIRE(valueUInt16 >= 1000);
		REQUIRE(valueUInt16 <= 1020);

		uint32_t valueUInt32 = NRandom::GetUInt32(10000, 10020);
		REQUIRE(valueUInt32 >= 10000);
		REQUIRE(valueUInt32 <= 10020);

		uint64_t valueUInt64 = NRandom::GetUInt64(100000, 100020);
		REQUIRE(valueUInt64 >= 100000);
		REQUIRE(valueUInt64 <= 100020);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "GetSecureData")
{
	uint8_t theBuffer[256]{};

	NRandom::GetSecureData(sizeof(theBuffer), &theBuffer);

	size_t theDigest = NDataDigest::GetRuntime(sizeof(theBuffer), &theBuffer);
	REQUIRE(theDigest != 0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "MultipleStreams")
{


	// Perform the test
	NRandom streamA1(1);
	NRandom streamA2(1);

	NRandom streamB1(2);
	NRandom streamB2(2);

	NRandom streamC1(3);
	NRandom streamC2(3);

	for (size_t n = 0; n < 1000; n++)
	{
		uint64_t valueA1 = streamA1.NextUInt64();
		uint64_t valueA2 = streamA2.NextUInt64();

		uint64_t valueB1 = streamB1.NextUInt64();
		uint64_t valueB2 = streamB2.NextUInt64();

		uint64_t valueC1 = streamC1.NextUInt64();
		uint64_t valueC2 = streamC2.NextUInt64();

		REQUIRE(valueA1 == valueA2);
		REQUIRE(valueB1 == valueB2);
		REQUIRE(valueC1 == valueC2);

		REQUIRE(valueA1 != valueB1);
		REQUIRE(valueA1 != valueC1);
		REQUIRE(valueB1 != valueC1);
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "FinalState")
{


	// Perform the test
	theStream.SetSeed(0x22042012);

	bool valueBool{};
	for (size_t n = 0; n < 1000; n++)
	{
		valueBool = theStream.NextBool();
	}
	REQUIRE(valueBool == true);


	uint8_t valueUInt8{};
	for (size_t n = 0; n < 1000; n++)
	{
		valueUInt8 = theStream.NextUInt8();
	}
	REQUIRE(valueUInt8 == 0x1f);


	uint16_t valueUInt16{};
	for (size_t n = 0; n < 1000; n++)
	{
		valueUInt16 = theStream.NextUInt16();
	}
	REQUIRE(valueUInt16 == 0xdee7);


	uint32_t valueUInt32{};
	for (size_t n = 0; n < 1000; n++)
	{
		valueUInt32 = theStream.NextUInt32();
	}
	REQUIRE(valueUInt32 == 0xe48787b3);


	uint64_t valueUInt64{};
	for (size_t n = 0; n < 1000; n++)
	{
		valueUInt64 = theStream.NextUInt64();
	}
	REQUIRE(valueUInt64 == 0x6f279a4886e21ffdULL);


	float32_t valueFloat32{};
	for (size_t n = 0; n < 1000; n++)
	{
		valueFloat32 = theStream.NextFloat32();
	}
	size_t digestFloat32 = NDataDigest::GetXXHash64(sizeof(valueFloat32), &valueFloat32);
	REQUIRE(digestFloat32 == 0x90a05d796e08def4ULL);
	REQUIRE(valueFloat32 == 0.110932469f);


	float64_t valueFloat64{};
	for (size_t n = 0; n < 1000; n++)
	{
		valueFloat64 = theStream.NextFloat64();
	}
	size_t digestFloat64 = NDataDigest::GetXXHash64(sizeof(valueFloat64), &valueFloat64);
	REQUIRE(digestFloat64 == 0x0429687d7cfc1ebfULL);
	REQUIRE(valueFloat64 == 0.62953353154464109);


	uint64_t valueData[10]{};
	for (size_t n = 0; n < 1000; n++)
	{
		theStream.NextData(sizeof(valueData), &valueData);
	}
	size_t digestData = NDataDigest::GetXXHash64(sizeof(valueData), &valueData);
	REQUIRE(digestData == 0x6ce43cf7f3415b5aULL);
}
