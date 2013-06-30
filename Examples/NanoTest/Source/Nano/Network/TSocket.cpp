/*	NAME:
		TSocket.cpp

	DESCRIPTION:
		NSocket tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NThreadUtilities.h"
#include "NMathUtilities.h"
#include "NTestFixture.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Server
static const UInt16 kServerPortMin									= 7000;
static const UInt16 kServerPortMax									= 7999;

static const UInt32 kTestNumItems									= 100;





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class TSocketServer : public NSocketDelegate {
public:
										TSocketServer(void);
	virtual							   ~TSocketServer(void);


	// Start/stop the server
	void								Start(UInt16 thePort);
	void								Stop( void);


protected:
	// Handle events
	void								SocketDidClose(     NSocket *theSocket, NStatus theErr);
	NSocketConnectionFunctor			SocketHasConnection(NSocket *theSocket, NSocket *newSocket);


private:
	void								HandleConnection(NSocket *theSocket);


private:
	NSocket							   *mSocket;
};





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class TSocketClient : public NSocketDelegate {
public:
										TSocketClient(void);
	virtual							   ~TSocketClient(void);


	// Execute the tests
	void								ExecuteWeb(   bool *isDone, NData *theData);
	void								ExecuteCustom(bool *isDone, UInt16 thePort);


protected:
	// Handle events
	void								SocketDidClose(NSocket *theSocket, NStatus theErr);


private:


};





//============================================================================
//		TSocketServer::TSocketServer : Constructor.
//----------------------------------------------------------------------------
TSocketServer::TSocketServer(void)
{


	// Initialise ourselves
	mSocket = NULL;
}





//============================================================================
//		TSocketServer::~TSocketServer : Destructor.
//----------------------------------------------------------------------------
TSocketServer::~TSocketServer(void)
{


	// Validate our state
	NN_ASSERT(mSocket == NULL);
}





//============================================================================
//		TSocketServer::Start : Start the server.
//----------------------------------------------------------------------------
void TSocketServer::Start(UInt16 thePort)
{


	// Validate our state
	NN_ASSERT(mSocket == NULL);



	// Start the server
	mSocket = new NSocket(this);
	mSocket->Open(thePort);

	while (mSocket->GetStatus() != kNSocketOpened)
		NThread::Sleep();
}





//============================================================================
//		TSocketServer::Stop : Stop the server.
//----------------------------------------------------------------------------
void TSocketServer::Stop(void)
{


	// Validate our state
	NN_ASSERT(mSocket != NULL);
	
	
	
	// Stop the server
	mSocket->Close();

	while (mSocket->GetStatus() != kNSocketClosed)
		NThread::Sleep();



	// Clean up
	delete mSocket;
	mSocket = NULL;
}





//============================================================================
//		TSocketServer::SocketDidClose : The socket has closed.
//----------------------------------------------------------------------------
void TSocketServer::SocketDidClose(NSocket * /*theSocket*/, NStatus theErr)
{


	// Log the event
	if (theErr != kNoErr)
		NN_LOG("TSocketServer closed with error: %d", theErr);
}





//============================================================================
//		TSocketServer::SocketHasConnection : The socket has a connection.
//----------------------------------------------------------------------------
NSocketConnectionFunctor TSocketServer::SocketHasConnection(NSocket * /*theSocket*/, NSocket * /*newSocket*/)
{


	// Get the functor
	return(BindSelf(TSocketServer::HandleConnection, _1));
}





//============================================================================
//		TSocketServer::HandleConnection : Handle a connection.
//----------------------------------------------------------------------------
void TSocketServer::HandleConnection(NSocket *theSocket)
{	UInt32		n, theValue;
	NStatus		theErr;



	// Read the incoming data
	for (n = 0; n < kTestNumItems; n++)
		{
		theErr = theSocket->ReadUInt32(theValue);
		NN_ASSERT_NOERR(theErr);
		NN_ASSERT(theValue == n);
		}



	// Send a response
	for (n = 0; n < kTestNumItems; n++)
		{
		theErr = theSocket->WriteUInt32(n * n);
		NN_ASSERT_NOERR(theErr);
		}
}





#pragma mark TSocketClient
//============================================================================
//		TSocketClient::TSocketClient : Constructor.
//----------------------------------------------------------------------------
TSocketClient::TSocketClient(void)
{
}





