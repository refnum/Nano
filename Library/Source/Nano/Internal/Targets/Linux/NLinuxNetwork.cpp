/*	NAME:
		NLinuxNetwork.cpp

	DESCRIPTION:
		Linux network support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetNetwork.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Sockets
typedef struct NSocketInfo {
	NSocket *nano_socket;
	int	native_socket;
} NSocketInfo;





//============================================================================
//      NTargetNetwork::IsReachable : Is a URL reachable?
//----------------------------------------------------------------------------
bool NTargetNetwork::IsReachable(const NURL &theURL)
{


	// dair, to do
	NN_LOG("NTargetNetwork::IsReachable not implemented!");
	return(false);
}





//============================================================================
//      NTargetNetwork::URLResponseCreate : Create a URL response.
//----------------------------------------------------------------------------
NURLResponseRef NTargetNetwork::URLResponseCreate(NURLResponse *theResponse)
{


	// dair, to do
	NN_LOG("NTargetNetwork::URLResponseCreate not implemented!");
	return(0);
}





//============================================================================
//      NTargetNetwork::URLResponseDestroy : Destroy a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseDestroy(NURLResponseRef theResponse)
{


	// dair, to do
	NN_LOG("NTargetNetwork::URLResponseDestroy not implemented!");
}





//============================================================================
//      NTargetNetwork::URLResponseStart : Start a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseStart(NURLResponseRef theResponse)
{


	// dair, to do
	NN_LOG("NTargetNetwork::URLResponseStart not implemented!");
}





//============================================================================
//      NTargetNetwork::URLResponseCancel : Cancel a URL response.
//----------------------------------------------------------------------------
void NTargetNetwork::URLResponseCancel(NURLResponseRef theResponse)
{


	// dair, to do
	NN_LOG("NTargetNetwork::URLResponseCancel not implemented!");
}





//============================================================================
//      NTargetNetwork::ServicesAvailable : Are network services available?
//----------------------------------------------------------------------------
bool NTargetNetwork::ServicesAvailable(void)
{


	// dair, to do
	NN_LOG("NTargetNetwork::ServicesAvailable not implemented!");
	return(false);
}





//============================================================================
//      NTargetNetwork::ServiceAdvertiserCreate : Create a service advertiser.
//----------------------------------------------------------------------------
NServiceAdvertiserRef NTargetNetwork::ServiceAdvertiserCreate(const NString &serviceType, UInt16 thePort, const NString &theName)
{


	// dair, to do
	NN_LOG("NTargetNetwork::ServiceAdvertiserCreate not implemented!");
	return(NULL);
}





//============================================================================
//      NTargetNetwork::ServiceAdvertiserDestroy : Destroy a service advertiser.
//----------------------------------------------------------------------------
void NTargetNetwork::ServiceAdvertiserDestroy(NServiceAdvertiserRef theAdvertiser)
{


	// dair, to do
	NN_LOG("NTargetNetwork::ServiceAdvertiserDestroy not implemented!");
}





//============================================================================
//      NTargetNetwork::ServiceBrowserCreate : Create a service browser.
//----------------------------------------------------------------------------
NServiceBrowserRef NTargetNetwork::ServiceBrowserCreate(const NString &serviceType, const NNetworkBrowserEventFunctor &theFunctor)
{


	// dair, to do
	NN_LOG("NTargetNetwork::ServiceBrowserCreate not implemented!");
	return(NULL);
}





//============================================================================
//      NTargetNetwork::ServiceBrowserDestroy : Destroy a service browser.
//----------------------------------------------------------------------------
void NTargetNetwork::ServiceBrowserDestroy(NServiceBrowserRef theBrowser)
{


	// dair, to do
	NN_LOG("NTargetNetwork::ServiceBrowserDestroy not implemented!");
}





//============================================================================
//      NTargetNetwork::SocketOpen : Open a socket.
//----------------------------------------------------------------------------
NSocketRef NTargetNetwork::SocketOpen(NSocket *nanoSocket, const NString &theHost, UInt16 thePort)
{
	NSocketRef socket_ref; // NSocketInfo*
	NString port;
	struct addrinfo hints;
	struct addrinfo *result;
	struct addrinfo *rp;
	int status;
	int tmp_socket;

	memset(&hints, 0, sizeof(struct addrinfo));

	hints.ai_socktype = (true) ? SOCK_DGRAM : SOCK_STREAM;

	port.Format("%d",thePort);

	if (theHost.IsEmpty()) {
		hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
		hints.ai_flags = AI_PASSIVE; // Any interface for Server
		hints.ai_protocol = 0;
		hints.ai_canonname = NULL;
		hints.ai_addr = NULL;
		hints.ai_next = NULL;

		status = getaddrinfo(NULL, port.GetUTF8(), &hints, &result);
	} else {
		hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
		hints.ai_flags = 0;
		hints.ai_protocol = 0;

		status = getaddrinfo(theHost.GetUTF8(), port.GetUTF8(), &hints, &result);
	}

	NN_ASSERT_NOERR(status);

	if (status!=0)
		return (NULL);

	// Loop result until we find an address that will connect or bind
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		tmp_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (tmp_socket == -1)
			continue;

		if (theHost.IsEmpty()){
			if (bind(tmp_socket, rp->ai_addr, rp->ai_addrlen) != -1)
				break; // Success
		} else {
			if (connect(tmp_socket, rp->ai_addr, rp->ai_addrlen) != -1)
				break; // Success
		}

		close(tmp_socket);
	}

	NN_ASSERT(rp!=NULL);

	if (rp == NULL)
		return (NULL);

	// Clean up
	freeaddrinfo(result);

	// Setup signal handler for closed sockets
	/*sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	NN_ASSERT(sigaction(SIGCHLD, &sa, NULL) != -1);*/

	// TODO Create thread for listening/accepting connections

	socket_ref = new NSocketInfo();
	socket_ref->nano_socket=nanoSocket;
	socket_ref->native_socket=tmp_socket;

	return socket_ref;
}





