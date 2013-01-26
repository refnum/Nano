/*	NAME:
		NMessageServer.cpp

	DESCRIPTION:
		Message server.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"

#include "NMessageServer.h"





//============================================================================
//		NMessageServer::NMessageServer : Constructor.
//----------------------------------------------------------------------------
NMessageServer::NMessageServer(void)
{


	// Initialise ourselves
	SetID(kNEntityServer);
	mSocket.SetDelegate(this);

	mStatus     = kNServerStopped;
	mPort       = 0;
	mMaxClients = 1;
}





//============================================================================
//		NMessageServer::~NMessageServer : Destructor.
//----------------------------------------------------------------------------
NMessageServer::~NMessageServer(void)
{
}





//============================================================================
//		NMessageServer::GetStatus : Get the status.
//----------------------------------------------------------------------------
NMessageServerStatus NMessageServer::GetStatus(void) const
{	StLock	acquireLock(mLock);



	// Get the status
	return(mStatus);
}





//============================================================================
//		NMessageServer::GetPort : Get the port.
//----------------------------------------------------------------------------
UInt16 NMessageServer::GetPort(void) const
{


	// Get the port
	return(mPort);
}





//============================================================================
//		NMessageServer::SetPort : Set the port.
//----------------------------------------------------------------------------
void NMessageServer::SetPort(UInt16 thePort)
{


	// Validate our state
	NN_ASSERT(GetStatus() == kNServerStopped);



	// Set the port
	mPort = thePort;
}





//============================================================================
//		NMessageServer::GetMaxClients : Get the max clients.
//----------------------------------------------------------------------------
NIndex NMessageServer::GetMaxClients(void) const
{


	// Get the max clients
	return(mMaxClients);
}





//============================================================================
//		NMessageServer::SetMaxClients : Set the max clients.
//----------------------------------------------------------------------------
void NMessageServer::SetMaxClients(NIndex maxClients)
{


	// Validate our parameters and state
	NN_ASSERT(maxClients >= 1 && maxClients <= kNEntityClientsMax);

	NN_ASSERT(GetStatus() == kNServerStopped);



	// Set the max clients
	mMaxClients = maxClients;
}





//============================================================================
//		NMessageServer::GetPassword : Get the password.
//----------------------------------------------------------------------------
NString NMessageServer::GetPassword(void) const
{


	// Get the password
	return(mPassword);
}





//============================================================================
//		NMessageServer::SetPassword : Set the password.
//----------------------------------------------------------------------------
void NMessageServer::SetPassword(const NString &thePassword)
{


	// Validate our state
	NN_ASSERT(GetStatus() == kNServerStopped);



	// Set the password
	mPassword = thePassword;
}





//============================================================================
//		NMessageServer::Start : Start the server.
//----------------------------------------------------------------------------
void NMessageServer::Start(void)
{	StLock		acquireLock(mLock);



	// Validate our parameters and state
	NN_ASSERT(mPort       != 0);
	NN_ASSERT(mMaxClients >= 1 && mMaxClients <= kNEntityClientsMax);

	NN_ASSERT(mStatus == kNServerStopped);



	// Start the server
	mStatus = kNServerStarting;

	mSocket.Open(mPort);
}





//============================================================================
//		NMessageServer::Stop : Stop the server.
//----------------------------------------------------------------------------
void NMessageServer::Stop(void)
{


	// Validate our state
	NN_ASSERT(mStatus == kNServerStarted);



	// Stop the server
	mSocket.Close(kNoErr);
}





//============================================================================
//		NMessageServer::SendMessage : Send a message.
//----------------------------------------------------------------------------
void NMessageServer::SendMessage(const NNetworkMessage &theMsg)
{	StLock							acquireLock(mLock);
	NClientInfoMapConstIterator		theIter;
	NEntityID						dstID;



	// Validate our state
	NN_ASSERT(!mClients.empty());



	// Get the state we need
	dstID = theMsg.GetDestination();



	// Send to self
	if (dstID == GetID())
		ReceivedMessage(theMsg);


	// Send to everyone
	else if (dstID == kNEntityEveryone)
		{
		ReceivedMessage(theMsg);

		for (theIter = mClients.begin(); theIter != mClients.end(); theIter++)
			(void) WriteMessage(theIter->second.theSocket, theMsg, false);
		}


	// Send to a client
	//
	// To handle tainted data, we check as well as assert.
	else
		{
		theIter = mClients.find(dstID);
		NN_ASSERT(theIter != mClients.end());

		if (theIter != mClients.end())
			(void) WriteMessage(theIter->second.theSocket, theMsg, false);
		}
}





//============================================================================
//		NMessageServer::GetPendingWrites : Get the amount of pending write data.
//----------------------------------------------------------------------------
NIndex NMessageServer::GetPendingWrites(void) const
{	StLock							acquireLock(mLock);
	NClientInfoMapConstIterator		theIter;
	NIndex							theSize;



	// Check the sockets
	theSize = 0;

	for (theIter = mClients.begin(); theIter != mClients.end(); theIter++)
		theSize += theIter->second.theSocket->GetPendingWrites();
	
	return(theSize);
}





//============================================================================
//		NMessageServer::ServerStarted : The server has started.
//----------------------------------------------------------------------------
void NMessageServer::ServerStarted(void)
{
}





//============================================================================
//		NMessageServer::ServerStopped : The server has stopped.
//----------------------------------------------------------------------------
void NMessageServer::ServerStopped(NStatus /*theErr*/)
{
}





