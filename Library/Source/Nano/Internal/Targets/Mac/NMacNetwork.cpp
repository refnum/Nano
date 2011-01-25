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

#include "NTimer.h"
#include "NCoreFoundation.h"
#include "NCocoa.h"
#include "NCFObject.h"
#include "NTargetNetwork.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
static const NTime kHeartbeatTime									= 0.1f;





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



