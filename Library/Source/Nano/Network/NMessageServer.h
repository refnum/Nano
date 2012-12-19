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
	kNServerStarted,
	kNServerStopped,
	kNServerStarting,
	kNServerStopping
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
	void								SendMessage(const NNetworkMessage &theMsg);


protected:
	// Handle messages
	//
	// Unknown messages must be passed to the base class for processing.
	//
	// Invoked on an internal thread.
	void								DispatchMessage(const NNetworkMessage &theMsg);
	virtual void						ReceivedMessage(const NNetworkMessage &theMsg);


	// Handle server events
	//
	// Invoked on an internal thread.
	virtual void						ServerDidStart(   void);
	virtual void						ServerDidStop(    void);
	virtual void						ServerAddedClient(  NEntityID clientID);
	virtual void						ServerRemovedClient(NEntityID clientID);
	virtual void						ServerReceivedError(NStatus theErr);


	// Handle socket events
	void								SocketDidOpen(           NSocket *theSocket);
	void								SocketDidClose(          NSocket *theSocket);
	NSocketConnectionFunctor			SocketReceivedConnection(NSocket *theSocket, NSocket *newSocket);
	void								SocketReceivedError(     NSocket *theSocket, NStatus theErr);


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






