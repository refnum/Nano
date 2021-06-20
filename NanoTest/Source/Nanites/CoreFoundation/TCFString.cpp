/*	NAME:
		TCFString.cpp

	DESCRIPTION:
		NCFString tests.

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
#include "NCFString.h"
#include "NTestFixture.h"





//=============================================================================
//		Internal Constants
//-----------------------------------------------------------------------------
static const NString     kTestNString                       = "Hello World";
static const CFStringRef kTestCFString                      = CFSTR("Hello World");





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
#define TEST_NCFSTRING(...)                                 TEST_NANO(TCFString, ##__VA_ARGS__)

FIXTURE_NANO(TCFString)
{
	NCFString theString;
	NCFObject cfString;
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
TEST_NCFSTRING("Assign", "[cf]")
{


	// Perform the test
	theString = kTestNString;
	cfString  = theString.GetObject();
	REQUIRE(CFStringCompare(cfString, kTestCFString, 0) == (CFComparisonResult) kCFCompareEqualTo);

	theString = NCFString(kTestCFString, false);
	REQUIRE(theString == kTestNString);

	theString.SetObject(kTestCFString, false);
	REQUIRE(theString == kTestNString);

	theString.SetObject(CFStringCreateCopy(kCFAllocatorDefault, kTestCFString), true);
	REQUIRE(theString == kTestNString);
}
