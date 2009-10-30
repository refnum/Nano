/*	NAME:
		NCarbonEventHandler.cpp

	DESCRIPTION:
		Carbon Event handler.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <algorithm>

#include "NSTLUtilities.h"
#include "NCarbonEventHandler.h"





//============================================================================
//      Internal macros
//----------------------------------------------------------------------------
// Default event handler
#define DEFAULT_EVENT_METHOD(_method)										\
	OSStatus NCarbonEventHandler::_method(NCarbonEvent &/*theEvent*/)		\
	{																		\
		return(eventNotHandledErr);											\
	}


// Dispatch an event
#define DISPATCH_EVENT_TO_METHOD(_type, _method)							\
	case _type:																\
		theErr = _method(theEvent);											\
		break





//============================================================================
//      Static variables
//----------------------------------------------------------------------------
EventHandlerUPP NCarbonEventHandler::mEventHandlerUPP = NewEventHandlerUPP(EventHandler);





//============================================================================
//		NCarbonEventHandler::NCarbonEventHandler : Constructor.
//----------------------------------------------------------------------------
NCarbonEventHandler::NCarbonEventHandler(void)
{


	// Initialize ourselves
	mEventHandler = NULL;
	
	mIsDead      = NULL;
	mDelegate    = NULL;
	mNextHandler = NULL;
}





//============================================================================
//		NCarbonEventHandler::~NCarbonEventHandler : Destructor.
//----------------------------------------------------------------------------
NCarbonEventHandler::~NCarbonEventHandler(void)
{


	// Remove the handler
	RemoveHandler();



	// Report our death
	if (mIsDead != NULL)
		*mIsDead = true;
}





//============================================================================
//		NCarbonEventHandler::InstallHandler : Install the event handler.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::InstallHandler(const EventTypeSpec *theEvents)
{


	// Install the handler
	return(InstallHandler(GetApplicationEventTarget(), theEvents));
}





//============================================================================
//		NCarbonEventHandler::InstallHandler : Install the event handler.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::InstallHandler(MenuRef theMenu, const EventTypeSpec *theEvents)
{


	// Validate our parameters
	NN_ASSERT(IsValidMenu(theMenu));



	// Install the handler
	return(InstallHandler(GetMenuEventTarget(theMenu), theEvents));
}





//============================================================================
//		NCarbonEventHandler::InstallHandler : Install the event handler.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::InstallHandler(HIWindowRef theWindow, const EventTypeSpec *theEvents)
{


	// Validate our parameters
	NN_ASSERT(IsValidWindowPtr(theWindow));



	// Install the handler
	return(InstallHandler(GetWindowEventTarget(theWindow), theEvents));
}





//============================================================================
//		NCarbonEventHandler::InstallHandler : Install the event handler.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::InstallHandler(ControlRef theControl, const EventTypeSpec *theEvents)
{


	// Validate our parameters
	NN_ASSERT(IsValidControlHandle(theControl));



	// Install the handler
	return(InstallHandler(GetControlEventTarget(theControl), theEvents));
}





//============================================================================
//		NCarbonEventHandler::InstallHandler : Install the event handler.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::InstallHandler(HIObjectRef theObject, const EventTypeSpec *theEvents)
{


	// Validate our parameters
	NN_ASSERT(theObject != NULL);



	// Install the handler
	return(InstallHandler(HIObjectGetEventTarget(theObject), theEvents));
}





//============================================================================
//		NCarbonEventHandler::RemoveHandler : Remove the handler.
//----------------------------------------------------------------------------
void NCarbonEventHandler::RemoveHandler(void)
{


	// Remove the handler
	if (mEventHandler != NULL)
		{
		RemoveEventHandler(mEventHandler);
		mEventHandler = NULL;
		}
}





//============================================================================
//		NCarbonEventHandler::GetEventDelegate : Get the delegate.
//----------------------------------------------------------------------------
NCarbonEventHandler *NCarbonEventHandler::GetEventDelegate(void) const
{


	// Get the delegate
	return(mDelegate);
}





//============================================================================
//		NCarbonEventHandler::SetEventDelegate : Set the delegate.
//----------------------------------------------------------------------------
void NCarbonEventHandler::SetEventDelegate(NCarbonEventHandler *theDelegate)
{


	// Validate our parameters
	NN_ASSERT(theDelegate != this);



	// Set the delegate
	mDelegate = theDelegate;
}





//============================================================================
//		NCarbonEventHandler::AddEvent : Add an event.
//----------------------------------------------------------------------------
void NCarbonEventHandler::AddEvent(UInt32 theClass, UInt32 theKind)
{	EventTypeSpec		theEvent = { theClass, theKind };
	EventTypeSpecList	eventList;



	// Add the event
	eventList.push_back(theEvent);

	AddEvents(eventList);
}





//============================================================================
//		NCarbonEventHandler::AddEvents : Add events.
//----------------------------------------------------------------------------
void NCarbonEventHandler::AddEvents(const EventTypeSpec *theEvents)
{	EventTypeSpecList	eventList;



	// Add the events
	if (theEvents != NULL)
		{
		while (theEvents->eventClass != kEventClassNone && theEvents->eventKind != kEventKindNone)
			eventList.push_back(*theEvents++);
		}

	AddEvents(eventList);
}





//============================================================================
//		NCarbonEventHandler::AddEvents : Add events.
//----------------------------------------------------------------------------
void NCarbonEventHandler::AddEvents(const EventTypeSpecList &theEvents)
{	OSStatus	theErr;



	// Add the events
	if (!theEvents.empty())
		{
		// Update the handler
		//
		// Events can be added before a handler has been installed; they are saved
		// for later, to be installed along with the handler when it is applied.
		if (mEventHandler != NULL)
			{
			theErr = AddEventTypesToHandler(mEventHandler, theEvents.size(), &theEvents[0]);
			NN_ASSERT_NOERR(theErr);
			}



		// Update our state
        append(mEvents, theEvents); 

		if (mEventHandler != NULL)
			DoEventsChanged();
		}
}





//============================================================================
//		NCarbonEventHandler::RemoveEvent : Remove an event.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::RemoveEvent(UInt32 theClass, UInt32 theKind)
{	EventTypeSpec		theEvent = { theClass, theKind };
	EventTypeSpecList	eventList;



	// Remove the event
	eventList.push_back(theEvent);

	return(RemoveEvents(eventList));
}





//============================================================================
//		NCarbonEventHandler::RemoveEvents : Remove events.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::RemoveEvents(const EventTypeSpec *theEvents)
{	EventTypeSpecList	eventList;



	// Remove the events
	if (theEvents != NULL)
		{
		while (theEvents->eventClass != kEventClassNone && theEvents->eventKind != kEventKindNone)
			eventList.push_back(*theEvents++);
		}

	return(RemoveEvents(eventList));
}





//============================================================================
//		NCarbonEventHandler::RemoveEvents : Remove events.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::RemoveEvents(const EventTypeSpecList &theEvents)
{	EventTypeSpecListConstIterator	theIter;
	OSStatus						theErr;



	// Update the handler
	theErr = noErr;

	if (mEventHandler != NULL && !theEvents.empty())
		theErr = RemoveEventTypesFromHandler(mEventHandler, theEvents.size(), &theEvents[0]);



	// Update our state
	if (theErr == noErr)
		{
		for (theIter = theEvents.begin(); theIter != theEvents.end(); theIter++)
			mEvents.erase(std::remove(mEvents.begin(), mEvents.end(), *theIter), mEvents.end());

		if (mEventHandler != NULL)
			DoEventsChanged();
		}
	
	return(theErr);
}





//============================================================================
//		NCarbonEventHandler::IsEventActive : Is an event active?
//----------------------------------------------------------------------------
bool NCarbonEventHandler::IsEventActive(UInt32 theClass, UInt32 theKind) const
{	EventTypeSpecListConstIterator	theIter;



	// Check the event list
	for (theIter = mEvents.begin(); theIter != mEvents.end(); theIter++)
		{
		if (theIter->eventClass == theClass && theIter->eventKind == theKind)
			return(true);
		}
	
	return(false);
}





