/*	NAME:
		TMessageServer.cpp

	DESCRIPTION:
		NMessageServer tests.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NMessageClient.h"

#include "TMessageServer.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Server
static const UInt16 kServerPortMin									= 6000;
static const UInt16 kServerPortMax									= 6999;

static const NMessageType kTestClientBroadcastMsg					= 1;
static const NMessageType kTestClientServerMsg						= 2;
static const NMessageType kTestServerClientMsg						= 3;


// Properties
static const NString kTestTokenKey									= "TestToken";

static const NString kTokenClientBroadcast							= "Incoming Joke!";
static const NString kTokenClientServer								= "Why did the chicken cross the road?";
static const NString kTokenServerClient								= "To get to the other side.";


// State
static const NString kStateServerStarted							= "ServerStarted/";
static const NString kStateServerStopped							= "ServerStopped:%ld/";
static const NString kStateServerClientConnected					= "ServerClientConnected/";
static const NString kStateServerClientDisconnected					= "ServerClientDisconnected/";
static const NString kStateServerReceivedMessage					= "ServerReceivedMessage/";
static const NString kStateServerSentMessage						= "ServerSentMessage/";

static const NString kStateClientConnected							= "ClientConnected/";
static const NString kStateClientDisconnected						= "ClientDisconnected:%ld/";
static const NString kStateClientReceivedMessage					= "ClientReceivedMessage/";
static const NString kStateClientSentMessage						= "ClientSentMessage/";

static const NString kStateServer									= kStateServerStarted				+
																	  kStateServerClientConnected		+
																	  kStateServerClientConnected		+
																	  kStateServerReceivedMessage		+
																	  kStateServerReceivedMessage		+
																	  kStateServerSentMessage			+
																	  kStateServerClientDisconnected	+
																	  kStateServerClientDisconnected	+
																	  NFormatString(kStateServerStopped, kNoErr);

static const NString kStateClient1									= kStateClientConnected				+
																	  kStateClientSentMessage			+
																	  kStateClientSentMessage			+
																	  kStateClientReceivedMessage		+
																	  kStateClientReceivedMessage		+
																	  NFormatString(kStateClientDisconnected, kNoErr);

static const NString kStateClient2									= kStateClientConnected				+
																	  kStateClientReceivedMessage		+
																	  NFormatString(kStateClientDisconnected, kNoErr);





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class TTestServer : public NMessageServer {
public:
										TTestServer(void);
	virtual							   ~TTestServer(void);


	// Get the state
	NString								GetState(void) const { return(mState); }


protected:
	// Handle server events
	void								ServerStarted(void);
	void								ServerStopped(NStatus theErr);
	void								ClientConnected(   NEntityID clientID);
	void								ClientDisconnected(NEntityID clientID);
	void								ReceivedMessage(const NNetworkMessage &theMsg);


private:
	NString								mState;
};





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class TTestClient : public NMessageClient {
public:
										TTestClient(void);
	virtual							   ~TTestClient(void);


	// Execute the test
	void								Execute(void);


	// Get the state
	NString								GetState(void) const { return(mState); }


protected:
	// Handle client events
	void								ClientConnected(void);
	void								ClientDisconnected(NStatus theErr);
	void								ReceivedMessage(const NNetworkMessage &theMsg);


private:
	bool								mIsDone;
	NString								mState;
};





//============================================================================
//		TTestServer::TTestServer : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
TTestServer::TTestServer(void)
{
}





//============================================================================
//		TTestServer::~TTestServer : Destructor.
//----------------------------------------------------------------------------
TTestServer::~TTestServer(void)
{


	// Validate our state
	NN_ASSERT(GetStatus() == kNServerStopped);
}





//============================================================================
//		TTestServer::ServerStarted : The server has started.
//----------------------------------------------------------------------------
void TTestServer::ServerStarted(void)
{


	// Update our state
	mState += kStateServerStarted;
}





//============================================================================
//		TTestServer::ServerStopped : The server has stopped.
//----------------------------------------------------------------------------
void TTestServer::ServerStopped(NStatus theErr)
{


	// Update our state
	mState += NFormatString(kStateServerStopped, theErr);
}





//============================================================================
//		TTestServer::ClientConnected : A client has connected.
//----------------------------------------------------------------------------
void TTestServer::ClientConnected(NEntityID /*clientID*/)
{


	// Update our state
	mState += kStateServerClientConnected;
}





//============================================================================
//		TTestServer::ClientDisconnected : A client has disconnected.
//----------------------------------------------------------------------------
void TTestServer::ClientDisconnected(NEntityID /*clientID*/)
{


	// Update our state
	mState += kStateServerClientDisconnected;
}





