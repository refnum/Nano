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

#include "NCocoa.h"
#include "NCFObject.h"
#include "NTargetNetwork.h"





//============================================================================
//      Internal class declaration
//----------------------------------------------------------------------------
@interface MacURLResponse : NSObject {
@private
	NURLResponse				*mResponse;
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
	mConnection = [[NSURLConnection alloc] initWithRequest:nsRequest delegate:self startImmediately:NO];

	[mConnection scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];

	return(self);
}





//============================================================================
//      MacURLResponse::dealloc : Deallocator
//----------------------------------------------------------------------------
- (void) dealloc
{


	// Clean up
	[mConnection release];
	
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



