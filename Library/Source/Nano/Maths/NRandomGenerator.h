/*	NAME:
		NRandomGenerator.h

	DESCRIPTION:
		UniformRandomBitGenerator object.

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
#ifndef NRANDOM_GENERATOR_H
#define NRANDOM_GENERATOR_H
//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
// Nano
#include "NRandom.h"
#include "NanoTypes.h"





//=============================================================================
//		Class Declaration
//-----------------------------------------------------------------------------
class NRandomGenerator
{
public:
										NRandomGenerator(uint64_t theSeed = 0);


	// Get the stream
	NRandom&                            GetStream();


	// Get the range
	static constexpr uint64_t           min();
	static constexpr uint64_t           max();


	// Generate a value
	//
	// A random generator provides a random stream for methods that
	// take a UniformRandomBitGenerator instance (std::shuffle, etc).
	uint64_t                            operator()();


public:
	using result_type =                 uint64_t;


private:
	NRandom                             mStream;
};





//=============================================================================
//		Includes
//-----------------------------------------------------------------------------
#include "NRandomGenerator.inl"



#endif // NRANDOM_GENERATOR_H
