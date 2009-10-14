/*	NAME:
		NHIWebView.h

	DESCRIPTION:
		WebKit view.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NHIWEBVIEW_HDR
#define NHIWEBVIEW_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIView.h"
#include "NCFURL.h"
#include "NFile.h"





//============================================================================
//		Types
//----------------------------------------------------------------------------
#ifdef __OBJC__
	@class WebView;
	@class NHIWebViewDelegate;
#else
	typedef void *WebView;
	typedef void *NHIWebViewDelegate;
#endif





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NHIWebView : public NHIView {
public:
										DECLARE_HIVIEW_SUBCLASS(NHIWebView);

										NHIWebView(HIWindowRef				theWindow,
														const HIRect		&theRect,
														const NString		&theURL = "");
	virtual								~NHIWebView(void);


	// Get/set the current URL
	NCFURL								GetURL(void) const;
	void								SetURL(const NCFURL &theURL);


	// Go backwards/forwards
	bool								GoBack(   void) const;
	bool								GoForward(void) const;


	// Can we go back/forwards?
	bool								CanGoBack(   void) const;
	bool								CanGoForward(void) const;


	// Get the web view
	WebView								*GetWebView(void) const;


protected:
	// Initialize the view
	void								InitializeView(void);


	// Handle event changes
	void								DoEventsChanged(void);


	// Handle events
	virtual OSStatus					DoEventCommandProcess(					NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlBoundsChanged(			NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlDraw(						NCarbonEvent &theEvent);
	virtual OSStatus					DoEventControlOwningWindowChanged(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameSetTitle(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebFrameSetIcon(				NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSelectWindow(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceCloseWindow(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceGetWindowBounds(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetWindowBounds(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceGetViewFrame(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetViewFrame(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceIsToolbarVisible(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetToolbarVisible(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceIsWindowResizable(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebInterfaceSetWindowResizable(	NCarbonEvent &theEvent);
	virtual OSStatus					DoEventNWebPolicyNavigationAction(		NCarbonEvent &theEvent);
	virtual OSStatus					DoEventWindowBoundsChanged(				NCarbonEvent &theEvent);


	// Get the status of a command
	virtual CommandStatus				GetCommandStatus(const HICommandExtended &hiCmd);


private:
	void								AttachToWindow(HIWindowRef theWindow);

	void								SyncWebView(void);
	

private:
	NHIView								*mView;
	NCarbonEventHandler					mWindowHandler;

	WebView								*mWebView;
	NHIWebViewDelegate					*mDelegate;
};





#endif // NHIWEBVIEW_HDR


