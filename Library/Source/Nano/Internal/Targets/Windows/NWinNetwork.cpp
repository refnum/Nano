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
#include <Winsock2.h>

#include "NThread.h"
#include "NWindows.h"
#include "NAtomicList.h"
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





//============================================================================
//		DNS-SD
//----------------------------------------------------------------------------
#define uint16_t													UInt16
#define uint32_t													UInt32
#define int32_t														SInt32

typedef struct _DNSServiceRef_t	*DNSServiceRef;
typedef uint32_t				 DNSServiceFlags;
typedef int32_t					 DNSServiceErrorType;

#define DNSSD_API													__stdcall
#define kDNSServiceErr_NoError										0
#define kDNSServiceInterfaceIndexAny								0
#define kDNSServiceFlagsAdd											0x2

typedef void (DNSSD_API *DNSServiceRegisterReply)
(
	DNSServiceRef						sdRef,
	DNSServiceFlags						flags,
	DNSServiceErrorType					errorCode,
	const char							*name,
	const char							*regtype,
	const char							*domain,
	void								*context
);

typedef void (DNSSD_API *DNSServiceBrowseReply)
(
	DNSServiceRef						sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	DNSServiceErrorType					errorCode,
	const char							*serviceName,
	const char							*regtype,
	const char							*replyDomain,
	void								*context
);

typedef void (DNSSD_API *DNSServiceResolveReply)
(
	DNSServiceRef						sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	DNSServiceErrorType					errorCode,
	const char							*fullname,
	const char							*hosttarget,
	uint16_t							port,
	uint16_t							txtLen,
	const unsigned char					*txtRecord,
	void								*context
);

typedef DNSServiceErrorType (DNSSD_API *DNSServiceRegisterProc)
(
	DNSServiceRef						*sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	const char							*name,
	const char							*regtype,
	const char							*domain,
	const char							*host,
	uint16_t							port,
	uint16_t							txtLen,
	const void							*txtRecord,
	DNSServiceRegisterReply				callBack,
	void								*context
);

typedef DNSServiceErrorType (DNSSD_API *DNSServiceBrowseProc)
(
	DNSServiceRef						*sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	const char							*regtype,
	const char							*domain,
	DNSServiceBrowseReply				callBack,
	void								*context
);

typedef DNSServiceErrorType (DNSSD_API *DNSServiceResolveProc)
(
	DNSServiceRef						*sdRef,
	DNSServiceFlags						flags,
	uint32_t							interfaceIndex,
	const char							*name,
	const char							*regtype,
	const char							*domain,
	DNSServiceResolveReply				callBack,
	void								*context
);

typedef int (DNSSD_API *DNSServiceRefSockFDProc)
(
	DNSServiceRef						sdRef
);

typedef DNSServiceErrorType (DNSSD_API *DNSServiceProcessResultProc)
(
	DNSServiceRef						sdRef
);

typedef void (DNSSD_API *DNSServiceRefDeallocateProc)
(
	DNSServiceRef						sdRef
);





//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------
// URL Response
static const NIndex kHTTP_BufferSize								= 16 * kKilobyte;
static const TCHAR *kHTTP_AllTypes[]								= { L"*/*", NULL };
static const TCHAR *kHTTP_GET										= L"GET";
static const TCHAR *kHTTP_HEAD										= L"HEAD";
static const TCHAR *kHTTP_POST										= L"POST";


// Network services
static const NTime kNServiceBrowseUpdateTime						= 1.1;





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// URL Response
typedef struct {
	NURLResponse				   *theResponse;

	HINTERNET						hSession;
	HINTERNET						hConnect;
	HINTERNET						hRequest;

	bool							didCancel;
	UInt8							theBuffer[kHTTP_BufferSize];
} WinURLResponse;


// Network services
typedef struct NServiceAdvertiser {
	DNSServiceRef					theService;
} NServiceAdvertiser;


typedef struct NServiceBrowser {
	DNSServiceRef					theService;
	NNetworkBrowserEventFunctor		theFunctor;
} NServiceBrowser;

typedef std::vector<NServiceBrowser*>								NServiceBrowserList;
typedef NServiceBrowserList::iterator								NServiceBrowserListListIterator;
typedef NServiceBrowserList::const_iterator							NServiceBrowserListConstIterator;



// Sockets
typedef struct NSocketInfo {
	NSocket						   *theSocket;
} NSocketInfo;





