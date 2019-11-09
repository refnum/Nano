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
#define TEST_NXMLPARSER(...)										TEST_NANO(TXMLParser, ##__VA_ARGS__)

FIXTURE_NANO(TXMLParser)
{
	NXMLParser	theParser;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NXMLPARSER("Default")
{


	// Perform the test
	REQUIRE(theParser.GetOptions() == kNXMLParserDefault);
}



