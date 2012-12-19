/*	NAME:
		TSocket.cpp

	DESCRIPTION:
		NSocket tests.

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
#include "NMathUtilities.h"
#include "TSocket.h"





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
	NSocketConnectionFunctor			SocketReceivedConnection(NSocket *theSocket, NSocket *newSocket);
	void								SocketReceivedError(     NSocket *theSocket, NStatus theErr);


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
	void								ExecuteWeb(   bool *isDone);
	void								ExecuteCustom(bool *isDone, UInt16 thePort);


protected:
	// Handle events
	void								SocketReceivedError(NSocket *theSocket, NStatus theErr);


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
//		TSocketServer::SocketReceivedConnection : The socket has received a connection.
//----------------------------------------------------------------------------
NSocketConnectionFunctor TSocketServer::SocketReceivedConnection(NSocket * /*theSocket*/, NSocket * /*newSocket*/)
{


	// Get the functor
	return(BindSelf(TSocketServer::HandleConnection, _1));
}





//============================================================================
//		TSocketServer::SocketReceivedError : The socket has received an error.
//----------------------------------------------------------------------------
void TSocketServer::SocketReceivedError(NSocket * /*theSocket*/, NStatus theErr)
{


	// Compiler warnings
	NN_UNUSED(theErr);



	// Log the event
	NN_LOG("Socket received error: %d", theErr);
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





//============================================================================
//		TSocketClient::TSocketClient : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
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
void TSocketClient::ExecuteWeb(bool *isDone)
{	NSocket		*theSocket;
	NData		theData;
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
	theErr = theSocket->ReadData(400, theData);
	NN_ASSERT_NOERR(theErr);
	
	theText.SetData(theData);
	NN_ASSERT(theText.Contains("text/html"));



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
//		TSocketClient::SocketReceivedError : The socket has received an error.
//----------------------------------------------------------------------------
void TSocketClient::SocketReceivedError(NSocket * /*theSocket*/, NStatus theErr)
{


	// Compiler warnings
	NN_UNUSED(theErr);



	// Log the event
	NN_LOG("Socket received error: %d", theErr);
}





//============================================================================
//		TSocket::Execute : Execute the tests.
//----------------------------------------------------------------------------
#pragma mark -
void TSocket::Execute(void)
{	TSocketServer		*theServer;
	TSocketClient		*theClient;
	UInt16				thePort;
	bool				isDone;



	// Get the state we need
	thePort = NMathUtilities::GetRandomUInt16(kServerPortMin, kServerPortMax);

	theServer = new TSocketServer;
	theClient = new TSocketClient;



	// Execute a web test
	isDone = false;
	NThreadUtilities::DelayFunctor(BindMethod(theClient, TSocketClient::ExecuteWeb, &isDone), 0.0, false);

	while (!isDone)
		NThread::Sleep();



	// Execute a custom test
	theServer->Start(thePort);

	isDone = false;
	NThreadUtilities::DelayFunctor(BindMethod(theClient, TSocketClient::ExecuteCustom, &isDone, thePort), 0.0, false);

	while (!isDone)
		NThread::Sleep();

	theServer->Stop();



	// Clean up
	delete theServer;
	delete theClient;
}