//============================================================================
//      Globals
//----------------------------------------------------------------------------
// Network services
//
// Semi-public to allow for embedded Bonjour usage.
//
// This may be removed in future, in favour of linking a glue version of
// Bonjour into Nano (which would load the DLL if present, or invoke an
// embedded version if not).
//
// The standard Bonjour SDK is difficult to use, due to its requirements of
// MFC and a particular flavour of VC++ runtime library.
DNSServiceRegisterProc		gDNSServiceRegister						= NULL;
DNSServiceBrowseProc		gDNSServiceBrowse						= NULL;
DNSServiceResolveProc		gDNSServiceResolve						= NULL;
DNSServiceRefSockFDProc		gDNSServiceRefSockFD					= NULL;
DNSServiceProcessResultProc	gDNSServiceProcessResult				= NULL;
DNSServiceRefDeallocateProc	gDNSServiceRefDeallocate				= NULL;





//============================================================================
//      Internal globals
//----------------------------------------------------------------------------
// Network services
static NSpinLock			gDNSServiceLock;
static NServiceBrowserList	gDNSServiceBrowsers;
static bool					gDNSServiceInitialised					= false;
static NTimer			   *gDNSServiceTimer						= NULL;





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
//		NetworkServiceUpdateBrowsers : Update the browsers.
//----------------------------------------------------------------------------
//		Note :	The Windows Bonjour API does not provide a run-loop style
//				mechanism for handling events, so we need to either poll or
//				use a blocking window/thread.
//
//				For now we poll, as this allows us to handle multiple browsers
//				without having a window/thread per browser.
//----------------------------------------------------------------------------
static void NetworkServiceUpdateBrowsers(void)
{	int									numFDs, sysErr, theFD;
	struct timeval						theTimeout;
	NServiceBrowserListConstIterator	theIter;
	fd_set								readFDs;
	DNSServiceErrorType					dnsErr;



	// Get the state we need
	memset(&theTimeout, 0x00, sizeof(theTimeout));

	FD_ZERO(&readFDs);
	numFDs = 0;



	// Prepare the file descriptors
	for (theIter = gDNSServiceBrowsers.begin(); theIter != gDNSServiceBrowsers.end(); theIter++)
		{
		// Get the state we need
		theFD = gDNSServiceRefSockFD((*theIter)->theService);
		NN_ASSERT(theFD != -1);


		// Set the FD
		#pragma warning(disable: 4389)
			FD_SET(theFD, &readFDs);
			numFDs = std::max(numFDs, theFD+1);
		#pragma warning(default: 4389)
		}



	// Check for events
	sysErr = select(numFDs, &readFDs, NULL, NULL, &theTimeout);
	
	if (sysErr <= 0)
		return;



	// Process the results
	for (theIter = gDNSServiceBrowsers.begin(); theIter != gDNSServiceBrowsers.end(); theIter++)
		{
		// Get the state we need
		theFD = gDNSServiceRefSockFD((*theIter)->theService);
		NN_ASSERT(theFD != -1);
		
		
		// Process the result
		if (FD_ISSET(theFD, &readFDs))
			{
			dnsErr = gDNSServiceProcessResult((*theIter)->theService);
			NN_ASSERT_NOERR(dnsErr);
			}
		}
}





//============================================================================
//		NetworkServiceRegisterReply : Handle a register reply.
//----------------------------------------------------------------------------
static void DNSSD_API NetworkServiceRegisterReply(	DNSServiceRef			/*theService*/,
													DNSServiceFlags			/*theFlags*/,
													DNSServiceErrorType		/*dnsErr*/,
													const char				* /*serviceName*/,
													const char				* /*regType*/,
													const char				* /*replyDomain*/,
													void					* /*userData*/)
{
}





//============================================================================
//		NetworkServiceResolveReply : Handle a resolve reply.
//----------------------------------------------------------------------------
static void DNSSD_API NetworkServiceResolveReply(	DNSServiceRef			/*theService*/,
													DNSServiceFlags			/*theFlags*/,
													uint32_t				/*interfaceIndex*/,
													DNSServiceErrorType		dnsErr,
													const char				* /*fullName*/,
													const char				*hostName,
													uint16_t				thePort,
													uint16_t				/*textSize*/,
													const unsigned char		* /*textRecord*/,
													void					*userData)
{	NNetworkBrowserEvent	*theEvent = (NNetworkBrowserEvent *) userData;



	// Check our parameters
	if (dnsErr != kDNSServiceErr_NoError)
		return;



	// Update the event
	theEvent->hostName = NString(hostName, kNStringLength);
	theEvent->hostPort = NSwapUInt16_BtoN(thePort);
}





