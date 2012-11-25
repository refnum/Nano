/*	NAME:
		NMacNetwork.cpp

	DESCRIPTION:
		Mac network support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <SystemConfiguration/SystemConfiguration.h>
#include <dns_sd.h>

#include "NTimer.h"
#include "NCoreFoundation.h"
#include "NCocoa.h"
#include "NCFObject.h"
#include "NByteSwap.h"
#include "NTargetNetwork.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
static const NTime kHeartbeatTime									= 0.1f;





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
// Network services
typedef struct NServiceAdvertiser {
	DNSServiceRef					theService;
} NServiceAdvertiser;


typedef struct NServiceBrowser {
	DNSServiceRef					theService;
	NNetworkBrowserEventFunctor		theFunctor;
	CFSocketRef						cfSocket;
	CFRunLoopSourceRef				cfSource;
} NServiceBrowser;





//============================================================================
//      Internal class declaration
//----------------------------------------------------------------------------
@interface MacURLResponse : NSObject {
@private
	NURLResponse				*mResponse;
	NTimer						*mHeartbeat;
	NSURLConnection				*mConnection;
}

@end





//============================================================================
//      MacURLResponse::initWithResponse : Initialiser
//----------------------------------------------------------------------------
@implementation MacURLResponse

- (id) initWithResponse:(NURLResponse *)theResponse
{	NSMutableURLRequest		*nsRequest;
	NURLRequest				theRequest;
	NString					userAgent;
	NSURL					*nsURL;



	// Create the request
	theRequest = theResponse->GetRequest();
	userAgent  = theRequest.GetValueString(kNURLRequestUserAgentKey);

	nsURL     = ToNS(theRequest.GetURL());
	nsRequest = [[[NSMutableURLRequest alloc] initWithURL:nsURL] autorelease];

	if (!userAgent.IsEmpty())
		[nsRequest setValue:ToNS(userAgent) forHTTPHeaderField:@"User-Agent"];



	// Initialise ourselves
	NSInitSuper(init);
	
	mResponse   = theResponse;
	mHeartbeat  = new NTimer;
	mConnection = [[NSURLConnection alloc] initWithRequest:nsRequest delegate:self startImmediately:NO];



	// Schedule the connection
	//
	// See NTargetThread::ThreadSleep; the response is driven by a private mode on
	// the main run looop, allowing us to run it even if the main thread is slept.
	//
	// However since the main thread may not be slept, we also need a timer to ensure
	// our private mode is executed regularly.
	//
	//
	// This is a little klunky, but the alternatives are equally so.
	//
	// E.g., creating an extra thread to run the NSURLConnection (which creates its
	// own internal thrad anyway, since it's designed to hide threading via the use
	// of a runloop), then spinning that thread or using the synchronous API and not
	// being able to deliver updates as data arrives.
	[mConnection scheduleInRunLoop:[NSRunLoop mainRunLoop] forMode:(NSString *)kNanoRunLoopMode];
	mHeartbeat->AddTimer(BindFunction(CFRunLoopRunInMode, kNanoRunLoopMode, 0.0, true), kHeartbeatTime, kHeartbeatTime);

	return(self);
}





//============================================================================
//      MacURLResponse::dealloc : Deallocator
//----------------------------------------------------------------------------
- (void) dealloc
{


	// Clean up
	[mConnection release];
	delete mHeartbeat;

	[super dealloc];
}





//============================================================================
//      MacURLResponse::start : Start the connection.
//----------------------------------------------------------------------------
- (void) start
{


	// Start the connection
	[mConnection start];
}





//============================================================================
//      MacURLResponse::cancel : Cancel the connection.
//----------------------------------------------------------------------------
- (void) cancel
{


	// Cancel the connection
	[mConnection cancel];
}





//============================================================================
//      MacURLResponse::didReceiveData : Receive data.
//----------------------------------------------------------------------------
- (void) connection:(NSURLConnection *)nsConnection didReceiveData:(NSData *)nsData
{


	// Compiler warning
	NN_UNUSED(nsConnection);



	// Update the delegate
	mResponse->DelegateData([nsData length], [nsData bytes]);
}





//============================================================================
//      MacURLResponse::connectionDidFinishLoading : Handle completion.
//----------------------------------------------------------------------------
- (void) connectionDidFinishLoading:(NSURLConnection *)nsConnection
{


	// Compiler warning
	NN_UNUSED(nsConnection);



	// Update the delegate
	mResponse->DelegateFinished(kNoErr);
}





//============================================================================
//      MacURLResponse::didFailWithError : Handle failure.
//----------------------------------------------------------------------------
- (void) connection:(NSURLConnection *)nsConnection didFailWithError:(NSError *)nsError
{


	// Compiler warning
	NN_UNUSED(nsConnection);



	// Update the delegate
	mResponse->DelegateFinished([nsError code]);
}

@end





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//		NetworkServiceRegisterReply : Handle a register reply.
//----------------------------------------------------------------------------
#pragma mark -
static void NetworkServiceRegisterReply(DNSServiceRef			/*theService*/,
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
static void NetworkServiceResolveReply(	DNSServiceRef			/*theService*/,
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
static void NetworkServiceBrowseReply(	DNSServiceRef			/*theService*/,
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
	theEvent.eventKind = (theFlags & kDNSServiceFlagsAdd) ? kNServiceWasAdded : kNServiceWasRemoved;

	theEvent.serviceType   = NString(regType,     kNStringLength);
	theEvent.serviceName   = NString(serviceName, kNStringLength);
	theEvent.serviceDomain = NString(replyDomain, kNStringLength);
	theEvent.hostName      = "";
	theEvent.hostPort      = 0;



	// Resolve the name
	dnsErr = DNSServiceResolve(	&theResolver,
								0, interfaceIndex,
								serviceName, regType, replyDomain,
								NetworkServiceResolveReply,
								&theEvent);

	if (dnsErr == kDNSServiceErr_NoError)
		{
		dnsErr = DNSServiceProcessResult(theResolver);
		DNSServiceRefDeallocate(         theResolver);

		NN_ASSERT_NOERR(dnsErr);
		}



	// Dispatch the event
	theBrowser->theFunctor(theEvent);
}





//============================================================================
//      NetworkServiceSocketEvent : Handle a socket event.
//----------------------------------------------------------------------------
static void NetworkServiceSocketEvent(	CFSocketRef				/*cfSocket*/,
										CFSocketCallBackType	/*theType*/,
										CFDataRef				/*theAddress*/,
										const void				* /*theData*/,
										void					*userData)
{	NServiceBrowserRef		theBrowser = (NServiceBrowserRef) userData;
	DNSServiceErrorType		dnsErr;



	// Process the event
	dnsErr = DNSServiceProcessResult(theBrowser->theService);
	NN_ASSERT_NOERR(dnsErr);
}





//============================================================================
//      NTargetNetwork::IsReachable : Is a URL reachable?
//----------------------------------------------------------------------------
#pragma mark -
bool NTargetNetwork::IsReachable(const NURL &theURL)
{	bool						isReachable;
	SCNetworkReachabilityRef	scTarget;
	SCNetworkConnectionFlags	scFlags;
	NString						theHost;



	// Get the state we need
	theHost = theURL.GetHost();
	scFlags = 0;



	// Get the flags
	scTarget    = SCNetworkReachabilityCreateWithName(kCFAllocatorNano, theHost.GetUTF8());
	isReachable = (scTarget != NULL);
	
	if (scTarget != NULL)
		{
		SCNetworkReachabilityGetFlags(scTarget, &scFlags);
		CFRelease(scTarget);
		}



	// Check the flags
	if (isReachable)
		isReachable = (bool)  (scFlags & kSCNetworkFlagsReachable);

	if (isReachable)
		isReachable = (bool) !(scFlags & kSCNetworkFlagsConnectionRequired);

	return(isReachable);
}





//============================================================================
//      NTargetNetwork::URLResponseCreate : Create a URL response.
//----------------------------------------------------------------------------
NURLResponseRef NTargetNetwork::URLResponseCreate(NURLResponse *theResponse)
{	MacURLResponse			*macResponse;



	// Create the response
	macResponse = [[MacURLResponse alloc] initWithResponse:theResponse];
	
	return((NURLResponseRef) macResponse);
}





//============================================================================
//      NTargetNetwork::URLResponseDestroy : Destroy a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseDestroy(NURLResponseRef theResponse)
{	MacURLResponse			*macResponse = (MacURLResponse *) theResponse;



	// Destroy the response
	[macResponse release];
}





//============================================================================
//      NTargetNetwork::URLResponseStart : Start a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseStart(NURLResponseRef theResponse)
{	MacURLResponse			*macResponse = (MacURLResponse *) theResponse;



	// Start the response
	[macResponse start];
}





//============================================================================
//      NTargetNetwork::URLResponseCancel : Cancel a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseCancel(NURLResponseRef theResponse)
{	MacURLResponse			*macResponse = (MacURLResponse *) theResponse;



	// Cancel the response
	[macResponse cancel];
}





//============================================================================
//      NTargetNetwork::ServicesAvailable : Are network services available?
//----------------------------------------------------------------------------
bool NTargetNetwork::ServicesAvailable(void)
{


	// Check our state
	return(true);
}





//============================================================================
//      NTargetNetwork::ServiceAdvertiserCreate : Create a service advertiser.
//----------------------------------------------------------------------------
NServiceAdvertiserRef NTargetNetwork::ServiceAdvertiserCreate(const NString &serviceType, UInt16 thePort, const NString &theName)
{	NServiceAdvertiserRef	theAdvertiser;
	DNSServiceErrorType		dnsErr;



	// Create the advertiser
	theAdvertiser = new NServiceAdvertiser;



	// Register the service
	dnsErr = DNSServiceRegister(	&theAdvertiser->theService,
									0, 0,
									theName.IsEmpty() ? NULL : theName.GetUTF8(),
									serviceType.GetUTF8(),
									NULL, NULL,
									NSwapUInt16_NtoB(thePort),
									0, NULL,
									NetworkServiceRegisterReply, theAdvertiser);

	if (dnsErr == kDNSServiceErr_NoError)
		dnsErr = DNSServiceProcessResult(theAdvertiser->theService);



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


	// Destroy the advertiser
	DNSServiceRefDeallocate(theAdvertiser->theService);

	delete theAdvertiser;
}





//============================================================================
//      NTargetNetwork::ServiceBrowserCreate : Create a service browser.
//----------------------------------------------------------------------------
NServiceBrowserRef NTargetNetwork::ServiceBrowserCreate(const NString &serviceType, const NNetworkBrowserEventFunctor &theFunctor)
{	CFSocketContext			socketContext;
	CFOptionFlags			socketFlags;
	NServiceBrowserRef		theBrowser;
	DNSServiceErrorType		dnsErr;



	// Create the browser
	theBrowser             = new NServiceBrowser;
	theBrowser->theService = NULL;
	theBrowser->theFunctor = theFunctor;
	theBrowser->cfSocket   = NULL;
	theBrowser->cfSource   = NULL;
	
	memset(&socketContext, 0x00, sizeof(socketContext));
	socketContext.info = theBrowser;



	// Register the browser
	dnsErr = DNSServiceBrowse(	&theBrowser->theService,
								0, kDNSServiceInterfaceIndexAny,
								serviceType.GetUTF8(), NULL,
								NetworkServiceBrowseReply, theBrowser);



	// Create the socket
	//
	// kCFSocketCloseOnInvalidate is cleared to ensure DNSServiceRef's
	// socket isn't closed when the CFSocket is invalidated.
	if (dnsErr == kDNSServiceErr_NoError)
		{
		theBrowser->cfSocket = CFSocketCreateWithNative(kCFAllocatorNano,
														DNSServiceRefSockFD(theBrowser->theService),
														kCFSocketReadCallBack,
														NetworkServiceSocketEvent,
														&socketContext);

		if (theBrowser->cfSocket != NULL)
			{
			socketFlags = CFSocketGetSocketFlags(theBrowser->cfSocket);
			CFSocketSetSocketFlags(theBrowser->cfSocket, socketFlags & ~kCFSocketCloseOnInvalidate);
			}
		else
			dnsErr = kDNSServiceErr_Unknown;
		}



	// Schedule the socket
	if (dnsErr == kDNSServiceErr_NoError)
		{
		theBrowser->cfSource = CFSocketCreateRunLoopSource(kCFAllocatorNano, theBrowser->cfSocket, 0);

		if (theBrowser->cfSource != NULL)
			CFRunLoopAddSource(CFRunLoopGetMain(), theBrowser->cfSource, kCFRunLoopCommonModes);
		else
			dnsErr = kDNSServiceErr_Unknown;
		}



	// Handle failure
	if (dnsErr != kDNSServiceErr_NoError)
		{
		CFSafeRelease(theBrowser->cfSocket);
		CFSafeRelease(theBrowser->cfSource);

		delete theBrowser;
		theBrowser = NULL;
		}
	
	return(theBrowser);
}





//============================================================================
//      NTargetNetwork::ServiceBrowserDestroy : Destroy a service browser.
//----------------------------------------------------------------------------
void NTargetNetwork::ServiceBrowserDestroy(NServiceBrowserRef theBrowser)
{


	// Invalidate the socket
	//
	// To workaround rdar://3585273 we must give time to CFSocket's select thread
	// so that it can remove the socket from its FD set before we close the socket
	// by calling DNSServiceRefDeallocate.
	CFRunLoopRemoveSource(CFRunLoopGetMain(), theBrowser->cfSource, kCFRunLoopCommonModes);
	CFSocketInvalidate(theBrowser->cfSocket);

	usleep(1000);



	// Destroy the browser
	DNSServiceRefDeallocate(theBrowser->theService);

	CFSafeRelease(theBrowser->cfSocket);
	CFSafeRelease(theBrowser->cfSource);

	delete theBrowser;
}



