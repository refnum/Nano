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
	//
	// Messages are sent asynchronously.
	void								SendMessage(const NNetworkMessage &theMsg);


protected:
	// Handle client events
	//
	// Invoked on an internal thread.
	virtual bool						ClientWillConnect(const NDictionary &serverInfo);
	virtual void						ClientDidConnect(   void);
	virtual void						ClientDidDisconnect(void);
	virtual void						ClientReceivedMessage(const NNetworkMessage &theMsg);
	virtual void						ClientReceivedError(NStatus theErr);


	// Handle message events
	void								ProcessMessage(const NNetworkMessage &theMsg);


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






