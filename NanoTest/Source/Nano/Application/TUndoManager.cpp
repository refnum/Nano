/*	NAME:
		TUndoManager.cpp

	DESCRIPTION:
		NUndoManager tests.

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
#include "NUndoManager.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NUNDOMANAGER(...)										TEST_NANO(TUndoManager, ##__VA_ARGS__)

FIXTURE_NANO(TUndoManager)
{
	NUndoManager	undoManager;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNDOMANAGER("Default")
{


	// Perform the test
	REQUIRE(!undoManager.CanUndo());
	REQUIRE(!undoManager.CanRedo());
}





