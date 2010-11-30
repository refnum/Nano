/*	NAME:
		TNetworkManager.cpp

	DESCRIPTION:
		NNetworkManager tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkManager.h"

#include "TNetworkManager.h"





//============================================================================
//		TNetworkManager::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TNetworkManager::Execute(void)
{	NNetworkManager		*netManager;



	// Get the state we need
	netManager = NNetworkManager::Get();



	// Execute the tests
	NN_ASSERT(netManager->IsReachable("http://www.apple.com/"));
}



