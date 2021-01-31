/*	NAME:
		TUInt128.cpp

	DESCRIPTION:
		NUInt128 tests.

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
#include "NTestFixture.h"
#include "NUInt128.h"





//=============================================================================
//		Internal Functions
//-----------------------------------------------------------------------------
//		Make128Nano : Make a NUInt128.
//-----------------------------------------------------------------------------
constexpr NUInt128 Make128Nano(uint64_t valueHi, uint64_t valueLo)
{
	return NUInt128(valueHi, valueLo);
}





//=============================================================================
//		GetHi : Get the high bits.
//-----------------------------------------------------------------------------
constexpr uint64_t GetHi(const NUInt128& theValue)
{
	return theValue.GetHi();
}





//=============================================================================
//		GetLo : Get the low bits.
//-----------------------------------------------------------------------------
constexpr uint64_t GetLo(const NUInt128& theValue)
{
	return theValue.GetLo();
}



#if NN_UINT128_IS_NATIVE
//=============================================================================
//		Make128Native : Make a uint128_t.
//-----------------------------------------------------------------------------
constexpr uint128_t Make128Native(uint64_t valueHi, uint64_t valueLo)
{
	return uint128_t(valueHi) << 64 | uint128_t(valueLo);
}





//=============================================================================
//		GetHi : Get the high bits.
//-----------------------------------------------------------------------------
constexpr uint64_t GetHi(const uint128_t& theValue)
{
	return uint64_t(theValue >> 64);
}





//=============================================================================
//		GetLo : Get the low bits.
//-----------------------------------------------------------------------------
constexpr uint64_t GetLo(const uint128_t& theValue)
{
	return uint64_t(theValue);
}
#endif // #if NN_UINT128_IS_NATIVE





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TUInt128){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TUInt128, "NUInt128/Default")
{


	// Perform the test
	NUInt128 theValue;

	REQUIRE(GetHi(theValue) == 0);
	REQUIRE(GetLo(theValue) == 0);
}





//=============================================================================
//		NUInt128 Tests
//-----------------------------------------------------------------------------
#define NUINT128                                            NUInt128
#define NANO_TEST_UINT128(_name)                            NANO_TEST(TUInt128, "NUInt128/" _name)
#define Make128(_hi, _lo)                                   Make128Nano(_hi, _lo)

#include "TUInt128.imp"





//=============================================================================
//		uint128_t Tests
//-----------------------------------------------------------------------------
#if NN_UINT128_IS_NATIVE
	#undef NUINT128
	#undef NANO_TEST_UINT128
	#undef Make128

	#define NUINT128                                        uint128_t
	#define NANO_TEST_UINT128(_name)                        NANO_TEST(TUInt128, "uint128_t/" _name)
	#define Make128(_hi, _lo)                               Make128Native(_hi, _lo)

	#include "TUInt128.imp"
#endif // NN_UINT128_IS_NATIVE
