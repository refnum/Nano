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
	kNSocketGotError,
	kNSocketCanRead,
	kNSocketCanWrite
} NSocketEvent;


// Options
typedef enum {
	kNSocketNoDelay						// bool, Disable Nagle's algorithm
} NSocketOption;





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NSocketDelegate;


// Internal
typedef struct NSocketInfo *NSocketRef;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NSocket : public NUncopyable {
public:
										NSocket(NSocketDelegate *theDelegate);
	virtual							   ~NSocket(void);


	// Get the status
	NSocketStatus						GetStatus(void) const;


	// Open the socket
	//
	// Opens a socket on a local port for listening, or connects
	// to the specified port on a remote host.
	//
	// The delegate will be informed when the operation is complete.
	//
	// May be called from any thread.
	void								Open(                        UInt16 thePort);
	void								Open(const NString &theHost, UInt16 thePort);


	// Close the socket
	//
	// The current read/write request, if any, will be stopped with a kNErrCancelled.
	//
	// Queued read/write requests will be discarded, and future read/writes refused.
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
	// The delegate is not informed of the operation.
	//
	// May  not be called from the main thread - the current thread will
	// be blocked until the operation has completed.
	NStatus								ReadUInt8( UInt8  &theValue);
	NStatus								ReadUInt16(UInt16 &theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadUInt32(UInt32 &theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadUInt64(UInt64 &theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadSInt8( SInt8  &theValue);
	NStatus								ReadSInt16(SInt16 &theValue,  NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadSInt32(SInt32 &theValue,  NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadSInt64(SInt64 &theValue,  NEndianFormat wireFormat=kNEndianBig);
	NStatus								ReadData(  NIndex  theSize, void  *theData);
	NStatus								ReadData(  NIndex  theSize, NData &theData);

	NStatus								WriteUInt8( UInt8  theValue);
	NStatus								WriteUInt16(UInt16 theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteUInt32(UInt32 theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteUInt64(UInt64 theValue, NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteSInt8( SInt8  theValue);
	NStatus								WriteSInt16(SInt16 theValue,  NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteSInt32(SInt32 theValue,  NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteSInt64(SInt64 theValue,  NEndianFormat wireFormat=kNEndianBig);
	NStatus								WriteData(  NIndex theSize, const void  *theData);
	NStatus								WriteData(                  const NData &theData);


	// Get/set the delegate
	//
	// The delegate is invoked on an internal thread.
	NSocketDelegate					   *GetDelegate(void) const;
	void								SetDelegate(NSocketDelegate *theDelegate);


	// Get/set an option
	//
	// The meaning of the value is option-specific.
	SInt32								GetOption(NSocketOption theOption) const;
	NStatus								SetOption(NSocketOption theOption, SInt32 theValue);


public:
	// Handle events
	void								SocketEvent(NSocketEvent theEvent, UInt32 theValue=0);


private:
	void								SocketOpened(void);
	void								SocketClosed(void);
	void								SocketError(NStatus theErr);

	void								RemoveRequests( NStatus theErr);
	void								AddReadRequest( NSocketRequest *theRequest);
	void								AddWriteRequest(NSocketRequest *theRequest);

	void								ContinueReading(void);
	void								ContinueWriting(void);

	void								FinishedReading(void);
	void								FinishedWriting(void);


private:
	mutable NSpinLock					mLock;

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
	virtual void						SocketOpened(NSocket *theSocket);
	virtual void						SocketClosed(NSocket *theSocket);


	// The socket has reported an error
	virtual void						SocketReceivedError(NSocket *theSocket, NStatus theErr);


	// The socket has finished a read/write
	virtual void						SocketFinishedRead( NSocket *theSocket, const NData &theData, NStatus theErr);
	virtual void						SocketFinishedWrite(NSocket *theSocket, const NData &theData, NStatus theErr);
};





#endif // NSOCKET_HDR


