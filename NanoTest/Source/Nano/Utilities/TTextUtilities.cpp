/*	NAME:
		TTextUtilities.cpp

	DESCRIPTION:
		NTextUtilities tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTextUtilities.h"
#include "NTestFixture.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NString kEntitiesDecoded								= "<&'\"'&>";
static const NString kEntitiesEncoded								= "&lt;&amp;&apos;&quot;&apos;&amp;&gt;";





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NTEXTUTILITIES(...)									TEST_NANO(TTextUtilities, ##__VA_ARGS__)

FIXTURE_NANO(TTextUtilities)
{
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NTEXTUTILITIES("Entities")
{

	// Perform the test
	REQUIRE(NTextUtilities::EncodeEntities(kEntitiesDecoded) == kEntitiesEncoded);
	REQUIRE(NTextUtilities::DecodeEntities(kEntitiesEncoded) == kEntitiesDecoded);
}

