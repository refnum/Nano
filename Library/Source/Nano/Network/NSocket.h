/*	NAME:
		NSocket.h

	DESCRIPTION:
		Network socket.

	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NSOCKET_HDR
#define NSOCKET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSocketRequest.h"
#include "NUncopyable.h"
#include "NByteSwap.h"
#include "NLock.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Status
typedef enum {
	kNSocketOpened,
	kNSocketClosed,
	kNSocketOpening,
	kNSocketClosing
} NSocketStatus;


// Events
typedef enum {
	kNSocketDidOpen,
	kNSocketDidClose,
	kNSocketReceivedConnection,
	kNSocketReceivedError,
	kNSocketCanRead,
	kNSocketCanWrite
} NSocketEvent;


// Options
typedef enum {
	kNSocketNoDelay													// bool, Disable Nagle's algorithm
} NSocketOption;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NSocket;
class NSocketDelegate;


// Functors
typedef nfunctor<void (NSocket *theSocket)>							NSocketConnectionFunctor;


// Internal
typedef struct NSocketInfo *NSocketRef;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSocket : public NUncopyable {
public:
										NSocket(NSocketDelegate *theDelegate);
										NSocket(NSocketRef theSocket, NSocket *parentSocket);
	virtual							   ~NSocket(void);


	// Get the status
	NSocketStatus						GetStatus(void) const;


	// Open the socket
	//
	// Opening a socket with a local port creates a listening socket. A listening
	// socket can not be read from/written to, and creates a new connecting socket
	// to handle each connection it receives.
	//
	// Opening a socket to a remote host and port creates a connecting socket.
	// A connecting socket can be read from/written to.
	//
	// The delegate will be informed when the operation is complete.
	//
	// May be called from any thread.
	void								Open(                        UInt16 thePort);
	void								Open(const NString &theHost, UInt16 thePort);


	// Close the socket
	//
	// Closing the socket will stop the current read/write request with kNErrCancelled.
	// Queued read/write requests will be discarded, and future read/writes denied.
	//
	// The delegate will be informed when the operation is complete.
	//
	// May be called from any thread.
	void								Close(void);


	// Submit a read/write request
	//
	// The delegate will be informed when the operation is complete.
	//
	// May be called from any thread.
	void								Read( NIndex theSize);
	void								Write(NIndex theSize, const void *thePtr);


	// Perform a read/write operation
	//
	// The current thread is blocked until the operation has completed.
	//
	// The delegate will not be informed of the operation.
	//
	// May be called from any thread, except the main thread.
	NStatus								ReadUInt8( UInt8  &theValue);
	NStatus								ReadUInt16(UInt16 &theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadUInt32(UInt32 &theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadUInt64(UInt64 &theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadSInt8( SInt8  &theValue);
	NStatus								ReadSInt16(SInt16 &theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadSInt32(SInt32 &theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadSInt64(SInt64 &theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadData(  NIndex  theSize, void  *theData);
	NStatus								ReadData(  NIndex  theSize, NData &theData);

	NStatus								WriteUInt8( UInt8  theValue);
	NStatus								WriteUInt16(UInt16 theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteUInt32(UInt32 theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteUInt64(UInt64 theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteSInt8( SInt8  theValue);
	NStatus								WriteSInt16(SInt16 theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteSInt32(SInt32 theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteSInt64(SInt64 theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteData(  NIndex theSize, const void  *theData);
	NStatus								WriteData(                  const NData &theData);


	// Get/set the delegate
	//
	// The delegate is invoked on an internal thread, which must not be blocked.
	NSocketDelegate					   *GetDelegate(void) const;
	void								SetDelegate(NSocketDelegate *theDelegate);


	// Get/set an option
	//
	// The meaning of the value is option-specific.
	SInt32								GetOption(NSocketOption theOption) const;
	NStatus								SetOption(NSocketOption theOption, SInt32 theValue);


public:
	// Handle events
	void								SocketEvent(NSocketEvent theEvent, UIntPtr theValue=0);


private:
	void								InitialiseSelf(NSocketDelegate *theDelegate);

	void								SocketDidOpen(void);
	void								SocketDidClose(void);
	void								SocketReceivedConnection(NSocket *newSocket);
	void								SocketReceivedError(     NStatus  theErr);

	void								RemoveRequests( NStatus theErr);
	void								AddReadRequest( NSocketRequest *theRequest);
	void								AddWriteRequest(NSocketRequest *theRequest);

	void								ContinueReading(void);
	void								ContinueWriting(void);

	void								FinishedReading(void);
	void								FinishedWriting(void);
	
	static void							ConnectionThread(const NSocketConnectionFunctor &theFunctor, NSocket *theSocket);


private:
	mutable NMutexLock					mLock;

	NSocketStatus						mStatus;
	NSocketRef							mSocket;
	NSocketDelegate					   *mDelegate;

	NSocketRequest					   *mReadRequest;
	NSocketRequestList					mReadRequests;
	NSemaphore							mReadSemaphore;
	NIndex								mReadBufferOffset;
	NIndex								mReadBufferAvailable;
	NData								mReadBuffer;

	NSocketRequest					   *mWriteRequest;
	NSocketRequestList					mWriteRequests;
	NSemaphore							mWriteSemaphore;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSocketDelegate {
public:
										NSocketDelegate(void);
	virtual							   ~NSocketDelegate(void);


	// The socket has been opened/closed
	//
	// Once opened, a listening socket is able to receive connections and a
	// connecting socket can be used to read/write data.
	virtual void						SocketDidOpen( NSocket *theSocket);
	virtual void						SocketDidClose(NSocket *theSocket);


	// The socket has received a connection
	//
	// Invoked with the new socket that has connected to the listening socket.
	//
	// Returns a functor to handle the connection, or NULL to refuse the connection.
	//
	//
	// The new socket starts with the same delegate as the listening socket, but
	// this can be changed before the connection is accepted and passed to the
	// connection functor.
	//
	// The new socket's delegate will receive a SocketDidOpen before the functor
	// is invoked, and a SocketDidClose once the functor returns.
	//
	//
	// Once the new socket has opened, the connection functor will be invoked on
	// a new thread to handle the connection.
	//
	// Once the functor returns, the new socket will be be closed (if necessary)
	// and disposed of automatically.
	virtual NSocketConnectionFunctor	SocketReceivedConnection(NSocket *theSocket, NSocket *newSocket);


	// The socket has received an error
	//
	// The socket will be closed automatically after the error is handled.
	virtual void						SocketReceivedError(NSocket *theSocket, NStatus theErr);


	// The socket has finished a read/write
	virtual void						SocketFinishedRead( NSocket *theSocket, const NData &theData, NStatus theErr);
	virtual void						SocketFinishedWrite(NSocket *theSocket, const NData &theData, NStatus theErr);
};





#endif // NSOCKET_HDR


