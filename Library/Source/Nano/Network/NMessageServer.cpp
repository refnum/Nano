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
//		NMessageServer::Start : Start the server.
//----------------------------------------------------------------------------
void NMessageServer::Start(UInt16 thePort, NIndex maxClients, const NString &thePassword)
{	StLock		acquireLock(mLock);



	// Validate our parameters and state
	NN_ASSERT(thePort    != 0);
	NN_ASSERT(maxClients >= 1 && maxClients <= kNEntityClientsMax);

	NN_ASSERT(mStatus == kNServerStopped);



	// Start the server
	//
	// To handle tainted data, we clamp as well as assert.
	mMaxClients = std::min(maxClients, (NIndex) kNEntityClientsMax);
	mPassword   = thePassword;

	mStatus = kNServerStarting;
	mSocket.Open(thePort);
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
		ServerReceivedMessage(theMsg);


	// Send to everyone
	else if (dstID == kNEntityEveryone)
		{
		ServerReceivedMessage(theMsg);

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
//		NMessageServer::ServerDidStart : The server has started.
//----------------------------------------------------------------------------
void NMessageServer::ServerDidStart(void)
{
}





//============================================================================
//		NMessageServer::ServerDidStop : The server has stopped.
//----------------------------------------------------------------------------
void NMessageServer::ServerDidStop(NStatus /*theErr*/)
{
}





//============================================================================
//		NMessageServer::ServerGetProperties : Get the server properties.
//----------------------------------------------------------------------------
NDictionary NMessageServer::ServerGetProperties(void)
{	NDictionary		theProperties;



	// Get the properties
	return(theProperties);
}





//============================================================================
//		NMessageServer::ServerAddedClient : The server has added a client.
//----------------------------------------------------------------------------
void NMessageServer::ServerAddedClient(NEntityID /*clientID*/)
{
}





//============================================================================
//		NMessageServer::ServerRemovedClient : The server has removed a client.
//----------------------------------------------------------------------------
void NMessageServer::ServerRemovedClient(NEntityID /*clientID*/)
{
}





//============================================================================
//		NMessageServer::ServerReceivedMessage : The server received a message.
//----------------------------------------------------------------------------
void NMessageServer::ServerReceivedMessage(const NNetworkMessage &theMsg)
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
		ServerReceivedMessage(theMsg);
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
		ServerDidStart();
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
		ServerDidStop(theErr);
		}



	// Remove a client
	else
		{
		mLock.Lock();

		for (theIter = mClients.begin(); theIter != mClients.end(); theIter++)
			{
			if (theIter->second.theSocket == theSocket)
				{
				ServerRemovedClient(theIter->first);
				RemoveClient(       theIter->first);
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
{	NNetworkMessage			msgServerInfo, msgJoinRequest, msgJoinResponse;
	NMessageHandshake		clientHandshake;
	NString					thePassword;
	NEntityID				clientID;
	NStatus					theErr;



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
	msgServerInfo = CreateMessage(kNMessageServerInfo, kNEntityEveryone);
	msgServerInfo.SetProperties(ServerGetProperties());
	msgServerInfo.SetValue(kNMessageServerMaxClientsKey, mMaxClients);
	msgServerInfo.SetValue(kNMessageServerNumClientsKey, (NIndex) mClients.size());
	mLock.Unlock();

	theErr = WriteMessage(theSocket, msgServerInfo);
	if (theErr != kNoErr)
		{
		theSocket->Close(theErr);
		return;
		}


	// Wait for the client
	//
	// If the client was only interested in the current server state then they
	// will disconnect rather than send a join request.
	theErr = ReadMessage(theSocket, msgJoinRequest);
	if (theErr != kNoErr)
		{
		theSocket->Close(theErr);
		return;
		}
	
	NN_ASSERT(msgJoinRequest.GetType()   == kNMessageJoinRequest);
	NN_ASSERT(msgJoinRequest.GetSource() == kNEntityInvalid);



	// Accept the client
	mLock.Lock();
	
	theErr   = kNoErr;
	clientID = GetNextClientID();
	
	if (msgJoinRequest.GetSource() != kNEntityInvalid)
		theErr = kNErrParam;

	else if ((NIndex) mClients.size() >= mMaxClients || clientID == kNEntityInvalid)
		theErr = kNErrBusy;

	else if (!mPassword.IsEmpty())
		{
		thePassword = msgJoinRequest.GetValueString(kNMessageClientPasswordKey);
	
		if (mPassword != thePassword)
			theErr = kNErrPermission;
		}

	msgJoinResponse = CreateMessage(kNMessageJoinResponse, clientID);
	msgJoinResponse.SetValue(kNMessageStatusKey, theErr);

	theErr = WriteMessage(theSocket, msgJoinResponse);
	if (theErr == kNoErr)
		AddClient(clientID, theSocket);

	mLock.Unlock();
	
	if (theErr != kNoErr)
		{
		theSocket->Close();
		return;
		}



	// Process messages
	ServerAddedClient(clientID);

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








