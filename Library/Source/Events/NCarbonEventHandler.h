/*	NAME:
		NCarbonEventHandler.h

	DESCRIPTION:
		Carbon Event handler.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NCARBONEVENTHANDLER_HDR
#define NCARBONEVENTHANDLER_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <vector>

#include "NCarbonEvent.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
// Events
enum {
	// Meta
	kEventClassNone													= 0,
	kEventKindNone													= 0,


	// Control events
	kEventClassNControl												= 'Nctl',
	kEventNControlInitialized,


	// NHIWebView events
	kEventClassNWebView												= 'Nweb',

	kEventNWebFrameLoadStarted,
	kEventNWebFrameLoadCommitted,
	kEventNWebFrameLoadFinished,
	kEventNWebFrameLoadFailed,
	kEventNWebFrameSetTitle,
	kEventNWebFrameSetIcon,
	kEventNWebFrameServerRedirected,
	kEventNWebFrameClientRedirecting,
	kEventNWebFrameClientRedirectCancelled,
	kEventNWebFrameClosing,
	kEventNWebFrameChangedLocation,
	
	kEventNWebResourceGetIdentifier,
	kEventNWebResourceSendingRequest,
	kEventNWebResourceReceivedChallenge,
	kEventNWebResourceReceivedResponse,
	kEventNWebResourceReceivedContentLength,
	kEventNWebResourceChallengeCancelled,
	kEventNWebResourceLoadFinished,
	kEventNWebResourceLoadFailed,
	kEventNWebResourcePlugInFailed,

	kEventNWebInterfaceCreateWindow,
	kEventNWebInterfaceSelectWindow,
	kEventNWebInterfaceCloseWindow,
	kEventNWebInterfaceGetWindowBounds,
	kEventNWebInterfaceSetWindowBounds,
	kEventNWebInterfaceGetViewFrame,
	kEventNWebInterfaceSetViewFrame,
	kEventNWebInterfaceMouseOver,
	kEventNWebInterfaceGetStatusText,
	kEventNWebInterfaceSetStatusText,
	kEventNWebInterfaceIsToolbarVisible,
	kEventNWebInterfaceSetToolbarVisible,
	kEventNWebInterfaceIsStatusVisible,
	kEventNWebInterfaceSetStatusVisible,
	kEventNWebInterfaceIsWindowResizable,
	kEventNWebInterfaceSetWindowResizable,
	kEventNWebInterfaceJavaScriptAlert,
	kEventNWebInterfaceJavaScriptConfirm,
	kEventNWebInterfaceJavaScriptInput,
	kEventNWebInterfaceChooseFile
};


// Event parameters
enum {
	// kEventClassControl / kEventControlDraw
	kEventNParamIsPrinting											= 'Nprn',	// typeBoolean
	kEventNParamPageRect											= 'Nrct',	// typeHIRect
	
	
	// kEventClassNWebView / kEventNWebxxx
	kEventNParamAcepted												= 'Nacc',	// typeBoolean
	kEventNParamContentLength										= 'Ncol',	// typeUInt32
	kEventNParamDate												= 'Ndat',	// typeCFTypeRef		(CFDateRef)
	kEventNParamDefault												= 'Ndef',	// typeUnicodeText
	kEventNParamDelay												= 'Ndel',	// typeFloat
	kEventNParamElementInfo											= 'Nein',	// typeCFTypeRef		(typeCFDictionaryRef)
	kEventNParamIdentifier											= 'Nide',	// typeCFTypeRef
	kEventNParamImage												= 'Nimg',	// typeCFTypeRef		(CGImageRef)
	kEventNParamIsMainFrame											= 'Nism',	// typeBoolean
	kEventNParamMessage												= 'Nmsg',	// typeUnicodeText
	kEventNParamNSError												= 'Nner',	// typeNSError
	kEventNParamNSURLAuthenticationChallenge						= 'Nnuc',	// typeNSURLAuthenticationChallenge
	kEventNParamNSURLRequest										= 'Nnur',	// typeNSURLRequest
	kEventNParamNSURLResponse										= 'Nnup',	// typeNSURLResponse
	kEventNParamResizable											= 'Nsiz',	// typeBoolean
	kEventNParamStatus												= 'Nsta',	// typeUnicodeText
	kEventNParamTitle												= 'Nttl',	// typeUnicodeText
	kEventNParamURL													= 'Nurl',	// typeCFTypeRef		(CFURLRef)
	kEventNParamVisibile											= 'Nvis',	// typeBoolean
	kEventNParamWebDataSource										= 'Nwds',	// typeWebDataSource
	kEventNParamWebFrame											= 'Nwfr',	// typeWebFrame
	kEventNParamWebView												= 'Nwvw',	// typeWebView


	// Types
	typeNSError														= 'nNEr',	// NSError*
	typeNSURLAuthenticationChallenge								= 'nNAC',	// NSURLAuthenticationChallenge*
	typeNSURLRequest												= 'nNUr',	// NSURLRequest*
	typeNSURLResponse												= 'nNRe',	// NSURLResponse*
	typeWebDataSource												= 'nWDs',	// WebDataSource*
	typeWebFrame													= 'nWFr',	// WebFrame*
	typeWebView														= 'nWVw'	// WebView*
};


// Commands
enum {
	// Toggle toolbar
	//
	// Although HIToolbar.h provides commands to show and hide a toolbar, typically a
	// single menu command would be used to adjust a toolbar's visibility.
	//
	// This command (requested as rdar://4409814, available as kHICommandToggleToolbar
	// in 10.5) can be used to produce a menu item that will toggle the visibility of
	// the window's toolbar, and whose text switches between "Show Toolbar" and "Hide
	// Toolbar".
	//
	// This command is typically processed by NWindowController.
	kHICommandNToggleToolbar										= 'tbtg',


	// Back/forwards
	//
	// These commands are typically process by NHIWebView.
	kHICommandNBack													= 'Nbak',
	kHICommandNForward												= 'Nfwd',
	

	// Null command
	kHICommandNone													= 0
};


// Command status
typedef enum {
	kCommandEnable,
	kCommandDisable,
	kCommandDefault
} CommandStatus;





//============================================================================
//		Types
//----------------------------------------------------------------------------
typedef std::vector<EventTypeSpec>							EventTypeSpecList;
typedef EventTypeSpecList::iterator							EventTypeSpecListIterator;
typedef EventTypeSpecList::const_iterator					EventTypeSpecListConstIterator;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NCarbonEventHandler {
public:
										NCarbonEventHandler(void);
	virtual								~NCarbonEventHandler(void);


	// Install the handler
	//
	// To receive events, a handler must be registered on an event target. The
	// handler may only be assigned once, and can not be changed.
	//
	// A list of initial events to register for may also be supplied, terminated
	// with { kEventClassNone, kEventKindNone };
	OSStatus							InstallHandler(                        const EventTypeSpec *theEvents=NULL);
	OSStatus							InstallHandler(MenuRef     theMenu,    const EventTypeSpec *theEvents=NULL);
	OSStatus							InstallHandler(HIWindowRef theWindow,  const EventTypeSpec *theEvents=NULL);
	OSStatus							InstallHandler(ControlRef  theControl, const EventTypeSpec *theEvents=NULL);
	OSStatus							InstallHandler(HIObjectRef theObject,  const EventTypeSpec *theEvents=NULL);


	// Remove the handler
	void								RemoveHandler(void);


	// Get/set the delegate handler
	//
	// The delegate handler is forwarded events prior to them being processed by this object.
	// This allows one object to receive events from two handlers, by registering itself as
	// a delegate of another handler.
	//
	// This is most commonly used to collect events that are only dispatched to a particular
	// class of handler.
	//
	// For example, a view which wished to receive kEventRawKeyModifiersChanged events could
	// install itself as a delegate of an application event handler, and receive events that
	// are normally only sent to the application.
	NCarbonEventHandler					*GetEventDelegate(void) const;
	void								SetEventDelegate(NCarbonEventHandler *theDelegate);


	// Add/remove events
	//
	// Events may be added/removed before or after the handler is installed.
	void								AddEvent(UInt32 theClass, UInt32 theKind);
	void								AddEvents(const EventTypeSpec     *theEvents);
	void								AddEvents(const EventTypeSpecList &theEvents);

	OSStatus							RemoveEvent(UInt32 theClass, UInt32 theKind);
	OSStatus							RemoveEvents(const EventTypeSpec     *theEvents);
	OSStatus							RemoveEvents(const EventTypeSpecList &theEvents);


	// Is an event active?
	//
	// Events must be added to a handler to be active; typically a handler will only receive
	// the events it has registered for.
	bool								IsEventActive(UInt32 theClass, UInt32 theKind) const;
	

	// Dispatch an event
	//
	// Disposes the event to the appropriate DoEventXXXX method. Events dispatched with
	// this method will not be passed to the standard handler if they are not handled.
	OSStatus							DispatchEvent(NCarbonEvent &theEvent, EventHandlerCallRef callRef=NULL);


protected:
	// Call the next handler
	//
	// May only be called from within a DoEventXXXX method.
	OSStatus							CallNextHandler(NCarbonEvent &theEvent);


	// Convert a bool to a CommandStatus
	CommandStatus						EnableIf(bool theTest, CommandStatus notEnabled=kCommandDisable);


	// Get the status of a command
	//
	// The base class provides a default kEventCommandUpdateStatus handler which
	// invokes this method to determine the current status of menu commands.
	//
	// Sub-classes should return the current status for their commands, and call
	// through to the base class for unknown commands.
	virtual CommandStatus				GetCommandStatus(const HICommandExtended &hiCmd);


	// Handle event changes
	//
	// Sub-classes can override to detect when the list of events registered on
	// this object's handler is changed.
	virtual void						DoEventsChanged(void);


	// Handle events
	//
	// All event decoding and dispatch is handled by the base class. Sub-classes
	// should install their handler, register for events, and then override the
	// relevant methods to process those events.
	//
	// Methods are named after their event; to process kEventFooBar, sub-classes
	// should override the DoEventFooBar method.
	//
	// Unknown events (e.g., custom application events) are passed to DoEvent for
	// processing if no other method handles the event.
	virtual OSStatus					DoEvent(NCarbonEvent &theEvent);

	virtual OSStatus					DoEventAccessibleGetAllActionNames(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAccessibleGetAllAttributeNames(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAccessibleGetAllParameterizedAttributeNames(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAccessibleGetChildAtPoint(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAccessibleGetFocusedChild(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAccessibleGetNamedActionDescription(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAccessibleGetNamedAttribute(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAccessibleIsNamedAttributeSettable(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAccessiblePerformNamedAction(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAccessibleSetNamedAttribute(					NCarbonEvent &theEvent);

	virtual OSStatus					DoEventAppActivated(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppActiveWindowChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppAvailableWindowBoundsChanged(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppDeactivated(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppFocusDrawer(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppFocusMenuBar(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppFocusNextDocumentWindow(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppFocusNextFloatingWindow(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppFocusToolbar(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppFrontSwitched(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppGetDockTileMenu(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppHidden(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppIsEventInInstantMouser(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppLaunched(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppLaunchNotification(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppQuit(							NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppShown(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppSystemUIModeChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventAppTerminated(					NCarbonEvent &theEvent);

	virtual OSStatus					DoEventAppearanceScrollBarVariantChanged(NCarbonEvent &theEvent);

	virtual OSStatus					DoEventAppleEvent(NCarbonEvent &theEvent);

	virtual OSStatus					DoEventComboBoxListItemSelected(NCarbonEvent &theEvent);

	virtual OSStatus					DoEventCommandProcess(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventCommandUpdateStatus(	NCarbonEvent &theEvent);

	virtual OSStatus					DoEventControlActivate(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlAddedSubControl(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlApplyBackground(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlApplyTextColor(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlArbitraryMessage(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlBoundsChanged(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlClick(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlContextualMenuClick(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDeactivate(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDispose(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDragEnter(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDragLeave(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDragReceive(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDragWithin(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDraw(							NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlEnabledStateChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetActionProcPart(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetAutoToggleValue(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetClickActivation(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetData(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetFocusPart(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetFrameMetrics(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetIndicatorDragConstraint(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetNextFocusCandidate(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetOptimalBounds(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetPartBounds(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetPartRegion(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetScrollToHereStartPoint(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGetSizeConstraints(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlGhostingFinished(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlHiliteChanged(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlHit(							NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlHitTest(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlIndicatorMoved(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlInitialize(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlInterceptSubviewClick(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlInvalidateForSizeChange(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlLayoutInfoChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlOwningWindowChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlRemovingSubControl(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlSetCursor(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlSetData(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlSetFocusPart(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlSimulateHit(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlTitleChanged(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlTrack(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlTrackingAreaEntered(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlTrackingAreaExited(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlValueFieldChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlVisibilityChanged(			NCarbonEvent &theEvent);

	virtual OSStatus					DoEventHIObjectConstruct(     NCarbonEvent &theEvent);
	virtual OSStatus					DoEventHIObjectInitialize(    NCarbonEvent &theEvent);
	virtual OSStatus					DoEventHIObjectDestruct(      NCarbonEvent &theEvent);
	virtual OSStatus					DoEventHIObjectIsEqual(       NCarbonEvent &theEvent);
	virtual OSStatus					DoEventHIObjectPrintDebugInfo(NCarbonEvent &theEvent);
	virtual OSStatus					DoEventHIObjectEncode(        NCarbonEvent &theEvent);

	virtual OSStatus					DoEventHotKeyPressed(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventHotKeyReleased(	NCarbonEvent &theEvent);

	virtual OSStatus					DoEventInkGesture(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventInkPoint(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventInkText(		NCarbonEvent &theEvent);

	virtual OSStatus					DoEventMenuBarHidden(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuBarShown(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuBecomeScrollable(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuBeginTracking(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuCalculateSize(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuCeaseToBeScrollable(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuChangeTrackingMode(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuClosed(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuCreateFrameView(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuDispose(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuDrawItem(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuDrawItemContent(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuEnableItems(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuEndTracking(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuGetFrameBounds(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuMatchKey(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuMeasureItemHeight(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuMeasureItemWidth(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuOpening(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuPopulate(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMenuTargetItem(			NCarbonEvent &theEvent);

	virtual OSStatus					DoEventMouseDown(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMouseDragged(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMouseEntered(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMouseExited(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMouseMoved(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMouseUp(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventMouseWheelMoved(	NCarbonEvent &theEvent);

	virtual OSStatus					DoEventNControlInitialized(NCarbonEvent &theEvent);

	virtual OSStatus					DoEventNWebFrameLoadStarted(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameLoadCommitted(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameLoadFinished(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameLoadFailed(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameSetTitle(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameSetIcon(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameServerRedirected(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameClientRedirecting(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameClientRedirectCancelled(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameClosing(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameChangedLocation(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebResourceGetIdentifier(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebResourceSendingRequest(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebResourceReceivedChallenge(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebResourceReceivedResponse(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebResourceReceivedContentLength(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebResourceChallengeCancelled(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebResourceLoadFinished(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebResourceLoadFailed(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebResourcePlugInFailed(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceCreateWindow(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSelectWindow(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceCloseWindow(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceGetWindowBounds(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetWindowBounds(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceGetViewFrame(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetViewFrame(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceMouseOver(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceGetStatusText(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetStatusText(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceIsToolbarVisible(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetToolbarVisible(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceIsStatusVisible(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetStatusVisible(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceIsWindowResizable(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetWindowResizable(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceJavaScriptAlert(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceJavaScriptConfirm(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceJavaScriptInput(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceChooseFile(				NCarbonEvent &theEvent);

	virtual OSStatus					DoEventServiceCopy(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventServiceGetTypes(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventServicePaste(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventServicePerform(	NCarbonEvent &theEvent);

	virtual OSStatus					DoEventSystemTimeDateChanged(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventSystemUserSessionActivated(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventSystemUserSessionDeactivated(NCarbonEvent &theEvent);

	virtual OSStatus					DoEventTabletPoint(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTabletProximity(	NCarbonEvent &theEvent);

	virtual OSStatus					DoEventTextAccepted(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextDidChange(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextShouldChangeInRange(	NCarbonEvent &theEvent);

	virtual OSStatus					DoEventTextInputFilterText(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextInputGetSelectedText(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextInputOffsetToPos(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextInputPosToOffset(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextInputShowHideBottomWindow(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextInputUnicodeForKeyEvent(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextInputUnicodeText(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTextInputUpdateActiveInputArea(	NCarbonEvent &theEvent);

	virtual OSStatus					DoEventToolbarBeginMultiChange(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarCreateItemFromDrag(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarCreateItemWithIdentifier(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarDisplayModeChanged(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarDisplaySizeChanged(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarEndMultiChange(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarGetAllowedIdentifiers(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarGetDefaultIdentifiers(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarGetSelectableIdentifiers(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemAdded(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemRemoved(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarLayoutChanged(			NCarbonEvent &theEvent);

	virtual OSStatus					DoEventToolbarItemAcceptDrop(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemCommandIDChanged(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemCreateCustomView(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemEnabledStateChanged(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemGetPersistentData(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemHelpTextChanged(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemImageChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemLabelChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemPerformAction(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemSelectedStateChanged(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemWouldAcceptDrop(		NCarbonEvent &theEvent);

	virtual OSStatus					DoEventToolbarItemViewConfigForMode(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemViewConfigForSize(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemViewEnterConfigMode(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventToolbarItemViewExitConfigMode(	NCarbonEvent &theEvent);

	virtual OSStatus					DoEventTSMDocumentAccessGetCharacters(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTSMDocumentAccessGetCharactersPtr(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTSMDocumentAccessGetCharactersPtrForLargestBuffer(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTSMDocumentAccessGetFont(							NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTSMDocumentAccessGetGlyphInfo(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTSMDocumentAccessGetLength(							NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTSMDocumentAccessGetSelectedRange(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTSMDocumentAccessLockDocument(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventTSMDocumentAccessUnlockDocument(						NCarbonEvent &theEvent);

	virtual OSStatus					DoEventRawKeyDown(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventRawKeyModifiersChanged(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventRawKeyRepeat(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventRawKeyUp(				NCarbonEvent &theEvent);

	virtual OSStatus					DoEventScrollableGetInfo(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventScrollableInfoChanged(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventScrollableScrollTo(		NCarbonEvent &theEvent);

	virtual OSStatus					DoEventVolumeMounted(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventVolumeUnmounted(	NCarbonEvent &theEvent);

	virtual OSStatus					DoEventWindowActivated(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowBoundsChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowBoundsChanging(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClickCloseRgn(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClickCollapseRgn(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClickContentRgn(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClickDragRgn(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClickProxyIconRgn(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClickResizeRgn(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClickStructureRgn(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClickToolbarButtonRgn(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClickZoomRgn(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClose(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowCloseAll(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowClosed(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowCollapse(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowCollapseAll(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowCollapsed(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowCollapsing(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowConstrain(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowContextualMenuSelect(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowCursorChange(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDeactivated(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDispose(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDragCompleted(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDragHilite(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDragStarted(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDrawContent(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDrawerClosed(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDrawerClosing(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDrawerOpened(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDrawerOpening(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDrawFrame(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDrawGrowBox(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowDrawPart(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowExpand(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowExpandAll(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowExpanded(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowExpanding(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowFocusAcquired(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowFocusContent(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowFocusDrawer(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowFocusRelinquish(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowFocusToolbar(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowGetClickActivation(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowGetClickModality(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowGetDockTileMenu(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowGetGrowImageRegion(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowGetIdealSize(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowGetMaximumSize(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowGetMinimumSize(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowGetRegion(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowHandleActivate(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowHandleContentClick(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowHandleDeactivate(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowHidden(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowHiding(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowHitTest(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowInit(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowMeasureTitle(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowModified(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowPaint(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowPathSelect(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowProxyBeginDrag(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowProxyEndDrag(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowResizeCompleted(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowResizeStarted(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowSetupProxyDragImage(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowSheetClosed(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowSheetClosing(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowSheetOpened(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowSheetOpening(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowShowing(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowShown(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowStateChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowToolbarSwitchMode(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowTransitionCompleted(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowTransitionStarted(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowUpdate(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowZoom(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowZoomAll(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowZoomed(				NCarbonEvent &theEvent);


private:
	OSStatus							InstallHandler(EventTargetRef theTarget, const EventTypeSpec *theEvents);

	static OSStatus						EventHandler(EventHandlerCallRef callRef, EventRef eventRef, void *userData);


private:
	EventTypeSpecList					mEvents;
	EventHandlerRef						mEventHandler;

	bool								*mIsDead;
	NCarbonEventHandler					*mDelegate;
	EventHandlerCallRef					mNextHandler;

	static EventHandlerUPP				mEventHandlerUPP;
};




#endif // NCARBONEVENTHANDLER_HDR