//============================================================================
//		NCarbonEventHandler::DispatchEvent : Dispatch an event.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::DispatchEvent(NCarbonEvent &theEvent, EventHandlerCallRef callRef)
{	bool		isDead, *oldFlag;
	OSStatus	theErr;



	// Initialize ourselves
	mNextHandler = callRef;
	theErr       = eventNotHandledErr;



	// Set the is-dead flag
	//
	// To allow this object to be deleted as a result of handling an event, we need
	// to watch for our destructor being called while we are dispatching the event.
	//
	// We do this by setting a member variable to point to a variable on our stack,
	// which our destructor will update when it sees it is non-NULL.
	//
	// To allow re-entrancy, we also need to save the previous value of the flag on
	// our stack and restore it when we're done.
	isDead = false;

	oldFlag = mIsDead;
	mIsDead = &isDead;



	// Inform the delegate
	//
	// Delegates get to see the event first, to allow them to consume it if necessary.
	//
	// The exception this is the HIObject class; these events are always sent directly
	// to the target object, since tasks such as construction/destruction should only
	// be handled once and only by the target object.
	if (mDelegate != NULL)
		{
		if (theEvent.GetClass() != kEventClassHIObject)
			theErr = mDelegate->DispatchEvent(theEvent, callRef);
		}



	// Dispatch the event
	if (theErr == eventNotHandledErr)
		{
		switch (theEvent.GetClass()) {
			case kEventClassAccessibility:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventAccessibleGetAllActionNames,					DoEventAccessibleGetAllActionNames);
					DISPATCH_EVENT_TO_METHOD(kEventAccessibleGetAllAttributeNames,				DoEventAccessibleGetAllAttributeNames);
					DISPATCH_EVENT_TO_METHOD(kEventAccessibleGetAllParameterizedAttributeNames,	DoEventAccessibleGetAllParameterizedAttributeNames);
					DISPATCH_EVENT_TO_METHOD(kEventAccessibleGetChildAtPoint,					DoEventAccessibleGetChildAtPoint);
					DISPATCH_EVENT_TO_METHOD(kEventAccessibleGetFocusedChild,					DoEventAccessibleGetFocusedChild);
					DISPATCH_EVENT_TO_METHOD(kEventAccessibleGetNamedActionDescription,			DoEventAccessibleGetNamedActionDescription);
					DISPATCH_EVENT_TO_METHOD(kEventAccessibleGetNamedAttribute,					DoEventAccessibleGetNamedAttribute);
					DISPATCH_EVENT_TO_METHOD(kEventAccessibleIsNamedAttributeSettable,			DoEventAccessibleIsNamedAttributeSettable);
					DISPATCH_EVENT_TO_METHOD(kEventAccessiblePerformNamedAction,				DoEventAccessiblePerformNamedAction);
					DISPATCH_EVENT_TO_METHOD(kEventAccessibleSetNamedAttribute,					DoEventAccessibleSetNamedAttribute);
					}
				break;

			case kEventClassApplication:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventAppActivated,					DoEventAppActivated);
					DISPATCH_EVENT_TO_METHOD(kEventAppActiveWindowChanged,			DoEventAppActiveWindowChanged);
					DISPATCH_EVENT_TO_METHOD(kEventAppAvailableWindowBoundsChanged,	DoEventAppAvailableWindowBoundsChanged);
					DISPATCH_EVENT_TO_METHOD(kEventAppDeactivated,					DoEventAppDeactivated);
					DISPATCH_EVENT_TO_METHOD(kEventAppFocusDrawer,					DoEventAppFocusDrawer);
					DISPATCH_EVENT_TO_METHOD(kEventAppFocusMenuBar,					DoEventAppFocusMenuBar);
					DISPATCH_EVENT_TO_METHOD(kEventAppFocusNextDocumentWindow,		DoEventAppFocusNextDocumentWindow);
					DISPATCH_EVENT_TO_METHOD(kEventAppFocusNextFloatingWindow,		DoEventAppFocusNextFloatingWindow);
					DISPATCH_EVENT_TO_METHOD(kEventAppFocusToolbar,					DoEventAppFocusToolbar);
					DISPATCH_EVENT_TO_METHOD(kEventAppFrontSwitched,				DoEventAppFrontSwitched);
					DISPATCH_EVENT_TO_METHOD(kEventAppGetDockTileMenu,				DoEventAppGetDockTileMenu);
					DISPATCH_EVENT_TO_METHOD(kEventAppHidden,						DoEventAppHidden);
					DISPATCH_EVENT_TO_METHOD(kEventAppIsEventInInstantMouser,		DoEventAppIsEventInInstantMouser);
					DISPATCH_EVENT_TO_METHOD(kEventAppLaunched,						DoEventAppLaunched);
					DISPATCH_EVENT_TO_METHOD(kEventAppLaunchNotification,			DoEventAppLaunchNotification);
					DISPATCH_EVENT_TO_METHOD(kEventAppQuit,							DoEventAppQuit);
					DISPATCH_EVENT_TO_METHOD(kEventAppShown,						DoEventAppShown);
					DISPATCH_EVENT_TO_METHOD(kEventAppSystemUIModeChanged,			DoEventAppSystemUIModeChanged);
					DISPATCH_EVENT_TO_METHOD(kEventAppTerminated,					DoEventAppTerminated);
					}
				break;

			case kEventClassAppearance:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventAppearanceScrollBarVariantChanged, DoEventAppearanceScrollBarVariantChanged);
					}
				break;
	
			case kEventClassAppleEvent:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventAppleEvent, DoEventAppleEvent);
					}
				break;
	
			case kEventClassHIComboBox:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventComboBoxListItemSelected, DoEventComboBoxListItemSelected);
					}
				break;
	
			case kEventClassCommand:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventCommandProcess,		DoEventCommandProcess);
					DISPATCH_EVENT_TO_METHOD(kEventCommandUpdateStatus,	DoEventCommandUpdateStatus);
					}
				break;
	
			case kEventClassControl:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventControlActivate,						DoEventControlActivate);
					DISPATCH_EVENT_TO_METHOD(kEventControlAddedSubControl,				DoEventControlAddedSubControl);
					DISPATCH_EVENT_TO_METHOD(kEventControlApplyBackground,				DoEventControlApplyBackground);
					DISPATCH_EVENT_TO_METHOD(kEventControlApplyTextColor,				DoEventControlApplyTextColor);
					DISPATCH_EVENT_TO_METHOD(kEventControlArbitraryMessage,				DoEventControlArbitraryMessage);
					DISPATCH_EVENT_TO_METHOD(kEventControlBoundsChanged,				DoEventControlBoundsChanged);
					DISPATCH_EVENT_TO_METHOD(kEventControlClick,						DoEventControlClick);
					DISPATCH_EVENT_TO_METHOD(kEventControlContextualMenuClick,			DoEventControlContextualMenuClick);
					DISPATCH_EVENT_TO_METHOD(kEventControlDeactivate,					DoEventControlDeactivate);
					DISPATCH_EVENT_TO_METHOD(kEventControlDispose,						DoEventControlDispose);
					DISPATCH_EVENT_TO_METHOD(kEventControlDragEnter,					DoEventControlDragEnter);
					DISPATCH_EVENT_TO_METHOD(kEventControlDragLeave,					DoEventControlDragLeave);
					DISPATCH_EVENT_TO_METHOD(kEventControlDragReceive,					DoEventControlDragReceive);
					DISPATCH_EVENT_TO_METHOD(kEventControlDragWithin,					DoEventControlDragWithin);
					DISPATCH_EVENT_TO_METHOD(kEventControlDraw,							DoEventControlDraw);
					DISPATCH_EVENT_TO_METHOD(kEventControlEnabledStateChanged,			DoEventControlEnabledStateChanged);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetActionProcPart,			DoEventControlGetActionProcPart);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetAutoToggleValue,			DoEventControlGetAutoToggleValue);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetClickActivation,			DoEventControlGetClickActivation);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetData,						DoEventControlGetData);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetFocusPart,					DoEventControlGetFocusPart);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetFrameMetrics,				DoEventControlGetFrameMetrics);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetIndicatorDragConstraint,	DoEventControlGetIndicatorDragConstraint);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetNextFocusCandidate,		DoEventControlGetNextFocusCandidate);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetOptimalBounds,				DoEventControlGetOptimalBounds);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetPartBounds,				DoEventControlGetPartBounds);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetPartRegion,				DoEventControlGetPartRegion);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetScrollToHereStartPoint,	DoEventControlGetScrollToHereStartPoint);
					DISPATCH_EVENT_TO_METHOD(kEventControlGetSizeConstraints,			DoEventControlGetSizeConstraints);
					DISPATCH_EVENT_TO_METHOD(kEventControlGhostingFinished,				DoEventControlGhostingFinished);
					DISPATCH_EVENT_TO_METHOD(kEventControlHiliteChanged,				DoEventControlHiliteChanged);
					DISPATCH_EVENT_TO_METHOD(kEventControlHit,							DoEventControlHit);
					DISPATCH_EVENT_TO_METHOD(kEventControlHitTest,						DoEventControlHitTest);
					DISPATCH_EVENT_TO_METHOD(kEventControlIndicatorMoved,				DoEventControlIndicatorMoved);
					DISPATCH_EVENT_TO_METHOD(kEventControlInitialize,					DoEventControlInitialize);
					DISPATCH_EVENT_TO_METHOD(kEventControlInterceptSubviewClick,		DoEventControlInterceptSubviewClick);
					DISPATCH_EVENT_TO_METHOD(kEventControlInvalidateForSizeChange,		DoEventControlInvalidateForSizeChange);
					DISPATCH_EVENT_TO_METHOD(kEventControlLayoutInfoChanged,			DoEventControlLayoutInfoChanged);
					DISPATCH_EVENT_TO_METHOD(kEventControlOwningWindowChanged,			DoEventControlOwningWindowChanged);
					DISPATCH_EVENT_TO_METHOD(kEventControlRemovingSubControl,			DoEventControlRemovingSubControl);
					DISPATCH_EVENT_TO_METHOD(kEventControlSetCursor,					DoEventControlSetCursor);
					DISPATCH_EVENT_TO_METHOD(kEventControlSetData,						DoEventControlSetData);
					DISPATCH_EVENT_TO_METHOD(kEventControlSetFocusPart,					DoEventControlSetFocusPart);
					DISPATCH_EVENT_TO_METHOD(kEventControlSimulateHit,					DoEventControlSimulateHit);
					DISPATCH_EVENT_TO_METHOD(kEventControlTitleChanged,					DoEventControlTitleChanged);
					DISPATCH_EVENT_TO_METHOD(kEventControlTrack,						DoEventControlTrack);
					DISPATCH_EVENT_TO_METHOD(kEventControlTrackingAreaEntered,			DoEventControlTrackingAreaEntered);
					DISPATCH_EVENT_TO_METHOD(kEventControlTrackingAreaExited,			DoEventControlTrackingAreaExited);
					DISPATCH_EVENT_TO_METHOD(kEventControlValueFieldChanged,			DoEventControlValueFieldChanged);
					DISPATCH_EVENT_TO_METHOD(kEventControlVisibilityChanged,			DoEventControlVisibilityChanged);
					}
				break;
	
			case kEventClassFont:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventFontPanelClosed,		DoEventFontPanelClosed);
					DISPATCH_EVENT_TO_METHOD(kEventFontSelection,		DoEventFontSelection);
					}
				break;

			case kEventClassHIObject:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventHIObjectConstruct,		DoEventHIObjectConstruct);
					DISPATCH_EVENT_TO_METHOD(kEventHIObjectInitialize,		DoEventHIObjectInitialize);
					DISPATCH_EVENT_TO_METHOD(kEventHIObjectDestruct,		DoEventHIObjectDestruct);
					DISPATCH_EVENT_TO_METHOD(kEventHIObjectIsEqual,			DoEventHIObjectIsEqual);
					DISPATCH_EVENT_TO_METHOD(kEventHIObjectPrintDebugInfo,	DoEventHIObjectPrintDebugInfo);
					DISPATCH_EVENT_TO_METHOD(kEventHIObjectEncode,			DoEventHIObjectEncode);
					}
				break;
	
			case kEventClassKeyboard:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventHotKeyPressed,			DoEventHotKeyPressed);
					DISPATCH_EVENT_TO_METHOD(kEventHotKeyReleased,			DoEventHotKeyReleased);
					DISPATCH_EVENT_TO_METHOD(kEventRawKeyDown,				DoEventRawKeyDown);
					DISPATCH_EVENT_TO_METHOD(kEventRawKeyModifiersChanged,	DoEventRawKeyModifiersChanged);
					DISPATCH_EVENT_TO_METHOD(kEventRawKeyRepeat,			DoEventRawKeyRepeat);
					DISPATCH_EVENT_TO_METHOD(kEventRawKeyUp,				DoEventRawKeyUp);
					}
				break;
	
			case kEventClassInk:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventInkGesture,	DoEventInkGesture);
					DISPATCH_EVENT_TO_METHOD(kEventInkPoint,	DoEventInkPoint);
					DISPATCH_EVENT_TO_METHOD(kEventInkText,		DoEventInkText);
					}
				break;
	
			case kEventClassMenu:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventMenuBarHidden,			DoEventMenuBarHidden);
					DISPATCH_EVENT_TO_METHOD(kEventMenuBarShown,			DoEventMenuBarShown);
					DISPATCH_EVENT_TO_METHOD(kEventMenuBecomeScrollable,	DoEventMenuBecomeScrollable);
					DISPATCH_EVENT_TO_METHOD(kEventMenuBeginTracking,		DoEventMenuBeginTracking);
					DISPATCH_EVENT_TO_METHOD(kEventMenuCalculateSize,		DoEventMenuCalculateSize);
					DISPATCH_EVENT_TO_METHOD(kEventMenuCeaseToBeScrollable,	DoEventMenuCeaseToBeScrollable);
					DISPATCH_EVENT_TO_METHOD(kEventMenuChangeTrackingMode,	DoEventMenuChangeTrackingMode);
					DISPATCH_EVENT_TO_METHOD(kEventMenuClosed,				DoEventMenuClosed);
					DISPATCH_EVENT_TO_METHOD(kEventMenuCreateFrameView,		DoEventMenuCreateFrameView);
					DISPATCH_EVENT_TO_METHOD(kEventMenuDispose,				DoEventMenuDispose);
					DISPATCH_EVENT_TO_METHOD(kEventMenuDrawItem,			DoEventMenuDrawItem);
					DISPATCH_EVENT_TO_METHOD(kEventMenuDrawItemContent,		DoEventMenuDrawItemContent);
					DISPATCH_EVENT_TO_METHOD(kEventMenuEnableItems,			DoEventMenuEnableItems);
					DISPATCH_EVENT_TO_METHOD(kEventMenuEndTracking,			DoEventMenuEndTracking);
					DISPATCH_EVENT_TO_METHOD(kEventMenuGetFrameBounds,		DoEventMenuGetFrameBounds);
					DISPATCH_EVENT_TO_METHOD(kEventMenuMatchKey,			DoEventMenuMatchKey);
					DISPATCH_EVENT_TO_METHOD(kEventMenuMeasureItemHeight,	DoEventMenuMeasureItemHeight);
					DISPATCH_EVENT_TO_METHOD(kEventMenuMeasureItemWidth,	DoEventMenuMeasureItemWidth);
					DISPATCH_EVENT_TO_METHOD(kEventMenuOpening,				DoEventMenuOpening);
					DISPATCH_EVENT_TO_METHOD(kEventMenuPopulate,			DoEventMenuPopulate);
					DISPATCH_EVENT_TO_METHOD(kEventMenuTargetItem,			DoEventMenuTargetItem);
					}
				break;
	
			case kEventClassMouse:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventMouseDown,		DoEventMouseDown);
					DISPATCH_EVENT_TO_METHOD(kEventMouseDragged,	DoEventMouseDragged);
					DISPATCH_EVENT_TO_METHOD(kEventMouseEntered,	DoEventMouseEntered);
					DISPATCH_EVENT_TO_METHOD(kEventMouseExited,		DoEventMouseExited);
					DISPATCH_EVENT_TO_METHOD(kEventMouseMoved,		DoEventMouseMoved);
					DISPATCH_EVENT_TO_METHOD(kEventMouseScroll,		DoEventMouseScroll);
					DISPATCH_EVENT_TO_METHOD(kEventMouseUp,			DoEventMouseUp);
					DISPATCH_EVENT_TO_METHOD(kEventMouseWheelMoved,	DoEventMouseWheelMoved);
					}
				break;
				
			case kEventClassNApplication:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventNAppGetUndoManager,	DoEventNAppGetUndoManager);
					}
				break;

			case kEventClassNControl:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventNControlInitialized,        DoEventNControlInitialized);
                    DISPATCH_EVENT_TO_METHOD(kEventNControlTrackingAreaWithin, DoEventNControlTrackingAreaWithin);
					}
				break;

			case kEventClassNWebView:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameLoadStarted,				DoEventNWebFrameLoadStarted);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameLoadCommitted,				DoEventNWebFrameLoadCommitted);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameLoadFinished,				DoEventNWebFrameLoadFinished);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameLoadFailed,					DoEventNWebFrameLoadFailed);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameSetTitle,					DoEventNWebFrameSetTitle);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameSetIcon,					DoEventNWebFrameSetIcon);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameServerRedirected,			DoEventNWebFrameServerRedirected);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameClientRedirecting,			DoEventNWebFrameClientRedirecting);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameClientRedirectCancelled,	DoEventNWebFrameClientRedirectCancelled);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameClosing,					DoEventNWebFrameClosing);
					DISPATCH_EVENT_TO_METHOD(kEventNWebFrameChangedLocation,			DoEventNWebFrameChangedLocation);
					DISPATCH_EVENT_TO_METHOD(kEventNWebResourceGetIdentifier,			DoEventNWebResourceGetIdentifier);
					DISPATCH_EVENT_TO_METHOD(kEventNWebResourceSendingRequest,			DoEventNWebResourceSendingRequest);
					DISPATCH_EVENT_TO_METHOD(kEventNWebResourceReceivedChallenge,		DoEventNWebResourceReceivedChallenge);
					DISPATCH_EVENT_TO_METHOD(kEventNWebResourceReceivedResponse,		DoEventNWebResourceReceivedResponse);
					DISPATCH_EVENT_TO_METHOD(kEventNWebResourceReceivedContentLength,	DoEventNWebResourceReceivedContentLength);
					DISPATCH_EVENT_TO_METHOD(kEventNWebResourceChallengeCancelled,		DoEventNWebResourceChallengeCancelled);
					DISPATCH_EVENT_TO_METHOD(kEventNWebResourceLoadFinished,			DoEventNWebResourceLoadFinished);
					DISPATCH_EVENT_TO_METHOD(kEventNWebResourceLoadFailed,				DoEventNWebResourceLoadFailed);
					DISPATCH_EVENT_TO_METHOD(kEventNWebResourcePlugInFailed,			DoEventNWebResourcePlugInFailed);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceCreateWindow,			DoEventNWebInterfaceCreateWindow);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceSelectWindow,			DoEventNWebInterfaceSelectWindow);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceCloseWindow,			DoEventNWebInterfaceCloseWindow);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceGetWindowBounds,		DoEventNWebInterfaceGetWindowBounds);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceSetWindowBounds,		DoEventNWebInterfaceSetWindowBounds);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceGetViewFrame,			DoEventNWebInterfaceGetViewFrame);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceSetViewFrame,			DoEventNWebInterfaceSetViewFrame);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceMouseOver,				DoEventNWebInterfaceMouseOver);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceContextualMenu,			DoEventNWebInterfaceContextualMenu);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceGetStatusText,			DoEventNWebInterfaceGetStatusText);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceSetStatusText,			DoEventNWebInterfaceSetStatusText);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceIsToolbarVisible,		DoEventNWebInterfaceIsToolbarVisible);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceSetToolbarVisible,		DoEventNWebInterfaceSetToolbarVisible);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceIsStatusVisible,		DoEventNWebInterfaceIsStatusVisible);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceSetStatusVisible,		DoEventNWebInterfaceSetStatusVisible);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceIsWindowResizable,		DoEventNWebInterfaceIsWindowResizable);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceSetWindowResizable,		DoEventNWebInterfaceSetWindowResizable);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceJavaScriptAlert,		DoEventNWebInterfaceJavaScriptAlert);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceJavaScriptConfirm,		DoEventNWebInterfaceJavaScriptConfirm);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceJavaScriptInput,		DoEventNWebInterfaceJavaScriptInput);
					DISPATCH_EVENT_TO_METHOD(kEventNWebInterfaceChooseFile,				DoEventNWebInterfaceChooseFile);
					DISPATCH_EVENT_TO_METHOD(kEventNWebPolicyNewWindow,					DoEventNWebPolicyNewWindow);
					DISPATCH_EVENT_TO_METHOD(kEventNWebPolicyNavigationAction,			DoEventNWebPolicyNavigationAction);
					}
				break;
	
			case kEventClassSearchField:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventSearchFieldCancelClicked,	DoEventSearchFieldCancelClicked);
					DISPATCH_EVENT_TO_METHOD(kEventSearchFieldSearchClicked,	DoEventSearchFieldSearchClicked);
					}
				break;

			case kEventClassService:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventServiceCopy,		DoEventServiceCopy);
					DISPATCH_EVENT_TO_METHOD(kEventServiceGetTypes,	DoEventServiceGetTypes);
					DISPATCH_EVENT_TO_METHOD(kEventServicePaste,	DoEventServicePaste);
					DISPATCH_EVENT_TO_METHOD(kEventServicePerform,	DoEventServicePerform);
					}
				break;
	
			case kEventClassSystem:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventSystemTimeDateChanged,			DoEventSystemTimeDateChanged);
					DISPATCH_EVENT_TO_METHOD(kEventSystemUserSessionActivated,		DoEventSystemUserSessionActivated);
					DISPATCH_EVENT_TO_METHOD(kEventSystemUserSessionDeactivated,	DoEventSystemUserSessionDeactivated);
					}
				break;
	
			case kEventClassTablet:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventTabletPoint,		DoEventTabletPoint);
					DISPATCH_EVENT_TO_METHOD(kEventTabletProximity,	DoEventTabletProximity);
					}
				break;
	
			case kEventClassTextField:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventTextAccepted,			DoEventTextAccepted);
					DISPATCH_EVENT_TO_METHOD(kEventTextDidChange,			DoEventTextDidChange);
					DISPATCH_EVENT_TO_METHOD(kEventTextShouldChangeInRange,	DoEventTextShouldChangeInRange);
					}
				break;
	
			case kEventClassTextInput:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventTextInputFilterText,				DoEventTextInputFilterText);
					DISPATCH_EVENT_TO_METHOD(kEventTextInputGetSelectedText,		DoEventTextInputGetSelectedText);
					DISPATCH_EVENT_TO_METHOD(kEventTextInputOffsetToPos,			DoEventTextInputOffsetToPos);
					DISPATCH_EVENT_TO_METHOD(kEventTextInputPosToOffset,			DoEventTextInputPosToOffset);
					DISPATCH_EVENT_TO_METHOD(kEventTextInputShowHideBottomWindow,	DoEventTextInputShowHideBottomWindow);
					DISPATCH_EVENT_TO_METHOD(kEventTextInputUnicodeForKeyEvent,		DoEventTextInputUnicodeForKeyEvent);
					DISPATCH_EVENT_TO_METHOD(kEventTextInputUnicodeText,			DoEventTextInputUnicodeText);
					DISPATCH_EVENT_TO_METHOD(kEventTextInputUpdateActiveInputArea,	DoEventTextInputUpdateActiveInputArea);
					}
				break;
	
			case kEventClassToolbar:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventToolbarBeginMultiChange,			DoEventToolbarBeginMultiChange);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarCreateItemFromDrag,		DoEventToolbarCreateItemFromDrag);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarCreateItemWithIdentifier,	DoEventToolbarCreateItemWithIdentifier);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarDisplayModeChanged,		DoEventToolbarDisplayModeChanged);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarDisplaySizeChanged,		DoEventToolbarDisplaySizeChanged);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarEndMultiChange,			DoEventToolbarEndMultiChange);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarGetAllowedIdentifiers,	DoEventToolbarGetAllowedIdentifiers);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarGetDefaultIdentifiers,	DoEventToolbarGetDefaultIdentifiers);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarGetSelectableIdentifiers,	DoEventToolbarGetSelectableIdentifiers);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemAdded,				DoEventToolbarItemAdded);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemRemoved,				DoEventToolbarItemRemoved);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarLayoutChanged,			DoEventToolbarLayoutChanged);
					}
				break;
	
			case kEventClassToolbarItem:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemAcceptDrop,			DoEventToolbarItemAcceptDrop);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemCommandIDChanged,		DoEventToolbarItemCommandIDChanged);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemCreateCustomView,		DoEventToolbarItemCreateCustomView);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemEnabledStateChanged,	DoEventToolbarItemEnabledStateChanged);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemGetPersistentData,	DoEventToolbarItemGetPersistentData);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemHelpTextChanged,		DoEventToolbarItemHelpTextChanged);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemImageChanged,			DoEventToolbarItemImageChanged);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemLabelChanged,			DoEventToolbarItemLabelChanged);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemPerformAction,		DoEventToolbarItemPerformAction);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemSelectedStateChanged,	DoEventToolbarItemSelectedStateChanged);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemWouldAcceptDrop,		DoEventToolbarItemWouldAcceptDrop);
					}
				break;
	
			case kEventClassToolbarItemView:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemViewConfigForMode,	DoEventToolbarItemViewConfigForMode);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemViewConfigForSize,	DoEventToolbarItemViewConfigForSize);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemViewEnterConfigMode,	DoEventToolbarItemViewEnterConfigMode);
					DISPATCH_EVENT_TO_METHOD(kEventToolbarItemViewExitConfigMode,	DoEventToolbarItemViewExitConfigMode);
					}
				break;
	
			case kEventClassTSMDocumentAccess:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventTSMDocumentAccessGetCharacters,						DoEventTSMDocumentAccessGetCharacters);
					DISPATCH_EVENT_TO_METHOD(kEventTSMDocumentAccessGetCharactersPtr,					DoEventTSMDocumentAccessGetCharactersPtr);
					DISPATCH_EVENT_TO_METHOD(kEventTSMDocumentAccessGetCharactersPtrForLargestBuffer,	DoEventTSMDocumentAccessGetCharactersPtrForLargestBuffer);
					DISPATCH_EVENT_TO_METHOD(kEventTSMDocumentAccessGetFont,							DoEventTSMDocumentAccessGetFont);
					DISPATCH_EVENT_TO_METHOD(kEventTSMDocumentAccessGetGlyphInfo,						DoEventTSMDocumentAccessGetGlyphInfo);
					DISPATCH_EVENT_TO_METHOD(kEventTSMDocumentAccessGetLength,							DoEventTSMDocumentAccessGetLength);
					DISPATCH_EVENT_TO_METHOD(kEventTSMDocumentAccessGetSelectedRange,					DoEventTSMDocumentAccessGetSelectedRange);
					DISPATCH_EVENT_TO_METHOD(kEventTSMDocumentAccessLockDocument,						DoEventTSMDocumentAccessLockDocument);
					DISPATCH_EVENT_TO_METHOD(kEventTSMDocumentAccessUnlockDocument,						DoEventTSMDocumentAccessUnlockDocument);
					}
				break;
	
			case kEventClassScrollable:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventScrollableGetInfo,		DoEventScrollableGetInfo);
					DISPATCH_EVENT_TO_METHOD(kEventScrollableInfoChanged,	DoEventScrollableInfoChanged);
					DISPATCH_EVENT_TO_METHOD(kEventScrollableScrollTo,		DoEventScrollableScrollTo);
					}
				break;
	
			case kEventClassVolume:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventVolumeMounted,	DoEventVolumeMounted);
					DISPATCH_EVENT_TO_METHOD(kEventVolumeUnmounted,	DoEventVolumeUnmounted);
					}
				break;
	
			case kEventClassWindow:
				switch (theEvent.GetKind()) {
					DISPATCH_EVENT_TO_METHOD(kEventWindowActivated,				DoEventWindowActivated);
					DISPATCH_EVENT_TO_METHOD(kEventWindowBoundsChanged,			DoEventWindowBoundsChanged);
					DISPATCH_EVENT_TO_METHOD(kEventWindowBoundsChanging,		DoEventWindowBoundsChanging);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClickCloseRgn,			DoEventWindowClickCloseRgn);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClickCollapseRgn,		DoEventWindowClickCollapseRgn);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClickContentRgn,		DoEventWindowClickContentRgn);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClickDragRgn,			DoEventWindowClickDragRgn);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClickProxyIconRgn,		DoEventWindowClickProxyIconRgn);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClickResizeRgn,		DoEventWindowClickResizeRgn);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClickStructureRgn,		DoEventWindowClickStructureRgn);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClickToolbarButtonRgn,	DoEventWindowClickToolbarButtonRgn);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClickZoomRgn,			DoEventWindowClickZoomRgn);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClose,					DoEventWindowClose);
					DISPATCH_EVENT_TO_METHOD(kEventWindowCloseAll,				DoEventWindowCloseAll);
					DISPATCH_EVENT_TO_METHOD(kEventWindowClosed,				DoEventWindowClosed);
					DISPATCH_EVENT_TO_METHOD(kEventWindowCollapse,				DoEventWindowCollapse);
					DISPATCH_EVENT_TO_METHOD(kEventWindowCollapseAll,			DoEventWindowCollapseAll);
					DISPATCH_EVENT_TO_METHOD(kEventWindowCollapsed,				DoEventWindowCollapsed);
					DISPATCH_EVENT_TO_METHOD(kEventWindowCollapsing,			DoEventWindowCollapsing);
					DISPATCH_EVENT_TO_METHOD(kEventWindowConstrain,				DoEventWindowConstrain);
					DISPATCH_EVENT_TO_METHOD(kEventWindowContextualMenuSelect,	DoEventWindowContextualMenuSelect);
					DISPATCH_EVENT_TO_METHOD(kEventWindowCursorChange,			DoEventWindowCursorChange);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDeactivated,			DoEventWindowDeactivated);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDispose,				DoEventWindowDispose);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDragCompleted,			DoEventWindowDragCompleted);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDragHilite,			DoEventWindowDragHilite);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDragStarted,			DoEventWindowDragStarted);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDrawContent,			DoEventWindowDrawContent);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDrawerClosed,			DoEventWindowDrawerClosed);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDrawerClosing,			DoEventWindowDrawerClosing);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDrawerOpened,			DoEventWindowDrawerOpened);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDrawerOpening,			DoEventWindowDrawerOpening);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDrawFrame,				DoEventWindowDrawFrame);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDrawGrowBox,			DoEventWindowDrawGrowBox);
					DISPATCH_EVENT_TO_METHOD(kEventWindowDrawPart,				DoEventWindowDrawPart);
					DISPATCH_EVENT_TO_METHOD(kEventWindowExpand,				DoEventWindowExpand);
					DISPATCH_EVENT_TO_METHOD(kEventWindowExpandAll,				DoEventWindowExpandAll);
					DISPATCH_EVENT_TO_METHOD(kEventWindowExpanded,				DoEventWindowExpanded);
					DISPATCH_EVENT_TO_METHOD(kEventWindowExpanding,				DoEventWindowExpanding);
					DISPATCH_EVENT_TO_METHOD(kEventWindowFocusAcquired,			DoEventWindowFocusAcquired);
					DISPATCH_EVENT_TO_METHOD(kEventWindowFocusContent,			DoEventWindowFocusContent);
					DISPATCH_EVENT_TO_METHOD(kEventWindowFocusDrawer,			DoEventWindowFocusDrawer);
					DISPATCH_EVENT_TO_METHOD(kEventWindowFocusRelinquish,		DoEventWindowFocusRelinquish);
					DISPATCH_EVENT_TO_METHOD(kEventWindowFocusToolbar,			DoEventWindowFocusToolbar);
					DISPATCH_EVENT_TO_METHOD(kEventWindowGetClickActivation,	DoEventWindowGetClickActivation);
					DISPATCH_EVENT_TO_METHOD(kEventWindowGetClickModality,		DoEventWindowGetClickModality);
					DISPATCH_EVENT_TO_METHOD(kEventWindowGetDockTileMenu,		DoEventWindowGetDockTileMenu);
					DISPATCH_EVENT_TO_METHOD(kEventWindowGetGrowImageRegion,	DoEventWindowGetGrowImageRegion);
					DISPATCH_EVENT_TO_METHOD(kEventWindowGetIdealSize,			DoEventWindowGetIdealSize);
					DISPATCH_EVENT_TO_METHOD(kEventWindowGetMaximumSize,		DoEventWindowGetMaximumSize);
					DISPATCH_EVENT_TO_METHOD(kEventWindowGetMinimumSize,		DoEventWindowGetMinimumSize);
					DISPATCH_EVENT_TO_METHOD(kEventWindowGetRegion,				DoEventWindowGetRegion);
					DISPATCH_EVENT_TO_METHOD(kEventWindowHandleActivate,		DoEventWindowHandleActivate);
					DISPATCH_EVENT_TO_METHOD(kEventWindowHandleContentClick,	DoEventWindowHandleContentClick);
					DISPATCH_EVENT_TO_METHOD(kEventWindowHandleDeactivate,		DoEventWindowHandleDeactivate);
					DISPATCH_EVENT_TO_METHOD(kEventWindowHidden,				DoEventWindowHidden);
					DISPATCH_EVENT_TO_METHOD(kEventWindowHiding,				DoEventWindowHiding);
					DISPATCH_EVENT_TO_METHOD(kEventWindowHitTest,				DoEventWindowHitTest);
					DISPATCH_EVENT_TO_METHOD(kEventWindowInit,					DoEventWindowInit);
					DISPATCH_EVENT_TO_METHOD(kEventWindowMeasureTitle,			DoEventWindowMeasureTitle);
					DISPATCH_EVENT_TO_METHOD(kEventWindowModified,				DoEventWindowModified);
					DISPATCH_EVENT_TO_METHOD(kEventWindowPaint,					DoEventWindowPaint);
					DISPATCH_EVENT_TO_METHOD(kEventWindowPathSelect,			DoEventWindowPathSelect);
					DISPATCH_EVENT_TO_METHOD(kEventWindowProxyBeginDrag,		DoEventWindowProxyBeginDrag);
					DISPATCH_EVENT_TO_METHOD(kEventWindowProxyEndDrag,			DoEventWindowProxyEndDrag);
					DISPATCH_EVENT_TO_METHOD(kEventWindowResizeCompleted,		DoEventWindowResizeCompleted);
					DISPATCH_EVENT_TO_METHOD(kEventWindowResizeStarted,			DoEventWindowResizeStarted);
					DISPATCH_EVENT_TO_METHOD(kEventWindowRestoreFromDock,		DoEventWindowRestoreFromDock);
					DISPATCH_EVENT_TO_METHOD(kEventWindowSetupProxyDragImage,	DoEventWindowSetupProxyDragImage);
					DISPATCH_EVENT_TO_METHOD(kEventWindowSheetClosed,			DoEventWindowSheetClosed);
					DISPATCH_EVENT_TO_METHOD(kEventWindowSheetClosing,			DoEventWindowSheetClosing);
					DISPATCH_EVENT_TO_METHOD(kEventWindowSheetOpened,			DoEventWindowSheetOpened);
					DISPATCH_EVENT_TO_METHOD(kEventWindowSheetOpening,			DoEventWindowSheetOpening);
					DISPATCH_EVENT_TO_METHOD(kEventWindowShowing,				DoEventWindowShowing);
					DISPATCH_EVENT_TO_METHOD(kEventWindowShown,					DoEventWindowShown);
					DISPATCH_EVENT_TO_METHOD(kEventWindowStateChanged,			DoEventWindowStateChanged);
					DISPATCH_EVENT_TO_METHOD(kEventWindowToolbarSwitchMode,		DoEventWindowToolbarSwitchMode);
					DISPATCH_EVENT_TO_METHOD(kEventWindowTransitionCompleted,	DoEventWindowTransitionCompleted);
					DISPATCH_EVENT_TO_METHOD(kEventWindowTransitionStarted,		DoEventWindowTransitionStarted);
					DISPATCH_EVENT_TO_METHOD(kEventWindowUpdate,				DoEventWindowUpdate);
					DISPATCH_EVENT_TO_METHOD(kEventWindowZoom,					DoEventWindowZoom);
					DISPATCH_EVENT_TO_METHOD(kEventWindowZoomAll,				DoEventWindowZoomAll);
					DISPATCH_EVENT_TO_METHOD(kEventWindowZoomed,				DoEventWindowZoomed);
					}
				break;
			}
		}



	// Handle unprocessed events
	if (theErr == eventNotHandledErr && !isDead)
		theErr = DoEvent(theEvent);



	// Reset our state
	if (!isDead)
		{
		mIsDead      = oldFlag;
		mNextHandler = NULL;
		}
	
	return(theErr);
}





