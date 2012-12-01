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
#include "TSocket.h"





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class TSocketClient : public NSocketDelegate {
public:
										TSocketClient(void);
	virtual							   ~TSocketClient(void);


	// Execute the tests
	void								Execute(bool *isDone);


protected:
	// Handle events
	void								SocketReceivedError(NSocket *theSocket, NStatus theErr);


private:


};





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
//		TSocketClient::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TSocketClient::Execute(bool *isDone)
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
//		TSocketClient::SocketReceivedError : The socket has received an error.
//----------------------------------------------------------------------------
void TSocketClient::SocketReceivedError(NSocket * /*theSocket*/, NStatus theErr)
{


	// Log the event
	NN_LOG("Socket received error: %d", theErr);
}





//============================================================================
//		TSocket::Execute : Execute the tests.
//----------------------------------------------------------------------------
#pragma mark -
void TSocket::Execute(void)
{	TSocketClient		*theClient;
	bool				isDone;



	// Get the state we need
	theClient = new TSocketClient;
	isDone    = false;



	// Execute the tests
	NThreadUtilities::DelayFunctor(BindMethod(theClient, TSocketClient::Execute, &isDone), 0.0, false);

	while (!isDone)
		usleep(100);
	
	
	
	// Clean up
	delete theClient;
}


