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
{


	// Validate our state
	NN_ASSERT(mStatus == kNClientConnected);



	// Disconnect from the server
	mSocket.Close();
}





//============================================================================
//		NMessageClient::SendMessage : Send a message.
//----------------------------------------------------------------------------
void NMessageClient::SendMessage(const NNetworkMessage &theMsg)
{	NEntityID						dstID;



	// Get the state we need
	dstID = theMsg.GetDestination();



	// Send to self
	if (dstID == GetID())
		ReceivedMessage(theMsg);
	
	
	// Send via the server
	else
		(void) WriteMessage(&mSocket, theMsg, false);
}





//============================================================================
//		NMessageClient::AcceptConnection : Accept the connection.
//----------------------------------------------------------------------------
#pragma mark -
NStatus NMessageClient::AcceptConnection(const NDictionary &/*serverInfo*/, NDictionary &/*clientInfo*/)
{


	// Accept the connection
	return(kNoErr);
}





//============================================================================
//		NMessageClient::ClientConnected : The client has connected.
//----------------------------------------------------------------------------
void NMessageClient::ClientConnected(void)
{
}





//============================================================================
//		NMessageClient::ClientDisconnected : The client has disconnected.
//----------------------------------------------------------------------------
void NMessageClient::ClientDisconnected(NStatus /*theErr*/)
{
}





//============================================================================
//		NMessageClient::ReceivedMessage : The client received a message.
//----------------------------------------------------------------------------
void NMessageClient::ReceivedMessage(const NNetworkMessage &theMsg)
{


	// Validate our parameters
	NN_ASSERT(	theMsg.GetDestination() == GetID() ||
				theMsg.GetDestination() == kNEntityEveryone);

	NN_UNUSED(theMsg);
}





//============================================================================
//		NMessageClient::ProcessMessage : Process a message.
//----------------------------------------------------------------------------
#pragma mark -
void NMessageClient::ProcessMessage(const NNetworkMessage &theMsg)
{	NEntityID		dstID;



	// Validate our parameters and state
	NN_ASSERT(theMsg.GetSource() != kNEntityEveryone);
	NN_ASSERT(theMsg.GetSource() != kNEntityInvalid);



	// Get the state we need
	dstID = theMsg.GetDestination();

	NN_ASSERT(dstID == GetID() || dstID == kNEntityEveryone);



	// Process the message
	//
	// To handle tainted data, we check as well as assert.
	if (dstID == GetID() || dstID == kNEntityEveryone)
		ReceivedMessage(theMsg);
}





//============================================================================
//		NMessageClient::SocketDidOpen : The socket has opened.
//----------------------------------------------------------------------------
void NMessageClient::SocketDidOpen(NSocket *theSocket)
{


	// Validate our parameters
	NN_ASSERT(theSocket == &mSocket);



	// Start the client
	NThreadUtilities::DetachFunctor(BindSelf(NMessageClient::ClientThread, theSocket));
}





//============================================================================
//		NMessageClient::SocketDidClose : The socket has closed.
//----------------------------------------------------------------------------
void NMessageClient::SocketDidClose(NSocket *theSocket, NStatus theErr)
{	StLock		acquireLock(mLock);



	// Validate our parameters and state
	NN_ASSERT(theSocket == &mSocket);
	NN_ASSERT(mStatus  != kNClientDisconnected);

	NN_UNUSED(theSocket);



	// Close the session
	mStatus = kNClientDisconnected;

	ClientDisconnected(theErr);
}





//============================================================================
//		NMessageClient::ClientThread : Client thread.
//----------------------------------------------------------------------------
#pragma mark -
void NMessageClient::ClientThread(NSocket *theSocket)
{	NNetworkMessage			msgServerInfo, msgConnectRequest, msgConnectResponse;
	NMessageHandshake		serverHandshake;
	NDictionary				clientInfo;
	NStatus					theErr;



	// Validate our state
	NN_ASSERT(mStatus == kNClientConnecting);



	// Do the handshake
	theErr = WriteHandshake(theSocket, CreateHandshake());

	if (theErr == kNoErr)
		theErr = ReadHandshake(theSocket, serverHandshake);

	if (theErr == kNoErr)
		theErr = ValidateHandshake(serverHandshake);

	if (theErr != kNoErr)
		{
		theSocket->Close(theErr);
		return;
		}



	// Read the server info
	theErr = ReadMessage(theSocket, msgServerInfo);
	NN_ASSERT(msgServerInfo.GetType() == kNMessageServerInfo);

	if (theErr != kNoErr)
		{
		theSocket->Close(theErr);
		return;
		}



	// Accept the connection
	//
	// Clients which do not wish to proceed may disconnect at this point.
	theErr = AcceptConnection(msgServerInfo.GetProperties(), clientInfo);
	if (theErr != kNoErr)
		{
		theSocket->Close(kNoErr);
		return;
		}



	// Open the connection
	msgConnectRequest = CreateMessage(kNMessageConnectRequest, kNEntityServer);
	msgConnectRequest.SetProperties(clientInfo);
	if (!mPassword.IsEmpty())
		msgConnectRequest.SetValue(kNMessagePasswordKey, EncryptPassword(mPassword));

	theErr = WriteMessage(theSocket, msgConnectRequest);
	if (theErr == kNoErr)
		{
		theErr = ReadMessage(theSocket, msgConnectResponse);
		NN_ASSERT(msgConnectResponse.GetType() == kNMessageConnectResponse);

		if (theErr == kNoErr)
			theErr = msgConnectResponse.GetValueSInt32(kNMessageStatusKey);

		if (theErr == kNoErr)
			{
			SetID(msgConnectResponse.GetDestination());
			NN_ASSERT(GetID() != kNEntityInvalid);
			}
		}

	if (theErr != kNoErr)
		{
		theSocket->Close(theErr);
		return;
		}



	// Process messages
	mStatus = kNClientConnected;
	ClientConnected();

	ProcessMessages(theSocket);
}




