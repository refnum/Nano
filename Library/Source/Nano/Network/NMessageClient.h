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
	// Accept the connection
	//
	// Servers respond to connection requests with a dictionary of their state.
	//
	// Clients should return true/false as they wish to continue connecting to
	// this server, and can provide their own dictionary that the server may use
	// to accept or reject the client in turn.
	//
	// Invoked on an internal thread.
	virtual bool						AcceptConnection(const NDictionary &serverInfo, NDictionary &clientInfo);


	// The client has connected
	//
	// Invoked on an internal thread.
	virtual void						ClientConnected(void);


	// The client has disconnected
	//
	// Clients which fail to connect will return their error through this method.
	//
	// Invoked on an internal thread.
	virtual void						ClientDisconnected(NStatus theErr);


	// The client received a message
	//
	// Unknown messages should be passed to the base class for processing.
	//
	// Invoked on an internal thread.
	virtual void						ReceivedMessage(const NNetworkMessage &theMsg);


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






