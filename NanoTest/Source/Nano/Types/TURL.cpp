/*	NAME:
		TURL.cpp

	DESCRIPTION:
		NURL tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTestFixture.h"
#include "NURL.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NURL kTestURL1											= "http://www.apple.com/path/to/file.txt";
static const NURL kTestURL2											= "http://www.apple.com:80/path/to/file.txt";

static const NString kTestScheme									= "http";
static const NString kTestHost										= "www.apple.com";
static const NIndex  kTestPort										= 80;
static const NString kTestPath										= "/path/to/file.txt";





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NURL(...)												TEST_NANO(TURL, ##__VA_ARGS__)

FIXTURE_NANO(TURL)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NURL("Components")
{


	// Perform the test
	REQUIRE(kTestURL1.GetScheme() == kTestScheme);
	REQUIRE(kTestURL1.GetHost()   == kTestHost);
	REQUIRE(kTestURL1.GetPort()   == kTestPort);
	REQUIRE(kTestURL1.GetPath()   == kTestPath);
	
	REQUIRE(kTestURL2.GetScheme() == kTestScheme);
	REQUIRE(kTestURL2.GetHost()   == kTestHost);
	REQUIRE(kTestURL2.GetPort()   == kTestPort);
	REQUIRE(kTestURL2.GetPath()   == kTestPath);
}


