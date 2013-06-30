/*	NAME:
		TNetworkBrowser.cpp

	DESCRIPTION:
		NNetworkBrowser tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NNetworkService.h"
#include "NTestFixture.h"

#include "CTestUtilities.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kTestService									= "_afpovertcp._tcp";
static const UInt16  kTestPort										= 666;

static const NString kTestResult1									= "added: type='_afpovertcp._tcp.', name='";
static const NString kTestResult2									= ".local.', port='666'";





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NNETWORKBROWSER(...)									TEST_NANO(TNetworkBrowser, ##__VA_ARGS__)

FIXTURE_NANO(TNetworkBrowser)
{
};





//============================================================================
//		BrowseEvent : Handle a browse event.
//----------------------------------------------------------------------------
static void BrowseEvent(const NNetworkBrowserEvent &theEvent, NString *theResult)
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





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NNETWORKBROWSER("State")
{	NNetworkBrowser		theBrowser;
	NNetworkService		theService;
	NString				theResult;
	NStatus				theErr;



	// Prepare the service
	//
	// Bonjour may not be available, in which case the tests are skipped.
	theErr = theService.AddService( kTestService, kTestPort);
	if (theErr == kNErrNotSupported)
		{
		NN_LOG("TNetworkBrowser not supported on this platform");
		return;
		}

	REQUIRE(theService.HasService(kTestService, kTestPort));
	REQUIRE_NOERR(theErr);



	// Prepare the browser
	theBrowser.SetServices(nvector(kTestService));
	theBrowser.SetEventHandler(BindFunction(BrowseEvent, _1, &theResult));



	// Browse for services
	theBrowser.StartBrowsing();
	CTestUtilities::ExecuteRunloop(2);

	if (0)
		NN_LOG(theResult);

	REQUIRE(theResult.Contains(kTestResult1));
	REQUIRE(theResult.Contains(kTestResult2));



	// Clean up
	theBrowser.StopBrowsing();
	theService.RemoveServices();
}




