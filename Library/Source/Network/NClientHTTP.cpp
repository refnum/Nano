/*	NAME:
		NClientHTTP.cpp

	DESCRIPTION:
		HTTP client.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMathUtilities.h"
#include "NSystemUtilities.h"
#include "NCFBundle.h"
#include "NClientHTTP.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const EventTime kProgressUpdate									= 0.5f;





//============================================================================
//		NClientHTTP::NClientHTTP : Constructor.
//----------------------------------------------------------------------------
NClientHTTP::NClientHTTP(void)
{


	// Initialize ourselves
	mUserAgent    = GetDefaultUserAgent();
	mAutoRedirect = true;
	
	ResetStream();



	// Register for events
	mStream.AddListener(this);
}





//============================================================================
//		NClientHTTP::~NClientHTTP : Destructor.
//----------------------------------------------------------------------------
NClientHTTP::~NClientHTTP(void)
{
}





//============================================================================
//		NClientHTTP::GetUserAgent : Get the user agent.
//----------------------------------------------------------------------------
NString NClientHTTP::GetUserAgent(void) const
{


	// Get the user agent
	return(mUserAgent);
}





//============================================================================
//		NClientHTTP::SetUserAgent : Set the user agent.
//----------------------------------------------------------------------------
void NClientHTTP::SetUserAgent(const NString &theValue)
{


	// Set the user agent
	mUserAgent = theValue;
}





//============================================================================
//		NClientHTTP::IsAutoRedirecting : Get the auto-redirect state.
//----------------------------------------------------------------------------
bool NClientHTTP::IsAutoRedirecting(void) const
{


	// Get the state
	return(mAutoRedirect);
}





//============================================================================
//		NClientHTTP::SetAutoRedirect : Set the auto-redirect state.
//----------------------------------------------------------------------------
void NClientHTTP::SetAutoRedirect(bool autoRedirect)
{


	// Set the state
	mAutoRedirect = autoRedirect;
}





//============================================================================
//		NClientHTTP::IsBusy : Is a request in progress?
//----------------------------------------------------------------------------
bool NClientHTTP::IsBusy(void) const
{


	// Check our state
	if (!mStream.IsValid())
		return(false);



	// Check the stream status
	switch (mStream.GetStatus()) {
		case kCFStreamStatusNotOpen:
		case kCFStreamStatusClosed:
			return(false);
			break;
		
		default:
			break;
		}

	return(true);
}





//============================================================================
//		NClientHTTP::SubmitRequest : Submit a request.
//----------------------------------------------------------------------------
OSStatus NClientHTTP::SubmitRequest(const NCFURL		&theURL,
									NCFStream			*theOutput,
									const NString		&theMethod,
									const NDictionary	*theHeaders,
									const NData			*theBody)
{	NCFObject					cfRequest;
	NString						theValue;
	NStringListConstIterator	theIter;
	NStringList					theKeys;



	// Check our state
	if (IsBusy())
		return(kEBUSYErr);
		


	// Create the request
	if (!cfRequest.Set(CFHTTPMessageCreateRequest(kCFAllocatorNano, theMethod, theURL, kCFHTTPVersion1_1)))
		return(paramErr);



	// Create the message
	if (mUserAgent.IsNotEmpty())
		CFHTTPMessageSetHeaderFieldValue(cfRequest, kHeaderUserAgent, mUserAgent);

	if (theHeaders != NULL)
		{
		theKeys = theHeaders->GetKeys();
		for (theIter = theKeys.begin(); theIter != theKeys.end(); theIter++)
			{
			theValue = theHeaders->GetValueString(*theIter);
			CFHTTPMessageSetHeaderFieldValue(cfRequest, *theIter, theValue);
			}
		}

	if (theBody != NULL)
		CFHTTPMessageSetBody(cfRequest, *theBody);



	// Create the stream
	if (!mStream.Set(CFReadStreamCreateForHTTPRequest(kCFAllocatorNano, cfRequest)))
		return(networkErr);

	if (mAutoRedirect)
		CFReadStreamSetProperty(mStream, kCFStreamPropertyHTTPShouldAutoredirect, kCFBooleanTrue);



	// Update our state
	ResetStream();
	
	mStreamOutput             = theOutput;
	mStreamEvent.streamMethod = theMethod;
	
	if (theMethod == kMethodPost && theBody != NULL)
		mStreamEvent.writeContentLength = theBody->GetSize();



	// Submit the request
	CFReadStreamOpen(mStream);

	return(noErr);
}





//============================================================================
//		NClientHTTP::Cancel : Cancel the current request.
//----------------------------------------------------------------------------
void NClientHTTP::Cancel(void)
{


	// Cancel the request
	if (IsBusy())
		mStream.Close();
	
	ResetStream();
}





//============================================================================
//		NClientHTTP::UpdateProgress : Update the progress state.
//----------------------------------------------------------------------------
void NClientHTTP::UpdateProgress(UInt32 bytesRead)
{	UInt32			bytesDone, bytesTotal, bytesDelta;
	EventTime		timeNow, timeDelta;



	// Validate our parameters and state
	NN_ASSERT(IsBusy());

	if (bytesRead != 0)
		NN_ASSERT(mStreamEvent.streamMethod == kMethodGet);



	// Get the state we need
	if (mStreamEvent.streamMethod == kMethodPost)
		{
		bytesDone  = mStreamEvent.writeContentDone;
		bytesTotal = mStreamEvent.writeContentLength;
		}
	else
		{
		bytesDone  = mStreamEvent.readContentDone + bytesRead;
		bytesTotal = mStreamEvent.readContentLength;
		}



	// Update the progress state
	//
	// The content length value may not be supplied by the server if we're reading
	// a response, and so we may not be able to calculate any useful progress.
	mStreamEvent.readContentDone += bytesRead;
	
	if (bytesTotal != 0)
		{
		mStreamEvent.progressBytesLeft   = bytesTotal - bytesDone;
		mStreamEvent.progressPercentDone = (float) bytesDone / (float) bytesTotal;
		}



	// Estimate the transfer rate/ETA
	//
	// We require a minimum progress amount before we start trying to calculate this
	// level of progress info, to let the connection do a useful amount of work first.
	//
	// This also handles the no-progress-avaialble situation, as above.
	if (mStreamEvent.progressPercentDone > 0)
		{
		timeNow = GetCurrentEventTime();
		if (timeNow > (mStreamUpdateTime + kProgressUpdate))
			{
			if (NMathUtilities::NotZero(mStreamUpdateTime))
				{
				bytesDelta = bytesDone  - mStreamUpdateDone;
				timeDelta  = timeNow    - mStreamUpdateTime;
		
				mStreamEvent.progressBytesPerSec = (UInt32) ((float) bytesDelta / timeDelta);

				if (mStreamEvent.progressBytesLeft == 0)
					mStreamEvent.progressTimeLeft = 0.0;

				else if (mStreamEvent.progressBytesPerSec == 0)
					mStreamEvent.progressTimeLeft = kEventDurationForever;
				
				else
					mStreamEvent.progressTimeLeft = (float) mStreamEvent.progressBytesLeft / (float) mStreamEvent.progressBytesPerSec;
				}

			mStreamUpdateDone = bytesDone;
			mStreamUpdateTime = timeNow;
			}
		}
}





//============================================================================
//		NClientHTTP::DoMessage : Handle messages.
//----------------------------------------------------------------------------
#pragma mark -
void NClientHTTP::DoMessage(BroadcastMsg theMsg, void *msgData)
{


	// Handle the message
	switch (theMsg) {
		case kMsgCFStreamEvent:
			DoStreamEvent(*((CFStreamEventType *) msgData));
			break;
		
		default:
			break;
		}
}





//============================================================================
//		NClientHTTP::GetDefaultUserAgent : Get the default user agent.
//----------------------------------------------------------------------------
#pragma mark -
NString NClientHTTP::GetDefaultUserAgent(void)
{	NString				appName, appVersion, macPlatform;
	UInt32				macSpeed, macMemory, macArch;
	SystemVersion		sysVersion;
	NCFBundle			appBundle;
	NString				theResult;



	// Get the state we need
	appName    = appBundle.GetInfoString(kCFBundleExecutableKey);
	appVersion = appBundle.GetInfoString(kCFBundleVersionKey);
	sysVersion = NSystemUtilities::GetSystemVersion();
	macSpeed   = NSystemUtilities::GetGestaltValue(gestaltProcClkSpeedMHz);
	macMemory  = NSystemUtilities::GetGestaltValue(gestaltPhysicalRAMSizeInMegabytes);
	macArch    = NSystemUtilities::GetGestaltValue(gestaltSysArchitecture);



	// Build the platform string
	switch (macArch) {
		case gestaltPowerPC:
			macPlatform = "PPC";
			break;

		case gestaltIntel:
			macPlatform = "x86";
			break;
		
		default:
			macPlatform.Format("%d", macArch);
			break;
		}

	#if TARGET_RT_64_BIT
		macPlatform += "-64";
	#endif



	// Build the user agent
	theResult.Format("%@/%@ (Mac OS %d.%d.%d; %dMhz; %dMb; %@)",
						appName,
						appVersion,
						(UInt8) ((sysVersion >> 16) & 0xFF),
						(UInt8) ((sysVersion >>  8) & 0xFF),
						(UInt8) ((sysVersion >>  0) & 0xFF),
						macSpeed,
						macMemory,
						macPlatform);
	
	return(theResult);
}





//============================================================================
//		NClientHTTP::ResetStream : Reset the stream state.
//----------------------------------------------------------------------------
void NClientHTTP::ResetStream(void)
{


	// Reset the stream state
	mStreamOutput = NULL;

	mStreamEvent.theClient = this;
	mStreamEvent.theStream = &mStream;

	mStreamEvent.streamEvent = kCFStreamEventNone;
	mStreamEvent.streamMethod.Clear();
	mStreamEvent.streamResponse.Set(NULL);
	
	mStreamEvent.writeContentLength  = 0;
	mStreamEvent.writeContentDone    = 0;
	mStreamEvent.readContentLength   = 0;
	mStreamEvent.readContentDone     = 0;
	mStreamEvent.progressPercentDone = 0.0f;
	mStreamEvent.progressBytesPerSec = UINT_MAX;
	mStreamEvent.progressBytesLeft   = 0;
	mStreamEvent.progressTimeLeft    = 0;
	
	mStreamUpdateDone = 0;
	mStreamUpdateTime = 0.0;
}





//============================================================================
//		NClientHTTP::DoStreamEvent : Handle stream events.
//----------------------------------------------------------------------------
void NClientHTTP::DoStreamEvent(CFStreamEventType theEvent)
{	NCFObject	cfContentLength;
	


	// Update the stream state
	mStreamEvent.streamEvent = theEvent;

	if (!mStreamEvent.streamResponse.IsValid())
		mStreamEvent.streamResponse = mStream.GetProperty(kCFStreamPropertyHTTPResponseHeader);



	// Update the progress state
	if (mStreamEvent.streamMethod == kMethodPost)
		mStreamEvent.writeContentDone = mStream.GetPropertyUInt32(kCFStreamPropertyHTTPRequestBytesWrittenCount);

	if (mStreamEvent.streamResponse.IsValid() && mStreamEvent.readContentLength == 0)
		{
		if (cfContentLength.Set(CFHTTPMessageCopyHeaderFieldValue(mStreamEvent.streamResponse, kHeaderContentLength)))
			mStreamEvent.readContentLength = CFStringGetIntValue(cfContentLength);
		}

	UpdateProgress(0);



	// Handle the event
	switch (theEvent) {
		case kCFStreamEventHasBytesAvailable:
			// Read to the output stream
			//
			// Available data is transferred to the output stream, and our progress updated.
			if (mStreamOutput != NULL)
				{
				if (mStreamOutput->GetStatus() == kCFStreamStatusNotOpen)
					mStreamOutput->Open();
					
				UpdateProgress(mStream.TransferData(*mStreamOutput));
				}
			break;


		case kCFStreamEventErrorOccurred:
		case kCFStreamEventEndEncountered:
			// Clean up
			mStream.Close();

			if (mStreamOutput != NULL && mStreamOutput->GetStatus() == kCFStreamStatusOpen)
				mStreamOutput->Close();
			break;


		default:
			break;
		}



	// Let everyone know
	BroadcastMessage(kMsgClientHTTPEvent, &mStreamEvent);
}