//============================================================================
//		NCarbonEventHandler::CallNextHandler : Call the next handler.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NCarbonEventHandler::CallNextHandler(NCarbonEvent &theEvent)
{	OSStatus	theErr;



	// Validate our state
	NN_ASSERT(mNextHandler != NULL);



	// Call the handler
	theErr = CallNextEventHandler(mNextHandler, theEvent.GetEvent());
	
	return(theErr);
}





//============================================================================
//		NCarbonEventHandler::EnableIf : Convert a bool to a CommandStatus.
//----------------------------------------------------------------------------
CommandStatus NCarbonEventHandler::EnableIf(bool theTest, CommandStatus notEnabled)
{


	// Get the status
	return(theTest ? kCommandEnable : notEnabled);
}





//============================================================================
//		NCarbonEventHandler::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus NCarbonEventHandler::GetCommandStatus(const HICommandExtended &/*hiCmd*/)
{


	// Get the status
	//
	// Menu commands should be set to the enabled state (for "always available"
	// actions such as about, quit, etc) or "auto-disable".
	//
	// Auto-disabled menu items will be disabled unless a kEventCommandUpdateStatus
	// handler explicitly enables them, and the base NCarbonEventHandler class
	// provides a handler which uses GetCommandStatus to determine their state.
	//
	// Typically a sub-class would just override GetCommandStatus and indicate
	// which commands should be enabled for the current situation.
	return(kCommandDefault);
}





