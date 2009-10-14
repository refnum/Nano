/*	NAME:
		CWindowDocument.h

	DESCRIPTION:
		NanoWeb document window.
	
	COPYRIGHT:
		Copyright (c) 2006, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CWINDOWDOCUMENT_HDR
#define CWINDOWDOCUMENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHIWebView.h"
#include "NDocument.h"

#include "CToolbarDocument.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class CWindowDocument : public NDocument {
public:
										CWindowDocument(void);
	virtual								~CWindowDocument(void);


protected:
	// Initialize ourselves
	OSStatus							InitializeSelf(void);


	// Handle events
	OSStatus							DoEventNWebFrameLoadStarted( NCarbonEvent &theEvent);
	OSStatus							DoEventNWebFrameLoadFinished(NCarbonEvent &theEvent);
	OSStatus							DoEventTextAccepted(         NCarbonEvent &theEvent);


	// Get the status of a command
	CommandStatus						GetCommandStatus(const HICommandExtended &hiCmd);


	// Update the window appearance
	void								UpdateWindowTitle(void);
	void								UpdateWindowProxy(void);


	// Get the view to print
	NHIView								*GetPrintView(void);


private:
	NIB_VIEW('webv', NHIWebView,		WebView);
	CToolbarDocument					*mToolbar;
};





#endif // CWINDOWDOCUMENT_HDR


