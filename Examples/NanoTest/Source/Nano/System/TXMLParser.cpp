/*	NAME:
		TXMLParser.cpp

	DESCRIPTION:
		NXMLParser tests.

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
#include "NXMLParser.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NXMLPARSER(_name, _desc)								NANO_TEST(TXMLParser, _name, _desc)

NANO_FIXTURE(TXMLParser)
{
	NXMLParser	theParser;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NXMLPARSER("Default", "Default state")
{


	// Perform the test
	REQUIRE(theParser.GetOptions() == kNXMLParserDefault);
}



