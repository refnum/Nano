/*	NAME:
		NTargetNetwork.h

	DESCRIPTION:
		Network support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_NETWORK_HDR
#define NTARGET_NETWORK_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NURL.h"
#include "NURLRequest.h"
#include "NURLResponse.h"
#include "NNetworkBrowser.h"
#include "NNetworkService.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetNetwork {
public:
	// Is a URL reachable?
	static bool							IsReachable(const NURL &theURL);


	// URL response
	static NURLResponseRef				URLResponseCreate( NURLResponse   *theResponse);
	static void							URLResponseDestroy(NURLResponseRef theResponse);
	static void							URLResponseStart(  NURLResponseRef theResponse);
	static void							URLResponseCancel( NURLResponseRef theResponse);


	// Services
	static bool							ServicesAvailable(void);
	static NServiceAdvertiserRef		ServiceAdvertiserCreate(const NString &serviceType, UInt16 thePort, const NString &theName);
	static void							ServiceAdvertiserDestroy(NServiceAdvertiserRef theAdvertiser);
	static NServiceBrowserRef			ServiceBrowserCreate(const NString &serviceType, const NNetworkBrowserEventFunctor &theFunctor);
	static void							ServiceBrowserDestroy(NServiceBrowserRef theBrowser);
};





#endif // NTARGET_NETWORK_HDR
