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
#include "NAtomicInt.h"
#include "NByteSwap.h"
#include "NLock.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Status
typedef enum {
	kNSocketOpening,
	kNSocketOpened,
	kNSocketClosed,
} NSocketStatus;


// Events
typedef enum {
	kNSocketDidOpen,
	kNSocketDidClose,
	kNSocketHasConnection,
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
										NSocket(NSocketRef theSocket, NSocket *parentSocket);

										NSocket(NSocketDelegate *theDelegate=NULL);
	virtual							   ~NSocket(void);


	// Get the status
	NSocketStatus						GetStatus(void) const;


	// Get the result
	//
	// Returns the final error that the socket was closed with.
	NStatus								GetResult(void) const;


	// Get the amount of pending data
	//
	// Returns the number of bytes written to, but not yet dispatched by, the socket.
	NIndex								GetPendingWrites(void) const;


	// Open the socket
	//
	// Opening a socket with a local port creates a listening socket. A listening
	// socket can not be read from/written to, and creates a new connecting socket
	// to handle each connection it receives.
	//
	// Opening a socket to a remote host and port creates a connecting socket. A
	// connecting socket can be read from/written to.
	//
	// The delegate will be informed when the operation completes, or an error occurrs.
	//
	// May be called from any thread.
	void								Open(                        UInt16 thePort);
	void								Open(const NString &theHost, UInt16 thePort);


	// Close the socket
	//
	// On closing the socket any active read/write requests will be stopped with
	// kNErrCancelled, queued read/write requests will be discarded, and future
	// read/writes will be denied.
	//
	// The current thread is blocked until the operation has completed.
	//
	// The delegate will be informed when the operation completes.
	//
	// May be called from any thread.
	void								Close(NStatus theErr=kNoErr);


	// Submit a read/write request
	//
	// The delegate will be informed when the operation completes, or an error occurrs.
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
	void								SocketDidClose(NStatus theErr);
	void								SocketHasConnection(NSocket *newSocket);

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
	NStatus								mResult;
	NSocketDelegate					   *mDelegate;

	NSocketRef							mSocket;
	bool								mSocketClosing;

	NSocketRequest					   *mReadRequest;
	NSocketRequestList					mReadRequests;
	NSemaphore							mReadSemaphore;
	NIndex								mReadBufferOffset;
	NIndex								mReadBufferAvailable;
	NData								mReadBuffer;

	NSocketRequest					   *mWriteRequest;
	NSocketRequestList					mWriteRequests;
	NSemaphore							mWriteSemaphore;
	NAtomicInt							mWritesPending;
};





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSocketDelegate {
public:
										NSocketDelegate(void);
	virtual							   ~NSocketDelegate(void);


	// The socket has opened
	virtual void						SocketDidOpen(NSocket *theSocket);


	// The socket has closed
	//
	// Sockets are automatically closed if they experience an error.
	//
	// Sockets which fail to open will also return their error through this method.
	virtual void						SocketDidClose(NSocket *theSocket, NStatus theErr);


	// The socket has a connection
	//
	// A listening socket's delegate can return a functor to handle the new
	// connection, or NULL to refuse the connection.
	//
	//
	// The new socket starts with the same delegate as the listening socket.
	//
	// If a different delegate is to be used, the new delegate should be
	// assigned before the functor is returned.
	//
	//
	// If a functor is returned, the new socket is opened. Once the new socket
	// has finished opening, the functor is invoked on a new thread to handle
	// the connection.
	//
	// The new socket will be closed (if necessary) and disposed of once the
	// functor has returned.
	virtual NSocketConnectionFunctor	SocketHasConnection(NSocket *theSocket, NSocket *newSocket);


	// The socket has finished a read/write
	virtual void						SocketFinishedRead( NSocket *theSocket, const NData &theData, NStatus theErr);
	virtual void						SocketFinishedWrite(NSocket *theSocket, const NData &theData, NStatus theErr);
};





#endif // NSOCKET_HDR


