/*	NAME:
		NRandom.cpp

	DESCRIPTION:
		Random numbers.

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
#include "NRandom.h"





//=============================================================================
//		NRandom::NRandom : Constructor.
//-----------------------------------------------------------------------------
NRandom::NRandom(uint64_t theSeed)
	: mSeed{}
	, mState{}
{


	// Validate our state
	static_assert(sizeof(mState) == sizeof(NUInt128), "Insufficient state for PCG-64");


	// Set the seed
	SetSeed(theSeed);
}





//=============================================================================
//		NRandom::GetSeed : Get the seed.
//-----------------------------------------------------------------------------
uint64_t NRandom::GetSeed() const
{


	// Get the seed
	return mSeed;
}





//=============================================================================
//		NRandom::SetSeed : Set the seed.
//-----------------------------------------------------------------------------
void NRandom::SetSeed(uint64_t theSeed)
{


	// Set the seed
	if (theSeed == 0)
	{
		theSeed = GenerateSeed();
	}

	mSeed = theSeed;



	// Initialise the state
	//
	// The initial state must be non-zero and derived from the seed.
	uint128_t theState = NMakeUInt128(mSeed * 3, mSeed * 7);

	static_assert(sizeof(mState) == sizeof(theState));
	memcpy(&mState, &theState, sizeof(theState));
}





//=============================================================================
//		NRandom::NextFloat32 : Get the next float32_t.
//-----------------------------------------------------------------------------
float32_t NRandom::NextFloat32()
{


	// Generate a float32_t
	//
	// See NextFloat64.
	uint32_t n = NextUInt32();

	if (n >= 0xff800000U)
	{
		constexpr uint32_t numerator   = 0x00000200U;
		constexpr uint32_t denominator = 0x00800001U;
		constexpr uint32_t mask        = 0x00ffffffU;

		uint32_t m = 0;
		do
		{
			m = NextUInt32() & mask;
		} while (m >= denominator);

		if (m < numerator)
		{
			return 1.0f;
		}
	}

	uint32_t  valueInteger = 0x3f800000U | (n & 0x007fffffU);
	float32_t valueReal;

	memcpy(&valueReal, &valueInteger, sizeof(valueReal));

	return valueReal - 1.0f;
}





//=============================================================================
//		NRandom::NextFloat64 : Get the next float64_t.
//-----------------------------------------------------------------------------
float64_t NRandom::NextFloat64()
{


	// Generate a float64_t
	//
	// Generate a random float64_t with uniform distribution using Gainey's
	// algorithm:
	//
	//		https://experilous.com/1/blog/post/perfect-fast-random-floating-point-numbers
	//		https://archive.is/FM3a2
	//
	// A review of alternative algorithms, and how they diverge from uniform
	// distribution, is at:
	//
	//		https://hal.archives-ouvertes.fr/hal-02427338/document
	//
	//
	// This approach uses a 52-bit random integer to generate reals that
	// are uniformly distributed from 1.0 to 2.0, then subtracting 1.0.
	//
	// The upper bound, 1.0, is omitted from this approach.
	//
	// It is restored by randomly inserting it into the results with the
	// appropriate probability as if it could occur.
	uint64_t n = NextUInt64();

	if (n >= 0xfff0000000000000ULL)
	{
		constexpr uint64_t numerator   = 0x0000000000001000ULL;
		constexpr uint64_t denominator = 0x0010000000000001ULL;
		constexpr uint64_t mask        = 0x001fffffffffffffULL;

		uint64_t m = 0;
		do
		{
			m = NextUInt64() & mask;
		} while (m >= denominator);

		if (m < numerator)
		{
			return 1.0;
		}
	}

	uint64_t  valueInteger = 0x3ff0000000000000ULL | (n & 0x000fffffffffffffULL);
	float64_t valueReal;

	memcpy(&valueReal, &valueInteger, sizeof(valueReal));

	return valueReal - 1.0;
}





//=============================================================================
//		NRandom::NextUInt8 : Get the next uint8_t.
//-----------------------------------------------------------------------------
uint8_t NRandom::NextUInt8(uint8_t minValue, uint8_t maxValue)
{


	// Get the value
	return PCG_XSH_RR(minValue, maxValue);
}





//=============================================================================
//		NRandom::NextUInt16 : Get the next uint16_t.
//-----------------------------------------------------------------------------
uint16_t NRandom::NextUInt16(uint16_t minValue, uint16_t maxValue)
{


	// Get the value
	return PCG_XSH_RR(minValue, maxValue);
}





//=============================================================================
//		NRandom::NextUInt32 : Get the next uint32_t.
//-----------------------------------------------------------------------------
uint32_t NRandom::NextUInt32(uint32_t minValue, uint32_t maxValue)
{


	// Get the value
	return PCG_XSH_RR(minValue, maxValue);
}





//=============================================================================
//		NRandom::NextUInt64 : Get the next uint64_t.
//-----------------------------------------------------------------------------
uint64_t NRandom::NextUInt64(uint64_t minValue, uint64_t maxValue)
{


	// Get the value
	return PCG_XSH_RR(minValue, maxValue);
}





//=============================================================================
//		NRandom::NextData : Get the next random data.
//-----------------------------------------------------------------------------
void NRandom::NextData(size_t theSize, void* thePtr)
{


	// Fill 8-byte chunks
	uint8_t* dstPtr = reinterpret_cast<uint8_t*>(thePtr);
	uint64_t theChunk;

	while (theSize > sizeof(theChunk))
	{
		theChunk = NextUInt64();
		memcpy(dstPtr, &theChunk, sizeof(theChunk));

		dstPtr += sizeof(theChunk);
		theSize -= sizeof(theChunk);
	}



	// Fill remaining bytes
	if (theSize > 0)
	{
		theChunk = NextUInt64();
		memcpy(dstPtr, &theChunk, theSize);
	}
}





#pragma mark private
//=============================================================================
//		NRandom::GenerateSeed : Generate a random seed.
//-----------------------------------------------------------------------------
uint64_t NRandom::GenerateSeed()
{


	// Generate the seed
	uint64_t theSeed = 0;

	GetSecureData(sizeof(theSeed), &theSeed);
	NN_REQUIRE(theSeed != 0);

	return theSeed;
}





//=============================================================================
//		NRandom::GetStream : Get the per-thread random stream.
//-----------------------------------------------------------------------------
NRandom& NRandom::GetStream()
{


	// Get the stream
	static thread_local NRandom sRandom;

	return sRandom;
}
