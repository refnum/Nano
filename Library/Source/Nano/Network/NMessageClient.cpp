/*	NAME:
		NMessageClient.cpp

	DESCRIPTION:
		Message server client.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadUtilities.h"
#include "NMessageClient.h"





//============================================================================
//		NMessageClient::NMessageClient : Constructor.
//----------------------------------------------------------------------------
NMessageClient::NMessageClient(void)
{


	// Initialise ourselves
	mSocket.SetDelegate(this);

	mStatus = kNClientDisconnected;
}





//============================================================================
//		NMessageClient::~NMessageClient : Destructor.
//----------------------------------------------------------------------------
NMessageClient::~NMessageClient(void)
{
}





//============================================================================
//		NMessageClient::GetStatus : Get the status.
//----------------------------------------------------------------------------
NMessageClientStatus NMessageClient::GetStatus(void) const
{	StLock	acquireLock(mLock);



	// Get the status
	return(mStatus);
}





//============================================================================
//		NMessageClient::Connect : Connect to a server.
//----------------------------------------------------------------------------
void NMessageClient::Connect(const NString &theHost, UInt16 thePort, const NString &thePassword)
{	StLock		acquireLock(mLock);



	// Validate our parameters and state
	NN_ASSERT(!theHost.IsEmpty());
	NN_ASSERT(thePort != 0);
	
	NN_ASSERT(mStatus == kNClientDisconnected);



	// Connect to the server
	mStatus   = kNClientConnecting;
	mPassword = thePassword;

	mSocket.Open(theHost, thePort);
}





//============================================================================
//		NMessageClient::Disconnect : Disconnect from a server.
//----------------------------------------------------------------------------
void NMessageClient::Disconnect(void)
{	StLock		acquireLock(mLock);



	// Validate our state
	NN_ASSERT(mStatus == kNClientConnected);



	// Disconnect from the server
	mStatus = kNClientDisconnecting;
	mSocket.Close();
}





//============================================================================
//		NMessageClient::SendMessage : Send a message.
//----------------------------------------------------------------------------
void NMessageClient::SendMessage(const NNetworkMessage &theMsg)
{


	// Send the message
	(void) WriteMessage(&mSocket, theMsg, false);
}





//============================================================================
//		NMessageClient::ReceivedMessage : Handle a message.
//----------------------------------------------------------------------------
#pragma mark -
void NMessageClient::ReceivedMessage(const NNetworkMessage &theMsg)
{


	// Validate our parameters
	NN_ASSERT(	theMsg.GetDestination() == GetID() ||
				theMsg.GetDestination() == kNEntityEveryone);

	NN_UNUSED(theMsg);
}





//============================================================================
//		NMessageClient::ClientWillConnect : The client will connect to the server.
//----------------------------------------------------------------------------
bool NMessageClient::ClientWillConnect(const NDictionary &/*serverInfo*/)
{


	// Continue connecting
	return(true);
}





//============================================================================
//		NMessageClient::ClientDidConnect : The client did connect to the server.
//----------------------------------------------------------------------------
void NMessageClient::ClientDidConnect(void)
{
}





//============================================================================
//		NMessageClient::ClientDidDisconnect : The client was disconnected.
//----------------------------------------------------------------------------
void NMessageClient::ClientDidDisconnect(void)
{
}





//============================================================================
//		NMessageClient::ClientReceivedError : The client received an error.
//----------------------------------------------------------------------------
void NMessageClient::ClientReceivedError(NStatus /*theErr*/)
{
}





//============================================================================
//		NMessageClient::SocketDidOpen : The socket has been opened.
//----------------------------------------------------------------------------
void NMessageClient::SocketDidOpen(NSocket *theSocket)
{


	// Validate our parameters
	NN_ASSERT(theSocket == &mSocket);



	// Start the client
	NThreadUtilities::DelayFunctor(BindSelf(NMessageClient::ClientThread, theSocket), 0.0, false);
}





