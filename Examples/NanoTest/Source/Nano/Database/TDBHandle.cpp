/*	NAME:
		TDBHandle.cpp

	DESCRIPTION:
		NDBHandle tests.

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
#include "NDBHandle.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDBHANDLE(_name, _desc)								TEST_NANO(TDBHandle, _name, _desc)

FIXTURE_NANO(TDBHandle)
{
	NDBHandle	dbHandle;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDBHANDLE("Default", "Default state")
{


	// Perform the test
	REQUIRE(!dbHandle.IsOpen());
}





