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
#include "NanoTypes.h"





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
	bool                                NextBool();
	uint8_t                             NextUInt8();
	uint16_t                            NextUInt16();
	uint32_t                            NextUInt32();
	uint64_t                            NextUInt64();
	float32_t                           NextFloat32();
	float64_t                           NextFloat64();
	void                                NextData(size_t theSize, void* thePtr);


private:
	uint64_t                            RandomSeed();
	uint32_t                            RandomUInt32();


private:
	uint64_t                            mSeed;
	uint64_t                            mState;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NRandom.inl"



#endif // NRANDOM_H
