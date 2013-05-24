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
#include "NURLResponse.h"

#include "TURLResponse.h"





//============================================================================
//		TURLResponse::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TURLResponse::Execute(void)
{	NURLResponse		*theResponse;
	NURLRequest			theRequest;
	NData				theData;
	NStatus				theErr;



	// Wait for results
	theRequest  = NURLRequest("http://www.refnum.com/");
	theResponse = new NURLResponse(theRequest);

	theErr = theResponse->WaitForReply(theData);
	NN_ASSERT_NOERR(theErr);
	NN_ASSERT(NString(theData).Contains("refNum Software"));

	delete theResponse;



	// Cancel results
	theRequest  = NURLRequest("http://www.google.com/");
	theResponse = new NURLResponse(theRequest);

	theResponse->Start();
#if NN_TARGET_WINDOWS
	NN_LOG("NURLRequest::Cancel not supported from console apps");
#else
	theResponse->Cancel();
#endif

	delete theResponse;
}




