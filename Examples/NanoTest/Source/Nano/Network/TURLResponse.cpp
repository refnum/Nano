/*	NAME:
		TURLResponse.cpp

	DESCRIPTION:
		NURLResponse tests.

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
#include "NURLResponse.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NURLRESPONSE(_name, _desc)								TEST_NANO(TURLResponse, _name, _desc)

FIXTURE_NANO(TURLResponse)
{
	NURLResponse	*theResponse;
	NURLRequest		theRequest;

	SETUP
	{
		theRequest  = NURLRequest("http://www.refnum.com/");
		theResponse = new NURLResponse(theRequest);
	}

	TEARDOWN
	{
		delete theResponse;
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NURLRESPONSE("Normal", "Normal request")
{	NData		theData;
	NStatus		theErr;



	// Perform the test
	theErr = theResponse->WaitForReply(theData);
	REQUIRE_NOERR(theErr);
	REQUIRE(NString(theData).Contains("refNum Software"));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NURLRESPONSE("Cancel", "Cancel a request")
{


	// Perform the test
	theResponse->Start();
#if NN_TARGET_WINDOWS
	NN_LOG("NURLRequest::Cancel not supported from console apps");
#else
	theResponse->Cancel();
#endif
}




