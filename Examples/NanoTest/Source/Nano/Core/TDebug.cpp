/*	NAME:
		TDebug.cpp

	DESCRIPTION:
		NDebug tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDebug.h"
#include "NTestFixture.h"

#include "CTestUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestMessage									= "Hello World";





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDEBUG(_name, _desc)									NANO_TEST(TDebug, _name, _desc)

NANO_FIXTURE(TDebug)
{
	NDebug		*nDebug;

	SETUP
	{
		nDebug = NDebug::Get();
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDEBUG("Paths", "Path logging")
{


	// Perform the test
	REQUIRE(!nDebug->ShowPath());

	nDebug->SetShowPath(true);
	REQUIRE(nDebug->ShowPath());

	nDebug->SetShowPath(false);
	REQUIRE(!nDebug->ShowPath());
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDEBUG("Output", "Log output")
{	NString	theValue;



	// Perform the test
	CTestUtilities::SetDebugCapture(true);

	nDebug->LogMessage(__FILE__, __LINE__, kTestMessage);
	
	theValue = CTestUtilities::SetDebugCapture(false);
	theValue.Trim();

	REQUIRE(theValue.Contains("TDebug.cpp"));
	REQUIRE(theValue.EndsWith(kTestMessage));
}



