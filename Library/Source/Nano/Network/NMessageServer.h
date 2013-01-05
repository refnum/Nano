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
	void								StartServer(UInt16 thePort, NIndex maxClients=1, const NString &thePassword="");
	void								StopServer(void);


	// Send a message
	//
	// Messages are sent asynchronously.
	void								SendMessage(const NNetworkMessage &theMsg);


protected:
	// The server has started
	//
	// Invoked on an internal thread.
	virtual void						ServerStarted(void);


	// The server has stopped
	//
	// Invoked on an internal thread.
	virtual void						ServerStopped(NStatus theErr);


	// Get the connection info
	//
	// Servers respond to connection requests with a dictionary of their state.
	//
	// Invoked on an internal thread.
	virtual NDictionary					GetConnectionInfo(void);


	// Accept a connection
	//
	// After accepting the server's connection info, clients which wish to continue
	// connecting provide their own dictionary that the server may use to accept or
	// reject the client in turn.
	//
	// Invoked on an internal thread.
	virtual NStatus						AcceptConnection(const NDictionary &serverInfo, const NDictionary &clientInfo, NEntityID clientID);


	// A client has connected
	//
	// Invoked on an internal thread.
	virtual void						ClientConnected(NEntityID clientID);


	// A client has disconnected
	//
	// Invoked on an internal thread.
	virtual void						ClientDisconnected(NEntityID clientID);


	// The server received a message
	//
	// Unknown messages should be passed to the base class for processing.
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