//============================================================================
//		NMessageClient::SocketDidClose : The socket has been closed.
//----------------------------------------------------------------------------
void NMessageClient::SocketDidClose(NSocket *theSocket)
{	StLock		acquireLock(mLock);



	// Validate our parameters
	NN_ASSERT(theSocket == &mSocket);

	NN_UNUSED(theSocket);



	// Close the session
	switch (mStatus) {
		case kNClientConnected:
		case kNClientConnecting:
		case kNClientDisconnecting:
			mStatus = kNClientDisconnected;
			ClientDidDisconnect();
			break;

		case kNClientDisconnected:
			NN_ASSERT("Client socket disconnected twice!");
			break;
		
		default:
			NN_LOG("Unknown status: %d", mStatus);
			break;
		}
}





//============================================================================
//		NMessageClient::SocketReceivedError : The socket received an error.
//----------------------------------------------------------------------------
void NMessageClient::SocketReceivedError(NSocket *theSocket, NStatus theErr)
{	StLock		acquireLock(mLock);



	// Validate our parameters
	NN_ASSERT(theSocket == &mSocket);

	NN_UNUSED(theSocket);
	
	

	// Close the session
	switch (mStatus) {
		case kNClientConnected:
		case kNClientConnecting:
			mStatus = kNClientDisconnecting;
			ClientReceivedError(theErr);
			break;

		case kNClientDisconnected:
		case kNClientDisconnecting:
			// Ignore
			break;

		default:
			NN_LOG("Unknown status: %d", mStatus);
			break;
		}
}





//============================================================================
//		NMessageClient::ClientThread : Client thread.
//----------------------------------------------------------------------------
#pragma mark -
void NMessageClient::ClientThread(NSocket *theSocket)
{	NNetworkMessage			msgServerInfo, msgJoinRequest, msgJoinResponse;
	NMessageHandshake		clientHandshake, serverHandshake;
	NStatus					theErr;



	// Validate our state
	NN_ASSERT(mStatus == kNClientConnecting);



	// Get the state we need
	clientHandshake = CreateHandshake();
	
	memset(&serverHandshake, 0x00, sizeof(serverHandshake));



	// Do the handshake
	theErr = WriteHandshake(theSocket, clientHandshake);

	if (theErr == kNoErr)
		theErr = ReadHandshake(theSocket, serverHandshake);

	if (theErr == kNoErr)
		{
		if (serverHandshake.theMagic != kNMessageHandshakeMagic)
			theErr = kNErrMalformed;

		else if (serverHandshake.theVersion > clientHandshake.theVersion)
			theErr = kNErrVersionTooNew;

		else if (serverHandshake.theVersion < clientHandshake.theVersion)
			theErr = kNErrVersionTooOld;
		}
	
	if (theErr != kNoErr)
		{
		ClientReceivedError(theErr);
		theSocket->Close();
		return;
		}



	// Read the server info
	theErr = ReadMessage(theSocket, msgServerInfo);
	NN_ASSERT(msgServerInfo.GetType() == kNMessageServerInfo);

	if (theErr != kNoErr)
		{
		ClientReceivedError(theErr);
		theSocket->Close();
		return;
		}



	// Connect to the server
	if (!ClientWillConnect(msgServerInfo.GetProperties()))
		{
		theSocket->Close();
		return;
		}

	msgJoinRequest = CreateMessage(kNMessageJoinRequest, kNEntityServer);

	if (!mPassword.IsEmpty())
		msgJoinRequest.SetValue(kNMessageClientPasswordKey, mPassword);

	theErr = WriteMessage(theSocket, msgJoinRequest);
	if (theErr == kNoErr)
		{
		theErr = ReadMessage(theSocket, msgJoinResponse);
		NN_ASSERT(msgJoinResponse.GetType() == kNMessageJoinResponse);

		if (theErr == kNoErr)
			theErr = msgJoinResponse.GetValueSInt32(kNMessageStatusKey);

		if (theErr == kNoErr)
			{
			SetID(msgJoinResponse.GetDestination());
			NN_ASSERT(GetID() != kNEntityInvalid);
			}
		}

	if (theErr != kNoErr)
		{
		ClientReceivedError(theErr);
		theSocket->Close();
		return;
		}



	// Open the session
	mStatus = kNClientConnected;
	ClientDidConnect();

	ProcessMessages(theSocket);
}




