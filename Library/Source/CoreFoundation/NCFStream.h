/*	NAME:
		NCFStream.h

	DESCRIPTION:
		CoreFoundation stream object.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCFSTREAM_HDR
#define NCFSTREAM_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NMsgBroadcaster.h"
#include "NCFObject.h"
#include "NFile.h"
#include "NData.h"
#include "NString.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Messages
static const BroadcastMsg kMsgCFStreamEvent							= 'sevt';


// Streams 
typedef enum {
	kCFStreamRead,
	kCFStreamWrite
} CFStreamType;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCFStream : public NCFObject, public NMsgBroadcaster {
public:
										NCFStream(UInt32 theSize, const void *thePtr, CFStreamType theType=kCFStreamRead);
										NCFStream(const NFile &theFile,               CFStreamType theType=kCFStreamRead);
										NCFStream(CFReadStreamRef  cfStream);
										NCFStream(CFWriteStreamRef cfStream=NULL);

	virtual								~NCFStream(void);


	// Is this a read/write stream?
	bool								IsRead( void) const;
	bool								IsWrite(void) const;


	// Get the current status
	CFStreamStatus						GetStatus(void) const;


	// Get the current error
	CFStreamError						GetError(void) const;


	// Open/close the stream
	bool								Open( void);
	void								Close(void);


	// Can data be read/written from the stream?
	//
	// Returns as the stream can be read/written without blocking.
	bool								CanRead( void) const;
	bool								CanWrite(void) const;


	// Read/write the stream
	//
	// Reading and writing the stream will block unless data is available.
	//
	// Returns the number of bytes read/written, or -1 on error.
	SInt32								ReadData( UInt32 theSize,       void *thePtr);
	SInt32								WriteData(UInt32 theSize, const void *thePtr);


	// Transfer data between streams
	//
	// Transfers the available data between streams without blocking, returning the
	// total amount of data that was copied.
	//
	// If invoked on a read stream, the other stream must be a write stream that can
	// receive the data from the stream (and vice versa: if invoked on a write stream,
	// the other stream must be a read stream that can supply data for the stream).
	UInt32								TransferData(NCFStream &otherStream);


	// Get/set a property
	NCFObject							GetProperty(          const NString &theKey) const;
	bool								GetPropertyBoolean(   const NString &theKey) const;
	UInt32								GetPropertyUInt32(    const NString &theKey) const;
	NString								GetPropertyString(    const NString &theKey) const;
	NDictionary							GetPropertyDictionary(const NString &theKey) const;
	NData								GetPropertyData(      const NString &theKey) const;

	void								SetProperty(          const NString &theKey, CFTypeRef          theValue);
	void								SetPropertyBoolean(   const NString &theKey, bool               theValue);
	void								SetPropertyUInt32(    const NString &theKey, UInt32             theValue);
	void								SetPropertyString(    const NString &theKey, const NString     &theValue);
	void								SetPropertyDictionary(const NString &theKey, const NDictionary &theValue);
	void								SetPropertyData(      const NString &theKey, const NData       &theValue);


protected:
	// Handle an object change
	void								DoObjectChanging(void);
	void								DoObjectChanged( void);


private:
	void								InitializeSelf(void *cfStream, bool takeOwnership);
	
	void								AttachStream(void);
	void								DetachStream(void);

	static void							EventRead( CFReadStreamRef  cfStream, CFStreamEventType theEvent, void *userData);
	static void							EventWrite(CFWriteStreamRef cfStream, CFStreamEventType theEvent, void *userData);


private:


};







#endif // NCFSTREAM_HDR


