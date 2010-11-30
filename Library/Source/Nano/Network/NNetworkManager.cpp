/*	NAME:
		NNetworkManager.cpp

	DESCRIPTION:
		Network manager.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetNetwork.h"
#include "NNetworkManager.h"





//============================================================================
//		NNetworkManager::NNetworkManager : Constructor.
//----------------------------------------------------------------------------
NNetworkManager::NNetworkManager(void)
{
}





//============================================================================
//		NNetworkManager::~NNetworkManager : Destructor.
//----------------------------------------------------------------------------
NNetworkManager::~NNetworkManager(void)
{
}





//============================================================================
//		NNetworkManager::IsReachable : Is a URL reachable?
//----------------------------------------------------------------------------
bool NNetworkManager::IsReachable(const NURL &theURL)
{


	// Check the URL
	return(NTargetNetwork::IsReachable(theURL));
}





//============================================================================
//		NNetworkManager::Get : Get the instance.
//----------------------------------------------------------------------------
NNetworkManager *NNetworkManager::Get(void)
{	static NNetworkManager		sInstance;



	// Get the instance
	return(&sInstance);
}
