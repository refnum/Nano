/*	NAME:
		TUTI.cpp

	DESCRIPTION:
		NUTI tests.

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
#include "NFile.h"
#include "NUTI.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NUTI(_name, _desc)										TEST_NANO(TUTI, _name, _desc)

FIXTURE_NANO(TUTI)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUTI("Types", "Types")
{


	// Perform the test
	REQUIRE(NFile("test.gif").GetUTI() == NUTI(kNUTTypeGIF));
	REQUIRE(NFile("test.png").GetUTI() == NUTI(kNUTTypePNG));
	REQUIRE(NFile("test.jpg").GetUTI() == NUTI(kNUTTypeJPEG));
}


