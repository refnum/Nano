/*	NAME:
		TTimeUtils.cpp

	DESCRIPTION:
		NTimeUtils tests.

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
#include "NTimeUtils.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static constexpr NInterval       kTestInterval              = 123.111222444;
static constexpr struct timespec kTestTimespec              = {123, 111222444};

static constexpr NTime kTestTime                            = 0.0;





//=============================================================================
//		Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TTimeUtils){};





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TTimeUtils, "GetTime")
{


	// Perform the test
	REQUIRE(NTimeUtils::GetTime() != 0.0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TTimeUtils, "GetUpTime")
{


	// Perform the test
	REQUIRE(NTimeUtils::GetUpTime() != 0.0);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TTimeUtils, "Convert/timespec")
{


	// Perform the test
	auto timeSpec = NTimeUtils::ToTimespec(kTestInterval);
	REQUIRE(timeSpec.tv_sec == kTestTimespec.tv_sec);
	REQUIRE(timeSpec.tv_nsec == kTestTimespec.tv_nsec);
	REQUIRE(NTimeUtils::ToInterval(timeSpec) == kTestInterval);
}





//=============================================================================
//		Test case
//-----------------------------------------------------------------------------
NANO_TEST(TTimeUtils, "Convert/tm")
{


	// Perform the test
	auto timeUTC = NTimeUtils::ToTmUTC(kTestTime);
	REQUIRE(timeUTC.tm_sec == 0);
	REQUIRE(timeUTC.tm_min == 0);
	REQUIRE(timeUTC.tm_hour == 0);
	REQUIRE(timeUTC.tm_mday == 1);
	REQUIRE(timeUTC.tm_mon == 0);
	REQUIRE(timeUTC.tm_year == 101);
	REQUIRE(timeUTC.tm_wday == 1);
	REQUIRE(timeUTC.tm_yday == 0);
	REQUIRE(timeUTC.tm_isdst == 0);
	REQUIRE(NTimeUtils::ToTime(timeUTC) == kTestTime);
}
