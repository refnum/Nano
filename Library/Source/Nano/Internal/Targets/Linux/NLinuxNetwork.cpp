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





//============================================================================
//      NTargetNetwork::IsReachable : Is a URL reachable?
//----------------------------------------------------------------------------
#pragma mark -
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
	return(NULL);
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
	return(kNErrPermission);
}



