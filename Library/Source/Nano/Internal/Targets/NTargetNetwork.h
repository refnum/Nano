/*	NAME:
		NTargetNetwork.h

	DESCRIPTION:
		Network support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
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
#include "NSocket.h"





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
	static NServiceAdvertiserRef		ServiceAdvertiserCreate(const NString &serviceType, uint16_t thePort, const NString &theName);
	static void							ServiceAdvertiserDestroy(NServiceAdvertiserRef theAdvertiser);
	static NServiceBrowserRef			ServiceBrowserCreate(const NString &serviceType, const NNetworkBrowserEventFunctor &theFunctor);
	static void							ServiceBrowserDestroy(NServiceBrowserRef theBrowser);


	// Sockets
	static NSocketRef					SocketOpen(     NSocket   *theSocket, const NString &theHost, uint16_t thePort);
	static void							SocketClose(    NSocketRef theSocket);
	static bool							SocketCanRead(  NSocketRef theSocket);
	static bool							SocketCanWrite( NSocketRef theSocket);
	static NIndex						SocketRead(     NSocketRef theSocket, NIndex theSize,       void *thePtr);
	static NIndex						SocketWrite(    NSocketRef theSocket, NIndex theSize, const void *thePtr);
	static int32_t						SocketGetOption(NSocketRef theSocket, NSocketOption theOption);
	static NStatus						SocketSetOption(NSocketRef theSocket, NSocketOption theOption, int32_t theValue);
};





#endif // NTARGET_NETWORK_HDR
