/*	NAME:
		TXMLNode.cpp

	DESCRIPTION:
		NXMLNode tests.

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
#include "NXMLNode.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NXMLNODE(_name, _desc)									NANO_TEST(TXMLNode, _name, _desc)

NANO_FIXTURE(TXMLNode)
{
	NXMLNode	*theNode;
	
	SETUP
	{
		theNode = NULL;
	}
	
	TEARDOWN
	{
		delete theNode;
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NXMLNODE("Default", "Default state")
{


	// Perform the test
	theNode = new NXMLNode(kNXMLNodeDocument, "");
	REQUIRE(theNode->GetParent() == (NXMLNode *) NULL);
	REQUIRE(!theNode->HasChildren());
}


