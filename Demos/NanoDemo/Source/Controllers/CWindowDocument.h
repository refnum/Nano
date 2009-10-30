/*	NAME:
		CWindowDocument.h

	DESCRIPTION:
		NanoDemo document window.
	
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
#include "NDocument.h"

#include "CShapeView.h"
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


	// Handle events
	OSStatus							DoEventCommandProcess(NCarbonEvent &theEvent);


	// Handle preference changes
	void								DoPrefChanged(NCFPreferences *thePrefs, const NString &theKey);


	// Encode/decode the document state
	OSStatus							EncodeState(      NDictionary &theState) const;
	OSStatus							DecodeState(const NDictionary &theState);


	// Get the view to print
	NHIView								*GetPrintView(void);


private:
	OSStatus							CommandShape(UInt32 theCmd);
	void								SyncToolbar(void);
	
	void								SetShape(ShapeType     theShape);
	void								SetColor(const NColor &theColor);


private:
	NIB_VIEW('shap', CShapeView,		ShapeView);
	CToolbarDocument					*mToolbar;
};





#endif // CWINDOWDOCUMENT_HDR