//============================================================================
//		NCarbonEventHandler::DoEventsChanged : Handle event changes.
//----------------------------------------------------------------------------
void NCarbonEventHandler::DoEventsChanged(void)
{
}





//============================================================================
//		NCarbonEventHandler::DoEventCommandUpdateStatus : Handle kEventCommandUpdateStatus.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::DoEventCommandUpdateStatus(NCarbonEvent &theEvent)
{	CommandStatus		theStatus;
	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Get the command status
	hiCmd     = theEvent.GetHICommand();
	theStatus = GetCommandStatus(hiCmd);



	// Update the user interface
	//
	// Sub-classes can obtain context-sensitive enabling/disabling of their
	// menu/controls by returning the appropriate status from GetCommandStatus.
	theErr = (theStatus == kCommandDefault) ? (OSStatus) eventNotHandledErr : (OSStatus) noErr;

	if (hiCmd.attributes & kHICommandFromMenu)
		{
		if (theStatus == kCommandEnable)
			EnableMenuItem( hiCmd.source.menu.menuRef, hiCmd.source.menu.menuItemIndex);

		else if (theStatus == kCommandDisable)
			DisableMenuItem(hiCmd.source.menu.menuRef, hiCmd.source.menu.menuItemIndex);
		}
	
	else if (hiCmd.attributes & kHICommandFromControl)
		{
		if (theStatus == kCommandEnable)
			EnableControl( hiCmd.source.control);

		else if (theStatus == kCommandDisable)
			DisableControl(hiCmd.source.control);
		}
	
	return(theErr);
}





