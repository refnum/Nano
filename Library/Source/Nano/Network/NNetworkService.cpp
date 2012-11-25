/*	NAME:
		NNetworkService

	DESCRIPTION:
		Network service advertiser.
	
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
#include "NNetworkService.h"





//============================================================================
//		NNetworkService::NNetworkService : Constructor.
//----------------------------------------------------------------------------
NNetworkService::NNetworkService(void)
{
}





//============================================================================
//		NNetworkService::~NNetworkService : Destructor.
//----------------------------------------------------------------------------
NNetworkService::~NNetworkService(void)
{


	// Clean up
	RemoveServices();
}





//============================================================================
//		NNetworkService::AddService : Add a service.
//----------------------------------------------------------------------------
NStatus NNetworkService::AddService(const NString &serviceType, UInt16 thePort, const NString &theName)
{	NServiceAdvertiserRef	theAdvertiser;
	NStatus					theErr;
	NString					theID;



	// Validate our parameters
	NN_ASSERT(!serviceType.IsEmpty());
	NN_ASSERT(thePort != 0);



	// Get the state we need
	theID         = GetServiceID(serviceType, thePort);
	theAdvertiser = NULL;



	// Check our state
	if (!NTargetNetwork::ServicesAvailable())
		return(kNErrNotSupported);

	if (mServices.find(theID) != mServices.end())
		return(kNErrDuplicate);



	// Add the service
	theAdvertiser = NTargetNetwork::ServiceAdvertiserCreate(serviceType, thePort, theName);
	theErr        = (theAdvertiser != NULL) ? kNoErr : kNErrInternal;
	
	if (theErr == kNoErr)
		mServices[theID] = theAdvertiser;
	
	return(theErr);
}





//============================================================================
//		NNetworkService::RemoveService : Remove a service.
//----------------------------------------------------------------------------
void NNetworkService::RemoveService(const NString &serviceType, UInt16 thePort)
{	NServiceAdvertiserMapIterator	theIter;
	NString							theID;



	// Validate our parameters
	NN_ASSERT(!serviceType.IsEmpty());
	NN_ASSERT(thePort != 0);



	// Get the state we need
	theID   = GetServiceID(serviceType, thePort);
	theIter = mServices.find(theID);
	NN_ASSERT(theIter != mServices.end());



	// Remove the service
	if (theIter != mServices.end())
		NTargetNetwork::ServiceAdvertiserDestroy(theIter->second);

	mServices.erase(theIter);
}





//============================================================================
//		NNetworkService::RemoveServices : Remove the services.
//----------------------------------------------------------------------------
void NNetworkService::RemoveServices(void)
{	NServiceAdvertiserMapConstIterator		theIter;



	// Remove the services
	for (theIter = mServices.begin(); theIter != mServices.end(); theIter++)
		NTargetNetwork::ServiceAdvertiserDestroy(theIter->second);

	mServices.clear();
}





//============================================================================
//		NNetworkService::GetServiceID : Get a service ID.
//----------------------------------------------------------------------------
#pragma mark -
NString NNetworkService::GetServiceID(const NString &serviceType, UInt16 thePort)
{	NString		theID;



	// Validate our parameters
	NN_ASSERT(!serviceType.IsEmpty());
	NN_ASSERT(thePort != 0);



	// Get the ID
	theID.Format("%@:%d", serviceType, thePort);
	
	return(theID);
}





