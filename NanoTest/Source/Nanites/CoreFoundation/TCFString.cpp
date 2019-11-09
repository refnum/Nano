/*	NAME:
		TCFString.cpp

	DESCRIPTION:
		NCFString tests.

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
#include "NCFString.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString     kTestNString								= "Hello World";
static const CFStringRef kTestCFString								= CFSTR("Hello World");





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NCFSTRING(...)											TEST_NANO(TCFString, ##__VA_ARGS__)

FIXTURE_NANO(TCFString)
{
	NCFString		theString;
	NCFObject		cfString;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
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


