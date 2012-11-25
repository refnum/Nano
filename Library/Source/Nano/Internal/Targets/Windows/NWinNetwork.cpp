/*	NAME:
		NWinNetwork.cpp

	DESCRIPTION:
		Windows network support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <Winhttp.h>

#include "NThread.h"
#include "NWindows.h"
#include "NTargetNetwork.h"





//============================================================================
//		Windows
//----------------------------------------------------------------------------
// Work around Visual Studio header bug:
//
//
//	http://social.msdn.microsoft.com/Forums/en/windowssdk/thread/8f468d9f-3f15-452c-803d-fc63ab3f684e
//
// Wininet.h and Winhttp.h can't be included in the same scope, due to the
// same types being declared twice.
//
// We only need one function from Wininet.h, so declare our own prototype for it.
EXTERN_C DECLSPEC_IMPORT BOOL STDAPICALLTYPE InternetGetConnectedState(LPDWORD lpdwFlags, DWORD dwReserved);





//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------
static const NIndex kHTTP_BufferSize									= 16 * kKilobyte;
static const TCHAR *kHTTP_AllTypes[]									= { L"*/*", NULL };
static const TCHAR *kHTTP_GET											= L"GET";
static const TCHAR *kHTTP_HEAD											= L"HEAD";
static const TCHAR *kHTTP_POST											= L"POST";





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// URL Response
typedef struct {
	NURLResponse				*theResponse;

	HINTERNET					hSession;
	HINTERNET					hConnect;
	HINTERNET					hRequest;

	bool						didCancel;
	UInt8						theBuffer[kHTTP_BufferSize];
} WinURLResponse;


// Network services
typedef struct NServiceAdvertiser {
	void						   *theService;
} NServiceAdvertiser;


typedef struct NServiceBrowser {
	void						   *theService;
	NNetworkBrowserEventFunctor		theFunctor;
} NServiceBrowser;





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//      GetHttpCommand : Get an HTTP command.
//----------------------------------------------------------------------------
static const TCHAR *GetHttpCommand(NURLCommand theCmd)
{	const TCHAR		*theResult;



	// Get the command
	switch (theCmd) {
		case kURLGet:		theResult = kHTTP_GET;		break;
		case kURLHead:		theResult = kHTTP_HEAD;		break;
		case kURLPost:		theResult = kHTTP_POST;		break;
		default:
			NN_LOG("Unknown http command: %d", theCmd);
			theResult = kHTTP_GET;
			break;
		}
	
	return(theResult);
}





//============================================================================
//      WinURLResponseCallback : WinURLResponse callback.
//----------------------------------------------------------------------------
static void CALLBACK WinURLResponseCallback(HINTERNET /*hInternet*/, DWORD_PTR theContext, DWORD theEvent, void * /*statusInfo*/, DWORD dwStatusSize)
{	WinURLResponse			*winResponse = (WinURLResponse *) theContext;
	NData					theData;
	BOOL					wasOK;



	// Handle the event
	switch (theEvent) {
		case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
			// Request the headers
			wasOK = WinHttpReceiveResponse(winResponse->hRequest, NULL);
			NN_ASSERT(wasOK);
			break;


		case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
			// Request the content
			wasOK = WinHttpQueryDataAvailable(winResponse->hRequest, NULL);
			NN_ASSERT(wasOK);
			break;


		case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
			// Request the data
			wasOK = WinHttpReadData(winResponse->hRequest, winResponse->theBuffer, kHTTP_BufferSize, NULL);
			NN_ASSERT(wasOK);
			break;


		case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
			// Read the data
			//
			// The arrival of each chunk of data triggers the read of the next chunk.
			// Once the final chunk of data arrives, the next request will return 0.
			if (dwStatusSize == 0)
				winResponse->theResponse->DelegateFinished(kNoErr);
			else
				{
				NN_ASSERT(dwStatusSize >= 1 && dwStatusSize <= kHTTP_BufferSize);
				winResponse->theResponse->DelegateData(dwStatusSize, winResponse->theBuffer);

				wasOK = WinHttpReadData(winResponse->hRequest, winResponse->theBuffer, kHTTP_BufferSize, NULL);
				NN_ASSERT(wasOK);
				}
			break;


		case WINHTTP_CALLBACK_STATUS_HANDLE_CLOSING:
			// Cancel the response
			//
			// The handle must be cleared first, since invoking the delegate may
			// destroy the response.
			if (winResponse->didCancel)
				{
				winResponse->hRequest = NULL;
				winResponse->theResponse->DelegateFinished(kNErrCancelled);
				}
			break;


		default:
			break;
		}
}





//============================================================================
//      NTargetNetwork::IsReachable : Is a URL reachable?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetNetwork::IsReachable(const NURL &/*theURL*/)
{	DWORD		dwFlags;



	// Check the status
	return(InternetGetConnectedState(&dwFlags, 0) == TRUE);
}





