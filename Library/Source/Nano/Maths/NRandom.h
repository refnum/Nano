/*	NAME:
		NRandom.h

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
#ifndef NRANDOM_H
#define NRANDOM_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NanoConstants.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NRandom
{
public:
										NRandom(uint64_t theSeed = 0);


	// Get / set the seed
	//
	// A seed of zero will generate a new, random, seed.
	uint64_t                            GetSeed() const;
	void                                SetSeed(uint64_t theSeed = 0);


	// Get the next value
	//
	// Returns the next value in the random number stream.
	//
	// Integers are returned as values between 0 and their maximum value,
	// or between minValue and maxValue inclusive.
	//
	// Reals are returned as values between 0.0 and 1.0 inclusive.
	bool                                NextBool();
	uint8_t                             NextUInt8();
	uint16_t                            NextUInt16();
	uint32_t                            NextUInt32();
	uint64_t                            NextUInt64();
	float32_t                           NextFloat32();
	float64_t                           NextFloat64();

	uint8_t                             NextUInt8(uint8_t   minValue, uint8_t  maxValue);
	uint16_t                            NextUInt16(uint16_t minValue, uint16_t maxValue);
	uint32_t                            NextUInt32(uint32_t minValue, uint32_t maxValue);
	uint64_t                            NextUInt64(uint64_t minValue, uint64_t maxValue);
	void                                NextData(size_t theSize, void* thePtr);


	// Get a random value
	//
	// Returns the next value from a per-thread random number stream.
	//
	// Integers are returned as values between 0 and their maximum value,
	// or between minValue and maxValue inclusive.
	//
	// Reals are returned as values between 0.0 and 1.0 inclusive.
	static bool                         GetBool();
	static uint8_t                      GetUInt8();
	static uint16_t                     GetUInt16();
	static uint32_t                     GetUInt32();
	static uint64_t                     GetUInt64();
	static float32_t                    GetFloat32();
	static float64_t                    GetFloat64();

	static uint8_t                      GetUInt8(uint8_t   minValue, uint8_t  maxValue);
	static uint16_t                     GetUInt16(uint16_t minValue, uint16_t maxValue);
	static uint32_t                     GetUInt32(uint32_t minValue, uint32_t maxValue);
	static uint64_t                     GetUInt64(uint64_t minValue, uint64_t maxValue);
	static void                         GetData(size_t theSize, void* thePtr);


	// Get random data
	//
	// Returns cryptographically-secure random data.
	static void                         GetSecureData(size_t theSize, void* thePtr);


private:
	uint64_t                            GenerateSeed();

	static NRandom&                     GetStream();

	template<typename Output>
	Output                              PCG_XSH_RR();

	template<typename Output>
	Output                              PCG_XSH_RR(Output minValue, Output maxValue);



private:
	uint64_t                            mSeed;
	uint8_t                             mState[16];
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NRandom.inl"



#endif // NRANDOM_H
