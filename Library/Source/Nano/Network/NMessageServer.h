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


	// Get/set the port
	UInt16								GetPort(void) const;
	void								SetPort(UInt16 thePort);
	
	
	// Get/set the max clients
	//
	// Servers can support between 1 and kNEntityClientsMax clients.
	NIndex								GetMaxClients(void) const;
	void								SetMaxClients(NIndex maxClients);


	// Get/set the password
	NString								GetPassword(void) const;
	void								SetPassword(const NString &thePassword);


	// Start/stop the server
	void								Start(void);
	void								Stop( void);


	// Send a message
	//
	// Messages are sent asynchronously.
	void								SendMessage(const NNetworkMessage &theMsg);


	// Get the amount of pending write data
	//
	// Returns the number of bytes written, but not yet dispatched.
	NIndex								GetPendingWrites(void) const;


protected:
	// The server has started/stopped
	//
	// Invoked on an internal thread.
	virtual void						ServerStarted(void);
	virtual void						ServerStopped(NStatus theErr);


	// Get the connection info
	//
	// Servers respond to connection requests with a dictionary of their state.
	//
	// Sub-classes should call through before adding custom values.
	//
	// Invoked on an internal thread.
	virtual NDictionary					GetConnectionInfo(void);


	// Accept a connection
	//
	// After accepting the server's connection info, clients which wish to continue
	// connecting provide their own dictionary that the server may use to accept or
	// reject the client in turn.
	//
	// Sub-classes should call through before performing their own validation.
	//
	// Invoked on an internal thread.
	virtual NStatus						AcceptConnection(const NDictionary &serverInfo, const NDictionary &clientInfo, NEntityID clientID);


	// A client has connected/disconnected
	//
	// Invoked on an internal thread.
	virtual void						ClientConnected(   NEntityID clientID);
	virtual void						ClientDisconnected(NEntityID clientID);


	// The server received a message
	//
	// Sub-classes should call through with unknown messages.
	//
	// Invoked on an internal thread.
	virtual void						ReceivedMessage(const NNetworkMessage &theMsg);


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
	void								DisconnectClient(NEntityID clientID);
	
	void								AddClient(   NEntityID clientID, NSocket *theSocket);
	void								RemoveClient(NEntityID clientID);


private:
	mutable NMutexLock					mLock;

	NMessageServerStatus				mStatus;
	UInt16								mPort;
	NIndex								mMaxClients;
	NString								mPassword;

	NSocket								mSocket;
	NClientInfoMap						mClients;
};






#endif // NMESSAGESERVER_HDR






