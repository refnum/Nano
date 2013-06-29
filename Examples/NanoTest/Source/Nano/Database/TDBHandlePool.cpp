/*	NAME:
		TDBHandlePool.cpp

	DESCRIPTION:
		NDBHandlePool tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NDBHandlePool.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NDBHANDLEPOOL(_name, _desc)							TEST_NANO(TDBHandlePool, _name, _desc)

FIXTURE_NANO(TDBHandlePool)
{
	NDBHandlePool	dbPool;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NDBHANDLEPOOL("Default", "Default state")
{


	// Perform the test
	REQUIRE(!dbPool.IsOpen());
}





