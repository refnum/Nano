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
#define TEST_NDBHANDLE(...)											TEST_NANO(TDBHandle, ##__VA_ARGS__)

FIXTURE_NANO(TDBHandle)
{
	NDBHandle	dbHandle;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDBHANDLE("Default")
{


	// Perform the test
	REQUIRE(!dbHandle.IsOpen());
}