//============================================================================
//		TTestServer::ReceivedMessage : The server received a message.
//----------------------------------------------------------------------------
void TTestServer::ReceivedMessage(const NNetworkMessage &theMsg)
{	NString				theValue;
	NNetworkMessage		replyMsg;



	// Update our state
	mState += kStateServerReceivedMessage;



	// Handle the message
	switch (theMsg.GetType()) {
		case kTestClientBroadcastMsg:
			// Validate the message
			theValue = theMsg.GetValueString(kTestTokenKey);
			NN_ASSERT(theValue == kTokenClientBroadcast);
			break;


		case kTestClientServerMsg:
			// Validate the message
			theValue = theMsg.GetValueString(kTestTokenKey);
			NN_ASSERT(theValue == kTokenClientServer);


			// Send a response
			replyMsg = CreateMessage(kTestServerClientMsg, theMsg.GetSource());
			replyMsg.SetValue(kTestTokenKey, kTokenServerClient);
			
			SendMessage(replyMsg);
			mState += kStateServerSentMessage;
			break;


		default:
			NMessageServer::ReceivedMessage(theMsg);
			break;
		}
}





//============================================================================
//		TTestClient::TTestClient : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
TTestClient::TTestClient(void)
{


	// Initialise ourselves
	mIsDone = false;
}





//============================================================================
//		TTestClient::~TTestClient : Destructor.
//----------------------------------------------------------------------------
TTestClient::~TTestClient(void)
{


	// Validate our state
	NN_ASSERT(GetStatus() == kNClientDisconnected);
}





//============================================================================
//		TTestClient::Execute : Execute the test.
//----------------------------------------------------------------------------
void TTestClient::Execute(void)
{	NString				theValue;
	NNetworkMessage		theMsg;



	// Send a message to everyone
	theMsg = CreateMessage(kTestClientBroadcastMsg, kNEntityEveryone);
	theMsg.SetValue(kTestTokenKey, kTokenClientBroadcast);

	SendMessage(theMsg);
	mState += kStateClientSentMessage;



	// Send a message to the server
	theMsg = CreateMessage(kTestClientServerMsg, kNEntityServer);
	theMsg.SetValue(kTestTokenKey, kTokenClientServer);

	SendMessage(theMsg);
	mState += kStateClientSentMessage;



	// Wait for a response
	while (!mIsDone)
		NThread::Sleep();
}





//============================================================================
//		TTestClient::ClientConnected : The client has connected.
//----------------------------------------------------------------------------
void TTestClient::ClientConnected(void)
{


	// Update our state
	mState += kStateClientConnected;
}





//============================================================================
//		TTestClient::ClientDisconnected : The client was disconnected.
//----------------------------------------------------------------------------
void TTestClient::ClientDisconnected(NStatus theErr)
{


	// Update our state
	mState += NFormatString(kStateClientDisconnected, theErr);
}





//============================================================================
//		TTestClient::ReceivedMessage : The client has received a message.
//----------------------------------------------------------------------------
void TTestClient::ReceivedMessage(const NNetworkMessage &theMsg)
{	NString		theValue;



	// Update our state
	mState += kStateClientReceivedMessage;



	// Handle the message
	switch (theMsg.GetType()) {
		case kTestClientBroadcastMsg:
			// Validate the message
			theValue = theMsg.GetValueString(kTestTokenKey);
			NN_ASSERT(theValue == kTokenClientBroadcast);
			break;


		case kTestServerClientMsg:
			// Validate the message
			theValue = theMsg.GetValueString(kTestTokenKey);
			NN_ASSERT(theValue == kTokenServerClient);
			
			// Update our state
			mIsDone = true;
			break;


		default:
			NMessageClient::ReceivedMessage(theMsg);
			break;
		}
}





//============================================================================
//		TMessageServer::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TMessageServer::Execute(void)
{	TTestClient		testClient1, testClient2;
	TTestServer		testServer;
	UInt16			thePort;



	// Get the state we need
	thePort = NMathUtilities::GetRandomUInt16(kServerPortMin, kServerPortMax);



	// Start the server
	testServer.StartServer(thePort, 2);

	while (testServer.GetStatus() != kNServerStarted)
		NThread::Sleep();



	// Connect the clients
	testClient1.Connect("127.0.0.1", thePort);
	testClient2.Connect("127.0.0.1", thePort);

	while (	testClient1.GetStatus() != kNClientConnected &&
			testClient2.GetStatus() != kNClientConnected)
		NThread::Sleep();

	testClient1.Execute();



	// Disconnect the client1
	testClient1.Disconnect();
	testClient2.Disconnect();

	while (	testClient1.GetStatus() != kNClientDisconnected &&
			testClient2.GetStatus() != kNClientDisconnected)
		NThread::Sleep();



	// Stop the server
	testServer.StopServer();

	while (testServer.GetStatus() != kNServerStopped)
		NThread::Sleep();



	// Validate our state
	NN_ASSERT( testServer.GetState() == kStateServer);
	NN_ASSERT(testClient1.GetState() == kStateClient1);
	NN_ASSERT(testClient2.GetState() == kStateClient2);
}





