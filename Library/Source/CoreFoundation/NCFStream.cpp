/*	NAME:
		NCFStream.cpp

	DESCRIPTION:
		CoreFoundation stream object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <SystemConfiguration/SystemConfiguration.h>

#include "NThread.h"
#include "NCFStream.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt32 kTransferBlockSize								= 1024 * 256;

static const CFOptionFlags kCFStreamEventsAll						= kCFStreamEventOpenCompleted     |
																	  kCFStreamEventHasBytesAvailable |
																	  kCFStreamEventCanAcceptBytes    |
																	  kCFStreamEventErrorOccurred     | 
																	  kCFStreamEventEndEncountered;





//============================================================================
//		NCFStream::NCFStream : Constructor.
//----------------------------------------------------------------------------
NCFStream::NCFStream(UInt32 theSize, const void *thePtr, CFStreamType theType)
{	void	*cfStream;



	// Validate our parameters
	if (theType == kCFStreamRead)
		{
		NN_ASSERT(theSize >  0);
		NN_ASSERT(thePtr  != NULL);
		}



	// Create the stream
	//
	// If no buffer is supplied for a write stream, the stream will allocate its own
	// buffer that can be retrieved with the kCFStreamPropertyDataWritten property.
	if (theType == kCFStreamRead)
		cfStream = CFReadStreamCreateWithBytesNoCopy(kCFAllocatorNano, (const UInt8 *) thePtr, theSize, kCFAllocatorNull);
	else
		{
		if (theSize != 0 && thePtr != NULL)
			cfStream = CFWriteStreamCreateWithBuffer(kCFAllocatorNano, (UInt8 *) thePtr, theSize);
		else
			cfStream = CFWriteStreamCreateWithAllocatedBuffers(kCFAllocatorNano, kCFAllocatorNano);
		}



	// Initialize ourselves
	InitializeSelf(cfStream, true);
}





//============================================================================
//		NCFStream::NCFStream : Constructor.
//----------------------------------------------------------------------------
NCFStream::NCFStream(const NFile &theFile, CFStreamType theType)
{	void	*cfStream;



	// Create the stream
	if (theType == kCFStreamRead)
		cfStream = CFReadStreamCreateWithFile( kCFAllocatorNano, theFile.GetURL());
	else
		cfStream = CFWriteStreamCreateWithFile(kCFAllocatorNano, theFile.GetURL());



	// Initialize ourselves
	InitializeSelf(cfStream, true);
}





//============================================================================
//		NCFStream::NCFStream : Constructor.
//----------------------------------------------------------------------------
NCFStream::NCFStream(CFReadStreamRef cfStream)
{


	// Initialize ourselves
	InitializeSelf(cfStream, false);
}





//============================================================================
//		NCFStream::NCFStream : Constructor.
//----------------------------------------------------------------------------
NCFStream::NCFStream(CFWriteStreamRef cfStream)
{


	// Initialize ourselves
	InitializeSelf(cfStream, false);
}





//============================================================================
//		NCFStream::~NCFStream : Destructor.
//----------------------------------------------------------------------------
NCFStream::~NCFStream(void)
{


	// Clean up
	if (IsValid())
		{
		DetachStream();

		if (GetStatus() != kCFStreamStatusClosed)
			Close();
		}
}





//============================================================================
//		NCFStream::IsRead : Is this a read stream?
//----------------------------------------------------------------------------
bool NCFStream::IsRead(void) const
{


	// Get our state
	return(IsValid() && GetType() == CFReadStreamGetTypeID());
}





//============================================================================
//		NCFStream::IsWrite : Is this a write stream?
//----------------------------------------------------------------------------
bool NCFStream::IsWrite(void) const
{


	// Get our state
	return(IsValid() && GetType() == CFWriteStreamGetTypeID());
}





//============================================================================
//		NCFStream::GetStatus : Get the stream status.
//----------------------------------------------------------------------------
CFStreamStatus NCFStream::GetStatus(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());
	


	// Get the status
	if (IsRead())
		return(CFReadStreamGetStatus( *this));
	else
		return(CFWriteStreamGetStatus(*this));
}





//============================================================================
//		NCFStream::GetError : Get the stream error.
//----------------------------------------------------------------------------
CFStreamError NCFStream::GetError(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());
	


	// Get the error
	if (IsRead())
		return(CFReadStreamGetError( *this));
	else
		return(CFWriteStreamGetError(*this));
}





//============================================================================
//		NCFStream::Open : Open the stream.
//----------------------------------------------------------------------------
bool NCFStream::Open(void)
{


	// Validate our state
	NN_ASSERT(IsValid());
	NN_ASSERT(GetStatus() == kCFStreamStatusNotOpen);
	


	// Open the stream
	if (IsRead())
		return(CFReadStreamOpen( *this));
	else
		return(CFWriteStreamOpen(*this));
}





//============================================================================
//		NCFStream::Close : Close the stream.
//----------------------------------------------------------------------------
void NCFStream::Close(void)
{


	// Validate our state
	NN_ASSERT(IsValid());
	NN_ASSERT(GetStatus() != kCFStreamStatusClosed);



	// Close the stream
	if (IsRead())
		CFReadStreamClose( *this);
	else
		CFWriteStreamClose(*this);
}





//============================================================================
//		NCFStream::CanRead : Can the stream be read?
//----------------------------------------------------------------------------
bool NCFStream::CanRead(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());
	


	// Check the stream state
	if (IsRead())
		return(CFReadStreamHasBytesAvailable(*this));
	else
		return(false);
}





//============================================================================
//		NCFStream::CanWrite : Can the stream be written to?
//----------------------------------------------------------------------------
bool NCFStream::CanWrite(void) const
{


	// Validate our state
	NN_ASSERT(IsValid());
	


	// Check the stream state
	if (IsRead())
		return(false);
	else
		return(CFWriteStreamCanAcceptBytes(*this));
}





//============================================================================
//		NCFStream::ReadData : Read the stream.
//----------------------------------------------------------------------------
SInt32 NCFStream::ReadData(UInt32 theSize, void *thePtr)
{


	// Validate our state
	NN_ASSERT(IsValid());
	


	// Read from the stream
	if (IsRead())
		return(CFReadStreamRead(*this, (UInt8 *) thePtr, theSize));
	else
		return(-1);
}





//============================================================================
//		NCFStream::WriteData : Write the stream.
//----------------------------------------------------------------------------
SInt32 NCFStream::WriteData(UInt32 theSize, const void *thePtr)
{


	// Validate our state
	NN_ASSERT(IsValid());
	


	// Write to the stream
	if (IsRead())
		return(-1);
	else
		return(CFWriteStreamWrite(*this, (const UInt8 *) thePtr, theSize));
}





//============================================================================
//		NCFStream::TransferData : Transfer data between streams.
//----------------------------------------------------------------------------
UInt32 NCFStream::TransferData(NCFStream &otherStream)
{	SInt32			numRead, numWritten, theTotal;
	NCFStream		*readStream, *writeStream;
	NData			theBuffer;
	


	// Validate our parameters and state
	NN_ASSERT((IsRead() && otherStream.IsWrite()) || (IsWrite() && otherStream.IsRead()));



	// Get the state we need
	if (!theBuffer.SetSize(kTransferBlockSize))
		return(0);

	if (IsRead())
		{
		readStream  = this;
		writeStream = &otherStream;
		}
	else
		{
		readStream  = &otherStream;
		writeStream = this;
		}



	// Check we can transfer
	if (!readStream->CanRead() || !writeStream->CanWrite())
		return(0);



	// Transfer the data
	//
	// Unfortunately CFStream does not provide a way to tell how much data is available
	// prior to reading, and so we read fixed size blocks until the stream is exhausted.
	theTotal = 0;

	while (readStream->CanRead())
		{
		numRead = readStream->ReadData(theBuffer.GetSize(), theBuffer.GetData());
		if (numRead == -1)
			break;
		
		numWritten = writeStream->WriteData(numRead, theBuffer.GetData());
		if (numWritten == -1)
			break;

		theTotal += numWritten;
		}

	return(theTotal);
}





//============================================================================
//		NCFStream::GetProperty : Get a property.
//----------------------------------------------------------------------------
NCFObject NCFStream::GetProperty(const NString &theKey) const
{	NCFObject	theValue;



	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Get the property
	if (IsRead())
		theValue.Set(CFReadStreamCopyProperty( *this, theKey));
	else
		theValue.Set(CFWriteStreamCopyProperty(*this, theKey));
	
	return(theValue);
}





//============================================================================
//		NCFStream::GetPropertyBoolean : Get a boolean property.
//----------------------------------------------------------------------------
bool NCFStream::GetPropertyBoolean(const NString &theKey) const
{	bool		theValue;
	NCFObject	cfValue;



	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Get the property
	theValue = false;
	cfValue  = GetProperty(theKey);
	
	if (cfValue.IsValid())
		theValue = CFBooleanGetValue(cfValue);

	return(theValue);
}





//============================================================================
//		NCFStream::GetPropertyUInt32 : Get a UInt32 property.
//----------------------------------------------------------------------------
UInt32 NCFStream::GetPropertyUInt32(const NString &theKey) const
{	UInt32		theValue;
	NCFObject	cfValue;



	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Get the property
	theValue = 0;
	cfValue  = GetProperty(theKey);
	
	if (cfValue.IsValid())
		CFNumberGetValue(cfValue, kCFNumberSInt32Type, &theValue);

	return(theValue);
}





//============================================================================
//		NCFStream::GetPropertyString : Get a string property.
//----------------------------------------------------------------------------
NString NCFStream::GetPropertyString(const NString &theKey) const
{	NString		theValue;



	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Get the property
	theValue.Set(GetProperty(theKey), false);
	
	return(theValue);
}





//============================================================================
//		NCFStream::GetPropertyDictionary : Get a dictionary property.
//----------------------------------------------------------------------------
NDictionary NCFStream::GetPropertyDictionary(const NString &theKey) const
{	NDictionary	theValue;



	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Get the property
	theValue.Set(GetProperty(theKey), false);
	
	return(theValue);
}





//============================================================================
//		NCFStream::GetPropertyData : Get a data property.
//----------------------------------------------------------------------------
NData NCFStream::GetPropertyData(const NString &theKey) const
{	NData	theValue;



	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Get the property
	theValue.Set(GetProperty(theKey), false);
	
	return(theValue);
}





//============================================================================
//		NCFStream::SetProperty : Set a property.
//----------------------------------------------------------------------------
void NCFStream::SetProperty(const NString &theKey, CFTypeRef theValue)
{


	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Set the property
	if (IsRead())
		CFReadStreamSetProperty( *this, theKey, theValue);
	else
		CFWriteStreamSetProperty(*this, theKey, theValue);
}





//============================================================================
//		NCFStream::SetPropertyBoolean : Set a boolean property.
//----------------------------------------------------------------------------
void NCFStream::SetPropertyBoolean(const NString &theKey, bool theValue)
{


	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Set the property
	SetProperty(theKey, theValue ? kCFBooleanTrue : kCFBooleanFalse);
}





//============================================================================
//		NCFStream::SetPropertyUInt32 : Set a UInt32 property.
//----------------------------------------------------------------------------
void NCFStream::SetPropertyUInt32(const NString &theKey, UInt32 theValue)
{	NCFObject	cfValue;



	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Set the property
	if (cfValue.Set(CFNumberCreate(kCFAllocatorNano, kCFNumberSInt32Type, &theValue)))
		SetProperty(theKey, cfValue);
}





//============================================================================
//		NCFStream::SetPropertyString : Set a string property.
//----------------------------------------------------------------------------
void NCFStream::SetPropertyString(const NString &theKey, const NString &theValue)
{


	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Set the property
	SetProperty(theKey, theValue);
}





//============================================================================
//		NCFStream::SetPropertyDictionary : Set a dictionary property.
//----------------------------------------------------------------------------
void NCFStream::SetPropertyDictionary(const NString &theKey, const NDictionary &theValue)
{


	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Set the property
	SetProperty(theKey, theValue);
}





//============================================================================
//		NCFStream::SetPropertyData : Set a data property.
//----------------------------------------------------------------------------
void NCFStream::SetPropertyData(const NString &theKey, const NData &theValue)
{


	// Validate our parameters and state
	NN_ASSERT(theKey.IsNotEmpty());
	NN_ASSERT(IsValid());



	// Set the property
	SetProperty(theKey, theValue);
}





//============================================================================
//		NCFStream::DoObjectChanging : Handle an object change.
//----------------------------------------------------------------------------
#pragma mark -
void NCFStream::DoObjectChanging(void)
{


	// Detach from the current stream
	DetachStream();
}





//============================================================================
//		NCFStream::DoObjectChanged : Handle an object change.
//----------------------------------------------------------------------------
void NCFStream::DoObjectChanged(void)
{


	// Attach to the new stream
	AttachStream();
}





//============================================================================
//		NCFStream::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
void NCFStream::InitializeSelf(void *cfStream, bool takeOwnership)
{


	// Initialize ourselves
	Set(cfStream, takeOwnership);
	
	if (cfStream != NULL)
		NN_ASSERT(IsValid());
}





//============================================================================
//		NCFStream::AttachStream : Attach to the stream.
//----------------------------------------------------------------------------
void NCFStream::AttachStream(void)
{	CFStreamClientContext	clientInfo = { 0, this, NULL, NULL, NULL };
	NDictionary				theProxies;



	// Check our state
	if (!IsValid())
		return;



	// Register for events
	if (IsRead())
		{
		if (CFReadStreamSetClient(*this, kCFStreamEventsAll, EventRead, &clientInfo))
			CFReadStreamScheduleWithRunLoop(*this, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
		}
	else
		{
		if (CFWriteStreamSetClient(*this, kCFStreamEventsAll, EventWrite, &clientInfo))
			CFWriteStreamScheduleWithRunLoop(*this, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
		}



	// Apply the proxy state
	if (theProxies.Set(SCDynamicStoreCopyProxies(NULL)))
		{
		SetPropertyDictionary(kCFStreamPropertyHTTPProxy, theProxies);
		SetPropertyDictionary(kCFStreamPropertyFTPProxy,  theProxies);
		}
}





//============================================================================
//		NCFStream::DetachStream : Detach from the stream.
//----------------------------------------------------------------------------
void NCFStream::DetachStream(void)
{


	// Check our state
	if (!IsValid())
		return;



	// Stop getting events
	if (IsRead())
		{
		CFReadStreamSetClient(             *this, kCFStreamEventNone, NULL, NULL);
		CFReadStreamUnscheduleFromRunLoop( *this, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
		}
	else
		{
		CFWriteStreamSetClient(            *this, kCFStreamEventNone, NULL, NULL);
		CFWriteStreamUnscheduleFromRunLoop(*this, CFRunLoopGetCurrent(), kCFRunLoopCommonModes);
		}
}





//============================================================================
//		NCFStream::EventRead : Stream read event callback.
//----------------------------------------------------------------------------
void NCFStream::EventRead(CFReadStreamRef /*cfStream*/, CFStreamEventType theEvent, void *userData)
{	NCFStream	*thisPtr = (NCFStream *) userData;



	// Validate our state
	NN_ASSERT(thisPtr->IsRead());



	// Let everyone know
	BroadcastEvent(thisPtr, theEvent);
}





//============================================================================
//		NCFStream::EventWrite : Stream write event callback.
//----------------------------------------------------------------------------
void NCFStream::EventWrite(CFWriteStreamRef /*cfStream*/, CFStreamEventType theEvent, void *userData)
{	NCFStream	*thisPtr = (NCFStream *) userData;



	// Validate our state
	NN_ASSERT(thisPtr->IsWrite());



	// Let everyone know
	BroadcastEvent(thisPtr, theEvent);
}





//============================================================================
//		NCFStream::BroadcastEvent : Broadcast a stream event.
//----------------------------------------------------------------------------
void NCFStream::BroadcastEvent(NCFStream *thisPtr, CFStreamEventType theEvent)
{


	// Let everyone know
	//
	// CFStream can cause the main runloop to crash if a thread schedules a
	// stream on the runloop of a different thread (rdar://5916811).
	//
	// To work around this, all streams are scheduled on the current run loop
	// and events are then passed to the main thread for broadcasting.
	NThread::InvokeMain(BindMethod(thisPtr, NCFStream::BroadcastMessage, kMsgCFStreamEvent, &theEvent));
}