//============================================================================
//		NCarbonEventHandler::DoEventXXXX : Handle kEventXXXX.
//----------------------------------------------------------------------------
DEFAULT_EVENT_METHOD(DoEvent)

DEFAULT_EVENT_METHOD(DoEventAccessibleGetAllActionNames)
DEFAULT_EVENT_METHOD(DoEventAccessibleGetAllAttributeNames)
DEFAULT_EVENT_METHOD(DoEventAccessibleGetAllParameterizedAttributeNames)
DEFAULT_EVENT_METHOD(DoEventAccessibleGetChildAtPoint)
DEFAULT_EVENT_METHOD(DoEventAccessibleGetFocusedChild)
DEFAULT_EVENT_METHOD(DoEventAccessibleGetNamedActionDescription)
DEFAULT_EVENT_METHOD(DoEventAccessibleGetNamedAttribute)
DEFAULT_EVENT_METHOD(DoEventAccessibleIsNamedAttributeSettable)
DEFAULT_EVENT_METHOD(DoEventAccessiblePerformNamedAction)
DEFAULT_EVENT_METHOD(DoEventAccessibleSetNamedAttribute)

DEFAULT_EVENT_METHOD(DoEventAppActivated)
DEFAULT_EVENT_METHOD(DoEventAppActiveWindowChanged)
DEFAULT_EVENT_METHOD(DoEventAppAvailableWindowBoundsChanged)
DEFAULT_EVENT_METHOD(DoEventAppDeactivated)
DEFAULT_EVENT_METHOD(DoEventAppFocusDrawer)
DEFAULT_EVENT_METHOD(DoEventAppFocusMenuBar)
DEFAULT_EVENT_METHOD(DoEventAppFocusNextDocumentWindow)
DEFAULT_EVENT_METHOD(DoEventAppFocusNextFloatingWindow)
DEFAULT_EVENT_METHOD(DoEventAppFocusToolbar)
DEFAULT_EVENT_METHOD(DoEventAppFrontSwitched)
DEFAULT_EVENT_METHOD(DoEventAppGetDockTileMenu)
DEFAULT_EVENT_METHOD(DoEventAppHidden)
DEFAULT_EVENT_METHOD(DoEventAppIsEventInInstantMouser)
DEFAULT_EVENT_METHOD(DoEventAppLaunched)
DEFAULT_EVENT_METHOD(DoEventAppLaunchNotification)
DEFAULT_EVENT_METHOD(DoEventAppQuit)
DEFAULT_EVENT_METHOD(DoEventAppShown)
DEFAULT_EVENT_METHOD(DoEventAppSystemUIModeChanged)
DEFAULT_EVENT_METHOD(DoEventAppTerminated)

