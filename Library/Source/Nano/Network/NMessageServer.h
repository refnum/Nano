/*	NAME:
		NMessageServer.h

	DESCRIPTION:
		Message server.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NMESSAGESERVER_HDR
#define NMESSAGESERVER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMessageEntity.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Status
typedef enum {
	kNServerStarting,
	kNServerStarted,
	kNServerStopped,
} NMessageServerStatus;





//============================================================================
//      Types
//----------------------------------------------------------------------------
// Client info
typedef struct {
	NSocket		*theSocket;
} NClientInfo;


// Lists
typedef std::map<NEntityID, NClientInfo>							NClientInfoMap;
typedef NClientInfoMap::iterator									NClientInfoMapIterator;
typedef NClientInfoMap::const_iterator								NClientInfoMapConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NMessageServer :	public NMessageEntity,
						public NSocketDelegate {
public:
										NMessageServer(void);
	virtual							   ~NMessageServer(void);


	// Get the status
	NMessageServerStatus				GetStatus(void) const;


	// Start/stop the server
	void								Start(UInt16 thePort, NIndex maxClients=1, const NString &thePassword="");
	void								Stop( void);


	// Send a message
	//
	// Messages are sent asynchronously.
	void								SendMessage(const NNetworkMessage &theMsg);


protected:
	// The server has started
	//
	// Invoked on an internal thread.
	virtual void						ServerDidStart(void);


	// The server has stopped
	//
	// Invoked on an internal thread.
	virtual void						ServerDidStop(NStatus theErr);


	// Get the server connection state
	//
	// Servers respond to a connection request with a dictionary of their state.
	//
	// Invoked on an internal thread.
	virtual NDictionary					ServerConnectionState(void);


	// The server has a connection request
	//
	// Clients provide a dictionary of their state as part of the connection.
	//
	// Invoked on an internal thread.
	virtual NStatus						ServerConnectionRequest(const NDictionary &serverInfo, const NDictionary &clientInfo, NEntityID clientID);


	// The server has added a client.
	//
	// Invoked on an internal thread.
	virtual void						ServerAddedClient(NEntityID clientID);


	// The server has removed a client.
	//
	// Invoked on an internal thread.
	virtual void						ServerRemovedClient(NEntityID clientID);


	// The server has received a message
	//
	// Unknown messages should be passed to the base class for processing.
	//
	// Invoked on an internal thread.
	virtual void						ServerReceivedMessage(const NNetworkMessage &theMsg);


protected:
	// Process messages
	void								ProcessMessage(const NNetworkMessage &theMsg);


	// Handle socket events
	void								SocketDidOpen(      NSocket *theSocket);
	void								SocketDidClose(     NSocket *theSocket, NStatus theErr);
	NSocketConnectionFunctor			SocketHasConnection(NSocket *theSocket, NSocket *newSocket);


private:
	void								ServerThread(NSocket *theSocket);

	NEntityID							GetNextClientID(void);
	void								AddClient(   NEntityID clientID, NSocket *theSocket);
	void								RemoveClient(NEntityID clientID);


private:
	mutable NMutexLock					mLock;
	NSocket								mSocket;

	NMessageServerStatus				mStatus;
	NString								mPassword;
	NIndex								mMaxClients;
	NClientInfoMap						mClients;
};






#endif // NMESSAGESERVER_HDR