//============================================================================
//		NMessageServer::GetConnectionInfo : Get the connection info.
//----------------------------------------------------------------------------
NDictionary NMessageServer::GetConnectionInfo(void)
{	NDictionary		theInfo;



	// Validate our state
	NN_ASSERT(mLock.IsLocked());



	// Get the info
	theInfo.SetValue(kNMessageMaxClientsKey, mMaxClients);
	theInfo.SetValue(kNMessageNumClientsKey, (NIndex) mClients.size());

	return(theInfo);
}





//============================================================================
//		NMessageServer::AcceptConnection : Accept a connection.
//----------------------------------------------------------------------------
NStatus NMessageServer::AcceptConnection(const NDictionary &/*serverInfo*/, const NDictionary &clientInfo, NEntityID clientID)
{	NString		serverPassword, clientPassword;



	// Check the client state
	if ((NIndex) mClients.size() >= mMaxClients || clientID == kNEntityInvalid)
		return(kNErrBusy);



	// Check the password
	if (!mPassword.IsEmpty())
		{
		clientPassword = clientInfo.GetValueString(kNMessagePasswordKey);
		serverPassword = EncryptPassword(mPassword);

		if (clientPassword != serverPassword)
			return(kNErrLocked);
		}

	return(kNoErr);
}





//============================================================================
//		NMessageServer::ClientConnected : A client has connected.
//----------------------------------------------------------------------------
void NMessageServer::ClientConnected(NEntityID /*clientID*/)
{
}





//============================================================================
//		NMessageServer::ClientDisconnected : A client has disconnected.
//----------------------------------------------------------------------------
void NMessageServer::ClientDisconnected(NEntityID /*clientID*/)
{
}





//============================================================================
//		NMessageServer::ReceivedMessage : The server received a message.
//----------------------------------------------------------------------------
void NMessageServer::ReceivedMessage(const NNetworkMessage &theMsg)
{


	// Validate our parameters
	NN_ASSERT(	theMsg.GetDestination() == GetID() ||
				theMsg.GetDestination() == kNEntityEveryone);

	NN_UNUSED(theMsg);
}





//============================================================================
//		NMessageServer::ProcessMessage : Process a message.
//----------------------------------------------------------------------------
void NMessageServer::ProcessMessage(const NNetworkMessage &theMsg)
{	NEntityID		dstID;



	// Get the state we need
	dstID = theMsg.GetDestination();



	// Process the message
	if (dstID == GetID())
		ReceivedMessage(theMsg);
	else
		SendMessage(theMsg);
}





//============================================================================
//		NMessageServer::SocketDidOpen : A socket has been opened.
//----------------------------------------------------------------------------
void NMessageServer::SocketDidOpen(NSocket *theSocket)
{	StLock		acquireLock(mLock);



	// Start the server
	if (theSocket == &mSocket)
		{
		NN_ASSERT(mStatus == kNServerStarting);

		mStatus = kNServerStarted;
		ServerStarted();
		}
}





//============================================================================
//		NMessageServer::SocketDidClose : A socket has been closed.
//----------------------------------------------------------------------------
void NMessageServer::SocketDidClose(NSocket *theSocket, NStatus theErr)
{	NSocket						*clientSocket;
	NClientInfoMapIterator		theIter;



	// Stop the server
	if (theSocket == &mSocket)
		{
		// Remove the clients
		//
		// Closing the client sockets will cause them to call us back (as we are
		// the socket delegate), which will cause us to remove them from the client
		// list).
		do
			{
			// Get the state we need
			mLock.Lock();
				if (mClients.empty())
					clientSocket = NULL;
				else
					clientSocket = mClients.begin()->second.theSocket;
			mLock.Unlock();


			// Remove the client
			if (clientSocket != NULL)
				clientSocket->Close();
			}
		while (clientSocket != NULL);


		// Stop the server
		mStatus = kNServerStopped;
		ServerStopped(theErr);
		}



	// Remove a client
	//
	// The client may not have been accepted by the server, so we may not actually
	// find its socket in the client list. Clients which never reached the connected
	// state can just be ignored when their socket is closed.
	else
		{
		mLock.Lock();

		for (theIter = mClients.begin(); theIter != mClients.end(); theIter++)
			{
			if (theIter->second.theSocket == theSocket)
				{
				ClientDisconnected(theIter->first);
				RemoveClient(      theIter->first);
				break;
				}
			}

		mLock.Unlock();
		}
}





