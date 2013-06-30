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
#include "NTestFixture.h"
#include "NUndoSource.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NUNDOSOURCE(...)										TEST_NANO(TUndoSource, ##__VA_ARGS__)

FIXTURE_NANO(TUndoSource)
{
	NUndoSource	undoSource;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNDOSOURCE("Default")
{


	// Perform the test
	REQUIRE(undoSource.GetUndoManager() == (NUndoManager *) NULL);
}





