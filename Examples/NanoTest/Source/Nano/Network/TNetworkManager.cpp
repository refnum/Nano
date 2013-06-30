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
#define TEST_NNETWORKMANAGER(...)									TEST_NANO(TNetworkManager, ##__VA_ARGS__)

FIXTURE_NANO(TNetworkManager)
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
TEST_NNETWORKMANAGER("Connection")
{	NString				theResult;



	// Perform the test
	REQUIRE(netManager->IsReachable("http://www.apple.com/"));
	
	theResult = netManager->GetUserAgent();
	REQUIRE(!theResult.IsEmpty());
}




