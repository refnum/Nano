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
#include "NMessageServer.h"





//============================================================================
//		NMessageServer::NMessageServer : Constructor.
//----------------------------------------------------------------------------
NMessageServer::NMessageServer(void)
				: mSocket(this)
{


	// Initialise ourselves
	mStatus = kNServerStopped;

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
	NN_ASSERT(maxClients >= 1);

	NN_ASSERT(mStatus == kNServerStopped);



	// Start the server
	mMaxClients = maxClients;
	mPassword   = thePassword;

	mStatus = kNServerStarting;
	mSocket.Open(thePort);
}





//============================================================================
//		NMessageServer::Stop : Stop the server.
//----------------------------------------------------------------------------
void NMessageServer::Stop(void)
{NClientInfoList				theClients;
	NClientInfoListIterator		theIter;



	// Validate our state
	NN_ASSERT(mStatus == kNServerStarted);



	// Update our state
	//
	// The clients list needs to be copied to allow us to close those sockets
	// outside of our lock, to avoid a deadlock whereby the client sockets call
	// us back as their delegate to inform us that they're closed.
	mLock.Lock();
	
	mStatus    = kNServerStopping;
	theClients = mClients;

	mLock.Unlock();



	// Stop the server
	//
	// If we have no clients we can close our socket now, otherwise we close the
	// clients first - once the last client has closed, we close our socket.
	//
	// Once our socket has closed, the server has stopped.
	if (theClients.empty())
		mSocket.Close();
	else
		{
		for (theIter = theClients.begin(); theIter != theClients.end(); theIter++)
			theIter->theSocket->Close();
		}
}





//============================================================================
//		NMessageServer::SendMessage : Send a message.
//----------------------------------------------------------------------------
void NMessageServer::SendMessage(const NNetworkMessage &theMsg)
{	StLock		acquireLock(mLock);
	NStatus		theErr;



	// Validate our state
	NN_ASSERT(!mClients.empty());



	// Send the message
	theErr = WriteMessage(mClients.front().theSocket, theMsg, false);
	NN_ASSERT_NOERR(theErr);
}





//============================================================================
//		NMessageServer::ReceivedMessage : Handle a message.
//----------------------------------------------------------------------------
#pragma mark -
void NMessageServer::ReceivedMessage(const NNetworkMessage &/*theMsg*/)
{
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
void NMessageServer::ServerDidStop(void)
{
}





//============================================================================
//		NMessageServer::ServerAddedClient : The server has added a client.
//----------------------------------------------------------------------------
void NMessageServer::ServerAddedClient(void)
{
}





//============================================================================
//		NMessageServer::ServerReceivedError : The server received an error.
//----------------------------------------------------------------------------
void NMessageServer::ServerReceivedError(NStatus /*theErr*/)
{
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
void NMessageServer::SocketDidClose(NSocket *theSocket)
{	StLock						acquireLock(mLock);
	NClientInfoListIterator		theIter;



	// Stop the server
	if (theSocket == &mSocket)
		{
		NN_ASSERT(mStatus == kNServerStopping);

		mStatus = kNServerStopped;
		ServerDidStop();
		}
	
	
	
	// Remove a client
	else
		{
		// Remove the client
		for (theIter = mClients.begin(); theIter != mClients.end(); theIter++)
			{
			if (theIter->theSocket == theSocket)
				{
				mClients.erase(theIter);
				break;
				}
			}


		// Stop the server
		if (mStatus == kNServerStopping && mClients.empty())
			mSocket.Close();
		}
}





//============================================================================
//		NMessageServer::SocketReceivedConnection : The socket received a connection.
//----------------------------------------------------------------------------
NSocketConnectionFunctor NMessageServer::SocketReceivedConnection(NSocket *theSocket, NSocket *newSocket)
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
//		NMessageServer::SocketReceivedError : A socket received an error.
//----------------------------------------------------------------------------
void NMessageServer::SocketReceivedError(NSocket *theSocket, NStatus theErr)
{	StLock		acquireLock(mLock);



	// Stop the server
	if (theSocket == &mSocket)
		{
		switch (mStatus) {
			case kNServerStarted:
			case kNServerStarting:
				ServerReceivedError(theErr);
				mStatus = kNServerStopping;
				break;

			case kNServerStopped:
			case kNServerStopping:
				// Ignore
				break;

			default:
				NN_LOG("Unknown status: %d", mStatus);
				break;
			}
		}
}





//============================================================================
//		NMessageServer::ServerThread : Server thread.
//----------------------------------------------------------------------------
#pragma mark -
void NMessageServer::ServerThread(NSocket *theSocket)
{	NNetworkMessage			msgServerInfo, msgJoinRequest, msgJoinResponse;
	NMessageHandshake		serverHandshake, clientHandshake;
	NString					thePassword;
	NClientInfo				clientInfo;
	NStatus					theErr;



	// Get the state we need
	serverHandshake = GetHandshake();
	
	memset(&clientHandshake, 0x00, sizeof(clientHandshake));



	// Validate our state
	NN_ASSERT(mStatus == kNServerStarted);



	// Do the handshake
	theErr = ReadHandshake(theSocket, clientHandshake);

	if (theErr == kNoErr)
		theErr = WriteHandshake(theSocket, serverHandshake);

	if (theErr == kNoErr)
		{
		if (clientHandshake.theMagic != kNMessageHandshakeMagic)
			theErr = kNErrMalformed;

		else if (clientHandshake.theVersion > serverHandshake.theVersion)
			theErr = kNErrVersionTooNew;

		else if (clientHandshake.theVersion < serverHandshake.theVersion)
			theErr = kNErrVersionTooOld;
		}

	if (theErr != kNoErr)
		{
		ServerReceivedError(theErr);
		theSocket->Close();
		return;
		}



	// Send the server info
	mLock.Lock();
	msgServerInfo.SetType( kNMessageServerInfo);
	msgServerInfo.SetValue(kNMessageServerMaxClientsKey, mMaxClients);
	msgServerInfo.SetValue(kNMessageServerNumClientsKey, (NIndex) mClients.size());
	mLock.Unlock();

	theErr = WriteMessage(theSocket, msgServerInfo);
	
	if (theErr != kNoErr)
		{
		ServerReceivedError(theErr);
		theSocket->Close();
		return;
		}



	// Wait for the client
	//
	// If the client was only interested in the current server state then they
	// will disconnect rather than send a join request.
	theErr = ReadMessage(theSocket, msgJoinRequest);
	NN_ASSERT(msgJoinRequest.GetType() == kNMessageJoinRequest);

	if (theErr != kNoErr)
		{
		theSocket->Close();
		return;
		}



	// Accept the client
	mLock.Lock();
	
	theErr = kNoErr;

	if ((NIndex) mClients.size() >= mMaxClients)
		theErr = kNErrBusy;

	else if (!mPassword.IsEmpty())
		{
		thePassword = msgJoinRequest.GetValueString(kNMessageClientPasswordKey);
	
		if (mPassword != thePassword)
			theErr = kNErrPermission;
		}

	msgJoinResponse.SetType( kNMessageJoinResponse);
	msgJoinResponse.SetValue(kNMessageStatusKey, theErr);

	theErr = WriteMessage(theSocket, msgJoinResponse);
	
	if (theErr == kNoErr)
		{
		clientInfo.theSocket = theSocket;
		mClients.push_back(clientInfo);
		}

	mLock.Unlock();
	
	if (theErr != kNoErr)
		{
		theSocket->Close();
		return;
		}



	// Open the session
	ServerAddedClient();

	ReceiveMessages(theSocket);
}