DEFAULT_EVENT_METHOD(DoEventAppearanceScrollBarVariantChanged)

DEFAULT_EVENT_METHOD(DoEventAppleEvent)

DEFAULT_EVENT_METHOD(DoEventComboBoxListItemSelected)

DEFAULT_EVENT_METHOD(DoEventCommandProcess)

DEFAULT_EVENT_METHOD(DoEventControlActivate)
DEFAULT_EVENT_METHOD(DoEventControlAddedSubControl)
DEFAULT_EVENT_METHOD(DoEventControlApplyBackground)
DEFAULT_EVENT_METHOD(DoEventControlApplyTextColor)
DEFAULT_EVENT_METHOD(DoEventControlArbitraryMessage)
DEFAULT_EVENT_METHOD(DoEventControlBoundsChanged)
DEFAULT_EVENT_METHOD(DoEventControlClick)
DEFAULT_EVENT_METHOD(DoEventControlContextualMenuClick)
DEFAULT_EVENT_METHOD(DoEventControlDeactivate)
DEFAULT_EVENT_METHOD(DoEventControlDispose)
DEFAULT_EVENT_METHOD(DoEventControlDragEnter)
DEFAULT_EVENT_METHOD(DoEventControlDragLeave)
DEFAULT_EVENT_METHOD(DoEventControlDragReceive)
DEFAULT_EVENT_METHOD(DoEventControlDragWithin)
DEFAULT_EVENT_METHOD(DoEventControlDraw)
DEFAULT_EVENT_METHOD(DoEventControlEnabledStateChanged)
DEFAULT_EVENT_METHOD(DoEventControlGetActionProcPart)
DEFAULT_EVENT_METHOD(DoEventControlGetAutoToggleValue)
DEFAULT_EVENT_METHOD(DoEventControlGetClickActivation)
DEFAULT_EVENT_METHOD(DoEventControlGetData)
DEFAULT_EVENT_METHOD(DoEventControlGetFocusPart)
DEFAULT_EVENT_METHOD(DoEventControlGetFrameMetrics)
DEFAULT_EVENT_METHOD(DoEventControlGetIndicatorDragConstraint)
DEFAULT_EVENT_METHOD(DoEventControlGetNextFocusCandidate)
DEFAULT_EVENT_METHOD(DoEventControlGetOptimalBounds)
DEFAULT_EVENT_METHOD(DoEventControlGetPartBounds)
DEFAULT_EVENT_METHOD(DoEventControlGetPartRegion)
DEFAULT_EVENT_METHOD(DoEventControlGetScrollToHereStartPoint)
DEFAULT_EVENT_METHOD(DoEventControlGetSizeConstraints)
DEFAULT_EVENT_METHOD(DoEventControlGhostingFinished)
DEFAULT_EVENT_METHOD(DoEventControlHiliteChanged)
DEFAULT_EVENT_METHOD(DoEventControlHit)
DEFAULT_EVENT_METHOD(DoEventControlHitTest)
DEFAULT_EVENT_METHOD(DoEventControlIndicatorMoved)
DEFAULT_EVENT_METHOD(DoEventControlInitialize)
DEFAULT_EVENT_METHOD(DoEventControlInterceptSubviewClick)
DEFAULT_EVENT_METHOD(DoEventControlInvalidateForSizeChange)
DEFAULT_EVENT_METHOD(DoEventControlLayoutInfoChanged)
DEFAULT_EVENT_METHOD(DoEventControlOwningWindowChanged)
DEFAULT_EVENT_METHOD(DoEventControlRemovingSubControl)
DEFAULT_EVENT_METHOD(DoEventControlSetCursor)
DEFAULT_EVENT_METHOD(DoEventControlSetData)
DEFAULT_EVENT_METHOD(DoEventControlSetFocusPart)
DEFAULT_EVENT_METHOD(DoEventControlSimulateHit)
DEFAULT_EVENT_METHOD(DoEventControlTitleChanged)
DEFAULT_EVENT_METHOD(DoEventControlTrack)
DEFAULT_EVENT_METHOD(DoEventControlTrackingAreaEntered)
DEFAULT_EVENT_METHOD(DoEventControlTrackingAreaExited)
DEFAULT_EVENT_METHOD(DoEventControlValueFieldChanged)
DEFAULT_EVENT_METHOD(DoEventControlVisibilityChanged)

DEFAULT_EVENT_METHOD(DoEventFontPanelClosed)
DEFAULT_EVENT_METHOD(DoEventFontSelection)

DEFAULT_EVENT_METHOD(DoEventHIObjectConstruct)
DEFAULT_EVENT_METHOD(DoEventHIObjectInitialize)
DEFAULT_EVENT_METHOD(DoEventHIObjectDestruct)
DEFAULT_EVENT_METHOD(DoEventHIObjectIsEqual)
DEFAULT_EVENT_METHOD(DoEventHIObjectPrintDebugInfo)
DEFAULT_EVENT_METHOD(DoEventHIObjectEncode)

DEFAULT_EVENT_METHOD(DoEventHotKeyPressed)
DEFAULT_EVENT_METHOD(DoEventHotKeyReleased)

DEFAULT_EVENT_METHOD(DoEventInkGesture)
DEFAULT_EVENT_METHOD(DoEventInkPoint)
DEFAULT_EVENT_METHOD(DoEventInkText)