//============================================================================
//      NTargetNetwork::URLResponseCreate : Create a URL response.
//----------------------------------------------------------------------------
NURLResponseRef NTargetNetwork::URLResponseCreate(NURLResponse *theResponse)
{	NString						userAgent, theHost, thePath;
	bool						wasOK, isSecure;
	WinURLResponse				*winResponse;
	WINHTTP_STATUS_CALLBACK		prevCallback;
	const TCHAR					*theCommand;
	NURLRequest					theRequest;
	INTERNET_PORT				thePort;
	NURL						theURL;



	// Get the state we need
	theRequest = theResponse->GetRequest();
	userAgent  = theRequest.GetValueString(kNURLRequestUserAgentKey);
	theCommand = GetHttpCommand(theRequest.GetCommand());

	theURL   = theRequest.GetURL();
	theHost  = theURL.GetHost();
	thePath  = theURL.GetPath();
	thePort  = (INTERNET_PORT) theURL.GetPort();
	isSecure = (theURL.GetScheme() == "https");



	// Create the response
	winResponse = (WinURLResponse *) calloc(1, sizeof(WinURLResponse));
	if (winResponse == NULL)
		return(NULL);

	winResponse->theResponse = theResponse;
	winResponse->didCancel   = false;



	// Create the request
	wasOK                 = false;
	winResponse->hSession = WinHttpOpen(ToWN(userAgent),
										WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
										WINHTTP_NO_PROXY_NAME,
										WINHTTP_NO_PROXY_BYPASS,
										WINHTTP_FLAG_ASYNC);

	if (winResponse->hSession != NULL)
		winResponse->hConnect = WinHttpConnect(winResponse->hSession, ToWN(theHost), thePort, 0);

	if (winResponse->hConnect != NULL)
		winResponse->hRequest = WinHttpOpenRequest(	winResponse->hConnect,
													theCommand,
													ToWN(thePath),
													NULL, WINHTTP_NO_REFERER,
													kHTTP_AllTypes,
													isSecure ? WINHTTP_FLAG_SECURE : 0);

	if (winResponse->hRequest != NULL)
		{
		prevCallback = WinHttpSetStatusCallback(winResponse->hRequest,
												WinURLResponseCallback,
												WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS,
												NULL);

		wasOK = (prevCallback != WINHTTP_INVALID_STATUS_CALLBACK);
		}



	// Handle falure
	if (!wasOK)
		{
		URLResponseDestroy((NURLResponseRef) winResponse);
		winResponse = NULL;
		}

	return((NURLResponseRef) winResponse);
}





//============================================================================
//      NTargetNetwork::URLResponseDestroy : Destroy a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseDestroy(NURLResponseRef theResponse)
{	WinURLResponse			*winResponse = (WinURLResponse *) theResponse;



	// Clean up
	if (winResponse->hRequest != NULL)
		WinHttpCloseHandle(winResponse->hRequest);

	if (winResponse->hConnect != NULL)
		WinHttpCloseHandle(winResponse->hConnect);

	if (winResponse->hSession != NULL)
		WinHttpCloseHandle(winResponse->hSession);
	
	free(winResponse);
}





//============================================================================
//      NTargetNetwork::URLResponseStart : Start a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseStart(NURLResponseRef theResponse)
{	WinURLResponse			*winResponse = (WinURLResponse *) theResponse;
	BOOL					wasOK;



	// Start the response
	wasOK = WinHttpSendRequest(	winResponse->hRequest,
								WINHTTP_NO_ADDITIONAL_HEADERS, 0,
								WINHTTP_NO_REQUEST_DATA,       0, 0,
								(DWORD_PTR) winResponse);
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetNetwork::URLResponseCancel : Cancel a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseCancel(NURLResponseRef theResponse)
{	WinURLResponse			*winResponse = (WinURLResponse *) theResponse;
	BOOL					wasOK;



	// Cancel the response
	winResponse->didCancel = true;
	
	wasOK = WinHttpCloseHandle(winResponse->hRequest);
	NN_ASSERT(wasOK);



	// Wait for it to cancel
	//
	// Cancelling is an async operation, and so we need to wait
	// until the request has been processed before we can return.
	if (wasOK)
		{
		while (winResponse->hRequest != NULL)
			NThread::Sleep();
		}
}





//============================================================================
//      NTargetNetwork::ServicesAvailable : Are network services available?
//----------------------------------------------------------------------------
bool NTargetNetwork::ServicesAvailable(void)
{


	// Check our state
	return(false);
}





//============================================================================
//      NTargetNetwork::ServiceAdvertiserCreate : Create a service advertiser.
//----------------------------------------------------------------------------
NServiceAdvertiserRef NTargetNetwork::ServiceAdvertiserCreate(const NString &serviceType, UInt16 thePort, const NString &theName)
{


	// TO DO
	NN_LOG("NTargetNetwork::ServiceAdvertiserCreate not implemented");
	
	return(NULL);
}





//============================================================================
//      NTargetNetwork::ServiceAdvertiserDestroy : Destroy a service advertiser.
//----------------------------------------------------------------------------
void NTargetNetwork::ServiceAdvertiserDestroy(NServiceAdvertiserRef theAdvertiser)
{


	// TO DO
	NN_LOG("NTargetNetwork::ServiceAdvertiserDestroy not implemented");
}





//============================================================================
//      NTargetNetwork::ServiceBrowserCreate : Create a service browser.
//----------------------------------------------------------------------------
NServiceBrowserRef NTargetNetwork::ServiceBrowserCreate(const NString &serviceType, const NNetworkBrowserEventFunctor &theFunctor)
{


	// TO DO
	NN_LOG("NTargetNetwork::ServiceBrowserCreate not implemented");
	
	return(NULL);
}





//============================================================================
//      NTargetNetwork::ServiceBrowserDestroy : Destroy a service browser.
//----------------------------------------------------------------------------
void NTargetNetwork::ServiceBrowserDestroy(NServiceBrowserRef theBrowser)
{


	// TO DO
	NN_LOG("NTargetNetwork::ServiceBrowserDestroy not implemented");
}



