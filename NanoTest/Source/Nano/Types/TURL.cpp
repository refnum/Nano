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
static const NString  kTestURL                              = "http://www.aaaaa.com:80/path/to/file.txt";
static const NString  kTestScheme                           = "http";
static const NString  kTestHost                             = "www.aaaaa.com";
static const uint16_t kTestPort                             = 80;
static const NString  kTestPath                             = "/path/to/file.txt";





//=============================================================================
//		Test Fixture
//-----------------------------------------------------------------------------
NANO_FIXTURE(TURL)
{
	NURL theURL;

	SETUP
	{
		theURL = kTestURL;
	}
};





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "Constructor")
{


	// Perform the test
	NURL urlText("http://www.apple.com/");
	NURL urlString(NString("http://www.apple.com/"));
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "IsValid")
{


	// Perform the test
	theURL = NURL();
	REQUIRE(!theURL.IsValid());

	theURL = kTestURL;
	REQUIRE(theURL.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "Clear")
{


	// Perform the test
	REQUIRE(theURL.IsValid());

	theURL.Clear();
	REQUIRE(!theURL.IsValid());
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "GetURL")
{


	// Perform the test
	REQUIRE(theURL.GetURL() == kTestURL);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "SetURL")
{


	// Perform the test
	REQUIRE(theURL.GetURL() != "http://www.bbbbb.com/");

	theURL.SetURL("http://www.bbbbb.com/");
	REQUIRE(theURL.GetURL() == "http://www.bbbbb.com/");
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "GetScheme")
{


	// Perform the test
	REQUIRE(theURL.GetScheme() == kTestScheme);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "GetHost")
{


	// Perform the test
	REQUIRE(theURL.GetHost() == kTestHost);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "GetPort")
{


	// Perform the test
	REQUIRE(theURL.GetPort() == kTestPort);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "GetPath")
{


	// Perform the test
	REQUIRE(theURL.GetPath() == kTestPath);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "CompareEqual")
{


	// Perform the test
	NURL urlA("http://www.aaaaa.com/");
	NURL urlB("http://www.bbbbb.com/");

	REQUIRE(urlA == urlA);
	REQUIRE(urlB == urlB);
	REQUIRE(urlA != urlB);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "CompareOrder")
{


	// Perform the test
	NURL urlA("http://www.aaaaa.com/");
	NURL urlB("http://www.bbbbb.com/");

	REQUIRE(urlA <= urlA);
	REQUIRE(urlB <= urlB);
	REQUIRE(urlA < urlB);
	REQUIRE(urlB > urlA);
}





//=============================================================================
//		Test Case
//-----------------------------------------------------------------------------
NANO_TEST(TURL, "Format")
{


	// Perform the test
	REQUIRE(NFormat("{}", theURL) == kTestURL);
}
