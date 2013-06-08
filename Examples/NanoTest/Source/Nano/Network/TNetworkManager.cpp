/*	NAME:
		TNetworkManager.cpp

	DESCRIPTION:
		NNetworkManager tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkManager.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NNETWORKMANAGER(_name, _desc)							NANO_TEST(TNetworkManager, _name, _desc)

NANO_FIXTURE(TNetworkManager)
{
	NNetworkManager		*netManager;
	
	SETUP
	{
		netManager = NNetworkManager::Get();
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NNETWORKMANAGER("Connection", "Connection test")
{	NString				theResult;



	// Perform the test
	REQUIRE(netManager->IsReachable("http://www.apple.com/"));
	
	theResult = netManager->GetUserAgent();
	REQUIRE(!theResult.IsEmpty());
}