//============================================================================
//		NetworkServiceBrowseReply : Handle a browse reply.
//----------------------------------------------------------------------------
static void DNSSD_API NetworkServiceBrowseReply(	DNSServiceRef			/*theService*/,
													DNSServiceFlags			theFlags,
													uint32_t				interfaceIndex,
													DNSServiceErrorType		dnsErr,
													const char              *serviceName,
													const char              *regType,
													const char              *replyDomain,
													void                    *userData)
{	NServiceBrowserRef			theBrowser = (NServiceBrowserRef) userData;
	DNSServiceRef				theResolver;
	NNetworkBrowserEvent		theEvent;



	// Check our parameters
	if (dnsErr != kDNSServiceErr_NoError)
		return;



	// Prepare the event
	theEvent.eventType = (theFlags & kDNSServiceFlagsAdd) ? kNServiceWasAdded : kNServiceWasRemoved;

	theEvent.serviceType   = NString(regType,     kNStringLength);
	theEvent.serviceName   = NString(serviceName, kNStringLength);
	theEvent.serviceDomain = NString(replyDomain, kNStringLength);
	theEvent.hostName      = "";
	theEvent.hostPort      = 0;



	// Resolve the name
	dnsErr = gDNSServiceResolve(&theResolver,
								0, interfaceIndex,
								serviceName, regType, replyDomain,
								NetworkServiceResolveReply,
								&theEvent);

	if (dnsErr == kDNSServiceErr_NoError)
		{
		dnsErr = gDNSServiceProcessResult(theResolver);
		gDNSServiceRefDeallocate(         theResolver);

		NN_ASSERT_NOERR(dnsErr);
		}



	// Dispatch the event
	theBrowser->theFunctor(theEvent);
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
{	StLock		acquireLock(gDNSServiceLock);
	bool		isAvailable;
	HMODULE		hModule;



	// Load the library
	if (!gDNSServiceInitialised)
		{
		hModule = LoadLibrary(L"dns-sd.dll");
		if (hModule != NULL)
			{
			gDNSServiceRegister      = (DNSServiceRegisterProc)      GetProcAddress(hModule, "DNSServiceRegister");
			gDNSServiceBrowse        = (DNSServiceBrowseProc)        GetProcAddress(hModule, "DNSServiceBrowse");
			gDNSServiceResolve       = (DNSServiceResolveProc)       GetProcAddress(hModule, "DNSServiceResolve");
			gDNSServiceRefSockFD     = (DNSServiceProcessResultProc) GetProcAddress(hModule, "DNSServiceRefSockFD");
			gDNSServiceProcessResult = (DNSServiceProcessResultProc) GetProcAddress(hModule, "DNSServiceProcessResult");
			gDNSServiceRefDeallocate = (DNSServiceRefDeallocateProc) GetProcAddress(hModule, "DNSServiceRefDeallocate");
			}

		gDNSServiceInitialised = true;
		gDNSServiceTimer       = new NTimer;
		}



	// Check our state
	isAvailable = (	gDNSServiceRegister      != NULL &&
					gDNSServiceBrowse        != NULL &&
					gDNSServiceResolve       != NULL &&
					gDNSServiceRefSockFD     != NULL &&
					gDNSServiceProcessResult != NULL &&
					gDNSServiceRefDeallocate != NULL);

	return(isAvailable);
}





//============================================================================
//      NTargetNetwork::ServiceAdvertiserCreate : Create a service advertiser.
//----------------------------------------------------------------------------
NServiceAdvertiserRef NTargetNetwork::ServiceAdvertiserCreate(const NString &serviceType, UInt16 thePort, const NString &theName)
{	NServiceAdvertiserRef	theAdvertiser;
	DNSServiceErrorType		dnsErr;



	// Validate our state
	NN_ASSERT(gDNSServiceInitialised);



	// Create the advertiser
	theAdvertiser = new NServiceAdvertiser;



	// Register the service
	dnsErr = gDNSServiceRegister(	&theAdvertiser->theService,
									0, 0,
									theName.IsEmpty() ? NULL : theName.GetUTF8(),
									serviceType.GetUTF8(),
									NULL, NULL,
									NSwapUInt16_NtoB(thePort),
									0, NULL,
									NetworkServiceRegisterReply, theAdvertiser);

	if (dnsErr == kDNSServiceErr_NoError)
		dnsErr = gDNSServiceProcessResult(theAdvertiser->theService);



	// Handle failure
	if (dnsErr != kDNSServiceErr_NoError)
		{
		delete theAdvertiser;
		theAdvertiser = NULL;
		}
	
	return(theAdvertiser);
}





//============================================================================
//      NTargetNetwork::ServiceAdvertiserDestroy : Destroy a service advertiser.
//----------------------------------------------------------------------------
void NTargetNetwork::ServiceAdvertiserDestroy(NServiceAdvertiserRef theAdvertiser)
{


	// Validate our state
	NN_ASSERT(gDNSServiceInitialised);



	// Destroy the advertiser
	gDNSServiceRefDeallocate(theAdvertiser->theService);

	delete theAdvertiser;
}





//============================================================================
//      NTargetNetwork::ServiceBrowserCreate : Create a service browser.
//----------------------------------------------------------------------------
NServiceBrowserRef NTargetNetwork::ServiceBrowserCreate(const NString &serviceType, const NNetworkBrowserEventFunctor &theFunctor)
{	StLock					acquireLock(gDNSServiceLock);
	NServiceBrowserRef		theBrowser;
	DNSServiceErrorType		dnsErr;



	// Validate our state
	NN_ASSERT(gDNSServiceInitialised);



	// Create the browser
	theBrowser             = new NServiceBrowser;
	theBrowser->theService = NULL;
	theBrowser->theFunctor = theFunctor;



	// Register the browser
	dnsErr = gDNSServiceBrowse(	&theBrowser->theService,
								0, kDNSServiceInterfaceIndexAny,
								serviceType.GetUTF8(), NULL,
								NetworkServiceBrowseReply, theBrowser);



	// Register for events
	if (dnsErr == kDNSServiceErr_NoError)
		{
		gDNSServiceBrowsers.push_back(theBrowser);
		if (gDNSServiceBrowsers.size() == 1)
			{
			NN_ASSERT(!gDNSServiceTimer->HasTimer());
			gDNSServiceTimer->AddTimer(BindFunction(NetworkServiceUpdateBrowsers), kNServiceBrowseUpdateTime);
			}
		}



	// Handle failure
	if (dnsErr != kDNSServiceErr_NoError)
		{
		delete theBrowser;
		theBrowser = NULL;
		}
	
	return(theBrowser);
}





//============================================================================
//      NTargetNetwork::ServiceBrowserDestroy : Destroy a service browser.
//----------------------------------------------------------------------------
void NTargetNetwork::ServiceBrowserDestroy(NServiceBrowserRef theBrowser)
{	StLock		acquireLock(gDNSServiceLock);



	// Validate our state
	NN_ASSERT( gDNSServiceInitialised);
	NN_ASSERT(!gDNSServiceBrowsers.empty());



	// Update our state
	erase(gDNSServiceBrowsers, theBrowser);
	
	if (gDNSServiceBrowsers.empty())
		gDNSServiceTimer->RemoveTimer();



	// Destroy the browser
	gDNSServiceRefDeallocate(theBrowser->theService);

	delete theBrowser;
}





//============================================================================
//      NTargetNetwork::SocketOpen : Open a socket.
//----------------------------------------------------------------------------
NSocketRef NTargetNetwork::SocketOpen(NSocket *socketObject, const NString &theHost, UInt16 thePort)
{


	// dair, to do
	NN_LOG("NTargetNetwork::SocketOpen not implemented!");
	
	return(NULL);
}





//============================================================================
//      NTargetNetwork::SocketClose : Close a socket.
//----------------------------------------------------------------------------
void NTargetNetwork::SocketClose(NSocketRef theSocket)
{


	// dair, to do
	NN_LOG("NTargetNetwork::SocketClose not implemented!");
}





//============================================================================
//      NTargetNetwork::SocketCanRead : Can a socket be read from?
//----------------------------------------------------------------------------
bool NTargetNetwork::SocketCanRead(NSocketRef theSocket)
{


	// dair, to do
	NN_LOG("NTargetNetwork::SocketCanRead not implemented!");
	
	return(false);
}





//============================================================================
//      NTargetNetwork::SocketCanWrite : Can a socket be written to?
//----------------------------------------------------------------------------
bool NTargetNetwork::SocketCanWrite(NSocketRef theSocket)
{


	// dair, to do
	NN_LOG("NTargetNetwork::SocketCanWrite not implemented!");
	
	return(false);
}





//============================================================================
//      NTargetNetwork::SocketRead : Read from a socket.
//----------------------------------------------------------------------------
NIndex NTargetNetwork::SocketRead(NSocketRef theSocket, NIndex theSize, void *thePtr)
{


	// dair, to do
	NN_LOG("NTargetNetwork::SocketRead not implemented!");
	
	return(0);
}





//============================================================================
//      NTargetNetwork::SocketWrite : Write to a socket.
//----------------------------------------------------------------------------
NIndex NTargetNetwork::SocketWrite(NSocketRef theSocket, NIndex theSize, const void *thePtr)
{


	// dair, to do
	NN_LOG("NTargetNetwork::SocketWrite not implemented!");
	
	return(0);
}





//============================================================================
//      NTargetNetwork::SocketGetOption : Get a socket option.
//----------------------------------------------------------------------------
SInt32 NTargetNetwork::SocketGetOption(NSocketRef theSocket, NSocketOption theOption)
{


	// dair, to do
	NN_LOG("NTargetNetwork::SocketGetOption not implemented!");
	
	return(0);
}





//============================================================================
//      NTargetNetwork::SocketSetOption : Set a socket option.
//----------------------------------------------------------------------------
NStatus NTargetNetwork::SocketSetOption(NSocketRef theSocket, NSocketOption theOption, SInt32 theValue)
{


	// dair, to do
	NN_LOG("NTargetNetwork::SocketSetOption not implemented!");
	
	return(kNErrNotSupported);
}




