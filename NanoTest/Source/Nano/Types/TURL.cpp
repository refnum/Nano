/*	NAME:
		TURL.cpp

	DESCRIPTION:
		NURL tests.

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
#include "NFormat.h"
#include "NTestFixture.h"
#include "NURL.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NURL kTestURL1                                 = "http://www.apple.com/path/to/file.txt";
static const NURL kTestURL2                                 = "http://www.apple.com:80/path/to/file.txt";

static const NString kTestScheme                            = "http";
static const NString kTestHost                              = "www.apple.com";
static const NIndex  kTestPort                              = 80;
static const NString kTestPath                              = "/path/to/file.txt";





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
#define TEST_NURL(...)                                      TEST_NANO(TURL, ##__VA_ARGS__)

FIXTURE_NANO(TURL){};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
TEST_NURL("Components")
{


	// Perform the test
	REQUIRE(kTestURL1.GetScheme() == kTestScheme);
	REQUIRE(kTestURL1.GetHost() == kTestHost);
	REQUIRE(kTestURL1.GetPort() == kTestPort);
	REQUIRE(kTestURL1.GetPath() == kTestPath);

	REQUIRE(kTestURL2.GetScheme() == kTestScheme);
	REQUIRE(kTestURL2.GetHost() == kTestHost);
	REQUIRE(kTestURL2.GetPort() == kTestPort);
	REQUIRE(kTestURL2.GetPath() == kTestPath);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", kTestURL1) == kTestURL1);
}
