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

static const NMessageType kTestQuestionMsg							= 1;
static const NMessageType kTestAnswerMsg							= 2;


// Properties
static const NString kTestTokenKey									= "TestToken";

static const NString kTokenRequest									= "Why did the chicken cross the road?";
static const NString kTokenResponse									= "To get to the other side.";


// State
static const NString kStateServerDidStart							= "ServerDidStart/";
static const NString kStateServerDidStop							= "ServerDidStop/";
static const NString kStateServerAddedClient						= "ServerAddedClient/";
static const NString kStateServerRemovedClient						= "ServerRemovedClient/";
static const NString kStateServerReceivedMessage					= "ServerReceivedMessage/";
static const NString kStateServerSentMessage						= "ServerSentMessage/";
static const NString kStateServerReceivedError						= "ServerReceivedError/";

static const NString kStateClientDidOpen							= "ClientDidOpen/";
static const NString kStateClientDidClose							= "ClientDidClose/";
static const NString kStateClientReceivedMessage					= "ClientReceivedMessage/";
static const NString kStateClientSentMessage						= "ClientSentMessage/";
static const NString kStateClientReceivedError						= "ClientReceivedError/";

static const NString kStateServer									= kStateServerDidStart			+
																	  kStateServerAddedClient		+
																	  kStateServerReceivedMessage	+
																	  kStateServerSentMessage		+
																	  kStateServerRemovedClient		+
																	  kStateServerDidStop;

static const NString kStateClient									= kStateClientDidOpen			+
																	  kStateClientSentMessage		+
																	  kStateClientReceivedMessage	+
																	  kStateClientDidClose;





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
	// Handle a message
	void								ReceivedMessage(const NNetworkMessage &theMsg);


	// Handle server events
	void								ServerDidStart(   void);
	void								ServerDidStop(    void);
	void								ServerAddedClient(  NEntityID clientID);
	void								ServerRemovedClient(NEntityID clientID);
	void								ServerReceivedError(NStatus theErr);


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


	// Are we finished?
	bool								IsDone(void) const { return(mIsDone); }


	// Get the state
	NString								GetState(void) const { return(mState); }


protected:
	// Handle a message
	void								ReceivedMessage(const NNetworkMessage &theMsg);


	// Handle client events
	void								SessionDidOpen( void);
	void								SessionDidClose(void);
	void								SessionReceivedError(NStatus theErr);


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
//		TTestServer::ReceivedMessage : Handle a message.
//----------------------------------------------------------------------------
void TTestServer::ReceivedMessage(const NNetworkMessage &theMsg)
{	NString				theValue;
	NNetworkMessage		replyMsg;



	// Update our state
	mState += kStateServerReceivedMessage;



	// Handle the message
	switch (theMsg.GetType()) {
		case kTestQuestionMsg:
			// Check the question
			theValue = theMsg.GetValueString(kTestTokenKey);
			NN_ASSERT(theValue == kTokenRequest);
			
			
			
			// Send a reply
			replyMsg = PrepareMessage(kTestAnswerMsg, theMsg.GetSource());
			replyMsg.SetValue(kTestTokenKey, kTokenResponse);
			
			SendMessage(replyMsg);
			mState += kStateServerSentMessage;
			break;


		default:
			NMessageServer::ReceivedMessage(theMsg);
			break;
		}
}





//============================================================================
//		TTestServer::ServerDidStart : The server has started.
//----------------------------------------------------------------------------
void TTestServer::ServerDidStart(void)
{


	// Update our state
	mState += kStateServerDidStart;
}





//============================================================================
//		TTestServer::ServerDidStop : The server has stopped.
//----------------------------------------------------------------------------
void TTestServer::ServerDidStop(void)
{


	// Update our state
	mState += kStateServerDidStop;
}





//============================================================================
//		TTestServer::ServerAddedClient : The server has added a client.
//----------------------------------------------------------------------------
void TTestServer::ServerAddedClient(NEntityID /*clientID*/)
{


	// Update our state
	mState += kStateServerAddedClient;
}





//============================================================================
//		TTestServer::ServerRemovedClient : The server has removed a client.
//----------------------------------------------------------------------------
void TTestServer::ServerRemovedClient(NEntityID /*clientID*/)
{


	// Update our state
	mState += kStateServerRemovedClient;
}





//============================================================================
//		TTestServer::ServerReceivedError : The server has received an error.
//----------------------------------------------------------------------------
void TTestServer::ServerReceivedError(NStatus theErr)
{


	// Update our state
	NN_ASSERT_NOERR(theErr);

	mState += kStateServerReceivedError;
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
//		TTestClient::ReceivedMessage : Handle a message.
//----------------------------------------------------------------------------
void TTestClient::ReceivedMessage(const NNetworkMessage &theMsg)
{	NString		theValue;



	// Update our state
	mState += kStateClientReceivedMessage;



	// Handle the message
	switch (theMsg.GetType()) {
		case kTestAnswerMsg:
			// Check the answer
			theValue = theMsg.GetValueString(kTestTokenKey);
			NN_ASSERT(theValue == kTokenResponse);
			
			
			// Update our state
			mIsDone = true;
			break;


		default:
			NMessageClient::ReceivedMessage(theMsg);
			break;
		}
}





//============================================================================
//		TTestClient::SessionDidOpen : The session has opened.
//----------------------------------------------------------------------------
void TTestClient::SessionDidOpen(void)
{	NString				theValue;
	NNetworkMessage		theMsg;



	// Update our state
	mState += kStateClientDidOpen;



	// Send the question
	theMsg = PrepareMessage(kTestQuestionMsg, kNEntityServer);
	theMsg.SetValue(kTestTokenKey, kTokenRequest);

	SendMessage(theMsg);
	mState += kStateClientSentMessage;
}





//============================================================================
//		TTestClient::SessionDidClose : The session has closed.
//----------------------------------------------------------------------------
void TTestClient::SessionDidClose(void)
{


	// Update our state
	mState += kStateClientDidClose;
}





//============================================================================
//		TTestClient::SessionReceivedError : The session has received an error.
//----------------------------------------------------------------------------
void TTestClient::SessionReceivedError(NStatus theErr)
{


	// Update our state
	NN_ASSERT_NOERR(theErr);

	mState += kStateClientReceivedError;
}





//============================================================================
//		TMessageServer::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TMessageServer::Execute(void)
{	TTestServer		testServer;
	TTestClient		testClient;
	UInt16			thePort;



	// Get the state we need
	thePort = NMathUtilities::GetRandomUInt16(kServerPortMin, kServerPortMax);



	// Start the server
	testServer.Start(thePort);

	while (testServer.GetStatus() != kNServerStarted)
		NThread::Sleep();



	// Connect the client
	testClient.Connect("127.0.0.1", thePort);

	while (!testClient.IsDone())
		NThread::Sleep();



	// Disconnect the client
	testClient.Disconnect();

	while (testClient.GetStatus() != kNClientDisconnected)
		NThread::Sleep();



	// Stop the server
	testServer.Stop();

	while (testServer.GetStatus() != kNServerStopped)
		NThread::Sleep();



	// Validate our state
	NN_ASSERT(testServer.GetState() == kStateServer);
	NN_ASSERT(testClient.GetState() == kStateClient);
}