DEFAULT_EVENT_METHOD(DoEventMenuBarHidden)
DEFAULT_EVENT_METHOD(DoEventMenuBarShown)
DEFAULT_EVENT_METHOD(DoEventMenuBecomeScrollable)
DEFAULT_EVENT_METHOD(DoEventMenuBeginTracking)
DEFAULT_EVENT_METHOD(DoEventMenuCalculateSize)
DEFAULT_EVENT_METHOD(DoEventMenuCeaseToBeScrollable)
DEFAULT_EVENT_METHOD(DoEventMenuChangeTrackingMode)
DEFAULT_EVENT_METHOD(DoEventMenuClosed)
DEFAULT_EVENT_METHOD(DoEventMenuCreateFrameView)
DEFAULT_EVENT_METHOD(DoEventMenuDispose)
DEFAULT_EVENT_METHOD(DoEventMenuDrawItem)
DEFAULT_EVENT_METHOD(DoEventMenuDrawItemContent)
DEFAULT_EVENT_METHOD(DoEventMenuEnableItems)
DEFAULT_EVENT_METHOD(DoEventMenuEndTracking)
DEFAULT_EVENT_METHOD(DoEventMenuGetFrameBounds)
DEFAULT_EVENT_METHOD(DoEventMenuMatchKey)
DEFAULT_EVENT_METHOD(DoEventMenuMeasureItemHeight)
DEFAULT_EVENT_METHOD(DoEventMenuMeasureItemWidth)
DEFAULT_EVENT_METHOD(DoEventMenuOpening)
DEFAULT_EVENT_METHOD(DoEventMenuPopulate)
DEFAULT_EVENT_METHOD(DoEventMenuTargetItem)

DEFAULT_EVENT_METHOD(DoEventMouseDown)
DEFAULT_EVENT_METHOD(DoEventMouseDragged)
DEFAULT_EVENT_METHOD(DoEventMouseEntered)
DEFAULT_EVENT_METHOD(DoEventMouseExited)
DEFAULT_EVENT_METHOD(DoEventMouseMoved)
DEFAULT_EVENT_METHOD(DoEventMouseScroll)
DEFAULT_EVENT_METHOD(DoEventMouseUp)
DEFAULT_EVENT_METHOD(DoEventMouseWheelMoved)

DEFAULT_EVENT_METHOD(DoEventNAppGetUndoManager)

DEFAULT_EVENT_METHOD(DoEventNControlInitialized)
DEFAULT_EVENT_METHOD(DoEventNControlTrackingAreaWithin)

DEFAULT_EVENT_METHOD(DoEventNWebFrameLoadStarted)
DEFAULT_EVENT_METHOD(DoEventNWebFrameLoadCommitted)
DEFAULT_EVENT_METHOD(DoEventNWebFrameLoadFinished)
DEFAULT_EVENT_METHOD(DoEventNWebFrameLoadFailed)
DEFAULT_EVENT_METHOD(DoEventNWebFrameSetTitle)
DEFAULT_EVENT_METHOD(DoEventNWebFrameSetIcon)
DEFAULT_EVENT_METHOD(DoEventNWebFrameServerRedirected)
DEFAULT_EVENT_METHOD(DoEventNWebFrameClientRedirecting)
DEFAULT_EVENT_METHOD(DoEventNWebFrameClientRedirectCancelled)
DEFAULT_EVENT_METHOD(DoEventNWebFrameClosing)
DEFAULT_EVENT_METHOD(DoEventNWebFrameChangedLocation)
DEFAULT_EVENT_METHOD(DoEventNWebResourceGetIdentifier)
DEFAULT_EVENT_METHOD(DoEventNWebResourceSendingRequest)
DEFAULT_EVENT_METHOD(DoEventNWebResourceReceivedChallenge)
DEFAULT_EVENT_METHOD(DoEventNWebResourceReceivedResponse)
DEFAULT_EVENT_METHOD(DoEventNWebResourceReceivedContentLength)
DEFAULT_EVENT_METHOD(DoEventNWebResourceChallengeCancelled)
DEFAULT_EVENT_METHOD(DoEventNWebResourceLoadFinished)
DEFAULT_EVENT_METHOD(DoEventNWebResourceLoadFailed)
DEFAULT_EVENT_METHOD(DoEventNWebResourcePlugInFailed)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceCreateWindow)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceSelectWindow)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceCloseWindow)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceGetWindowBounds)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceSetWindowBounds)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceGetViewFrame)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceSetViewFrame)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceMouseOver)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceContextualMenu)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceGetStatusText)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceSetStatusText)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceIsToolbarVisible)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceSetToolbarVisible)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceIsStatusVisible)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceSetStatusVisible)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceIsWindowResizable)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceSetWindowResizable)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceJavaScriptAlert)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceJavaScriptConfirm)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceJavaScriptInput)
DEFAULT_EVENT_METHOD(DoEventNWebInterfaceChooseFile)
DEFAULT_EVENT_METHOD(DoEventNWebPolicyNewWindow)
DEFAULT_EVENT_METHOD(DoEventNWebPolicyNavigationAction)

DEFAULT_EVENT_METHOD(DoEventSearchFieldCancelClicked)
DEFAULT_EVENT_METHOD(DoEventSearchFieldSearchClicked)

DEFAULT_EVENT_METHOD(DoEventServiceCopy)
DEFAULT_EVENT_METHOD(DoEventServiceGetTypes)
DEFAULT_EVENT_METHOD(DoEventServicePaste)
DEFAULT_EVENT_METHOD(DoEventServicePerform)

DEFAULT_EVENT_METHOD(DoEventSystemTimeDateChanged)
DEFAULT_EVENT_METHOD(DoEventSystemUserSessionActivated)
DEFAULT_EVENT_METHOD(DoEventSystemUserSessionDeactivated)

DEFAULT_EVENT_METHOD(DoEventTabletPoint)
DEFAULT_EVENT_METHOD(DoEventTabletProximity)

DEFAULT_EVENT_METHOD(DoEventTextAccepted)
DEFAULT_EVENT_METHOD(DoEventTextDidChange)
DEFAULT_EVENT_METHOD(DoEventTextShouldChangeInRange)

DEFAULT_EVENT_METHOD(DoEventTextInputFilterText)
DEFAULT_EVENT_METHOD(DoEventTextInputGetSelectedText)
DEFAULT_EVENT_METHOD(DoEventTextInputOffsetToPos)
DEFAULT_EVENT_METHOD(DoEventTextInputPosToOffset)
DEFAULT_EVENT_METHOD(DoEventTextInputShowHideBottomWindow)
DEFAULT_EVENT_METHOD(DoEventTextInputUnicodeForKeyEvent)
DEFAULT_EVENT_METHOD(DoEventTextInputUnicodeText)
DEFAULT_EVENT_METHOD(DoEventTextInputUpdateActiveInputArea)

DEFAULT_EVENT_METHOD(DoEventToolbarBeginMultiChange)
DEFAULT_EVENT_METHOD(DoEventToolbarCreateItemFromDrag)
DEFAULT_EVENT_METHOD(DoEventToolbarCreateItemWithIdentifier)
DEFAULT_EVENT_METHOD(DoEventToolbarDisplayModeChanged)
DEFAULT_EVENT_METHOD(DoEventToolbarDisplaySizeChanged)
DEFAULT_EVENT_METHOD(DoEventToolbarEndMultiChange)
DEFAULT_EVENT_METHOD(DoEventToolbarGetAllowedIdentifiers)
DEFAULT_EVENT_METHOD(DoEventToolbarGetDefaultIdentifiers)
DEFAULT_EVENT_METHOD(DoEventToolbarGetSelectableIdentifiers)
DEFAULT_EVENT_METHOD(DoEventToolbarItemAdded)
DEFAULT_EVENT_METHOD(DoEventToolbarItemRemoved)
DEFAULT_EVENT_METHOD(DoEventToolbarLayoutChanged)

DEFAULT_EVENT_METHOD(DoEventToolbarItemAcceptDrop)
DEFAULT_EVENT_METHOD(DoEventToolbarItemCommandIDChanged)
DEFAULT_EVENT_METHOD(DoEventToolbarItemCreateCustomView)
DEFAULT_EVENT_METHOD(DoEventToolbarItemEnabledStateChanged)
DEFAULT_EVENT_METHOD(DoEventToolbarItemGetPersistentData)
DEFAULT_EVENT_METHOD(DoEventToolbarItemHelpTextChanged)
DEFAULT_EVENT_METHOD(DoEventToolbarItemImageChanged)
DEFAULT_EVENT_METHOD(DoEventToolbarItemLabelChanged)
DEFAULT_EVENT_METHOD(DoEventToolbarItemPerformAction)
DEFAULT_EVENT_METHOD(DoEventToolbarItemSelectedStateChanged)
DEFAULT_EVENT_METHOD(DoEventToolbarItemWouldAcceptDrop)

DEFAULT_EVENT_METHOD(DoEventToolbarItemViewConfigForMode)
DEFAULT_EVENT_METHOD(DoEventToolbarItemViewConfigForSize)
DEFAULT_EVENT_METHOD(DoEventToolbarItemViewEnterConfigMode)
DEFAULT_EVENT_METHOD(DoEventToolbarItemViewExitConfigMode)

