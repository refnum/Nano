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
#define TEST_NUNDOMANAGER(_name, _desc)								TEST_NANO(TUndoManager, _name, _desc)

FIXTURE_NANO(TUndoManager)
{
	NUndoManager	undoManager;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNDOMANAGER("Default", "Default state")
{


	// Perform the test
	REQUIRE(!undoManager.CanUndo());
	REQUIRE(!undoManager.CanRedo());
}





