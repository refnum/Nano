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

	uint32_t  prevUInt32  = 0;
	uint64_t  prevUInt64  = 0;
	float32_t prevFloat32 = 0.0f;
	float64_t prevFloat64 = 0.0;
	size_t    prevBuffer  = 0;

	for (size_t n = 0; n < 1000; n++)
	{
		(void) theStream.NextBool();
		(void) theStream.NextUInt8();
		(void) theStream.NextUInt16();

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

		prevUInt32  = valueUInt32;
		prevUInt64  = valueUInt64;
		prevFloat32 = valueFloat32;
		prevBuffer  = valueBuffer;
	}
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TRandom, "MultipleStreams")
{


	// Perform the test
	NRandom streamA(1);
	NRandom streamB(2);
	NRandom streamC(3);

	for (size_t n = 0; n < 1000; n++)
	{
		uint64_t valueA = streamA.NextUInt64();
		uint64_t valueB = streamB.NextUInt64();
		uint64_t valueC = streamC.NextUInt64();

		REQUIRE(valueA != valueB);
		REQUIRE(valueA != valueC);
		REQUIRE(valueB != valueC);
	}
}
