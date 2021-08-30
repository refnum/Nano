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
#include "NData.h"
#include "NDictionary.h"
#include "NMutex.h"
#include "NNumber.h"
#include "NPreference.h"
#include "NString.h"
#include "NTestFixture.h"
#include "NTime.h"
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
#if NN_BENCHMARK_SIZEOF
	NN_LOG_INFO("sizeof(NAny)        = {}", sizeof(NAny));
	NN_LOG_INFO("sizeof(NArray)      = {}", sizeof(NArray));
	NN_LOG_INFO("sizeof(NData)       = {}", sizeof(NData));
	NN_LOG_INFO("sizeof(NDictionary) = {}", sizeof(NDictionary));
	NN_LOG_INFO("sizeof(NMutex)      = {}", sizeof(NMutex));
	NN_LOG_INFO("sizeof(NNumber)     = {}", sizeof(NNumber));
	NN_LOG_INFO("sizeof(NString)     = {}", sizeof(NString));
	NN_LOG_INFO("sizeof(NTime)       = {}", sizeof(NTime));
	NN_LOG_INFO("sizeof(NVariant)    = {}", sizeof(NVariant<bool>));

	NN_LOG_INFO("sizeof(NPreferenceBool)     = {}", sizeof(NPreferenceBool));
	NN_LOG_INFO("sizeof(NPreferenceUInt32)   = {}", sizeof(NPreferenceUInt32));
	NN_LOG_INFO("sizeof(NPreferenceUInt64)   = {}", sizeof(NPreferenceUInt64));
	NN_LOG_INFO("sizeof(NPreferenceInt32)    = {}", sizeof(NPreferenceInt32));
	NN_LOG_INFO("sizeof(NPreferenceInt64)    = {}", sizeof(NPreferenceInt64));
	NN_LOG_INFO("sizeof(NPreferenceFloat32)  = {}", sizeof(NPreferenceFloat32));
	NN_LOG_INFO("sizeof(NPreferenceFloat64)  = {}", sizeof(NPreferenceFloat64));
#endif // NN_BENCHMARK_SIZEOF



	// Perform the test
	REQUIRE(sizeof(NAny) == sizeof(std::any));

#if NN_COMPILER_CLANG
	REQUIRE(sizeof(NAny) == 32);
	REQUIRE(sizeof(NArray) == 24);
	REQUIRE(sizeof(NData) == 32);
	REQUIRE(sizeof(NDictionary) == 96);
	REQUIRE(sizeof(NMutex) == 32);
	REQUIRE(sizeof(NNumber) == 16);
	REQUIRE(sizeof(NString) == 32);
	REQUIRE(sizeof(NTime) == 8);
	REQUIRE(sizeof(NVariant<bool>) == 8);

	REQUIRE(sizeof(NPreferenceBool) == sizeof(bool));
	REQUIRE(sizeof(NPreferenceUInt32) == sizeof(uint32_t));
	REQUIRE(sizeof(NPreferenceUInt64) == sizeof(uint64_t));
	REQUIRE(sizeof(NPreferenceInt32) == sizeof(int32_t));
	REQUIRE(sizeof(NPreferenceInt64) == sizeof(int64_t));
	REQUIRE(sizeof(NPreferenceFloat32) == sizeof(float32_t));
	REQUIRE(sizeof(NPreferenceFloat64) == sizeof(float64_t));
#endif // NN_COMPILER_CLANG
}
