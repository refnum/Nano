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
#define TEST_NUNDOSOURCE(_name, _desc)								TEST_NANO(TUndoSource, _name, _desc)

FIXTURE_NANO(TUndoSource)
{
	NUndoSource	undoSource;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNDOSOURCE("Default", "Default state")
{


	// Perform the test
	REQUIRE(undoSource.GetUndoManager() == (NUndoManager *) NULL);
}





