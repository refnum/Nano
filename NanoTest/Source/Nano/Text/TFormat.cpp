/*	NAME:
		TFormat.cpp

	DESCRIPTION:
		NFormat tests.

	COPYRIGHT:
		Copyright (c) 2006-2020, refNum Software
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
#include "NFormat.h"
#include "NTestFixture.h"

// System
#include <inttypes.h>





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr uint8_t  kTestUInt8                        = 8;
static constexpr uint16_t kTestUInt16                       = 1616;
static constexpr uint32_t kTestUInt32                       = 32323232;
static constexpr uint64_t kTestUInt64                       = 6464646464646464ULL;

static constexpr int8_t  kTestInt8                          = -8;
static constexpr int16_t kTestInt16                         = -1616;
static constexpr int32_t kTestInt32                         = -32323232;
static constexpr int64_t kTestInt64                         = -6464646464646464LL;

static constexpr float64_t kTestFloat64                     = 3.141592653589793;

static const NString kTestString                            = "Hello World";





//=============================================================================
//		Test fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(NFormat)
{
	NString theResult;
};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Default/format")
{


	// Perform the test
	theResult = NFormat("ArgUInt8 [{}]", kTestUInt8);
	REQUIRE(theResult == "ArgUInt8 [8]");

	theResult = NFormat("ArgUInt16 [{}]", kTestUInt16);
	REQUIRE(theResult == "ArgUInt16 [1616]");

	theResult = NFormat("ArgUInt32 [{}]", kTestUInt32);
	REQUIRE(theResult == "ArgUInt32 [32323232]");

	theResult = NFormat("ArgUInt64 [{}]", kTestUInt64);
	REQUIRE(theResult == "ArgUInt64 [6464646464646464]");


	theResult = NFormat("ArgSInt8 [{}]", kTestInt8);
	REQUIRE(theResult == "ArgSInt8 [-8]");

	theResult = NFormat("ArgSInt16 [{}]", kTestInt16);
	REQUIRE(theResult == "ArgSInt16 [-1616]");

	theResult = NFormat("ArgSInt32 [{}]", kTestInt32);
	REQUIRE(theResult == "ArgSInt32 [-32323232]");

	theResult = NFormat("ArgSInt64 [{}]", kTestInt64);
	REQUIRE(theResult == "ArgSInt64 [-6464646464646464]");


	theResult = NFormat("ArgFloat32 [{:.7}]", kTestFloat64);
	REQUIRE(theResult == "ArgFloat32 [3.141593]");

	theResult = NFormat("ArgFloat64 [{:.15}]", kTestFloat64);
	REQUIRE(theResult == "ArgFloat64 [3.14159265358979]");


	theResult = NFormat("ArgHex [{:x}]", kTestUInt32);
	REQUIRE(theResult == "ArgHex [1ed36a0]");

	theResult = NFormat("ArgHEX [{:X}]", kTestUInt32);
	REQUIRE(theResult == "ArgHEX [1ED36A0]");


	theResult = NFormat("ArgChar [{}]", 'h');
	REQUIRE(theResult == "ArgChar [h]");

	theResult = NFormat("ArgText [{}]", "Hello World");
	REQUIRE(theResult == "ArgText [Hello World]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Default/printf")
{


	// Perform the test
	theResult = NFormat("ArgUInt8 [%" PRIu8 "]", kTestUInt8);
	REQUIRE(theResult == "ArgUInt8 [8]");

	theResult = NFormat("ArgUInt16 [%" PRIu16 "]", kTestUInt16);
	REQUIRE(theResult == "ArgUInt16 [1616]");

	theResult = NFormat("ArgUInt32 [%" PRIu32 "]", kTestUInt32);
	REQUIRE(theResult == "ArgUInt32 [32323232]");

	theResult = NFormat("ArgUInt64 [%" PRIu64 "]", kTestUInt64);
	REQUIRE(theResult == "ArgUInt64 [6464646464646464]");


	theResult = NFormat("ArgSInt8 [%" PRId8 "]", kTestInt8);
	REQUIRE(theResult == "ArgSInt8 [-8]");

	theResult = NFormat("ArgSInt16 [%" PRId16 "]", kTestInt16);
	REQUIRE(theResult == "ArgSInt16 [-1616]");

	theResult = NFormat("ArgSInt32 [%" PRId32 "]", kTestInt32);
	REQUIRE(theResult == "ArgSInt32 [-32323232]");

	theResult = NFormat("ArgSInt64 [%" PRId64 "]", kTestInt64);
	REQUIRE(theResult == "ArgSInt64 [-6464646464646464]");


	theResult = NFormat("ArgFloat32 [%.7g]", kTestFloat64);
	REQUIRE(theResult == "ArgFloat32 [3.141593]");

	theResult = NFormat("ArgFloat64 [%.15g]", kTestFloat64);
	REQUIRE(theResult == "ArgFloat64 [3.14159265358979]");


	theResult = NFormat("ArgHex [%" PRIx32 "]", kTestUInt32);
	REQUIRE(theResult == "ArgHex [1ed36a0]");

	theResult = NFormat("ArgHEX [%" PRIX32 "]", kTestUInt32);
	REQUIRE(theResult == "ArgHEX [1ED36A0]");


	theResult = NFormat("ArgChar [%c]", 'h');
	REQUIRE(theResult == "ArgChar [h]");

	theResult = NFormat("ArgText [%s]", "Hello World");
	REQUIRE(theResult == "ArgText [Hello World]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Position/format")
{


	// Perform the test
	theResult = NFormat("ArgPos [{2}] [{0}] [{1}]", 11, 22, 33);
	REQUIRE(theResult == "ArgPos [33] [11] [22]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Position/printf")
{


	// Perform the test
	theResult = NFormat("ArgPos [%3$d] [%1$d] [%2$d]", 11, 22, 33);
	REQUIRE(theResult == "ArgPos [33] [11] [22]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Width/format")
{


	// Perform the test
	int32_t valueInt = 10;
	theResult        = NFormat("ArgSInt16 [{:{}}]", kTestInt16, valueInt);
	REQUIRE(theResult == "ArgSInt16 [     -1616]");

	theResult = NFormat("ArgSInt16 [{:10}]", kTestInt16);
	REQUIRE(theResult == "ArgSInt16 [     -1616]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Width/printf")
{


	// Perform the test
	int32_t valueInt = 10;
	theResult        = NFormat("ArgSInt16 [%*d]", valueInt, kTestInt16);
	REQUIRE(theResult == "ArgSInt16 [     -1616]");

	theResult = NFormat("ArgSInt16 [%10d]", kTestInt16);
	REQUIRE(theResult == "ArgSInt16 [     -1616]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Precision/format")
{


	// Perform the test
	int32_t valueInt = 5;
	theResult        = NFormat("ArgFloat32 [{:.{}}]", kTestFloat64, valueInt);
	REQUIRE(theResult == "ArgFloat32 [3.1416]");

	theResult = NFormat("ArgFloat32 [{:.5}]", kTestFloat64);
	REQUIRE(theResult == "ArgFloat32 [3.1416]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Precision/printf")
{


	// Perform the test
	int32_t valueInt = 5;
	theResult        = NFormat("ArgFloat32 [%.*g]", valueInt, kTestFloat64);
	REQUIRE(theResult == "ArgFloat32 [3.1416]");

	theResult = NFormat("ArgFloat32 [%.5g]", kTestFloat64);
	REQUIRE(theResult == "ArgFloat32 [3.1416]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Alignment/format")
{


	// Perform the test
	theResult = NFormat("ArgUInt16 [{:>7}]", kTestUInt16);
	REQUIRE(theResult == "ArgUInt16 [   1616]");

	theResult = NFormat("ArgUInt16 [{:<7}]", kTestUInt16);
	REQUIRE(theResult == "ArgUInt16 [1616   ]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Alignment/printf")
{


	// Perform the test
	theResult = NFormat("ArgUInt16 [%7d]", kTestUInt16);
	REQUIRE(theResult == "ArgUInt16 [   1616]");

	theResult = NFormat("ArgUInt16 [%-7d]", kTestUInt16);
	REQUIRE(theResult == "ArgUInt16 [1616   ]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Count/format")
{


	// Perform the test
	theResult = NFormat("OneArg [{}]", 42);
	REQUIRE(theResult == "OneArg [42]");

	theResult = NFormat("TwoArgs [{}] [{}]", 42, 23);
	REQUIRE(theResult == "TwoArgs [42] [23]");

	theResult = NFormat("ThreeArgs [{}] [{}] [{}]", 42, 23, 101);
	REQUIRE(theResult == "ThreeArgs [42] [23] [101]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Count/printf")
{


	// Perform the test
	theResult = NFormat("OneArg [%d]", 42);
	REQUIRE(theResult == "OneArg [42]");

	theResult = NFormat("TwoArgs [%d] [%d]", 42, 23);
	REQUIRE(theResult == "TwoArgs [42] [23]");

	theResult = NFormat("ThreeArgs [%d] [%d] [%d]", 42, 23, 101);
	REQUIRE(theResult == "ThreeArgs [42] [23] [101]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Objects/format")
{


	// Perform the test
	theResult = NFormat("String [{}]", kTestString);
	REQUIRE(theResult == "String [Hello World]");
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(NFormat, "Objects/printf")
{


	// Perform the test
	theResult = NFormat("String [%s]", kTestString.GetUTF8());
	REQUIRE(theResult == "String [Hello World]");
}
