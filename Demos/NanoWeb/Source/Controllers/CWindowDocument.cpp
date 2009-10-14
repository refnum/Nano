/*	NAME:
		CWindowDocument.cpp

	DESCRIPTION:
		NanoWeb document window.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NCFBundle.h"

#include "NanoWeb Build.h"
#include "CApplication.h"
#include "CWindowDocument.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NString kSchemeHTTP								= "http://";
static const char   *kAutoWebAddress							= "www.%s.com/";





//============================================================================
//		CWindowDocument::CWindowDocument : Constructor.
//----------------------------------------------------------------------------
CWindowDocument::CWindowDocument(void)
{
}





//============================================================================
//		CWindowDocument::~CWindowDocument : Destructor.
//----------------------------------------------------------------------------
CWindowDocument::~CWindowDocument(void)
{
}





//============================================================================
//		CWindowDocument::InitializeSelf : Initialize ourselves.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus CWindowDocument::InitializeSelf(void)
{	EventTypeSpec		addressEvents[] = {	{ kEventClassTextField, kEventTextAccepted			},
											{ kEventClassNone,      kEventKindNone				} };
	EventTypeSpec		webEvents[]  = {	{ kEventClassNWebView,  kEventNWebFrameLoadStarted	},
											{ kEventClassNWebView,  kEventNWebFrameLoadFinished	},
											{ kEventClassNone,      kEventKindNone				} };
	NEditText			*addressField;
	OSStatus			theErr;



	// Initialize the base class
	theErr = NDocument::InitializeSelf();
	if (theErr != noErr)
		return(theErr);



	// Install the toolbar
	mToolbar = new CToolbarDocument;
	mToolbar->Attach(GetWindow());
	mToolbar->SetDisplayMode(kHIToolbarDisplayModeIconOnly);

	addressField = mToolbar->GetAddress();



	// Register for events
	mWebView->AddEvents(webEvents);
	mWebView->SetEventDelegate(this);

	if (addressField != NULL)
		{
		addressField->AddEvents(addressEvents);
		addressField->SetEventDelegate(this);
		}



	// Configure the window
	UpdateToolbarStatus();

	mWebView->SetURL(NCFPreferences::GetPrefs()->GetValueString(kPrefHomePageKey));

	return(theErr);
}





//============================================================================
//		CWindowDocument::DoEventNWebFrameLoadStarted : Handle kEventNWebFrameLoadStarted.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::DoEventNWebFrameLoadStarted(NCarbonEvent &theEvent)
{	NEditText		*addressField;
	NChasingArrows	*theProgress;
	NCFURL			cfURL;



	// Get the state we need
	cfURL.Set(theEvent.GetParameterCFType(kEventNParamURL), false);



	// Update the toolbar
	UpdateToolbarStatus();

	if (theEvent.GetParameterBoolean(kEventNParamIsMainFrame))
		{
		addressField = mToolbar->GetAddress();
		theProgress  = mToolbar->GetProgress();
		
		if (addressField != NULL)
			addressField->SetTextValue(cfURL.GetString());

		if (theProgress != NULL)
			theProgress->SetAnimating(true);
		}

	return(eventNotHandledErr);
}





//============================================================================
//		CWindowDocument::DoEventNWebFrameLoadFinished : Handle kEventNWebFrameLoadFinished.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::DoEventNWebFrameLoadFinished(NCarbonEvent &theEvent)
{	NChasingArrows	*theProgress;



	// Update the toolbar
	UpdateToolbarStatus();

	if (theEvent.GetParameterBoolean(kEventNParamIsMainFrame))
		{
		theProgress = mToolbar->GetProgress();
		
		if (theProgress != NULL)
			theProgress->SetAnimating(false);
		}
	
	return(eventNotHandledErr);
}





//============================================================================
//		CWindowDocument::DoEventTextAccepted : Handle kEventTextAccepted.
//----------------------------------------------------------------------------
OSStatus CWindowDocument::DoEventTextAccepted(NCarbonEvent &/*theEvent*/)
{	NString		theURL;



	// Prepare the URL
	theURL = mToolbar->GetAddress()->GetTextValue();

	theURL.ReplaceAll(" ", "");
	theURL.TrimWhitespace();
	theURL.TrimString(kSchemeHTTP);
	
	if (!theURL.Contains("."))
		theURL.Format(kAutoWebAddress, theURL.GetUTF8());
	
	theURL = kSchemeHTTP + theURL;



	// Update the web view
	mWebView->SetURL(theURL);
	
	return(noErr);
}





//============================================================================
//		CWindowDocument::GetCommandStatus : Get the status of a command.
//----------------------------------------------------------------------------
CommandStatus CWindowDocument::GetCommandStatus(const HICommandExtended &hiCmd)
{	CommandStatus	theStatus;



	// Get the command status
	switch (hiCmd.commandID) {
		case kHICommandNBack:
			// NHIWebView will respond to back/forward commands automatically when
			// it has the focus (thus taking care of the menu items).
			//
			// However since we also use them for the toolbar, we (as the window
			// controller) also need to respond to them to update the toolbar.
			theStatus = EnableIf(mWebView->CanGoBack());
			break;

		case kHICommandNForward:
			theStatus = EnableIf(mWebView->CanGoForward());
			break;

		default:
			theStatus = NDocument::GetCommandStatus(hiCmd);
			break;
		}
	
	return(theStatus);
}





//============================================================================
//		CWindowDocument::UpdateWindowTitle : Update the window title.
//----------------------------------------------------------------------------
void CWindowDocument::UpdateWindowTitle(void)
{


	// Suppress the NDocument behaviour
	//
	// NHIWebView updates it's owning window's appearance automatically,
	// so we override NDocument's default implementation to prevent it
	// from clashing with the web view.
}





//============================================================================
//		CWindowDocument::UpdateWindowProxy : Update the window proxy.
//----------------------------------------------------------------------------
void CWindowDocument::UpdateWindowProxy(void)
{


	// Suppress the NDocument behaviour
	//
	// NHIWebView updates it's owning window's appearance automatically,
	// so we override NDocument's default implementation to prevent it
	// from clashing with the web view.
}





//============================================================================
//		CWindowDocument::GetPrintView : Get the view to print.
//----------------------------------------------------------------------------
NHIView *CWindowDocument::GetPrintView(void)
{


	// Get the view to print
	return(mWebView);
}








