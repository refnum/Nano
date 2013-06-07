/*	NAME:
		TUndoSource.cpp

	DESCRIPTION:
		NUndoSource tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUndoSource.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NUNDOSOURCE(_name, _desc)								NANO_TEST(TUndoSource, _name, _desc)

NANO_FIXTURE(TUndoSource)
{
	NUndoSource	undoSource;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNDOSOURCE("Default", "Default state")
{


	// Perform the test
	REQUIRE(undoSource.GetUndoManager() == NULL);
}





