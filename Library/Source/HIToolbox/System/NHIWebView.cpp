/*	NAME:
		NHIWebView.cpp

	DESCRIPTION:
		WebKit view.
		
		
		Unfortunately Interface Builder does not support HIWebView. And since
		adding an HIView with the HIWebView class ID to a nib causes a crash
		(rdar://4513317), we can not subclass HIWebView directly.

		The approach NHIWebView takes is to register as a standard HIView
		subclass, and embed an HIWebView inside us that is bound to our sides.
		
		
		The WebView class uses an Objective-C delegate for more customisation
		tasks. The NHIWebView class converts these delegates methods into
		Carbon Events, and dispatches them to the view for processing.
		
		Sub-classes should register for the events they are interested in, and
		process them like any other HIView object.
		
		NHIWebView registers for the more common events, and provides a default
		implementation that an be customised as required.
		
		Note that like most view-specific events, these events are only sent
		to the view, and are not dispatched up the view hierarchy.
		
		If a higher level object such as a document wants to process events on
		behalf of a web view, without using a subclass, SetEventDelegate can be
		used to forward events.

	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <WebKit/WebKit.h>
#include <WebKit/HIWebView.h>
#include <WebKit/CarbonUtils.h>

#include "NCoreGraphicsUtilities.h"
#include "NHIViewUtilities.h"
#include "NHIWebView.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Tags
static const OSType kTagURL											= 'gurl';





//============================================================================
//		Internal macros
//----------------------------------------------------------------------------
DEFINE_HIVIEW_SUBCLASS(NHIWebView, "com.nano.hiwebview", kHIViewClassID);





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
@interface NHIWebViewDelegate : NSObject
{
	NHIWebView		*mView;
}

- (id) initWithView:(NHIWebView*) theView;

@end





//============================================================================
//		NHIWebViewDelegate::init : Initializer.
//----------------------------------------------------------------------------
@implementation NHIWebViewDelegate

- (id) initWithView:(NHIWebView*) theView
{


	// Initialize ourselves
    [super init];
	
	mView = theView;

    return(self);
}





//============================================================================
//		NHIWebViewDelegate::getHIRect : Get an HIRect.
//----------------------------------------------------------------------------
- (HIRect) getHIRect:(NSRect) nsRect
{	HIRect		hiRect;



	// Convert the rectangle
	hiRect.origin.x    = nsRect.origin.x;
	hiRect.origin.y    = nsRect.origin.y;
	hiRect.size.width  = nsRect.size.width;
	hiRect.size.height = nsRect.size.height;

	return(hiRect);
}





//============================================================================
//		NHIWebViewDelegate::getNSRect : Get an NSRect.
//----------------------------------------------------------------------------
- (NSRect) getNSRect:(HIRect) hiRect
{	NSRect		nsRect;



	// Convert the rectangle
	nsRect.origin.x    = hiRect.origin.x;
	nsRect.origin.y    = hiRect.origin.y;
	nsRect.size.width  = hiRect.size.width;
	nsRect.size.height = hiRect.size.height;

	return(nsRect);
}





//============================================================================
//		NHIWebViewDelegate::getWebEvent : Get a web event.
//----------------------------------------------------------------------------
- (NCarbonEvent) getWebEvent:(UInt32) theKind forFrame:(WebFrame *) theFrame withSource:(WebDataSource *) theSource
{	NCarbonEvent	theEvent(kEventClassNWebView, theKind);
	bool			isMainFrame;



	// Get the state we need
	isMainFrame = (theFrame != NULL && theFrame == [mView->GetWebView() mainFrame]);

	if (theFrame != NULL && theSource == NULL)
		theSource = [theFrame dataSource];



	// Prepare the event
	theEvent.SetParameterHIView( kEventParamDirectObject, *mView);
	theEvent.SetParameterBoolean(kEventNParamIsMainFrame, isMainFrame);
	theEvent.SetParameter(       kEventNParamWebFrame,      typeWebFrame,      sizeof(WebFrame      *), &theFrame);
	theEvent.SetParameter(       kEventNParamWebDataSource, typeWebDataSource, sizeof(WebDataSource *), &theSource);
	
	return(theEvent);
}





//============================================================================
//		NHIWebViewDelegate::sendWebEvent : Send a web event.
//----------------------------------------------------------------------------
- (OSStatus) sendWebEvent:(NCarbonEvent) theEvent
{


	// Send the event
	return(theEvent.SendToHIView(*mView, kEventTargetDontPropagate));
}





//============================================================================
//		NHIWebViewDelegate::respondsToSelector : Do we respond to a selector?
//----------------------------------------------------------------------------
- (BOOL) respondsToSelector:(SEL) theSelector
{


	// Check our selectors
	//
	// For efficieny, and to obtain the default WebKit behaviour for unhandled
	// methods, we implement all selectors but only respond to those that our
	// view's handler is listening for.
	#define CHECK_WEB_EVENT(_eventKind, _selector)								\
		if (theSelector == @selector(_selector))								\
			return(mView->IsEventActive(kEventClassNWebView, _eventKind));

	CHECK_WEB_EVENT(kEventNWebFrameLoadStarted,					webView:didStartProvisionalLoadForFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameLoadCommitted,				webView:didCommitLoadForFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameLoadFinished,				webView:didFinishLoadForFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameLoadFailed,					webView:didFailLoadWithError:forFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameLoadFailed,					webView:didFailProvisionalLoadWithError:forFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameSetTitle,					webView:didReceiveTitle:forFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameSetIcon,						webView:didReceiveIcon:forFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameServerRedirected,			webView:didReceiveServerRedirectForProvisionalLoadForFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameClientRedirecting,			webView:willPerformClientRedirectToURL:delay:fireDate:forFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameClientRedirectCancelled,		webView:didCancelClientRedirectForFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameClosing,						webView:willCloseFrame:);
	CHECK_WEB_EVENT(kEventNWebFrameChangedLocation,				webView:didChangeLocationWithinPageForFrame:);
	CHECK_WEB_EVENT(kEventNWebResourceGetIdentifier,			webView:identifierForInitialRequest:fromDataSource:);
	CHECK_WEB_EVENT(kEventNWebResourceSendingRequest,			webView:resource:willSendRequest:redirectResponse:fromDataSource:);
	CHECK_WEB_EVENT(kEventNWebResourceReceivedChallenge,		webView:resource:didReceiveAuthenticationChallenge:fromDataSource:);
	CHECK_WEB_EVENT(kEventNWebResourceReceivedResponse,			webView:resource:didReceiveResponse:fromDataSource:);
	CHECK_WEB_EVENT(kEventNWebResourceReceivedContentLength,	webView:resource:didReceiveContentLength:fromDataSource:);
	CHECK_WEB_EVENT(kEventNWebResourceChallengeCancelled,		webView:resource:didCancelAuthenticationChallenge:fromDataSource:);
	CHECK_WEB_EVENT(kEventNWebResourceLoadFinished,				webView:resource:didFinishLoadingFromDataSource:);
	CHECK_WEB_EVENT(kEventNWebResourceLoadFailed,				webView:resource:didFailLoadingWithError:fromDataSource:);
	CHECK_WEB_EVENT(kEventNWebResourcePlugInFailed,				webView:plugInFailedWithError:dataSource:);
	CHECK_WEB_EVENT(kEventNWebInterfaceCreateWindow,			webView:createWebViewWithRequest:);
	CHECK_WEB_EVENT(kEventNWebInterfaceSelectWindow,			webViewShow:);
	CHECK_WEB_EVENT(kEventNWebInterfaceSelectWindow,			webViewFocus:);
	CHECK_WEB_EVENT(kEventNWebInterfaceCloseWindow,				webViewClose:);
	CHECK_WEB_EVENT(kEventNWebInterfaceSetWindowBounds,			webView:setFrame:);
	CHECK_WEB_EVENT(kEventNWebInterfaceGetWindowBounds,			webViewFrame:);
	CHECK_WEB_EVENT(kEventNWebInterfaceSetViewFrame,			webView:setContentRect:);
	CHECK_WEB_EVENT(kEventNWebInterfaceGetViewFrame,			webViewContentRect:);
	CHECK_WEB_EVENT(kEventNWebInterfaceMouseOver,				webView:mouseDidMoveOverElement:modifierFlags:);
	CHECK_WEB_EVENT(kEventNWebInterfaceGetStatusText,			webViewStatusText:);
	CHECK_WEB_EVENT(kEventNWebInterfaceSetStatusText,			webView:setStatusText:);
	CHECK_WEB_EVENT(kEventNWebInterfaceIsToolbarVisible,		webViewAreToolbarsVisible:);
	CHECK_WEB_EVENT(kEventNWebInterfaceSetToolbarVisible,		webView:setToolbarsVisible:);
	CHECK_WEB_EVENT(kEventNWebInterfaceIsStatusVisible,			webViewIsStatusBarVisible:);
	CHECK_WEB_EVENT(kEventNWebInterfaceSetStatusVisible,		webView:setStatusBarVisible:);
	CHECK_WEB_EVENT(kEventNWebInterfaceIsWindowResizable,		webViewIsResizable:);
	CHECK_WEB_EVENT(kEventNWebInterfaceSetWindowResizable,		webView:setResizable:);
	CHECK_WEB_EVENT(kEventNWebInterfaceJavaScriptAlert,			webView:runJavaScriptAlertPanelWithMessage:);
	CHECK_WEB_EVENT(kEventNWebInterfaceJavaScriptConfirm,		webView:runJavaScriptConfirmPanelWithMessage:);
	CHECK_WEB_EVENT(kEventNWebInterfaceJavaScriptInput,			webView:runJavaScriptTextInputPanelWithPrompt:defaultText:);
	CHECK_WEB_EVENT(kEventNWebInterfaceChooseFile,				webView:runOpenPanelForFileButtonWithResultListener:);

	#undef CHECK_WEB_EVENT



	// Check our parent
	return([super respondsToSelector:theSelector]);
}





//============================================================================
//		NHIWebViewDelegate::webView:didStartProvisionalLoadForFrame : Start a load.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView didStartProvisionalLoadForFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameLoadStarted forFrame:theFrame withSource:NULL];
	theEvent.SetParameterCFType(kEventNParamURL, (CFURLRef) [[[theFrame provisionalDataSource] request] URL]);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:didCommitLoadForFrame : Commit a load.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView didCommitLoadForFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameLoadCommitted forFrame:theFrame withSource:NULL];

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:didFinishLoadForFrame : Finish a load.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView didFinishLoadForFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameLoadFinished forFrame:theFrame withSource:NULL];

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:didFailLoadWithError : Handle failure.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView didFailLoadWithError:(NSError *) theError forFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameLoadFailed forFrame:theFrame withSource:NULL];
	theEvent.SetParameter(kEventNParamNSError, typeNSError, sizeof(NSError*), &theError);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:didFailProvisionalLoadWithError : Handle failure.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView didFailProvisionalLoadWithError:(NSError *) theError forFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameLoadFailed forFrame:theFrame withSource:NULL];
	theEvent.SetParameter(kEventNParamNSError, typeNSError, sizeof(NSError*), &theError);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:didReceiveTitle : Set the title.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView didReceiveTitle:(NSString *) theTitle forFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameSetTitle forFrame:theFrame withSource:NULL];
	theEvent.SetParameterString(kEventNParamTitle, (CFStringRef) theTitle);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:didReceiveIcon : Set the icon.
//----------------------------------------------------------------------------
- (void)webView:(WebView *) webView didReceiveIcon:(NSImage *) theImage forFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;
	NCGImage		cgImage;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	if (cgImage.Set(WebConvertNSImageToCGImageRef(theImage)))
		{
		theEvent = [self getWebEvent:kEventNWebFrameSetIcon forFrame:theFrame withSource:NULL];
		theEvent.SetParameterCFType(kEventNParamImage, (CGImageRef) cgImage);

		[self sendWebEvent:theEvent];
		}
}





//============================================================================
//		NHIWebViewDelegate::webView:didReceiveServerRedirectForProvisionalLoadForFrame : Handle redirects.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView didReceiveServerRedirectForProvisionalLoadForFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameServerRedirected forFrame:theFrame withSource:NULL];

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:willPerformClientRedirectToURL : Handle redirects.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView willPerformClientRedirectToURL:(NSURL *) theURL delay:(NSTimeInterval) theDelay fireDate:(NSDate *) theDate forFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameClientRedirecting forFrame:theFrame withSource:NULL];
	theEvent.SetParameterCFType( kEventNParamURL,   (CFURLRef)  theURL);
	theEvent.SetParameterCFType( kEventNParamDate,  (CFDateRef) theDate);
	theEvent.SetParameterFloat64(kEventNParamDelay,             theDelay);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:didCancelClientRedirectForFrame : Cancel a redirect.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView didCancelClientRedirectForFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameClientRedirectCancelled forFrame:theFrame withSource:NULL];

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:willCloseFrame : The frame has closed.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView willCloseFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameClosing forFrame:theFrame withSource:NULL];

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:didChangeLocationWithinPageForFrame : The location has changed.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView didChangeLocationWithinPageForFrame:(WebFrame *) theFrame
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebFrameChangedLocation forFrame:theFrame withSource:NULL];

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:identifierForInitialRequest : Get the resource identifier.
//----------------------------------------------------------------------------
- (id) webView:(WebView *) webView identifierForInitialRequest:(NSURLRequest *) theRequest fromDataSource:(WebDataSource *) dataSource
{	id				theResult;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = NULL;
	theEvent  = [self getWebEvent:kEventNWebResourceGetIdentifier forFrame:NULL withSource:dataSource];
	theEvent.SetParameter(kEventNParamNSURLRequest, typeNSURLRequest, sizeof(NSURLRequest*), &theRequest);

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theResult = (id) theEvent.GetParameterCFType(kEventNParamIdentifier);
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::webView:resource:willSendRequest : Prepare a request.
//----------------------------------------------------------------------------
- (NSURLRequest *) webView:(WebView *) webView resource:(id) theID willSendRequest:(NSURLRequest *) theRequest redirectResponse:(NSURLResponse *) redirectResponse fromDataSource:(WebDataSource *) dataSource
{	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebResourceSendingRequest forFrame:NULL withSource:dataSource];
	theEvent.SetParameterCFType(kEventNParamIdentifier, (CFTypeRef) theID);
	theEvent.SetParameter(kEventNParamNSURLRequest,  typeNSURLRequest,  sizeof(NSURLRequest  *), &theRequest);
	theEvent.SetParameter(kEventNParamNSURLResponse, typeNSURLResponse, sizeof(NSURLResponse *), &redirectResponse);

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theEvent.GetParameter(kEventNParamNSURLRequest, typeNSURLRequest, sizeof(NSURLRequest*), &theRequest);
	
	return(theRequest);
}





//============================================================================
//		NHIWebViewDelegate::webView:resource:didReceiveAuthenticationChallenge : Received a challenge.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView resource:(id) theID didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *) theChallenge fromDataSource:(WebDataSource *) dataSource
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebResourceReceivedChallenge forFrame:NULL withSource:dataSource];
	theEvent.SetParameterCFType(kEventNParamIdentifier, (CFTypeRef) theID);
	theEvent.SetParameter(kEventNParamNSURLAuthenticationChallenge, typeNSURLAuthenticationChallenge, sizeof(NSURLAuthenticationChallenge*), &theChallenge);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:resource:didReceiveResponse : Received some data.
//----------------------------------------------------------------------------
- (void)webView:(WebView *) webView resource:(id) theID didReceiveResponse:(NSURLResponse *) theResponse fromDataSource:(WebDataSource *) dataSource
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebResourceReceivedResponse forFrame:NULL withSource:dataSource];
	theEvent.SetParameterCFType(kEventNParamIdentifier, (CFTypeRef) theID);
	theEvent.SetParameter(kEventNParamNSURLResponse, typeNSURLResponse, sizeof(NSURLResponse*), &theResponse);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:resource:didReceiveContentLength : Received the size.
//----------------------------------------------------------------------------
- (void)webView:(WebView *) webView resource:(id) theID didReceiveContentLength: (unsigned) theLength fromDataSource:(WebDataSource *) dataSource
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebResourceReceivedContentLength forFrame:NULL withSource:dataSource];
	theEvent.SetParameterCFType(kEventNParamIdentifier, (CFTypeRef) theID);
	theEvent.SetParameterUInt32(kEventNParamContentLength, theLength);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:resource:didCancelAuthenticationChallenge : A challenge has been cancelled.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView resource:(id) theID didCancelAuthenticationChallenge:(NSURLAuthenticationChallenge *) theChallenge fromDataSource:(WebDataSource *) dataSource
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebResourceChallengeCancelled forFrame:NULL withSource:dataSource];
	theEvent.SetParameterCFType(kEventNParamIdentifier, (CFTypeRef) theID);
	theEvent.SetParameter(kEventNParamNSURLAuthenticationChallenge, typeNSURLAuthenticationChallenge, sizeof(NSURLAuthenticationChallenge*), &theChallenge);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:resource:didFinishLoadingFromDataSource : Finished loading.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView resource:(id) theID didFinishLoadingFromDataSource:(WebDataSource *) dataSource
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebResourceLoadFinished forFrame:NULL withSource:dataSource];
	theEvent.SetParameterCFType(kEventNParamIdentifier, (CFTypeRef) theID);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:didFailLoadingWithError : Handle failure.
//----------------------------------------------------------------------------
- (void)webView:(WebView *) webView resource:(id) theID didFailLoadingWithError:(NSError *) theError fromDataSource:(WebDataSource *) dataSource
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebResourceLoadFailed forFrame:NULL withSource:dataSource];
	theEvent.SetParameterCFType(kEventNParamIdentifier, (CFTypeRef) theID);
	theEvent.SetParameter(kEventNParamNSError, typeNSError, sizeof(NSError*), &theError);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:plugInFailedWithError : Handle failure.
//----------------------------------------------------------------------------
- (void)webView:(WebView *) webView plugInFailedWithError:(NSError *) theError dataSource:(WebDataSource *) dataSource
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebResourcePlugInFailed forFrame:NULL withSource:dataSource];
	theEvent.SetParameter(kEventNParamNSError, typeNSError, sizeof(NSError*), &theError);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:createWebViewWithRequest : Create a new window.
//----------------------------------------------------------------------------
- (WebView *) webView:(WebView *) webView createWebViewWithRequest:(NSURLRequest *) theRequest;
{	WebView			*theResult;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = NULL;
	theEvent  = [self getWebEvent:kEventNWebInterfaceCreateWindow forFrame:NULL withSource:NULL];
	theEvent.SetParameter(kEventNParamNSURLRequest, typeNSURLRequest, sizeof(NSURLRequest*), &theRequest);

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theEvent.GetParameter(kEventNParamWebView, typeWebView, sizeof(WebView*), &theResult);
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::webViewShow : Select a WebView window.
//----------------------------------------------------------------------------
- (void) webViewShow:(WebView *) webView;
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceSelectWindow forFrame:NULL withSource:NULL];

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webViewFocus : Select a WebView window.
//----------------------------------------------------------------------------
- (void) webViewFocus:(WebView *) webView;
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceSelectWindow forFrame:NULL withSource:NULL];

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webViewClose : Close a WebView window.
//----------------------------------------------------------------------------
- (void) webViewClose:(WebView *) webView;
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceCloseWindow forFrame:NULL withSource:NULL];

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webViewFrame : Get the window bounds.
//----------------------------------------------------------------------------
- (NSRect) webViewFrame:(WebView *) webView
{	NSRect			theResult;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = NSZeroRect;
	theEvent  = [self getWebEvent:kEventNWebInterfaceGetWindowBounds forFrame:NULL withSource:NULL];

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theResult = [self getNSRect:theEvent.GetParameterHIRect(kEventParamCurrentBounds)];
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::webView:setFrame : Set the window bounds.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView setFrame:(NSRect) theFrame;
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceSetWindowBounds forFrame:NULL withSource:NULL];
	theEvent.SetParameterHIRect(kEventParamCurrentBounds, [self getHIRect:theFrame]);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webViewContentRect : Get the view frame.
//----------------------------------------------------------------------------
- (NSRect) webViewContentRect:(WebView *) webView
{	NSRect			theResult;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = NSZeroRect;
	theEvent  = [self getWebEvent:kEventNWebInterfaceGetViewFrame forFrame:NULL withSource:NULL];

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theResult = [self getNSRect:theEvent.GetParameterHIRect(kEventParamCurrentBounds)];
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::webView:setContentRect : Set the view frame.
//----------------------------------------------------------------------------
- (void)webView:(WebView *) webView setContentRect:(NSRect) contentRect
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceSetViewFrame forFrame:NULL withSource:NULL];
	theEvent.SetParameterHIRect(kEventParamCurrentBounds, [self getHIRect:contentRect]);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:mouseDidMoveOverElement : Handle mouse overs.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView mouseDidMoveOverElement:(NSDictionary *) elementInformation modifierFlags:(unsigned int) modifierFlags
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceMouseOver forFrame:NULL withSource:NULL];
	theEvent.SetParameterCFType(kEventNParamElementInfo, (CFDictionaryRef) elementInformation);
	theEvent.SetParameterUInt32(kEventParamKeyModifiers, modifierFlags);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webViewStatusText : Get the status text.
//----------------------------------------------------------------------------
- (NSString *) webViewStatusText:(WebView *) webView
{	NSString		*theResult;
	NString			theString;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = NULL;
	theEvent  = [self getWebEvent:kEventNWebInterfaceGetStatusText forFrame:NULL withSource:NULL];

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		{
		theString = theEvent.GetParameterString(kEventNParamStatus);
		theResult = [NSString stringWithUTF8String:theString.GetUTF8()];
		}
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::webView:setStatusText : Set the status text.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView setStatusText:(NSString *) theText
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceSetStatusText forFrame:NULL withSource:NULL];
	theEvent.SetParameterString(kEventNParamStatus, (CFStringRef) theText);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webViewAreToolbarsVisible : Get the toolbar visibility.
//----------------------------------------------------------------------------
- (BOOL) webViewAreToolbarsVisible:(WebView *) webView
{	BOOL			theResult;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = FALSE;
	theEvent  = [self getWebEvent:kEventNWebInterfaceIsToolbarVisible forFrame:NULL withSource:NULL];

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theResult = theEvent.GetParameterBoolean(kEventNParamVisibile);
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::webView:setToolbarsVisible : Set the toolbar visibility.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView setToolbarsVisible:(BOOL) visible
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceSetToolbarVisible forFrame:NULL withSource:NULL];
	theEvent.SetParameterBoolean(kEventNParamVisibile, visible);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webViewIsStatusBarVisible : Get the status bar visibility.
//----------------------------------------------------------------------------
- (BOOL) webViewIsStatusBarVisible:(WebView *) webView
{	BOOL			theResult;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = FALSE;
	theEvent  = [self getWebEvent:kEventNWebInterfaceIsStatusVisible forFrame:NULL withSource:NULL];

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theResult = theEvent.GetParameterBoolean(kEventNParamVisibile);
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::webView:setStatusBarVisible : Set the status bar visibility.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView setStatusBarVisible:(BOOL) visible
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceSetStatusVisible forFrame:NULL withSource:NULL];
	theEvent.SetParameterBoolean(kEventNParamVisibile, visible);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webViewIsResizable : Get the window resizability.
//----------------------------------------------------------------------------
- (BOOL) webViewIsResizable:(WebView *) webView
{	BOOL			theResult;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = FALSE;
	theEvent  = [self getWebEvent:kEventNWebInterfaceIsWindowResizable forFrame:NULL withSource:NULL];

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theResult = theEvent.GetParameterBoolean(kEventNParamResizable);
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::webView:setResizable : Set the window resizability.
//----------------------------------------------------------------------------
- (void) webView:(WebView *)webView setResizable:(BOOL) resizable
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceSetWindowResizable forFrame:NULL withSource:NULL];
	theEvent.SetParameterBoolean(kEventNParamResizable, resizable);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::webView:runJavaScriptAlertPanelWithMessage : JavaScript alert.
//----------------------------------------------------------------------------
- (void) webView:(WebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *) message
{	NCarbonEvent	theEvent;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theEvent = [self getWebEvent:kEventNWebInterfaceJavaScriptAlert forFrame:NULL withSource:NULL];
	theEvent.SetParameterString(kEventNParamMessage, (CFStringRef) message);

	[self sendWebEvent:theEvent];
}





//============================================================================
//		NHIWebViewDelegate::runJavaScriptConfirmPanelWithMessage : JavaScript alert.
//----------------------------------------------------------------------------
- (BOOL) webView:(WebView *)webView runJavaScriptConfirmPanelWithMessage:(NSString *) message
{	BOOL			theResult;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = FALSE;
	theEvent  = [self getWebEvent:kEventNWebInterfaceJavaScriptConfirm forFrame:NULL withSource:NULL];
	theEvent.SetParameterString(kEventNParamMessage, (CFStringRef) message);

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theResult = theEvent.GetParameterBoolean(kEventNParamAcepted);
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::runJavaScriptTextInputPanelWithPrompt : JavaScript alert.
//----------------------------------------------------------------------------
- (NSString *) webView:(WebView *) webView runJavaScriptTextInputPanelWithPrompt:(NSString *) prompt defaultText:(NSString *) defaultText
{	NSString		*theResult;
	NString			theString;
	NCarbonEvent	theEvent;
	OSStatus		theErr;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	theResult = FALSE;
	theEvent  = [self getWebEvent:kEventNWebInterfaceJavaScriptInput forFrame:NULL withSource:NULL];
	theEvent.SetParameterString(kEventNParamMessage, (CFStringRef) prompt);
	theEvent.SetParameterString(kEventNParamDefault, (CFStringRef) defaultText);

	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		{
		theString = theEvent.GetParameterString(kEventNParamMessage);
		theResult = [NSString stringWithUTF8String:theString.GetUTF8()];
		}
	
	return(theResult);
}





//============================================================================
//		NHIWebViewDelegate::runOpenPanelForFileButtonWithResultListener : Display an open dialog.
//----------------------------------------------------------------------------
- (void) webView:(WebView *) webView runOpenPanelForFileButtonWithResultListener:(id<WebOpenPanelResultListener>)resultListener;
{	NString			theString;
	NCarbonEvent	theEvent;
	OSStatus		theErr;
	CFURLRef		cfURL;



	// Ignore unused parameters
	(void) webView;



	// Dispatch the event
	cfURL    = NULL;
	theEvent = [self getWebEvent:kEventNWebInterfaceChooseFile forFrame:NULL withSource:NULL];
	
	theErr = [self sendWebEvent:theEvent];
	if (theErr == noErr)
		theErr = theEvent.GetParameter(kEventNParamURL, typeCFTypeRef, sizeof(CFURLRef), &cfURL);



	// Update the listener
	if (theErr == noErr && cfURL != NULL)
		[resultListener chooseFilename:[((NSURL *) cfURL) path]];
	else
		[resultListener cancel];
}

@end;





//============================================================================
//		NHIWebView::NHIWebView : Constructor.
//----------------------------------------------------------------------------
#pragma mark -
NHIWebView::NHIWebView(HIWindowRef			theWindow,
						const HIRect		&theRect,
						const NString		&theURL)
{


	// Create the view
	ConstructView(HIViewCreate(kClassID, this), theWindow, &theRect);



	// Configure the view
	SetURL(theURL);
}





//============================================================================
//		NHIWebView::~NHIWebView : Destructor.
//----------------------------------------------------------------------------
NHIWebView::~NHIWebView(void)
{


	// Clean up
	[mDelegate release];
}





//============================================================================
//		NHIWebView::GetURL : Get the current URL.
//----------------------------------------------------------------------------
NCFURL NHIWebView::GetURL(void) const
{	NCFURL		theURL;



	// Get the URL
	if (!theURL.Set((CFURLRef) [[[[mWebView mainFrame] dataSource]            request] URL], false))
		 theURL.Set((CFURLRef) [[[[mWebView mainFrame] provisionalDataSource] request] URL], false);

	return(theURL);
}





//============================================================================
//		NHIWebView::SetURL : Set the current URL.
//----------------------------------------------------------------------------
void NHIWebView::SetURL(const NCFURL &theURL)
{	NSURLRequest	*nsRequest;
	NSURL			*nsURL;



	// Load the page
	nsURL     = (NSURL *) ((CFURLRef) theURL);
	nsRequest = [NSURLRequest requestWithURL:nsURL];

	[[mWebView mainFrame] loadRequest:nsRequest];
}





//============================================================================
//		NHIWebView::GoBack : Go back.
//----------------------------------------------------------------------------
bool NHIWebView::GoBack(void) const
{


	// Go back
	return((bool) [mWebView goBack]);
}





//============================================================================
//		NHIWebView::GoForward : Go forward.
//----------------------------------------------------------------------------
bool NHIWebView::GoForward(void) const
{


	// Go forward
	return((bool) [mWebView goForward]);
}





//============================================================================
//		NHIWebView::CanGoBack : Can we go back?
//----------------------------------------------------------------------------
bool NHIWebView::CanGoBack(void) const
{


	// Go back
	return((bool) [mWebView canGoBack]);
}





//============================================================================
//		NHIWebView::CanGoForward : Can we go forward?
//----------------------------------------------------------------------------
bool NHIWebView::CanGoForward(void) const
{


	// Go forward
	return((bool) [mWebView canGoForward]);
}





//============================================================================
//		NHIWebView::GetWebView : Get the web view.
//----------------------------------------------------------------------------
WebView *NHIWebView::GetWebView(void) const
{


	// Get the web view
	return(mWebView);
}





//============================================================================
//		NHIWebView::InitializeView : Initialize the view.
//----------------------------------------------------------------------------
#pragma mark -
void NHIWebView::InitializeView(void)
{	EventTypeSpec		theEvents[] = {	{ kEventClassCommand,  kEventCommandProcess						},
										{ kEventClassCommand,  kEventCommandUpdateStatus				},
										{ kEventClassControl,  kEventControlDraw						},
										{ kEventClassNWebView, kEventNWebFrameSetTitle					},
										{ kEventClassNWebView, kEventNWebFrameSetIcon					},
										{ kEventClassNWebView, kEventNWebInterfaceSelectWindow			},
										{ kEventClassNWebView, kEventNWebInterfaceCloseWindow			},
										{ kEventClassNWebView, kEventNWebInterfaceGetWindowBounds		},
										{ kEventClassNWebView, kEventNWebInterfaceSetWindowBounds		},
										{ kEventClassNWebView, kEventNWebInterfaceGetViewFrame			},
										{ kEventClassNWebView, kEventNWebInterfaceSetViewFrame			},
										{ kEventClassNWebView, kEventNWebInterfaceIsToolbarVisible		},
										{ kEventClassNWebView, kEventNWebInterfaceSetToolbarVisible		},
										{ kEventClassNWebView, kEventNWebInterfaceIsWindowResizable		},
										{ kEventClassNWebView, kEventNWebInterfaceSetWindowResizable	},
										{ kEventClassNone,     kEventKindNone							} };
	OSStatus	theErr;
	HIViewRef	hiView;



	// Initialize WebKit
	WebInitForCarbon();



	// Initialize ourselves
	mView     = NULL;
	mWebView  = NULL;
	mDelegate = NULL;
	
	SetFeatures(kHIViewFeatureAllowsSubviews);



	// Register for events
	AddEvents(theEvents);



	// Create the view
	theErr = HIWebViewCreate(&hiView);
	NN_ASSERT_NOERR(theErr);

	mView = NHIViewUtilities::GetView(hiView);
	NN_ASSERT(mView != NULL);

	mWebView = HIWebViewGetWebView(hiView);
	NN_ASSERT(mWebView != NULL);



	// Create the delegate
	mDelegate = [[NHIWebViewDelegate alloc] initWithView:this];
	NN_ASSERT(mDelegate != NULL);

	DoEventsChanged();



	// Configure the view
	AddSubView(mView);

	mView->SetLayoutInfo(NHIViewUtilities::GetLayoutInfo(kBindAllSides));
	mView->SetFrame(GetBounds());
	mView->SetVisible();



	// Apply our properties
	if (HasProperty(             kPropertyNano, kTagURL))
		SetURL(GetPropertyString(kPropertyNano, kTagURL));
}





//============================================================================
//		NHIWebView::DoEventsChanged : Handle event changes.
//----------------------------------------------------------------------------
void NHIWebView::DoEventsChanged(void)
{


	// Reset the delegate
	//
	// The WebView queries its delegate when it is assigned, and caches the supported
	// methods internally.
	//
	// Our delegate determines what to respond to by examining the events that are
	// registered (both for efficiency, and to allow us to obtain the default WebKit
	// behaviour for events that no handler is installed for).
	//
	// In order to keep the WebView in sync, we need to re-apply the delegate whenever
	// our handler's event list changes.
	//
	//
	// Note that this method will be invoked before our view gets a chance to initialize
	// itself (as the base class is initialized, it will add its own events).
	//
	// To ensure we don't use our member variables before they have been initializedd,
	// we need to wait until one of our events has been added to the handler.
	if (IsEventActive(kEventClassNWebView, kEventNWebFrameSetTitle))
		{
		if (mWebView != NULL && mDelegate != NULL)
			{
			[mWebView    setFrameLoadDelegate:mDelegate];
			[mWebView setResourceLoadDelegate:mDelegate];
			[mWebView           setUIDelegate:mDelegate];
			}
		}
}





//============================================================================
//		NHIWebView::DoEventCommandProcess : Handle kEventCommandProcess.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventCommandProcess(NCarbonEvent &theEvent)
{	OSStatus			theErr;
	HICommandExtended	hiCmd;



	// Handle the command
	hiCmd  = theEvent.GetHICommand();
	theErr = noErr;
	
	switch (hiCmd.commandID) {
		case kHICommandNBack:
			GoBack();
			break;
			
		case kHICommandNForward:
			GoForward();
			break;

		default:
			theErr = NHIView::DoEventCommandProcess(theEvent);
			break;
		}
	
	return(theErr);
}





//============================================================================
//		NHIWebView::DoEventControlDraw : Handle kEventControlDraw.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventControlDraw(NCarbonEvent &/*theEvent*/)
{


	// Invalidate the view
	//
	// HIWebView does not invalidate correctly when placed in a composited
	// window on Mac OS X 10.4 (rdar://4550080).
	//
	// To work around this, we invalidate the WebView ourselves.
	[mWebView setNeedsDisplay:YES];

	return(eventNotHandledErr);
}





