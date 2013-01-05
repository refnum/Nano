/*	NAME:
		NSocket.cpp

	DESCRIPTION:
		Network socket.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetNetwork.h"
#include "NThread.h"
#include "NSocket.h"





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
static const NIndex kReadBufferSize									= 8 * kNKilobyte;





//============================================================================
//		NSocket::NSocket : Constructor.
//----------------------------------------------------------------------------
NSocket::NSocket(NSocketRef theSocket, NSocket *parentSocket)
{


	// Initialise ourselves
	InitialiseSelf(parentSocket->GetDelegate());
	
	mStatus = kNSocketOpening;
	mSocket = theSocket;
}





//============================================================================
//		NSocket::NSocket : Constructor.
//----------------------------------------------------------------------------
NSocket::NSocket(NSocketDelegate *theDelegate)
{


	// Initialise ourselves
	InitialiseSelf(theDelegate);
}





//============================================================================
//		NSocket::~NSocket : Destructor.
//----------------------------------------------------------------------------
NSocket::~NSocket(void)
{


	// Validate our state
	NN_ASSERT(mStatus == kNSocketClosed);
}





//============================================================================
//		NSocket::GetStatus : Get the status.
//----------------------------------------------------------------------------
NSocketStatus NSocket::GetStatus(void) const
{	StLock	acquireLock(mLock);



	// Get the status
	return(mStatus);
}





//============================================================================
//		NSocket::Open : Open the socket.
//----------------------------------------------------------------------------
void NSocket::Open(UInt16 thePort)
{


	// Open the socket
	Open("", thePort);
}





//============================================================================
//		NSocket::Open : Open the socket.
//----------------------------------------------------------------------------
void NSocket::Open(const NString &theHost, UInt16 thePort)
{	StLock		acquireLock(mLock);



	// Validate our state
	NN_ASSERT(mStatus   == kNSocketClosed);
	NN_ASSERT(mDelegate != NULL);
	
	
	
	// Open the socket
	mStatus = kNSocketOpening;
	mSocket = NTargetNetwork::SocketOpen(this, theHost, thePort);



	// Handle failure
	if (mSocket == NULL)
		SocketDidClose(kNErrInternal);
}





//============================================================================
//		NSocket::Close : Close the socket.
//----------------------------------------------------------------------------
void NSocket::Close(NStatus theErr)
{	StLock		acquireLock(mLock);



	// Validate our state
	NN_ASSERT(mStatus == kNSocketOpened);



	// Close the socket
	SocketDidClose(theErr);
}





//============================================================================
//		NSocket::Read : Submit a read request.
//----------------------------------------------------------------------------
void NSocket::Read(NIndex theSize)
{	NSocketRequest		*theRequest;



	// Validate our parameters
	NN_ASSERT(theSize >= 1);



	// Check our state
	if (GetStatus() != kNSocketOpened)
		{
		mDelegate->SocketFinishedRead(this, NData(), kNErrPermission);
		return;
		}



	// Submit the request
	theRequest = new NSocketRequest(NData(theSize));

	AddReadRequest(theRequest);



	// Clean up
	theRequest->Release();
}





//============================================================================
//		NSocket::Write : Submit a write request.
//----------------------------------------------------------------------------
void NSocket::Write(NIndex theSize, const void *thePtr)
{	NSocketRequest		*theRequest;



	// Validate our parameters
	NN_ASSERT(theSize >= 1);
	NN_ASSERT(thePtr  != NULL);



	// Check our state
	if (GetStatus() != kNSocketOpened)
		{
		mDelegate->SocketFinishedWrite(this, NData(), kNErrPermission);
		return;
		}



	// Submit the request
	theRequest = new NSocketRequest(NData(theSize, thePtr));

	AddWriteRequest(theRequest);



	// Clean up
	theRequest->Release();
}





//============================================================================
//		NSocket::ReadUInt8 : Read a UInt8.
//----------------------------------------------------------------------------
NStatus NSocket::ReadUInt8(UInt8 &theValue)
{	NStatus		theErr;



	// Read the value
	theErr = ReadData(sizeof(theValue), &theValue);

	return(theErr);
}





//============================================================================
//		NSocket::ReadUInt16 : Read a UInt16.
//----------------------------------------------------------------------------
NStatus NSocket::ReadUInt16(UInt16 &theValue, NEndianFormat wireFormat)
{	NStatus		theErr;



	// Read the value
	theErr = ReadData(sizeof(theValue), &theValue);
	if (theErr == kNoErr && wireFormat != kNEndianNative)
		NSwapUInt16(&theValue);
	
	return(theErr);
}





//============================================================================
//		NSocket::ReadUInt32 : Read a UInt32.
//----------------------------------------------------------------------------
NStatus NSocket::ReadUInt32(UInt32 &theValue, NEndianFormat wireFormat)
{	NStatus		theErr;



	// Read the value
	theErr = ReadData(sizeof(theValue), &theValue);
	if (theErr == kNoErr && wireFormat != kNEndianNative)
		NSwapUInt32(&theValue);
	
	return(theErr);
}





//============================================================================
//		NSocket::ReadUInt64 : Read a UInt64.
//----------------------------------------------------------------------------
NStatus NSocket::ReadUInt64(UInt64 &theValue, NEndianFormat wireFormat)
{	NStatus		theErr;



	// Read the value
	theErr = ReadData(sizeof(theValue), &theValue);
	if (theErr == kNoErr && wireFormat != kNEndianNative)
		NSwapUInt64(&theValue);
	
	return(theErr);
}





//============================================================================
//		NSocket::ReadSInt8 : Read an SInt8.
//----------------------------------------------------------------------------
NStatus NSocket::ReadSInt8(SInt8 &theValue)
{	NStatus		theErr;



	// Write the value
	theErr = ReadData(sizeof(theValue), &theValue);

	return(theErr);
}





//============================================================================
//		NSocket::ReadSInt16 : Read an SInt16.
//----------------------------------------------------------------------------
NStatus NSocket::ReadSInt16(SInt16 &theValue, NEndianFormat wireFormat)
{	NStatus		theErr;



	// Read the value
	theErr = ReadData(sizeof(theValue), &theValue);
	if (theErr == kNoErr && wireFormat != kNEndianNative)
		NSwapSInt16(&theValue);
	
	return(theErr);
}





//============================================================================
//		NSocket::ReadSInt32 : Read an SInt32.
//----------------------------------------------------------------------------
NStatus NSocket::ReadSInt32(SInt32 &theValue, NEndianFormat wireFormat)
{	NStatus		theErr;



	// Read the value
	theErr = ReadData(sizeof(theValue), &theValue);
	if (theErr == kNoErr && wireFormat != kNEndianNative)
		NSwapSInt32(&theValue);
	
	return(theErr);
}





//============================================================================
//		NSocket::ReadSInt64 : Read an SInt64.
//----------------------------------------------------------------------------
NStatus NSocket::ReadSInt64(SInt64 &theValue, NEndianFormat wireFormat)
{	NStatus		theErr;



	// Read the value
	theErr = ReadData(sizeof(theValue), &theValue);
	if (theErr == kNoErr && wireFormat != kNEndianNative)
		NSwapSInt64(&theValue);
	
	return(theErr);
}





//============================================================================
//		NSocket::ReadData : Read data.
//----------------------------------------------------------------------------
NStatus NSocket::ReadData(NIndex theSize, void *thePtr)
{	NData		theData;
	NStatus		theErr;



	// Read the data
	theErr = ReadData(theSize, theData);

	if (theErr == kNoErr)
		{
		NN_ASSERT(theData.GetSize() == theSize);
		memcpy(thePtr, theData.GetData(), theSize);
		}
	
	return(theErr);
}





//============================================================================
//		NSocket::ReadData : Read data.
//----------------------------------------------------------------------------
NStatus NSocket::ReadData(NIndex theSize, NData &theData)
{	NSocketRequest		*theRequest;
	NStatus				theErr;



	// Validate our parameters
	NN_ASSERT(theSize >= 1);



	// Check our state
	if (NN_DEBUG && NThread::IsMain())
		NN_LOG("NSocket performing blocking read of %ld bytes from network on main thread!", theSize);

	if (GetStatus() != kNSocketOpened)
		return(kNErrPermission);



	// Submit the request
	theRequest = new NSocketRequest(NData(theSize));
	theRequest->SetSemaphore(&mReadSemaphore);
	
	AddReadRequest(theRequest);



	// Wait for it to finish
	mReadSemaphore.Wait();
	
	theErr = theRequest->GetStatus();
	
	if (theErr == kNoErr)
		{
		theData = theRequest->GetFinishedData();
		NN_ASSERT(theData.GetSize() == theSize);
		}



	// Clean up
	theRequest->Release();

	return(theErr);
}





//============================================================================
//		NSocket::WriteUInt8 : Write a UInt8.
//----------------------------------------------------------------------------
NStatus NSocket::WriteUInt8(UInt8 theValue)
{


	// Write the value
	return(WriteData(sizeof(theValue), &theValue));
}





//============================================================================
//		NSocket::WriteUInt16 : Write a UInt16.
//----------------------------------------------------------------------------
NStatus NSocket::WriteUInt16(UInt16 theValue, NEndianFormat wireFormat)
{


	// Write the value
	if (wireFormat != kNEndianNative)
		NSwapUInt16(&theValue);

	return(WriteData(sizeof(theValue), &theValue));
}





//============================================================================
//		NSocket::WriteUInt32 : Write a UInt32.
//----------------------------------------------------------------------------
NStatus NSocket::WriteUInt32(UInt32 theValue, NEndianFormat wireFormat)
{


	// Write the value
	if (wireFormat != kNEndianNative)
		NSwapUInt32(&theValue);

	return(WriteData(sizeof(theValue), &theValue));
}





//============================================================================
//		NSocket::WriteUInt64 : Write a UInt64.
//----------------------------------------------------------------------------
NStatus NSocket::WriteUInt64(UInt64 theValue, NEndianFormat wireFormat)
{


	// Write the value
	if (wireFormat != kNEndianNative)
		NSwapUInt64(&theValue);

	return(WriteData(sizeof(theValue), &theValue));
}





//============================================================================
//		NSocket::WriteSInt8 : Write a SInt8.
//----------------------------------------------------------------------------
NStatus NSocket::WriteSInt8(SInt8 theValue)
{


	// Write the value
	return(WriteData(sizeof(theValue), &theValue));
}





//============================================================================
//		NSocket::WriteSInt16 : Write a SInt16.
//----------------------------------------------------------------------------
NStatus NSocket::WriteSInt16(SInt16 theValue, NEndianFormat wireFormat)
{


	// Write the value
	if (wireFormat != kNEndianNative)
		NSwapSInt16(&theValue);

	return(WriteData(sizeof(theValue), &theValue));
}





//============================================================================
//		NSocket::WriteSInt32 : Write a SInt32.
//----------------------------------------------------------------------------
NStatus NSocket::WriteSInt32(SInt32 theValue, NEndianFormat wireFormat)
{


	// Write the value
	if (wireFormat != kNEndianNative)
		NSwapSInt32(&theValue);

	return(WriteData(sizeof(theValue), &theValue));
}





//============================================================================
//		NSocket::WriteSInt64 : Write a SInt64.
//----------------------------------------------------------------------------
NStatus NSocket::WriteSInt64(SInt64 theValue, NEndianFormat wireFormat)
{


	// Write the value
	if (wireFormat != kNEndianNative)
		NSwapSInt64(&theValue);

	return(WriteData(sizeof(theValue), &theValue));
}





//============================================================================
//		NSocket::WriteData : Write data.
//----------------------------------------------------------------------------
NStatus NSocket::WriteData(NIndex theSize, const void *thePtr)
{


	// Write the data
	return(WriteData(NData(theSize, thePtr)));
}





//============================================================================
//		NSocket::WriteData : Write data.
//----------------------------------------------------------------------------
NStatus NSocket::WriteData(const NData &theData)
{	NSocketRequest		*theRequest;
	NStatus				theErr;



	// Validate our parameters
	NN_ASSERT(!theData.IsEmpty());



	// Check our state
	if (NN_DEBUG && NThread::IsMain())
		NN_LOG("NSocket performing blocking write of %ld bytes to network on main thread!", theData.GetSize());

	if (GetStatus() != kNSocketOpened)
		return(kNErrPermission);



	// Submit the request
	theRequest = new NSocketRequest(theData);
	theRequest->SetSemaphore(&mWriteSemaphore);
	
	AddWriteRequest(theRequest);



	// Wait for it to finish
	mWriteSemaphore.Wait();
	
	theErr = theRequest->GetStatus();



	// Clean up
	theRequest->Release();

	return(theErr);
}





//============================================================================
//		NSocket::GetDelegate : Get the delegate.
//----------------------------------------------------------------------------
NSocketDelegate *NSocket::GetDelegate(void) const
{


	// Get the delegate
	return(mDelegate);
}





//============================================================================
//		NSocket::SetDelegate : Set the delegate.
//----------------------------------------------------------------------------
void NSocket::SetDelegate(NSocketDelegate *theDelegate)
{


	// Set the delegate
	mDelegate = theDelegate;
}





//============================================================================
///		NSocket::GetOption : Get an option.
//----------------------------------------------------------------------------
SInt32 NSocket::GetOption(NSocketOption theOption) const
{


	// Validate our state
	NN_ASSERT(GetStatus() == kNSocketOpened);
	
	
	
	// Get the option
	return(NTargetNetwork::SocketGetOption(mSocket, theOption));
}





//============================================================================
///		NSocket::GetOption : Get an option.
//----------------------------------------------------------------------------
NStatus NSocket::SetOption(NSocketOption theOption, SInt32 theValue)
{


	// Validate our state
	NN_ASSERT(GetStatus() == kNSocketOpened);
	
	
	
	// Get the option
	return(NTargetNetwork::SocketSetOption(mSocket, theOption, theValue));
}





//============================================================================
//		NSocket::SocketEvent : Handle events.
//----------------------------------------------------------------------------
#pragma mark -
void NSocket::SocketEvent(NSocketEvent theEvent, UIntPtr theValue)
{	StLock		acquireLock(mLock);



	// Handle the event
	switch (theEvent) {
		case kNSocketDidOpen:
			SocketDidOpen();
			break;

		case kNSocketDidClose:
			SocketDidClose((NStatus) theValue);
			break;
		
		case kNSocketHasConnection:
			SocketHasConnection((NSocket *) theValue);
			break;

		case kNSocketCanRead:
			ContinueReading();
			break;

		case kNSocketCanWrite:
			ContinueWriting();
			break;

		default:
			NN_LOG("Unknown socket event: %d/%d", theEvent, theValue);
			break;
		}
}





//============================================================================
//		NSocket::InitialiseSelf : Initialise ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NSocket::InitialiseSelf(NSocketDelegate *theDelegate)
{


	// Initialise ourselves
	mStatus   = kNSocketClosed;
	mSocket   = NULL;
	mDelegate = theDelegate;
	
	mReadRequest         = NULL;
	mReadBufferOffset    = 0;
	mReadBufferAvailable = 0;
	mReadBuffer.SetSize(kReadBufferSize);
	
	mWriteRequest = NULL;
}





//============================================================================
//		NSocket::SocketDidOpen : The socket has opened.
//----------------------------------------------------------------------------
void NSocket::SocketDidOpen(void)
{


	// Validate our state
	NN_ASSERT(mStatus == kNSocketOpening);



	// Update our state
	mStatus = kNSocketOpened;

	mDelegate->SocketDidOpen(this);
}





//============================================================================
//		NSocket::SocketDidClose : The socket has closed.
//----------------------------------------------------------------------------
void NSocket::SocketDidClose(NStatus theErr)
{	NSocketRef		theSocket;



	// Close the socket
	//
	// The socket can be closed via Close(), or due to a kNSocketDidClose event.
	//
	// In both cases we are inside our lock, however the use of a lock means that
	// the socket may be closed by Close() on one thread which triggers a close
	// event on another.
	//
	// As such we need to check to see if the socket is already closed, and can
	// ignore the second notification.
	if (mStatus != kNSocketClosed)
		{
		// Update our state
		theSocket = mSocket;
		mStatus   = kNSocketClosed;
		mSocket   = NULL;

		RemoveRequests(kNErrCancelled);



		// Close the socket
		mDelegate->SocketDidClose(this, theErr);

		NTargetNetwork::SocketClose(theSocket);
		}
}





//============================================================================
//		NSocket::SocketHasConnection : The socket has a connection.
//----------------------------------------------------------------------------
void NSocket::SocketHasConnection(NSocket *newSocket)
{	NSocketConnectionFunctor	theFunctor;



	// Validate our state
	NN_ASSERT(mStatus == kNSocketOpened);



	// Handle the connection
	theFunctor = mDelegate->SocketHasConnection(this, newSocket);

	NThreadUtilities::DetachFunctor(BindFunction(NSocket::ConnectionThread, theFunctor, newSocket));
}





//============================================================================
//		NSocket::RemoveRequests : Remove the active/pending requests.
//----------------------------------------------------------------------------
void NSocket::RemoveRequests(NStatus theErr)
{	NSocketRequestListConstIterator		theIter;



	// Flush the pending requests
	for (theIter = mReadRequests.begin(); theIter != mReadRequests.end(); theIter++)
		(*theIter)->Release();

	for (theIter = mWriteRequests.begin(); theIter != mWriteRequests.end(); theIter++)
		(*theIter)->Release();

	mReadRequests.clear();
	mWriteRequests.clear();



	// Stop the active requests
	if (mReadRequest != NULL)
		{
		mReadRequest->SetStatus(theErr);
		FinishedReading();
		}
	
	if (mWriteRequest != NULL)
		{
		mWriteRequest->SetStatus(theErr);
		FinishedWriting();
		}
}





//============================================================================
//		NSocket::AddReadRequest : Add a read request.
//----------------------------------------------------------------------------
void NSocket::AddReadRequest(NSocketRequest *theRequest)
{	StLock		acquireLock(mLock);



	// Add the request
	theRequest->Retain();
	mReadRequests.push_back(theRequest);

	ContinueReading();
}





//============================================================================
//		NSocket::AddWriteRequest : Add a write request.
//----------------------------------------------------------------------------
void NSocket::AddWriteRequest(NSocketRequest *theRequest)
{	StLock		acquireLock(mLock);



	// Add the request
	theRequest->Retain();
	mWriteRequests.push_back(theRequest);

	ContinueWriting();
}





//============================================================================
//		NSocket::ContinueReading : Continue reading.
//----------------------------------------------------------------------------
void NSocket::ContinueReading(void)
{	NIndex		readSize, readOffset;



	// Continue reading
	//
	// Reading is done in three phases:
	//
	//	- Read data into the request from our buffer
	//	- Read data into the request from the network
	//	- Read data into the buffer  from the network
	//
	// After reading from the network into the request we read again into the buffer
	// since the request may have been be smaller than the data currently held by the
	// OS (so there may be more data available to read).
	//
	// This then requires us to loop, since the final read into the buffer may also
	// be the last part of the response (which means no further kNSocketCanRead events
	// would be seen, and ContinueReading not re-called).
	//
	//
	// However this looping also helps us to reduce notification overhead - on fast
	// networks the OS may have more data available to read by the time we finish
	// updating the request from the buffer.
	//
	// Rather than exit the event handler and have to wait for the OS to re-schedule
	// another event, our next pass will also check to see if there's more data.
	//
	// Ultimately we stop when we run out of requests to process, run out of data to
	// read, or fill the buffer.
	while (true)
		{
		// Start the next request
		if (mReadRequest == NULL)
			{
			if (mReadRequests.empty())
				break;
		
			mReadRequest = extract_front(mReadRequests);
			}



		// Read from the buffer
		if (mReadBufferAvailable != 0)
			{
			// Read into the request
			readSize = std::min(mReadBufferAvailable, mReadRequest->GetRemainingSize());
			memcpy(mReadRequest->GetRemainingData(), mReadBuffer.GetData(mReadBufferOffset), readSize);
			mReadRequest->ProcessedData(readSize);


			// Update our state
			mReadBufferOffset    += readSize;
			mReadBufferAvailable -= readSize;

			if (mReadBufferAvailable == 0)
				mReadBufferOffset = 0;
			}



		// Read from the network
		if (!mReadRequest->IsFinished())
			{
			if (!NTargetNetwork::SocketCanRead(mSocket))
				break;

			readSize = NTargetNetwork::SocketRead(mSocket, mReadRequest->GetRemainingSize(), mReadRequest->GetRemainingData());
			mReadRequest->ProcessedData(readSize);
			}



		// Read into the buffer
		if (NTargetNetwork::SocketCanRead(mSocket))
			{
			readOffset = mReadBufferOffset + mReadBufferAvailable;
			readSize   = mReadBuffer.GetSize() - readOffset;

			if (readSize != 0)
				{
				readSize              = NTargetNetwork::SocketRead(mSocket, readSize, mReadBuffer.GetData(readOffset));
				mReadBufferAvailable += readSize;
				}
			}



		// Finish the request
		if (mReadRequest->IsFinished())
			FinishedReading();
		}
}





//============================================================================
//		NSocket::ContinueWriting : Continue writing.
//----------------------------------------------------------------------------
void NSocket::ContinueWriting(void)
{	NIndex		writeSize;



	// Continue writing
	while (true)
		{
		// Start the next request
		if (mWriteRequest == NULL)
			{
			if (mWriteRequests.empty())
				break;
		
			mWriteRequest = extract_front(mWriteRequests);
			}



		// Write to the network
		if (!NTargetNetwork::SocketCanWrite(mSocket))
			break;
		
		writeSize = NTargetNetwork::SocketWrite(mSocket, mWriteRequest->GetRemainingSize(), mWriteRequest->GetRemainingData());
		mWriteRequest->ProcessedData(writeSize);



		// Finish the request
		if (mWriteRequest->IsFinished())
			FinishedWriting();
		}
}





//============================================================================
//		NSocket::FinishedReading : Finish a read request.
//----------------------------------------------------------------------------
void NSocket::FinishedReading(void)
{	NSemaphore		*theSemaphore;



	// Get the state we need
	theSemaphore = mReadRequest->GetSemaphore();



	// Inform the source
	if (theSemaphore != NULL)
		theSemaphore->Signal();
	else
		mDelegate->SocketFinishedRead(this, mReadRequest->GetFinishedData(), mReadRequest->GetStatus());



	// Finish the request
	mReadRequest->Release();
	mReadRequest = NULL;
}





//============================================================================
//		NSocket::FinishedWriting : Finish a write request.
//----------------------------------------------------------------------------
void NSocket::FinishedWriting(void)
{	NSemaphore		*theSemaphore;



	// Get the state we need
	theSemaphore = mWriteRequest->GetSemaphore();



	// Inform the source
	if (theSemaphore != NULL)
		theSemaphore->Signal();
	else
		mDelegate->SocketFinishedWrite(this, mWriteRequest->GetFinishedData(), mWriteRequest->GetStatus());



	// Finish the request
	mWriteRequest->Release();
	mWriteRequest = NULL;
}





//============================================================================
//		NSocket::ConnectionThread : Run a connection thread.
//----------------------------------------------------------------------------
void NSocket::ConnectionThread(const NSocketConnectionFunctor &theFunctor, NSocket *theSocket)
{


	// Wait for the socket to open
	while (theSocket->GetStatus() == kNSocketOpening)
		NThread::Sleep();



	// Handle the connection
	if (theSocket->GetStatus() == kNSocketOpened)
		{
		if (theFunctor != NULL)
			theFunctor(theSocket);
		}



	// Close the socket
	if (theSocket->GetStatus() == kNSocketOpened)
		theSocket->Close();



	// Clean up
	delete theSocket;
}





//============================================================================
//		NSocketDelegate::NSocketDelegate : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NSocketDelegate::NSocketDelegate(void)
{
}





//============================================================================
//		NSocketDelegate::~NSocketDelegate : Destructor.
//----------------------------------------------------------------------------
NSocketDelegate::~NSocketDelegate(void)
{
}





//============================================================================
//		NSocketDelegate::SocketDidOpen : The socket has opened.
//----------------------------------------------------------------------------
void NSocketDelegate::SocketDidOpen(NSocket * /*theSocket*/)
{
}





//============================================================================
//		NSocketDelegate::SocketDidClose : The socket has closed.
//----------------------------------------------------------------------------
void NSocketDelegate::SocketDidClose(NSocket * /*theSocket*/, NStatus /*theErr*/)
{
}





//============================================================================
//		NSocketDelegate::SocketHasConnection : The socket has a connection.
//----------------------------------------------------------------------------
NSocketConnectionFunctor NSocketDelegate::SocketHasConnection(NSocket * /*theSocket*/, NSocket * /*newSocket*/)
{
	return(NULL);
}





//============================================================================
//		NSocketDelegate::SocketFinishedRead : The socket has finished a read.
//----------------------------------------------------------------------------
void NSocketDelegate::SocketFinishedRead( NSocket * /*theSocket*/, const NData &/*theData*/, NStatus /*theErr*/)
{
}





//============================================================================
//		NSocketDelegate::SocketFinishedWrite : The socket has finished a write.
//----------------------------------------------------------------------------
void NSocketDelegate::SocketFinishedWrite( NSocket * /*theSocket*/, const NData &/*theData*/, NStatus /*theErr*/)
{
}



