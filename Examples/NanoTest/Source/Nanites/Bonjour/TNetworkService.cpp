/*	NAME:
		TNetworkService.cpp

	DESCRIPTION:
		NNetworkService tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "TNetworkService.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestService									= "_afpovertcp._tcp";
static const UInt16  kTestPort										= 666;

static const NString kTestResult1									= "added: type='_afpovertcp._tcp.', name='";
static const NString kTestResult2									= ".local.', port='666'";





//============================================================================
//		TNetworkService::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TNetworkService::Execute(void)
{	NNetworkService				*theService;
	NServiceRef					theBrowser;
	NServiceBrowserFunctor		theFunctor;
	NString						theResult;
	NStatus						theErr;
	NIndex						n;



	// Get the state we need
	theService = NNetworkService::Get();
	theFunctor = BindFunction(TNetworkService::BrowseEvent, _1, _2, &theResult);



	// Add a service
	theErr = theService->RegisterService(kTestService, kTestPort);
	NN_ASSERT_NOERR(theErr);



	// Browse for services
	theBrowser = theService->AddBrowser(kTestService, theFunctor);
	NN_ASSERT(theBrowser != NULL);

	for (n = 0; n < 10; n++)
		{
		theService->UpdateBrowsers();
		CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1, true);
		}

	if (0)
		NN_LOG(theResult);

	NN_ASSERT(theResult.Contains(kTestResult1));
	NN_ASSERT(theResult.Contains(kTestResult2));



	// Clean up
	theService->RemoveBrowser(theBrowser);
	theService->UnregisterService(kTestService, kTestPort);
}





//============================================================================
//		TNetworkService::BrowseEvent : Handle a browse event.
//----------------------------------------------------------------------------
void TNetworkService::BrowseEvent(const NServiceInfo &theInfo, NServiceEvent theEvent, NString *theResult)
{	NString		theLine;



	// Get the state we need
	theLine.Format("%s: type='%@', name='%@', domain='%@', host='%@', port='%d'\n",
						(theEvent == kNServiceAdded) ? "added" : "removed",
						theInfo.serviceType,
						theInfo.serviceName,
						theInfo.serviceDomain,
						theInfo.hostName,
						theInfo.hostPort);



	// Update the result
	*theResult += theLine;
}