//============================================================================
//		NHIWebView::DoEventNWebFrameSetTitle : Handle kEventNWebFrameSetTitle.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebFrameSetTitle(NCarbonEvent &theEvent)
{	NHIWindow	theWindow(GetHIWindow());



	// Set the window title
	if (theEvent.GetParameterBoolean(kEventNParamIsMainFrame))
		theWindow.SetTitle(theEvent.GetParameterString(kEventNParamTitle));
	
	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebFrameSetIcon : Handle kEventNWebFrameSetIcon.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebFrameSetIcon(NCarbonEvent &theEvent)
{	NHIWindow		theWindow(GetHIWindow());
	CGImageRef		cgImage;
	NIcon			theIcon;



	// Set the proxy icon
	if (theEvent.GetParameterBoolean(kEventNParamIsMainFrame))
		{
		cgImage = (CGImageRef) theEvent.GetParameterCFType(kEventNParamImage);
		theIcon = NCoreGraphicsUtilities::CreateIconFromImage(cgImage);
	
		theWindow.SetProxyIcon(theIcon);
		}
	
	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceSelectWindow : Handle kEventNWebInterfaceSelectWindow.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceSelectWindow(NCarbonEvent &/*theEvent*/)
{	NHIWindow		theWindow(GetHIWindow());



	// Select the window
	theWindow.Select();

	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceCloseWindow : Handle kEventNWebInterfaceCloseWindow.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceCloseWindow(NCarbonEvent &/*theEvent*/)
{	NCarbonEvent	closeEvent(kEventClassWindow, kEventWindowClose);



	// Close the window
	closeEvent.SendToHIWindow(GetHIWindow());

	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceGetWindowBounds : Handle kEventNWebInterfaceGetWindowBounds.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceGetWindowBounds(NCarbonEvent &theEvent)
{	NHIWindow		theWindow(GetHIWindow());
	HIRect			theRect;



	// Get the window bounds
	theRect = theWindow.GetFrame(kWindowStructureRgn);
	theEvent.SetParameterHIRect(kEventParamCurrentBounds, theRect);

	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceSetWindowBounds : Handle kEventNWebInterfaceSetWindowBounds.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceSetWindowBounds(NCarbonEvent &theEvent)
{	NHIWindow		theWindow(GetHIWindow());
	HIRect			theRect;



	// Set the window bounds
	theRect = theEvent.GetParameterHIRect(kEventParamCurrentBounds);
	theWindow.SetFrame(theRect, kWindowStructureRgn);

	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceGetViewFrame : Handle kEventNWebInterfaceGetViewFrame.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceGetViewFrame(NCarbonEvent &theEvent)
{	HIRect	theRect;



	// Get the view frame
	theRect = GetFrame();
	theEvent.SetParameterHIRect(kEventParamCurrentBounds, theRect);

	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceSetViewFrame : Handle kEventNWebInterfaceSetViewFrame.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceSetViewFrame(NCarbonEvent &theEvent)
{	HIRect	theRect;



	// Set the view frame
	theRect = theEvent.GetParameterHIRect(kEventParamCurrentBounds);
	SetFrame(theRect);

	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceIsToolbarVisible : Handle kEventNWebInterfaceIsToolbarVisible.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceIsToolbarVisible(NCarbonEvent &theEvent)
{	NHIWindow		theWindow(GetHIWindow());
	bool			isVisible;



	// Get the toolbar visibility
	isVisible = theWindow.IsToolbarVisible();
	theEvent.SetParameterBoolean(kEventNParamVisibile, isVisible);

	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceSetToolbarVisible : Handle kEventNWebInterfaceSetToolbarVisible.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceSetToolbarVisible(NCarbonEvent &theEvent)
{	NHIWindow		theWindow(GetHIWindow());
	bool			isVisible;



	// Set the toolbar visibility
	isVisible = theEvent.GetParameterBoolean(kEventNParamVisibile);
	theWindow.SetToolbarVisible(isVisible);

	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceIsWindowResizable : Handle kEventNWebInterfaceIsWindowResizable.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceIsWindowResizable(NCarbonEvent &theEvent)
{	NHIWindow		theWindow(GetHIWindow());
	bool			isResizable;



	// Get the window resizability
	isResizable = (theWindow.GetAttributes() & kWindowResizableAttribute);
	theEvent.SetParameterBoolean(kEventNParamResizable, isResizable);

	return(noErr);
}





//============================================================================
//		NHIWebView::DoEventNWebInterfaceSetWindowResizable : Handle kEventNWebInterfaceSetWindowResizable.
//----------------------------------------------------------------------------
OSStatus NHIWebView::DoEventNWebInterfaceSetWindowResizable(NCarbonEvent &theEvent)
{	NHIWindow		theWindow(GetHIWindow());
	bool			isResizable;



	// Set the window resizability
	isResizable = theEvent.GetParameterBoolean(kEventNParamResizable);
	
	if (isResizable)
		theWindow.SetAttributes(kWindowResizableAttribute);
	else
		theWindow.SetAttributes(kWindowNoAttributes, kWindowResizableAttribute);

	return(noErr);
}





//============================================================================
//		NHIWebView::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus NHIWebView::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;
	NString			theText;



	// Get the command status
	switch (hiCmd.commandID) {
		case kHICommandNBack:
			theStatus = EnableIf(CanGoBack());
			break;
		
		case kHICommandNForward:
			theStatus = EnableIf(CanGoForward());
			break;
		
		default:
			theStatus = NHIView::GetCommandStatus(hiCmd);
			break;
		}
	
	return(theStatus);
}