//============================================================================
//		TSocketClient::~TSocketClient : Destructor.
//----------------------------------------------------------------------------
TSocketClient::~TSocketClient(void)
{
}





//============================================================================
//		TSocketClient::ExecuteWeb : Execute a web client test.
//----------------------------------------------------------------------------
void TSocketClient::ExecuteWeb(bool *isDone, NData *theData)
{	NSocket		*theSocket;
	NString		theText;
	NStatus		theErr;



	// Open the socket
	theSocket = new NSocket(this);
	theSocket->Open("www.google.com", 80);
	
	while (theSocket->GetStatus() != kNSocketOpened)
		NThread::Sleep();



	// Write some data
	theText = "GET / HTTP/1.0\r\n\r\n";
	theErr  = theSocket->WriteData(theText.GetData());
	NN_ASSERT_NOERR(theErr);



	// Read some data
	theErr = theSocket->ReadData(400, *theData);
	NN_ASSERT_NOERR(theErr);



	// Clean up
	theSocket->Close();

	while (theSocket->GetStatus() != kNSocketClosed)
		NThread::Sleep();
	
	delete theSocket;



	// Finish off
	*isDone = true;
}





//============================================================================
//		TSocketClient::ExecuteCustom : Execute a custom client test.
//----------------------------------------------------------------------------
void TSocketClient::ExecuteCustom(bool *isDone, UInt16 thePort)
{	UInt32		n, theValue;
	NSocket		*theSocket;
	NStatus		theErr;



	// Open the socket
	theSocket = new NSocket(this);
	theSocket->Open("127.0.0.1", thePort);
	
	while (theSocket->GetStatus() != kNSocketOpened)
		NThread::Sleep();



	// Get/set optins
	theErr = theSocket->SetOption(kNSocketNoDelay, (SInt32) true);
	NN_ASSERT_NOERR(theErr);
	
	theValue = theSocket->GetOption(kNSocketNoDelay);
	NN_ASSERT(theValue == (SInt32) true);



	// Write some data
	for (n = 0; n < kTestNumItems; n++)
		{
		theErr = theSocket->WriteUInt32(n);
		NN_ASSERT_NOERR(theErr);
		}



	// Read the response
	for (n = 0; n < kTestNumItems; n++)
		{
		theErr = theSocket->ReadUInt32(theValue);
		NN_ASSERT_NOERR(theErr);
		NN_ASSERT(theValue == (n * n));
		}



	// Clean up
	theSocket->Close();

	while (theSocket->GetStatus() != kNSocketClosed)
		NThread::Sleep();
	
	delete theSocket;



	// Finish off
	*isDone = true;
}





//============================================================================
//		TSocketClient::SocketDidClose : The socket has closed.
//----------------------------------------------------------------------------
void TSocketClient::SocketDidClose(NSocket * /*theSocket*/, NStatus theErr)
{


	// Log the event
	if (theErr != kNoErr)
		NN_LOG("TSocketClient closed with error: %d", theErr);
}





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NSOCKET(...)											TEST_NANO(TSocket, ##__VA_ARGS__)

FIXTURE_NANO(TSocket)
{
	TSocketServer		*theServer;
	TSocketClient		*theClient;
	UInt16				thePort;
	bool				isDone;

	SETUP
	{
		theServer = new TSocketServer;
		theClient = new TSocketClient;

		thePort = NMathUtilities::GetRandomUInt16(kServerPortMin, kServerPortMax);
		isDone  = false;
	}
	
	TEARDOWN
	{
		delete theServer;
		delete theClient;
	}
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSOCKET("Client")
{	NData		theData;
	NString		theText;



	// Perform the test
	NThreadUtilities::DetachFunctor(BindMethod(theClient, TSocketClient::ExecuteWeb, &isDone, &theData));

	while (!isDone)
		NThread::Sleep();

	theText.SetData(theData);
	REQUIRE(theText.Contains("text/html"));
}





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NSOCKET("Server")
{


	// Perform the test
	theServer->Start(thePort);

	NThreadUtilities::DetachFunctor(BindMethod(theClient, TSocketClient::ExecuteCustom, &isDone, thePort));

	while (!isDone)
		NThread::Sleep();

	theServer->Stop();
}




