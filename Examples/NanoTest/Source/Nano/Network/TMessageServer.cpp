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
#include "NMessageClient.h"

#include "TMessageServer.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const NMessageType kTestMessageQuestion						= 1;
static const NMessageType kTestMessageAnswer						= 2;

static const NString kTestMessageValueKey							= "TestValue";


// Values
static const UInt16 kValueServerPort								= 6027;

static const NString kTestValueQuestion								= "Why did the chicken cross the road?";
static const NString kTestValueAnswer								= "To get to the other side.";





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class TTestServer : public NMessageServer {
public:
										TTestServer(void);
	virtual							   ~TTestServer(void);


protected:
	// Handle a message
	void								ReceivedMessage(const NNetworkMessage &theMsg);


	// Handle server events
	void								ServerDidStart(   void);
	void								ServerDidStop(    void);
	void								ServerAddedClient(void);
	void								ServerReceivedError(NStatus theErr);


private:
	NIndex								mStateNumStarts;
	NIndex								mStateNumStops;
	NIndex								mStateNumMsgs;
	NIndex								mStateNumClients;
	NStatus								mStateError;
};





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class TTestClient : public NMessageClient {
public:
										TTestClient(void);
	virtual							   ~TTestClient(void);


	// Are we finished?
	bool								IsDone(void) const;


protected:
	// Handle a message
	void								ReceivedMessage(const NNetworkMessage &theMsg);


	// Handle client events
	void								SessionDidOpen( void);
	void								SessionDidClose(void);
	void								SessionReceivedError(NStatus theErr);


private:
	bool								mIsDone;

	NIndex								mStateNumOpens;
	NIndex								mStateNumCloses;
	NIndex								mStateNumMsgs;
	NStatus								mStateError;
};





//============================================================================
//		TTestServer::TTestServer : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
TTestServer::TTestServer(void)
{


	// Initialise ourselves
	mStateNumStarts  = 0;
	mStateNumStops   = 0;
	mStateNumMsgs    = 0;
	mStateNumClients = 0;
	mStateError      = kNoErr;
}





//============================================================================
//		TTestServer::~TTestServer : Destructor.
//----------------------------------------------------------------------------
TTestServer::~TTestServer(void)
{


	// Validate our state
	NN_ASSERT(GetStatus() == kNServerStopped);
	
	NN_ASSERT(mStateNumStarts  == 1);
	NN_ASSERT(mStateNumStops   == 1);
	NN_ASSERT(mStateNumMsgs    == 1);
	NN_ASSERT(mStateNumClients == 1);
	NN_ASSERT_NOERR(mStateError);
}





//============================================================================
//		TTestServer::ReceivedMessage : Handle a message.
//----------------------------------------------------------------------------
void TTestServer::ReceivedMessage(const NNetworkMessage &theMsg)
{	NString				theValue;
	NNetworkMessage		replyMsg;



	// Update our state
	mStateNumMsgs++;



	// Handle the message
	switch (theMsg.GetType()) {
		case kTestMessageQuestion:
			// Check the question
			theValue = theMsg.GetValueString(kTestMessageValueKey);
			NN_ASSERT(theValue == kTestValueQuestion);
			
			
			
			// Send a reply
			replyMsg.SetType( kTestMessageAnswer);
			replyMsg.SetValue(kTestMessageValueKey, kTestValueAnswer);
			
			SendMessage(replyMsg);
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
	mStateNumStarts++;
}





//============================================================================
//		TTestServer::ServerDidStop : The server has stopped.
//----------------------------------------------------------------------------
void TTestServer::ServerDidStop(void)
{


	// Update our state
	mStateNumStops++;
}





//============================================================================
//		TTestServer::ServerAddedClient : The server has added a client.
//----------------------------------------------------------------------------
void TTestServer::ServerAddedClient(void)
{


	// Update our state
	mStateNumClients++;
}





//============================================================================
//		TTestServer::ServerReceivedError : The server has received an error.
//----------------------------------------------------------------------------
void TTestServer::ServerReceivedError(NStatus theErr)
{


	// Update our state
	mStateError = theErr;
}





//============================================================================
//		TTestClient::TTestClient : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
TTestClient::TTestClient(void)
{


	// Initialise ourselves
	mIsDone = false;

	mStateNumOpens  = 0;
	mStateNumCloses = 0;
	mStateNumMsgs   = 0;
	mStateError     = kNoErr;
}





//============================================================================
//		TTestClient::~TTestClient : Destructor.
//----------------------------------------------------------------------------
TTestClient::~TTestClient(void)
{


	// Validate our state
	NN_ASSERT(GetStatus() == kNClientDisconnected);
	
	NN_ASSERT(mStateNumOpens  == 1);
	NN_ASSERT(mStateNumCloses == 1);
	NN_ASSERT(mStateNumMsgs   == 1);
	NN_ASSERT_NOERR(mStateError);
}





//============================================================================
//		TTestClient::IsDone : Is the client done?
//----------------------------------------------------------------------------
bool TTestClient::IsDone(void) const
{


	// Check our state
	return(mIsDone);
}





//============================================================================
//		TTestClient::ReceivedMessage : Handle a message.
//----------------------------------------------------------------------------
void TTestClient::ReceivedMessage(const NNetworkMessage &theMsg)
{	NString		theValue;



	// Update our state
	mStateNumMsgs++;



	// Handle the message
	switch (theMsg.GetType()) {
		case kTestMessageAnswer:
			// Check the answer
			theValue = theMsg.GetValueString(kTestMessageValueKey);
			NN_ASSERT(theValue == kTestValueAnswer);
			
			
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
	mStateNumOpens++;



	// Send the question
	theMsg.SetType( kTestMessageQuestion);
	theMsg.SetValue(kTestMessageValueKey, kTestValueQuestion);

	SendMessage(theMsg);
}





//============================================================================
//		TTestClient::SessionDidClose : The session has closed.
//----------------------------------------------------------------------------
void TTestClient::SessionDidClose(void)
{


	// Update our state
	mStateNumCloses++;
}





//============================================================================
//		TTestClient::SessionReceivedError : The session has received an error.
//----------------------------------------------------------------------------
void TTestClient::SessionReceivedError(NStatus theErr)
{


	// Update our state
	mStateError = theErr;
}





//============================================================================
//		TMessageServer::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TMessageServer::Execute(void)
{	TTestServer		testServer;
	TTestClient		testClient;



	// Execute the tests
	testServer.Start(kValueServerPort);
	testClient.Connect("127.0.0.1", kValueServerPort);
	
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
}







