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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "NTargetNetwork.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
static const int kSocketHandleInvalid								= -1;





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// Sockets
typedef struct NSocketInfo {
	NSocket						   *nanoSocket;
	int								nativeSocket;
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
{	struct addrinfo		*theAddress, *addrList;
	int					sysErr, tmpSocket;
	NSocketRef			theSocket;
	const char			*hostName;
	struct addrinfo		addrInfo;
	bool				isListen;
	NString				portNum;



	// Get the state we need
	isListen  = theHost.IsEmpty();
	theAddress = NULL;

	hostName = isListen ? NULL : theHost.GetUTF8();
	portNum.Format("%d",thePort);



	// Create the address
	memset(&addrInfo, 0, sizeof(struct addrinfo));

	addrInfo.ai_family   = AF_UNSPEC;						// Allow IPv4 or IPv6
	addrInfo.ai_socktype = SOCK_STREAM;						// Always TCP
	addrInfo.ai_flags    = isListen ? AI_PASSIVE : 0;

	sysErr = getaddrinfo(hostName, portNum.GetUTF8(), &addrInfo, &addrList);
	NN_ASSERT_NOERR(sysErr);

	if (sysErr != 0)
		return(NULL);



	// Create the socket
	//
	// As we may have multiple addresses, we loop until we find one that we can connect/bind.
	for (theAddress = addrList; theAddress != NULL; theAddress = theAddress->ai_next)
		{
		tmpSocket = socket(theAddress->ai_family, theAddress->ai_socktype, theAddress->ai_protocol);
		if (tmpSocket == kSocketHandleInvalid)
			continue;

		if (isListen)
			{
			if (bind(tmpSocket, theAddress->ai_addr, theAddress->ai_addrlen) == 0)
				break;
			}
		else
			{
			if (connect(tmpSocket, theAddress->ai_addr, theAddress->ai_addrlen) == 0)
				break;
			}

		close(tmpSocket);
		}

	freeaddrinfo(addrList);

	NN_ASSERT(theAddress != NULL);
	if (theAddress == NULL)
		return(NULL);



	// Setup signal handler for closed sockets
	//
	// TO DO
		/*
		sa.sa_handler = sigchld_handler;
		sigemptyset(&sa.sa_mask);

		sa.sa_flags = SA_RESTART;
		NN_ASSERT(sigaction(SIGCHLD, &sa, NULL) != -1);
	*/

	// TODO Create thread for listening/accepting connections



	// Create the socket info
	theSocket = new NSocketInfo;

	theSocket->nanoSocket   = nanoSocket;
	theSocket->nativeSocket = tmpSocket;

	return(theSocket);
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
	NN_ASSERT(theSocket->nativeSocket != kSocketHandleInvalid);



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
	NN_ASSERT(theSocket->nativeSocket != kSocketHandleInvalid);



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