//============================================================================
//		NMessageServer::SocketHasConnection : The socket has a connection.
//----------------------------------------------------------------------------
NSocketConnectionFunctor NMessageServer::SocketHasConnection(NSocket *theSocket, NSocket *newSocket)
{	StLock							acquireLock(mLock);
	NSocketConnectionFunctor		theFunctor;



	// Validate our parameters and state
	NN_ASSERT(theSocket == &mSocket);
	
	NN_UNUSED(theSocket);



	// Get the functor
	if (mStatus == kNServerStarted)
		theFunctor = BindSelf(NMessageServer::ServerThread, newSocket);
	
	return(theFunctor);
}





//============================================================================
//		NMessageServer::ServerThread : Server thread.
//----------------------------------------------------------------------------
#pragma mark -
void NMessageServer::ServerThread(NSocket *theSocket)
{	NNetworkMessage			msgServerInfo, msgConnectRequest, msgConnectResponse;
	NStatus					theErr, acceptErr;
	NMessageHandshake		clientHandshake;
	NString					thePassword;
	NDictionary				serverInfo;
	bool					addClient;
	NEntityID				clientID;



	// Validate our state
	NN_ASSERT(mStatus == kNServerStarted);



	// Do the handshake
	theErr = ReadHandshake(theSocket, clientHandshake);

	if (theErr == kNoErr)
		theErr = WriteHandshake(theSocket, CreateHandshake());

	if (theErr == kNoErr)
		theErr = ValidateHandshake(clientHandshake);

	if (theErr != kNoErr)
		{
		theSocket->Close(theErr);
		return;
		}



	// Send the server info
	mLock.Lock();
	serverInfo    = GetConnectionInfo();
	msgServerInfo = CreateMessage(kNMessageServerInfoMsg, kNEntityEveryone);
	msgServerInfo.SetProperties(serverInfo);
	mLock.Unlock();

	theErr = WriteMessage(theSocket, msgServerInfo);
	if (theErr != kNoErr)
		{
		theSocket->Close(theErr);
		return;
		}



	// Read the connection request
	//
	// If the client does not wish to continue connecting, they will disconnect.
	theErr = ReadMessage(theSocket, msgConnectRequest);
	if (theErr != kNoErr)
		{
		theSocket->Close();
		return;
		}

	NN_ASSERT(msgConnectRequest.GetType()   == kNMessageConnectRequestMsg);
	NN_ASSERT(msgConnectRequest.GetSource() == kNEntityInvalid);



	// Accept the connection
	mLock.Lock();

	clientID  = GetNextClientID();
	acceptErr = AcceptConnection(serverInfo, msgConnectRequest.GetProperties(), clientID);

	msgConnectResponse = CreateMessage(kNMessageConnectResponseMsg, clientID);
	msgConnectResponse.SetValue(kNMessageStatusKey, acceptErr);

	theErr    = WriteMessage(theSocket, msgConnectResponse);
	addClient = (theErr == kNoErr && acceptErr == kNoErr);
	
	if (addClient)
		AddClient(clientID, theSocket);

	mLock.Unlock();

	if (!addClient)
		{
		theSocket->Close(theErr);
		return;
		}



	// Process messages
	ClientConnected(clientID);

	ProcessMessages(theSocket);
}





//============================================================================
//		NMessageServer::GetNextClientID : Get the next client ID.
//----------------------------------------------------------------------------
NEntityID NMessageServer::GetNextClientID(void)
{	NEntityID	n;



	// Get the next available ID
	for (n = kNEntityClientsFirst; n <= kNEntityClientsLast; n++)
		{
		if (mClients.find(n) == mClients.end())
			return(n);
		}

	return(kNEntityInvalid);
}





//============================================================================
//		NMessageServer::AddClient : Add a client.
//----------------------------------------------------------------------------
void NMessageServer::AddClient(NEntityID clientID, NSocket *theSocket)
{	NClientInfo		clientInfo;



	// Validate our parameters and state
	NN_ASSERT(clientID >= kNEntityClientsFirst && clientID <= kNEntityClientsLast);

	NN_ASSERT(mLock.IsLocked());
	NN_ASSERT(mClients.find(clientID) == mClients.end());



	// Add the client
	//
	// To handle tainted data, we check as well as assert.
	if (clientID != kNEntityInvalid)
		{
		clientInfo.theSocket = theSocket;
		mClients[clientID]   = clientInfo;
		}
}





//============================================================================
//		NMessageServer::RemoveClient : Remove a client.
//----------------------------------------------------------------------------
void NMessageServer::RemoveClient(NEntityID clientID)
{	NClientInfoMapIterator		theIter;



	// Validate our parameters and state
	NN_ASSERT(clientID >= kNEntityClientsFirst && clientID <= kNEntityClientsLast);

	NN_ASSERT(mLock.IsLocked());
	NN_ASSERT(mClients.find(clientID) != mClients.end());



	// Remove the client
	//
	// To handle tainted data, we check as well as assert.
	theIter = mClients.find(clientID);

	if (theIter != mClients.end())
		mClients.erase(theIter);
}