DEFAULT_EVENT_METHOD(DoEventTSMDocumentAccessGetCharacters)
DEFAULT_EVENT_METHOD(DoEventTSMDocumentAccessGetCharactersPtr)
DEFAULT_EVENT_METHOD(DoEventTSMDocumentAccessGetCharactersPtrForLargestBuffer)
DEFAULT_EVENT_METHOD(DoEventTSMDocumentAccessGetFont)
DEFAULT_EVENT_METHOD(DoEventTSMDocumentAccessGetGlyphInfo)
DEFAULT_EVENT_METHOD(DoEventTSMDocumentAccessGetLength)
DEFAULT_EVENT_METHOD(DoEventTSMDocumentAccessGetSelectedRange)
DEFAULT_EVENT_METHOD(DoEventTSMDocumentAccessLockDocument)
DEFAULT_EVENT_METHOD(DoEventTSMDocumentAccessUnlockDocument)

DEFAULT_EVENT_METHOD(DoEventRawKeyDown)
DEFAULT_EVENT_METHOD(DoEventRawKeyModifiersChanged)
DEFAULT_EVENT_METHOD(DoEventRawKeyRepeat)
DEFAULT_EVENT_METHOD(DoEventRawKeyUp)

DEFAULT_EVENT_METHOD(DoEventScrollableGetInfo)
DEFAULT_EVENT_METHOD(DoEventScrollableInfoChanged)
DEFAULT_EVENT_METHOD(DoEventScrollableScrollTo)

DEFAULT_EVENT_METHOD(DoEventVolumeMounted)
DEFAULT_EVENT_METHOD(DoEventVolumeUnmounted)

DEFAULT_EVENT_METHOD(DoEventWindowActivated)
DEFAULT_EVENT_METHOD(DoEventWindowBoundsChanged)
DEFAULT_EVENT_METHOD(DoEventWindowBoundsChanging)
DEFAULT_EVENT_METHOD(DoEventWindowClickCloseRgn)
DEFAULT_EVENT_METHOD(DoEventWindowClickCollapseRgn)
DEFAULT_EVENT_METHOD(DoEventWindowClickContentRgn)
DEFAULT_EVENT_METHOD(DoEventWindowClickDragRgn)
DEFAULT_EVENT_METHOD(DoEventWindowClickProxyIconRgn)
DEFAULT_EVENT_METHOD(DoEventWindowClickResizeRgn)
DEFAULT_EVENT_METHOD(DoEventWindowClickStructureRgn)
DEFAULT_EVENT_METHOD(DoEventWindowClickToolbarButtonRgn)
DEFAULT_EVENT_METHOD(DoEventWindowClickZoomRgn)
DEFAULT_EVENT_METHOD(DoEventWindowClose)
DEFAULT_EVENT_METHOD(DoEventWindowCloseAll)
DEFAULT_EVENT_METHOD(DoEventWindowClosed)
DEFAULT_EVENT_METHOD(DoEventWindowCollapse)
DEFAULT_EVENT_METHOD(DoEventWindowCollapseAll)
DEFAULT_EVENT_METHOD(DoEventWindowCollapsed)
DEFAULT_EVENT_METHOD(DoEventWindowCollapsing)
DEFAULT_EVENT_METHOD(DoEventWindowConstrain)
DEFAULT_EVENT_METHOD(DoEventWindowContextualMenuSelect)
DEFAULT_EVENT_METHOD(DoEventWindowCursorChange)
DEFAULT_EVENT_METHOD(DoEventWindowDeactivated)
DEFAULT_EVENT_METHOD(DoEventWindowDispose)
DEFAULT_EVENT_METHOD(DoEventWindowDragCompleted)
DEFAULT_EVENT_METHOD(DoEventWindowDragHilite)
DEFAULT_EVENT_METHOD(DoEventWindowDragStarted)
DEFAULT_EVENT_METHOD(DoEventWindowDrawContent)
DEFAULT_EVENT_METHOD(DoEventWindowDrawerClosed)
DEFAULT_EVENT_METHOD(DoEventWindowDrawerClosing)
DEFAULT_EVENT_METHOD(DoEventWindowDrawerOpened)
DEFAULT_EVENT_METHOD(DoEventWindowDrawerOpening)
DEFAULT_EVENT_METHOD(DoEventWindowDrawFrame)
DEFAULT_EVENT_METHOD(DoEventWindowDrawGrowBox)
DEFAULT_EVENT_METHOD(DoEventWindowDrawPart)
DEFAULT_EVENT_METHOD(DoEventWindowExpand)
DEFAULT_EVENT_METHOD(DoEventWindowExpandAll)
DEFAULT_EVENT_METHOD(DoEventWindowExpanded)
DEFAULT_EVENT_METHOD(DoEventWindowExpanding)
DEFAULT_EVENT_METHOD(DoEventWindowFocusAcquired)
DEFAULT_EVENT_METHOD(DoEventWindowFocusContent)
DEFAULT_EVENT_METHOD(DoEventWindowFocusDrawer)
DEFAULT_EVENT_METHOD(DoEventWindowFocusRelinquish)
DEFAULT_EVENT_METHOD(DoEventWindowFocusToolbar)
DEFAULT_EVENT_METHOD(DoEventWindowGetClickActivation)
DEFAULT_EVENT_METHOD(DoEventWindowGetClickModality)
DEFAULT_EVENT_METHOD(DoEventWindowGetDockTileMenu)
DEFAULT_EVENT_METHOD(DoEventWindowGetGrowImageRegion)
DEFAULT_EVENT_METHOD(DoEventWindowGetIdealSize)
DEFAULT_EVENT_METHOD(DoEventWindowGetMaximumSize)
DEFAULT_EVENT_METHOD(DoEventWindowGetMinimumSize)
DEFAULT_EVENT_METHOD(DoEventWindowGetRegion)
DEFAULT_EVENT_METHOD(DoEventWindowHandleActivate)
DEFAULT_EVENT_METHOD(DoEventWindowHandleContentClick)
DEFAULT_EVENT_METHOD(DoEventWindowHandleDeactivate)
DEFAULT_EVENT_METHOD(DoEventWindowHidden)
DEFAULT_EVENT_METHOD(DoEventWindowHiding)
DEFAULT_EVENT_METHOD(DoEventWindowHitTest)
DEFAULT_EVENT_METHOD(DoEventWindowInit)
DEFAULT_EVENT_METHOD(DoEventWindowMeasureTitle)
DEFAULT_EVENT_METHOD(DoEventWindowModified)
DEFAULT_EVENT_METHOD(DoEventWindowPaint)
DEFAULT_EVENT_METHOD(DoEventWindowPathSelect)
DEFAULT_EVENT_METHOD(DoEventWindowProxyBeginDrag)
DEFAULT_EVENT_METHOD(DoEventWindowProxyEndDrag)
DEFAULT_EVENT_METHOD(DoEventWindowResizeCompleted)
DEFAULT_EVENT_METHOD(DoEventWindowResizeStarted)
DEFAULT_EVENT_METHOD(DoEventWindowRestoreFromDock)
DEFAULT_EVENT_METHOD(DoEventWindowSetupProxyDragImage)
DEFAULT_EVENT_METHOD(DoEventWindowSheetClosed)
DEFAULT_EVENT_METHOD(DoEventWindowSheetClosing)
DEFAULT_EVENT_METHOD(DoEventWindowSheetOpened)
DEFAULT_EVENT_METHOD(DoEventWindowSheetOpening)
DEFAULT_EVENT_METHOD(DoEventWindowShowing)
DEFAULT_EVENT_METHOD(DoEventWindowShown)
DEFAULT_EVENT_METHOD(DoEventWindowStateChanged)
DEFAULT_EVENT_METHOD(DoEventWindowToolbarSwitchMode)
DEFAULT_EVENT_METHOD(DoEventWindowTransitionCompleted)
DEFAULT_EVENT_METHOD(DoEventWindowTransitionStarted)
DEFAULT_EVENT_METHOD(DoEventWindowUpdate)
DEFAULT_EVENT_METHOD(DoEventWindowZoom)
DEFAULT_EVENT_METHOD(DoEventWindowZoomAll)
DEFAULT_EVENT_METHOD(DoEventWindowZoomed)





//============================================================================
//		NCarbonEventHandler::InstallHandler : Install a handler.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NCarbonEventHandler::InstallHandler(EventTargetRef theTarget, const EventTypeSpec *theEvents)
{	OSStatus	theErr;



	// Validate our parameters
	NN_ASSERT(theTarget != NULL);



	// Remove any existing handler
	RemoveHandler();



	// Add the events
	AddEvents(theEvents);



	// Install the handler
	theEvents = mEvents.empty() ? NULL : &mEvents[0];

	theErr = InstallEventHandler(theTarget, mEventHandlerUPP, mEvents.size(), theEvents, this, &mEventHandler);
	NN_ASSERT_NOERR(theErr);
	
	return(theErr);
}





//============================================================================
//		NCarbonEventHandler::EventHandler : Carbon Event handler.
//----------------------------------------------------------------------------
OSStatus NCarbonEventHandler::EventHandler(EventHandlerCallRef callRef, EventRef eventRef, void *userData)
{	NCarbonEventHandler		*thisPtr = (NCarbonEventHandler *) userData;
	NCarbonEvent			theEvent(eventRef);



	// Validate our parameters
	NN_ASSERT(eventRef != NULL);
	NN_ASSERT(thisPtr  != NULL);



	// Dispatch the event
	return(thisPtr->DispatchEvent(theEvent, callRef));
}


