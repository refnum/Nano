/*	NAME:
		NAppleEventHandler.cpp

	DESCRIPTION:
		Apple Event handler.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NAppleEventHandler.h"





//============================================================================
//      Internal macros
//----------------------------------------------------------------------------
// Default event handler
#define DEFAULT_EVENT_METHOD(_method)										\
	OSStatus NAppleEventHandler::_method(const NAppleEvent &/*theEvent*/)	\
	{																		\
		return(eventNotHandledErr);											\
	}


// Dispatch an event
#define DISPATCH_EVENT_TO_METHOD(_type, _method)							\
	case _type:																\
		theErr = _method(theEvent);											\
		break





//============================================================================
//		NAppleEventHandler::NAppleEventHandler : Constructor.
//----------------------------------------------------------------------------
NAppleEventHandler::NAppleEventHandler(void)
{
}





//============================================================================
//		NAppleEventHandler::~NAppleEventHandler : Destructor.
//----------------------------------------------------------------------------
NAppleEventHandler::~NAppleEventHandler(void)
{


	// Remove the handler
	RemoveHandler();
}





//============================================================================
//		NAppleEventHandler::InstallHandler : Install the handler.
//----------------------------------------------------------------------------
OSStatus NAppleEventHandler::InstallHandler(const EventTypeSpec *theEvents)
{	const EventTypeSpec		*theEvent;
	bool					isKnown;
	OSStatus				theErr;



	// Validate our parameters
	NN_ASSERT(theEvents != NULL);



	// Install the handler
	//
	// We can only install the handler for recognized events, since Apple
	// Events are installed individually rather than on a generic handler.
	theEvent = theEvents;
	theErr   = noErr;
	
	while (theEvent->eventClass != kEventClassNone)
		{
		// Examine the event
		isKnown = false;
		
		switch (theEvent->eventClass) {
			case kCoreEventClass:
				switch (theEvent->eventKind) {
					case kAEOpenApplication:
					case kAEOpenDocuments:
					case kAEPrintDocuments:
					case kAEQuitApplication:
					case kAEReopenApplication:
						isKnown = true;
						break;
					}
				break;
			
			case kInternetEventClass:
				switch(theEvent->eventKind) {
					case kAEGetURL:
						isKnown = true;
						break;
				}
				break;
			}
		
		
		
		// Install the handler
		NN_ASSERT(isKnown);
		
		if (isKnown)
			{
			theErr = AEInstallEventHandler(theEvent->eventClass, theEvent->eventKind,
											EventHandler, (long) this, false);
			NN_ASSERT_NOERR(theErr);
			}



		// Save the event
		mEvents.push_back(*theEvent);

		theEvent++;
		}
	return(theErr);
}		





//============================================================================
//		NAppleEventHandler::RemoveHandler : Remove the handler.
//----------------------------------------------------------------------------
void NAppleEventHandler::RemoveHandler(void)
{	EventTypeSpecListConstIterator		theIter;
	OSStatus							theErr;



	// Remove the handler
	for (theIter = mEvents.begin(); theIter != mEvents.end(); theIter++)
		{
		theErr = AERemoveEventHandler(theIter->eventClass, theIter->eventKind, EventHandler, false);
		NN_ASSERT_NOERR(theErr);
		}
	
	mEvents.clear();
}





//============================================================================
//		NAppleEventHandler::DispatchEvent : Dispatch an event.
//----------------------------------------------------------------------------
OSStatus NAppleEventHandler::DispatchEvent(const NAppleEvent &theEvent)
{	OSStatus	theErr;



	// Dispatch the event
	theErr = eventNotHandledErr;
	
	switch (theEvent.GetClass()) {
		case kCoreEventClass:
			switch (theEvent.GetID()) {
				DISPATCH_EVENT_TO_METHOD(kAEOpenApplication,   DoAppleEventOpenApp);
				DISPATCH_EVENT_TO_METHOD(kAEOpenDocuments,     DoAppleEventOpenDocs);
				DISPATCH_EVENT_TO_METHOD(kAEPrintDocuments,    DoAppleEventPrintDocs);
				DISPATCH_EVENT_TO_METHOD(kAEQuitApplication,   DoAppleEventQuitApp);
				DISPATCH_EVENT_TO_METHOD(kAEReopenApplication, DoAppleEventReopenApp);
				}
			break;
		
		case kInternetEventClass:
			switch (theEvent.GetID()) {
				DISPATCH_EVENT_TO_METHOD(kAEGetURL, DoAppleEventGetURL);
				}
			break;
		}



	// Dispatch un-handled events
	if (theErr == eventNotHandledErr)
		theErr = DoAppleEvent(theEvent);
	
	return(theErr);
}





//============================================================================
//		NAppleEventHandler::DoAppleEventXXX : Handle events.
//----------------------------------------------------------------------------
//		Note :	As the system events are quite numerous, and will probably be
//				expanded over time, we use a macro to provide the default
//				implementations.
//----------------------------------------------------------------------------
#pragma mark -
DEFAULT_EVENT_METHOD(DoAppleEvent)

DEFAULT_EVENT_METHOD(DoAppleEventOpenApp)
DEFAULT_EVENT_METHOD(DoAppleEventOpenDocs)
DEFAULT_EVENT_METHOD(DoAppleEventPrintDocs)
DEFAULT_EVENT_METHOD(DoAppleEventQuitApp)
DEFAULT_EVENT_METHOD(DoAppleEventReopenApp)

DEFAULT_EVENT_METHOD(DoAppleEventGetURL)





//============================================================================
//		NAppleEventHandler::EventHandler : Apple Event callback.
//----------------------------------------------------------------------------
#pragma mark -
OSErr NAppleEventHandler::EventHandler(const AppleEvent		*theEvent,
											 AppleEvent		*/*theReply*/,
											 long			userData)
{	NAppleEventHandler		*thisPtr = (NAppleEventHandler *) userData;
	OSStatus				theErr;



	// Validate our parameters
	NN_ASSERT(theEvent != NULL);
	NN_ASSERT(thisPtr);
	


	// Dispatch the event
	theErr = thisPtr->DispatchEvent(*theEvent);
	
	return(theErr);
}	
