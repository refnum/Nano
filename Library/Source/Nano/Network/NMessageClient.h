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
	kNClientConnected,
	kNClientDisconnected,
	kNClientConnecting,
	kNClientDisconnecting
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
	void								SendMessage(const NNetworkMessage &theMsg);


protected:
	// Handle messages
	//
	// Unknown messages must be passed to the base class for processing.
	//
	// Invoked on an internal thread.
	virtual void						ReceivedMessage(const NNetworkMessage &theMsg);


	// Handle client events
	//
	// Once connected, the session will be closed if an error is received.
	//
	// Invoked on an internal thread.
	virtual void						SessionDidOpen( void);
	virtual void						SessionDidClose(void);
	virtual void						SessionReceivedError(NStatus theErr);


	// Handle socket events
	void								SocketDidOpen(      NSocket *theSocket);
	void								SocketDidClose(     NSocket *theSocket);
	void								SocketReceivedError(NSocket *theSocket, NStatus theErr);


private:
	void								ClientThread(NSocket *theSocket);


private:
	mutable NMutexLock					mLock;
	NSocket								mSocket;

	NMessageClientStatus				mStatus;
	NString								mPassword;
};





#endif // NMESSAGECLIENT_HDR






