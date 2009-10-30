/*	NAME:
		CWindowDocument.h

	DESCRIPTION:
		NanoText document window.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef CWINDOWDOCUMENT_HDR
#define CWINDOWDOCUMENT_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NHITextView.h"
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
	OSStatus							InitializeProperties(const NDictionary *appProperties);
	OSStatus							InitializeSelf(void);


	// Handle messages
	void								DoMessage(BroadcastMsg theMsg, void *msgData);


	// Handle preference changes
	void								DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);


	// Load/save the document
	OSStatus							LoadSelf(void);
	OSStatus							SaveSelf(void);


	// Get the view to print
	NHIView								*GetPrintView(void);


private:
	void								SyncModified(void);


private:
	NIB_VIEW('text', NHITextView,		TextView);
	CToolbarDocument					*mToolbar;
};





#endif // CWINDOWDOCUMENT_HDR


