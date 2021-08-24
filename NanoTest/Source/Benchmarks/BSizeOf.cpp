/*	NAME:
		BSizeOf.cpp

	DESCRIPTION:
		Size benchmark.

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
#include "NAny.h"
#include "NArray.h"
#include "NDictionary.h"
#include "NMutex.h"
#include "NNumber.h"
#include "NPreference.h"
#include "NString.h"
#include "NTestFixture.h"
#include "NVariant.h"

// System
#include <any>





//=============================================================================
//		Internal Macros
//-----------------------------------------------------------------------------
#define NN_BENCHMARK_SIZEOF                                 0





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(BSizeOf){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(BSizeOf, "Types")
{


	// Perform the test
	REQUIRE(sizeof(NAny) == sizeof(std::any));

#if NN_COMPILER_CLANG
	REQUIRE(sizeof(NAny) == 32);
	REQUIRE(sizeof(NArray) == 24);
	REQUIRE(sizeof(NDictionary) == 40);
	REQUIRE(sizeof(NMutex) == 32);
	REQUIRE(sizeof(NNumber) == 16);
	REQUIRE(sizeof(NPreference) == 112);
	REQUIRE(sizeof(NString) == 32);
	REQUIRE(sizeof(NVariant<uint8_t>) == 8);
#endif // NN_COMPILER_CLANG

#if NN_BENCHMARK_SIZEOF
	NN_LOG_INFO("sizeof(NAny)        = {}", sizeof(NAny));
	NN_LOG_INFO("sizeof(NArray)      = {}", sizeof(NArray));
	NN_LOG_INFO("sizeof(NDictionary) = {}", sizeof(NDictionary));
	NN_LOG_INFO("sizeof(NMutex)      = {}", sizeof(NMutex));
	NN_LOG_INFO("sizeof(NNumber)     = {}", sizeof(NNumber));
	NN_LOG_INFO("sizeof(NPreference) = {}", sizeof(NPreference));
	NN_LOG_INFO("sizeof(NString)     = {}", sizeof(NString));
	NN_LOG_INFO("sizeof(NVariant)    = {}", sizeof(NVariant<uint8_t>));
#endif // NN_BENCHMARK_SIZEOF
}
