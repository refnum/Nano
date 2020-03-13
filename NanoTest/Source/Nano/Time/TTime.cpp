/*	NAME:
		TTime.cpp

	DESCRIPTION:
		NTime tests.

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
#include "NTestFixture.h"
#include "NTime.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr NTime kTestTime                            = 388391022.1234;
static constexpr NTime kTestTimeConversion                  = 0.111222444;

static constexpr NTime kTestEarly                           = 1000;
static constexpr NTime kTestLate                            = 2000;





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TTime){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TTime, "Default")
{


	// Perform the test
	REQUIRE(NTime() == 0.0);
	REQUIRE(NTime() != kTestTime);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TTime, "Epochs")
{


	// Perform the test
	NTime nanoTime = 1234;
	NTime otherTime;

	otherTime = nanoTime + kNanoEpochTo2001;
	REQUIRE(nanoTime == NTime(otherTime, kNanoEpochFrom2001));

	otherTime = nanoTime + kNanoEpochTo1970;
	REQUIRE(nanoTime == NTime(otherTime, kNanoEpochFrom1970));

	otherTime = nanoTime + kNanoEpochTo1904;
	REQUIRE(nanoTime == NTime(otherTime, kNanoEpochFrom1904));

	otherTime = nanoTime + kNanoEpochTo1601;
	REQUIRE(nanoTime == NTime(otherTime, kNanoEpochFrom1601));
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TTime, "Conversion")
{


	// Perform the test
	REQUIRE(kTestTimeConversion.ToMS() == 111);
	REQUIRE(kTestTimeConversion.ToUS() == 111222);
	REQUIRE(kTestTimeConversion.ToNS() == 111222444);

	REQUIRE(NTime::ToMS(kTestTimeConversion) == 111);
	REQUIRE(NTime::ToUS(kTestTimeConversion) == 111222);
	REQUIRE(NTime::ToNS(kTestTimeConversion) == 111222444);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TTime, "Comparison")
{


	// Perform the test
	REQUIRE(kTestEarly == kTestEarly);
	REQUIRE(kTestEarly != kTestLate);

	REQUIRE(kTestEarly < kTestLate);
	REQUIRE(kTestLate > kTestEarly);

	REQUIRE(kTestEarly <= kTestLate);
	REQUIRE(kTestEarly <= kTestEarly);

	REQUIRE(kTestLate >= kTestEarly);
	REQUIRE(kTestLate >= kTestLate);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TTime, "Arithmetic")
{


	// Perform the test
	NTime theResult;

	theResult = kTestEarly;
	theResult += kTestEarly;
	REQUIRE(theResult == kTestLate);

	theResult = kTestLate;
	theResult -= kTestEarly;
	REQUIRE(theResult == kTestEarly);

	theResult = kTestEarly;
	theResult *= 2.0;
	REQUIRE(theResult == kTestLate);

	theResult = kTestLate;
	theResult /= 2.0;
	REQUIRE(theResult == kTestEarly);

	REQUIRE((kTestEarly + kTestEarly) == kTestLate);
	REQUIRE((kTestLate - kTestEarly) == kTestEarly);

	REQUIRE(((kTestEarly * 2.0) / 2.0) == kTestEarly);
	REQUIRE(((kTestEarly / 2.0) * 2.0) == kTestEarly);
}
