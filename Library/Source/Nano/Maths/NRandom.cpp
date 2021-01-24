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

// System
#include <random>





//=============================================================================
//		NRandom::NRandom : Constructor.
//-----------------------------------------------------------------------------
NRandom::NRandom(uint64_t theSeed)
	: mSeed{}
	, mState{}
{


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


	// Generate a seed
	if (theSeed == 0)
	{
		theSeed = RandomSeed();
	}



	// Set the seed
	mSeed  = theSeed;
	mState = ~mSeed;
}





//=============================================================================
//		NRandom::NextData : Get the next random data.
//-----------------------------------------------------------------------------
void NRandom::NextData(size_t theSize, void* thePtr)
{


	// Fill the data
	uint64_t* outputPtr = reinterpret_cast<uint64_t*>(thePtr);

	while (theSize > sizeof(uint64_t))
	{
		*outputPtr++ = NextUInt64();
		theSize -= sizeof(uint64_t);
	}



	// Fill the remainder
	if (theSize > 0)
	{
		uint64_t theValue = NextUInt64();
		memcpy(outputPtr, &theValue, theSize);
	}
}





#pragma mark private
//=============================================================================
//		NRandom::RandomSeed : Generate a random seed.
//-----------------------------------------------------------------------------
uint64_t NRandom::RandomSeed()
{


	// Generate the seed
	std::random_device sRandomDevice;

	return sRandomDevice();
}





//=============================================================================
//		NRandom::RandomUInt32 : Generate the next random value.
//-----------------------------------------------------------------------------
uint32_t NRandom::RandomUInt32()
{


	// Generate the value
	//
	// We use PCG-XSH-RR as our core generator.
	//
	// https://www.pcg-random.org
	//
	uint64_t oldState  = mState;
	uint32_t rotateBy  = oldState >> 59u;
	uint32_t theResult = uint32_t(oldState ^ (oldState >> 18u) >> 27u);

	if (rotateBy != 0)
	{
		theResult = NMathUtils::RotateRight(theResult, rotateBy);
	}



	// Update our state
	mState = (oldState * 6364136223846793005ULL) + (mSeed | 1);

	return theResult;
}
