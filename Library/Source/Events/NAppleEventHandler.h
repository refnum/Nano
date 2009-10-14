/*	NAME:
		NAppleEventHandler.h

	DESCRIPTION:
		Apple Event handler.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NAPPLEEVENTHANDLER_HDR
#define NAPPLEEVENTHANDLER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCarbonEventHandler.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NAppleEventHandler {
public:
										NAppleEventHandler(void);
	virtual								~NAppleEventHandler(void);


	// Install the handler
	//
	// A list of events to register for must also be supplied, terminated
	// with { kEventClassNone, kEventKindNone };
	OSStatus							InstallHandler(const EventTypeSpec *theEvents);


	// Remove the handler
	void								RemoveHandler(void);


	// Dispatch an event
	//
	// Disposes the event to the appropriate DoAppleEventXXXX method.
	OSStatus							DispatchEvent(const AppleEvent &theEvent);


protected:
	// Handle events
	//
	// All event decoding and dispatch is handled by the base class. Sub-classes
	// should install their handler, register for events, and then override the
	// relevant methods to process those events.
	//
	// Unknown events (e.g., custom application events) are passed to DoEvent for
	// processing if no other method handles the event.
	virtual OSStatus					DoAppleEvent(const AppleEvent &theEvent);

	virtual OSStatus					DoAppleEventOpenApp(  const AppleEvent &theEvent);
	virtual OSStatus					DoAppleEventOpenDocs( const AppleEvent &theEvent);
	virtual OSStatus					DoAppleEventPrintDocs(const AppleEvent &theEvent);
	virtual OSStatus					DoAppleEventQuitApp(  const AppleEvent &theEvent);
	virtual OSStatus					DoAppleEventReopenApp(const AppleEvent &theEvent);

	virtual OSStatus					DoAppleEventGetURL(const AppleEvent &theEvent);


private:
	UInt32								GetEventClass(const AppleEvent &theEvent);
	UInt32								GetEventKind( const AppleEvent &theEvent);
	
	static OSErr						EventHandler(const AppleEvent	*theEvent,
														   AppleEvent	*theReply,
														   long			userData);


private:
	EventTypeSpecList					mEvents;

	AEEventHandlerUPP					mEventHandlerUPP;
};





#endif // NAPPLEEVENTHANDLER_HDR