//============================================================================
//      NTargetNetwork::SocketClose : Close a socket.
//----------------------------------------------------------------------------
void NTargetNetwork::SocketClose(NSocketRef theSocket)
{


	// Close the socket
	close(theSocket->nativeSocket);

	delete theSocket;
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
{	NIndex	numRead;



	// Validate our parameters
	NN_ASSERT(theSocket->nativeSocket != -1);



	// Read from the stream
	numRead = (NIndex) read(theSocket->nativeSocket, thePtr, theSize);
	if (numRead < 0)
		numRead = 0;

	return(numRead);
}





//============================================================================
//      NTargetNetwork::SocketWrite : Write to a socket.
//----------------------------------------------------------------------------
NIndex NTargetNetwork::SocketWrite(NSocketRef theSocket, NIndex theSize, const void *thePtr)
{	NIndex		numWritten;



	// Validate our parameters
	NN_ASSERT(theSocket->nativeSocket != -1);



	// Write to the socket
	numWritten = (NIndex) write(theSocket->nativeSocket, thePtr, theSize);
	if (numWritten < 0)
		numWritten = 0;

	return(numWritten);
}





//============================================================================
//      NTargetNetwork::SocketGetOption : Get a socket option.
//----------------------------------------------------------------------------
SInt32 NTargetNetwork::SocketGetOption(NSocketRef theSocket, NSocketOption theOption)
{	socklen_t		valueSize;
	int				valueInt;
	SInt32			theValue;



	// Validate our parameters
	NN_ASSERT(theSocket->nativeSocket != kSocketHandleInvalid);



	// Get the state we need
	theValue = 0;



	// Get the option
	switch (theOption) {
		case kNSocketNoDelay:
			valueInt  = 0;
			valueSize = sizeof(valueInt);

			if (getsockopt(theSocket->nativeSocket, IPPROTO_TCP, TCP_NODELAY, &valueInt, &valueSize) == 0)
				theValue = (valueInt != 0);
			break;

		default:
			NN_LOG("Unknown option: %d", theOption);
			break;
		}
	
	return(theValue);
}





//============================================================================
//      NTargetNetwork::SocketSetOption : Set a socket option.
//----------------------------------------------------------------------------
NStatus NTargetNetwork::SocketSetOption(NSocketRef theSocket, NSocketOption theOption, SInt32 theValue)
{	int			valueInt;
	NStatus		theErr;



	// Validate our parameters
	NN_ASSERT(theSocket->nativeSocket != kSocketHandleInvalid);



	// Get the state we need
	theErr = kNErrNotSupported;



	// Set the option
	switch (theOption) {
		case kNSocketNoDelay:
			valueInt = (theValue != 0) ? 1 : 0;

			if (setsockopt(theSocket->nativeSocket, IPPROTO_TCP, TCP_NODELAY, &valueInt, sizeof(valueInt)) == 0)
				theErr = kNoErr;
			break;

		default:
			NN_LOG("Unknown option: %d", theOption);
			break;
		}
	
	return(theErr);
}


