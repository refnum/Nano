/*	NAME:
		NMessageClient.h

	DESCRIPTION:
		Message server client.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMESSAGECLIENT_HDR
#define NMESSAGECLIENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NLock.h"
#include "NMessageEntity.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Status
typedef enum {
	kNClientConnecting,
	kNClientConnected,
	kNClientDisconnected,
} NMessageClientStatus;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMessageClient :	public NMessageEntity,
						public NSocketDelegate {
public:
										NMessageClient(void);
	virtual							   ~NMessageClient(void);


	// Get the status
	NMessageClientStatus				GetStatus(void) const;


	// Connect/disconnect from a server
	void								Connect(const NString &theHost, UInt16 thePort, const NString &thePassword="");
	void								Disconnect(void);


	// Send a message
	//
	// Messages are sent asynchronously.
	void								SendMessage(const NNetworkMessage &theMsg);


protected:
	// The client will connect to the server
	//
	// Servers respond to a connection request with a dictionary that describes
	// their state; clients can provide a corresponding dictionary which the
	// server may use to perform its own acceptance/rejection.
	//
	// Returns true as the connection should continue.
	virtual bool						ClientWillConnect(const NDictionary &serverInfo, NDictionary &clientInfo);


	// The client has connected to the server
	virtual void						ClientDidConnect(void);


	// The client has disconnected from the server
	//
	// Clients which fail to connect will return their error through this method.
	virtual void						ClientDidDisconnect(NStatus theErr);


	// The client has received a message
	//
	// Unknown messages should be passed to the base class for processing.
	virtual void						ClientReceivedMessage(const NNetworkMessage &theMsg);


protected:
	// Handle messages
	void								ProcessMessage(const NNetworkMessage &theMsg);


	// Handle socket events
	void								SocketDidOpen( NSocket *theSocket);
	void								SocketDidClose(NSocket *theSocket, NStatus theErr);


private:
	void								ClientThread(NSocket *theSocket);


private:
	mutable NMutexLock					mLock;
	NSocket								mSocket;

	NMessageClientStatus				mStatus;
	NString								mPassword;
};





#endif // NMESSAGECLIENT_HDR






