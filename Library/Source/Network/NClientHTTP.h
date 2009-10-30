/*	NAME:
		NClientHTTP.h

	DESCRIPTION:
		HTTP client.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCLIENTHTTP_HDR
#define NCLIENTHTTP_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NMsgListener.h"
#include "NDictionary.h"
#include "NCFStream.h"
#include "NString.h"
#include "NCFURL.h"
#include "NData.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgClientHTTPEvent						= 'htte';


// HTTP
static const NString kMethodGet										= "GET";
static const NString kMethodPost									= "POST";

static const NString kHeaderUserAgent								= "User-Agent";
static const NString kHeaderContentLength							= "Content-Length";





//============================================================================
//		Types
//----------------------------------------------------------------------------
// Classes
class NClientHTTP;


// Message data for kMsgClientHTTPEvent
typedef struct {
	// Misc
	const NClientHTTP		*theClient;
	NCFStream				*theStream;
	
	
	// Stream
	//
	// The streamResponse object is the CFHTTPMessage response message.
	CFStreamEventType		streamEvent;
	NString					streamMethod;
	NCFObject				streamResponse;


	// Progress
	//
	// If progress information is required, UpdateProgress should be called when
	// data is read from the response (this will happen automatically for post
	// requests, or when an output stream was supplied for get requests).
	UInt32					writeContentLength;
	UInt32					writeContentDone;

	UInt32					readContentLength;
	UInt32					readContentDone;

	float					progressPercentDone;
	UInt32					progressBytesPerSec;
	UInt32					progressBytesLeft;
	EventTime				progressTimeLeft;
} ClientHTTPEvent;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NClientHTTP : public NMsgBroadcaster, public NMsgListener {
public:
										NClientHTTP(void);
	virtual								~NClientHTTP(void);


	// Get/set the user agent
	NString								GetUserAgent(void) const;
	void								SetUserAgent(const NString &theValue);


	// Get/set the auto-redirect state
	bool								IsAutoRedirecting(void) const;
	void								SetAutoRedirect(bool autoRedirect);


	// Is a request in progress?
	bool								IsBusy(void) const;


	// Submit a request
	//
	// Only one request can be submitted at a time.
	//
	// If an output stream is supplied, the body of the response will automatically
	// be read from the server and written to this stream as it arrives.
	OSStatus							SubmitRequest(const NCFURL		&theURL,
													  NCFStream			*theOutput  = NULL,
													  const NString		&theMethod  = kMethodGet,
													  const NDictionary	*theHeaders = NULL,
													  const NData		*theBody    = NULL);


	// Cancel the current request
	void								Cancel(void);


	// Update the progress state
	//
	// Callers should invoke this function while processing a kMsgClientHTTPEvent event,
	// to allow the client to update the progress fields in the event.
	//
	// This will happen automatically for post requests, and for get requests that are
	// receiving the response through an output stream.
	//
	// Typically this would only need to be called if a client is extracting data from
	// the response stream themselves, and need to signal when data has been read.
	void								UpdateProgress(UInt32 bytesRead);


protected:
	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, void *msgData);


private:
	NString								GetDefaultUserAgent(void);

	void								ResetStream(void);
	void								DoStreamEvent(CFStreamEventType theEvent);
	

private:
	NString								mUserAgent;
	bool								mAutoRedirect;

	NCFStream							mStream;
	NCFStream							*mStreamOutput;
	ClientHTTPEvent						mStreamEvent;
	UInt32								mStreamUpdateDone;
	EventTime							mStreamUpdateTime;
};







#endif // NCLIENTHTTP_HDR


