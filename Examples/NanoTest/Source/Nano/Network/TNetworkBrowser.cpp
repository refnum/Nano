/*	NAME:
		TNetworkBrowser.cpp

	DESCRIPTION:
		NNetworkBrowser tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkService.h"

#include "TNetworkBrowser.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestService									= "_afpovertcp._tcp";
static const UInt16  kTestPort										= 666;

static const NString kTestResult1									= "added: type='_afpovertcp._tcp.', name='";
static const NString kTestResult2									= ".local.', port='666'";





//============================================================================
//		TNetworkBrowser::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TNetworkBrowser::Execute(void)
{	NNetworkBrowser		theBrowser;
	NNetworkService		theService;
	NString				theResult;



	// Prepare the test
	theService.AddService(kTestService, kTestPort);

	theBrowser.SetServices(vector(kTestService));
	theBrowser.SetEventHandler(BindFunction(TNetworkBrowser::BrowseEvent, _1, &theResult));



	// Browse for services
	theBrowser.StartBrowsing();

#if NN_TARGET_MAC
	CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1, false);
#elif NN_TARGET_WINDOWS
	NThread::Sleep(1);
#else
	UNKNOWN TARGET
#endif

	if (0)
		NN_LOG(theResult);

	NN_ASSERT(theResult.Contains(kTestResult1));
	NN_ASSERT(theResult.Contains(kTestResult2));



	// Clean up
	theBrowser.StopBrowsing();
	theService.RemoveServices();
}





//============================================================================
//		TNetworkBrowser::BrowseEvent : Handle a browse event.
//----------------------------------------------------------------------------
void TNetworkBrowser::BrowseEvent(const NNetworkBrowserEvent &theEvent, NString *theResult)
{	NString		theLine;



	// Get the state we need
	theLine.Format("%s: type='%@', name='%@', domain='%@', host='%@', port='%d'\n",
						(theEvent.eventType == kNServiceWasAdded) ? "added" : "removed",
						theEvent.serviceType,
						theEvent.serviceName,
						theEvent.serviceDomain,
						theEvent.hostName,
						theEvent.hostPort);



	// Update the result
	*theResult += theLine;
}


